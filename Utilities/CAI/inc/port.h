/*
 * Warning, this file was automatically created by the TIFF configure script
 * VERSION:	 v3.5.5
 * DATE:	 Fri Oct 6 16:24:03 MET DST 2000
 * TARGET:	 sparc-sun-solaris2.7
 * CCOMPILER:	 /usr/local/bin/gcc-2.95.2 19991024 (release)
 */
#ifndef _PORT_
#define _PORT_ 1
#ifdef __cplusplus
extern "C" {
#endif
#include <sys/types.h>
#define HOST_FILLORDER FILLORDER_MSB2LSB
#define HOST_BIGENDIAN	1
#define HAVE_MMAP 1
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
typedef double dblparam_t;
#ifdef __STRICT_ANSI__
#define	INLINE	__inline__
#else
#define	INLINE	inline
#endif
#define GLOBALDATA(TYPE,NAME)	extern TYPE NAME
#ifdef __cplusplus
}
#endif
#endif
