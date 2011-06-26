#include <stdio.h>
#include <string.h>
#ifdef __MSDOS__
#include <graphics.h>
#endif

#ifdef __MSDOS__
#include <alloc.h>
#include <mem.h>
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


#include <ossim/vpfutil/vpftable.h>
#include <ossim/vpfutil/vpfview.h>
#include <ossim/vpfutil/vpfinit.h>

extern color_type menutextcolor,menubordercolor,menucolor;
#if 0
/*************************************************************************
 *
 *N  feature_class_dictionary
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function displays the data dictionary for a feature class.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    fc  <input> == (int) feature class number.
 *    lib <input> == (library_type) VPF library structure.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Mody Buchbinder  May 1991   Original Version   DOS Turbo C
 *    Barry Michaels July 1991 - Separated into two routines
 *    Barry Michaels  Oct 1991 - Modified narrative table read
 *E
 *************************************************************************/
void feature_class_dictionary( int fc, library_type lib )
{
   int                i,j,k,
		      it_pos,val_pos,des_pos,att_pos;
   char               *outfile,*line,temp[128],temp2[128],
		      *item_buf, *att_buf, *des_buf, *tval;
   vpf_table_type     ft,table,nar;
   storage_type       stor = disk;
   FILE               *out;
   row_type           row;
   float              fval;
   ossim_int32           ival,n;

   hidemousecursor();
   time_cursor();
   showmousecursor();

   /** prepare temporary display file **/
   outfile = (char *)malloc(L_tmpnam + 2);
   outfile = tmpnam(outfile);
   out = fopen(outfile,"w");

   /** start to write to display file **/
   fprintf(out,"FEATURE CLASS: %s \n",lib.fc[fc].description);
   fprintf(out,"COVERAGE     : %s \n",
	   lib.cover[lib.fc[fc].coverage].description);

   ft = vpf_open_table(lib.fc[fc].table,stor,"rb",NULL);

   fprintf(out,"ATTRIBUTES:\n");

   for(i=0;i<ft.nfields;i++) {
     fprintf(out,"   %s - %s \n",ft.header[i].name,ft.header[i].description);
     if(ft.header[i].vdt[0] != '\0')

	 /** create table name **/
      { strcpy(temp,lib.fc[fc].table);
	 /** cut temp to path and name **/
	for(j=strlen(temp);temp[j] != '\\';j--);
	temp[j] = '\0';

	rightjust(temp);

	 /** add to the path the new table name **/
	strcpy(temp2,temp);
	strcat(temp2,"\\");
	strcat(temp2,ft.header[i].vdt);
	if(access(temp2,0) == 0)
	  {
	    table   = vpf_open_table(temp2,stor,"rb",NULL);
	    it_pos  = table_pos("TABLE",table);
	    val_pos = table_pos("VALUE",table);
	    des_pos = table_pos("DESCRIPTION",table);
	    att_pos = table_pos("ATTRIBUTE",table);

	    for(k=1;k<=table.nrows;k++)
	      {
		if (stor == disk)
		   row = read_next_row(table);
		else
		   row = get_row(k,table);
		item_buf = (char *)get_table_element( it_pos
						,row,table,NULL,&n);

		rightjust(item_buf);

		att_buf  = (char *)get_table_element(att_pos
						,row,table,NULL,&n);

		rightjust(att_buf);

		if(ossim_strcasecmp(item_buf,&temp[j+1]) == 0 &&
                   ossim_strcasecmp(att_buf,ft.header[i].name) == 0)
		   { des_buf  = (char *)get_table_element(des_pos
						,row,table,NULL,&n);
		     switch (ft.header[i].type) {
			case 'T':
		     tval = (char *)get_table_element(val_pos
						,row,table,NULL,&n);
		     fprintf(out,"    %s =  %s  \n",tval,des_buf);
		     free(tval);break;
			case 'I':
		     get_table_element(val_pos,row,table,&ival,&n);
		     fprintf(out,"    %12ld =  %s  \n",ival,des_buf);
		     break;
			case 'F':
		     get_table_element(val_pos,row,table,&fval,&n);
		     fprintf(out,"    %12f =  %s  \n",fval,des_buf);
		     break;
		     } /** switch **/
		     free(des_buf);
		   }  /** if strcmp **/

		free(item_buf);
		free(att_buf);
		free_row(row,table);
	      }  /** for k on table.nrows **/
	    vpf_close_table(&table);
	  } /** if access **/
      } /** if there is vdt **/
   }  /** for fields **/

   /** get information from narrative file if it exists **/
   if(ft.narrative[0] != '\0') {
      sprintf(temp,"%s%s",ft.path,ft.narrative);
      if (access(temp,0)==0) {
	 fprintf(out,"\n\n");
	 nar = vpf_open_table(temp,disk,"rb",NULL);
	 for (i=1;i<=nar.nrows;i++) {
	    row = read_next_row(nar);
	    line = (char *)get_table_element(1,row,nar,NULL,&n);
	    rightjust(line);
	    fprintf(out,"%s\n",line);
	    free(line);
	    free_row(row,nar);
	 }
	 vpf_close_table(&nar);
      }
   }

   /** clean up **/
   vpf_close_table(&ft);
   fclose(out);

   arrow_cursor();

   viewfile(outfile,getmaxy()/3);
   remove(outfile);
   free(outfile);
}

/*************************************************************************
 *
 *N  display_data_dictionary
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function shows a feature class menu, lets the user select
 *     a feature class and print the information from the data dictionary
 *     for this feature class
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    lib <in> == (library_type) VPF library structure.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Mody Buchbinder  May 1991   Original Version   DOS Turbo C
 *    Barry Michaels  June 1991 - Separated into two routines.
 *    Barry Michaels   Oct 1991 - Feature class selection from a panel.
 *    Barry Michaels   Jan 1992 - Select FCs by coverage first.
 *E
 *************************************************************************/
void display_data_dictionary(library_type lib)
{
   int       fc,cov;
   view_type dummyview;
   char      title[80];

   dummyview.nthemes = 0;

   cov = 0;
   while (cov >= 0) {
      sprintf(title,"%s COVERAGES",lib.name);
      cov = select_coverage(&dummyview,&lib,title);
      if (cov < 0) break;
      fc = 0;
      while (fc >= 0) {
	 sprintf(title,"%s Feature Classes",lib.cover[cov].description);
	 fc = select_feature_class(&dummyview,&lib,title,cov);
	 if (fc < 0) break;
	 feature_class_dictionary( fc, lib );
      }
   }

}

#endif
