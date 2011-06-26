/*************************************************************************
 *
 * Environmental Systems Research Institute (ESRI) Applications Programming
 *
 *N  Module VPFTIDX
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *P
 *   Purpose: Subroutines to create and read both the thematic index
 *	      or a gazeteer index on a VPF table column.
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
 *    Dave Flinn       September/October 1991
 *    Barry Michaels   January 1992   A few adjustments for DOS.
 *                                    Added sets to create function to
 *                                    take advantage of SET's disk-
 *				      swapping ability when low on memory.
 *E
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions:
 *F
 *
 *    set_type read_thematic_index ( char *tablename,
 *				     char *value ) ;
 *
 *    ThematicIndex open_thematic_index ( char *tablename );
 *
 *    set_type search_thematic_index ( ThematicIndex *themindex,
 *				       char *value ) ;
 *
 *    void close_thematic_index ( ThematicIndex *themindex );
 *
 *    set_type read_gazetteer_index (char * vpfname, char * query_str ) ;
 *
 *    set_type search_gazetteer_index (ThematicIndex *idx,
 *				       char * query_str ) ;
 *
 *    ossim_int32 read_gazetteer_index_directory (
 *					ThematicIndexDirectory ** gid,
 *					ThematicIndexHeader     * gi,
 *					FILE * idx_fp);
 *
 *    ossim_int32 write_thematic_index_header ( ThematicIndexHeader h ,
 *					     FILE *fp ) ;
 *
 *    ossim_int32 read_thematic_index_header ( ThematicIndexHeader *h ,
 *					    FILE *fp ) ;
 *
 *    ossim_int32 write_thematic_index_directory (
 *				       ThematicIndexHeader h ,
 *				       ThematicIndexDirectory *d,
 *				       ossim_int32 size,
 *				       FILE *fp ) ;
 *
 *    ossim_int32 write_gazetteer_index_directory (
 *				       ThematicIndexHeader h ,
 *				       ThematicIndexDirectory *d,
 *				       ossim_int32 size,
 *				       FILE *fp ) ;
 *
 *    ossim_int32 create_thematic_index ( char indextype,
 *				       char *tablename,
 *				       char *idxname ,
 *				       char *columnname,
 *				       char *idx_set );
 *
 *    ossim_int32 create_gazetteer_index (char *tablename,
 *				       char *idx_fname ,
 *				       char *columnname,
 *				       char *idx_set);
 *
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *
 *   Portability:
 *O
 *    This module should be ANSI C compatible. Created on UNIX first
 *E
 *************************************************************************/

#include <ctype.h>
#include <ossim/vpfutil/machine.h>
#include <ossim/vpfutil/vpfmisc.h>
#include <ossim/vpfutil/vpftable.h>
#include <ossim/vpfutil/vpftidx.h>
#include <string.h>
#include <stdlib.h>
#include <memory.h>

#ifndef __MSDOS__ 
#  ifndef _WIN32
#    include <unistd.h>
#  endif
#endif

void *vpfmalloc(unsigned long size);

#define Whimper(str) {\
      return ((ossim_int32)0) ; }

#define SWhimper(str) {\
      set_type err; err = set_init (1) ;\
      printf("\nvpftidx: < %s >\n", str ) ;\
      return err ; }

#define OWhimper(str) {\
      printf("\nvpftidx: < %s >\n", str ) ;\
      return idx ; }


/*
 * an index file directory entry takes up a different amount
 * of space in memory than it does on disk; the following constant
 * should be used to calculate the starting offset for
 * index data
 *
 * example: if the number of items to be indexed is t, and each item is
 *          s bytes long, then use
 *
 *          sizeof(ThematicIndexHeader) + t * (s + DIR_ON_DISK_INCR)
 *
 * to find out where the first byte of indexing data will be placed.
 *
 * That first byte is what must be stored as the value of Header.nbytes
 */

#define DIR_ON_DISK_INCR (2 * sizeof(ossim_int32))

/*************************************************************************
 *
 *N  create_thematic_index
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *
 *      Create a thematic index file on a given vpf table.
 *P
 *
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    indextype  <input> == (char) either 'T' for thematic index or
 *					  'G' for gazetteer.
 *    tablename  <input> == (char *) path name of the vpf table.
 *    idxname    <input> == (char *) name of the index file to be created.
 *    columnname <input> == (char *) name of column to create index on
 *			    if the column is a triplet id, the columnname
 *			    should be as such: EDG_ID\ID
 *					       EDG_ID\TILE_ID
 *					       EDG_ID\EXT_ID
 *    idxset     <input> == (char *) array of character values to index on
 *                          if the index type is gazetteer.  Not used for
 *                          index type = 'T'.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Dave Flinn       October	 1991 
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
 *    void *vpfmalloc                                    VPFMISC.C
 *    vpf_open_table					 VPFTABLE.C
 *    table_pos
 *    table_element
 *    vpf_close_table
 *    write_thematic_index_header			see below
 *    write_thematic_index_directory			see below
 *    Vpf_Write_* macros
 *    set_init						SET.C
 *    set_insert
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This module should be ANSI C compatible.  developed on UNIX
 *E
 *************************************************************************/
ossim_int32 create_thematic_index ( char indextype,
		 	         char *tablename,
				 char *idxname ,
				 char *columnname,
				 char *idx_set ) 
{
   unsigned int i;
   unsigned int column_length;
   FILE           *ifp ;
   ossim_int32	 j,tablepos,k,
      keycolumn = 0 ,	/* 1 if key column */
      itemp , n,
      idsize ,		/* either 2 or 4 bytes */
      datasize ;		/* Directory value data size */
   char           *buf ,
      hack[80] ;
   vpf_table_type table ;
   ThematicIndexHeader		h ;
   ThematicIndexDirectory	*d ;
   short int			stemp ;
   float				ftemp ;
   double			dtemp ;
   id_triplet_type		key ;
   
   if ( indextype == 'G' )
   {
      return ( create_gazetteer_index ( tablename, idxname,columnname,idx_set)) ;
   }
   
   /* convert columnname to uppercase and check for id triplet */
   column_length = (unsigned int)strlen(columnname);
   for ( i=0 ; i < column_length; i++ )
   {
      /* copy into header structure */
      h.vpf_column_name[i] = toupper ( columnname[i] ) ;
   }
   
   h.vpf_column_name[i] = '\0';
   
   h.index_type = 'T' ;		/* default */
   h.type_count = 1 ;

   for (i=0 ; i < column_length; i++ )
   {
      columnname[i] = toupper ( columnname[i] ) ;

#if 0
    /* check if request is on a triplet id field */

    if ( columnname[i] == '\\' ) {

      columnname[i+1] = toupper ( columnname[i+1] ) ;
      
      switch ( columnname[i+1] ) {	/* locate sub-column key */
      case 'I':	/* ID of triplet id */
	keycolumn = 1 ; break ;
      case 'T':	/* TILE_ID of triplet id */
	keycolumn = 2 ; break ;
      case 'E':	/* EXT_ID of triplet id */
	keycolumn = 3 ; break ;
      default:
	Whimper ("error in key column request") ;
	/* break ; Whimper contains a return */
      }	/* end of switch */
      strcpy( h.vpf_column_name, strupr ( columnname )) ;
      columnname[i] = (char) NULL ;
    }     /* end of if columnname == \ */
#endif
  }	/* end of for loop */

  table = vpf_open_table ( tablename, disk, "rb", NULL ) ;


  /* Determine the id_data_size. This will save 50% of file size  */

  if ( table.nrows > MAX_ID ) {
    h.id_data_type = 'I' ;		/* ossim_int32s */
    idsize = sizeof ( ossim_int32 ) ;
  } else {
    h.id_data_type = 'S' ;		/* ossim_int32s */
    idsize = sizeof ( short int ) ;
  }

  /* Find position of column in input table */

  tablepos = table_pos ( columnname, table ) ;

  /* Now check data type */

  switch (table.header[tablepos].type) {
  case 'X':
    Whimper ("Cannot make index on Null Column") ;
    /* A return is in Whimper - break statement cannot be reached */
  case 'T':
    /* Don't bother with variable length, it's too hard for now */
    if ( table.header[tablepos].count <= 0 )
      Whimper ("Cannot make index on variable length text column") ;
    datasize = table.header[tablepos].count ;
    h.type_count = datasize ;		/* the special case */
    break;
  case 'D': 
    datasize = sizeof ( date_type ) ;
    Whimper ("Not implemented yet") ;
  case 'I':
    datasize = sizeof ( ossim_int32 ) ;
    break ;
  case 'K':				/* treat keys like integers */
    Whimper ("Cannot make index on key column") ;
  case 'S':
    datasize = sizeof ( short int ) ;
    break ;
  case 'F':
    datasize = sizeof ( float ) ;
    break ;
  case 'R':
    datasize = sizeof ( double ) ;
    break ;
  case 'C':
  case 'Z':
  case 'B':
  case 'Y':
    Whimper ("Cannot make index on Coordinate Column") ;
  default:
    sprintf (hack,"No such type < %c >", table.header[tablepos].type ) ;
    Whimper ( hack ) ;
  }  /* end of switch */

  /* open output index file */

  if (( ifp = fopen ( idxname, "wb" )) == NULL )
    Whimper ( idxname ) ;

  /* Create directory size as big as table, for worst case */

  d = (ThematicIndexDirectory *)
    vpfmalloc(sizeof(ThematicIndexDirectory)) ;
  h.nbins = 0 ;		/* initialize the directory counter */

#define REALLOC_DIRECTORY(loc) {\
  if ((d = (ThematicIndexDirectory *) \
       realloc(d,sizeof(ThematicIndexDirectory) * (loc+1))) == NULL )\
    Whimper ("error in reallocing directory") ; \
  d[loc].num_items = 0 ;\
  d[loc].idset = set_init(table.nrows+1); }

#define INSERT_ID(loc,val) {\
  d[loc].num_items++ ; \
  set_insert(val,d[loc].idset); }

/* Fill up id sets */

  switch ( table.header[tablepos].type ) {
  case 'I':

    for ( i=1; (int)i <= table.nrows; i++ ) {
      /* read in record from disk */
      table_element ( tablepos,i,table,&itemp,&n);
      for ( k=0; k < h.nbins; k++ )	/* Search directory for value*/
	if ( d[k].value.ival == itemp )
	  break ;/* Found a match */
      if ( k == h.nbins ) { 		/* New value in column */
	REALLOC_DIRECTORY ( k ) ;
	d[k].value.ival = itemp ;
	h.nbins++ ;
      }
      INSERT_ID(k,i) ;
    }   /* end of for i loop */ 

    break ;
  case 'T':

    /* initialize */
    buf = (char *) table_element (tablepos,1,table,NULL,&n);
    REALLOC_DIRECTORY ( 0 ) ;
    d[0].value.strval = (char *) vpfmalloc ( (unsigned long)strlen ( buf ) +1) ;
    memcpy( d[0].value.strval, buf, (unsigned long)strlen(buf) ) ;
    free (buf) ;
    h.nbins++ ;

    for ( i=1; (int)i <= table.nrows; i++ ) {
      /* read in record from disk */
      buf = (char *) table_element (tablepos,i,table,NULL,&n);

      /* strlen ( buf ) should equal datasize */

      for ( k=0; k < h.nbins; k++ )	/* Search directory for value*/
	if ( ! strcmp ( d[k].value.strval, buf ) )
	  break ;/* Found a match */

      if ( k == h.nbins ) { 		/* New value in column */
	REALLOC_DIRECTORY ( k ) ;
	d[k].value.strval = (char *) vpfmalloc ( (unsigned long)strlen ( buf ) +1) ;
	memcpy( d[0].value.strval, buf, strlen(buf) ) ;
	h.nbins++ ;
      }
      INSERT_ID(k,i) ;
      free(buf);
    }   /* end of for i loop */

    break ;
  case 'S':

    for ( i=1; (int)i <= table.nrows; i++ ) {
      /* read in record from disk */
      table_element ( tablepos,i,table,&stemp,&n);
      for ( k=0; k < h.nbins; k++ )	/* Search directory for value*/
	if ( d[k].value.sval == stemp ) 
	  break ;/* Found a match */
      if ( k == h.nbins ) { 		/* New value in column */
	REALLOC_DIRECTORY ( k ) ;
	d[k].value.sval = stemp ;
	h.nbins++ ;
      }
      INSERT_ID (k,i) ;
    }   /* end of for i loop */

    break ;
  case 'F':

    for ( i=1; (int)i <= table.nrows; i++ ) {
      /* read in record from disk */
      table_element ( tablepos,i,table,&ftemp,&n);
      for ( k=0; k < h.nbins; k++ )	/* Search directory for value*/
	if ( d[k].value.fval == ftemp ) 
	  break ;/* Found a match */
      if ( k == h.nbins ) { 		/* New value in column */
	REALLOC_DIRECTORY ( k ) ;
	d[k].value.fval = ftemp ;
	h.nbins++ ;
      }
      INSERT_ID (k,i) ;
    }   /* end of for i loop */

    break ;
  case 'R':
    for ( i=1; (int)i <= table.nrows; i++ ) {
      /* read in record from disk */
      table_element ( tablepos,i,table,&dtemp,&n);
      for ( k=0; k < h.nbins; k++ )	/* Search directory for value*/
	if ( d[k].value.dval == dtemp )
	  break ;/* Found a match */
      if ( k == h.nbins ) { 		/* New value in column */
	REALLOC_DIRECTORY ( k ) ;
	d[k].value.dval = dtemp ;
	h.nbins++ ;
      }
      INSERT_ID (k,i) ;
    }   /* end of for i loop */

    break ;
  case 'K':

    for ( i=1; (int)i <= table.nrows; i++ ) {
      /* read in record from disk */
      table_element ( tablepos,i,table,&key,&n);

      if ( keycolumn == 1 ) 
	itemp = key.id ;
      else if ( keycolumn == 2 ) 
	itemp = key.tile ;
      else 
	itemp = key.exid ;

      for ( k=0; k < h.nbins; k++ )	/* Search directory for value*/
	if ( d[k].value.ival == itemp ) 
	  break ;/* Found a match */
      if ( k == h.nbins ) { 		/* New value in column */
	REALLOC_DIRECTORY ( k ) ;
	d[k].value.ival = itemp ;
	h.nbins++ ;
      }
      INSERT_ID (k,i) ;
    }   /* end of for i loop */ 

    break ;
  default:
    Whimper ("error in table reading switch, no such type") ;
  } /* end of switch */

  /* set up header and write it out */
  /* h.id_data_size and columnname set up above */
  /* h.type_count set above */

  h.nbytes = THEMATIC_INDEX_HEADER_SIZE +
             h.nbins * ( datasize + DIRECTORY_SIZE ) ;

  h.column_type = table.header[tablepos].type ;
  if ( h.column_type == 'K' )		/* It's really an I */
    h.column_type = 'I' ;
  for ( i=0; i < 4; i++ )
    h.padding[i] = '\0' ;

  /* only write the table name, no pathname */

  for ( i = (unsigned int)strlen ( tablename ); i > 0; i-- )
    if ( tablename[i] == '/' ) break ;
  if ( i && i < strlen (tablename) )
    strcpy ( h.vpf_table_name, strupr ( &tablename[i+1] ) ) ;
  else
    strcpy( h.vpf_table_name, strupr ( tablename) );
  
  for ( i=(unsigned int)strlen(h.vpf_table_name); i < 12 ; i++ )
    h.vpf_table_name[i] = ' ' ;
  h.vpf_table_name[11] = '\0';

  /* columname setup above */

  h.table_nrows = table.nrows ;

  if ( write_thematic_index_header ( h, ifp ) == (ossim_int32)0 )
    Whimper ( "error writing index header" ) ;

  /* Now write out the rest of the header directory */

  if ( write_thematic_index_directory ( h, d, idsize, ifp ) == (ossim_int32)0 )
    Whimper ( "error writing index directory" ) ;

  /* now write the data */
  if ( h.id_data_type == 'I' ) {
    for ( i=0; (int)i < h.nbins; i++ )
      for ( j=1; j <= table.nrows; j++ )
	if (set_member(j,d[i].idset))
	  Write_Vpf_Int ( &j, ifp, 1 ) ;
  } else {
    for ( i=0; (int)i < h.nbins; i++ )
      for ( j=1; j <= table.nrows; j++ )
	if (set_member(j,d[i].idset)) {
	  stemp = (short int)j;
	  Write_Vpf_Short ( &stemp, ifp, 1 ) ;
	}
  }

  /* close up shop and exit */

  vpf_close_table ( &table );
  fclose ( ifp ) ;
  for ( i=0; (int)i < h.nbins; i++ )
    set_nuke(&d[i].idset);
  free ( d ) ;
  return ( h.nbins ) ;

}


/*************************************************************************
 *
 *N  read_thematic_index
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *	To read an index file, regardless of its type ( T or G ) and 
 *	return a set type array
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    idxname <input> == (char *) name of index file
 *    value   <input> == (char *) address of value to search on. 
 *			 THE PROGRAMMER MUST INSURE THAT THIS ADDRESS
 *			 CORRESPONDS TO THE TABLE COLUMN AND INDEX FILE.
 *
 *   Returns:
 *
 *    set_type		a set corresponding to the vpf table, where each
 *			bit relates to the ID in the table.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Dave Flinn       October 1991
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
 *	read_thematic_index_header		see below
 *	set_init				SET.C
 *	set_insert
 *	Vpf_Read_*                              VPFREAD.C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This module should be ANSI C compatible.
 *E
 *************************************************************************/
set_type read_thematic_index ( char *idxname,
			       char *value )
{
  ossim_int32 			i , ival = 0, Match = -1;
  short int			sval = 0;
  float				fval = 0.0 ;
  double			dval = 0.0 , atof () ;
  FILE				*ifp ;
  char				hack[80] , *buf = (char *) NULL, bufalloc=0 ;
  ThematicIndexHeader		h ;
  ThematicIndexDirectory	d ;
  set_type			s ;

  /* open output index file */

  if (( ifp = fopen ( idxname, "rb" )) == NULL ) {
    sprintf ( hack, "No such index < %s >", idxname ) ;
    SWhimper ( hack ) ;
  }

  if ( read_thematic_index_header ( &h, ifp ) == (ossim_int32)0 )
    SWhimper ( "error reading index header" ) ;

  if ( h.index_type == 'G' ) {
    fclose ( ifp ) ;
    s = read_gazetteer_index ( idxname, value ) ;
    return s ;
  }

  if ( value ) 		/* search for pattern */
    switch ( h.column_type ) {			/* using address */
    case	'I':
      memcpy ( &ival, value, sizeof (ossim_int32)) ;
      break ;
    case	'S':
      memcpy ( &sval, value, sizeof (short int)) ;
      break ;
    case	'F':
      memcpy ( &fval, value, sizeof (float)) ;
      break ;
    case	'R':
      memcpy ( &dval, value, sizeof (double)) ;
      break ;
    case	'T':
      buf = (char *) vpfmalloc ( h.type_count ) ;
      /* leave value as is */
      bufalloc = 1;
      break ;
    }
  else
    SWhimper ( "require a value to search on.") ;

  /* create set */

  s = set_init ( h.table_nrows ) ;

  /* Look for a matching directory entry */

  for ( i=0; i < h.nbins; i++ ) {

    switch ( h.column_type ) {
    case 	'I':
      Read_Vpf_Int ( &d.value.ival, ifp, 1) ;
      if ( ival == d.value.ival ) {
	/* found the match */
	Match = i ;
	i = h.nbins ;	/* exit */
      }
      break ;
    case 	'S':         
      Read_Vpf_Short ( &d.value.sval, ifp, 1) ;
      if ( sval == d.value.sval ) {
	/* found the match */
	Match = i ;
	i = h.nbins ;	/* exit */
      }
      break ;
    case 	'F':
      Read_Vpf_Float ( &d.value.fval, ifp, 1) ;
      if ( fval == d.value.fval ) {
	/* found the match */
	Match = i ;
	i = h.nbins ;	/* exit */
      }
      break ;
    case 	'R':         
      Read_Vpf_Float ( &d.value.dval, ifp, 1) ;
      if ( dval == d.value.dval ) {
	/* found the match */
	Match = i ;
	i = h.nbins ;	/* exit */
      }
      break ;
    case 	'T':
      Read_Vpf_Char ( buf, ifp, h.type_count ) ;
      bufalloc = 1;
      if ( ! strcmp ( value, buf ) ) {
	/* found the match */
	Match = i ;
	i = h.nbins ;	/* exit */
      }
      break ;
    } /* end of switch */

    Read_Vpf_Int ( &d.start_offset, ifp, 1) ;
    Read_Vpf_Int ( &d.num_items, ifp, 1) ;

  }  /* end of i loop */

  if ( bufalloc ) free ( buf ) ;

  if ( Match < 0 ) {
     fclose(ifp);
     return s;
  }

  /* We've got a match, so now read the ids in */

  if ( fseek ( ifp, d.start_offset, 0 ) != 0 )
    SWhimper ( "error in fseek") ;

  /* read data into user-defined pointer.  User must free this pointer */

  if ( h.id_data_type == 'I' )
    for ( i=0 ; i < d.num_items; i++ ) {
      Read_Vpf_Int ( &ival, ifp, 1 ) ;
      set_insert ( ival, s ) ;			/* set the id in the set */
    }
  else
    for ( i=0 ; i < d.num_items; i++ ) {
      Read_Vpf_Short ( &sval, ifp, 1 ) ;
      set_insert ( (ossim_int32) sval, s ) ;
    }

  fclose ( ifp ) ;

  return s ;					/* also return set */

}   /*end of read_index */


/*************************************************************************
 *
 *N  open_thematic_index
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     Open and initialize a thematic index.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    idxname <input> == (char *) name of index file
 *
 *   Returns:
 *
 *    ThematicIndex     Thematic index structure.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Dave Flinn       October 1991
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
 *	read_thematic_index_header		see below
 *	set_init				SET.C
 *	set_insert
 *	Vpf_Read_* macros
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This module should be ANSI C compatible.
 *E
 *************************************************************************/
ThematicIndex open_thematic_index ( char *idxname )
{
  char				hack[80];
  ThematicIndex		        idx ;

  idx.d = NULL;
  idx.gid = NULL;

  /* open output index file */

  if (( idx.fp = fopen ( idxname, "rb" )) == NULL ) {
    sprintf ( hack, "No such index < %s >", idxname ) ;
    OWhimper ( hack ) ;
  }

  if ( read_thematic_index_header ( &idx.h, idx.fp ) == (ossim_int32)0 )
    OWhimper ( "error reading index header" ) ;

  if ( idx.h.index_type == 'G' ) {
    /* gazetteer_index  */
     if (read_gazetteer_index_directory(&idx.gid,&idx.h,idx.fp) == (ossim_int32)0) {
	   fclose(idx.fp);
	   idx.fp = NULL;
     }
  }

  return idx;
}

/*************************************************************************
 *
 *N  search_thematic_index
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *	To search an index file, regardless of its type ( T or G ) and
 *	return a set type array
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    idx     <input> == (ThematicIndex) thematic index
 *    value   <input> == (char *) address of value to search on.
 *			 THE PROGRAMMER MUST INSURE THAT THIS ADDRESS
 *			 CORRESPONDS TO THE TABLE COLUMN AND INDEX FILE.
 *
 *   Returns:
 *
 *    set_type		a set corresponding to the vpf table, where each
 *			bit relates to the ID in the table.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Dave Flinn       October 1991
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
 *	read_thematic_index_header		see below
 *	set_init				SET.C
 *	set_insert
 *	Vpf_Read_* macros
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This module should be ANSI C compatible.
 *E
 *************************************************************************/
set_type search_thematic_index ( ThematicIndex *idx,
				 char *value )
{
  ossim_int32 			i , ival = 0, Match = -1;
  short int			sval = 0;
  float				fval = 0.0 ;
  double			dval = 0.0 , atof () ;
  char				*buf = (char *) NULL, bufalloc=0 ;
  ThematicIndexDirectory	d ;
  set_type			s ;

  /* open output index file */

  if (!idx->fp) {
     s = set_init(1);
     return s;
  }

  if ( idx->h.index_type == 'G' ) {
    s = search_gazetteer_index ( idx, value ) ;
    return s ;
  }

  if ( value ) 		/* search for pattern */
    switch ( idx->h.column_type ) {			/* using address */
    case	'I':
      memcpy ( &ival, value, sizeof (ossim_int32)) ;
      break ;
    case	'S':
      memcpy ( &sval, value, sizeof (short int)) ;
      break ;
    case	'F':
      memcpy ( &fval, value, sizeof (float)) ;
      break ;
    case	'R':
      memcpy ( &dval, value, sizeof (double)) ;
      break ;
    case	'T':
      buf = (char *) vpfmalloc ( idx->h.type_count ) ;
      /* leave value as is */
      bufalloc = 1;
      break ;
    }
  else
    SWhimper ( "require a value to search on.") ;

  /* create set */

  s = set_init ( idx->h.table_nrows ) ;

  /* Look for a matching directory entry */

  fseek(idx->fp,THEMATIC_INDEX_HEADER_SIZE,SEEK_SET);

  for ( i=0; i < idx->h.nbins; i++ ) {

    switch ( idx->h.column_type ) {
    case 	'I':
      Read_Vpf_Int ( &d.value.ival, idx->fp, 1) ;
      if ( ival == d.value.ival ) {
	/* found the match */
	Match = i ;
	i = idx->h.nbins ;	/* exit */
      }
      break ;
    case 	'S':         
      Read_Vpf_Short ( &d.value.sval, idx->fp, 1) ;
      if ( sval == d.value.sval ) {
	/* found the match */
	Match = i ;
	i = idx->h.nbins ;	/* exit */
      }
      break ;
    case 	'F':
      Read_Vpf_Float ( &d.value.fval, idx->fp, 1) ;
      if ( fval == d.value.fval ) {
	/* found the match */
	Match = i ;
	i = idx->h.nbins ;	/* exit */
      }
      break ;
    case 	'R':         
      Read_Vpf_Float ( &d.value.dval, idx->fp, 1) ;
      if ( dval == d.value.dval ) {
	/* found the match */
	Match = i ;
	i = idx->h.nbins ;	/* exit */
      }
      break ;
    case 	'T':
      Read_Vpf_Char ( buf, idx->fp, idx->h.type_count ) ;
      bufalloc = 1;
      if ( ! strcmp ( value, buf ) ) {
	/* found the match */
	Match = i ;
	i = idx->h.nbins ;	/* exit */
      }
      break ;
    } /* end of switch */

    Read_Vpf_Int ( &d.start_offset, idx->fp, 1) ;
    Read_Vpf_Int ( &d.num_items, idx->fp, 1) ;

  }  /* end of i loop */

  if ( bufalloc ) free ( buf ) ;

  if ( Match < 0 ) {
     return s;
  }

  /* We've got a match, so now read the ids in */

  if ( fseek ( idx->fp, d.start_offset, 0 ) != 0 )
    SWhimper ( "error in fseek") ;

  /* read data into user-defined pointer.  User must free this pointer */

  if ( idx->h.id_data_type == 'I' )
    for ( i=0 ; i < d.num_items; i++ ) {
      Read_Vpf_Int ( &ival, idx->fp, 1 ) ;
      set_insert ( ival, s ) ;			/* set the id in the set */
    }
  else
    for ( i=0 ; i < d.num_items; i++ ) {
      Read_Vpf_Short ( &sval, idx->fp, 1 ) ;
      set_insert ( (ossim_int32) sval, s ) ;
    }

  return s ;					/* also return set */

}   /*end of search_index */


/*************************************************************************
 *
 *N  close_thematic_index
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *	Close a thematic index.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    idx <input> == (ThematicIndex *) thematic index
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Dave Flinn       October 1991
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
 *	read_thematic_index_header		see below
 *	set_init				SET.C
 *	set_insert
 *	Vpf_Read_* macros
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This module should be ANSI C compatible.
 *E
 *************************************************************************/
void close_thematic_index ( ThematicIndex *idx )
{
  fclose ( idx->fp ) ;
  if (idx->d) free(idx->d);
  if (idx->gid) free(idx->gid);
}

/*************************************************************************
 *
 *N  create_gazetteer_index
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *	To create a gazetteer file
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *
 *    tablename  <input> == (char *) the name of an existing file
 *					<= 11 chars long
 *    idx_fname  <input> == (char *) name of index file
 *    columnname <input> == (char *) is a column existing in tablename
 *				        <= 24 chars long
 *
 *    idx_set <input> == (char *) is an array of printable ASCII characters 
 *				with no duplicates
 *
 *    return value < 0
 *
 *  	  -1 some OS error, check errno for something more exact
 *        -2 tablename is not a vpf table
 *        -3 columnname is not in the vpf table 
 *        -4 column type not text
 *
 *    return value = 0
 *         an index record for each character in index_set has been created
 *
 *         the index file name is the same as tablename but with .?ti
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    	Jim Tenbrink
 *	Dave Flinn       October 1991
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
 *	read_thematic_index_header		see below
 *	set_init				SET.C
 *	set_insert
 *	Vpf_Read_* macros
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This module should be ANSI C compatible.
 *E
 *************************************************************************/

ossim_int32 create_gazetteer_index (char *tablename,
				 char *idx_fname ,
				 char *columnname,
				 char *idx_set)
{
  vpf_table_type t;
  row_type       r;
  ossim_int32       c;
  FILE         * idx_fp;
  ThematicIndexHeader
                 gi;
  ThematicIndexDirectory
               * gid;
  set_type     * idx_bit_sets;
  register ossim_int32   i,
                      j;
  ossim_uint32 l,
                    set_byte_size;

  t = vpf_open_table(tablename, disk, "rb", NULL);
  c = table_pos(strupr(columnname), t);

  if (t.header[c].type != 'T') {
    vpf_close_table(&t);
    return -4;
  }

  idx_fp = fopen(idx_fname, "wb");

  if (idx_fp == NULL)
    return -1;

  /* only write out the table name, not the rest */

  for ( i = (long)strlen ( tablename ); i > 0; i-- )
    if ( tablename[i] == '/' ) break ;
  if ( i && (unsigned int)i < strlen (tablename) )
    strcpy ( gi.vpf_table_name, strupr ( &tablename[i+1] ) ) ;
  else
    strcpy(gi.vpf_table_name, strupr ( tablename) );
  strcpy(gi.vpf_column_name, columnname);

  gi.column_type = 'T';
  gi.index_type  = 'G';
  gi.type_count = 1 ;
  gi.id_data_type = 'S' ;
  gi.nbins       = (long)strlen(idx_set);
  gi.table_nrows = t.nrows;
  set_byte_size  = (unsigned int)ceil(t.nrows/8.0);

  /* force input string to lower case , rdf */

  for ( i=0; i < gi.nbins; i++ )
    idx_set[i] = tolower ( idx_set[i] ) ;

  /*
   * ti.nbytes is the offset into the index file at which the
   * bit arrays start
   */

  gi.nbytes = THEMATIC_INDEX_HEADER_SIZE +
              gi.nbins * ( sizeof (char) + DIRECTORY_SIZE ) ;

  gid = (ThematicIndexDirectory *) malloc(sizeof(ThematicIndexDirectory)
					  * gi.nbins);
  idx_bit_sets = (set_type *) malloc(sizeof(set_type) * gi.nbins);

  for (i = 0; i < gi.nbins; i++) {
    idx_bit_sets[i]   = set_init(t.nrows);
    gid[i].value.cval = idx_set[i];
    gid[i].num_items  = 1;
  }

  /*
   * lets suck up some CPU cycles here
   */
  for (l = 0; (int)l < t.nrows; l++) {

    r = get_row(l+1, t);

    for (i = 0; i < gi.nbins; i++)
      for (j = 0; j < r[c].count; j++)
	/* No matter the input character, make it lower case , rdf added */
        if ( tolower (*((char *) r[c].ptr + j)) == idx_set[i]) {
          set_insert(l, idx_bit_sets[i]);
          break;
        }

    free_row(r, t);

  }

  vpf_close_table(&t);

  if (write_thematic_index_header(gi, idx_fp) == (ossim_int32)0) {
    fclose(idx_fp);
    for (i = 0; i < gi.nbins; i++)
      set_nuke(&idx_bit_sets[i]);
    return -1;
  }

  if (write_gazetteer_index_directory(gi,
                                     gid,
                                     set_byte_size,
                                     idx_fp) == 0         ) {
    fclose(idx_fp);
    for (i = 0; i < gi.nbins; i++)
      set_nuke(&idx_bit_sets[i]);
    return -1;
  }

  for (i = 0; i < gi.nbins; i++) {
    if ( ! Write_Vpf_Char(idx_bit_sets[i].buf, idx_fp, set_byte_size) ) {
      fclose(idx_fp);
      for (i = 0; i < gi.nbins; i++)
        set_nuke(&idx_bit_sets[i]);
      return -1;
    }
  }

  fclose(idx_fp);
  for (i = 0; i < gi.nbins; i++)
    set_nuke(&idx_bit_sets[i]);
  free(idx_bit_sets);

  return 0;
}



/*************************************************************************
 *
 *N  read_gazetteer_index
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *	To read an gazeeteer index file, and return a set type array
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    idx_fname <input> == (char *) the name of a vpf gazetteer index
 *                         created with a call to to create_gaz_index.
 *
 *    query_str <input> == (char *) string that contains only characters
 *                         which form a subset of the characters passed
 *                         to create_gaz_index when idx_fname was created.
 *
 *      the vpf table from which the file idx_fname was derived must
 *      exist in the same directory as idx_fname
 *
 * out: case the returned set size > 0
 *        a bit array which can be used as a rough filter for the
 *        vpf table from which the index file idx_fname was derived.
 *    
 *      bits which are set in the array are the record numbers in the vpf table
 *        (from which idx_fname was derived) which may contain query_string
 *
 *      case the returned set size = 0
 *        some system error occurred
 *        most likely the indexed vpf table couldn't be found in the
 *        same directory
 *
 * note: query_str is treated as a set, so duplicate occurrences
 *       of the same character are ignored. I need a way of flagging
 *       those duplicate occurrences. Instead of sorting the string
 *       and removing duplicates, I use the num_items field in each
 *       element of the directory array.
 *
 *   Returns:
 *
 *    set_type		a set corresponding to the vpf table, where each
 *			bit relates to the ID in the table.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    	Jim Tenbrink
 * 	Dave Flinn       October 1991
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
 *	read_thematic_index_header		see below
 *	set_init				SET.C
 *	set_insert
 *	Vpf_Read_* macros
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This module should be ANSI C compatible.
 *E
 *************************************************************************/
set_type read_gazetteer_index (char *idx_fname, char *query_str )
{
  ThematicIndexHeader gi;
  ThematicIndexDirectory
                    * gid;
  FILE              * idx_fp;
  set_type            query_set = {0, 0},
		      xsect_set,
                      result_set;
  register int        query_len = (int)strlen(query_str),
                      i,
                      j;
  unsigned long       set_byte_size;
  char                query_char;

  idx_fp = fopen(idx_fname, "rb");

  if (idx_fp == NULL)
    return query_set;

  if (read_thematic_index_header (&gi, idx_fp) == (ossim_int32)0) {
    fclose(idx_fp);
    return query_set;
  }

  if (read_gazetteer_index_directory (&gid, &gi, idx_fp) == (ossim_int32)0) {
    fclose(idx_fp);
    return query_set;
  }

  for (j = 0; j < gi.nbins; j++)
    gid[j].num_items = 0;

  query_set = set_init(gi.table_nrows);
  xsect_set = set_init(gi.table_nrows);

  set_on(query_set);
  set_byte_size = (unsigned int)ceil(gi.table_nrows/8.0);

  for (i = 0; i < query_len; i++) {

    query_char = tolower(query_str[i]);

    for (j = 0; j < gi.nbins; j++)
      if (gid[j].value.cval == query_char)  {
        gid[j].num_items++;
        break;
      }
    if (gid[j].num_items > 1)
      continue;

    fseek(idx_fp, gid[j].start_offset, 0);
    if ( ! Read_Vpf_Char(xsect_set.buf, idx_fp, set_byte_size) ) {
      set_type err = set_init ( 0 ) ;
      return err ;
    }

    result_set = set_intersection(query_set, xsect_set);
    set_assign(&query_set, result_set);
    set_nuke(&result_set);

  }

  fclose(idx_fp);
  free(gid);
  set_nuke(&xsect_set);

  return query_set;
}

/*************************************************************************
 *
 *N  search_gazetteer_index
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *	To search an gazeeteer index file, and return a set type array
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 * in : idx is the gazetteer index opened with open_thematic_index
 *
 *      query_str contains only characters which form a subset of
 *      the characters passed to create_gaz_index when idx_fname
 *      was created
 *
 *      the vpf table from which the file idx_fname was derived must
 *      exist in the same directory as idx_fname
 *
 * out: case the returned set size > 0
 *        a bit array which can be used as a rough filter for the
 *        vpf table from which the index file idx_fname was derived.
 *    
 *      bits which are set in the array are the record numbers in the vpf table
 *        (from which idx_fname was derived) which may contain query_string
 *
 *      case the returned set size = 0
 *        some system error occurred
 *        most likely the indexed vpf table couldn't be found in the
 *        same directory
 *
 * note: query_str is treated as a set, so duplicate occurrences
 *       of the same character are ignored. I need a way of flagging
 *       those duplicate occurrences. Instead of sorting the string
 *       and removing duplicates, I use the num_items field in each
 *       element of the directory array.
 *
 *   Returns:
 *
 *    set_type		a set corresponding to the vpf table, where each
 *			bit relates to the ID in the table.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    	Jim Tenbrink
 * 	Dave Flinn       October 1991
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
 *	read_thematic_index_header		see below
 *	set_init				SET.C
 *	set_insert
 *	Vpf_Read_* macros
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This module should be ANSI C compatible.
 *E
 *************************************************************************/
set_type search_gazetteer_index (ThematicIndex *idx, char *query_str )
{
  set_type            query_set = {0, 0, 0, 0},
		      xsect_set,
                      result_set;
  register int        query_len = (int)strlen(query_str),
                      i,
                      j;
  unsigned long       set_byte_size;
  char                query_char;

  if (idx->fp == NULL)
    return query_set;

  for (j = 0; j < idx->h.nbins; j++)
    idx->gid[j].num_items = 0;

  query_set = set_init(idx->h.table_nrows);
  xsect_set = set_init(idx->h.table_nrows);

  set_on(query_set);
  set_byte_size = (unsigned int)ceil(idx->h.table_nrows/8.0);

  for (i = 0; i < query_len; i++) {

    query_char = tolower(query_str[i]);

    for (j = 0; j < idx->h.nbins; j++)
      if (idx->gid[j].value.cval == query_char)  {
	idx->gid[j].num_items++;
	break;
      }
    if (idx->gid[j].num_items > 1)
      continue;

    fseek(idx->fp, idx->gid[j].start_offset, 0);
    if ( ! Read_Vpf_Char(xsect_set.buf, idx->fp, set_byte_size) ) {
      set_type err = set_init ( 1 ) ;
      return err ;
    }

    result_set = set_intersection(query_set, xsect_set);
    set_assign(&query_set, result_set);
    set_nuke(&result_set);

  }

  set_nuke(&xsect_set);

  return query_set;
}

/*************************************************************************
 *
 *N  read_gazetteer_index_directory
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *	To read an index file, regardless of its type ( T or G ) and
 *	return a set type array
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 * in : *gid is undefined
 *
 *      *gi is the gazetteer index header for the open index file
 *      referenced by idx_fp
 *
 * out: case return value is one
 *        *gid is an array of directory entries with gi->nbin elements
 *
 *        the sval member of the value union contains the indexed character
 *
 *        the start_offset member of the directory structure contains the
 *        disk offset for the start of the indexed characters bit array
 *
 *      case zero return value
 *        an error was probably encountered during the read
 *        *gid is undefined
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Dave Flinn       October 1991
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
 *	read_thematic_index_header		see below
 *	set_init				SET.C
 *	set_insert
 *	Vpf_Read_* macros
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This module should be ANSI C compatible.
 *E
 *************************************************************************/
ossim_int32 read_gazetteer_index_directory(
				   ThematicIndexDirectory **gid,
				   ThematicIndexHeader     *gi,
				   FILE                    *idx_fp)
{
  int i;

#if UNIX
  if ( fseek ( idx_fp,sizeof (ThematicIndexHeader)-sizeof(ossim_int32), 0 )!= 0 )
#else
  if (fseek(idx_fp, sizeof (ThematicIndexHeader), 0) != 0)
#endif
    return 0 ;

  *gid = (ThematicIndexDirectory *) malloc(sizeof(ThematicIndexDirectory)
                                           * gi->nbins);
  if (*gid == NULL)
    return 0;

  for (i = 0; i < gi->nbins; i++) {
    if ( ( ! Read_Vpf_Char(  &( (*gid)[i].value.cval ),   idx_fp, 1) ) ||
	 ( ! Read_Vpf_Int(   &( (*gid)[i].start_offset ), idx_fp, 1) ) ||
	 ( ! Read_Vpf_Int(   &( (*gid)[i].num_items ),    idx_fp, 1) )) {
      return (ossim_int32)0 ;
    }
  }
  return 1;
}

/*************************************************************************
 *
 *N  read_thematic_index_header
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *	To read the thematic index header in a standard way
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *	h < in/out > == (ThematicIndexHeader *) header structure to be filled
 *	ifp <input>  == ( FILE *ifp ) index file pointer
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Dave Flinn       October 1991
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
 *	Read_Vpf_Int
 *	Read_Vpf_Char
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This module should be ANSI C compatible.
 *E
 *************************************************************************/

#define RWhimper() {\
   return (ossim_int32)0 ; }

ossim_int32 read_thematic_index_header ( ThematicIndexHeader *h, FILE *ifp ) 
{
  if ( fseek ( ifp, 0, 0 ) != 0 ) 	/* rewind, just in case */
    return 0 ;				/* error */
  if ( ! Read_Vpf_Int ( &h->nbytes, ifp, 1 ) )
    RWhimper();
  if ( ! Read_Vpf_Int ( &h->nbins, ifp, 1 ) )
    RWhimper() ;
  if ( ! Read_Vpf_Int ( &h->table_nrows, ifp, 1 ) )
    RWhimper() ;
  if ( ! Read_Vpf_Char ( &h->index_type, ifp, 1 ) )
    RWhimper() ;
  if ( ! Read_Vpf_Char ( &h->column_type, ifp, 1 ) )
    RWhimper() ;
  if ( ! Read_Vpf_Int ( &h->type_count, ifp, 1 ) )
    RWhimper() ;
  if ( ! Read_Vpf_Char ( &h->id_data_type, ifp, 1 ) )
    RWhimper() ;
  if ( ! Read_Vpf_Char ( &h->vpf_table_name, ifp, 12 ) )
    RWhimper() ;
  if ( ! Read_Vpf_Char ( &h->vpf_column_name, ifp, 25 ) )
    RWhimper() ;
  if ( ! Read_Vpf_Char ( &h->padding, ifp, 4 ) )
    RWhimper() ;
  return 1 ;
}

/*************************************************************************
 *
 *N  write_thematic_index_header
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *	To write the thematic index header in a standard way
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *	h < input > == (ThematicIndexHeader) header structure to write from
 *	ifp <input>  == ( FILE *ifp ) index file pointer
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Dave Flinn       October 1991
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
 *	Write_Vpf_Int
 *	Write_Vpf_Char
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This module should be ANSI C compatible.
 *E
 *************************************************************************/

#define WWhimper() {\
   return (ossim_int32)0 ; }

ossim_int32 write_thematic_index_header ( ThematicIndexHeader h, FILE *ifp ) 
{
  if ( fseek ( ifp, 0, 0 ) != 0 ) 	/* rewind, just in case */
    WWhimper() ;				/* error */
  if ( ! Write_Vpf_Int ( &h.nbytes, ifp, 1 ) )
    WWhimper() ;
  if ( ! Write_Vpf_Int ( &h.nbins, ifp, 1 ) )
    WWhimper() ;
  if ( ! Write_Vpf_Int ( &h.table_nrows, ifp, 1 ) )
    WWhimper() ;
  if ( ! Write_Vpf_Char ( &h.index_type, ifp, 1 ) )
    WWhimper() ;
  if ( ! Write_Vpf_Char ( &h.column_type, ifp, 1 ) )
    WWhimper() ;
  if ( ! Write_Vpf_Int ( &h.type_count, ifp, 1 ) )
    WWhimper() ;
  if ( ! Write_Vpf_Char ( &h.id_data_type, ifp, 1 ) )
    WWhimper() ;
  if ( ! Write_Vpf_Char ( &h.vpf_table_name, ifp, 12 ) )
    WWhimper() ;
  if ( ! Write_Vpf_Char ( &h.vpf_column_name, ifp, 25 ) )
    WWhimper() ;
  if ( ! Write_Vpf_Char ( &h.padding, ifp, 4 ) )
    WWhimper() ;
  return 1 ;
}


/*************************************************************************
 *
 *N  write_thematic_index_directory
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *	To write the thematic index header in a standard way
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *	h < input > == (ThematicIndexHeader) header structure
 *	d < input > == (ThematicIndexDirectory *) directory array structure
 *	idsize < input > == (ossim_int32) size of each data element.
 *		for T indexes, this is either 2 bytes or 4 bytes
 *		for G indexes, it will be num_in_set (set) 
 *			       or the size of the bit array.
 *	ifp <input>  == ( FILE *ifp ) index file pointer
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Dave Flinn       October 1991
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
 *	Vpf_Write_*
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This module should be ANSI C compatible.
 *E
 *************************************************************************/

#define WTWhimper() {\
   return (ossim_int32)0 ; }

ossim_int32 write_thematic_index_directory ( ThematicIndexHeader h, 
					  ThematicIndexDirectory *d,
					  ossim_int32 idsize , /* size of data */
					  FILE *ifp )
{
  long	int	offset = h.nbytes ,
                i ;

  /* rewind, just in case */
#if UNIX
  if ( fseek ( ifp, sizeof (ThematicIndexHeader)-sizeof(ossim_int32), 0 ) != 0 )
#else
  if ( fseek ( ifp, sizeof (ThematicIndexHeader), 0 ) != 0 )
#endif
    WTWhimper() ;

  for ( i=0; i < h.nbins; i++ ) {
    
    switch ( h.column_type ) {
    case 'I':
      if ( ! Write_Vpf_Int ( &d[i].value.ival, ifp, 1 ) )
	WTWhimper() ;
      break ;
    case 'T':
      /* Not tested for text strings */
      if ( ! Write_Vpf_Char ( &d[i].value.strval, ifp, h.type_count ) )
	WTWhimper() ;
      break ;
    case 'S':
      if ( ! Write_Vpf_Short ( &d[i].value.sval, ifp, 1 ) )
	WTWhimper() ;				
      break ;
    case 'F':
      if ( ! Write_Vpf_Float ( &d[i].value.fval, ifp, 1 ) )
	WTWhimper() ;				
      break ;
    case 'R':
      if ( ! Write_Vpf_Double ( &d[i].value.dval, ifp, 1 ) )
	WTWhimper() ;				
      break ;
    }	/* end of switch */

    if ( ! Write_Vpf_Int ( &offset, ifp, 1 ) )
	WTWhimper() ;				
    if ( ! Write_Vpf_Int ( &d[i].num_items, ifp, 1 ) )
	WTWhimper() ;				
    
    /* this offset is constant, no matter the data type */

    offset += ( idsize * d[i].num_items ) ;

  }  /* end of i loop */

  return 1 ;

}    /* end of write_directory */


/*************************************************************************
 *
 *N  write_gazetteer_index_directory
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *	To write the gazetteer thematic index header in a standard way
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *	h < input > == (ThematicIndexHeader) header structure 
 *	d < input > == (ThematicIndexDirectory *) directory array structure
 *	idsize < input > == (ossim_int32) size of each data element.
 *		for T indexes, this is either 2 bytes or 4 bytes
 *		for G indexes, it will be num_in_set (set) 
 *			       or the size of the bit array.
 *	ifp <input>  == ( FILE *ifp ) index file pointer
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Dave Flinn       October 1991
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
 *	Vpf_Write_*
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This module should be ANSI C compatible.
 *E
 *************************************************************************/

#define WTGWhimper() {\
  return (ossim_int32)0 ; }

ossim_int32 write_gazetteer_index_directory ( ThematicIndexHeader h, 
					  ThematicIndexDirectory *d, 
					  ossim_int32 idsize , /* size of data */
					  FILE *ifp )
{
  long	int	offset = h.nbytes ,
                i ;

  /* rewind, just in case */

  if ( fseek ( ifp, THEMATIC_INDEX_HEADER_SIZE, 0 )!= 0 )
    WTWhimper() ;				

  for ( i=0; i < h.nbins; i++ ) {
    
    if ( ! Write_Vpf_Char ( &d[i].value.cval, ifp, 1 ) )
      WTGWhimper() ;

    if ( ! Write_Vpf_Int ( &offset, ifp, 1 ) )
	WTGWhimper() ;				
    if ( ! Write_Vpf_Int ( &d[i].num_items, ifp, 1 ) )
	WTGWhimper() ;				
    
    /* this offset is constant, no matter the data type */

    offset += ( idsize * d[i].num_items ) ;

  }  /* end of i loop */

  return 1 ;

}    /* end of write_directory */


