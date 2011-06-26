/*************************************************************************
 *
 * Environmental Systems Research Institute (ESRI) Applications Programming
 *
 *N  Module VPFTABLE.C
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This module contains functions to open, close and parse VPF relational
 *     tables.  VPF tables are defined as being a set of rows and columns.
 *     Each column may contain a single value, a fixed array of values,
 *     or a variable number of values.
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
 *    Barry Michaels   April 1991  - DOS Turbo C.
 *    Mody Buchbinder  May 1991    - Modified parse_data_def for new table
 *                                   header structure.
 *    Dave Flinn       July 1991   - Updated file for UNIX.
 *    JTB              10/91       - split off read routines into vpfread.c
 *                                   merged view and converter branches of
 *                                   this module; replaced various aborts()
 *                                   and exits with return codes
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    (function) displayerror(char *text[], int nlines) - function
 *       specified by the application to display a message to the user.
 *       It should return an integer indicating whether to retry (1) or
 *       not (0).
 *    FILE *errorfp - file pointer to error file.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *    N/A
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This module should be ANSI C compatible.
 *    Updated to port to UNIX platforms with big-endian architectures
 *E
 *************************************************************************/

#include <ossim/vpfutil/machine.h>
#ifdef CARBON
#define MACHINE_BYTE_ORDER 0
#endif

#ifdef __MSDOS__
#include <mem.h>
#include <io.h>
#include <alloc.h>
#include <dos.h>
#include <graphics.h>
#else
#ifndef _WIN32
#define UNIX 1
#include <unistd.h>
#endif
#endif

#if UNIX
#include <sys/stat.h>
#include <sys/types.h>
#define   getch()               /* Make this a null function */
#define   farcoreleft()	" "
#endif

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <ossim/vpfutil/values.h>
#include <ossim/vpfutil/vpftable.h>
#include <ossim/vpfutil/vpftidx.h>
#include <ossim/vpfutil/vpfmisc.h>

#ifndef __64K
#define __64K 65536
#endif

/*
 * per heap block byte overhead
 */
#define HEAP_OVERHEAD 4

/* Global variable shared by VPFTABLE.C, VPFREAD.C, and VPFWRITE.C  */
/* The above modules are logically one, only separated by file size */
/* constraints. */
int STORAGE_BYTE_ORDER = LEAST_SIGNIFICANT;  /* default */


int  vpfutilMachineByteOrder()
{
   unsigned short  test = 0x0001;
   unsigned char*  testPtr=NULL;
   
   testPtr       = (unsigned char*)(&test);
   return (testPtr[0] ? LEAST_SIGNIFICANT : MOST_SIGNIFICANT);
}

/* Include these to decrease external module dependencies */

#if 0 /* Defined in vpfmisc??? (drb) */
static char *rightjust( char *str )
{
   register int  len,i;

   len = strlen(str);
   i = len - 1;
   while ((i>0) && ((str[i]==0) || (str[i]==' '))) i--;
   if (i < (len-1)) str[i+1] = '\0';
   for (i=0;i<strlen(str);i++) if (str[i]=='\n') str[i] = '\0';
   return str;
}
#endif



void swap_two ( char *in, char *out )
{
  out[0] = in[1] ;
  out[1] = in[0] ;
}

void swap_four ( char *in, char *out )
{
  out[0] = in[3] ;
  out[1] = in[2] ;
  out[2] = in[1] ;
  out[3] = in[0] ;
}

void swap_eight ( char *in, char *out )
{
  out[0] = in[7] ;
  out[1] = in[6] ;
  out[2] = in[5] ;
  out[3] = in[4] ;
  out[4] = in[3] ;
  out[5] = in[2] ;
  out[6] = in[1] ;
  out[7] = in[0] ;
}

/* Return floating point Not a Number (for NULL values) */
double quiet_nan( int unused )
{
   char nanstr[8] = {-1,-1,-1,-1,-1,-1,-1,127};
   double n;
   memcpy(&n,&nanstr[0],sizeof(n));
   if (unused) return n;
   return n;
}

int is_vpf_null_float( float num )
{
   float nan;
   nan = (float)quiet_nan(0);
   if (memcmp(&nan,&num,sizeof(float))==0) return 1;
   return 0;
}

int is_vpf_null_double( double num )
{
   double nan;
   nan = (double)quiet_nan(0);
   if (memcmp(&nan,&num,sizeof(double))==0) return 1;
   return 0;
}


/***********Mody B**********/
/* get string until delimeter */
static char *cpy_del(char *src, char delimiter, ossim_int32 *ind )
{
  ossim_int32 i, skipchar ;
  char *temp, *tempstr ;

  /* remove all blanks ahead of good data */

  skipchar = 0 ;
  while ( src[skipchar] == SPACE || src[skipchar] == TAB )
    skipchar++ ;

  temp = &src[skipchar];

  /* If t
  he first character is a COMMENT, goto LINE_CONTINUE */

  if ( *temp == COMMENT ) {
    while ( *temp != LINE_CONTINUE && *temp != END_OF_FIELD && *temp != '\0'){
      temp++ ;
      skipchar ++ ;
    }
    skipchar++ ;
    temp++ ;		/* skip past LC, EOF, or NULL */
  }

  /* Start with temporary string value */

  tempstr = (char *)vpfmalloc ( (unsigned long)strlen ( temp ) + 10 ) ;

  if ( *temp == '"' ) {	/* If field is quoted, do no error checks */

    temp++ ; 	  /* skip past quote character */
    skipchar++ ;  /* update the position pointer */

    for ( i=0 ; *temp != '\0'; temp++,i++) {
      if ( *temp == LINE_CONTINUE || *temp == TAB ) {
	temp++ ;
	skipchar++ ;
      } else if ( *temp == '"' )
	break ;
      /* Now copy the char into the output string */
      tempstr[i] = *temp ;
    }
    tempstr[i] = '\0';		/* terminate string */
    *ind += ( i + skipchar + 2) ;	/* Increment position locate past */
    return tempstr ;			/* quote and semicolon */
  }

  /* search for delimiter to end, or end of string */

  i=0 ;	/* initialize */

  if ( *temp != END_OF_FIELD ) {	/* backward compatability check */

    for ( i=0; *temp != '\0';temp++,i++){/* Stop on NULL*/

      if ( ( *temp == LINE_CONTINUE && *(temp+1) == '\n') ||  *temp == TAB ) {
	temp++ ;
	skipchar++ ;
      } else if ( *temp == delimiter )
	break ;					/* break for delimiter  */
      /* Now copy the char into the output string */
      tempstr[i] = *temp ;
    }
                             /* Eat the delimiter from ind also */
    *ind += ( i + skipchar + 1) ;	/* Increment position locate */
  }	
  tempstr[i] = '\0';		/* terminate string */   
  return tempstr;
}
/***********Mody B*********/
char *get_string(ossim_int32 *ind,char *src,char delimeter )
{ char *temp;
  temp  = cpy_del(&src[*ind],delimeter, ind);
  if( ! strcmp ( temp, TEXT_NULL ))
    strcpy ( temp, "" ) ;
  return temp;
}
/**********Mody B*************/
char vpf_get_char(ossim_int32 *ind, char *src)
{  char temp;
   while ( src[*ind] == SPACE || src[*ind] == TAB ) (*ind)++ ;
   temp  = src[*ind];
   *ind += 2;
   return temp;
}
/***********Mody B***********/
ossim_int32 get_number(ossim_int32 *ind, char *src,char delimeter)
{  char *temp;
   ossim_int32  num;
   temp  = cpy_del(&src[*ind],delimeter, ind);
   if (strchr(temp, VARIABLE_COUNT ) == NULL)
      num = atoi(temp);
   else
      num = -1;
   free(temp);
   return num;
}


/*************************************************************************
 *
 *N  parse_data_def
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function parses a table's data definition and creates a header
 *     in memory that is associated with the table.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    table <inout> == (vpf_table_type *) vpf table structure.
 *    ddlen <input> == (ossim_int32) length of the table's data definition.
 *
 *    return value is the record length if all items are fixed length, or
 *    -1 if the record contains variable length items
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   April 1991                        DOS Turbo C
 *    Mody Buchbinder  May 1991 - Modified for new table header.
 *    Dave Flinn       July 1991 - updated for UNIX
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
 *    void *vpfmalloc()                                  VPFMISC.C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This module should be ANSI C compatible.
 *E
 *************************************************************************/
ossim_int32 parse_data_def( vpf_table_type *table )
{
   register ossim_int32 n,i;
   ossim_int32 p, k;
   char *buf,*des,*nar,*vdt, *tdx, *doc, byte ;/*temporary storage */
   char end_of_rec;
   int status;
   ossim_int32 ddlen;
   ossim_int32 reclen = 0;
#if UNIX
   /* just for backward compat check */
   ossim_int32 bkcompat = 1 ;
#endif

   if ( table->mode == Read ) {
     fread(&ddlen,sizeof(ddlen),1,table->fp);

     /* Check the next byte to see if the byte order is specified */
     fread(&byte,1,1,table->fp);
     p=0;
     table->byte_order = LEAST_SIGNIFICANT; /* default */
     switch (toupper(byte)) {
	case 'L':
	   p++;
	   break;
	case 'M':
	   table->byte_order = MOST_SIGNIFICANT;
	   p++;
	   break;
     }
     if (MACHINE_BYTE_ORDER != table->byte_order) {
	k = ddlen;
	swap_four((char *)&k,(char *)&ddlen);
     }
     if ( ddlen < 0 ) {
       return (ossim_int32)0 ;
     }

     STORAGE_BYTE_ORDER = table->byte_order;

     /* header without first 4 bytes */
     table->ddlen = ddlen + sizeof (ossim_int32) ;
     buf = (char *)vpfmalloc((ddlen+3)*sizeof(char));
     buf[0] = byte; /* already have the first byte of the buffer */
     Read_Vpf_Char(&buf[1],table->fp,ddlen-1) ;
   } else {
     table->ddlen = (long)strlen ( table->defstr ) ;
     ddlen = table->ddlen ;
     buf = (char *)vpfmalloc((ddlen+3)*sizeof(char));
     strncpy ( buf, table->defstr, ddlen ) ;
     p=0;
     table->byte_order = LEAST_SIGNIFICANT; /* default */
     byte = buf[0];
     switch (toupper(byte)) {
	case 'L':
	   p++;
	   break;
	case 'M':
	   table->byte_order = MOST_SIGNIFICANT;
	   p++;
	   break;
     }
     STORAGE_BYTE_ORDER = table->byte_order;
   }

   buf[ddlen-1] = '\0'; /* mark end of string for reading functions */
   if ( buf[p] == ';' )
     p++; /* buf[p] is semi-colon */
   des = get_string(&p,buf,COMPONENT_SEPERATOR );
   strncpy(table->description,des,80);
   free(des);
   nar = get_string(&p,buf,COMPONENT_SEPERATOR );
   strncpy(table->narrative ,nar,12);
   free(nar);
   n = 0 ;
   /* get number of fields */
   for (i=p; i < ddlen;i++)
     if ( buf[i] == LINE_CONTINUE )
       i++ ;	/* skip past line continue, and next character */
     else if (buf[i] == END_OF_FIELD ) 		/* Found end of field */
	n++;					/* increment nfields */
     else if (buf[i] == COMMENT )		/* skip past comments */
       while ( buf[i] != LINE_CONTINUE &&
	       buf[i] != END_OF_FIELD &&
	       buf[i] != '\0')
	 i++ ;					/* increment i */

   table->nfields = n ;
   table->header = (header_type)vpfmalloc((n+1)*sizeof(header_cell));

   for(i=0;i<n;i++) {
     end_of_rec = FALSE;
     table->header[i].name  = get_string(&p,buf, FIELD_COUNT);  /*****/
     rightjust(table->header[i].name);
     table->header[i].type  = toupper(vpf_get_char  (&p,buf));
     table->header[i].count = get_number(&p,buf,FIELD_SEPERATOR );

     if ( i == 0 )
       if ( ossim_strcasecmp ( table->header[0].name, "ID" ) ) {
	      return (ossim_int32)0 ;
       }

     if(table->header[i].count == -1)
       reclen = -1;			/* set reclen to variable len flag */

     /* Now set null values and add up record length, if fixed length */

     status = 0;

     switch (table->header[i].type) {
     case 'I':
       if ( reclen >= 0 )
	 reclen += (sizeof(ossim_int32)*table->header[i].count);
       table->header[i].nullval.Int = NULLINT ;
       break;
     case 'S':
       if ( reclen >= 0 )
	 reclen += (sizeof(short int)*table->header[i].count);
       table->header[i].nullval.Short = NULLSHORT ;
       break;
     case 'F':
       if ( reclen >= 0 )
	 reclen += (sizeof(float)*table->header[i].count);
       table->header[i].nullval.Float = NULLFLOAT ;
       break;
     case 'R':
       if ( reclen >= 0 )
	 reclen += (sizeof(double)*table->header[i].count);
       table->header[i].nullval.Double = NULLDOUBLE ;
       break;
     case 'T':
       if ( reclen >= 0 ) { 		/* if fixed length */
	 reclen += (sizeof(char)*table->header[i].count);
	 table->header[i].nullval.Char =
	   (char *) vpfmalloc ( table->header[i].count + 1 ) ;
	 for ( k=0; k < table->header[i].count; k++ )
	   table->header[i].nullval.Char[k] = NULLCHAR ;
	 table->header[i].nullval.Char[k] = '\0';
       } else {			/* variable length */
	 table->header[i].nullval.Char =
	   (char *) vpfmalloc ( VARIABLE_STRING_NULL_LENGTH + 1 ) ;
	 for ( k=0; k < VARIABLE_STRING_NULL_LENGTH ; k++ )
	   table->header[i].nullval.Char[k] = NULLCHAR ;
	 table->header[i].nullval.Char[k] = '\0';
       }
       break;
     case 'C':
       if ( reclen >= 0 )
	 reclen += (sizeof(coordinate_type)*table->header[i].count);
       table->header[i].nullval.Other = '\0';
       break;
     case 'Z':
       if ( reclen >= 0 )
	 reclen += (sizeof(tri_coordinate_type)*table->header[i].count);
       table->header[i].nullval.Other = '\0' ;
       break;
     case 'B':
       if ( reclen >= 0 )
	 reclen += (sizeof(double_coordinate_type)*table->header[i].count);
       table->header[i].nullval.Other = '\0' ;
       break;
     case 'Y':
       if ( reclen >= 0 )
	 reclen +=
	   (sizeof(double_tri_coordinate_type)*table->header[i].count);
       table->header[i].nullval.Other ='\0';
       break;
     case 'D':
       if ( reclen >= 0 )
	 reclen += ((sizeof(date_type)-1)*table->header[i].count);
       strcpy ( table->header[i].nullval.Date, NULLDATE ) ;
       break;
     case 'K':
       reclen = -1;
       table->header[i].nullval.Other = '\0' ;
       break;
     case 'X':
       /* do nothing */
       table->header[i].nullval.Other = '\0' ;
       break ;
     default:
       status = 1;
       break ;
     } /** switch type **/

     if (status) return (ossim_int32)0;

     table->header[i].keytype     = vpf_get_char  (&p,buf);
     des = get_string(&p,buf, FIELD_SEPERATOR );
     rightjust(des);
     strncpy(table->header[i].description,des,80);
     free(des);
     vdt = get_string(&p,buf, FIELD_SEPERATOR );
     strncpy(table->header[i].vdt,vdt,12);
     free(vdt);
#if UNIX
     /* This is for backward compatability qc checking */
     if ( bkcompat && buf[p] == END_OF_FIELD ) {
       bkcompat= 0 ;
     }
#endif
     tdx = get_string(&p,buf, FIELD_SEPERATOR ) ;
     if ( ! strcmp ( tdx, "" ) ) {
       table->header[i].tdx = (char *) NULL ;
       if (buf[p] == ':')
	 end_of_rec = TRUE;
     } else {
       if (strcmp(tdx,"-") != 0) {
	  table->header[i].tdx =(char*) vpfmalloc ( (unsigned long)strlen ( tdx ) +1 ) ;
	  strcpy (table->header[i].tdx, tdx );
       } else table->header[i].tdx = (char *)NULL;
     }
     free(tdx);
     if (!end_of_rec) {
	doc = get_string(&p,buf, FIELD_SEPERATOR ) ;
	if ( ! strcmp ( doc, "" ) ) {
	  table->header[i].narrative = (char *) NULL ;
	  end_of_rec = TRUE;
	} else {
	  if (strcmp(doc,"-") != 0) {
	     table->header[i].narrative = (char*)vpfmalloc ( (unsigned long)strlen(doc) +1) ;
	     strcpy (table->header[i].narrative, doc );
	  } else table->header[i].narrative = (char *)NULL;
	}
	free(doc);
     } else table->header[i].narrative = (char *)NULL;
     p += 1; /** eat semicolon **/
    }
   free(buf);
   return reclen;
}


/*************************************************************************
 *
 *N  vpfhandler
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function temporarily takes the place of the DOS hard error
 *     handler.  It is needed when 'vpfopencheck' replaces the DOS error
 *     handler for disk operations, so that the disk errors may be handled
 *     more suitably for our environment.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    May 1991                       DOS Turbo C
 *    Dave Flinn        July 1991  Updated for UNIX, basically commented
 *                                 the subroutine out
 *E
 *************************************************************************/
/*static int disk_error;*/
#define IGNORE 0
#define RETRY  1
#define ABORT  2
#define DISPLAY_STRING 0x09

#ifdef __MSDOS__
static int vpfhandler(ossim_int32 errval, ossim_int32 ax, ossim_int32 bp, ossim_int32 si)
{
   if (ax < 0) {
      bdosptr(DISPLAY_STRING,"device error$", 0);
      hardretn(-1);
   }
   disk_error = errval;

   si++; bp++;  /* get rid of compiler warning */
   return(IGNORE);
}
#endif


/*************************************************************************
 *
 *N  vpfopencheck
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function tries its darndest to open a file.  It initially calls
 *     fopen with the given filename and mode.  If that doesn't work and
 *     the file is not on the hard disk, it displays a message asking the
 *     user to enter the correct disk in the drive, waits for either a retry
 *     or a cancel response, and, if told to retry, tries again.  This
 *     process is repeated until either the file is opened or the user
 *     requests cancel.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    filename <input> == (char *) full path name of the file to be opened.
 *    mode     <input> == (char *) mode of the file.
 *    diskname <input> == (char *) descriptive name of the disk the file is
 *                                 on.
 *    return  <output> == (FILE *) file pointer newly associated with
 *                                 filename.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    May 1991                       DOS Turbo C
 *    Dave Flinn        July 1991     Updated for UNIX
 *E
 *************************************************************************/
FILE *vpfopencheck( const char *filename,
		    const char *mode,
		    const char * diskname )
{
   FILE *fp;
   char* tmpFilename = (char*) malloc ( strlen(filename) + 1 );
/*
   char *text[] = {"Please insert",
		   "                                        ",
		   "in data drive",
		   "                                                      "};
*/
   ossim_int32 retry = 0;
#ifdef __MSDOS__
   extern char home[255];
   void interrupt (*doshandler)();

   doshandler = getvect(36);
   harderr(vpfhandler);

   strncpy(text[1],diskname,strlen(text[1]));
   strcpy(text[3],filename);
#endif
   fp = NULL;


   /* copy the filename because we might modify it. */
   tmpFilename[strlen(filename)] = '\0'; /* just in case */
   strcpy(tmpFilename, filename);
   
   while (fp == NULL)
   {
      fp = fopen(tmpFilename,mode);
      if (fp == NULL)
      {
#ifdef __MSDOS__
	 if ( toupper(home[0]) != toupper(tmpFilename[0]) )
	    retry = displayerror(text,   4);
	 else
#else
            /* give names ending in dot another chance without the dot */
            if (tmpFilename[strlen(tmpFilename)-1] == '.')
            {
               tmpFilename[strlen(tmpFilename)-1] = '\0';
               retry = TRUE;
            }
            else
#endif
               retry = FALSE;
	 if (!retry) break;
      }
   }

   free(tmpFilename);
   tmpFilename = 0;
   
#ifdef __MSDOS__
   setvect(36,doshandler);
#endif
   return fp;
}


/*************************************************************************
 *
 *N  vpf_open_table
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function opens a vpf table and either loads it into RAM or sets
 *     up the structure to read off of disk.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    tablename <input> == (const char *) file name of the table.
 *    storage   <input> == (storage_type) table storage mode -
 *                                        either ram or disk.
 *    mode      <input> == (const char *) file mode for opening the table -
 *                                  same as fopen() mode in C.
 *    defstr    <input> == (char *) table definition string used for
 *                                  creating a writable table.
 *    vpf_open_table <output> == (vpf_table_type) VPF table structure.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   April 1991                   DOS Turbo C
 *    Dave Flinn       July 1991                    UNIX compatable
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
vpf_table_type vpf_open_table( const char * tablename,
			       storage_type   storage ,
			       const char * mode,
			       char         * defstr )
{
   vpf_table_type   table;
   char             tablepath[255],
		  * idxname;
   ossim_int32         i,
		    j;
   ossim_int32         tablesize,
		    idxsize,
		    memsize;
   ossim_uint32 ulval;
   char            * diskname = "VPF data disc";

   strcpy(tablepath,tablename);
   rightjust(tablepath);

   /* Parse out name and path */
   j = -1;
   i=(long)strlen(tablepath);
   while (i>0) {
#ifdef __MSDOS__
      if (tablepath[i] == '\\') {
#else
      if (tablepath[i] == '/') {
#endif
	 j = i;
	 break;
      }
      i--;
   }
   strncpy(table.name,&(tablepath[j+1]),12);
   rightjust(table.name);
   strupr(table.name);
   table.path = (char *)vpfmalloc(((unsigned long)strlen(tablepath)+5)*(unsigned long)sizeof(char));
   strcpy(table.path, tablepath);
   table.path[j+1] = '\0';

 /* Establish a read or write table operation */

   if ( mode[0] == 'r' )
     table.mode = Read ;
   else
     table.mode = Write ;

   /*fprintf(stderr, "vpf_open_table opening %s\n", tablepath);*/
   table.fp = vpfopencheck(tablepath,mode,diskname);
   if (table.fp == NULL) {
      perror(tablepath);
/*
#if __MSDOS__
      perror(tablepath);
      getch();
      if (getgraphmode() >= 0) closegraph();
      exit(0);
#endif
*/
      free(table.path);
      if (table.fp) {
	fclose(table.fp);
	table.fp = NULL;
      } 
      return table;
   }

   /* If file is to be created, copy the def string ptr into header for now */

   if ( table.mode == Write )
     table.defstr = defstr ;

#ifdef __MSDOS__
   tablesize = (filelength(fileno(table.fp)));
#else
   {  /* UNIX doesn't have a filelength function, so this is the best */
     struct stat statbuf ;
     if ( stat ( tablepath, &statbuf ) < 0 ) {
       fprintf (stderr, "vpf_open_table: can't stat file\n" ) ;
       free(table.path);
       fclose(table.fp);
       table.fp = NULL;
       return table;
     }
     tablesize = statbuf.st_size ;
   }
#endif

   /* Populate table structure with correct data, either for read or write */

   table.reclen = parse_data_def(&table);

   if ( table.mode == Write ) {   /* write out header */
     rewind ( table.fp ) ;
     Write_Vpf_Int ( &table.ddlen, table.fp, 1 ) ;
     Write_Vpf_Char ( table.defstr, table.fp, table.ddlen ) ;
     free ( table.defstr ) ;
     table.defstr = (char *) NULL ;
     table.nrows = 0 ;
   }

   if (table.reclen > 0) {      /* Index file */
      table.xstorage = (storage_type)COMPUTE;
      if (table.mode != Write)
	 table.nrows = (tablesize - table.ddlen)/table.reclen;
      table.xfp = (FILE *) NULL ;
   } else {
      idxname = strdup( tablepath );
#ifdef __MSDOS__
      idxname[strlen(tablepath)-1] = 'x';
#else
      if (idxname[strlen(tablepath)-1] == '.')
	idxname[strlen(tablepath)-2] = 'x';
      else
	idxname[strlen(tablepath)-1] = 'x';
#endif
      table.xfp = fopen(idxname, mode);

      if ((!table.xfp) && (table.mode == Read)) {
	 perror(idxname);
         fprintf(stderr, "hit RETURN to continue...");
	 i=getc(stdin);
	 free(idxname);
	 for (i = 0; i < table.nfields; i++)
	   free(table.header[i].name);
	 free(table.header);
	 free(table.path);
         fclose(table.fp);
         table.fp = NULL;
         return table;
      }

      free(idxname);

/*#ifdef __MSDOS__*/
      table.xstorage = (storage_type)DISK;   /* Worst case default */
/*#endif*/

   /* Only read in index if file is read only */

      if (table.xfp && ( table.mode == Read ) ) {
	Read_Vpf_Int (&(table.nrows), table.xfp, 1 ) ;
	Read_Vpf_Int (&ulval, table.xfp, 1 ) ;
	idxsize = table.nrows*sizeof(index_cell) + 10L;

#ifdef __MSDOS__
	if ( (idxsize < (farcoreleft()/2)) && (idxsize < __64K) )
#else
	if (0)
#endif
	  {
	    table.xstorage = (storage_type)RAM;
	    table.index = (index_type)vpfmalloc(idxsize);
	    for (i=0;i<table.nrows;i++) {
	      Read_Vpf_Int (&(table.index[i].pos), table.xfp, 1) ;
	      Read_Vpf_Int (&(table.index[i].length),table.xfp,1 ) ;
	    }
	    fclose(table.xfp);
	  }
      } else if (table.mode == Write) {

     /* Write out dummy header record for index file. vpf_close_table finishes
	the job. */

	 Write_Vpf_Int ( &(table.ddlen), table.xfp, 1 ) ;
	 Write_Vpf_Int ( &(table.ddlen), table.xfp, 1 ) ;
	 table.xstorage = (storage_type)DISK;
	 table.index = (index_type) NULL ;

      }
   }  /* end of if table .reclen */

    table.storage = (storage_type)DISK;
#ifdef __MSDOS__
   memsize = (ossim_int32)min(farcoreleft(),__64K);
#else
   memsize = MAXINT;
#endif

   if ( (storage != disk) && ( table.mode == Read ) ) {
      if (tablesize + table.nrows * table.nfields * HEAP_OVERHEAD < memsize) {
	 fseek(table.fp,index_pos(1,table),SEEK_SET);
	 table.row = (row_type *)vpfmalloc((table.nrows+1)*sizeof(row_type));
	 for (i=0;i<table.nrows;i++) {
	    table.row[i] = read_next_row(table);
	 }
	 fclose(table.fp);
	 table.storage = (storage_type)RAM;
      }
   }
   table.status = OPENED;
   return table;
 }

/*************************************************************************
 *
 *N  vpf_close_table
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function frees an entire table from memory.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    table       <inout> == (vpf_table_type) VPF table structure.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   May 1991                     DOS Turbo C
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
void vpf_close_table( vpf_table_type *table )
{
   register ossim_int32 i;

   if (table->status != OPENED) {
      return;
   }
   /*fprintf(stderr, "vpf_close_table closing %s%s\n", table->path, table->name);*/

   /* If the table is writable, write out the final record count */

   if ( table->mode == Write && table->xfp ) {
     rewind ( table->xfp ) ;
     Write_Vpf_Int ( &table->nrows, table->xfp, 1 ) ;
     Write_Vpf_Int ( &table->ddlen, table->xfp, 1 ) ;
   }

   for (i=0;i<table->nfields;i++) {
      free(table->header[i].name);
      /* free up null text string */
      if ( table->header[i].type == 'T')
	 free(table->header[i].nullval.Char);
      /* free up index file string */
      if (table->header[i].tdx!=(char *)NULL)
	free ( table->header[i].tdx ) ;
      /* free up narrative table string */
      if (table->header[i].narrative!=(char *)NULL) {
	free ( table->header[i].narrative ) ;
      }
   }
   free(table->header);

   switch (table->storage) {
      case RAM:
	 for (i=0;i<table->nrows;i++) free_row(table->row[i],*table);
	 free(table->row);
	 break;
      case DISK:
	 fclose(table->fp);
	 break;
      default:
	 printf("%s%s: unknown storage flag: %d\n",table->path,table->name,
		table->storage);
	 break;
   }

   switch (table->xstorage) {
      case RAM:
	 free(table->index);
	 break;
      case DISK:
	 fclose(table->xfp);
	 break;
      case COMPUTE:
	 break;
      default:
	 printf("%s%s: unknown index storage flag: %d\n",
		table->path,table->name,table->storage);
	 break;
   }
   table->nfields = 0;
   free(table->path);
   table->status = CLOSED;
}



ossim_int32 is_vpf_table( const char *fname )
{
   FILE *fp;
   ossim_int32 n, ok;

   fp = fopen( fname, "rb" );
   if (!fp) {
      return FALSE;
   }
   Read_Vpf_Int ( &n, fp, 1 ) ;
   fseek( fp, n-1, SEEK_CUR );
   if (fgetc(fp) == ';')
      ok = TRUE;
   else
      ok = FALSE;
   fclose(fp);
   return ok;
}

