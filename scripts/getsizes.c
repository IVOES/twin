/*
 *  getsizes.c  --  get sizeof() various data types and print it.
 *
 *  Copyright (C) 1999-2000 by Massimiliano Ghilardi
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>

#include "tw_autoconf.h"

#ifdef TW_HAVE_FCNTL_H
# include <fcntl.h>
#endif

#ifdef TW_HAVE_SYS_MMAN_H
# include <sys/mman.h>
#endif

#ifdef TW_HAVE_UNISTD_H
# include <unistd.h>
#endif

#include <Tw/datatypes.h>

#ifdef TW_HAVE_LONG_LONG
typedef unsigned long long ul;
# define UL "ull"
# define LX "%llX"
# define LX8 "%8llX"
# else
typedef unsigned long ul;
# define UL "ul"
# define LX "%lX"
# define LX8 "%8lX"
#endif

/* user can choose NOT to use 'long long' even if it is available... */
#if defined(CONF__LONG_LONG) && defined(HAVE_LONG_LONG)
# define ULLONG_NAME "unsigned long long"
# define ULLONG_SPEC "ull"
# define ULLONG_SIZE sizeof(ul)
# define ULLONG_MAX  MAXU(ul)
#else
# define ULLONG_NAME
# define ULLONG_SPEC
# define ULLONG_SIZE
# define ULLONG_MAX
#endif

int main(void) {
    char *str_byte16, *str_byte32;
    
    char *names[] = { "uldat",      "unsigned long",       "size_t",        ULLONG_NAME };
#if 0
    char *specs[] = { "",           "ul",                  "",              ULLONG_SPEC };
#endif
    int  sizes[]  = { sizeof(uldat), sizeof(unsigned long), sizeof(size_t), ULLONG_SIZE };
    ul   maxes[]  = { MAXU(uldat),   MAXU(unsigned long),   MAXU(size_t),   ULLONG_MAX  };
    
    int i, i_tany;
    
    if (sizeof(byte) != 1 || sizeof(udat) < 2 || sizeof(uldat) < 4 ||
	sizeof(hwcol) != 1 || sizeof(size_t) < 4) {
	
	fprintf(stderr, "Fatal: minimum requirements on data sizes not satisfied.\nSee scripts/getsizes.c\n");
	return 1;
    }
    if (sizeof(size_t) != sizeof(void *)) {
	fprintf(stderr, "Fatal: sizeof(size_t) != sizeof(void *)\nYour compiler and/or includes are seriously screwed up!\n");
	return 1;
    }
    if ((size_t)-1 < (size_t)0) {
	fprintf(stderr, "Fatal: (size_t)-1 < 0 : size_t must be unsigned!\nYour compiler and/or includes are seriously screwed up!\n");
	return 1;
    }
    
    if (sizeof(udat) == 2)
	str_byte16 = "udat";
    else if (sizeof(unsigned short) == 2)
	str_byte16 = "unsigned short";
    else {
	fprintf(stderr, "Fatal: could not find a 16-bit type to use as byte16.\nSee scripts/getsizes.c\n");
	return 1;
    }

    if (sizeof(udat) == 4)
	str_byte32 = "udat";
    else if (sizeof(uldat) == 4)
	str_byte32 = "uldat";
    else if (sizeof(unsigned int) == 4)
	str_byte32 = "unsigned int";
    else if (sizeof(unsigned long) == 4)
	str_byte32 = "unsigned long";
    else {
	fprintf(stderr, "Fatal: could not find a 32-bit type to use as byte32.\nSee scripts/getsizes.c\n");
	return 1;
    }

    for (i_tany = 0, i = 1; i < sizeof(sizes)/sizeof(sizes[0]); i++) {
	if (sizes[i_tany] < sizes[i])
	    i_tany = i;
    }

    
    printf("\n"
	   "/*\n"
	   " * This file was automatically generated by 'scripts/Getsizes'. Do no edit!\n"
	   " */\n"
	   "\n"
	   "#ifndef _TW_DATASIZES_H\n"
	   "#define _TW_DATASIZES_H\n"
	   "\n"
	   "#ifndef __asm\n"
	   "\n"
	   "/* an unsigned integer type as wide as (void *) */\n"
	   "typedef size_t	topaque;\n"
	   "\n"
	   "/*\n"
	   " * the widest libTw type. At least as wide as topaque, uldat, unsigned long and,\n"
	   " * if supported by the compiler and enabled, unsigned long long.\n"
	   " */\n"
	   "typedef %s	tany;\n"
	   "\n"
	   "#endif /* __asm */\n"
	   "\n"
	   "#define TW_SIZEOFBYTE         %d\n"
	   "#define TW_SIZEOFUDAT         %d\n"
	   "#define TW_SIZEOFULDAT        %d\n"
	   "#define TW_SIZEOFTOPAQUE      %d\n"
	   "#define TW_SIZEOFTANY         %d\n"
	   "\n"
	   "#define TW_SIZEOFINT          %d\n"
	   "\n"
	   "#define TW_MAXSBYTE    0x" LX "\n"
	   "#define TW_MAXBYTE     0x" LX "\n"
	   "#define TW_MAXDAT      0x" LX "\n"
	   "#define TW_MAXUDAT     0x" LX "\n"
	   "#define TW_MAXLDAT     0x" LX "\n"
	   "#define TW_MAXULDAT    0x" LX "\n"
	   "#define TW_MAXTOPAQUE  0x" LX "\n"
	   "\n"
	   "/*\n"
	   " * WARNING: if 'long long' support exists and is enabled,\n"
	   " * TW_MAXTANY may be so large that C preprocessor cannot handle it\n"
	   " */\n"
	   "#define TW_MAXTANY     0x" LX /*"%s"*/ "\n"
	   "\n"
	   "#define TW_MINSBYTE    0x" LX "\n"
	   "#define TW_MINDAT      0x" LX "\n"
	   "#define TW_MINLDAT     0x" LX "\n"
	   "\n"
	   "#define TW_BYTE16      %s\n"
	   "#define TW_BYTE32      %s\n"
	   "\n"
	   "#endif /* _TW_DATASIZES_H */\n",

	   names[i_tany],
	   
	   (int)sizeof(byte), (int)sizeof(udat), (int)sizeof(uldat),
	   (int)sizeof(size_t), sizes[i_tany], (int)sizeof(int),
	   

	   (ul)MAXSBYTE, (ul)MAXBYTE,
	   (ul)MAXDAT, (ul)MAXUDAT,
	   (ul)MAXLDAT, (ul)MAXULDAT,
	   (ul)MAXU(size_t), (ul)maxes[i_tany], /* specs[i_tany], */
	   
	   /*
	    * note about MINSBYTE, MINDAT, MINLDAT:
	    * if the type is unsigned, -(ul)0 == 0;
	    * if the type is signed, extending to (ul) will fill higher bytes with 0xFF.
	    * For example doing (ul)0x80000000 when (ul) is 8 bytes will give
	    * 0xFFFFFFFF80000000, and -(ul)0xFFFFFFFF80000000 will give back the correct 0x80000000.
	    */
	   -(ul)MINSBYTE, -(ul)MINDAT, -(ul)MINLDAT,
	   
	   str_byte16,
	   str_byte32
	   );
    return 0;
}

