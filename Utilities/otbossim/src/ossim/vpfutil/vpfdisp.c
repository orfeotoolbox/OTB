/*************************************************************************
 *
 *N  Module VPFDISP - Display VPF records
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This module contains functions used to display VPF records in
 *     human readable format.
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
 *    Barry Michaels    May 1991                      DOS Turbo C
 *E
 *************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#if defined(__MINGW32__)
#  include <dirent.h>
#  include <malloc.h>
#  include <mem.h>
#  include <dos.h>
#  ifdef __MSDOS__
#    include <graphics.h>
#    include <conio.h>
#    include <dos.h>
#    include <alloc.h>
#    include <mem.h>
#  endif
#  ifdef _MSC_VER
#    include <io.h>
#  endif
#  ifndef _WIN32
#    include <unistd.h>
#  endif
#  ifdef __APPLE__
#    include <sys/types.h>
#    include <sys/malloc.h>
#    define MAXSHORT 255
#    define MAXLONG  LONG_MAX
#  else
#    include <malloc.h>
#  endif
#endif


#include <string.h>
#include <ctype.h>
#include <math.h>
#if defined(__CYGWIN__) || defined(__APPLE__)|| defined(_WIN32)
#include <ossim/vpfutil/values.h>
#else
#include <ossim/vpfutil/values.h>
#endif
#include <ossim/vpfutil/vpfapi.h>
#include <ossim/vpfutil/vpftable.h>
#include <ossim/vpfutil/vpfview.h>

#if (!defined(_WIN32)||defined(__CYGWIN__))
#    include <unistd.h>  /* For "access" function. */
#endif

extern color_type menucolor, menubordercolor, menutextcolor;

#ifndef __MSDOS__
void strupr2(char* str) {
  char* s = str;
  while (*s)
  {
	  *s = toupper(*s);
	  ++s;
  }
}
#endif


/*************************************************************************
 *
 *N  format_date
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function converts a VPF date value into a string of the
 *     form  mm/dd/yyyy with time displayed as hh:mm:ss if non-zero.
 *     'fmtdate' must be a character string with at least 18 bytes
 *     allocated.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *     date     <input>==(date_type) VPF date type value.
 *     fmtdate <output>==(char *) output formatted date string.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    Sept 1991                     DOS Turbo C
 *E
 *************************************************************************/
void format_date( date_type date, char *fmtdate )
{
   char year[8], month[8], day[8], hour[8], min[8], sec[8];

   date[20]='\0';
   strncpy(year, date, 4);
   year[4]='\0';
   strncpy(month, &date[4], 2);
   month[2]='\0';
   strncpy(day, &date[6], 2);
   day[2]='\0';
   strncpy(hour, &date[8], 2);
   hour[2]='\0';
   strncpy(min, &date[10], 2);
   min[2]='\0';
   strncpy(sec, &date[12], 2);
   sec[2]='\0';
   if ( (strcmp(hour,"00") == 0) && (strcmp(min,"00")==0) &&
	(strcmp(sec,"00") == 0) )
      sprintf(fmtdate,"%s/%s/%s",month,day,year);
   else
      sprintf(fmtdate,"%s/%s/%s %s:%s:%s",month,day,year,hour,min,sec);
}

/*************************************************************************
 *
 *N  vpf_display_record
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function displays all of the fields of a particular VPF
 *     record, including the descriptions of the fields and their
 *     values.  It accesses the online data dictionary metadata
 *     contained in the specified Value Description Tables.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    row     <input> == (row_type) vpf table row structure.
 *    table   <input> == (vpf_table_type) vpf table.
 *    fp      <input> == (FILE *) pointer to the output file.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    Sept 1991                     DOS Turbo C
 *E
 *************************************************************************/
void vpf_display_record( row_type row,
			 vpf_table_type table,
			 FILE *fp )
{
   register int i,j;
   char *buf,*num, path[128], *tablename;
   char *attr,*val,*descr, vdtpath[128], fmtdate[40];
   vpf_table_type vdt;
   row_type vdtrow;
   int found;
   ossim_int32 l, lval, count, *lptr;
   short int s,sval,*sptr;
   float f,*fptr;
   date_type date;
   int TABLE_ = 1, ATTR_ = 2, VAL_ = 3, DESCR_ = 4;

   num = (char *)vpfmalloc(20*sizeof(char));

   strcpy( vdtpath, "" );

   /* Display all secondary attributes */
   for (i=0;i<table.nfields;i++) {
      switch (table.header[i].type) {
	 case 'T':
	    buf = (char *)get_table_element(i,row,table,
					    NULL,&count);
	    rightjust(buf);
	    if (strcmp( buf, "" ) != 0) {
	       fputs(table.header[i].name,fp);
	       fputs(" - ",fp);
	       fputs(table.header[i].description,fp);
	       fputs(": ",fp);
	       fputs(buf,fp);


	       strcpy( path, table.path );
	       strcat( path, table.header[i].vdt );
	       if ( (access(path,4)==0) &&
		    (strcmp(table.header[i].vdt,"") != 0) ) {

		  if (strcmp(vdtpath,path) != 0) {
		     if (strcmp(vdtpath,"") != 0) {
			vpf_close_table(&vdt);
		     }
		     vdt = vpf_open_table( path, disk, "rb", NULL );
		  }

		  strcpy( vdtpath, path );
		  for (j=1;j<=vdt.nrows;j++) {
		     vdtrow = read_row( j, vdt );
		     tablename = (char *)get_table_element( TABLE_, vdtrow,
							 vdt, NULL, &count );
		     rightjust(tablename);
		     strupr2(tablename);
		     attr = (char *)get_table_element( ATTR_, vdtrow, vdt,
						       NULL, &count );
		     rightjust(attr);
		     val = (char *)get_table_element( VAL_, vdtrow, vdt,
						      NULL, &count );
		     rightjust(val);

		     found = FALSE;
		     if ( (strstr(table.name,tablename)) &&
			  (ossim_strcasecmp(attr,table.header[i].name)==0) &&
			  (ossim_strcasecmp(val,buf)==0) ) {
			descr = (char *)get_table_element(DESCR_,vdtrow,vdt,
							  NULL, &count);
			rightjust(descr);
			fputs(" (",fp);
			fputs(descr,fp);
			fputs(")",fp);

			free(descr);
			found = TRUE;
		     }
		     free(tablename);
		     free(attr);
		     free(val);
		     free_row( vdtrow, vdt );
		     if (found) break;
		  }
	       }


	       fputc('\n',fp);
	    }
	    free(buf);
	    break;
	 case 'I':
	    if (table.header[i].count==1) {
	       get_table_element(i,row,table,&l,&count);
	       if (l != NULLINT) {
		  fputs(table.header[i].name,fp);
		  fputs(" - ",fp);
		  fputs(table.header[i].description,fp);
		  fputs(": ",fp);
		  ltoa((int)l,num,10);
		  fputs(num,fp);

		  strcpy( path, table.path );
		  strcat( path, table.header[i].vdt );
		  if ( (access(path,4)==0) &&
		     (strcmp(table.header[i].vdt,"") != 0) ) {

		     if (strcmp(vdtpath,path) != 0) {
			if (strcmp(vdtpath,"") != 0) {
			   vpf_close_table(&vdt);
			}
			vdt = vpf_open_table( path, disk, "rb", NULL );
		     }
		     strcpy( vdtpath, path );

		     for (j=1;j<=vdt.nrows;j++) {
			vdtrow = read_row( j, vdt );
			tablename = (char *)get_table_element( TABLE_,
					      vdtrow, vdt, NULL, &count );
			rightjust(tablename);
			strupr2(tablename);
			attr = (char *)get_table_element( ATTR_, vdtrow, vdt,
							  NULL, &count );
			rightjust(attr);
			get_table_element( VAL_, vdtrow, vdt,
					   &lval, &count );

			found = FALSE;
			if ( (strstr(table.name,tablename)) &&
			   (ossim_strcasecmp(attr,table.header[i].name)==0) &&
			   (lval==l) ) {
			   descr = (char *)get_table_element(DESCR_,vdtrow,
							 vdt, NULL, &count);
			   rightjust(descr);
			   fputs(" (",fp);
			   fputs(descr,fp);
			   fputs(")",fp);

			   free(descr);
			   found = TRUE;
			}
			free(tablename);
			free(attr);
			free_row( vdtrow, vdt );
			if (found) break;
		     }
		  }

		  fputc('\n',fp);
	       }
	    } else {
	       get_table_element(i,row,table,&lptr,&count);
	       fputs(table.header[i].name,fp);
	       fputs(" - ",fp);
	       fputs(table.header[i].description,fp);
	       fputs(": (",fp);
	       for (j=0;j<count;j++) {
		  ltoa((int)lptr[j],num,10);
		  fputs(num,fp);
		  if (j<count-1) fputs(", ",fp);
	       }
	       fputs(")\n",fp);
	    }
	    break;
	 case 'S':
	    if (table.header[i].count==1) {
	       get_table_element(i,row,table,&s,&count);
	       if (s != NULLSHORT) {
		  fputs(table.header[i].name,fp);
		  fputs(" - ",fp);
		  fputs(table.header[i].description,fp);
		  fputs(": ",fp);
		  itoa(s,num,10);
		  fputs(num,fp);

		  strcpy( path, table.path );
		  strcat( path, table.header[i].vdt );
		  if ( (access(path,4)==0) &&
		     (strcmp(table.header[i].vdt,"") != 0) ) {

		     if (strcmp(vdtpath,path) != 0) {
			if (strcmp(vdtpath,"") != 0) {
			   vpf_close_table(&vdt);
			}
			vdt = vpf_open_table( path, disk, "rb", NULL );
		     }
		     strcpy( vdtpath, path );

		     for (j=1;j<=vdt.nrows;j++) {
			vdtrow = read_row( j, vdt );
			tablename = (char *)get_table_element( TABLE_,
					      vdtrow, vdt, NULL, &count );
			rightjust(tablename);
			strupr2(tablename);
			attr = (char *)get_table_element( ATTR_, vdtrow, vdt,
							  NULL, &count );
			rightjust(attr);
			get_table_element( VAL_, vdtrow, vdt,
					   &sval, &count );

			found = FALSE;
			if ( (strstr(table.name,tablename)) &&
			   (ossim_strcasecmp(attr,table.header[i].name)==0) &&
			   (sval==s) ) {
			   descr = (char *)get_table_element(DESCR_,vdtrow,
							 vdt, NULL, &count);
			   rightjust(descr);
			   fputs(" (",fp);
			   fputs(descr,fp);
			   fputs(")",fp);

			   free(descr);
			   found = TRUE;
			}
			free(tablename);
			free(attr);
			free_row( vdtrow, vdt );
			if (found) break;
		     }
		  }

		  fputc('\n',fp);
	       }
	    } else {
	       get_table_element(i,row,table,&sptr,&count);
	       fputs(table.header[i].name,fp);
	       fputs(" - ",fp);
	       fputs(table.header[i].description,fp);
	       fputs(": (",fp);
	       for (j=0;j<count;j++) {
		  itoa(sptr[j],num,10);
		  fputs(num,fp);
		  if (j<count-1) fputs(", ",fp);
	       }
	       fputs(")\n",fp);
	    }
	    break;
	 case 'F':
	    if (table.header[i].count==1) {
	       get_table_element(i,row,table,&f,&count);
	       if (!is_vpf_null_float(f)) {
		  fputs(table.header[i].name,fp);
		  fputs(" - ",fp);
		  fputs(table.header[i].description,fp);
		  fputs(": ",fp);
#if !defined(__APPLE__) && !defined(__FreeBSD__)
		  gcvt(f,6,num);
#endif
		  fputs(num,fp);
		  fputc('\n',fp);
	       }
	    } else {
	       get_table_element(i,row,table,&fptr,&count);
	       fputs(table.header[i].name,fp);
	       fputs(" - ",fp);
	       fputs(table.header[i].description,fp);
	       fputs(": (",fp);
	       for (j=0;j<count;j++) {
		  if (is_vpf_null_float(fptr[j])) {
		     fputs("(null)",fp);
		  } else {
#if !defined(__APPLE__) && !defined(__FreeBSD__)
		     gcvt(fptr[j],6,num);
#endif
		     fputs(num,fp);
		  }
		  if (j<count-1) fputs(", ",fp);
	       }
	       fputs(")\n",fp);
	    }
	    break;
	 case 'D':
	    if (table.header[i].count==1) {
	       get_table_element(i,row,table,date,&count);
	       fputs(table.header[i].name,fp);
	       fputs(" - ",fp);
	       fputs(table.header[i].description,fp);
	       fputs(": ",fp);
	       format_date(date,fmtdate);
	       fputs(fmtdate,fp);
	       fputc('\n',fp);
	    }
	    break;
      }
   }

   if (strcmp(vdtpath,"") != 0)
      vpf_close_table( &vdt );

   free(num);
}




/*************************************************************************
 *
 *N  display_attributes
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function displays all of the attributes of a particular
 *     feature, including the descriptions of the fields and their
 *     values.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    fc      <input> == (int) feature class number.
 *    row     <input> == (ossim_int32) row of the attribute table.
 *    library <input> == (library_type *) VPF library structure.
 *    fp      <input> == (FILE *) pointer to the output file.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    May 1991                      DOS Turbo C
 *E
 *************************************************************************/
void display_attributes( int fc,
			 ossim_int32 row,
			 library_type *library,
			 FILE *fp )
{
   vpf_table_type feature_table;
   row_type vpfrow;

   feature_table = vpf_open_table(library->fc[fc].table,disk,"rb",NULL);

   vpfrow = read_row( row, feature_table );

   vpf_display_record( vpfrow, feature_table, fp );

   free_row(vpfrow,feature_table);

   vpf_close_table(&feature_table);
}
