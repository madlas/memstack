
#ifndef _BASE_TYPE_H
#define _BASE_TYPE_H

typedef int INT;
typedef short INT16;
typedef int INT32;

typedef unsigned int UINT;
typedef unsigned short UINT16;
typedef unsigned int UINT32;
typedef int BOOL;
typedef char bool;
typedef unsigned char BYTE;
typedef unsigned char * PBYTE;
typedef unsigned int  DWORD;
typedef unsigned short WORD;

//#ifndef PCHAR
typedef char * PCHAR;
typedef char  CHAR;
//#endif
typedef unsigned long ULONG;
typedef long LONG;

#define IN

#define OUT
#define true 1
#define false 0
#define TRUE					1
#define FALSE					0

#define SIZEOF(a)				(sizeof(a) / 1)

#ifndef min
#define min(a, b)				((a) < (b)) ? (a) : (b)
#endif

#ifndef max
#define max(a, b)				((a) < (b)) ? (b) : (a)
#endif

#define ASSERT(x) //{if (!x){printf("FILE=%s, LINE=%d error\n"__FILE__,__LINE__);exit(1);}}
#define TRACE(x)// printf x;

#ifndef O_BINARY
#define O_BINARY (0)
#endif

#define FILE_NAME_LEN			(64)

#endif
