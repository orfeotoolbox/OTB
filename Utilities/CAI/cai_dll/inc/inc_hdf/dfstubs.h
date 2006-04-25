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

/*
   ** FILE
   **   dfstubs.h
   ** PURPOSE
   **   Header file for "dfstubs.c" HDF 3.1 emulation using new routines
   **   from "hfile.c".
   ** AUTHOR
   **   Doug Ilg
 */

#ifndef DFSTUBS_H   /* avoid re-inclusion */
#define DFSTUBS_H
/* This is the master HDF driver (taking the place of df.c), so... */
#define DFMASTER
#undef PERM_OUT     /* used to "comment out" code */

#include "df.h"
#undef DFMASTER

#if !defined(__GNUC__) & !defined(CONVEX) & !defined(VMS)
#include <memory.h>
#endif /* !__GNUC__ & !CONVEX & !VMS */

#define DFACC_APPEND    8
#define DFEL_ABSENT 0
#define DFEL_RESIDENT   1
#define DFSRCH_OLD  0
#define DFSRCH_NEW  1

PRIVATE int32 DFid = 0;
PRIVATE int32 DFaid = 0;
PRIVATE int DFaccmode = 0;
PRIVATE int DFelaccmode = 0;
PRIVATE uint16 search_tag = 0;
PRIVATE uint16 search_ref = 0;
PRIVATE int search_stat = DFSRCH_NEW;
PRIVATE int32 search_aid = 0;
PRIVATE int DFelstat = DFEL_ABSENT;
PRIVATE int32 DFelsize = 0;
PRIVATE int32 DFelseekpos = 0;
PRIVATE uint16 acc_tag = 0;
PRIVATE uint16 acc_ref = 0;
PRIVATE char *DFelement = NULL;

#if defined c_plusplus || defined __cplusplus
extern      "C"
{
#endif                          /* c_plusplus || __cplusplus */

/* prototypes for internal routines */
    PRIVATE int DFIclearacc
                (void);

    PRIVATE int DFIcheck
                (DF * dfile);

#if defined c_plusplus || defined __cplusplus
}
#endif                          /* c_plusplus || __cplusplus */

#endif                          /* DFSTUBS_H */
