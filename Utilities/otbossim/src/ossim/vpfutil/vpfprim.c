/*************************************************************************
 *
 *N  Module VPFPRIM  -  VPF Primitives
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This module contains functions for handling VPF primitives
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
 *    Barry Michaels   April 1991                  DOS Turbo C
 *E
 *************************************************************************/

#include <stdio.h>
#ifdef __MSDOS__
#include <io.h>
#include <dos.h>
#include <graphics.h>
#else
#ifndef _WIN32
#include <unistd.h>
#endif
#endif

#include <string.h>
#include <ctype.h>
#if defined(__CYGWIN__) || defined(__APPLE__)|| defined(_WIN32)
#include <ossim/vpfutil/values.h>
#else
#include <ossim/vpfutil/values.h>
#endif
#ifdef __MSDOS__
#include <alloc.h>
#else
#ifdef __APPLE__ 
#include <sys/types.h>
#include <sys/malloc.h>
#else
#ifndef __FreeBSD__
#include <malloc.h>
#include <string.h>
#endif
#endif
#endif
#include <stdlib.h>


#include <ossim/vpfutil/vpftable.h>
#include <ossim/vpfutil/vpfprim.h>

/* Compute the offset from the start of the row to the given field */
static ossim_int32 row_offset( int field, row_type row, vpf_table_type table) 
{
   ossim_int32 offset,n,size;
   int i;
   id_triplet_type key;
   int keysize[] = {0,sizeof(char),sizeof(short int),sizeof(ossim_int32)};

   if (field < 0 || field >= table.nfields) return -1;

   offset = 0L;
   for (i=0;i<field;i++) {
      switch (table.header[i].type) {
	 case 'I':
	    offset += sizeof(ossim_int32)*row[i].count;
	    break;
	 case 'S':
	    offset += sizeof(short int)*row[i].count;
	    break;
	 case 'T':
	    offset += sizeof(char)*row[i].count;
	    break;
	 case 'F':
	    offset += sizeof(float)*row[i].count;
	    break;
	 case 'D':
	    offset += sizeof(date_type)*row[i].count;
	    break;
	 case 'K':
	    get_table_element(i,row,table,&key,&n);
	    size = sizeof(char) +
		   keysize[TYPE0(key.type)] +
		   keysize[TYPE1(key.type)] +
		   keysize[TYPE2(key.type)];
	    offset += size*row[i].count;
	    break;
	 case 'R':
	    offset += sizeof(double)*row[i].count;
	    break;
	 case 'C':
	    offset += sizeof(coordinate_type)*row[i].count;
	    break;
	 case 'B':
	    offset += sizeof(double_coordinate_type)*row[i].count;
	    break;
	 case 'Z':
	    offset += sizeof(tri_coordinate_type)*row[i].count;
	    break;
	 case 'Y':
	    offset += sizeof(double_tri_coordinate_type)*row[i].count;
	    break;
      }
   }
   return offset;
}


/*************************************************************************
 *
 *N  create_edge_rec
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function creates an edge record internal structure from a
 *     row of a VPF edge table.   NOTE:  This function allocates memory
 *     for "edge_rec.coord". This array should be freed when no longer
 *     needed.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    row        <input> == (row_type) VPF table row.
 *    edge_table <input> == (vpf_table_type) opened VPF table to read.
 *    return    <output> == (edge_rec_type) returned edge record.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    April 1991                      DOS Turbo C
 *E
 *************************************************************************/
edge_rec_type create_edge_rec( row_type row, vpf_table_type edge_table )
{
   edge_rec_type edge;
   ossim_int32 count,i;
   int rowid,start,end,right,left,rightfwd,leftfwd,coord;
   id_triplet_type key;
   tri_coordinate_type *Zcoord;
   double_coordinate_type *Bcoord;
   double_tri_coordinate_type *Ycoord;

   rowid = table_pos( "ID", edge_table );
   start = table_pos( "START_NODE", edge_table );
   end = table_pos( "END_NODE", edge_table );
   right = table_pos( "RIGHT_FACE", edge_table );
   left = table_pos( "LEFT_FACE", edge_table );
   rightfwd = table_pos( "RIGHT_EDGE", edge_table );
   leftfwd = table_pos( "LEFT_EDGE", edge_table );
   coord = table_pos( "COORDINATES", edge_table );

   get_table_element( rowid, row, edge_table, &(edge.id), &count );

   if (start >= 0)
      get_table_element( start, row, edge_table, &(edge.start), &count );
   else
      edge.start = 0;

   if (end >= 0)
      get_table_element( end, row, edge_table, &(edge.end), &count );
   else
      edge.end = 0;

   if (right >= 0) {
      if (edge_table.header[right].type=='K') {
	 get_table_element( right, row, edge_table, &key, &count );
	 edge.right = key.id;
      } else if (edge_table.header[right].type=='I') {
	 get_table_element( right, row, edge_table, &(edge.right),
			    &count );
      } else {
	 edge.right=1;
      }
   } else {
      edge.right = 1;
   }

   if (left >= 0) {
      if (edge_table.header[right].type=='K') {
	 get_table_element( left, row, edge_table, &key, &count );
	 edge.left = key.id;
      } else if (edge_table.header[left].type=='I') {
	 get_table_element( right, row, edge_table, &(edge.left),
			    &count );
      } else {
	 edge.left=1;
      }
   } else {
      edge.left = 1;
   }

   if (edge_table.header[rightfwd].type=='K') {
      get_table_element( rightfwd, row, edge_table, &key, &count );
      edge.rightfwd = key.id;
   } else if (edge_table.header[rightfwd].type=='I') {
      get_table_element( rightfwd, row, edge_table, &(edge.rightfwd),
			 &count );
   } else {
      edge.rightfwd=0;
   }

   if (edge_table.header[leftfwd].type=='K') {
      get_table_element( leftfwd, row, edge_table, &key, &count );
      edge.leftfwd = key.id;
   } else if (edge_table.header[leftfwd].type=='I') {
      get_table_element( leftfwd, row, edge_table, &(edge.leftfwd),
			 &count );
   } else {
      edge.leftfwd=0;
   }

   switch (edge_table.header[coord].type) {
      case 'C':
	 edge.coord = (coordinate_type *)get_table_element( coord,
						      row, edge_table,
						      NULL, &count );
	 break;
      case 'Z':
	 Zcoord = (tri_coordinate_type *)get_table_element( coord,
						      row, edge_table,
						      NULL, &count );
	 edge.coord = (coordinate_type *)malloc(count*
					  sizeof(coordinate_type));
	 if (edge.coord) {
	    for (i=0;i<count;i++) {
	       edge.coord[i].x = Zcoord[i].x;
	       edge.coord[i].y = Zcoord[i].y;
	    }
	 }
	 free(Zcoord);
	 break;
      case 'B':
	 Bcoord = (double_coordinate_type *)get_table_element( coord,
						      row, edge_table,
						      NULL, &count );
	 edge.coord = (coordinate_type *)malloc(count*
					  sizeof(coordinate_type));
	 if (edge.coord) {
	    for (i=0;i<count;i++) {
	       edge.coord[i].x = (float)Bcoord[i].x;
	       edge.coord[i].y = (float)Bcoord[i].y;
	    }
	 }
	 free(Bcoord);
	 break;
      case 'Y':
	 Ycoord = (double_tri_coordinate_type *)get_table_element( coord,
						      row, edge_table,
						      NULL, &count );
	 edge.coord = (coordinate_type *)malloc(count*
					  sizeof(coordinate_type));
	 if (edge.coord) {
	    for (i=0;i<count;i++) {
	       edge.coord[i].x = (float)Ycoord[i].x;
	       edge.coord[i].y = (float)Ycoord[i].y;
	    }
	 }
	 free(Ycoord);
	 break;
      default:
	 count = 0;
	 break;
   }
   edge.coord_type = edge_table.header[coord].type;
   edge.npts = count;

   edge.fp = NULL;
   if (!edge.coord) {
      edge.fp = edge_table.fp;
      edge.startpos = index_pos(edge.id,edge_table) +
		      row_offset(coord,row,edge_table) +
		      (ossim_int32)sizeof(ossim_int32);
      edge.pos = -1;
   }

   edge.current_coordinate = -1;

   edge.dir = ' ';

   return edge;
}


/*************************************************************************
 *
 *N  read_edge
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function reads a VPF edge record from the input VPF table.
 *     It performs a search for the specified line number, and, if found,
 *     allocates, reads, and returns the edge record.   NOTE:  This function
 *     allocates memory for "edge_rec.coord". This array should be freed
 *     when no longer needed.  If an invalid row id is passed in, this
 *     function will have unpredictable results.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    id         <input> == (ossim_int32) edge id number.
 *    edge_table <input> == (vpf_table_type) opened VPF table to read.
 *    return    <output> == (edge_rec_type) returned edge record.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    April 1991                      DOS Turbo C
 *E
 *************************************************************************/
edge_rec_type read_edge( ossim_int32  id,
			 vpf_table_type edge_table )
{
   edge_rec_type edge;
   row_type row;

   row = get_row( id, edge_table );
   edge = create_edge_rec( row, edge_table );
   free_row( row, edge_table );
   return edge;
}




/*************************************************************************
 *
 *N  read_next_edge
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function reads the next VPF edge record from the input VPF table.
 *     NOTE:  This function allocates memory for "edge_rec.coord".
 *     This array should be freed when no longer needed.
 *     Must have called vpf_open_table with DISK as the storage type,
 *     since this function accesses the disk to read the next row.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    edge_table  <input> == (vpf_table_type) opened VPF table to read.
 *    return     <output> == (edge_rec_type) returned edge record.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    April 1991                      DOS Turbo C
 *E
 *************************************************************************/
edge_rec_type read_next_edge( vpf_table_type edge_table )
{
   edge_rec_type edge;
   row_type row;

   row = read_next_row( edge_table );
   edge = create_edge_rec( row, edge_table );
   free_row( row, edge_table );

   return edge;
}

coordinate_type first_edge_coordinate( edge_rec_type *edge_rec )
{
   coordinate_type coord;
   tri_coordinate_type Zcoord;
   double_coordinate_type Bcoord;
   double_tri_coordinate_type Ycoord;
   int size;

   edge_rec->current_coordinate = 0;

   if (edge_rec->coord) {
      /* Coordinate array is in memory */
      return edge_rec->coord[0];
   }

   /* Read coordinate from table */
   fseek(edge_rec->fp,edge_rec->startpos,SEEK_SET);
   switch (edge_rec->coord_type) {
      case 'C':
	 fread(&coord,sizeof(coord),1,edge_rec->fp);
	 size = sizeof(coord);
	 break;
      case 'Z':
	 fread(&Zcoord,sizeof(Zcoord),1,edge_rec->fp);
	 coord.x = Zcoord.x;
	 coord.y = Zcoord.y;
	 size = sizeof(Zcoord);
	 break;
      case 'B':
	 fread(&Bcoord,sizeof(Bcoord),1,edge_rec->fp);
	 coord.x = (float)Bcoord.x;
	 coord.y = (float)Bcoord.y;
	 size = sizeof(Bcoord);
	 break;
      case 'Y':
	 fread(&Ycoord,sizeof(Ycoord),1,edge_rec->fp);
	 coord.x = (float)Ycoord.x;
	 coord.y = (float)Ycoord.y;
	 size = sizeof(Ycoord);
	 break;
      default:
	 coord.x = MAXFLOAT/2.0;
	 coord.y = MAXFLOAT/2.0;
	 size = 0;
	 break;
   }
   edge_rec->pos = edge_rec->startpos + size;

   return coord;
}

coordinate_type next_edge_coordinate( edge_rec_type *edge_rec )
{
   coordinate_type coord;
   tri_coordinate_type Zcoord;
   double_coordinate_type Bcoord;
   double_tri_coordinate_type Ycoord;
   int size;

   if (edge_rec->current_coordinate < 0)
      return first_edge_coordinate(edge_rec);

   edge_rec->current_coordinate++;

   if (edge_rec->current_coordinate >= edge_rec->npts) {
      edge_rec->current_coordinate = edge_rec->npts-1L;
      if (!edge_rec->coord)
	 fseek(edge_rec->fp,edge_rec->startpos +
			    (edge_rec->npts-1L)*sizeof(coord),
	       SEEK_SET);
   }

   if (edge_rec->coord) {
      /* Coordinate array is in memory */
      return edge_rec->coord[edge_rec->current_coordinate];
   }

   /* Read coordinate from table */
   switch (edge_rec->coord_type) {
      case 'C':
	 fread(&coord,sizeof(coord),1,edge_rec->fp);
	 size = sizeof(coord);
	 break;
      case 'Z':
	 fread(&Zcoord,sizeof(Zcoord),1,edge_rec->fp);
	 coord.x = Zcoord.x;
	 coord.y = Zcoord.y;
	 size = sizeof(Zcoord);
	 break;
      case 'B':
	 fread(&Bcoord,sizeof(Bcoord),1,edge_rec->fp);
	 coord.x = (float)Bcoord.x;
	 coord.y = (float)Bcoord.y;
	 size = sizeof(Bcoord);
	 break;
      case 'Y':
	 fread(&Ycoord,sizeof(Ycoord),1,edge_rec->fp);
	 coord.x = (float)Ycoord.x;
	 coord.y = (float)Ycoord.y;
	 size = sizeof(Ycoord);
	 break;
      default:
	 coord.x = MAXFLOAT/2.0;
	 coord.y = MAXFLOAT/2.0;
	 size = 0;
	 break;
   }
   edge_rec->pos = edge_rec->startpos + size;

   return coord;
}

coordinate_type get_edge_coordinate( ossim_int32 n,
				     edge_rec_type *edge_rec )
{
   coordinate_type coord;
   tri_coordinate_type Zcoord;
   double_coordinate_type Bcoord;
   double_tri_coordinate_type Ycoord;
   ossim_int32 size;

   if (n < 0)
      return first_edge_coordinate(edge_rec);

   if (n >= edge_rec->npts) n = edge_rec->npts-1L;

   edge_rec->current_coordinate = n;

   if (edge_rec->coord) {
      /* Coordinate array is in memory */
      return edge_rec->coord[n];
   }

   /* Read coordinate from table */
   switch (edge_rec->coord_type) {
      case 'C':
	 size = sizeof(coord);
	 break;
      case 'Z':
	 size = sizeof(Zcoord);
	 break;
      case 'B':
	 size = sizeof(Bcoord);
	 break;
      case 'Y':
	 size = sizeof(Ycoord);
	 break;
      default:
	 size = 0;
	 break;
   }
   edge_rec->pos = edge_rec->startpos + (n*size);
   fseek(edge_rec->fp,edge_rec->pos,SEEK_SET);
   switch (edge_rec->coord_type) {
      case 'C':
	 fread(&coord,sizeof(coord),1,edge_rec->fp);
	 break;
      case 'Z':
	 fread(&Zcoord,sizeof(Zcoord),1,edge_rec->fp);
	 coord.x = Zcoord.x;
	 coord.y = Zcoord.y;
	 break;
      case 'B':
	 fread(&Bcoord,sizeof(Bcoord),1,edge_rec->fp);
	 coord.x = (float)Bcoord.x;
	 coord.y = (float)Bcoord.y;
	 break;
      case 'Y':
	 fread(&Ycoord,sizeof(Ycoord),1,edge_rec->fp);
	 coord.x = (float)Ycoord.x;
	 coord.y = (float)Ycoord.y;
	 break;
      default:
	 coord.x = MAXFLOAT/2.0;
	 coord.y = MAXFLOAT/2.0;
	 break;
   }

   return coord;
}




/*************************************************************************
 *
 *N  read_face
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function reads a VPF face record from the input VPF table.
 *     It performs a search for the specified face number, and, if found,
 *     reads and returns the face record.  If id is out of range,
 *     either the lowest or highest id numbered face record type will be
 *     returned.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    id           <input> == (ossim_int32) face id number.
 *    face_table   <input> == (vpf_table_type) opened VPF table to read.
 *    return      <output> == (face_rec_type) returned face record.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    April 1991                      DOS Turbo C
 *E
 *************************************************************************/
face_rec_type read_face( ossim_int32  id,
			 vpf_table_type face_table )
{
   face_rec_type face;
   int rowid,ring;
   ossim_int32 count;
   row_type row;

   rowid = table_pos( "ID", face_table );
   ring = table_pos( "RING_PTR", face_table );

   row = get_row( id, face_table );

   get_table_element( rowid, row, face_table, &(face.id), &count );

   get_table_element( ring, row, face_table, &(face.ring), &count );

   free_row( row, face_table );

   return face;
}





/*************************************************************************
 *
 *N  read_next_face
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function reads the next VPF face record from the input VPF
 *     table.  Must have used vpf_open_table with DISK as the storage type.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    face_table   <input> == (vpf_table_type) opened VPF table to read.
 *    return      <output> == (face_rec_type) returned face record.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    April 1991                      DOS Turbo C
 *E
 *************************************************************************/
face_rec_type read_next_face( vpf_table_type face_table )
{
   face_rec_type face;
   int rowid,ring;
   ossim_int32 count;
   row_type row;

   rowid = table_pos( "ID", face_table );
   ring = table_pos( "RING_PTR", face_table );

   row = read_next_row( face_table );

   get_table_element( rowid, row, face_table, &(face.id), &count );

   get_table_element( ring, row, face_table, &(face.ring), &count );

   free_row( row, face_table );

   return face;
}





/*************************************************************************
 *
 *N  read_ring
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function reads a VPF ring record from the input VPF table.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    id           <input> == (ossim_int32) ring id number.
 *    ring_table   <input> == (vpf_table_type) opened VPF table to read.
 *    return      <output> == (ring_rec_type) returned ring record.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    April 1991                      DOS Turbo C
 *E
 *************************************************************************/
ring_rec_type read_ring( ossim_int32  id,
			 vpf_table_type ring_table )
{
   ring_rec_type ring;
   int rowid,face,edge;
   ossim_int32 count;
   row_type row;

   rowid = table_pos( "ID", ring_table );
   face = table_pos( "FACE_ID", ring_table );
   edge = table_pos( "START_EDGE", ring_table );

   row = get_row( id, ring_table );

   get_table_element( rowid, row, ring_table, &(ring.id), &count );

   get_table_element( face, row, ring_table, &(ring.face), &count );

   get_table_element( edge, row, ring_table, &(ring.edge), &count );

   free_row( row, ring_table );

   return ring;
}




/*************************************************************************
 *
 *N  read_next_ring
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function reads the next VPF ring record from the input VPF table.
 *     If read_next_ring goes past the end of the vpftable, ring_rec_type
 *     id will be a garbage number.  The programmer must ensure that this
 *     does NOT happen.  Must have called vpf_open_table with DISK as the
 *     storage type.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    ring_table   <input> == (vpf_table_type) opened VPF table to read.
 *    return      <output> == (ring_rec_type) returned ring record.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    April 1991                      DOS Turbo C
 *E
 *************************************************************************/
ring_rec_type read_next_ring( vpf_table_type ring_table )
{
   ring_rec_type ring;
   int rowid,face,edge;
   ossim_int32 count;
   row_type row;

   rowid = table_pos( "ID", ring_table );
   face = table_pos( "FACE_ID", ring_table );
   edge = table_pos( "START_EDGE", ring_table );

   row = read_next_row( ring_table );

   get_table_element( rowid, row, ring_table, &(ring.id), &count );

   get_table_element( face, row, ring_table, &(ring.face), &count );

   get_table_element( edge, row, ring_table, &(ring.edge), &count );

   free_row( row, ring_table );

   return ring;
}






/*************************************************************************
 *
 *N  read_point
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function reads a VPF point record from the input VPF table.
 *     It performs a search for the specified point number, and, if found,
 *     reads and returns the point record.  If read_point is given an
 *     invalid id (eg 1000000) it will return garbage.  It is the programmers
 *     responsibility to ensure that this does NOT happen.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    id            <input> == (ossim_int32) point id number.
 *    point_table   <input> == (vpf_table_type) opened VPF table to read.
 *    return       <output> == (point_rec_type) returned point record.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    April 1991                      DOS Turbo C
 *E
 *************************************************************************/
point_rec_type read_point( ossim_int32  id,
			   vpf_table_type point_table )
{
   point_rec_type point;
   int rowid,face,coord;
   ossim_int32 count;
   coordinate_type c;
   tri_coordinate_type Zcoord;
   double_coordinate_type Bcoord;
   double_tri_coordinate_type Ycoord;
   row_type row;

   rowid = table_pos( "ID", point_table );
   face = table_pos( "CONTAINING_FACE", point_table );
   coord = table_pos( "COORDINATE", point_table );

   row = get_row( id, point_table );

   get_table_element( rowid, row, point_table, &(point.id), &count );

   if (face == -1)
     point.face = -1;
   else
     get_table_element( face, row, point_table, &(point.face), &count );

   switch (point_table.header[coord].type) {
      case 'C':
	 get_table_element( coord, row, point_table, &c, &count );
	 point.x = c.x;
	 point.y = c.y;
	 break;
      case 'Z':
	 get_table_element( coord, row, point_table, &Zcoord, &count );
	 point.x = Zcoord.x;
	 point.y = Zcoord.y;
	 break;
      case 'B':
	 get_table_element( coord, row, point_table, &Bcoord, &count );
	 point.x = (float)Bcoord.x;
	 point.y = (float)Bcoord.y;
	 break;
      case 'Y':
	 get_table_element( coord, row, point_table, &Ycoord, &count );
	 point.x = (float)Ycoord.x;
	 point.y = (float)Ycoord.y;
	 break;
      default:
	 point.x = MAXFLOAT/2.0;
	 point.y = MAXFLOAT/2.0;
	 break;
   }

   free_row( row, point_table );

   return point;
}




/*************************************************************************
 *
 *N  read_next_point
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function reads the next VPF point record from the input
 *     VPF table.  Must have called vpf_open_table with DISK as the
 *     storage type.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    point_table   <input> == (vpf_table_type) opened VPF table to read.
 *    return       <output> == (point_rec_type) returned point record.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    April 1991                      DOS Turbo C
 *E
 *************************************************************************/
point_rec_type read_next_point( vpf_table_type point_table )
{
   point_rec_type point;
   int rowid,face,coord;
   ossim_int32 count;
   coordinate_type c;
   tri_coordinate_type Zcoord;
   double_coordinate_type Bcoord;
   double_tri_coordinate_type Ycoord;
   row_type row;

   rowid = table_pos( "ID", point_table );
   face = table_pos( "CONTAINING_FACE", point_table );
   coord = table_pos( "COORDINATE", point_table );

   row = read_next_row( point_table );

   get_table_element( rowid, row, point_table, &(point.id), &count );

   get_table_element( face, row, point_table, &(point.face), &count );

   switch (point_table.header[coord].type) {
      case 'C':
	 get_table_element( coord, row, point_table, &c, &count );
	 point.x = c.x;
	 point.y = c.y;
	 break;
      case 'Z':
	 get_table_element( coord, row, point_table, &Zcoord, &count );
	 point.x = Zcoord.x;
	 point.y = Zcoord.y;
	 break;
      case 'B':
	 get_table_element( coord, row, point_table, &Bcoord, &count );
	 point.x = (float)Bcoord.x;
	 point.y = (float)Bcoord.y;
	 break;
      case 'Y':
	 get_table_element( coord, row, point_table, &Ycoord, &count );
	 point.x = (float)Ycoord.x;
	 point.y = (float)Ycoord.y;
	 break;
      default:
	 point.x = MAXFLOAT/2.0;
	 point.y = MAXFLOAT/2.0;
	 break;
   }

   free_row( row, point_table );

   return point;
}






/*************************************************************************
 *
 *N  read_anno
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function reads a VPF annotation record from the input VPF table.
 *     It performs a search for the specified annotation id, and, if
 *     found, reads and returns the annotation record.  NOTE: This function
 *     allocates memory for "anno.text".  This must be freed when no
 *     longer needed.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    id           <input> == (ossim_int32) annotation id.
 *    anno_table   <input> == (vpf_table_type) opened VPF table to read.
 *    return      <output> == (anno_rec_type) returned annotation record.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    April 1991                      DOS Turbo C
 *E
 *************************************************************************/
anno_rec_type read_anno( ossim_int32 id,
			 vpf_table_type anno_table )
{
   anno_rec_type anno;
   int rowid,text,coord;
   ossim_int32 count;
   coordinate_type *c;
   tri_coordinate_type *Zcoord;
   double_coordinate_type *Bcoord;
   double_tri_coordinate_type *Ycoord;
   row_type row;

   rowid = table_pos( "ID", anno_table );
   text = table_pos( "STRING", anno_table );
   coord = table_pos( "SHAPE_LINE", anno_table );

   row = get_row( id, anno_table );

   get_table_element( rowid, row, anno_table, &(anno.id), &count );

   anno.text = (char*)get_table_element( text, row, anno_table, NULL, &count );

   switch (anno_table.header[coord].type) {
      case 'C':
	 c = (coordinate_type *)get_table_element( coord, row,
					  anno_table, NULL, &count );
	 anno.x = c[0].x;
	 anno.y = c[0].y;
	 free(c);
	 break;
      case 'Z':
	 Zcoord = (tri_coordinate_type *)get_table_element( coord, row,
					       anno_table, NULL, &count );
	 anno.x = Zcoord[0].x;
	 anno.y = Zcoord[0].y;
	 free(Zcoord);
	 break;
      case 'B':
	 Bcoord = (double_coordinate_type *)get_table_element( coord, row,
					       anno_table, NULL, &count );
	 anno.x = (float)Bcoord[0].x;
	 anno.y = (float)Bcoord[0].y;
	 free(Bcoord);
	 break;
      case 'Y':
	 Ycoord = (double_tri_coordinate_type *)get_table_element( coord,
					 row, anno_table, NULL, &count );
	 anno.x = (float)Ycoord[0].x;
	 anno.y = (float)Ycoord[0].y;
	 free(Ycoord);
	 break;
      default:
	 anno.x = MAXFLOAT/2.0;
	 anno.y = MAXFLOAT/2.0;
	 break;
   }

   free_row( row, anno_table );

   return anno;
}




/*************************************************************************
 *
 *N  read_next_anno
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function reads the next VPF annotation record from the input
 *     VPF table.  Must have called vpf_open_table with DISK as the storage
 *     type.  NOTE: This function allocates memory for "anno.text".  This
 *     must be freed when no longer needed.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    anno_table   <input> == (vpf_table_type) opened VPF table to read.
 *    return      <output> == (anno_rec_type) returned annotation record.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    April 1991                      DOS Turbo C
 *E
 *************************************************************************/
anno_rec_type read_next_anno( vpf_table_type anno_table )
{
   anno_rec_type anno;
   int rowid,text,coord;
   ossim_int32 count;
   coordinate_type *c;
   tri_coordinate_type *Zcoord;
   double_coordinate_type *Bcoord;
   double_tri_coordinate_type *Ycoord;
   row_type row;

   rowid = table_pos( "ID", anno_table );
   text = table_pos( "STRING", anno_table );
   coord = table_pos( "SHAPE_LINE", anno_table );

   row = read_next_row( anno_table );

   get_table_element( rowid, row, anno_table, &(anno.id), &count );

   anno.text = (char*)get_table_element( text, row, anno_table, NULL, &count );

   switch (anno_table.header[coord].type) {
      case 'C':
	 c = (coordinate_type *)get_table_element( coord, row,
					  anno_table, NULL, &count );
	 anno.x = c[0].x;
	 anno.y = c[0].y;
	 free(c);
	 break;
      case 'Z':
	 Zcoord = (tri_coordinate_type *)get_table_element( coord, row,
					       anno_table, NULL, &count );
	 anno.x = Zcoord[0].x;
	 anno.y = Zcoord[0].y;
	 free(Zcoord);
	 break;
      case 'B':
	 Bcoord = (double_coordinate_type *)get_table_element( coord, row,
					       anno_table, NULL, &count );
	 anno.x = (float)Bcoord[0].x;
	 anno.y = (float)Bcoord[0].y;
	 free(Bcoord);
	 break;
      case 'Y':
	 Ycoord = (double_tri_coordinate_type *)get_table_element( coord,
					 row, anno_table, NULL, &count );
	 anno.x = (float)Ycoord[0].x;
	 anno.y = (float)Ycoord[0].y;
	 free(Ycoord);
	 break;
      default:
	 anno.x = MAXFLOAT/2.0;
	 anno.y = MAXFLOAT/2.0;
	 break;
   }

   free_row( row, anno_table );

   return anno;
}


