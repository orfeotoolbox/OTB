
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
 * File:    dfsd.h
 * Purpose: header file for the Scientific Data set
 * Invokes: dfrig.h
 * Contents:
 *  Structure definitions: DFSsdg
 *  Constant definitions: DFS_MAXLEN
 * Remarks: This is included with user programs which use SDG
 *          Currently defined to be 2-D.  Will later be increased to
 *          multiple dimensions
 *---------------------------------------------------------------------------*/

#ifndef _DFSD_H   /* avoid re-inclusion */
#define _DFSD_H

#include "hdf.h"

/* include numbertype and aid for 3.2   S. Xu   */
/* structure to hold SDG info */
typedef struct DFSsdg
  {
      DFdi        data;         /* tag/ref of data in file */
      intn        rank;         /* number of dimensions */
      int32 *dimsizes;    /* dimensions of data */
      char *coordsys;
      char *dataluf[3];   /* label/unit/format of data */
      char **dimluf[3];   /* label/unit/format for each dim */
      uint8 **dimscales;  /* scales for each dimension */
      uint8       max_min[16];  /* max, min values of data, */
      /*  currently atmost 8 bytes each   */
      int32       numbertype;   /* default is float32      */
      uint8       filenumsubclass;  /* number format in the file, default is IEEE */
      int32       aid;          /* access id     */
      int32       compression;  /* 0 -- not compressed  */
      int32       isndg;        /* 0 -- pure sdg, written by 3.1 else ndg */
      float64     cal, cal_err; /* calibration multiplier stuff          */
      float64     ioff, ioff_err;   /* calibration offset stuff              */
      int32       cal_type;     /* number type of data after calibration */
      uint8       fill_value[DFSD_MAXFILL_LEN];     /* fill value if any specified  */
      intn        fill_fixed;   /* whether ther fill value is a fixed value, or it can change */
  }
DFSsdg;

/* DFnsdgle is the internal structure which stores SDG or NDS and   */
/* related SDG in an HDF file.                                      */
/* It is a linked list.                                             */

typedef struct DFnsdgle
  {
      DFdi        nsdg;         /* NDG from 3.2 or SDG from 3.1  */
      DFdi        sdg;          /* Only special NDF has values in this field */
      struct DFnsdgle *next;
  }
DFnsdgle;

typedef struct DFnsdg_t_hdr
  {
      uint32      size;
      DFnsdgle   *nsdg_t;
  }
DFnsdg_t_hdr;

#if defined c_plusplus || defined __cplusplus
extern      "C"
{
#endif                          /* c_plusplus || __cplusplus */

    extern int32 DFSDIopen
                (const char * filename, int acc_mode);

    extern int  DFSDIsdginfo
                (int32 file_id);

    extern int  DFSDIclear
                (DFSsdg * sdg);

    extern int  DFSDIclearNT
                (DFSsdg * sdg);

    extern int  DFSDIgetdata
                (const char * filename, intn rank, int32 maxsizes[], VOIDP data,
                 int isfortran);

    extern int  DFSDIputdata
                (const char * filename, intn rank, int32 * dimsizes, VOIDP data,
                 int accmode, int isfortran);

    extern int  DFSDIgetslice
                (const char * filename, int32 winst[], int32 windims[], VOIDP data,
                 int32 dims[], int isfortran);

    extern int  DFSDIputslice
                (int32 windims[], VOIDP data, int32 dims[], int isfortran);

    extern int  DFSDIendslice
                (int isfortran);

    extern intn DFSDIrefresh
                (char * filename);

    extern int  DFSDIisndg
                (intn * isndg);

    extern int  DFSDIgetrrank
                (intn * rank);

    extern int  DFSDIgetwrank
                (intn * rank);

    extern int  DFSDIsetdimstrs
                (int dim, const char * label, const char * unit, const char * format);

    extern int  DFSDIsetdatastrs
                (const char * label, const char * unit, const char * format,
                 const char * coordsys);

#if defined c_plusplus || defined __cplusplus
}
#endif                          /* c_plusplus || __cplusplus */

#endif                          /* _DFSD_H */
