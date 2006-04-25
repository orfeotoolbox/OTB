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
 * File:    df.h
 * Purpose: header file for HDF routines
 * Invokes: dfi.h
 * Contents:
 *  Structure definitions: DFddh, DFdd, DFdesc, DFdle, DF, DFdi, DFdata
 *  Procedure type definitions
 *  Global variables
 *  Tag definitions
 *  Error return codes
 *  Logical constants
 * Remarks: This file is included with user programs
 *          Since it includes stdio.h etc., do not include these after df.h
 *---------------------------------------------------------------------------*/

#ifndef DF_H    /* avoid re-inclusion */
#define DF_H

/* include DF (internal) header information */
#include "hdf.h"

/*-------------------------------------------------------------------------*/
/*                      Type declarations                                   */

typedef struct DFddh
  {                             /*format of data descriptor headers in file */
      int16       dds;          /* number of dds in header block */
      int32       next;         /* offset of next header block */
  }
DFddh;

typedef struct DFdd
  {                             /* format of data descriptors as in file */
      uint16      tag;          /* data tag */
      uint16      ref;          /* data reference number */
      int32       offset;       /* offset of data element in file */
      int32       length;       /* number of bytes */
  }
DFdd;

/* descriptor structure is same as dd structure.  ###Note: may be changed */
typedef DFdd DFdesc;

/* DLE is the internal structure which stores data descriptor information */
/* It is a linked list of DDs */
typedef struct DFdle
  {                             /* Data List element */
      struct DFdle *next;       /* link to next dle */
      DFddh       ddh;          /* To store headers */
      DFdd        dd[1];        /* dummy size */
  }
DFdle;

/* DF is the internal structure associated with each DF file */
/* It holds information associated with the file as a whole */
/* ### Note: there are hooks for having multiple DF files open at a time */
typedef struct DF
  {
      DFdle      *list;         /* Pointer to the DLE list */
      DFdle      *last_dle;     /* last_dle and last_dd are used in searches */
      /* to indicate element returned */
      /* by previous call to DFfind */
      DFdd       *up_dd;        /* DD of element being read/updated, */
      /* used by DFstart */
      uint16      last_tag;     /* Last tag searched for by DFfind */
      uint16      last_ref;     /* Last reference number searched for */
      intn        type;         /* 0= not in use, 1= normal, -1 = multiple */
      /* this is a hook for when */
      /* multiple files are open */
      intn        access;       /* permitted access types: */
      /* 0=none, 1=r, 2=w, 3=r/w */
      intn        changed;      /* True if anything in DDs modified */
      /* since last write */
      intn        last_dd;      /* see last_dle */
      intn        defdds;       /* default numer of DD's in each block */
      intn        up_access;    /* access permissions to element being */
      /* read/updated. Used by DFstart */
      /* File handle is a file pointer or file descriptor depending on whether */
      /* we use buffered or unbuffered I/O.  But, since this structure is a */
      /* fake, it doesn't matter whether I/O is buffered or not. */
      intn        file;         /* file descriptor */
  }
DF;

typedef struct DFdata
  {                             /* structure for returning status information */
      int32       version;      /* version number of program */
  }
DFdata;

/*--------------------------------------------------------------------------*/
/*                          Procedure types                                 */

#if defined c_plusplus || defined __cplusplus
extern      "C"
{
#endif                          /* c_plusplus || __cplusplus */

/* prototypes for dfstubs.c */
    extern DF  *DFopen
                (char *name, int acc_mode, int ndds);

    extern int  DFclose
                (DF * dfile);

    extern int  DFdescriptors
                (DF * dfile, DFdesc ptr[], int begin, int num);

    extern int  DFnumber
                (DF * dfile, uint16 tag);

    extern int  DFsetfind
                (DF * dfile, uint16 tag, uint16 ref);

    extern int  DFfind
                (DF * dfile, DFdesc * ptr);

    extern int  DFaccess
                (DF * dfile, uint16 tag, uint16 ref, char *acc_mode);

    extern int  DFstart
                (DF * dfile, uint16 tag, uint16 ref, char *acc_mode);

    extern int32 DFread
                (DF * dfile, char *ptr, int32 len);

    extern int32 DFseek
                (DF * dfile, int32 offset);

    extern int32 DFwrite
                (DF * dfile, char *ptr, int32 len);

    extern int  DFupdate
                (DF * dfile);

    extern int  DFstat
                (DF * dfile, DFdata * dfinfo);

    extern int32 DFgetelement
                (DF * dfile, uint16 tag, uint16 ref, char *ptr);

    extern int32 DFputelement
                (DF * dfile, uint16 tag, uint16 ref, char *ptr, int32 len);

    extern int  DFdup
                (DF * dfile, uint16 itag, uint16 iref, uint16 otag, uint16 oref);

    extern int  DFdel
                (DF * dfile, uint16 tag, uint16 ref);

    extern uint16 DFnewref
                (DF * dfile);

    extern int  DFishdf
                (char *filename);

    extern int  DFerrno
                (void);

    extern int  DFIerr
                (DF * dfile);

    extern int  DFImemcopy
                (char *from, char *to, int length);

    extern void *DFIgetspace
                (uint32 qty);

    extern void *DFIfreespace
                (void *ptr);

    extern int  DFIc2fstr
                (char *str, int len);

    extern char *DFIf2cstring
                (_fcd fdesc, intn len);

/* prototypes for dfconv.c */
    extern int  DFconvert
                (uint8 *source, uint8 *dest, int ntype, int sourcetype, int desttype, int32 size);

#if defined c_plusplus || defined __cplusplus
}
#endif                          /* c_plusplus || __cplusplus */

/*--------------------------------------------------------------------------*/
/*                          Global Variables                                */

#ifndef DFMASTER
extern
#endif                          /*DFMASTER */
int DFerror;            /* Error code for DF routines */

#define DFSETERR(error) (DFerror=(DFerror?DFerror:error))

#define DFTOFID(df) (int32)(df->list)

#endif /* DF_H */
