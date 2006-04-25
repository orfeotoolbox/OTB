
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
 * File:    hconv.h
 * Purpose: header file for data conversion information & structures
 * Invokes:
 * Contents:
 * Structure definitions:
 * Constant definitions: lots...
 *---------------------------------------------------------------------------*/

/* avoid re-inclusion */
#ifndef _HCONV_H
#define _HCONV_H

#ifndef VMS
#define DUFF
#else
#ifdef DUFF
#undef DUFF
#endif
#endif
#ifdef DUFF
#define DUFF_sb4b

#define DUFF_ui2i
#define DUFF_ui2s
#define DUFF_uo2i
#define DUFF_ui4i
#define DUFF_ui4s
#define DUFF_uo4i
#define DUFF_ui4f
#define DUFF_uo4f
#define DUFF_ui8f
#define DUFF_uo8f

#define DUFF_lui2i
#define DUFF_lui2s
#define DUFF_luo2i
#define DUFF_lui4i
#define DUFF_lui4s
#define DUFF_luo4i
#define DUFF_lui4f
#define DUFF_luo4f
#define DUFF_lui8f
#define DUFF_luo8f
#endif

/*****************************************************************************/
/* CONSTANT DEFINITIONS                                                      */
/*****************************************************************************/
/* Generally Big-Endian machines */
#if !defined(VMS) && !defined(INTEL86) && !defined(MIPSEL) && !defined(DEC_ALPHA) && !defined(I860) && !defined(SUN386) && !defined(__ia64) && !defined(__x86_64__)
#   if !defined(UNICOS)
#       define UI8_IN     DFKnb1b   /* Unsigned Integer, 8 bits */
#       define UI8_OUT    DFKnb1b
#   if defined(CRAYMPP)
#       define SI16_IN    DFKmi2s
#       define SI16_OUT   DFKmo2b
#       define UI16_IN    DFKmi2i
#       define UI16_OUT   DFKmo2b
#   else
#       define SI16_IN    DFKnb2b   /* S = Signed */
#       define SI16_OUT   DFKnb2b
#       define UI16_IN    DFKnb2b
#       define UI16_OUT   DFKnb2b
#   endif
#       define SI32_IN    DFKnb4b
#       define SI32_OUT   DFKnb4b
#       define UI32_IN    DFKnb4b
#       define UI32_OUT   DFKnb4b
#   if defined(CONVEXNATIVE)
#       define F32_IN     DFKci4f   /* CONVEX stuff */
#       define F32_OUT    DFKco4f
#       define F64_IN     DFKci8f
#       define F64_OUT    DFKco8f
#   elif defined(VP)
#       define F32_IN     DFKpi4f   /* Fujitsu VP stuff */
#       define F32_OUT    DFKpo4f
#       define F64_IN     DFKpi8f
#       define F64_OUT    DFKpo8f
#   else    /* !CONVEXNATIVE */
#       define F32_IN     DFKnb4b   /* Float, 32 bits */
#       define F32_OUT    DFKnb4b
#       define F64_IN     DFKnb8b
#       define F64_OUT    DFKnb8b
#   endif   /* CONVEXNATIVE */

#       define LUI8_IN    DFKnb1b   /* Little Endian Unsigned Integer, 8 bits */
#       define LUI8_OUT   DFKnb1b
#   if defined(CRAYMPP)
#       define LSI16_IN   DFKlmi2s
#       define LSI16_OUT  DFKlmo2b
#       define LUI16_IN   DFKlmi2i
#       define LUI16_OUT  DFKlmo2b
#   else
#       define LSI16_IN   DFKsb2b
#       define LSI16_OUT  DFKsb2b
#       define LUI16_IN   DFKsb2b
#       define LUI16_OUT  DFKsb2b
#   endif
#       define LSI32_IN   DFKsb4b
#       define LSI32_OUT  DFKsb4b
#       define LUI32_IN   DFKsb4b
#       define LUI32_OUT  DFKsb4b
#       if defined(CONVEXNATIVE)
#           define LF32_IN  DFKlci4f    /* CONVEX little-endian routines */
#           define LF32_OUT DFKlco4f
#           define LF64_IN  DFKlci8f
#           define LF64_OUT DFKlco8f
#       elif defined(VP)
#           define LF32_IN   DFKlpi4f   /* Fujitsu VP little-endian routines */
#           define LF32_OUT  DFKlpo4f
#           define LF64_IN   DFKlpi8f
#           define LF64_OUT  DFKlpo8f
#       else    /* !CONVEXNATIVE */
#           define LF32_IN  DFKsb4b
#           define LF32_OUT DFKsb4b
#           define LF64_IN  DFKsb8b
#           define LF64_OUT DFKsb8b
#       endif   /* CONVEXNATIVE */

#   else    /* UNICOS */
#       define UI8_IN     DFKnb1b   /* Big-Endian IEEE support */
#       define UI8_OUT    DFKnb1b
#       define SI16_IN    DFKui2s
#       define SI16_OUT   DFKuo2s
#       define UI16_IN    DFKui2i
#       define UI16_OUT   DFKuo2i
#       define SI32_IN    DFKui4s
#       define SI32_OUT   DFKuo4s
#       define UI32_IN    DFKui4i
#       define UI32_OUT   DFKuo4i
#       define F32_IN     DFKui4f
#       define F32_OUT    DFKuo4f
#       define F64_IN     DFKui8f
#       define F64_OUT    DFKuo8f

#       define LUI8_IN    DFKnb1b   /* Little-endian IEEE support */
#       define LUI8_OUT   DFKnb1b
#       define LSI16_IN   DFKlui2s
#       define LSI16_OUT  DFKluo2s
#       define LUI16_IN   DFKlui2i
#       define LUI16_OUT  DFKluo2i
#       define LSI32_IN   DFKlui4s
#       define LSI32_OUT  DFKluo4s
#       define LUI32_IN   DFKlui4i
#       define LUI32_OUT  DFKluo4i
#       define LF32_IN    DFKlui4f
#       define LF32_OUT   DFKluo4f
#       define LF64_IN    DFKlui8f
#       define LF64_OUT   DFKluo8f

#   endif   /* !UNICOS */
#else  /* must be VMS || INTEL86 || MIPSEL || DEC_ALPHA || I860 || SUN386 || IA64 || Linux64 (Generally, little-endian machines */
#   define UI8_IN     DFKnb1b   /* Big-Endian IEEE support */
#   define UI8_OUT    DFKnb1b   /* The s in DFKsb2b is for swap */
#   define SI16_IN    DFKsb2b
#   define SI16_OUT   DFKsb2b
#   define UI16_IN    DFKsb2b
#   define UI16_OUT   DFKsb2b
#   define SI32_IN    DFKsb4b
#   define SI32_OUT   DFKsb4b
#   define UI32_IN    DFKsb4b
#   define UI32_OUT   DFKsb4b
#   if defined(VMS)
#       define F32_IN     DFKvi4f
#       define F32_OUT    DFKvo4f
#       define F64_IN     DFKvi8f
#       define F64_OUT    DFKvo8f
#   else    /* !VMS */
#       define F32_IN     DFKsb4b
#       define F32_OUT    DFKsb4b
#       define F64_IN     DFKsb8b
#       define F64_OUT    DFKsb8b
#   endif   /* VMS */

#   define LUI8_IN    DFKnb1b   /* Little-Endian IEEE support */
#   define LUI8_OUT   DFKnb1b
#   define LSI16_IN   DFKnb2b
#   define LSI16_OUT  DFKnb2b
#   define LUI16_IN   DFKnb2b
#   define LUI16_OUT  DFKnb2b
#   define LSI32_IN   DFKnb4b
#   define LSI32_OUT  DFKnb4b
#   define LUI32_IN   DFKnb4b
#   define LUI32_OUT  DFKnb4b
#   if defined(VMS)
#       define LF32_IN     DFKlvi4f
#       define LF32_OUT    DFKlvo4f
#       define LF64_IN     DFKlvi8f
#       define LF64_OUT    DFKlvo8f
#   else    /* !VMS */
#       define LF32_IN    DFKnb4b
#       define LF32_OUT   DFKnb4b
#       define LF64_IN    DFKnb8b
#       define LF64_OUT   DFKnb8b
#   endif   /* VMS */

#endif /* !VMS && !INTEL86 && !MIPS && !DEC_ALPHA && !I860 && !SUN386 && !IA64 && !Linux64*/

/* All Machines (except the Cray) currently use the same routines */
/* for Native mode "conversions" */
#ifndef UNICOS
#     define NUI8_IN    DFKnb1b
#     define NUI8_OUT   DFKnb1b
#   if defined(CRAYMPP)
#     define NSI16_IN   DFKmi2s
#     define NSI16_OUT  DFKmo2b
#     define NUI16_IN   DFKmi2i
#     define NUI16_OUT  DFKmo2b
#   else
#     define NSI16_IN   DFKnb2b
#     define NSI16_OUT  DFKnb2b
#     define NUI16_IN   DFKnb2b
#     define NUI16_OUT  DFKnb2b
#   endif
#     define NSI32_IN   DFKnb4b
#     define NSI32_OUT  DFKnb4b
#     define NUI32_IN   DFKnb4b
#     define NUI32_OUT  DFKnb4b
#     define NF32_IN    DFKnb4b
#     define NF32_OUT   DFKnb4b
#     define NF64_IN    DFKnb8b
#     define NF64_OUT   DFKnb8b
#else  /* UNICOS */
#     define NUI8_IN    DFKnb1b
#     define NUI8_OUT   DFKnb1b
#     define NSI16_IN   DFKnb8b
#     define NSI16_OUT  DFKnb8b
#     define NUI16_IN   DFKnb8b
#     define NUI16_OUT  DFKnb8b
#     define NSI32_IN   DFKnb8b
#     define NSI32_OUT  DFKnb8b
#     define NUI32_IN   DFKnb8b
#     define NUI32_OUT  DFKnb8b
#     define NF32_IN    DFKnb8b
#     define NF32_OUT   DFKnb8b
#     define NF64_IN    DFKnb8b
#     define NF64_OUT   DFKnb8b
#endif /* UNICOS */

/*****************************************************************************/
/* STRUCTURE DEFINTIONS                                                      */
/*****************************************************************************/
union fpx
  {
      float       f;
      long        l;
  };

union float_uint_uchar
  {
      float32     f;
      int32       i;
      unsigned char c[4];
  };

#endif /* _HCONV_H */
