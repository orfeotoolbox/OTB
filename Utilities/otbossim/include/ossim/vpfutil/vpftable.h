/* ================ SCCS_ID[] = "@(#) vpftable.h 2.2 10/29/91"  =================

   Environmental Systems Research Institute (ESRI) Applications Programming

       Project: 		Conversion from ARC/INFO to VPF
       Original Coding:		Barry Michaels	April 1991
       Modifications:		David Flinn	April 1991
                                Barry           June 1991
                                Dave            October 1991
   ======================================================================== */

/* VPFTABLE.H */

#ifndef _VPF_TABLE_H_

#define _VPF_TABLE_H_

#include <stdio.h>
#include <ossim/vpfutil/machine.h>

#include <ossim/ossimConfig.h>
#ifdef __cplusplus
extern "C" {
#endif

#ifndef TRUE
#define TRUE  1
#endif
#ifndef FALSE
#define FALSE 0
#endif

/* Possible byte ordering of the data */
#define LEAST_SIGNIFICANT 0
#define MOST_SIGNIFICANT  1
int vpfutilMachineByteOrder();
/* The next definition is machine-specific and may need to be changed */
/* before recompiling on a different machine */
#define MACHINE_BYTE_ORDER (vpfutilMachineByteOrder())



#include <ossim/vpfutil/vpfio.h>

/* This should be the ISO definition of date */

typedef char date_type[21] ;	/* Include null end of string */

/* NULL valuse type */

typedef union {
   char 	  *Char ;
   short int	  Short ;
   ossim_int32       Int ;
   float	  Float ;
   double	  Double ;
   date_type	  Date ;
   char		  Other ;
} null_field;

/* The basic information carried for each field in the table */
typedef struct {
   char *name;           /* Name of the field */
   char description[81]; /* Field description */
   char keytype;         /* Type of key - (P)rimary, (F)oreign, (N)onkey */
   char vdt[13];         /* Value description table name */
   char *tdx;	 	 /* Thematic index file name */
   char type;            /* Data type - T,I,F,K,D */
   ossim_int32  count;      /* Number of items in this column (-1 =>variable)*/
   null_field nullval ;  /* This is used for the converter */
   char *narrative;      /* Name of a narrative table describing the field*/
} header_cell, *header_type;

typedef enum { ram, disk, either, compute } storage_type;
#define RAM 0
#define DISK 1
#define EITHER 2
#define COMPUTE 3

typedef enum { Read, Write } file_mode ;

#define CLOSED 0
#define OPENED 1

/* Each column in a table row has a count and a pointer to the data */
/*      and a null value default */
typedef struct {
   ossim_int32 count;
   void *ptr;
} column_type;

/* A table row is an array of columns */
typedef column_type *row_type;

/* Index for variable width tables.          */
/* One index cell for each row in the table. */
typedef struct {
   ossim_uint32 pos;
   ossim_uint32 length;
} index_cell, *index_type;

/* VPF table structure: */
typedef struct {
   char           name[13];        /* Name of the VPF table */
   char           *path;           /* Directory path to the table */
   ossim_int32       nfields;         /* Number of fields */
   char           description[81]; /* Table description */
   char           narrative[13];   /* Table narrative file name */
   header_type    header;          /* Table header structure */
   FILE           *xfp;            /* Index file pointer */
   index_type     index;           /* Index structure */
   storage_type   xstorage;        /* Flag indicating where index stored */
   FILE           *fp;             /* Table file pointer */
   ossim_int32       nrows;           /* Number of rows in the table */
   row_type       *row;            /* Array of table rows */
   ossim_int32       reclen;          /* Table record length (-1 => variable */
   ossim_int32       ddlen;           /* Data definition string length */
   char           *defstr ;        /* rdf, definition string */
   storage_type   storage;         /* Flag indicating table storage method */
   file_mode	  mode ;	   /* Table is either reading or writing */
   unsigned char  status;          /* VPF table status - OPENED or CLOSED */
   unsigned char  byte_order;      /* Byte order of the table's data */
} vpf_table_type;

typedef struct {
   float x,y;
} coordinate_type;

typedef struct {
   double x,y;
} double_coordinate_type;

typedef struct {
   float x,y,z;
} tri_coordinate_type;

typedef struct {
   double x,y, z;
} double_tri_coordinate_type;

/* These macros help determine the type in the key datatype */

#define TYPE0(cell) ((cell>>6)&(3))
#define TYPE1(cell) ((cell>>4)&(3))
#define TYPE2(cell) ((cell>>2)&(3))
#define TYPE3(cell) ((cell)&(3))

/* These macros set the value in the key datatype */

#define SETTYPE0(cell,value) cell = (((cell)&(~(3<<6)))|(((3)&(value))<<6))
#define SETTYPE1(cell,value) cell = (((cell)&(~(3<<4)))|(((3)&(value))<<4))
#define SETTYPE2(cell,value) cell = (((cell)&(~(3<<2)))|(((3)&(value))<<2))
#define SETTYPE3(cell,value) cell = (((cell)&(~(3)))|(((3)&(value))))

/* This macro helps to write out a key */

#define ASSIGN_KEY(tYPE,kEY,loc,val)\
{ \
   if (val < 1) \
   {  \
      tYPE(kEY.type,0);  \
   } else if (val < (1<<8)) \
   {  \
      tYPE(kEY.type,1);  \
      kEY.loc = val ;  \
   } else if ( val < (1<<16)) \
   {  \
      tYPE(kEY.type,2);  \
      kEY.loc = val;  \
   } else \
   {  \
      tYPE(kEY.type,3);  \
      kEY.loc = val; \
   } \
}

/* define NULL values */
#if defined(__CYGWIN__) || defined(__APPLE__) || defined(USING_VISUALAGE) || defined(_WIN32)
#include <ossim/vpfutil/values.h>
#else
#include <ossim/vpfutil/values.h>
#endif

#include <math.h>

#ifdef __MSDOS__
double quiet_nan(int unused);
#endif

int is_vpf_null_float( float num );
int is_vpf_null_double( double num );

#define		VARIABLE_STRING_NULL_LENGTH	10
#define 	NULLCHAR	' '
#define		NULLTEXT	" "
#define		NULLSHORT	-MAXSHORT
#define         NULLINT         -MAXLONG
#define		NULLDATE	"                    "
/* #if UNIX */
#define		NULLFLOAT	((float) quiet_nan (0))
#define		NULLDOUBLE	((double) quiet_nan (0))
/* #else
#define		NULLFLOAT	((float) MAXFLOAT)
#define		NULLDOUBLE	((double) MAXFLOAT)
#endif  */

typedef union {
   unsigned char f1;
   unsigned short int f2;
   ossim_uint32 f3;
} key_field;

/* id triplet internal storage type */
typedef struct {
   unsigned char type;
   ossim_int32 id, tile, exid;
} id_triplet_type;

typedef enum { idle_state, name_state, type_state,
	       tuple_state, count_state } ddef_state_type;

/* Functions: */

char *get_string(ossim_int32 *ind,char *src,char delimit);
char  get_char  (ossim_int32 *ind,char *src);
ossim_int32   get_number(ossim_int32 *ind,char *src,char delemit);

ossim_int32   parse_data_def( vpf_table_type *table );

char      *read_text_defstr( FILE *infile, FILE *outerr );

ossim_int32  index_length( ossim_int32 row_number,
		        vpf_table_type table );

ossim_int32  index_pos( ossim_int32 row_number,
		     vpf_table_type table );
#if 0
ossim_int32 row_offset( int field,
		     row_type row,
		     vpf_table_type table);
#endif

row_type  read_next_row( vpf_table_type table );

row_type  read_row( ossim_int32 row_number,
		    vpf_table_type table );

vpf_table_type vpf_open_table( const char *tablename,
			       storage_type storage,
			       const char *mode,
			       char *defstr );   /* rdf added */

row_type get_row( ossim_int32 row_number,
		  vpf_table_type table );

void free_row( row_type row, vpf_table_type table );

ossim_int32 table_pos( const char *field_name,
                    vpf_table_type table );

void *get_table_element( ossim_int32 field_number,
			 row_type row,
			 vpf_table_type table,
			 void *value,
			 ossim_int32  *count );

void *named_table_element( char *field_name,
			   ossim_int32  row_number,
			   vpf_table_type table,
			   void *value,
			   ossim_int32  *count );

void *table_element( ossim_int32 field_number,
		     ossim_int32  row_number,
		     vpf_table_type table,
		     void *value,
		     ossim_int32  *count );

void vpf_close_table( vpf_table_type *table );

void vpf_dump_table( char *tablename, char *outname );

ossim_int32 is_vpf_table( const char *fname );

/* Write functions */

ossim_int32 write_next_row( row_type row, vpf_table_type *table );

row_type create_row( vpf_table_type table );

void nullify_table_element( ossim_int32 field, row_type row,
			    vpf_table_type table );

ossim_int32 put_table_element( ossim_int32 field, row_type row,
			    vpf_table_type table,
			    void *value, ossim_int32 count );
void vpf_dump_table( char *tablename, char *outname );
void vpf_dump_doc_table( char *tablename, char *outname );

void swap_two(char*, char*);
void swap_four(char*, char*);
void swap_eight(char*, char*);

#if MAIN
  FILE * errorfp = stderr;
#else
  extern FILE * errorfp;
#endif
#if defined(__cplusplus)
}
#endif

#endif     /* #ifndef _VPF_TABLE_H_  */


