/*************************************************************************
 *
 *N  Module VPFDRAW - DRAW VPF FEATURES
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This module contains functions for drawing selected VPF
 *     features.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *     N/A
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   Nov 1991                           DOS Turbo C
 *E
 *************************************************************************/

#include <stdio.h>
#ifdef __MSDOS__
#include <io.h>
#endif
#include <stdlib.h>
#include <string.h>
#ifdef __MSDOS__
#include <alloc.h>
#include <mem.h>
#endif
#include <math.h>
#if defined(__CYGWIN__) || defined(__APPLE__) || defined(_WIN32)
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
#ifdef __MSDOS__
#include <ossim/vpfutil/mapgraph.h>
#include <ossim/vpfutil/projectn.h>
#endif
#include <ossim/vpfutil/polygrf.h>
#ifdef __MSDOS__
#include "grprim.h"
#endif
#include <ossim/vpfutil/vpfrelat.h>
#include <ossim/vpfutil/vpfdraw.h>


extern char home[255];


typedef struct {
   float x1,y1,x2,y2;
} line_segment_type;
#ifdef __MSDOS__
/*************************************************************************
 *
 *N  draw_edge_coordinates
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function draws the coordinates of an edge record.
 *     It tries its darndest to detect and get rid of projection
 *     "zingers" - lines that dart across the screen because of
 *     lines projected onto the map that wrap around behind the
 *     globe or change hemispheres.  The ability to handle these
 *     projection artifacts is what turns what should be a very
 *     simple function into a very ugly piece of code.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *     edge_rec <input>==(edge_rec_type *) pointer to an edge record.
 *     return  <output>==(int) 0 if the user escapes, 1 upon successful
 *                          completion.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   Feb 1992                           DOS Turbo C
 *E
 *************************************************************************/
int draw_edge_coordinates( edge_rec_type *edge_rec )
{
   int xscr,yscr, xprev,yprev, x1,y1,x2,y2, mbrx2, wrap;
   register ossim_int32 i;
   extent_type mbr;
   coordinate_type coord;

/* First compute the MBR from the coordinates. */
   coord = first_edge_coordinate(edge_rec);
   mbr.x1 = coord.x;
   mbr.y1 = coord.y;
   mbr.x2 = coord.x;
   mbr.y2 = coord.y;
   for (i=1;i<edge_rec->npts;i++) {
      coord = next_edge_coordinate(edge_rec);
      if (coord.x < mbr.x1) mbr.x1 = coord.x;
      if (coord.y < mbr.y1) mbr.y1 = coord.y;
      if (coord.x > mbr.x2) mbr.x2 = coord.x;
      if (coord.y > mbr.y2) mbr.y2 = coord.y;
   }
   screen_bounds(mbr.x1,mbr.y1,mbr.x2,mbr.y2,&x1,&y1,&x2,&y2);

   if ( (x1 < 0 && x2 < 0) || (x1 > gpgetmaxx() && x2 > gpgetmaxx()) ||
	(y1 < 0 && y2 < 0) || (y1 > gpgetmaxy() && y2 > gpgetmaxy()) )
      return 1;
   if (x1 > gpgetmaxx() && x2 < 0) return 1;

   wrap = 0;
   mbrx2 = x2;
   if (x2 < x1) {
      /* The box wraps around the screen */
      /* (or at least wraps off the edge of the screen) */
      wrap = gpgetmaxx();
      if (x2 < 0) wrap -= x2;
      /* Adjust the maximum screen value */
      x2 += wrap;
   }

   coord = first_edge_coordinate(edge_rec);
   screenxy(coord.x,coord.y,&xscr,&yscr);
   if (wrap && xscr < x1) xscr += wrap;
   gpmoveto(xscr,yscr);
   xprev = xscr;
   yprev = yscr;
   for (i=1;i<edge_rec->npts;i++) {
      coord = next_edge_coordinate(edge_rec);
      screenxy(coord.x,coord.y,&xscr,&yscr);
      if (xscr == MAXINT || yscr == MAXINT) {
	 xprev = xscr;
	 yprev = yscr;
	 continue;
      }

      if (wrap && xscr < x1) xscr += wrap;

      if (xscr==xprev && yscr==yprev) continue;

      if (xprev == MAXINT || yprev == MAXINT) gpmoveto(xscr,yscr);

      hidemousecursor();
      gplineto( xscr, yscr );
      showmousecursor();

      xprev = xscr;
      yprev = yscr;

      while (kbhit()) {
	 if (getch()==27) {
	    return 0;
	 }
      }

   }

   if (wrap && mbrx2 > 0) {
      /* The edge wraps around and is displayed on the other */
      /* edge of the screen. */
      x2 -= wrap;

      coord = first_edge_coordinate(edge_rec);
      screenxy(coord.x,coord.y,&xscr,&yscr);
      if (xscr > x2) xscr -= wrap;
      gpmoveto(xscr,yscr);
      xprev = xscr;
      yprev = yscr;
      for (i=1;i<edge_rec->npts;i++) {
	 coord = next_edge_coordinate(edge_rec);
	 screenxy(coord.x,coord.y,&xscr,&yscr);
	 if (xscr == MAXINT || yscr == MAXINT) {
	    xprev = xscr;
	    yprev = yscr;
	    continue;
	 }

	 if (xscr > x2) xscr -= wrap;

	 if (xscr==xprev && yscr==yprev) continue;

	 if (xprev == MAXINT || yprev == MAXINT) gpmoveto(xscr,yscr);

	 hidemousecursor();
	 gplineto( xscr, yscr );
	 showmousecursor();

	 xprev = xscr;
	 yprev = yscr;

	 while (kbhit()) {
	    if (getch()==27) {
	       return 0;
	    }
	 }
      }
   }

   return 1;
}


/*************************************************************************
 *
 *N  draw_edge_row
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function draws the selected edge for a given row in
 *     the edge primitive table.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *     row      <input>==(row_type)row in the edge primitive table.
 *     table    <input>==(vpf_table_type) edge table.
 *     return  <output>==(int) 0 if the user escapes, 1 upon successful
 *                          completion.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   August 1991                           DOS Turbo C
 *E
 *************************************************************************/
int draw_edge_row( row_type row, vpf_table_type table )
{
   edge_rec_type edge_rec;
   int stat;

   edge_rec = create_edge_rec(row,table);

   stat = draw_edge_coordinates(&edge_rec);

   if (edge_rec.coord) free(edge_rec.coord);

   gpstroke();

   while (kbhit()) {
      if (getch()==27) {
	 return 0;
      }
   }

   return stat;
}


/*************************************************************************
 *
 *N  draw_edge
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function draws the selected edge for a given row number in
 *     the edge primitive table.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *     rownum   <input>==(ossim_int32)row number of the edge primitive table.
 *     table    <input>==(vpf_table_type) edge table.
 *     return  <output>==(int) 0 if the user escapes, 1 upon successful
 *                          completion.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   August 1991                           DOS Turbo C
 *E
 *************************************************************************/
int draw_edge( ossim_int32 rownum, vpf_table_type table )
{
   int stat;
   row_type row;

   row = get_row( rownum, table );
   stat = draw_edge_row(row,table);
   free_row(row,table);
   return stat;
}



/*************************************************************************
 *
 *N  draw_point_row
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function draws an entity node for the row in the given
 *     entity node primitive table.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *     row         <input>==(row_type) row of the entity node
 *                          primitive table.
 *     table       <input>==(vpf_table_type) entity node primitive table.
 *     return     <output>==(int) 0 if the user escapes, 1 upon successful
 *                          completion.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   August 1991                        DOS Turbo C
 *E
 *************************************************************************/
int draw_point_row( row_type row, vpf_table_type table )
{
   int xscr,yscr, coord_pos;
   ossim_int32 n;
   coordinate_type pnt;

   coord_pos = table_pos("COORDINATE",table);
   get_table_element(coord_pos,row,table,&pnt,&n);

   screenxy(pnt.x,pnt.y,&xscr,&yscr);
   hidemousecursor();
   gpmarker(xscr,yscr);
   showmousecursor();

   while (kbhit()) {
      if (getch()==27) {
	 return 0;
      }
   }

   return 1;
}

/*************************************************************************
 *
 *N  draw_point
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function draws an entity node for the row number of the given
 *     entity node primitive table.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *     rownum      <input>==(ossim_int32) row number of the entity node
 *                          primitive table.
 *     table       <input>==(vpf_table_type) entity noe primitive table.
 *     return     <output>==(int) 0 if the user escapes, 1 upon successful
 *                          completion.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   August 1991                        DOS Turbo C
 *E
 *************************************************************************/
int draw_point( ossim_int32 rownum, vpf_table_type table )
{
   int stat;
   row_type row;

   row = get_row( rownum, table );
   stat = draw_point_row(row,table);
   free_row(row,table);
   return stat;
}



/*************************************************************************
 *
 *N  draw_text_row
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function draws annotation for a given row in the text
 *     pseudo-primitive table.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *     row         <input>==(row_type) row of the text table.
 *     table       <input>==(vpf_table_type) text primitive table.
 *     return     <output>==(int) 0 if the user escapes, 1 upon successful
 *                          completion.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   August 1991                        DOS Turbo C
 *E
 *************************************************************************/
int draw_text_row( row_type row, vpf_table_type table )
{
   int xscr,yscr, shape_pos, text_pos;
   ossim_int32 n, nshape;
   coordinate_type *shape_line;
   char *text;
   struct viewporttype vp;

   shape_pos = table_pos("SHAPE_LINE",table);
/**** MUST CHECK FOR 'Z', 'B', AND 'Y' TYPES - SEE VPFPRIM.C ****/
   shape_line = (coordinate_type *)get_table_element(shape_pos,row,table,
						     NULL,&nshape);
   text_pos = table_pos("STRING",table);
   text = (char *)get_table_element(text_pos,row,table,NULL,&n);

   screenxy(shape_line[0].x,shape_line[0].y,&xscr,&yscr);

   getviewsettings(&vp);
   if ((gpgetfont() != DEFAULT_FONT) || (yscr > gptextheight(text))) {
      hidemousecursor();
      gptext(xscr,yscr,text);
      showmousecursor();
   }

   free(shape_line);
   free(text);

   while (kbhit()) {
      if (getch()==27) {
	 return 0;
      }
   }

   return 1;
}

/*************************************************************************
 *
 *N  draw_text
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function draws annotation for a given row number in the text
 *     pseudo-primitive table.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *     rownum      <input>==(ossim_int32) row number of the text table.
 *     table       <input>==(vpf_table_type) text primitive table.
 *     return     <output>==(int) 0 if the user escapes, 1 upon successful
 *                          completion.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   August 1991                        DOS Turbo C
 *E
 *************************************************************************/
int draw_text( ossim_int32 rownum, vpf_table_type table )
{
   int stat;
   row_type row;

   row = get_row( rownum, table );
   stat = draw_text_row(row,table);
   free_row(row,table);
   return stat;
}



static IPoint make_ipoint( int x, int y )
{
   IPoint point;

   point.x = x;
   point.y = y;
   return point;
}

static IBox make_ibox( int x1, int y1, int x2, int y2 )
{
   IBox box;

   if (x1 < x2)
      box.origin.x = x1;
   else
      box.origin.x = x2;
   if (y1 < y2)
      box.origin.y = y1;
   else
      box.origin.y = y2;
   box.delta.x = abs(x2-x1);
   box.delta.y = abs(y2-y1);
   return box;
}


/*************************************************************************
 *
 *N  screen_bounds
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function determines the screen bounds of a rectangular
 *     map area, independent of any projection that may distort the
 *     boundaries.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *     x1    <input>==(double) lower left x map coordinate.
 *     y1    <input>==(double) lower left y map coordinate.
 *     x2    <input>==(double) upper right x map coordinate.
 *     y2    <input>==(double) upper right y map coordinate.
 *     xmin <output>==(int *) lower left x screen coordinate.
 *     ymin <output>==(int *) lower left y screen coordinate.
 *     xmax <output>==(int *) upper right x screen coordinate.
 *     ymax <output>==(int *) upper right y screen coordinate.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   May 1991                           DOS Turbo C
 *E
 *************************************************************************/
void screen_bounds( double x1, double y1, double x2, double y2,
		    int *xmin, int *ymin, int *xmax, int *ymax )
{
   int xscr,yscr;
   double xc,yc;        /* x center and y center */

   xc = (x1+x2)/2.0;
   yc = (y1+y2)/2.0;

   /* Find xmin from x1 */
   screenxy(x1,y1,&xscr,&yscr);
   *xmin = xscr;
   screenxy(x1,yc,&xscr,&yscr);
   if (xscr < (*xmin)) *xmin = xscr;
   screenxy(x1,y2,&xscr,&yscr);
   if (xscr < (*xmin)) *xmin = xscr;

   /* Find xmax from x2 */
   screenxy(x2,y2,&xscr,&yscr);
   *xmax = xscr;
   screenxy(x2,yc,&xscr,&yscr);
   if (xscr > (*xmax)) *xmax = xscr;
   screenxy(x2,y1,&xscr,&yscr);
   if (xscr > (*xmax)) *xmax = xscr;

   /* Find ymin from y2 (seems backwards - but so is the screen) */
   screenxy(x1,y2,&xscr,&yscr);
   *ymin = yscr;
   screenxy(xc,y2,&xscr,&yscr);
   if (yscr < (*ymin)) *ymin = yscr;
   screenxy(x2,y2,&xscr,&yscr);
   if (yscr < (*ymin)) *ymin = yscr;

   /* Find ymax from y1 */
   screenxy(x1,y1,&xscr,&yscr);
   *ymax = yscr;
   screenxy(xc,y1,&xscr,&yscr);
   if (yscr > (*ymax)) *ymax = yscr;
   screenxy(x2,y1,&xscr,&yscr);
   if (yscr > (*ymax)) *ymax = yscr;

}


/*************************************************************************
 *
 *N  gen_polygon_edge
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function generates a polygon edge in the Shademap structure
 *     given an edge record structure.
 *     It is complicated by having to check whether this polygon
 *     has wrapped around the screen from the projection and adjusting
 *     the coordinates, if so.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *     edge_rec <input>==(edge_rec_type) input edge record.
 *     poly     <inout>==(Shademap *) polygon structure being updated.
 *     wrap     <input>==(int) amount of wrap around if the polygon
 *                        wraps around the screen.  If this number
 *                        is positive, the coordinates will be
 *                        adjusted out to the right.  If negative,
 *                        they will adjusted to the left.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   May 1991                           DOS Turbo C
 *E
 *************************************************************************/
void gen_polygon_edge( edge_rec_type edge_rec,
		       Shademap *poly,
		       int wrap )
{
   register ossim_int32 i,start,end,inc;
   int xscr,yscr;
   IPoint pnt, make_ipoint();
   coordinate_type coord;

   if (edge_rec.left == edge_rec.right) return;

   if (edge_rec.dir == '-') {
      start = edge_rec.npts-1;
      end = 0;
      inc = -1;
   } else {
      start = 0;
      end = edge_rec.npts-1;
      inc = 1;
   }
   for (i=start;i!=end;i+=inc) {
      coord = get_edge_coordinate(i,&edge_rec);
      screenxy(coord.x,coord.y,&xscr,&yscr);

      if (wrap > 0 && xscr < poly->bounding.origin.x)
	 xscr += wrap;

      if (wrap < 0 &&
	  xscr > poly->bounding.origin.x+poly->bounding.delta.x)
	 xscr += wrap;

      pnt = make_ipoint(xscr,yscr);
      gpwriteshade( poly, pnt );
   }
   coord = get_edge_coordinate(i,&edge_rec);
   screenxy(coord.x,coord.y,&xscr,&yscr);

   if (wrap > 0 && xscr < poly->bounding.origin.x)
      xscr += wrap;

   if (wrap < 0 &&
       xscr > poly->bounding.origin.x+poly->bounding.delta.x)
      xscr += wrap;

   pnt = make_ipoint(xscr,yscr);
   gpwriteshade( poly, pnt );
}


/*************************************************************************
 *
 *N  draw_polygon_edge
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function draws an edge of a polygon.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *     edge_rec <input>==(edge_rec_type) input edge record.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   May 1991                           DOS Turbo C
 *E
 *************************************************************************/
void draw_polygon_edge( edge_rec_type edge_rec )
{
   if (edge_rec.left == edge_rec.right) return;
   draw_edge_coordinates(&edge_rec);
}



#ifdef TRACE
/*************************************************************************
 *
 *N  trace_polygon_loop
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function traces a loop (or ring) of a polygon by chaining
 *     through the right and/or left edges of the edge table, including
 *     tracing through all dangles.  This is winged-edge topology in action.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *     face_id    <input>==(ossim_int32) row id of the specified face.
 *     start_edge <input>==(ossim_int32) row id of the edge that starts the
 *                         loop.
 *     edgetable  <input>==(vpf_table_type) VPF edge primitive table.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   May 1991                           DOS Turbo C
 *E
 *************************************************************************/
void trace_polygon_loop(ossim_int32 face_id,
			ossim_int32 start_edge,
			vpf_table_type edgetable )
{
  edge_rec_type edge_rec;
  ossim_int32 next, prevnode;
  boolean done=FALSE;
  IPoint make_ipoint();

  edge_rec = read_edge( start_edge, edgetable );

#ifdef WATCH
   gotoxy(1,17);
   printf("edge: %5ld  left: %5ld  edge:%5ld  right: %5ld  edge: %5ld",
	  edge_rec.id,edge_rec.left,edge_rec.leftfwd,edge_rec.right,
	  edge_rec.rightfwd);
#endif
#ifdef STEP
   getch();
#endif

  edge_rec.dir = '+';

  prevnode = edge_rec.start;

  if (edge_rec.start != edge_rec.end) {

     if ( (edge_rec.right == face_id) && (edge_rec.left == face_id) ) {
	/* Dangle */
	if (prevnode == edge_rec.start) {
	   edge_rec.dir = '+';
	   next = edge_rec.rightfwd;
	   prevnode = edge_rec.end;
	} else if (prevnode == edge_rec.end) {
	   edge_rec.dir = '-';
	   next = edge_rec.leftfwd;
	   prevnode = edge_rec.start;
	} else next = -1;
     } else if (edge_rec.right == face_id) {
	next = edge_rec.rightfwd;
	edge_rec.dir = '+';
	prevnode = edge_rec.end;
     } else if (edge_rec.left == face_id) {
	next = edge_rec.leftfwd;
	edge_rec.dir = '-';
	prevnode = edge_rec.start;
     } else next = -1;

  } else {

     done = TRUE;

  }

  draw_polygon_edge( edge_rec );

  if (edge_rec.coord) free(edge_rec.coord);

  while (!done) {

     if (next < 0) {
#ifdef WATCH
	gotoxy(1,18);
	printf(" Aaargh(r)! Edge: %ld   face: %ld   left: %ld   right: %ld",
		edge_rec.id,face_id,edge_rec.left,edge_rec.right);
	getch();
	gotoxy(1,18);
	printf("                                                                                ");
#endif
	done = TRUE;
     }

     if (next==0) {
#ifdef WATCH
	gotoxy(1,19);
	printf("Next edge(%d) = 0  ",edge_rec.id);
#endif
	done = TRUE;
     }

     if (next == start_edge) done = TRUE;

     if (!done) {

	edge_rec = read_edge( next, edgetable );

#ifdef WATCH
	gotoxy(1,17);
	printf("edge: %5ld  left: %5ld  edge:%5ld  right: %5ld  edge: %5ld",
	       next,edge_rec.left,edge_rec.leftfwd,edge_rec.right,
	       edge_rec.rightfwd);
	gotoxy(1,18);
	printf("node: %5ld  start: %5ld  end: %5ld",
	prevnode,edge_rec.start,edge_rec.end);
#endif
#ifdef STEP
	getch();
#endif

	if ( (edge_rec.right == face_id) && (edge_rec.left == face_id) ) {
	   /* Dangle */
	   if (prevnode == edge_rec.start) {
	      edge_rec.dir = '+';
	      next = edge_rec.rightfwd;
	      prevnode = edge_rec.end;
	   } else if (prevnode == edge_rec.end) {
	      edge_rec.dir = '-';
	      next = edge_rec.leftfwd;
	      prevnode = edge_rec.start;
	   } else next = -1;
	} else if (edge_rec.right == face_id) {
	   edge_rec.dir = '+';
	   next = edge_rec.rightfwd;
	   prevnode = edge_rec.end;
	} else if (edge_rec.left == face_id) {
	   edge_rec.dir = '-';
	   next = edge_rec.leftfwd;
	   prevnode = edge_rec.start;
	} else next = -1;

	draw_polygon_edge( edge_rec );
	if (edge_rec.coord) free(edge_rec.coord);

     }
  }

}
#endif

#endif
/*************************************************************************
 *
 *N  next_polygon_edge
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function determines the next polygon edge id for a specified
 *     face from the given edge record and face id.  It follows the rules
 *     of winged-edge topology.  It does not follow the network through
 *     dangles (see trace_polygon_loop above).  It skips over them to
 *     stay along the ring boundary.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *     edge_rec   <input>==(edge_rec_type *) VPF edge record.
 *     prev_node  <inout>==(ossim_int32 *) the previous node visited.  This
 *                         value is updated by this function.
 *     face_id    <input>==(ossim_int32) row id of the specified face.
 *     next_polygon_edge <output>==(ossim_int32) the next edge id.  A value of -1
 *                         indicates a topology error.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   May 1991                           DOS Turbo C
 *E
 *************************************************************************/
ossim_int32 next_polygon_edge( edge_rec_type *edge_rec, ossim_int32 *prevnode,
			    ossim_int32 face_id )
{
   ossim_int32 next;

   if ( (edge_rec->right == face_id) && (edge_rec->left == face_id) ) {
      /* Dangle - go the opposite direction to continue along the boundary */
      if (*prevnode == edge_rec->start) {
	 edge_rec->dir = '-';
	 next = edge_rec->leftfwd;
	 *prevnode = edge_rec->start;
      } else if (*prevnode == edge_rec->end) {
	 edge_rec->dir = '+';
	 next = edge_rec->rightfwd;
	 *prevnode = edge_rec->end;
      } else next = -1;
   } else if (edge_rec->right == face_id) {
      /* The face is on the right - take the right forward edge */
      next = edge_rec->rightfwd;
      edge_rec->dir = '+';
      *prevnode = edge_rec->end;
   } else if (edge_rec->left == face_id) {
      /* The face is on the left - take the left forward edge */
      next = edge_rec->leftfwd;
      edge_rec->dir = '-';
      *prevnode = edge_rec->start;
   } else next = -1;

   return next;
}

#ifdef __MSDOS__
/*************************************************************************
 *
 *N  draw_polygon_loop
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function draws a loop (or ring) of a polygon by chaining
 *     through the right and/or left edges of the edge table.  Dangles
 *     are skipped to stay along the outer boundary.
 *     This is a good example of winged-edge topology.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *     face_id    <input>==(ossim_int32) row id of the specified face.
 *     start_edge <input>==(ossim_int32) row id of the edge that starts the
 *                         loop.
 *     edgetable  <input>==(vpf_table_type) VPF edge primitive table.
 *     poly       <inout>==(Shademap *) polygon shade structure.
 *     wrap       <input>==(int) wrap around if the polygon wraps
 *                         around the screen.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   May 1991                           DOS Turbo C
 *E
 *************************************************************************/
void draw_polygon_loop(ossim_int32 face_id,
		       ossim_int32 start_edge,
		       vpf_table_type edgetable,
		       Shademap *poly,
		       int wrap )
{
  edge_rec_type edge_rec;
  ossim_int32 next, prevnode;
  boolean done=FALSE;
  IPoint make_ipoint();
  int xscr,yscr, color;
  coordinate_type coord;

  edge_rec = read_edge( start_edge, edgetable );

  edge_rec.dir = '+';

  prevnode = edge_rec.start;

  if (edge_rec.start != edge_rec.end) {

     next = next_polygon_edge( &edge_rec, &prevnode, face_id );

  } else {

     done = TRUE;

  }

  gpgetlinecolor(&color);
  if (gpgetdevice()==SCREEN && color>=0)
     draw_polygon_edge( edge_rec );

  if (edge_rec.dir == '-')
     coord = get_edge_coordinate(edge_rec.npts-1,&edge_rec);
  else
     coord = get_edge_coordinate(0,&edge_rec);
  screenxy(coord.x,coord.y,&xscr,&yscr);
  gpmoveto(xscr,yscr);

  gen_polygon_edge( edge_rec, poly, wrap );

  if (edge_rec.coord) free(edge_rec.coord);

  while (!done) {

     if (next < 0) {
	gotoxy(1,18);
	printf(" VPF topology error! Edge: %ld   face: %ld   left: %ld   right: %ld",
		edge_rec.id,face_id,edge_rec.left,edge_rec.right);
	getch();
	gotoxy(1,18);
	printf("                                                                                ");
	done = TRUE;
     }

     if (next==0) {
	gotoxy(1,19);
	printf("Next edge(%d) = 0  ",edge_rec.id);
	done = TRUE;
     }

     if (next == start_edge) done = TRUE;

     if (!done) {

	edge_rec = read_edge( next, edgetable );

	next = next_polygon_edge( &edge_rec, &prevnode, face_id );

	if (gpgetdevice() == SCREEN && color>=0)
	   draw_polygon_edge( edge_rec );

	gen_polygon_edge( edge_rec, poly, wrap );
	if (edge_rec.coord) free(edge_rec.coord);

     }
  }

}



/*************************************************************************
 *
 *N  draw_polygon
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function draws a polygon for VPF face structure tables.
 *     It is complicated by the fact that it checks to see whether
 *     enough memory is available to draw the entire polygon in a
 *     Shademap structure, or whether the face must be broken up into
 *     smaller chunks so the bit arrays can fit into memory.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    mbrx1      <input> == (int) mbr corner 1 x coordinate.
 *    mbry1      <input> == (int) mbr corner 1 y coordinate.
 *    mbrx2      <input> == (int) mbr corner 2 x coordinate.
 *    mbry2      <input> == (int) mbr corner 2 y coordinate.
 *    face_rec   <input> == (face_rec_type)
 *    ringtable  <input> == (vpf_table_type) VPF ring table.
 *    edgetable  <input> == (vpf_table_type) VPF edge table.
 *    outline    <input> == (color_type) polygon outline color.
 *    pattern    <input> == (Pattern) polygon fill pattern.
 *    wrap       <input> == (int) wrap around if the polygon wraps around
 *                          the screen.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   May 1991                           DOS Turbo C
 *E
 *************************************************************************/
void draw_polygon( int mbrx1, int mbry1, int mbrx2, int mbry2,
		   face_rec_type face_rec,
		   vpf_table_type ringtable,
		   vpf_table_type edgetable,
		   color_type outline,
		   Pattern pattern,
		   int wrap )
{
   int  x1,y1,x2,y2,  nchunks,chunk,delta, ok, k;
   Shademap *poly;
   IBox     bounding,wdw,make_ibox();
   ring_rec_type ring_rec;

   x1 = mbrx1-2; y1 = mbry1-2; x2 = mbrx2+2; y2 = mbry2+2;
   delta = x2-x1;

   nchunks=1;

   wdw = make_ibox(0,0,gpgetmaxx()+1,gpgetmaxy());
   bounding = make_ibox(x1,y1,x2,y2);
   poly = OpenShade( bounding, wdw, outline, pattern);
   if (!poly) return;

   if (gpgetdevice() == SCREEN) {
/*      wdw = make_ibox(0,0,getmaxx()+1,getmaxy());
      bounding = make_ibox(x1,y1,x2,y2);
      poly = OpenShade( bounding, wdw, outline, pattern);
      if (!poly) return;  */
      ok = TRUE;
      if (!poly->Bordermap.array || !poly->Maskmap.array) {
	 /* Not enough memory - Split polygon up */
	 if (poly->Bordermap.array) FreeArrayOfBits(poly->Bordermap);
	 if (poly->Maskmap.array) FreeArrayOfBits(poly->Maskmap);
	 free(poly);
	 poly = (Shademap *)NULL;
	 k = 2;
	 delta = gpgetmaxx()/k;
	 nchunks = (mbrx2-mbrx1)/delta + 1;
	 ok = FALSE;
	 while (!ok) {
	    ok = TRUE;
	    bounding = make_ibox(0,y1,delta,y2);
	    poly = OpenShade( bounding, wdw, outline, pattern);
	    if (!poly) return;
	    if (!poly->Bordermap.array || !poly->Maskmap.array) {
	       ok = FALSE;
	       if (poly->Bordermap.array) FreeArrayOfBits(poly->Bordermap);
	       if (poly->Maskmap.array) FreeArrayOfBits(poly->Maskmap);
	       free(poly);
	       k++;
	       delta = gpgetmaxx()/k;
	       if (delta < 5) return;
	       nchunks = (mbrx2-mbrx1)/delta + 1;
	    } else {
	       FreeArrayOfBits(poly->Bordermap);
	       FreeArrayOfBits(poly->Maskmap);
	       free(poly);
	       poly = (Shademap *)NULL;
	    }
	 }
	 x2 = x1 + delta;
	 bounding = make_ibox(x1,y1,x2,y2);
	 poly = OpenShade(bounding,wdw,outline,pattern);
      }
   }

   for (chunk=0;chunk<nchunks;chunk++) {

      ring_rec = read_ring( face_rec.ring, ringtable );

      draw_polygon_loop( face_rec.id, ring_rec.edge, edgetable, poly,
			 wrap );
      gpcloseloop(poly);

      while (ring_rec.face == face_rec.id ) {
	 ring_rec = read_next_ring( ringtable );

	 if (feof(ringtable.fp)) {
	    gpcloseloop(poly);
	    break;
	 }

	 if (ring_rec.face == face_rec.id) {

	    draw_polygon_loop( face_rec.id, ring_rec.edge, edgetable,
			       poly, wrap );
	    gpcloseloop(poly);

	 }
      }
      hidemousecursor();
      gpfillpoly(poly);
      showmousecursor();

      if (gpgetdevice()==SCREEN) {
	 poly = (Shademap *)NULL;
	 if (x2 < mbrx2 && chunk < (nchunks)) {
	    x1 = x2;
	    x2 += delta;
	    bounding = make_ibox(x1,y1,x2,y2);
	    poly = OpenShade(bounding,wdw,outline,pattern);
	 }
      }
   }

   if (poly) {
      if (gpgetdevice()==SCREEN)
	 CloseShade(poly);
      else {
	 FreeArrayOfBits(poly->Bordermap);
	 FreeArrayOfBits(poly->Maskmap);
	 free(poly);
      }
   }

}

/*************************************************************************
 *
 *N  draw_face_row
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function draws the specified face from the given row of
 *     the given face table.
 *     It is complicated by the fact that it checks to see whether a
 *     face has wrapped around the screen due to projection effects.
 *     If so, it first adjusts the coordinates off the right edge of the
 *     screen to fill that portion of the face.  Then, if the face
 *     wraps around to the other edge of the screen, it adjusts the
 *     coordinates to the left edge of the screen and refills to
 *     display that portion of the face.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *     row       <input>==(row_type) row of the specified face table.
 *     facetable <input>==(vpf_table_type) VPF face primitive table.
 *     ringtable <input>==(vpf_table_type) VPF ring primitive table.
 *     edgetable <input>==(vpf_table_type) VPF edge primitive table.
 *     fbr       <input>==(vpf_table_type) VPF face bounding rectangle table.
 *     outline   <input>==(color_type) outline color.
 *     c1        <input>==(color_type) 1st color in the fill pattern.
 *     c2        <input>==(color_type) 2nd color in the fill pattern.
 *     c3        <input>==(color_type) 3rd color in the fill pattern.
 *     c4        <input>==(color_type) 4th color in the fill pattern.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   May 1991                           DOS Turbo C
 *E
 *************************************************************************/
void draw_face_row( row_type row,
		vpf_table_type facetable,
		vpf_table_type ringtable,
		vpf_table_type edgetable,
		vpf_table_type fbr,
		color_type outline,
		color_type c1,
		color_type c2,
		color_type c3,
		color_type c4 )
{
   ossim_int32 count;
   int  spx1,spy1,spx2,spy2,  rowid,ring;
   extent_type shade_box;
   Pattern  pattern;
   face_rec_type face_rec;
   int wrap, mbrx2;
   row_type fbrrow;
   int XMIN_,YMIN_,XMAX_,YMAX_;

   rowid = table_pos( "ID", facetable );
   ring = table_pos( "RING_PTR", facetable );
   get_table_element( rowid, row, facetable, &(face_rec.id), &count );
   get_table_element( ring, row, facetable, &(face_rec.ring), &count );

   gpsetlinecolor(outline);
   gpsetlinestyle(SOLID_LINE);
   gpsetlinewidth(NORM_WIDTH);

   wrap = 0;

   if (gpgetdevice()==SCREEN) {
      pattern = MakePattern(c1,c2,c3,c4);

      /* Read bounding box */
      fbrrow = read_row( face_rec.id, fbr );
      XMIN_ = table_pos( "XMIN", fbr );
      YMIN_ = table_pos( "YMIN", fbr );
      XMAX_ = table_pos( "XMAX", fbr );
      YMAX_ = table_pos( "YMAX", fbr );
      get_table_element(XMIN_,fbrrow,fbr,&shade_box.x1,&count);
      get_table_element(YMIN_,fbrrow,fbr,&shade_box.y1,&count);
      get_table_element(XMAX_,fbrrow,fbr,&shade_box.x2,&count);
      get_table_element(YMAX_,fbrrow,fbr,&shade_box.y2,&count);
      free_row(fbrrow,fbr);

      screen_bounds(shade_box.x1,shade_box.y1,
		 shade_box.x2,shade_box.y2,
		 &spx1,&spy1,&spx2,&spy2);
      mbrx2 = spx2;
      if (spx2<spx1) {
	 /* The face wraps around the screen */
	 /* (or at least wraps off the edge of the screen) */
	 wrap = gpgetmaxx();
	 if (spx2 < 0) wrap -= spx2;
	 /* Adjust the maximum screen value */
	 spx2 += wrap;
      }

   } else {
      gpstartpoly();
   }

   draw_polygon( spx1,spy1,spx2,spy2, face_rec, ringtable,
		 edgetable, outline, pattern, wrap );

   if (wrap && mbrx2 > 0) {
      /* The polygon has wrapped around to the other side of the */
      /* screen.  Must redraw the part left off on the first pass. */

      if (gpgetdevice()==SCREEN) {

	 spx1 -= wrap;
	 spx2 -= wrap;

      } else {
	 gpstartpoly();
      }

      wrap *= -1;
      draw_polygon( spx1,spy1,spx2,spy2, face_rec, ringtable,
		    edgetable, outline, pattern, wrap );

   }

}


/*************************************************************************
 *
 *N  draw_face
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function draws the specified face from the given face table.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *     row_num   <input>==(ossim_int32) row id of the specified face.
 *     facetable <input>==(vpf_table_type) VPF face primitive table.
 *     ringtable <input>==(vpf_table_type) VPF ring primitive table.
 *     edgetable <input>==(vpf_table_type) VPF edge primitive table.
 *     fbr       <input>==(vpf_table_type) VPF face bounding rectangle table.
 *     outline   <input>==(color_type) outline color.
 *     c1        <input>==(color_type) 1st color in the fill pattern.
 *     c2        <input>==(color_type) 2nd color in the fill pattern.
 *     c3        <input>==(color_type) 3rd color in the fill pattern.
 *     c4        <input>==(color_type) 4th color in the fill pattern.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   May 1991                           DOS Turbo C
 *E
 *************************************************************************/
void draw_face( ossim_int32 rownum,
		vpf_table_type facetable,
		vpf_table_type ringtable,
		vpf_table_type edgetable,
		vpf_table_type fbr,
		color_type outline,
		color_type c1,
		color_type c2,
		color_type c3,
		color_type c4 )
{
   row_type row;

   row = get_row( rownum, facetable );
   draw_face_row(row,facetable, ringtable, edgetable, fbr,
		 outline,c1,c2,c3,c4);
   free_row(row,facetable);
}




/*************************************************************************
 *
 *N  outline_polygon_loop
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function outlines a loop (or ring) of a polygon by chaining
 *     through the right forward and/or left forward edges of the edge
 *     table.  It does not fill a polygon.
 *
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *     face_id    <input>==(ossim_int32) row id of the specified face.
 *     start_edge <input>==(ossim_int32) row id of the edge that starts the
 *                         loop.
 *     edgetable <input>==(vpf_table_type) VPF edge primitive table.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   May 1991                           DOS Turbo C
 *E
 *************************************************************************/
void outline_polygon_loop(ossim_int32 face_id,
			  ossim_int32 start_edge,
			  vpf_table_type edgetable)
{
  edge_rec_type edge_rec;
  ossim_int32 next, prevnode;
  boolean done=FALSE;
  IPoint make_ipoint();

  edge_rec = read_edge( start_edge, edgetable );

  edge_rec.dir = '+';

  prevnode = edge_rec.start;

  if (edge_rec.start != edge_rec.end) {

     next = next_polygon_edge( &edge_rec, &prevnode, face_id );

  } else {

     done = TRUE;

  }

  draw_polygon_edge( edge_rec );

  if (edge_rec.coord) free(edge_rec.coord);

  while (!done) {

     if (next < 0) {
	gotoxy(1,18);
	printf(" VPF topology error! Edge: %ld   face: %ld   left: %ld   right: %ld",
		edge_rec.id,face_id,edge_rec.left,edge_rec.right);
	getch();
	gotoxy(1,18);
	printf("                                                                                ");
	done = TRUE;
     }

     if (next==0) {
	gotoxy(1,19);
	printf("Next edge(%d) = 0  ",edge_rec.id);
	done = TRUE;
     }

     if (next == start_edge) done = TRUE;

     if (!done) {

	edge_rec = read_edge( next, edgetable );

	next = next_polygon_edge( &edge_rec, &prevnode, face_id );

	draw_polygon_edge( edge_rec );

	if (edge_rec.coord) free(edge_rec.coord);

     }
  }

}



/*************************************************************************
 *
 *N  outline_face
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function outlines (without filling) the specified face from
 *     the given face table.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *     face_id   <input>==(ossim_int32) row id of the specified face.
 *     facetable <input>==(vpf_table_type) VPF facemitive table.
 *     ringtable <input>==(vpf_table_type) VPF ringmitive table.
 *     edgetable <input>==(vpf_table_type) VPF edge primitive table.
 *     outline   <input>==(color_type) outline color.
 *     inner     <input>==(int) if TRUE, draw inner rings;
 *                              if FALSE, only draw outer ring.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   May 1991                           DOS Turbo C
 *E
 *************************************************************************/
void outline_face( ossim_int32 face_id,
		   vpf_table_type facetable,
		   vpf_table_type ringtable,
		   vpf_table_type edgetable,
		   color_type outline,
		   int inner )
{
   face_rec_type face_rec;
   ring_rec_type ring_rec;

   gpsetlinecolor(outline);
   gpsetlinestyle(SOLID_LINE);
   gpsetlinewidth(NORM_WIDTH);

   face_rec = read_face( face_id, facetable );
   ring_rec = read_ring( face_rec.ring, ringtable );
   outline_polygon_loop( face_id, ring_rec.edge, edgetable );

   if (!inner) return;

   while (ring_rec.face == face_id ) {
      ring_rec = read_next_ring( ringtable );

      if (feof(ringtable.fp)) {
	 break;
      }

      if (ring_rec.face == face_id) {

	 outline_polygon_loop( face_id, ring_rec.edge, edgetable );

      }
   }
}



/*************************************************************************
 *
 *N  outline_face_table
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function outlines the specified face from a previously unopened
 *     face primitive table in the given color.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *     face_id  <input>==(ossim_int32) id of the face to be outlined.
 *     fname    <input>==(char *) file name of the face primitive table.
 *     color    <input>==(int) color to outline the face.
 *     inner    <input>==(int) if TRUE, draw inner rings;
 *                             if FALSE, only draw outer ring.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   May 1991                           DOS Turbo C
 *E
 *************************************************************************/
void outline_face_table( ossim_int32 face_id, char *fname,
			 int color, int inner )
{
   vpf_table_type facetable, ringtable, edgetable;
   char *name;

   name = (char *)vpfmalloc( 255*sizeof(char) );

   gpsetlinecolor( color );

   facetable = vpf_open_table(fname,disk, "rb", (char *)NULL);

   strupr(fname);

   strcpy( name, facetable.path );
   strcat( name, "RNG" );
   ringtable = vpf_open_table( name, disk, "rb", (char *)NULL );

   strcpy( name, facetable.path );
   strcat( name, "EDG" );
   edgetable = vpf_open_table( name, disk, "rb", (char *)NULL );

   free( name );

   hidemousecursor();

   outline_face( face_id, facetable, ringtable, edgetable, color,
		 inner );

   showmousecursor();

   vpf_close_table(&facetable);
   vpf_close_table(&ringtable);
   vpf_close_table(&edgetable);

}
#endif


