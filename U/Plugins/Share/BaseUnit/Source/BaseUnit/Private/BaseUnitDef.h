#pragma once

#ifndef MAXPATH
#define MAXPATH   260
#define MAXDIR    256
#define MAXFILE   256
#define MAXEXT    256
#define MAXDRIVE    3
#endif

#define MEMSIGN 1234567890

#define SEEK_ERROR		0xFFFFFFFF
#define GMAKEWORD(a, b)      ((unsigned short)(((unsigned char)(a)) | ((unsigned short)((unsigned char)(b))) << 8))
#define GMAKELONG(a, b)      ((long)(((unsigned char)(a)) | ((unsigned long)((unsigned char)(b))) << 16))
#define GLOWORD(l)           ((unsigned char)(l))
#define GHIWORD(l)           ((unsigned char)(((unsigned long)(l) >> 16) & 0xFFFF))
#define GLOBYTE(w)           ((unsigned char)(w))
#define GHIBYTE(w)           ((unsigned char)(((unsigned char)(w) >> 8) & 0xFF))

#define G_B						( 1 )
#define G_KB					( 1024 )
#define G_MB					( 1024 * 1024 )

#define RB_DEFHL	( 4	)
#define RB_SPACE	( 8 )
#define RB_DEFIBL	( G_KB * 16 + RB_SPACE )
#define RB_MAXPACK	( G_KB * 4  )

#ifndef INVALID_VALUE
#define INVALID_VALUE	( -1 )
#define SUCCESS_VALUE	( 0 )
#endif