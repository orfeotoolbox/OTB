#ifndef _SHAPEFILE_H_INCLUDED
#define _SHAPEFILE_H_INCLUDED

/******************************************************************************
 * $Id: shapefil.h 11039 2007-05-24 15:42:43Z gpotts $
 *
 * Project:  Shapelib
 * Purpose:  Primary include file for Shapelib.
 * Author:   Frank Warmerdam, warmerdam@pobox.com
 *
 ******************************************************************************
 * Copyright (c) 1999, Frank Warmerdam
 *
 * This software is available under the following "MIT Style" license,
 * or at the option of the licensee under the LGPL (see LICENSE.LGPL).  This
 * option is discussed in more detail in shapelib.html.
 *
 * --
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 ******************************************************************************
 *
 * $Log$
 * Revision 1.5  2005/10/08 12:57:26  gpotts
 * Added adiitional patches
 *
 * Revision 1.14  2005/03/14 15:21:29  fwarmerdam
 * updated
 *
 * Revision 1.30  2005/01/03 22:30:13  fwarmerdam
 * added support for saved quadtrees
 *
 * Revision 1.29  2004/09/26 20:09:35  fwarmerdam
 * avoid rcsid warnings
 *
 * Revision 1.28  2003/12/29 06:02:18  fwarmerdam
 * added cpl_error.h option
 *
 * Revision 1.27  2003/04/21 18:30:37  warmerda
 * added header write/update public methods
 *
 * Revision 1.26  2002/09/29 00:00:08  warmerda
 * added FTLogical and logical attribute read/write calls
 *
 * Revision 1.25  2002/05/07 13:46:30  warmerda
 * added DBFWriteAttributeDirectly().
 *
 * Revision 1.24  2002/04/10 16:59:54  warmerda
 * added SHPRewindObject
 *
 * Revision 1.23  2002/01/15 14:36:07  warmerda
 * updated email address
 *
 * Revision 1.22  2002/01/15 14:32:00  warmerda
 * try to improve SHPAPI_CALL docs
 *
 * Revision 1.21  2001/11/01 16:29:55  warmerda
 * move pabyRec into SHPInfo for thread safety
 *
 * Revision 1.20  2001/07/20 13:06:02  warmerda
 * fixed SHPAPI attribute for SHPTreeFindLikelyShapes
 *
 * Revision 1.19  2001/05/31 19:20:13  warmerda
 * added DBFGetFieldIndex()
 *
 * Revision 1.18  2001/05/31 18:15:40  warmerda
 * Added support for NULL fields in DBF files
 *
 * Revision 1.17  2001/05/23 13:36:52  warmerda
 * added use of SHPAPI_CALL
 *
 * Revision 1.16  2000/09/25 14:15:59  warmerda
 * added DBFGetNativeFieldType()
 *
 * Revision 1.15  2000/02/16 16:03:51  warmerda
 * added null shape support
 *
 * Revision 1.14  1999/11/05 14:12:05  warmerda
 * updated license terms
 *
 * Revision 1.13  1999/06/02 18:24:21  warmerda
 * added trimming code
 *
 * Revision 1.12  1999/06/02 17:56:12  warmerda
 * added quad'' subnode support for trees
 *
 * Revision 1.11  1999/05/18 19:11:11  warmerda
 * Added example searching capability
 *
 * Revision 1.10  1999/05/18 17:49:38  warmerda
 * added initial quadtree support
 *
 * Revision 1.9  1999/05/11 03:19:28  warmerda
 * added new Tuple api, and improved extension handling - add from candrsn
 *
 * Revision 1.8  1999/03/23 17:22:27  warmerda
 * Added extern "C" protection for C++ users of shapefil.h.
 *
 * Revision 1.7  1998/12/31 15:31:07  warmerda
 * Added the TRIM_DBF_WHITESPACE and DISABLE_MULTIPATCH_MEASURE options.
 *
 * Revision 1.6  1998/12/03 15:48:15  warmerda
 * Added SHPCalculateExtents().
 *
 * Revision 1.5  1998/11/09 20:57:16  warmerda
 * Altered SHPGetInfo() call.
 *
 * Revision 1.4  1998/11/09 20:19:33  warmerda
 * Added 3D support, and use of SHPObject.
 *
 * Revision 1.3  1995/08/23 02:24:05  warmerda
 * Added support for reading bounds.
 *
 * Revision 1.2  1995/08/04  03:17:39  warmerda
 * Added header.
 *
 */

#include <stdio.h>

#ifdef USE_DBMALLOC
#include <dbmalloc.h>
#endif

#ifdef USE_CPL
#include "cpl_error.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************/
/*                        Configuration options.                        */
/************************************************************************/

/* -------------------------------------------------------------------- */
/*      Should the DBFReadStringAttribute() strip leading and           */
/*      trailing white space?                                           */
/* -------------------------------------------------------------------- */
#define TRIM_DBF_WHITESPACE

/* -------------------------------------------------------------------- */
/*      Should we write measure values to the Multipatch object?        */
/*      Reportedly ArcView crashes if we do write it, so for now it     */
/*      is disabled.                                                    */
/* -------------------------------------------------------------------- */
#define DISABLE_MULTIPATCH_MEASURE

/* -------------------------------------------------------------------- */
/*      SHPAPI_CALL                                                     */
/*                                                                      */
/*      The following two macros are present to allow forcing           */
/*      various calling conventions on the Shapelib API.                */
/*                                                                      */
/*      To force __stdcall conventions (needed to call Shapelib         */
/*      from Visual Basic and/or Dephi I believe) the makefile could    */
/*      be modified to define:                                          */
/*                                                                      */
/*        /DSHPAPI_CALL=__stdcall                                       */
/*                                                                      */
/*      If it is desired to force export of the Shapelib API without    */
/*      using the shapelib.def file, use the following definition.      */
/*                                                                      */
/*        /DSHAPELIB_DLLEXPORT                                          */
/*                                                                      */
/*      To get both at once it will be necessary to hack this           */
/*      include file to define:                                         */
/*                                                                      */
/*        #define SHPAPI_CALL __declspec(dllexport) __stdcall           */
/*        #define SHPAPI_CALL1 __declspec(dllexport) * __stdcall        */
/*                                                                      */
/*      The complexity of the situtation is partly caused by the        */
/*      peculiar requirement of Visual C++ that __stdcall appear        */
/*      after any "*"'s in the return value of a function while the     */
/*      __declspec(dllexport) must appear before them.                  */
/* -------------------------------------------------------------------- */
#include <ossim/base/ossimConstants.h>

#ifdef OSSIMMAKINGDLL
#  define ossim_SHPAPI_CALL OSSIM_DLL
#  define ossim_SHPAPI_CALL1(x)  OSSIM_DLL_DATA(x)
#endif
#ifndef ossim_SHPAPI_CALL
#  define ossim_SHPAPI_CALL
#endif
#ifndef ossim_SHPAPI_CALL1
#  define ossim_SHPAPI_CALL1(x)      x ossim_SHPAPI_CALL
#endif

#if 0
#ifdef SHAPELIB_DLLEXPORT
#  define ossim_SHPAPI_CALL __declspec(dllexport)
#  define ossim_SHPAPI_CALL1(x)  __declspec(dllexport) x
#endif
#ifndef ossim_SHPAPI_CALL
#  define ossim_SHPAPI_CALL
#endif
#ifndef ossim_SHPAPI_CALL1
#  define ossim_SHPAPI_CALL1(x)      x ossim_SHPAPI_CALL
#endif
#endif

/* -------------------------------------------------------------------- */
/*      Macros for controlling CVSID and ensuring they don't appear     */
/*      as unreferenced variables resulting in lots of warnings.        */
/* -------------------------------------------------------------------- */
#ifndef DISABLE_CVSID
#  define ossim_SHP_CVSID(string)     static char cpl_cvsid[] = string; \
static char *cvsid_aw() { return( cvsid_aw() ? ((char *) NULL) : cpl_cvsid ); }
#else
#  define ossim_SHP_CVSID(string)
#endif

/************************************************************************/
/*                             ossim_SHP Support.                             */
/************************************************************************/
typedef	struct
{
    FILE        *fpSHP;
    FILE	*fpSHX;

    int		nShapeType;				/* SHPT_* */

    int		nFileSize;				/* SHP file */

    int         nRecords;
    int		nMaxRecords;
    int		*panRecOffset;
    int		*panRecSize;

    double	adBoundsMin[4];
    double	adBoundsMax[4];

    int		bUpdated;

    unsigned char *pabyRec;
    int         nBufSize;
} ossim_SHPInfo;

typedef ossim_SHPInfo * ossim_SHPHandle;

/* -------------------------------------------------------------------- */
/*      Shape types (nSHPType)                                          */
/* -------------------------------------------------------------------- */
#define ossim_SHPT_NULL	0
#define ossim_SHPT_POINT	1
#define ossim_SHPT_ARC	3
#define ossim_SHPT_POLYGON	5
#define ossim_SHPT_MULTIPOINT	8
#define ossim_SHPT_POINTZ	11
#define ossim_SHPT_ARCZ	13
#define ossim_SHPT_POLYGONZ	15
#define ossim_SHPT_MULTIPOINTZ 18
#define ossim_SHPT_POINTM	21
#define ossim_SHPT_ARCM	23
#define ossim_SHPT_POLYGONM	25
#define ossim_SHPT_MULTIPOINTM 28
#define ossim_SHPT_MULTIPATCH 31


/* -------------------------------------------------------------------- */
/*      Part types - everything but ossim_SHPT_MULTIPATCH just uses           */
/*      ossim_SHPP_RING.                                                      */
/* -------------------------------------------------------------------- */

#define ossim_SHPP_TRISTRIP	0
#define ossim_SHPP_TRIFAN	1
#define ossim_SHPP_OUTERRING	2
#define ossim_SHPP_INNERRING	3
#define ossim_SHPP_FIRSTRING	4
#define ossim_SHPP_RING	5

/* -------------------------------------------------------------------- */
/*      ossim_SHPObject - represents on shape (without attributes) read       */
/*      from the .shp file.                                             */
/* -------------------------------------------------------------------- */
typedef struct
{
    int		nSHPType;

    int		nShapeId; /* -1 is unknown/unassigned */

    int		nParts;
    int		*panPartStart;
    int		*panPartType;

    int		nVertices;
    double	*padfX;
    double	*padfY;
    double	*padfZ;
    double	*padfM;

    double	dfXMin;
    double	dfYMin;
    double	dfZMin;
    double	dfMMin;

    double	dfXMax;
    double	dfYMax;
    double	dfZMax;
    double	dfMMax;
} ossim_SHPObject;

/* -------------------------------------------------------------------- */
/*      ossim_SHP API Prototypes                                              */
/* -------------------------------------------------------------------- */
ossim_SHPHandle ossim_SHPAPI_CALL
      ossim_SHPOpen( const char * pszShapeFile, const char * pszAccess );
ossim_SHPHandle ossim_SHPAPI_CALL
      ossim_SHPCreate( const char * pszShapeFile, int nShapeType );
void ossim_SHPAPI_CALL
      ossim_SHPGetInfo( ossim_SHPHandle hSHP, int * pnEntities, int * pnShapeType,
                  double * padfMinBound, double * padfMaxBound );

ossim_SHPObject ossim_SHPAPI_CALL1(*)
      ossim_SHPReadObject( ossim_SHPHandle hSHP, int iShape );
int ossim_SHPAPI_CALL
      ossim_SHPWriteObject( ossim_SHPHandle hSHP, int iShape, ossim_SHPObject * psObject );

void ossim_SHPAPI_CALL
      ossim_SHPDestroyObject( ossim_SHPObject * psObject );
void ossim_SHPAPI_CALL
      ossim_SHPComputeExtents( ossim_SHPObject * psObject );
ossim_SHPObject ossim_SHPAPI_CALL1(*)
      ossim_SHPCreateObject( int nSHPType, int nShapeId, int nParts,
                       const int * panPartStart, const int * panPartType,
                       int nVertices,
                       const double * padfX, const double * padfY,
                       const double * padfZ, const double * padfM );
ossim_SHPObject ossim_SHPAPI_CALL1(*)
      ossim_SHPCreateSimpleObject( int nSHPType, int nVertices,
                             const double * padfX,
                             const double * padfY,
                             const double * padfZ );

int ossim_SHPAPI_CALL
      ossim_SHPRewindObject( ossim_SHPHandle hSHP, ossim_SHPObject * psObject );

void ossim_SHPAPI_CALL ossim_SHPClose( ossim_SHPHandle hSHP );
void ossim_SHPAPI_CALL ossim_SHPWriteHeader( ossim_SHPHandle hSHP );

const char ossim_SHPAPI_CALL1(*)
      ossim_SHPTypeName( int nSHPType );
const char ossim_SHPAPI_CALL1(*)
      ossim_SHPPartTypeName( int nPartType );

/* -------------------------------------------------------------------- */
/*      Shape quadtree indexing API.                                    */
/* -------------------------------------------------------------------- */

/* this can be two or four for binary or quad tree */
#define MAX_SUBNODE	4

typedef struct shape_tree_node
{
    /* region covered by this node */
    double	adfBoundsMin[4];
    double	adfBoundsMax[4];

    /* list of shapes stored at this node.  The papsShapeObj pointers
       or the whole list can be NULL */
    int		nShapeCount;
    int		*panShapeIds;
    ossim_SHPObject   **papsShapeObj;

    int		nSubNodes;
    struct shape_tree_node *apsSubNode[MAX_SUBNODE];

} ossim_SHPTreeNode;

typedef struct
{
    ossim_SHPHandle   hSHP;

    int		nMaxDepth;
    int		nDimension;
    int         nTotalCount;

    ossim_SHPTreeNode	*psRoot;
} ossim_SHPTree;

ossim_SHPTree ossim_SHPAPI_CALL1(*)
      ossim_SHPCreateTree( ossim_SHPHandle hSHP, int nDimension, int nMaxDepth,
                     double *padfBoundsMin, double *padfBoundsMax );
void    ossim_SHPAPI_CALL
      ossim_SHPDestroyTree( ossim_SHPTree * hTree );

int	ossim_SHPAPI_CALL
      ossim_SHPWriteTree( ossim_SHPTree *hTree, const char * pszFilename );
ossim_SHPTree ossim_SHPAPI_CALL
      ossim_SHPReadTree( const char * pszFilename );

int	ossim_SHPAPI_CALL
      ossim_SHPTreeAddObject( ossim_SHPTree * hTree, ossim_SHPObject * psObject );
int	ossim_SHPAPI_CALL
      ossim_SHPTreeAddShapeId( ossim_SHPTree * hTree, ossim_SHPObject * psObject );
int	ossim_SHPAPI_CALL
      ossim_SHPTreeRemoveShapeId( ossim_SHPTree * hTree, int nShapeId );

void 	ossim_SHPAPI_CALL
      ossim_SHPTreeTrimExtraNodes( ossim_SHPTree * hTree );

int    ossim_SHPAPI_CALL1(*)
      ossim_SHPTreeFindLikelyShapes( ossim_SHPTree * hTree,
                               double * padfBoundsMin,
                               double * padfBoundsMax,
                               int * );
int     ossim_SHPAPI_CALL
      ossim_SHPCheckBoundsOverlap( double *, double *, double *, double *, int );

int ossim_SHPAPI_CALL1(*)
    ossim_SHPSearchDiskTree( FILE *fp,
                   double *padfBoundsMin, double *padfBoundsMax,
                   int *pnShapeCount );

/************************************************************************/
/*                             DBF Support.                             */
/************************************************************************/
typedef	struct
{
    FILE	*fp;

    int         nRecords;

    int		nRecordLength;
    int		nHeaderLength;
    int		nFields;
    int		*panFieldOffset;
    int		*panFieldSize;
    int		*panFieldDecimals;
    char	*pachFieldType;

    char	*pszHeader;

    int		nCurrentRecord;
    int		bCurrentRecordModified;
    char	*pszCurrentRecord;

    int		bNoHeader;
    int		bUpdated;
} ossim_DBFInfo;

typedef ossim_DBFInfo * ossim_DBFHandle;

typedef enum {
  FTString,
  FTInteger,
  FTDouble,
  FTLogical,
  FTInvalid
} ossim_DBFFieldType;

#define XBASE_FLDHDR_SZ       32

ossim_DBFHandle ossim_SHPAPI_CALL
      ossim_DBFOpen( const char * pszDBFFile, const char * pszAccess );
ossim_DBFHandle ossim_SHPAPI_CALL
      ossim_DBFCreate( const char * pszDBFFile );

int	ossim_SHPAPI_CALL
      ossim_DBFGetFieldCount( ossim_DBFHandle psDBF );
int	ossim_SHPAPI_CALL
      ossim_DBFGetRecordCount( ossim_DBFHandle psDBF );
int	ossim_SHPAPI_CALL
      ossim_DBFAddField( ossim_DBFHandle hDBF, const char * pszFieldName,
                   ossim_DBFFieldType eType, int nWidth, int nDecimals );

ossim_DBFFieldType ossim_SHPAPI_CALL
      ossim_DBFGetFieldInfo( ossim_DBFHandle psDBF, int iField,
                       char * pszFieldName, int * pnWidth, int * pnDecimals );

int ossim_SHPAPI_CALL
      ossim_DBFGetFieldIndex(ossim_DBFHandle psDBF, const char *pszFieldName);

int 	ossim_SHPAPI_CALL
      ossim_DBFReadIntegerAttribute( ossim_DBFHandle hDBF, int iShape, int iField );
double 	ossim_SHPAPI_CALL
      ossim_DBFReadDoubleAttribute( ossim_DBFHandle hDBF, int iShape, int iField );
const char ossim_SHPAPI_CALL1(*)
      ossim_DBFReadStringAttribute( ossim_DBFHandle hDBF, int iShape, int iField );
const char ossim_SHPAPI_CALL1(*)
      ossim_DBFReadLogicalAttribute( ossim_DBFHandle hDBF, int iShape, int iField );
int     ossim_SHPAPI_CALL
      ossim_DBFIsAttributeNULL( ossim_DBFHandle hDBF, int iShape, int iField );

int ossim_SHPAPI_CALL
      ossim_DBFWriteIntegerAttribute( ossim_DBFHandle hDBF, int iShape, int iField,
                                int nFieldValue );
int ossim_SHPAPI_CALL
      ossim_DBFWriteDoubleAttribute( ossim_DBFHandle hDBF, int iShape, int iField,
                               double dFieldValue );
int ossim_SHPAPI_CALL
      ossim_DBFWriteStringAttribute( ossim_DBFHandle hDBF, int iShape, int iField,
                               const char * pszFieldValue );
int ossim_SHPAPI_CALL
     ossim_DBFWriteNULLAttribute( ossim_DBFHandle hDBF, int iShape, int iField );

int ossim_SHPAPI_CALL
     ossim_DBFWriteLogicalAttribute( ossim_DBFHandle hDBF, int iShape, int iField,
			       const char lFieldValue);
int ossim_SHPAPI_CALL
     ossim_DBFWriteAttributeDirectly(ossim_DBFHandle psDBF, int hEntity, int iField,
                               void * pValue );
const char ossim_SHPAPI_CALL1(*)
      ossim_DBFReadTuple(ossim_DBFHandle psDBF, int hEntity );
int ossim_SHPAPI_CALL
      ossim_DBFWriteTuple(ossim_DBFHandle psDBF, int hEntity, void * pRawTuple );

ossim_DBFHandle ossim_SHPAPI_CALL
      ossim_DBFCloneEmpty(ossim_DBFHandle psDBF, const char * pszFilename );

void	ossim_SHPAPI_CALL
      ossim_DBFClose( ossim_DBFHandle hDBF );
void    ossim_SHPAPI_CALL
      ossim_DBFUpdateHeader( ossim_DBFHandle hDBF );
char    ossim_SHPAPI_CALL
      ossim_DBFGetNativeFieldType( ossim_DBFHandle hDBF, int iField );

#ifdef __cplusplus
}
#endif

#endif /* ndef _SHAPEFILE_H_INCLUDED */
