/*       static char SCCS_ID[] = "@(#) vpftidx.h 1.5 10/9/91" ;            */
/*************************************************************************
 *
 * Environmental Systems Research Institute (ESRI) Applications Programming
 *
 *N  Module VPFTIDX
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *P
 *   Purpose: To create a thematic index on a VPF table column.
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
 *    Dave Flinn       September 1991  
 *E
  *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *
 *   Portability:
 *O
 *    This module should be ANSI C compatible
 *E
 *************************************************************************/

#ifndef _VPFTIDX_
#define _VPFTIDX_
#ifdef __cplusplus
extern "C" {
#endif
   
#include <ossim/vpfutil/set.h>
#include <ossim/base/ossimConstants.h>
   
typedef struct {			/* Total of 40 bytes */
  ossim_int32	nbytes ,		/* 40 + directory length */
                nbins ,			/* Directory size */
                table_nrows ;		/* Num rows in original table */
  char		index_type ,		/* T = thematic, G = gazetteer */
        	column_type ;		/* T, I, R, S, F, K */
  ossim_int32      type_count ;		/* usually 1, but more for T */
  char 		id_data_type ,		/* I if > 32767, else S */
                vpf_table_name[12] ,
                vpf_column_name[25] ,	/* 16 bytes + 9 for TILE_ID */
                padding[4] ;		/* To make it a nice 60 bytes */
} ThematicIndexHeader ;

#define         THEMATIC_INDEX_HEADER_SIZE    60

#define         DIRECTORY_SIZE                (sizeof(ossim_int32)*2)

#define		MAX_ID		32767	/* Threshold between S and I ids */

typedef struct {			/* length = sizeof (datatype) + */
  union {				/* 	    8 * nbins           */
    char	cval ,
                *strval ;
    ossim_int32	ival ;
    short int	sval ;
    float	fval ;
    double 	dval ;
  } value ;
  ossim_int32	start_offset ,
                num_items ;		/* For each value, count the ids */
  /* Store the row ids here */
/*  char    	*ids ;*/			/* depending on id_data_type, this */
  set_type idset;
} ThematicIndexDirectory ;

typedef struct {
   ThematicIndexHeader h;
   ThematicIndexDirectory *d, *gid;
   FILE *fp;
} ThematicIndex;

/* PROTOTYPES */

/*char* strupr(char*);*/

set_type	read_thematic_index ( char *tablename,
				       char *value ) ;

ThematicIndex	open_thematic_index ( char *tablename );

set_type	search_thematic_index ( ThematicIndex *themindex,
					char *value ) ;

void	        close_thematic_index ( ThematicIndex *themindex );

set_type 	read_gazetteer_index (char * vpfname, char * query_str ) ;

set_type 	search_gazetteer_index (ThematicIndex *idx,
					char * query_str ) ;

ossim_int32 	read_gazetteer_index_directory (
						ThematicIndexDirectory ** gid,
						ThematicIndexHeader     * gi,
						FILE * idx_fp);



ossim_int32	write_thematic_index_header ( ThematicIndexHeader h ,
					      FILE *fp ) ;

ossim_int32	read_thematic_index_header ( ThematicIndexHeader *h ,
					     FILE *fp ) ;

ossim_int32	write_thematic_index_directory ( 
				       ThematicIndexHeader h ,
				       ThematicIndexDirectory *d,
				       ossim_int32 size,
				       FILE *fp ) ;

ossim_int32	write_gazetteer_index_directory ( 
				       ThematicIndexHeader h ,
				       ThematicIndexDirectory *d,
				       ossim_int32 size,
				       FILE *fp ) ;



ossim_int32        create_thematic_index ( char indextype,
		 	                char *tablename,
                                        char *idxname ,
				        char *columnname,
				        char *idx_set );

ossim_int32        create_gazetteer_index (char *tablename,
				        char *idx_fname ,
				        char *columnname,
				        char *idx_set);

#ifdef __cplusplus
}
#endif
   
#endif
