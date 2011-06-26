/***************************************************************************
 *
 *N  VPFCNTNT.C  - VPF Contents
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    This module contains functions to display the contents of VPF tables
 *    and VPF databases and libraries.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels      November 1991
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 **************************************************************************/
#if defined(__CYGWIN__) || defined(__APPLE__)|| defined(_WIN32) || defined(__FreeBSD__) || defined(__OpenBSD__)
#  include "ossim/vpfutil/values.h"
#else
#  include <values.h>
#endif

#include <stdlib.h>

#ifdef __MSDOS__
#  include <graphics.h>
#  include <dir.h>
#  include <dos.h>
#  include <alloc.h>
#else
#  ifndef _WIN32
#    include <unistd.h>
#  endif
#  ifdef __APPLE__
#    include <sys/types.h>
#    include <sys/malloc.h>
#    define MAXINT INT_MAX
#    ifndef MAXSHORT
#        define MAXSHORT SHORT_MAX
#    endif
#    define MAXLONG  LONG_MAX
#  else
#    ifndef __FreeBSD__
#      include <malloc.h>
#    endif
#  endif
#endif
#ifdef _MSC_VER
#  include <io.h>
#endif

#include <string.h>
#include <ossim/vpfutil/vpftable.h>
#include <ossim/vpfutil/vpfview.h>
#include <ossim/vpfutil/system.h>
#include <ossim/vpfutil/vpfmisc.h>
#include <ossim/vpfutil/vpfdisp.h>

#include <ossim/vpfutil/vpfapi.h>



/**************************************************************************
 *
 *N  vpf_dump_table
 *
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    Dump the contents of a VPF table into an ASCII file
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    tablename <input> == (char *) VPF table to dump.
 *    outname   <input> == (char *) name of ASCII dump file.
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels  DOS Turbo C
 *E
 *************************************************************************/
void vpf_dump_table( char *tablename, char *outname )
{
   vpf_table_type table;
   ossim_int32   i,j,k,n;
   int        ival,*iptr;
   ossim_int32   lval,*lptr;
   float      fval,*fptr;
   date_type  dval,*dptr;
   id_triplet_type kval, *kptr;
   coordinate_type cval, *cptr;
   char       *buf,  ch, date[40];
   row_type   row;
   FILE       *fp;

   fp = fopen(outname,"w");

   table = vpf_open_table(tablename,disk,"rb",NULL);

   fprintf(fp,"%s\n%s\n\n",tablename,table.description);

   fprintf(fp,"Definition:\n");
   for (i=0;i<table.nfields;i++) {
      if (table.header[i].count < 0)
	 fprintf(fp,"%s (%c,*)  %s\n",
		 table.header[i].name,table.header[i].type,
		 table.header[i].description);
      else
	 fprintf(fp,"%s (%c,%ld)  %s\n",
		 table.header[i].name,table.header[i].type,
		 table.header[i].count,table.header[i].description);
   }

   fprintf(fp,"\nContents:\n");
   for (i=1;i<=table.nrows;i++) {
      row = read_next_row(table);
      for (j=0;j<table.nfields;j++) {
	 fprintf(fp,"%s: ",table.header[j].name);
	 switch (table.header[j].type) {
	    case 'T':
	       if (table.header[j].count==1) {
		  get_table_element(j,row,table,&ch,&n);
		  fprintf(fp,"%c\n",ch);
	       } else {
		  buf = (char *)get_table_element(j,row,table,NULL,&n);
		  n = (long)strlen(table.header[j].name) + 2;
		  for (k=0;(unsigned int)k<strlen(buf);k++) {
		     fprintf(fp,"%c",buf[k]);
		     n++;
		     if (n>80) {
			fprintf(fp,"\n");
			n = 0;
		     }
		  }
		  fprintf(fp,"\n");
		  free(buf);
	       }
	       break;
	    case 'I':
	       if (table.header[j].count==1) {
		  get_table_element(j,row,table,&lval,&n);
		  if (lval != MAXFLOAT)
		     fprintf(fp,"%ld\n",lval);
		  else
		     fprintf(fp,"(null)\n");
	       } else {
		  lptr = (ossim_int32*)get_table_element(j,row,table,NULL,&n);
		  for (k=0;k<n;k++) {
		     if (lptr[k] != MAXFLOAT)
			fprintf(fp,"%ld ",lptr[k]);
		     else
			fprintf(fp,"(null) ");
		  }
		  fprintf(fp,"\n");
		  free(lptr);
	       }
	       break;
	    case 'S':
	       if (table.header[j].count==1) {
		  get_table_element(j,row,table,&ival,&n);
		  if (ival != MAXINT)
		     fprintf(fp,"%d\n",ival);
		  else
		     fprintf(fp,"(null)\n");
	       } else {
		  iptr = (int*)get_table_element(j,row,table,NULL,&n);
		  for (k=0;k<n;k++) {
		     if (iptr[k] != MAXINT)
			fprintf(fp,"%d ",iptr[k]);
		     else
			fprintf(fp,"(null) ");
		  }
		  fprintf(fp,"\n");
		  free(iptr);
	       }
	       break;
	    case 'F':
	       if (table.header[j].count==1) {
		  get_table_element(j,row,table,&fval,&n);
		  if (fval != MAXFLOAT)
		     fprintf(fp,"%f\n",fval);
		  else
		     fprintf(fp,"(null)\n");
	       } else {
		  fptr = (float*)get_table_element(j,row,table,NULL,&n);
		  for (k=0;k<n;k++) {
		     if (fptr[k] != MAXFLOAT)
			fprintf(fp,"%f ",fptr[k]);
		     else
			fprintf(fp,"(null) ");
		  }
		  fprintf(fp,"\n");
		  free(fptr);
	       }
	       break;
	    case 'C':
	       if (table.header[j].count==1) {
		  get_table_element(j,row,table,&cval,&n);
		  fprintf(fp,"(%f,%f)\n",cval.x,cval.y);
	       } else {
		  cptr = (coordinate_type*)get_table_element(j,row,table,NULL,&n);
		  for (k=0;k<n;k++)
		     fprintf(fp,"(%f,%f) ",cptr[k].x,cptr[k].y);
		  fprintf(fp,"\n");
		  free(cptr);
	       }
	       break;
	    case 'K':
	       if (table.header[j].count==1) {
		  get_table_element(j,row,table,&kval,&n);
		  fprintf(fp,"(%ld,%ld,%ld)\n",
			  kval.id,kval.tile,kval.exid);
	       } else {
		  kptr = (id_triplet_type*)get_table_element(j,row,table,NULL,&n);
		  for (k=0;k<n;k++)
		     fprintf(fp,"(%ld,%ld,%ld)  ",
			     kptr[k].id,kptr[k].tile,kptr[k].exid);
		  fprintf(fp,"\n");
		  free(kptr);
	       }
	       break;
	    case 'D':   /* Date */
	       if (table.header[j].count==1) {
		  get_table_element(j,row,table,&dval,&n);
		  format_date(dval,date);
		  fprintf(fp,"%s\n",date);
	       } else {
		  dptr = (date_type*)get_table_element(j,row,table,NULL,&n);
		  for (k=0;k<n;k++) {
		     format_date((char*)(&dptr[k]),date);
		     fprintf(fp,"%s ",date);
		  }
		  fprintf(fp,"\n");
		  free(dptr);
	       }
	       break;
	 }
      }
      fprintf(fp,"\n");
      free_row( row, table );
   }

   fclose(fp);
   vpf_close_table( &table );
}


/* Dump a Narrative-style .DOC VPF table to an ASCII file */
/**************************************************************************
 *
 *N  vpf_dump_doc_table
 *
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    Dump the contents of a narrative-style .DOC VPF table into an
 *    ASCII file as a series of text strings.  This function checks
 *    to make sure that the given table is a real narrative file
 *    (two fields: ID and TEXT).  If not, it displays it as a normal
 *    VPF table.
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    tablename <input> == (char *) narrative-style .DOC VPF table to dump.
 *    outname   <input> == (char *) name of ASCII dump file.
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels  DOS Turbo C
 *E
 *************************************************************************/
void vpf_dump_doc_table( char *tablename, char *outname )
{
   vpf_table_type table;
   ossim_int32   i,n;
   char       *buf;
   row_type   row;
   FILE       *fp;

   fp = fopen(outname,"w");

   table = vpf_open_table(tablename,disk,"rb",NULL);

   /* Check header to make sure the table is a narrative table */
   if ((ossim_strcasecmp(table.header[1].name,"TEXT") != 0) ||
       (table.nfields != 2)) {
      /* Not a real narrative table -> normal VPF table dump */
      vpf_close_table(&table);
      vpf_dump_table(tablename,outname);
      return;
   }

   fprintf(fp,"%s\n%s\n\n",tablename,table.description);

   for (i=1;i<=table.nrows;i++) {
      row = read_next_row(table);
      buf = (char *)get_table_element(1,row,table,NULL,&n);
      fprintf(fp,"%s\n",buf);
      free(buf);
      free_row(row,table);
   }

   fclose(fp);
   vpf_close_table( &table );
}

#if 0
static void not_a_vpf_table( char *path )
{
   char *buf,*filename,*tempstr,ch;

   buf = (char *)vpfmalloc(255);
   filename = (char *)vpfmalloc(255);

   /* Not a VPF table - try to determine what type of file it is */
   if (strstr(path,"\\asi") ||
       strstr(path,"\\lsi") ||
       strstr(path,"\\nsi") ||
       strstr(path,"\\csi") ||
       strstr(path,"\\tsi")) {
      sprintf(filename,"%s is a spatial index file.",path);
      displaymessage(filename,
	   "It is not a VPF table and cannot be viewed directly",
	   NULL);
   } else if (strstr(path,".ati") ||
	      strstr(path,".lti") ||
	      strstr(path,".pti") ||
	      strstr(path,".tti")) {
      sprintf(filename,"%s is a thematic index file.",path);
      displaymessage(filename,
	       "It is not a VPF table and cannot be viewed directly",
	       NULL);
   } else if (path[strlen(path)-1] == 'x') {
      /* Need to determine what file it is the index for */
      sprintf(buf,"%s is a variable-length index file",path);
      strcpy(filename,path);
      for (ch='a';ch <= 'z';ch++) {
	 if (ch=='x') continue;
	 filename[strlen(path)-1] = ch;
	 if (access(filename,0) == 0) {
	    tempstr = strdup(filename);
	    sprintf(filename,"for %s.",tempstr);
	    free(tempstr);
	    break;
	 }
      }
      if (strcmp(path,filename)==0) {
	 displaymessage(buf,
	       "It is not a VPF table and cannot be viewed directly.",
	       NULL);
      } else {
	    displaymessage(buf, filename,
	       "It is not a VPF table and cannot be viewed directly.",
	       NULL);
      }
   } else {
      /* Not a VPF type of file */
      sprintf(filename,"%s: Not a VPF table",path);
      display_message(filename);
   }

   free(filename);
   free(buf);
}
#endif

#if 0
/**************************************************************************
 *
 *N  display_database_contents
 *
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    List the tables in the Database level directory and display their
 *    contents if selected.
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    database <input> == (database_type) VPF database.
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels  DOS Turbo C
 *E
 *************************************************************************/
void display_database_contents( database_type database )
{
   char *filename, *dbpath, *path, *dbstr;

   dbstr = (char *)vpfmalloc(128);
   dbpath = (char *)vpfmalloc(255);
   filename = (char *)vpfmalloc(128);
   path = (char *)vpfmalloc(255);

   strcpy( dbstr, "Database: " );
   strcat( dbstr, database.name );

   strcpy(dbpath,database.path);
   strcat(dbpath,"*.*");
   do {
      strcpy(filename,pickfile(dbpath,0,dbstr));
      if (strcmp(filename,"")==0) break;
      strcpy( path, database.path );
      strcat( path, filename );
      rightjust(path);
      strlwr(path);
      if (is_vpf_table(path)) {
	 if (strstr(path,".doc"))
	    vpf_dump_doc_table(path,"temp.out");
	 else
	    vpf_dump_table(path,"temp.out");
	 viewfile("temp.out", getmaxy()/3);
	 unlink("temp.out");
      } else {
	 not_a_vpf_table(path);
      }
   } while (strcmp(filename,"") != 0);

   free(path);
   free(filename);
   free(dbpath);
   free(dbstr);
}

/**************************************************************************
 *
 *N  display_library_contents
 *
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    List the tables in the Library level directory and display their
 *    contents if selected.
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    library   <input> == (library_type) VPF library structure.
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels  DOS Turbo C
 *E
 *************************************************************************/
void display_library_contents( library_type library )
{
   char *filename, *libpath, *path, *libstr;

   libstr = (char *)vpfmalloc(128);
   libpath = (char *)vpfmalloc(255);
   filename = (char *)vpfmalloc(128);
   path = (char *)vpfmalloc(255);

   strcpy( libstr, "Library: " );
   strcat( libstr, library.name );

   strcpy(libpath,library.path);
   strcat(libpath,"*.*");
   do {
      strcpy(filename,pickfile(libpath,0,libstr));
      if (strcmp(filename,"")==0) break;
      strcpy( path, library.path );
      strcat( path, filename );
      rightjust(path);
      leftjust(path);
      strlwr(path);
      if (is_vpf_table(path)) {
	 strlwr(path);
	 if (strstr(path,".doc"))
	    vpf_dump_doc_table(path,"temp.out");
	 else
	    vpf_dump_table(path,"temp.out");
	 viewfile("temp.out", getmaxy()/3);
	 unlink("temp.out");
      } else {
	 not_a_vpf_table(path);
      }
   } while (strcmp(filename,"") != 0);

   free(path);
   free(filename);
   free(libpath);
   free(libstr);
}
#endif
