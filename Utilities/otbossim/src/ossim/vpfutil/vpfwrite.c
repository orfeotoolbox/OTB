/*************************************************************************
 *
 *N  Module VPFWRITE.C
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This contains functions for writing data to VPF tables.
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
 *     Original Coding:  Tom Wood	Fall 1990
 *       Modifications:	 David Flinn	January 1991
 *					July 1991
 *		         Barry Michaels  October 1991
 *					Modified from converter
 *					software (UNIX) for
 *					VPFVIEW software (DOS).
 *			 Jim TenBrink   October 1991
 *					Made vpfread.c and vpfwrite.c
 *					disjoint
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This module should be ANSI C compatible.
 *E
 *************************************************************************/
#include <ossim/vpfutil/machine.h>

#ifdef CARBON
#define MACHINE_BYTE_ORDER 0
#endif

#if __MSDOS__
#include <io.h>
#include <alloc.h>
#include <dos.h>
#include <graphics.h>
#else
	#ifndef _WIN32
#include <unistd.h>
	#endif
#endif

#if UNIX
#include <sys/stat.h>
#define   SEEK_SET    0         /* Turbo C fseek value */
#define   SEEK_CUR    1
#define   farmalloc   malloc    /* no farmallocs on UNIX */
#endif

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <memory.h>
#include <ossim/vpfutil/vpftable.h>
#include <ossim/vpfutil/vpfview.h>
#  ifndef min
#    define min(a,b) ((a) <= (b) ? (a) : (b))
#  endif
#  ifndef max
#    define max(a,b) ((a) <= (b) ? (b) : (a))
#  endif

extern int STORAGE_BYTE_ORDER;

/* Include statically to reduce external module dependencies */


/*************************************************************************
 *
 *N  write_key
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function writes an id triplet key from the specified file.
 *     It is assumed that there is enough free disk space to write to the
 *     file. It is also assumed that the file pointer (fp) is already opened
 *     for writing.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    key     <input> == (id_triplet_type) id triplet key.
 *    fp      <input> == (FILE *) input file pointer.
 *    return <output> == (ossim_int32) size of the key.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Dave Flinn       July 1991      Based on read_key in vpftable.c
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This module should be ANSI C compatible.
 *E
 *************************************************************************/
ossim_int32 write_key( id_triplet_type key, FILE *fp )
{
  ossim_int32 size = 0 ;	/* to count size of key write */
  unsigned char tint ;
  ossim_int16 tshort ;

   /* Assume that any count value has been written before this */
   /* Only write one key in this subroutine, do not write more */

  Write_Vpf_Char (&(key.type),fp,1);
  size += sizeof ( char ) ;

   switch (TYPE0(key.type)) {
   case 0:
     break;
   case 1:
     tint = (unsigned char) key.id ;
     Write_Vpf_Char ( &tint, fp, 1 ) ;
     size += sizeof ( char ) ;
     break;
   case 2:
     tshort = (short) key.id ;
     Write_Vpf_Short ( &tshort, fp, 1 ) ;
     size += sizeof ( ossim_int16 ) ;
     break;
   case 3:
     Write_Vpf_Int (&(key.id), fp, 1 ) ;
     size += sizeof ( ossim_int32 ) ;
     break;
   }

   switch (TYPE1(key.type)) {
   case 0:
     break;
   case 1:
     tint = (unsigned char) key.tile ;
     Write_Vpf_Char ( &tint, fp, 1 ) ;
     size += sizeof ( char ) ;
     break;
   case 2:
     tshort = (short) key.tile ;
     Write_Vpf_Short ( &tshort, fp, 1 ) ;
     size += sizeof ( ossim_int16 ) ;
     break;
   case 3:
     Write_Vpf_Int (&(key.tile), fp, 1 ) ;
     size += sizeof ( ossim_int32 ) ;
     break;
   }

   switch (TYPE2(key.type)) {
   case 0:
     break;
   case 1:
     tint = (unsigned char) key.exid ;
     Write_Vpf_Char ( &tint, fp, 1 ) ;
     size += sizeof ( char ) ;
     break;
   case 2:
     tshort = (short) key.exid ;
     Write_Vpf_Short ( &tshort, fp, 1 ) ;
     size += sizeof ( ossim_int16 ) ;
     break;
   case 3:
     Write_Vpf_Int (&(key.exid), fp, 1 ) ;
     size += sizeof ( ossim_int32 ) ;
     break;
   }
  return size ;
}

/*************************************************************************
 *
 *N  write_next_row
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function writes the next row of the table.
 *     The parameter row must be initialized prior to this functional, either
 *     by being read in from an existing table or set to valid values.
 *     A row with any empty columns should not be written out.
 *     The parameter table must be a valid table and initialized prior to
 *     this function, by vpf_open_table.  It is assumed that there is
 *     enough free disk space to write to the file. It is also assumed that
 *     the file pointer (table->fp) is already opened for writing. The
 *     variable count, set to the values in row, must be greater than 0,
 *     otherwise, if count is -1 the vpf_write functions will lock up
 *     (row[].count should never have a value of 0). Note that if errorfp
 *     is used, it must be opened prior to this function.
 *
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    row        <input> == (row_type) the row to write to the table.
 *    table      <input> == (vpf_table_type *) vpf table structure.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Dave Flinn       July 1991      Based on read_next_row.
 *    Barry Michaels    Oct 1991      Added row as a parameter.
 *    JTB              10/91          guaranteed function always
 *                                    returns a value:
 *                                     0: record written
 *                                    -1: unknown field type
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *   None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This module should be ANSI C compatible.
 *E
 *************************************************************************/
ossim_int32 write_next_row(row_type row, vpf_table_type * table )
{
   register ossim_int32 i,
		     j;
   char            * tptr,
		   * output ;
   ossim_int32          recordsize = 0;
   ossim_int32	     count;
   id_triplet_type * keys;
   ossim_uint32 pos_for_ndx,
		     length;
   int               retn_val = 0;
   coordinate_type   dummycoord = {0.0,0.0};

   STORAGE_BYTE_ORDER = table->byte_order;

   table->nrows++;
   fseek(table->fp, 0L, SEEK_END);
   pos_for_ndx = ftell(table->fp); /* begining of new row */

   for (i = 0; i < table->nfields; i++) {   /* for each column */

     count = row[i].count ;          /* Retrieve count from row.  Should
					be 0 if variable length null */

     /* In case this column is variable length, write out count */

     if (count == 0) count = 1;

     if ( table->header[i].count < 0 ) {
       Write_Vpf_Int ( &count, table->fp, 1 ) ;
       recordsize += sizeof ( ossim_int32 ) ;
     }

     /* Now write out the data type */

     switch (table->header[i].type) {

     case 'T':
       if ( count == 0 ) 	/* Assume this is variable length text
				   and don't do anything */
	 break ;

       /* This loop insures that the exact number of characters are written
	  out to disk. */

       output = (char *) vpfmalloc ( count + 1 ) ;  /* include null byte */
       for (j = 0, tptr = (char*)row[i].ptr; j < count; j++, tptr++)
	 if ( *tptr )
	   output[j] = *tptr ;
	 else
	   output[j] = SPACE ;
       output[count] = '\0' ;
       Write_Vpf_Char( output ,table->fp, count) ;
       free ( output ) ;
       recordsize += sizeof ( char ) * count ;
       break;

     case 'I':
       Write_Vpf_Int (row[i].ptr, table->fp, count ) ;
       recordsize += sizeof ( ossim_int32 ) * count ;
       break;

     case 'S':
       Write_Vpf_Short (row[i].ptr, table->fp, count ) ;
       recordsize += sizeof ( ossim_int16 ) * count ;
       break;

     case 'F':
       Write_Vpf_Float (row[i].ptr, table->fp, count ) ;
       recordsize += sizeof ( float ) * count ;
       break;

     case 'R':
       Write_Vpf_Double (row[i].ptr, table->fp, count ) ;
       recordsize += sizeof ( double ) * count ;
       break;

     case 'D':	/* date has 21 chars in memory, not on disk */
       Write_Vpf_Date (row[i].ptr, table->fp, count ) ;
       recordsize += ( sizeof ( date_type ) - 1 ) * count ;
       break;

     case 'C':
       if (row[i].ptr) {
	  Write_Vpf_Coordinate(row[i].ptr,table->fp,count);
       } else {
	  for (j=0;j<count;j++)
	     Write_Vpf_Coordinate(&dummycoord,table->fp,count);
       }
       recordsize += sizeof ( coordinate_type ) * count ;
       break;

     case 'B':
       Write_Vpf_DoubleCoordinate(row[i].ptr,table->fp,count);
       recordsize += sizeof ( double_coordinate_type ) * count ;
       break;

     case 'Z':
       Write_Vpf_CoordinateZ(row[i].ptr,table->fp,count);
       recordsize += sizeof ( tri_coordinate_type ) * count ;
       break;

     case 'Y':
       Write_Vpf_DoubleCoordinateZ(row[i].ptr,table->fp,count);
       recordsize += sizeof ( double_tri_coordinate_type ) * count ;
       break;

     case 'K':
       keys = (id_triplet_type *) vpfmalloc (count*sizeof(id_triplet_type)) ;
       memcpy ( keys, row[i].ptr, count*sizeof(id_triplet_type) ) ;
       for (j=0;j<count;j++)
	 recordsize += write_key ( keys[j], table->fp);
       free ( keys ) ;
       break;

     case 'X':
       /* do nothing */
       break;

     default:
       return(-1);
     }
   }

   if ( table->xfp ) {  /* only for variable length columns */
     length = recordsize ;
     fseek( table->xfp, 0, SEEK_END );

     Write_Vpf_Int ( &pos_for_ndx, table->xfp, 1 ) ;
     Write_Vpf_Int ( &length, table->xfp, 1 ) ;
   }

   return retn_val;
}


/*************************************************************************
 *
 *N  create_row
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function creates a null row for the given table.
 *     The parameter table must be a valid table and initialized prior to
 *     this function, by vpf_open_table.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    table      <input> == (vpf_table_type) vpf table structure.
 *    return    <output> == (row_type) row of the table.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    Oct 1991
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *   None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This module should be ANSI C compatible.
 *E
 *************************************************************************/
row_type create_row( vpf_table_type table )
{
   ossim_int32 i;
   row_type row;

   row = (row_type)vpfmalloc(table.nfields*sizeof(column_type));
   for (i=0;i<table.nfields;i++) {
      row[i].count = table.header[i].count;
      row[i].ptr = NULL;
   }
   return row;
}



/*************************************************************************
 *
 *N  nullify_table_element
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     Frees one field element - no action is taken if the
 *     field index is invalid.
 *     The parameter row must be initialized prior to this functional, either
 *     buy being read in from an existing table or set to valid values. The
 *     parameter table must be a valid table and initialized prior to this
 *     function, by vpf_open_table.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *     field <input>  == (ossim_int32) column offset.
 *     row   <inout> == (row_type) row containing element to be removed.
 *     table <inout> == (vpf_table_type) VPF table owning row.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *   RDF  7/91  original
 *   JTB  10/91 removed call to exit();
 *E
 *************************************************************************/
void nullify_table_element( ossim_int32            field,
			    row_type       row,
			    vpf_table_type table )
{
   if (field < 0 || field >= table.nfields)
     return;

   if (row[field].ptr)
   {
     free(row[field].ptr);
     row[field].ptr = NULL;
     row[field].count = table.header[field].count;
   }
}

/*************************************************************************
 *
 *N  put_table_element
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     Copies one element into the designated field.
 *     The parameter row must be initialized prior to this functional, either
 *     buy being read in from an existing table or set to valid values. The
 *     parameter table must be a valid table and initialized prior to this
 *     function, by vpf_open_table. Note that if errorfp is used, it must
 *     be opened prior to this function.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *     field <input>  == (ossim_int32) column offset.
 *     row   <in-out> == (row_type) row containing target field.
 *     table <in-out> == (vpf_table_type) VPF table owning row.
 *     value <in>     == (void *) source field element.
 *     count <in>     == (ossim_int32) number of items in value.
 *     put_table_element <output> == (ossim_int32)
 *                                    0 --> element write succeeded
 *                                    1 --> unknown element type or
 *                                          invalid column offset
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *   RDF  7/91  original
 *   JTB  10/91 removed call to exit();
 *              guaranteed function always returns value
 *              0: element write succeeded
 *             -1: unknown element type or invalid column (field) offset
 *E
 *************************************************************************/
ossim_int32 put_table_element( ossim_int32              field,
		       row_type         row,
		       vpf_table_type   table,
		       void           * value,
		       ossim_int32         count )
{
   ossim_int32 i, len, stat;
   char *str;

   stat=0;

   if ((count != table.header[field].count) &&
       (table.header[field].count > 0)) {
      printf("Invalid element count! (%ld, %ld)\n",
	     count,table.header[field].count);
      return -1;
   }

   if (field < 0 || field >= table.nfields)
     return -1;

   row[field].count = count;

   if (row[field].ptr) {
      free(row[field].ptr);
      row[field].ptr = NULL;
   }

   switch ( table.header[field].type ) {
      case 'T':
	len = (ossim_int32)max(count,table.header[field].count);
	str = (char *) vpfmalloc( len + 1 );
	row[field].ptr = (char *) vpfmalloc ( len + 1 ) ;
	strcpy( (char*)str, (char*)value );
	for ( i = (long)strlen((char*)value) ; i < table.header[field].count; i++ )
	   str[i] = SPACE ;
	str[len] = '\0';
	memcpy(row[field].ptr, str, len+1);
	free(str);
	break ;

      case 'D':
	row[field].ptr = (date_type *) vpfmalloc (count*sizeof(date_type));
	memcpy ( row[field].ptr, value, sizeof (date_type) * count ) ;
	break;

      case 'I' :
	row[field].ptr = (ossim_int32 *) vpfmalloc (count*sizeof(ossim_int32));
	memcpy ( row[field].ptr, value, sizeof (ossim_int32) * count ) ;
	break;

      case 'S' :
	row[field].ptr = (ossim_int16 *) vpfmalloc (count*sizeof(ossim_int16));
	memcpy ( row[field].ptr, value, sizeof (ossim_int16) * count ) ;
	break;

      case 'F':
	row[field].ptr = (float *) vpfmalloc (count*sizeof(float));
	memcpy ( row[field].ptr, value, sizeof (float) * count ) ;
	break;

      case 'R':
	row[field].ptr = (double *) vpfmalloc (count*sizeof(double));
	memcpy ( row[field].ptr, value, sizeof (double) * count ) ;
	break;

      case 'K':
	row[field].ptr =
	  (id_triplet_type *) vpfmalloc ( count*sizeof(id_triplet_type ));
	memcpy ( row[field].ptr, value, sizeof(id_triplet_type)*count ) ;
	break;

      case 'C':
	if (value) {
	   row[field].ptr = (coordinate_type *)
	       malloc ( count * sizeof( coordinate_type ));
	   if (row[field].ptr)
	      memcpy ( row[field].ptr, value,
		       sizeof(coordinate_type)*count ) ;
	} else {
	   row[field].ptr = NULL;
	}
	break;

      case 'Z':
	if (value) {
	  row[field].ptr = (tri_coordinate_type *)
	     malloc ( count * sizeof( tri_coordinate_type ));
	  if (row[field].ptr)
	     memcpy ( row[field].ptr, value,
		      sizeof(tri_coordinate_type)*count ) ;
	} else {
	   row[field].ptr = NULL;
	}
	break;

      case 'B':
	if (value) {
	   row[field].ptr = (double_coordinate_type *)
	     malloc ( count * sizeof( double_coordinate_type ));
	   if (row[field].ptr)
	      memcpy ( row[field].ptr, value,
		       sizeof(double_coordinate_type)*count ) ;
	} else {
	   row[field].ptr = NULL;
	}
	break;

      case 'Y':
	if (value) {
	   row[field].ptr = (double_tri_coordinate_type *)
	     malloc ( count * sizeof( double_tri_coordinate_type ));
	   if (row[field].ptr)
	      memcpy( row[field].ptr, value,
		      sizeof(double_tri_coordinate_type)*count);
	} else {
	   row[field].ptr = NULL;
	}
	break;

      default:
	stat = -1;
	break ;
   }

   return stat;
}



/* ========================================================================

   Environmental Systems Research Institute (ESRI) Applications Programming

       Project: 		Conversion from ARC/INFO to VPF
       Original Coding:		Tom Wood	Fall 1990
       Modifications:		David Flinn	January 1991
						July 1991
				JTB             10/91

   The following functions are used when writing from a big-endian machine.
   VPF requires little-endian words, so byte-swapping must be performed.

   ======================================================================== */

/* #ifdef UNIX */

ossim_int32 VpfWrite ( void *from, VpfDataType type, ossim_int32 count, FILE *to )
{
  ossim_int32 retval=0 , i ;

  switch ( type ) {
  case VpfChar:
    retval = (long)fwrite ( from, sizeof (char), count, to ) ;
    break ;
  case VpfShort:
    {
      if (MACHINE_BYTE_ORDER != STORAGE_BYTE_ORDER) {
	ossim_int16 stemp ,
		*sptr = (short *) from ;
	for ( i=0; i < count; i++, sptr++ ) {
	   swap_two ( (char*)sptr, (char*)&stemp ) ;
	   retval = (long)fwrite ( &stemp, sizeof (short), 1, to ) ;
	}
      } else {
	retval = (long)fwrite ( from, sizeof (short), count, to ) ;
      }
    }
    break ;
  case VpfInteger:
    {
      if (MACHINE_BYTE_ORDER != STORAGE_BYTE_ORDER) {
	 ossim_int32 itemp,
	   *iptr = (ossim_int32 *) from ;
	 for ( i=0; i < count; i++, iptr++ ) {
	   swap_four ( (char*)iptr, (char*)&itemp ) ;
	   retval = (long)fwrite ( &itemp, sizeof (ossim_int32), 1, to ) ;
	 }
      } else {
	 retval = (long)fwrite ( from, sizeof (ossim_int32), count, to ) ;
      }
    }
    break ;
  case VpfFloat:
    {
      if (MACHINE_BYTE_ORDER != STORAGE_BYTE_ORDER) {
	 float ftemp ,
	    *fptr = (float *) from ;
	 for ( i=0; i < count; i++, fptr++ ) {
	   swap_four ( (char*)fptr, (char*)&ftemp ) ;
	   retval = (long)fwrite ( &ftemp, sizeof (float), 1, to ) ;
	 }
      } else {
	 retval = (long)fwrite ( from, sizeof (ossim_int32), count, to ) ;
      }
    }
    break ;
  case VpfDouble:
    {
      if (MACHINE_BYTE_ORDER != STORAGE_BYTE_ORDER) {
	 double dtemp ,
	     *dptr = (double *) from ;
	 for ( i=0; i < count; i++, dptr++ ) {
	   swap_eight ( (char*)dptr, (char*)&dtemp ) ;
	   retval = (long)fwrite ( &dtemp, sizeof (double), 1, to ) ;
	 }
      } else {
	 retval = (long)fwrite ( from, sizeof (double), count, to ) ;
      }
    }
    break ;
  case VpfDate:	/* only write out 20, not 21 chars */
    retval = (long)fwrite ( from, sizeof ( date_type ) - 1, count, to ) ;
    break ;
  case VpfCoordinate:
    {
      if (MACHINE_BYTE_ORDER != STORAGE_BYTE_ORDER) {
	 coordinate_type ctemp ,
		      *cptr = (coordinate_type *) from ;
	 for ( i=0; i < count; i++, cptr++ ) {
	   swap_four ( (char*)&cptr->x, (char*)&ctemp.x ) ;
	   swap_four ( (char*)&cptr->y, (char*)&ctemp.y ) ;
	   retval = (long)fwrite ( &ctemp, sizeof (coordinate_type), 1, to ) ;
	 }
      } else {
	 retval = (long)fwrite ( from, sizeof (coordinate_type), count, to ) ;
      }
    }
    break ;
  case VpfDoubleCoordinate:
    {
      if (MACHINE_BYTE_ORDER != STORAGE_BYTE_ORDER) {
	 double_coordinate_type dctemp ,
			     *dcptr = (double_coordinate_type *) from ;
	 for ( i=0; i < count; i++, dcptr++ ) {
	   swap_eight ( (char*)&dcptr->x, (char*)&dctemp.x ) ;
	   swap_eight ( (char*)&dcptr->y, (char*)&dctemp.y ) ;
	   retval = (long)fwrite ( &dctemp, sizeof (double_coordinate_type),
	                     1, to ) ;
	 }
      } else {
	 retval = (long)fwrite ( from, sizeof (double_coordinate_type),
			   count, to ) ;
      }
    }
    break ;
  case VpfTriCoordinate:
    {
      if (MACHINE_BYTE_ORDER != STORAGE_BYTE_ORDER) {
	tri_coordinate_type ttemp ,
			  *tptr = (tri_coordinate_type *) from ;
	for ( i=0; i < count; i++, tptr++ ) {
	   swap_four ( (char*)&tptr->x, (char*)&ttemp.x ) ;
	   swap_four ( (char*)&tptr->y, (char*)&ttemp.y ) ;
	   swap_four ( (char*)&tptr->z, (char*)&ttemp.z ) ;
	   retval = (long)fwrite ( &ttemp, sizeof (tri_coordinate_type), 1, to ) ;
	}
      } else {
	retval = (long)fwrite ( from, sizeof (tri_coordinate_type), count, to ) ;
      }
    }
    break ;
  case VpfDoubleTriCoordinate:
    {
      if (MACHINE_BYTE_ORDER != STORAGE_BYTE_ORDER) {
	double_tri_coordinate_type dttemp ,
		    *dtptr = (double_tri_coordinate_type *) from ;
	for ( i=0; i < count; i++, dtptr++ ) {
	   swap_eight ( (char*)&dtptr->x, (char*)&dttemp.x ) ;
	   swap_eight ( (char*)&dtptr->y, (char*)&dttemp.y ) ;
	   swap_eight ( (char*)&dtptr->z, (char*)&dttemp.z ) ;
	   retval = (long)fwrite ( &dttemp,sizeof (double_tri_coordinate_type),
			     1, to);
	}
      } else {
	retval = (long)fwrite ( from,sizeof (double_tri_coordinate_type),
			  count, to);
      }
    }
    break ;
  case VpfNull:
    /* Do Nothing */
    break ;
  default:
    break ;
  }

  return retval;
}

/* #endif */
