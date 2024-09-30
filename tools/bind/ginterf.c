// Emulation of standard windows function via DOS ones
// for sharing of this code with WIN16 KERNEL.EXE
// List of emulated functions
//   GlobalAlloc
//   GlobalLock
//   GlobalUnlock

#include <malloc.h>
#include <string.h>
#include <dos.h>
#include <stdio.h>

#include "winemu.h"

void * handles[255];
BYTE handlescount = 0;

// Emulation of GlobalAlloc. Actually returns segment allocated by int 21h
HGLOBAL WINAPI GlobalAlloc(WORD flags, DWORD size)
{
	HGLOBAL segm;
	
	segm=malloc(size);

	// Zero data
	if (flags & GMEM_ZEROINIT) memset(segm, 0, size);

	return segm;
}

// Produce far pointer from HGLOBAL
char FAR *  WINAPI GlobalLock(HGLOBAL h)
{
	return h;
}

// Actually, does nothing
BYTE WINAPI GlobalUnlock(HGLOBAL h)
{
	return 1;
}
