
/****************************************************************************
 * NCSA HDF                                                                 *
 * Software Development Group                                               *
 * National Center for Supercomputing Applications                          *
 * University of Illinois at Urbana-Champaign                               *
 * 605 E. Springfield, Champaign IL 61820                                   *
 *                                                                          *
 * For conditions of distribution and use, see the accompanying             *
 * hdf/COPYING file.                                                        *
 *                                                                          *
 ****************************************************************************/

/* $Id$ */

/*-----------------------------------------------------------------------------
 * File:    dfi.h
 * Purpose: HDF internal header file
 * Invokes: stdio.h, sys/file.h
 * Contents:
 *  Compilation parameters
 *  Machine-dependent definitions
 *  Flexibility definitions: i/o buffering, dynamic memory, structure i/o
 *  Size parameters
 * Remarks: To port to a new system, only dfi.h and Makefile need be modified.
 *          This file is included with user programs, but users do not see it.
 *---------------------------------------------------------------------------*/

#ifndef DFI_H
#define DFI_H

/*--------------------------------------------------------------------------*/
/*          Compilation Parameters for Flexibility and Portability          */

/* modify this line for buffered/unbuffered i/o */
#define DF_BUFFIO

/* modify this line for dynamic/static memory allocation */
#define DF_DYNAMIC

/* modify this line if structures cannot be read/written as is */
#undef  DF_STRUCTOK     /* leave it this way - hdfsh expects it */

/*--------------------------------------------------------------------------*/
/*                      Machine dependencies                                */
/*--------------------------------------------------------------------------*/

#ifdef IRIX
#undef DF_STRUCTOK
#include <sys/types.h>
#include <sys/file.h>   /* for unbuffered i/o stuff */
#ifndef DFmovmem
#define DFmovmem(from, to, len) bcopy(from, to, len)
#endif /* DFmovmem */
#ifndef DF_STRUCTOK
#define UINT16READ(p, x)    { x = ((*p++) & 255)<<8; x |= (*p++) & 255; }
#define INT16READ(p, x)     { x = (*p++)<<8; x |= (*p++) & 255; }
#define INT32READ(p, x)     { x = (*p++)<<24; x|=((*p++) & 255)<<16;    \
                                x|=((*p++) & 255)<<8; x|=(*p++) & 255; }
#define UINT16WRITE(p, x)   { *p++ = (x>>8) & 255; *p++ = x & 255; }
#define INT16WRITE(p, x)    { *p++ = (x>>8) & 255; *p++ = x & 255; }
#define INT32WRITE(p, x)    { *p++ = (x>>24) & 255; *p++ = (x>>16) & 255;   \
                                *p++ = (x>>8) & 255; *p++ = x & 255; }
#endif /*DF_STRUCTOK */
#define DF_CREAT(name, prot) creat(name, prot)
#ifndef DF_MT
#define DF_MT   DFMT_IRIX
#endif /* DF_MT  */
#endif /*IRIX */

#ifdef IBM6000  /* NOTE: IBM6000 defines are same as for SUN */
#if ! defined mc68010 && ! defined mc68020 && ! defined mc68030
#undef DF_STRUCTOK
#endif
#include <sys/file.h>   /* for unbuffered i/o stuff */
#define DFmovmem(from, to, len) memcpy(to, from, len)
#ifndef DF_STRUCTOK
#define UINT16READ(p, x) { x = ((*p++) & 255)<<8; x |= (*p++) & 255; }
#define INT16READ(p, x) { x = (*p++)<<8; x |= (*p++) & 255; }
#define INT32READ(p, x) { x = (*p++)<<24; x|=((*p++) & 255)<<16;    \
            x|=((*p++) & 255)<<8; x|=(*p++) & 255; }
#define UINT16WRITE(p, x) { *p++ = (x>>8) & 255; *p++ = x & 255; }
#define INT16WRITE(p, x) { *p++ = (x>>8) & 255; *p++ = x & 255; }
#define INT32WRITE(p, x) { *p++ = (x>>24) & 255; *p++ = (x>>16) & 255;  \
            *p++ = (x>>8) & 255; *p++ = x & 255; }
#endif /*DF_STRUCTOK */
#define DF_CREAT(name, prot) creat(name, prot)
#define DF_MT   DFMT_IBM6000
#endif /*IBM6000 */

#if defined (MAC) || defined (macintosh) || defined (SYMANTEC_C)
#undef DF_BUFFIO    /* use unbuffered i/o */
#include <memory.h>     /* malloc stuff for MPW 3.0 */
#include <fcntl.h>  /* unbuffered IO stuff for MPW 3.0 */
#ifdef SYMANTEC_C  /* for LightSpeed C */
#include <unix.h>
#else  /*SYMANTEC_C                   MPW, possibly others */
#include <Files.h>  /* for unbuffered i/o stuff */
#endif /*SYMANTEC_C */
#define DF_CAPFNAMES    /* fortran names are in all caps */
#define DF_DYNAMIC  /* use dynamic allocation */
#ifdef SYMANTEC_C  /* LightSpeed C does not have memcpy */
#define DFmovmem(from, to, len) DFImemcopy(from, to, len)
#else  /*SYMANTEC_C */
#define DFmovmem(from, to, len) memcpy(to, from, len)
#endif /*SYMANTEC_C */
#define malloc(x)   NewPtr((Size)   (x))    /* don't use malloc on the Mac */
#define free(x)     DisposPtr((Ptr) (x))    /* don't use free on the Nac   */
#undef DF_STRUCTOK
#define UINT16READ(p, x) { x = ((*p++) & 255)<<8; x |= (*p++) & 255; }
#define INT16READ(p, x) { x = (*p++)<<8; x |= (*p++) & 255; }
#define INT32READ(p, x) { x = (*p++)<<24; x|=((*p++) & 255)<<16;    \
            x|=((*p++) & 255)<<8; x|=(*p++) & 255; }
#define UINT16WRITE(p, x) { *p++ = (x>>8) & 255; *p++ = x & 255; }
#define INT16WRITE(p, x) { *p++ = (x>>8) & 255; *p++ = x & 255; }
#define INT32WRITE(p, x) { *p++ = (x>>24) & 255; *p++ = (x>>16) & 255;  \
            *p++ = (x>>8) & 255; *p++ = x & 255; }
#define DF_CREAT(name, prot) mopen(name, O_WRONLY|O_TRUNC|O_CREAT)
#define DF_MT   DFMT_MAC
#endif /*MAC */

#ifdef VMS
/*#undef DF_BUFFIO should be buff !!!! */
   /* use only unbuff i/o - buff doesn't work! */
#ifndef DFopen  /* avoid double includes */
/* #include "dfivms.h" */
#endif /*DFopen */
#undef DF_STRUCTOK
#define DF_CAPFNAMES    /* fortran names are in all caps */
#include <file.h>   /* for unbuffered i/o stuff */
#define DFmovmem(from, to, len) memcpy(to, from, len)
#ifndef DF_STRUCTOK
#define UINT16READ(p, x) { x = ((*p++) & 255)<<8; x |= (*p++) & 255; }
#define INT16READ(p, x) { x = (*p++)<<8; x |= (*p++) & 255; }
#define INT32READ(p, x) { x = (*p++)<<24; x|=((*p++) & 255)<<16;    \
            x|=((*p++) & 255)<<8; x|=(*p++) & 255; }
#define UINT16WRITE(p, x) { *p++ = (x>>8) & 255; *p++ = x & 255; }
#define INT16WRITE(p, x) { *p++ = (x>>8) & 255; *p++ = x & 255; }
#define INT32WRITE(p, x) { *p++ = (x>>24) & 255; *p++ = (x>>16) & 255;  \
            *p++ = (x>>8) & 255; *p++ = x & 255; }
#endif /*DF_STRUCTOK */
#define DF_CREAT(name, prot) creat(name, prot)
#define DF_MT   DFMT_VAX
#endif /*VMS */

#ifdef APOLLO
#if ! defined mc68010 && ! defined mc68020 && ! defined mc68030
#undef DF_STRUCTOK
#endif
#include <sys/file.h>   /* for unbuffered i/o stuff */
#define int8 char
#define uint8 unsigned char
#define int16 short int
#define uint16 unsigned short int
#define int32 long int
#define uint32 unsigned long int
#define float32 float
#define DFmovmem(from, to, len) memcpy(to, from, len)
#ifndef DF_STRUCTOK
#define UINT16READ(p, x) { x = ((*p++) & 255)<<8; x |= (*p++) & 255; }
#define INT16READ(p, x) { x = (*p++)<<8; x |= (*p++) & 255; }
#define INT32READ(p, x) { x = (*p++)<<24; x|=((*p++) & 255)<<16;    \
            x|=((*p++) & 255)<<8; x|=(*p++) & 255; }
#define UINT16WRITE(p, x) { *p++ = (x>>8) & 255; *p++ = x & 255; }
#define INT16WRITE(p, x) { *p++ = (x>>8) & 255; *p++ = x & 255; }
#define INT32WRITE(p, x) { *p++ = (x>>24) & 255; *p++ = (x>>16) & 255;  \
            *p++ = (x>>8) & 255; *p++ = x & 255; }
#endif /*DF_STRUCTOK */
#define DF_CREAT(name, prot) creat(name, prot)
#define DF_MT   DFMT_APOLLO
#endif /*APOLLO */

/*--------------------------------------------------------------------------*/
/*                      Flexibility parameters                              */
#if defined (MAC) || defined (macintosh) || defined (SYMANTEC_C)  /* MAC specific file manager calls */
#   define DF_OPEN(x,y) mopen(x,y)
#   define DF_CLOSE(x) mclose(x)
#   define DF_SEEK(x,y,z) mlseek(x,y,z)
#   define DF_SKEND(x,y,z) mlseek(x,-1*y,z)
#   define DF_TELL(x) mlseek(x,0L,1)
#   define DF_READ(a,b,c,d) mread(d,a,b*c)
#   define DF_WRITE(a,b,c,d) mwrite(d,a,b*c)
#   define DF_FLUSH(a)  /* no need to flush */
#   define DF_RDACCESS 0    /* dummy */
#   define DF_WRACCESS 0    /* dummy */
#   define DF_OPENERR(f)    ((f) == -1)
#else  /* !MAC */
#ifdef DF_BUFFIO    /* set all calls to do buffered I/O */
#define DF_OPEN(x,y) fopen(x,y)
#define DF_CLOSE(x) fclose(x)
#define DF_SEEK(x,y,z) fseek(x,y,z)
#define DF_SKEND(x,y,z) fseek(x,y,z)
#define DF_TELL(x) ftell(x)
#define DF_READ(a,b,c,d) fread(a,b,c,d)
#define DF_WRITE(a,b,c,d) fwrite(a,b,c,d)
#define DF_FLUSH(a) fflush(a)
#define DF_OPENERR(f)   (!(f))
#define DF_RDACCESS "rb"
#define DF_WRACCESS "rb+"

#else  /*DF_BUFFIO         unbuffered i/o */
#define DF_OPEN(x,y) open(x,y)
#define DF_CLOSE(x) close(x)
#define DF_SEEK(x,y,z) lseek(x,y,z)
#define DF_SKEND(x,y,z) lseek(x,-1*y,z)
#define DF_TELL(x) lseek(x,0L,1)
#define DF_READ(a,b,c,d) read(d,a,b*c)
#define DF_WRITE(a,b,c,d) write(d,a,b*c)
#define DF_OPENERR(f)   ((f) == -1)
#define DF_FLUSH(a)     /* no need to flush */
#define DF_RDACCESS O_RDONLY
#define DF_WRACCESS O_RDWR
#endif /* DF_BUFFIO */
#endif /* !MAC */

#ifndef FILE
#include <stdio.h>
#endif /*FILE */

#endif /* DFI_H */
