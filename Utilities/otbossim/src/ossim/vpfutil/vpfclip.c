/*************************************************************************
 *
 *N  Module VPFCLIP
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This module geometrically clips an edge table to a given map
 *     extent and creates a new edge table.  It does not attempt to
 *     maintain topology, nor does it maintain its relationship to
 *     any feature tables associated with it.  Its intent is to be
 *     used only for the Library Reference coverage.  It may also serve
 *     as a basic starting point for a complete GIS clipping algorithm.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    N/A
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    Dec 1991                     DOS Turbo C
 *E
 *************************************************************************/


#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#if defined(__MSDOS__) || defined(__MINGW32__)
#  include <malloc.h>
#  include <mem.h>
#else
#  ifdef __APPLE__ 
#    include <sys/types.h>
#    include <sys/malloc.h>
#  else
#    ifndef __FreeBSD__
#      include <malloc.h>
#      include <string.h>
#    endif
#  endif
#endif

#include <ossim/vpfutil/vpftable.h>
#include <ossim/vpfutil/vpfview.h>
#include <ossim/vpfutil/vpfprim.h>
#  ifndef min
#    define min(a,b) ((a) <= (b) ? (a) : (b))
#  endif
#  ifndef max
#    define max(a,b) ((a) <= (b) ? (b) : (a))
#  endif

typedef struct {
   float x1, y1, x2, y2;
} line_segment_type;

/* Defined in VPFPTPLY.C */
int intersect( line_segment_type l1, line_segment_type l2,
	       float *xint, float *yint );

edge_rec_type create_edge_rec( row_type row, vpf_table_type edge_table );

/* Determine if a line segment intersects a box. 		   */
/* If so return the intersection coordinate in the parameter list. */
static int box_intersection( line_segment_type lseg,
			     extent_type extent,
			     coordinate_type *coord )
{
   line_segment_type boxseg;
   float xint, yint;

   boxseg.x1 = extent.x1;
   boxseg.y1 = extent.y1;
   boxseg.x2 = extent.x2;
   boxseg.y2 = extent.y1;
   if (intersect(lseg,boxseg,&xint,&yint)) {
      coord->x = xint;
      coord->y = yint;
      return TRUE;
   }

   boxseg.x1 = extent.x2;
   boxseg.y1 = extent.y1;
   boxseg.x2 = extent.x2;
   boxseg.y2 = extent.y2;
   if (intersect(lseg,boxseg,&xint,&yint)) {
      coord->x = xint;
      coord->y = yint;
      return TRUE;
   }

   boxseg.x1 = extent.x2;
   boxseg.y1 = extent.y2;
   boxseg.x2 = extent.x1;
   boxseg.y2 = extent.y2;
   if (intersect(lseg,boxseg,&xint,&yint)) {
      coord->x = xint;
      coord->y = yint;
      return TRUE;
   }

   boxseg.x1 = extent.x1;
   boxseg.y1 = extent.y2;
   boxseg.x2 = extent.x1;
   boxseg.y2 = extent.y1;
   if (intersect(lseg,boxseg,&xint,&yint)) {
      coord->x = xint;
      coord->y = yint;
      return TRUE;
   }
   return FALSE;
}

/* Replace the given coordinate string into the given edge row */
/* and write the new row to the specified edge table.	       */
static void write_edge_record( ossim_int32 id, coordinate_type *coord,
			       ossim_int32 ncoord, row_type row,
			       vpf_table_type *table )
{
   int ID_, COORD_;

   ID_ = table_pos("ID",*table);
   COORD_ = table_pos("COORDINATES",*table);

   put_table_element(ID_, row, *table, &id, 1);
   put_table_element(COORD_, row, *table, coord, ncoord);
   write_next_row(row,table);

}



#define WITHIN(x,y,ext) ((x>=ext.x1)&&(x<=ext.x2)&&(y>=ext.y1)&&(y<=ext.y2))

/*************************************************************************
 *
 *N  vpf_edge_clip
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function clips an edge table to the given extent and writes
 *     the clipped table to the specified output path.
 *     NOTE: If a segment of an edge in the EDG table has both a beginning
 *     point and ending point outside of the given extent, and the line
 *     segment passes through the extent, the segment will not be written
 *     out to the new edge table.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    covpath <input> == (char *) path to the coverage of the edge table.
 *    extent  <input> == (extent_type) clipping extent.
 *    outpath <input> == (char *) output path for the clipped edge table.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    Dec 1991      Original Version    DOS Turbo C
 *E
 *************************************************************************/
void vpf_edge_clip( char *covpath, extent_type extent, char *outpath )
{
   vpf_table_type in, out;
   row_type row;
   edge_rec_type inedge, outedge;
   ossim_int32 i,j,n,id;
   line_segment_type lseg;
   coordinate_type coord;
   char path[255], *def;
   float xmin, xmax, ymin, ymax;

   /* Standardize extent, Lower Left, Upper Right */
   xmin = (float)min(extent.x1,extent.x2);
   xmax = (float)max(extent.x1,extent.x2);
   ymin = (float)min(extent.y1,extent.y2);
   ymax = (float)max(extent.y1,extent.y2);
   extent.x1 = xmin;
   extent.y1 = ymin;
   extent.x2 = xmax;
   extent.y2 = ymax;

   sprintf(path,"%sedg",covpath);
   in = vpf_open_table(path,disk,"rb",NULL);

   rewind(in.fp);
   fread(&n,sizeof(n),1,in.fp);
   def = (char *)vpfmalloc((n+1)*sizeof(char));
   fread(def,sizeof(char),n,in.fp);
   def[n] = '\0';
   sprintf(path,"%sedg",outpath);
   out = vpf_open_table(path,disk,"wb",def);

   for (i=1,id=1;i<=in.nrows;i++) {
      row = get_row( i, in );
      inedge = create_edge_rec( row, in );
      if (!inedge.coord) {
	 free_row(row,in);
	 continue;
      }
      outedge.coord = (coordinate_type *)malloc(inedge.npts*sizeof(
						coordinate_type));
      if (!outedge.coord) {
	 free(inedge.coord);
	 free_row(row,in);
	 continue;
      }

      n = 0;

      for (j=0;j<inedge.npts;j++) {

	 if (WITHIN(inedge.coord[j].x,inedge.coord[j].y,extent)) {
	    /* Current coordinate within extent */

	    if (j > 0) {
	       if (!WITHIN(inedge.coord[j-1].x,inedge.coord[j-1].y,
			    extent)) {
		  /* Previous coordinate was not in the extent - */
		  /* create an intersection vertex */

		  lseg.x1 = inedge.coord[j-1].x;
		  lseg.y1 = inedge.coord[j-1].y;
		  lseg.x2 = inedge.coord[j].x;
		  lseg.y2 = inedge.coord[j].y;
		  if (box_intersection(lseg,extent,&coord)) {
		     outedge.coord[n].x = coord.x;
		     outedge.coord[n].y = coord.y;
		  } else {
		     outedge.coord[n].x = lseg.x1;
		     outedge.coord[n].y = lseg.y1;
		  }
		  n++;
	       }
	    }
	    outedge.coord[n].x = inedge.coord[j].x;
	    outedge.coord[n].y = inedge.coord[j].y;
	    n++;
	 } else {
	    if (j > 0) {
	       if (WITHIN(inedge.coord[j-1].x,inedge.coord[j-1].y,extent)) {

		  /* The coordinate is not within the extent and     */
		  /* the previous one was - Create an intersection   */
		  /* vertex point and write the current edge record. */

		  lseg.x1 = inedge.coord[j-1].x;
		  lseg.y1 = inedge.coord[j-1].y;
		  lseg.x2 = inedge.coord[j].x;
		  lseg.y2 = inedge.coord[j].y;
		  if (box_intersection(lseg,extent,&coord)) {
		     outedge.coord[n].x = coord.x;
		     outedge.coord[n].y = coord.y;
		  } else {
		     outedge.coord[n].x = lseg.x2;
                     outedge.coord[n].y = lseg.y2;
		  }

		  /* Break the edge in (at least) two */
		  n++;
		  write_edge_record( id, outedge.coord, n, row, &out );
		  id++;
		  n=0;

	       }
	    }
	 }
      }
      free(inedge.coord);

      if (n > 0) {
	 write_edge_record( id, outedge.coord, n, row, &out );
	 id++;
      }

      free(outedge.coord);
      free_row(row,in);
   }

   vpf_close_table(&in);
   vpf_close_table(&out);
}

