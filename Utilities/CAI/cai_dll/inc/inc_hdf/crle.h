
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
 * File:    crle.h
 * Purpose: Header file for run-length encoding information.
 * Dependencies: should only be included from hcompi.h
 * Invokes: none
 * Contents: Structures & definitions for run-length encoding.  This header
 *              should only be included in hcomp.c and crle.c.
 * Structure definitions:
 * Constant definitions:
 *---------------------------------------------------------------------------*/

/* avoid re-inclusion */
#ifndef __CRLE_H
#define __CRLE_H

#if defined c_plusplus || defined __cplusplus
extern      "C"
{
#endif                          /* c_plusplus || __cplusplus */

/*
   ** from crle.c
 */

    extern int32 HCPcrle_stread
                (accrec_t * rec);

    extern int32 HCPcrle_stwrite
                (accrec_t * rec);

    extern int32 HCPcrle_seek
                (accrec_t * access_rec, int32 offset, int origin);

    extern int32 HCPcrle_inquire
                (accrec_t * access_rec, int32 *pfile_id, uint16 *ptag, uint16 *pref,
               int32 *plength, int32 *poffset, int32 *pposn, int16 *paccess,
                 int16 *pspecial);

    extern int32 HCPcrle_read
                (accrec_t * access_rec, int32 length, void * data);

    extern int32 HCPcrle_write
                (accrec_t * access_rec, int32 length, const void * data);

    extern intn HCPcrle_endaccess
                (accrec_t * access_rec);

#if defined c_plusplus || defined __cplusplus
}
#endif                          /* c_plusplus || __cplusplus */

/* size of the RLE buffer */
#define RLE_BUF_SIZE    128
/* NIL code for run bytes */
#define RLE_NIL         (-1)
/* minimum length of run */
#define RLE_MIN_RUN     3
/* maximum length of run */
#define RLE_MAX_RUN     (RLE_BUF_SIZE+RLE_MIN_RUN-1)
/* minimum length of mix */
#define RLE_MIN_MIX     1

/* RLE [en|de]coding information */
typedef struct
{
    int32       offset;         /* offset in the file */
    uint8       buffer[RLE_BUF_SIZE];   /* buffer for storing RLE bytes */
    intn        buf_length;     /* number of bytes in buffer */
    intn        buf_pos;        /* offset into the buffer */
    uintn       last_byte,      /* the last byte stored in the buffer */
                second_byte;    /* the second to last byte stored in the buffer */
    enum
      {
          RLE_INIT,             /* initial state, need to read a byte to determine
                                   next state */
          RLE_RUN,              /* buffer up to the current position is a run */
          RLE_MIX
      }                         /* buffer up to the current position is a mix */
    rle_state;                  /* state of the buffer storage */
}
comp_coder_rle_info_t;

#ifndef CRLE_MASTER
extern funclist_t crle_funcs;   /* functions to perform run-length encoding */
#else
funclist_t  crle_funcs =
{                               /* functions to perform run-length encoding */
    HCPcrle_stread,
    HCPcrle_stwrite,
    HCPcrle_seek,
    HCPcrle_inquire,
    HCPcrle_read,
    HCPcrle_write,
    HCPcrle_endaccess
};
#endif

#endif /* __CRLE_H */
