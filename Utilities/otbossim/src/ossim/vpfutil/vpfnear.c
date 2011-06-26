/***************************************************************************
 *
 *N  VPFNEAR.C
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    This module contains functions to determine the nearest distance
 *    from a given point to a specified edge.  The algorithm is to find
 *    the perpendicular bisector from each segment of the edge to the
 *    test point, if it exists, and calculate the distance.  If a
 *    perpendicular from the line to the test point does not exist,
 *    it computes the  distance to both the start and end node and takes
 *    the minimum.  The minimum distance found is the closest distance to
 *    the edge.
 *    There are two main calling interfaces in this module:
 *       distance_to_edge() if your edge table is already open.
 *       distance_to_edge_table() to open the edge table, calculate,
 *                                and then close the table when finished.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    Nov 1991
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 **************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#ifdef __MSDOS__
#include <conio.h>
#endif
#include <math.h>
#if defined(__CYGWIN__) || defined(__APPLE__)|| defined(_WIN32)
#include <ossim/vpfutil/values.h>
#else
#include <ossim/vpfutil/values.h>
#endif
#ifdef __MSDOS__
#include <graphics.h>
#endif
#include <ossim/vpfutil/vpfapi.h>
#include <ossim/vpfutil/vpftable.h>
#include <ossim/vpfutil/vpfprim.h>
#include <ossim/vpfutil/vpfview.h>
#include <ossim/vpfutil/mapgraph.h>
#  ifndef min
#    define min(a,b) ((a) <= (b) ? (a) : (b))
#  endif
#  ifndef max
#    define max(a,b) ((a) <= (b) ? (b) : (a))
#  endif
typedef struct {
   float x1;
   float y1;
   float x2;
   float y2;
} line_segment_type;


/**************************************************************************
 *
 *N  perpendicular_intersection
 *
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    Test whether a perpendicular to a line segment intersects a search
 *    point.  lseg must be of a type similar to line_segment_type.
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    lseg      <input> == (line_segment_type) line segment.
 *    xsearch   <input> == (float) search point x coordinate.
 *    ysearch   <input> == (float) search point y coordinate.
 *    xint      <inout> == (float *) intersection point x coordinate.
 *    yint      <inout> == (float *) intersection point y coordinate.
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels, DOS Turbo C   1991
 *    Ronald Rozensky, vertical and horizontal line handlers.
 *E
 *************************************************************************/
int perpendicular_intersection( line_segment_type lseg,
				float xsearch, float ysearch,
				float *xint, float *yint )
{
   float m1=0,mp=0,b1=0,bp=0; /* m's are for slopes, b's are for y intersections */
		      /* for the old y=mx+b point slope formula.         */

   if (lseg.y1 == lseg.y2)
      if (((lseg.x1 <= xsearch) && (xsearch <= lseg.x2)) ||
	  ((lseg.x2 <= xsearch) && (xsearch <= lseg.x1)))
	 {
	 *xint = xsearch;
	 *yint = lseg.y1;
	 return 1;
	 }

   if (lseg.x1 == lseg.x2)

      if (((lseg.y1 <= ysearch) && (ysearch <= lseg.y2)) ||
	  ((lseg.y2 <= ysearch) && (ysearch <= lseg.y1)))
	 {
	 *xint = lseg.x1;
	 *yint = ysearch;
	 return 1;
	 }

   if ( (lseg.x1==xsearch) && (lseg.y1==ysearch) ) {
      *xint = xsearch;
      *yint = ysearch;
      return 1;
   }
   if ( (lseg.x2==xsearch) && (lseg.y2==ysearch) ) {
      *xint = xsearch;
      *yint = ysearch;
      return 1;
   }

   if (lseg.x1 != lseg.x2) {
      m1 = (lseg.y2-lseg.y1)/(lseg.x2-lseg.x1);
      b1 = -1*m1*lseg.x1 + lseg.y1;
      if (m1 != 0.0) {
	 mp = (float)(-1.0/m1);       /* perpendicular */
      } else {
	 mp = MAXFLOAT;
      }
   } else {
      m1 = MAXFLOAT;
      mp = 0.0;
   }

   if (fabs(mp) < 1000000.0) {
      bp = ysearch - mp*xsearch;
   } else {
      bp = MAXFLOAT;
   }

   /* Find intersection point of lseg and its perpendicular */

   if ( (m1 != MAXFLOAT) && (mp != MAXFLOAT) ) {
      *xint = (bp-b1)/(m1-mp);
      *yint = m1*(*xint)+b1;
   } else {                   /* At least one vertical line */
      if (m1==MAXFLOAT) {          /* lseg is vertical */
	 *yint = ysearch;
	 *xint = lseg.x1;
      } else {                     /* perp is vertical */
	 *yint = lseg.y1;
	 *xint = xsearch;
      }
   }

   /* See if intersection point lies on both line segments */
   return ( (*xint >= (float)min(lseg.x1,lseg.x2)) &&
	    (*xint <= (float)max(lseg.x1,lseg.x2)) &&
	    (*yint >= (float)min(lseg.y1,lseg.y2)) &&
	    (*yint <= (float)max(lseg.y1,lseg.y2)) );
}


/* In VPFMISC.C for the VPFVIEW application */
float distance( double lat1, double lon1, double lat2, double lon2,
		int units );


/**************************************************************************
 *
 *N  distance_to_edge_rec
 *
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    This function computes the minimum distance from the given point
 *    to the given edge record by looking at each segment in the line.
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    x           <input> == (float) given point x coordinate.
 *    y           <input> == (float) given point y coordinate
 *    edge_rec    <input> == (edge_rec_type) given edge record.
 *    dec_degrees <inout> == (int) flag to indicate if coordinates are
 *                                 in decimal degrees.
 *    distance_to_edge_rec <output> == (float) minimum distance to the
 *                                     edge record.
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels, DOS Turbo C   1991
 *E
 *************************************************************************/
float distance_to_edge_rec( float x, float y, edge_rec_type edge_rec,
			    int dec_degrees )
{
   register ossim_int32 i;
   line_segment_type lseg;
   float xint,yint, d, dseg, d1, d2;
   coordinate_type coord1, coord2;

   d = MAXFLOAT;

   coord1 = first_edge_coordinate(&edge_rec);
   for (i=1;i<edge_rec.npts;i++) {
      coord2 = next_edge_coordinate(&edge_rec);
      lseg.x1 = coord1.x;
      lseg.y1 = coord1.y;
      lseg.x2 = coord2.x;
      lseg.y2 = coord2.y;

      if (perpendicular_intersection(lseg,x,y,&xint,&yint)) {
	 if (dec_degrees)
	    dseg = distance( y, x, yint, xint, 0 );
	 else
	    dseg = (float)(sqrt( (xint-x)*(xint-x) + (yint-y)*(yint-y) ));
      } else {
	 if (dec_degrees) {
	    dseg = (float)min( distance( y, x, lseg.y1, lseg.x1, 0 ),
			       distance( y, x, lseg.y2, lseg.x2, 0 ) );
	 } else {
	    d1 = (float)(sqrt( (lseg.x1-x)*(lseg.x1-x) + (lseg.y1-y)*(lseg.y1-y) ));
	    d2 = (float)(sqrt( (lseg.x2-x)*(lseg.x2-x) + (lseg.y2-y)*(lseg.y2-y) ));
	    dseg = (float)min(d1,d2);
	 }
      }

      d = (float)min(d,dseg);

      coord1 = coord2;
   }

   return d;
}



/**************************************************************************
 *
 *N  distance_to_edge
 *
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    This function reads the specified edge from the given edge table
 *    and computes the minimum distance from the given point to the
 *    specified edge.  'edge_id' must be contained in the edge table.
 *    edge_table must have been successfully opened with storage = DISK.
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    x           <input> == (float) given point x coordinate.
 *    y           <input> == (float) given point y coordinate
 *    edge_id     <input> == (ossim_int32) specified edge.
 *    edgetable   <input> == (vpf_table_type) given edge table.
 *    dec_degrees <input> == (float) flag to indicate if coordinates are
 *                                   in decimal degrees.
 *    distance_to_edge <output> == (float) minimum distance to the given
 *                                         edge from the given point.
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels, DOS Turbo C   1991
 *E
 *************************************************************************/
float distance_to_edge( float x, float y,
			ossim_int32 edge_id,
			vpf_table_type edgetable,
			int dec_degrees )
{
   edge_rec_type edge_rec;
   float result;

   edge_rec = read_edge( edge_id, edgetable );

   result = distance_to_edge_rec( x, y, edge_rec, dec_degrees );

   if (edge_rec.coord) free( edge_rec.coord );

   return result;
}



/**************************************************************************
 *
 *N  distance_to_edge_table
 *
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    This function reads the specified edge from the named edge table
 *    and computes the minimum distance from the given point to the
 *    specified edge.  'edge_id' must be contained in the edge table.
 *    'fname' must be a valid path to a VPF edge table.
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    x           <input> == (float) given point x coordinate.
 *    y           <input> == (float) given point y coordinate
 *    edge_id    <input> == (ossim_int32) specified edge id.
 *    fname      <input> == (char *) path to valid VPF edge table.
 *    dec_degrees <output> == (int) flag to indicate if coordinates are
 *                                  in decimal degrees.
 *    distance_to_edge_table <output> == (float) distance from the given
 *                                       point to the specified edge.
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels, DOS Turbo C   1991
 *E
 *************************************************************************/
float distance_to_edge_table( float x, float y,
			      ossim_int32 edge_id,
			      char *fname, int dec_degrees )
{
   vpf_table_type edgetable;
   float result;
   char *defstr=" ";

   edgetable = vpf_open_table(fname,disk, "rb", defstr);

   result = distance_to_edge( x, y, edge_id, edgetable, dec_degrees );

   vpf_close_table(&edgetable);

   return result;
}

