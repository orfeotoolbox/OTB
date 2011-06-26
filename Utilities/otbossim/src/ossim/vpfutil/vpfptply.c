/***************************************************************************
 *
 *N  VPFPTPLY.C
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    This module contains functions to test whether a given point lies
 *    inside of a VPF representation of a face (or polygon, geometrically).
 *    The method used is to create a plum line from the given test point
 *    out to basically infinity and count the number of intersections
 *    with all of the line segements of the outer and inner rings of the
 *    face.  An odd count indicates that the test point is inside of
 *    the face; an even count (0 is even) indicates that the point lies
 *    outside of the polygon (including inside of a hole).
 *    There are two main interface routines:
 *      point_in_face() if you already have the Face primitive tables open.
 *      point_in_face_table() if you want to just specify the path.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions:
 *F
 *    int intersect( line_segment_type l1, line_segment_type l2,
 *        float *xint, float *yint );
 *    int intersect_polygon_edge( float x, float y, edge_rec_type edge_rec );
 *    int intersect_polygon_loop( float x, float y,
 *        int face_id, ossim_int32 start_edge,
 *        vpf_table_type edgetable );
 *    int point_in_face( float x, float y,
 *	  ossim_int32 face_id,
 *        vpf_table_type facetable,
 *        vpf_table_type ringtable,
 *        vpf_table_type edgetable );
 *    int point_in_face_table( float x, float y,
 *	  ossim_int32 face_id,
 *        char *fname );
 *E
 **************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifdef __MSDOS__
#include <conio.h>
#endif
#include <math.h>
#if defined(__CYGWIN__)||defined(__APPLE__)|| defined(_WIN32)
#include <ossim/vpfutil/values.h>
#else
#include <ossim/vpfutil/values.h>
#endif
#ifdef __MSDOS__
#include <graphics.h>
#endif
#include <ossim/vpfutil/vpftable.h>
#include <ossim/vpfutil/vpfprim.h>
#include <ossim/vpfutil/vpfview.h>
#include <ossim/vpfutil/mapgraph.h>
#include <ossim/vpfutil/vpfdraw.h>
#include <ossim/vpfutil/vpftidx.h>
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
 *N  intersect
 *
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    Test whether line segment l1 intersects line segment l2
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    l1   <input> == (line_segment_type) line segment one.
 *    l2   <input> == (line_segment_type) line segment two.
 *    xint <inout> == (float *) intersection point x coordinate.
 *    yint <inout> == (float *) intersection point y coordinate.
 *    intersect <output> == (int) VPF_BOOLEAN:
 *                                1 --> line segments intersect
 *                                0 --> line segments do not intersect
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels  DOS Turbo C
 *E
 *************************************************************************/
int intersect( line_segment_type l1, line_segment_type l2,
	       float *xint, float *yint )
{
   double m1,m2,b1=0,b2=0;
   float tempy1,tempy2;

   if ( (l1.x1==l1.x2) && (l2.y1==l2.y2) )
   {
      /* l1 is vertical and l2 is horizontal */
      
      if ( ( ((l2.x1 <= l1.x1) && (l1.x1 <= l2.x2)) ||    /* X coordinate */
             ((l2.x2 <= l1.x1) && (l1.x1 <= l2.x1)) ) &&  /*  intersects  */
           ( ((l1.y1 <= l2.y1) && (l2.y1 <= l1.y2)) ||    /* Y coordinate */
             ((l1.y2 <= l2.y1) && (l2.y1 <= l1.y1)) ) )   /*  intersects  */
      {
         *xint = l1.x1;
         *yint = l2.y1;
         return 1;
      }
      else
      {
         return 0;
      }
   }

   if ( (l2.x1==l2.x2) && (l1.y1==l1.y2) )
   {
      /* l2 is vertical and l1 is horizontal */

      if ( ( ((l1.x1 <= l2.x1) && (l2.x1 <= l1.x2)) ||    /* X coordinate */
             ((l1.x2 <= l2.x1) && (l2.x1 <= l1.x1)) ) &&  /*  intersects  */
           ( ((l2.y1 <= l1.y1) && (l1.y1 <= l2.y2)) ||    /* Y coordinate */
             ((l2.y2 <= l1.y1) && (l1.y1 <= l2.y1)) ) )   /*  intersects  */
      {
         *xint = l2.x1;
         *yint = l1.y1;
         return 1;
      }
      else
      {
         return 0;
      }
   }

   if ( (l1.x1==l2.x1) && (l1.y1==l2.y1) ) {
      *xint = l1.x1;
      *yint = l1.y1;
      return 1;
   }
   if ( (l1.x2==l2.x2) && (l1.y2==l2.y2) ) {
      *xint = l1.x2;
      *yint = l1.y2;
      return 1;
   }
   if ( (l1.x1==l2.x2) && (l1.y1==l2.y2) ) {
      *xint = l1.x1;
      *yint = l1.y1;
      return 1;
   }
   if ( (l1.x2==l2.x1) && (l1.y2==l2.y1) ) {
      *xint = l1.x2;
      *yint = l1.y2;
      return 1;
   }

   if (l1.x1 != l1.x2) {
      m1 = (l1.y2-l1.y1)/(l1.x2-l1.x1);
      b1 = -1.0*m1*l1.x1 + l1.y1;
   } else {  /* l1 is a vertical line */
      m1 = MAXFLOAT;
   }

   if (l2.x1 != l2.x2) {
      m2 = (l2.y2-l2.y1)/(l2.x2-l2.x1);
      b2 = -1.0*m2*l2.x1 + l2.y1;
   } else {  /* l2 is a vertical line */
      m2 = MAXFLOAT;
   }


   /* Find intersection point of lines */

   if ((m1 == m2) && (m1 != MAXFLOAT)) return 0;

   if ( (m1 != MAXFLOAT) && (m2 != MAXFLOAT) ) {
   /* neither line is a vertical line */
      *xint = (float)((b2-b1)/(m1-m2));
      if (m1 == 0)            /* if l1 is a horizontal line */
       *yint = l1.y1;
      else
      {
       if (m2 == 0)       /* if l2 is a horizontal line */
	*yint = l2.y1;
       else
	*yint = (float)(m1*(*xint)+b1);
      }

   } else {                   /* At least one vertical line */
      if (m1==m2) {           /* Both vertical lines */
	 if (l1.x1==l2.x1) {  /* Coincident vertical lines */
	    *xint = l1.x1;
            tempy1 = (float) min( l1.y1, l1.y2 );
            tempy2 = (float) min( l2.y1, l2.y2 );
	    *yint = (float) max( tempy1, tempy2 );
	 } else {             /* Non-coincident vertical lines */
	    return 0;
	 }
      } else if (m1==MAXFLOAT) {   /* l1 is vertical */
         if ( ((l2.x1 <= l1.x1) && (l2.x2 >= l1.x1)) ||
              ((l2.x2 <= l1.x1) && (l2.x1 >= l1.x1)) )
         {
	  *yint = (float)(m2*l1.x1 + b2);  /* l2 intersects l1 */
	  *xint = l1.x1;
         }
         else
          return 0;       /* l2 does not intersect l1 */

      } else {                     /* l2 is vertical */
         if ( ((l1.x1 <= l2.x1) && (l1.x2 >= l2.x1)) ||
              ((l1.x2 <= l2.x1) && (l1.x1 >= l2.x1)) )
         {
          *yint = (float)(m1*l2.x1 + b1);  /* l1 intersects l2 */
	  *xint = l2.x1;
         }
         else
          return 0;       /* l1 does not intersect l2 */

      }
   }

   /* See if intersection point lies on both line segments */
   return ( (*xint >= (float)min(l1.x1,l1.x2)) &&
	    (*xint <= (float)max(l1.x1,l1.x2)) &&
	    (*yint >= (float)min(l1.y1,l1.y2)) &&
	    (*yint <= (float)max(l1.y1,l1.y2)) &&
	    (*xint >= (float)min(l2.x1,l2.x2)) &&
	    (*xint <= (float)max(l2.x1,l2.x2)) &&
	    (*yint >= (float)min(l2.y1,l2.y2)) &&
	    (*yint <= (float)max(l2.y1,l2.y2)) );

}



/***************************************************************************
 *
 *N  intersect_polygon_edge
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    Count the number of intersections between a plum line from the
 *    test point and infinity and the vertices of the given edge.
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    x   <input> == (float) test point x coordinate.
 *    y   <input> == (float) test point y coordinate.
 *    edge_rec <input> == (edge_rec_type) given edge.
 *    intersect_polygon_edge <output> == (int) number of intersection points.
 *E
 **************************************************************************/
int intersect_polygon_edge( float x, float y, edge_rec_type edge_rec )
{
   register ossim_int32 i;
   line_segment_type lseg, pseg;
   int n;
   float xint,yint;
   coordinate_type coord1,coord2;

   lseg.x1 = x;
   lseg.y1 = y;
   lseg.x2 = MAXFLOAT/2.0;
   lseg.y2 = y;

   n = 0;

   coord1 = first_edge_coordinate(&edge_rec);
   for (i=1;i<edge_rec.npts;i++) {
      coord2 = next_edge_coordinate(&edge_rec);
      pseg.x1 = coord1.x;
      pseg.y1 = coord1.y;
      pseg.x2 = coord2.x;
      pseg.y2 = coord2.y;

      if (intersect(lseg,pseg,&xint,&yint)) {
	 n++;
      }

      coord1 = coord2;
   }
   return n;
}

/***************************************************************************
 *
 *N  intersect_polygon_loop
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    Count the number of intersections of a plum line from the test
 *    point to infinity and each of the edges in a polygon loop (ring).
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    x  <input> == (float) test point x coordinate.
 *    y  <input> == (float) test point y coordinate.
 *    face_id <input> == (int) given polygon loop.
 *    start_edge <input> == (ossim_int32) one edge of the polygon loop.
 *    edge_table <input> == (vpf_table_type) VPF edge table.
 *    intersect_polygon_loop <output> == (int) number of intersections.
 *E
 **************************************************************************/
int intersect_polygon_loop( float x, float y,
			    int face_id, ossim_int32 start_edge,
			    vpf_table_type edgetable )
{
   edge_rec_type edge_rec;
   ossim_int32 next, prevnode;
   VPF_BOOLEAN done = FALSE;
   int n;

   edge_rec = read_edge( start_edge, edgetable );

   edge_rec.dir = '+';

   n = intersect_polygon_edge( x, y, edge_rec );

   prevnode = edge_rec.start;

   next = next_polygon_edge(&edge_rec,&prevnode,face_id);

   if (edge_rec.coord) free(edge_rec.coord);

   while (!done) {
      if (next <= 0) done = TRUE;
      if (next == start_edge) done = TRUE;
      if (!done) {
	 edge_rec = read_edge( next, edgetable );

	 next = next_polygon_edge(&edge_rec,&prevnode,face_id);

	 n += intersect_polygon_edge( x, y, edge_rec );

	 if (edge_rec.coord) free(edge_rec.coord);
      }
   }

   return n;
}

/***************************************************************************
 *
 *N  point_in_face
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    Test whether a line (x,y through infinity) intersects a face (which
 *    itself is multiple polygon_loop or ring structures).  Returns a 0
 *    if outside, 1 if inside.
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    x         <input> == (float) x coordinate of given point.
 *    y         <input> == (float) y coordinate of given point.
 *    face_id   <input> == (ossim_int32) given face.
 *    facetable <input> == (vpf_table_type) VPF face table.
 *    ringtable <input> == (vpf_table_type) VPF ring table.
 *    edgetable <input> == (vpf_table_type) VPF edge table.
 *    point_in_face <output> == (int) VPF_BOOLEAN:
 *                                            1 --> inside
 *                                            0 --> outside
 *E
 **************************************************************************/
int point_in_face( float x, float y,
		   ossim_int32 face_id,
		   vpf_table_type facetable,
		   vpf_table_type ringtable,
		   vpf_table_type edgetable )
{
   face_rec_type face_rec;
   ring_rec_type ring_rec;
   int n;

   face_rec = read_face( face_id, facetable );
   ring_rec = read_ring( face_rec.ring, ringtable );
   n = intersect_polygon_loop( x, y, face_id, ring_rec.edge, edgetable );

   while (ring_rec.face == face_id ) {
      ring_rec = read_next_ring( ringtable );

      if (feof(ringtable.fp)) {
	 break;
      }

      if (ring_rec.face == face_id) {
	 n += intersect_polygon_loop( x, y, face_id,
				      ring_rec.edge, edgetable );
      }
   }

   if (n%2 == 0)   /* Even number of intersections */
      return 0;    /*   Not inside polygon         */
   else            /* Odd number of intersections  */
      return 1;    /*    Inside polygon            */
}



static void dirpath( char *path )
{
   register unsigned int i;

   i = (int)strlen(path)-1;
   while ( (i>0) && (path[i] != '\\') ) i--;
   if (i<(strlen(path)-1)) i++;
   path[i] = '\0';
}

/***************************************************************************
 *
 *N  point_in_face_table
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    Test whether a point is inside a face or not.  Must have a valid path-
 *   name to a valid VPF Face file, or else be in graphics mode for the error
 *   message that will result.  Filename must have all directory seperators
 *   escapes; eg. c:\\vpf    NOT c:\vpf
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *   Parameters:
 *A
 *    x        <input> == (float) given point x coordinate.
 *    y        <input> == (float) given point y coordinate.
 *    face_id  <input> == (ossim_int32) face.
 *    fname    <input> == (char *) pathname to VPF face file.
 *    point_in_face_table <output> == VPF_BOOLEAN:
 *                                            1 --> point is inside
 *                                            0 --> point is not inside
 *E
 **************************************************************************/
int point_in_face_table( float x, float y,
			 ossim_int32 face_id,
			 char *fname )
{
   vpf_table_type facetable, ringtable, edgetable;
   char *name;
   int result;

   name = (char *)vpfmalloc( 255*sizeof(char) );

   facetable = vpf_open_table(fname,disk, "rb", NULL );

   strupr(fname);

   strcpy( name, fname );
   dirpath( name );
   strcat( name, "RNG" );
   ringtable = vpf_open_table( name, disk , "rb", NULL );

   strcpy( name, fname );
   dirpath( name );
   strcat( name, "EDG" );
   edgetable = vpf_open_table( name, disk , "rb", NULL );

   free( name );

   result = point_in_face( x, y, face_id, facetable, ringtable, edgetable );

   vpf_close_table(&facetable);
   vpf_close_table(&ringtable);
   vpf_close_table(&edgetable);

   return result;
}

