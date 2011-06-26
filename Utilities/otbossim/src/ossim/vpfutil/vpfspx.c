/*************************************************************************
 *
 *N  Module VPFSPX.C
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This module contains functions for reading the VPF spatial index
 *     file.  A VPF spatial index is a modified quad-tree index with a
 *     cross-over bin for features spanning more than one spatial cell.
 *     There is one interface function in this module -
 *     spatial_index_search().  The spatial index creation function is
 *     not included.
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
 *    Mody Buchbinder  Jul 1991   Original coding
 *    Barry Michaels   Aug 1991   Customized for VPFVIEW
 *E
 *************************************************************************/
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <fcntl.h>
#ifdef __MSDOS__
#  include <mem.h>
#  include <conio.h>
#  include <sys\stat.h>
#  include <dos.h> 
#else
#  include <sys/stat.h>
#endif
#include <stdlib.h>
#ifdef __MSDOS__
#  include <io.h>
#endif
#ifdef _MSC_VER
#  include <io.h>
#endif
#ifndef _MSDOS__
#ifndef _WIN32
#include <unistd.h>
#endif
#endif
#include <ossim/vpfutil/set.h>
#include <ossim/base/ossimConstants.h>
/******* cellmap definitions **/
#define  START     0
#define  NUMFEAT   1

#define OUT        0
#define IN         1

#define ID         1
#define BOUND      0

#define TOTAL_NUMBER 0
#define BOUND_START  1
#define CELLMAP_SIZE 5

#define XMIN 0
#define YMIN 1
#define XMAX 2
#define YMAX 3

typedef struct {
   int fd;                     /* File descriptor */
   ossim_int32 maplen;            /* Number of cells in the tree */
   ossim_int32 cellmap[1023][2];  /* The tree of cells */
   ossim_int32 buf[1023][2];      /* Buffer for one cell */
   ossim_int32 shift;             /* Size of data before real cell records */
   unsigned char box[4];       /* Search box */
} spx_type;


/*************************************************************************
 *
 *N  is_over
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    Determine if two boxes overlap.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    box1    <input>==(unsigned char[4]) first box to check.
 *    box2    <input>==(unsigned char[4]) second box to check.
 *    return <output>==(int) 1 if the boxes overlap,
 *                             0 if the boxes do not.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Mody Buchbinder  Jul 1991
 *E
 *************************************************************************/
static int is_over( unsigned char box1[4], unsigned char box2[4] )
{
   int xmin,xmax,ymin,ymax;

   if(box2[XMIN] >= box1[XMIN] && box2[XMIN] <= box1[XMAX])
      xmin = IN;
   else
      xmin = OUT;
   if(box2[XMAX] >= box1[XMIN] && box2[XMAX] <= box1[XMAX])
      xmax = IN;
   else
      xmax = OUT;
   if(box2[YMIN] >= box1[YMIN] && box2[YMIN] <= box1[YMAX])
      ymin = IN;
   else
      ymin = OUT;
   if(box2[YMAX] >= box1[YMIN] && box2[YMAX] <= box1[YMAX])
      ymax = IN;
   else
      ymax = OUT;
   /* complete overlap */
   if(xmin == OUT && xmax == OUT && box2[XMIN] <= box1[XMIN] &&
      box2[XMAX] >= box1[XMAX])
      xmin = IN;
   if(ymin == OUT && ymax == OUT && box2[YMIN] <= box1[YMIN] &&
      box2[YMAX] >= box1[YMAX])
      ymin = IN;

   if((xmin == IN || xmax == IN) && (ymin == IN || ymax == IN)) return 1;

   return 0;
}


/*************************************************************************
 *
 *N  get_record
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    Read spatial index record 'num'.  Insert the record into the search
 *    set if it falls within the search box.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    num  <input>==(ossim_int32) record number.
 *    spx  <input>==(spx_type *) spatial index record structure.
 *    set <output>==(set_type) search set to be updated.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Mody Buchbinder  Jul 1991
 *E
 *************************************************************************/
static void get_record(ossim_int32 num, spx_type *spx, set_type set )
{
   long offset;
   int j,index;
   unsigned char box[4];

   index  = num -1;
   offset = spx->cellmap[index][0] + spx->shift;
   lseek(spx->fd,offset,SEEK_SET);
   read(spx->fd,spx->buf,2*spx->cellmap[index][NUMFEAT]*sizeof(ossim_int32));

   for(j=0;j<spx->cellmap[index][NUMFEAT];j++) {
      memcpy(&box,&(spx->buf[j][BOUND]),4*sizeof(unsigned char));
      if (is_over(box,spx->box)) {
	 set_insert(spx->buf[j][ID],set);
      }
   }

}


/*************************************************************************
 *
 *N  search_cell
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    Search the current cell for local records and, if the box is in the
 *    search area, search the cell's children.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    record <input>==(ossim_int32) record number.
 *    level  <input>==(int) level of the index tree.
 *    bnd    <input>==(unsigned char[4]) bounding box.
 *    spx    <input>==(spx_type *) spatial index record structure.
 *    set   <output>==(set_type) search set to be updated.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Mody Buchbinder  Jul 1991
 *E
 *************************************************************************/
static void search_cell( ossim_int32 record, int level, unsigned char bnd[4],
			 spx_type *spx, set_type set )
{
   int i;
   unsigned char locbnd[4],cut;

  for(i=0;i<4;i++) locbnd[i] = bnd[i];
  if(level != 0)
    { /* level even -> cut on x , level odd -> cut on y */
      if(level%2 == 1) cut = bnd[0] + ((bnd[2] - bnd[0]) >> 1);
      else             cut = bnd[1] + ((bnd[3] - bnd[1]) >> 1);
      /* cut on x right cell */
      if(level%2 == 1 && record%2 == 0) locbnd[0] = cut;
      /* cut on x left cell */
      if(level%2 == 1 && record%2 == 1) locbnd[2] = cut;
      /* cut on y upper cell */
      if(level%2 == 0 && record%2 == 0) locbnd[1] = cut;
      /* cut on y lower cell */
      if(level%2 == 0 && record%2 == 1) locbnd[3] = cut;
     }
  if(is_over(locbnd,spx->box) == 1)
    { get_record(record,spx,set);
      if(record*2 <= spx->maplen)
       { search_cell(record*2,  level+1,locbnd, spx, set);
	 search_cell(record*2+1,level+1,locbnd, spx, set);
       }
    }
}


/*************************************************************************
 *
 *N  spatial_index_search
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    Search the named spatial index file to find all records falling
 *    within the specified extent.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    fname <input>==(char *) path name to a VPF spatial index file.
 *    x1    <input>==(float) left coordinate of the search box.
 *    y1    <input>==(float) bottom coordinate of the search box.
 *    x2    <input>==(float) right coordinate of the search box.
 *    y2    <input>==(float) top coordinate of the search box.
 *    set  <output>==(set_type) search set to be filled.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Mody Buchbinder  Jul 1991
 *E
 *************************************************************************/
set_type spatial_index_search( char *fname,
			       float x1, float y1, float x2, float y2 )
{
   long head[6];
   float bnd[4],xf,yf;
   unsigned char tempbox[4];
   set_type set;
   spx_type spx;

#ifdef __MSDOS__
   spx.fd = open(fname,O_RDONLY | O_BINARY);
#else
   spx.fd = open(fname,O_RDONLY);
#endif
   if(spx.fd == -1) {
      printf("Cannot open spatial index file (%s) \n",fname);
      perror("spatial_index_search: ");
      set.size = 0;
      set.buf = NULL;
      set.diskstorage = 0;
      set.fp = NULL;
      return set;
   }

/*   read basic header - int  0 -> number of features in coverage */
/*                       int  1 - 4 -> boundaries of coverage     */
/*                       int  5 -> size of tree in records - each */
/*                                 record = 2 integers (8 byte)   */
   read(spx.fd,head,6*sizeof(ossim_int32));
   spx.shift = 6*sizeof(ossim_int32);
   spx.maplen = head[CELLMAP_SIZE];

   set = set_init(head[TOTAL_NUMBER]+1L);

   memcpy(&bnd[0],&head[BOUND_START],4*sizeof(float));

   /* If the boundaries of the cellmap are completely within the */
   /* search area, all features are on */
   if ( (x1 <= bnd[XMIN]) && (x2 >= bnd[XMAX]) &&
	(y1 <= bnd[YMIN]) && (y2 >= bnd[YMAX]) ) {
      set_on(set);
      close(spx.fd);
      return set;
   }

/* read the cell tree */

   if( (size_t)read(spx.fd,spx.cellmap,2*spx.maplen*sizeof(ossim_int32)) !=
       (2*spx.maplen*sizeof(ossim_int32)) )
   {
      close(spx.fd);
      return set;
   }

   spx.shift += 2*spx.maplen*sizeof(ossim_int32);

/* translate search box to 1 byte numbers */
   xf = (float)(255.0 / (bnd[XMAX] - bnd[XMIN]));
   yf = (float)(255.0 / (bnd[YMAX] - bnd[YMIN]));
   spx.box[XMIN] = (char)((x1 - bnd[XMIN]) * xf);
   if (x1 <= bnd[XMIN]) spx.box[XMIN] = 0;
   if (x1 >= bnd[XMAX]) spx.box[XMIN] = 255;
   spx.box[YMIN] = (char)((y1 - bnd[YMIN]) * yf);
   if (y1 <= bnd[YMIN]) spx.box[YMIN] = 0;
   if (y1 >= bnd[YMAX]) spx.box[YMIN] = 255;
   spx.box[XMAX] = (char)(((x2 - bnd[XMIN]) * xf) + 1);
   if (x2 <= bnd[XMIN]) spx.box[XMAX] = 0;
   if (x2 >= bnd[XMAX]) spx.box[XMAX] = 255;
   spx.box[YMAX] = (char)(((y2 - bnd[YMIN]) * yf) + 1);
   if (y2 <= bnd[YMIN]) spx.box[YMAX] = 0;
   if (y2 >= bnd[YMAX]) spx.box[YMAX] = 255;

/** start search **/
   tempbox[XMIN] = 0;
   tempbox[YMIN] = 0;
   tempbox[XMAX] = 0xff;
   tempbox[YMAX] = 0xff;
   search_cell(1,0,tempbox,&spx,set);

   close(spx.fd);

   return set;
}
