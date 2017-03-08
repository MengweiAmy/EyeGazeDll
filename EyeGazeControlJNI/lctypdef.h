/*****************************************************************************

File Name:       LCTYPDEF.H
                 Commonly used data type abbreviations

Company:         LC Technologies, Inc.
                 10363 Democracy Lane
                 Fairfax, VA 22030
                 (703) 385-8800

*****************************************************************************/
#ifndef LCTYPDEF_H_INCLUDED
#define LCTYPDEF_H_INCLUDED
/****************************************************************************/

#if defined WIN32
   #include <windows.h>
#endif

/****************************************************************************/
/* Variable Type Definitions:                                               */

//#define  VOID    void
#define  CHAR    wchar_t
#define  INT     int
#define  BOOL    int
#define  LONG    long
#define  FLOAT   float
#define  DOUBLE  double
#define  BYTE    unsigned char
#define  UINT    unsigned int
#define  USHORT  unsigned short  // defined in windef.h  DixonC
#define  ULONG   unsigned long   // defined in windef.h  DixonC
#define  STRUCT  struct

#define  SVOID   static void
#define  SCHAR   static wchar_t
#define  SINT    static int
#define  SBOOL   static int
#define  SFLOAT  static float
#define  SBYTE   static unsigned char
#define  SUINT   static unsigned int
#define  SSTRUCT static struct

/****************************************************************************/
#endif   // LCTYPDEF_H_INCLUDED
/****************************************************************************/
