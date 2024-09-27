#ifndef DWORD
#define DWORD unsigned long
#endif

#ifndef WORD
#define WORD unsigned short
#endif

#ifndef BYTE
#define BYTE unsigned char
#endif

#ifndef HANDLE
#define HANDLE WORD
#endif

#ifndef HGLOBAL
#define HGLOBAL HANDLE
#endif

#define FAR far
#define WINAPI FAR pascal
#define HFILE WORD
#define UINT unsigned int
#define LONG long
#define LPVOID void FAR *
#define LPCSTR const char FAR *
#define LPSTR char FAR *

#define GMEM_FIXED          0x0000
#define GMEM_ZEROINIT       0x0040

#define OF_READ 0x0000
#define SEEK_SET 0
#define HFILE_ERROR -1
#pragma pack( push, 1 )

typedef struct
{
    WORD first; /* ordinal */
    WORD last;  /* ordinal */
    WORD next;  /* bundle */
} ET_BUNDLE;

typedef struct tagOFSTRUCT {
    BYTE    cBytes;
    BYTE    fFixedDisk;
    unsigned int	nErrCode;
    BYTE    reserved[4];
    char    szPathName[128];
} OFSTRUCT;

#pragma pack( pop )

#define Dos3Call __asm { int 21h }
HGLOBAL WINAPI GlobalAlloc(WORD flags, DWORD size);
char FAR *  WINAPI GlobalLock(HGLOBAL h);
HFILE WINAPI _lopen(LPCSTR lpPathName, int iReadWrite);
UINT WINAPI _lread(HFILE  hFile, LPVOID lpBuffer, UINT uBytes);
HFILE WINAPI _lclose(HFILE hFile);
LONG WINAPI _llseek( HFILE hFile, LONG lOffset, int nOrigin );
LPSTR WINAPI lstrcpy( LPSTR s, LPCSTR t );
int WINAPI lstrlen( LPCSTR s );
//void WINAPI         hmemcpy( void __huge *, const void __huge *, long );
