/***************************************************************************
 *
 *N  Module VPFRELAT.C
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    This module contains functions supporting relates between VPF
 *    feature classes and primitives (and vice versa).  It relies
 *    upon the information provided by the Feature Class Schema table.
 *    This table is used to generate a feature class relationship (fcrel)
 *    data structure for a feature class.  This structure contains all
 *    of the tables and their primary and foreign keys for the
 *    relationships between a feature table and its primitive, or
 *    from a primitive to its feature table (each relate chain is one way).
 *    This module tries to be as much of a black box as it can to
 *    enable a programmer to simply return the corresponding primitive
 *    row of a feature record, or the corresponding feature row of a
 *    primitive record.
 *
 *    This is one of the most difficult modules required to support
 *    a truly 'generic' VPF application, since VPF allows so many
 *    variations of feature-primitive relationships.  The final version
 *    of this module must support every allowed relationship.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels
 *
 *    Added one-to-many relates 3/2/92 - BJM
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 **************************************************************************/
#include <stdio.h>
#include <stdlib.h>

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


#if defined(__CYGWIN__)||defined(__APPLE__)|| defined(_WIN32)
#include <ossim/vpfutil/values.h>
#else
#include <ossim/vpfutil/values.h>
#endif
#include <string.h>
#include <stdarg.h>
#include <ossim/vpfutil/vpftable.h>
#include <ossim/vpfutil/vpfview.h>
#include <ossim/vpfutil/vpfrelat.h>
#include <ossim/vpfutil/vpftidx.h>

/* Determine if the given table name is in the given list of */
/* vpf relate structures.				     */
static int table_in_list( char *tablename, linked_list_type rlist )
{
   position_type p;
   vpf_relate_struct rcell;

   p = ll_first(rlist);
   while (!ll_end(p)) {
      ll_element(p,&rcell);
      if (strcmp(rcell.table1,tablename)==0) return 1;
      p = ll_next(p);
   }
   return 0;
}

/**************************************************************************
 *
 *N  fcs_relate_list
 *
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    Read the feature class schema table and create the list of
 *    tables to chain through.
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    fcname       <input> == (char *) feature class name.
 *    start_table  <input> == (char *) table to start from.
 *    end_table    <input> == (char *) table to end with.
 *    fcs          <input> == (vpf_table_type) feature class schema table.
 *    fcs_relate_list <output> == (linked_list_type) list of tables to
 *                                chain through.
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels  DOS Turbo C
 *E
 *************************************************************************/
linked_list_type fcs_relate_list( char *fcname, char *start_table,
				  char *end_table, vpf_table_type fcs )
{
   linked_list_type rlist;
   vpf_relate_struct rstruct;
   set_type fcset, set1, set2;
   char tablename[255], *buf, expr[255];
   row_type row;
   ossim_int32 rownum,n;
   int TABLE1_, KEY1_, TABLE2_, KEY2_;

   rlist = ll_init();

   sprintf(expr,"FEATURE_CLASS = %s",fcname);

   fcset = query_table(expr,fcs);

   if (set_empty(fcset)) {
      set_nuke(&fcset);
      return rlist;
   }

   TABLE1_ = table_pos("TABLE1",fcs);
   KEY1_ = table_pos("FOREIGN_KEY",fcs);
   if (KEY1_ < 0) KEY1_ = table_pos("TABLE1_KEY",fcs);
   TABLE2_ = table_pos("TABLE2",fcs);
   KEY2_ = table_pos("PRIMARY_KEY",fcs);
   if (KEY2_ < 0) KEY2_ = table_pos("TABLE2_KEY",fcs);

   strcpy( tablename, start_table );
   while (1) {
      sprintf(expr,"TABLE1 = %s",tablename);

      set1 = query_table(expr,fcs);
      set2 = set_intersection(set1,fcset);
      set_nuke(&set1);
      if (set_empty(set2)) {
	 set_nuke(&fcset);
	 set_nuke(&set2);
	 return rlist;
      }
      rownum = set_min(set2);

      set_nuke(&set2);

      row = get_row(rownum,fcs);

      buf = (char *)get_table_element(TABLE1_,row,fcs,NULL,&n);
      strcpy(rstruct.table1,buf);
      rightjust(rstruct.table1);
      free(buf);

      buf = (char *)get_table_element(KEY1_,row,fcs,NULL,&n);
      strcpy(rstruct.key1,buf);
      rightjust(rstruct.key1);
      free(buf);

      buf = (char *)get_table_element(TABLE2_,row,fcs,NULL,&n);
      strcpy(rstruct.table2,buf);
      rightjust(rstruct.table2);
      free(buf);

      buf = (char *)get_table_element(KEY2_,row,fcs,NULL,&n);
      strcpy(rstruct.key2,buf);
      rightjust(rstruct.key2);
      free(buf);

      rstruct.degree = R_ONE;  /* Default */

      free_row( row, fcs );

      if (table_in_list(rstruct.table1, rlist)) break;

      ll_insert( &rstruct, sizeof(rstruct), ll_last(rlist) );

      strcpy( tablename, rstruct.table2 );

      if (ossim_strcasecmp(tablename,end_table)==0) break;
   }

   set_nuke(&fcset);

   return rlist;
}


/**************************************************************************
 *
 *N  vpf_binary_search
 *
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    This function performs a binary search on a VPF table for the
 *    specified integer value.  Only VPF data type 'I' is supported.
 *    The table must be sorted on the specified field, or this function
 *    will give unpredictable results.  The table must have been
 *    successfully opened.  If more than one row matches the search
 *    value, only the first encountered will be returned.
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    srchval <input> == (ossim_int32) specified search value.
 *    field   <input> == (int) table sort field.
 *    table   <input> == (vpf_table_type) VPF table.
 *    vpf_binary_search <output> == (ossim_int32) first matching row.
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels  DOS Turbo C
 *E
 *************************************************************************/
ossim_int32 vpf_binary_search( ossim_int32 srchval,
			    int field,
			    vpf_table_type table )
{
   ossim_int32 left,right, ival, rowid, n;
   row_type row;

   left = 1;
   right = table.nrows;

   do {
      rowid = (left+right)/2;
      row = get_row(rowid,table);
      get_table_element(field,row,table,&ival,&n);
      free_row(row,table);
      if (ival < srchval)
	 right = rowid-1;
      else
	 left = rowid+1;
   } while ((srchval != ival) && (left <= right));

   if (srchval != ival) rowid = 0;

   return rowid;
}


/**************************************************************************
 *
 *N  related_row
 *
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    Return the related row of table2 based upon the value of table 1's key
 *    Table 2 must be the '1' side of an n:1 relationship  --  If it isn't,
 *    use 'related_rows()'.
 *    Supported data types - I and T<n>.
 *    Binary search supported only for data type I. (column must be sorted)
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels  DOS Turbo C
 *E
 *************************************************************************/
ossim_int32 related_row( void *keyval1,
		      vpf_table_type table2, char *key2,
		      int sort_flag )
{
   ossim_int32 rowid, i, ival, kval, n;
   row_type row;
   int KEY2_;
   char cval, *tval;

   if (ossim_strcasecmp(key2,"ID")==0) {
      memcpy( &rowid, keyval1, sizeof(rowid) );
      return rowid;
   }

   rowid = 0;

   KEY2_ = table_pos(key2,table2);

   if ((table2.header[KEY2_].type != 'I')&&
       (table2.header[KEY2_].type != 'T')) return rowid;

   if ((table2.header[KEY2_].type == 'I')&&
       (table2.header[KEY2_].count != 1)) return rowid;

   if ((table2.header[KEY2_].type == 'T')&&(sort_flag)) sort_flag = 0;

   if (table2.header[KEY2_].type == 'I') memcpy(&kval,keyval1,sizeof(kval));

   if (!sort_flag) {   /* Sequential search */

      for (i=1;i<=table2.nrows;i++) {
	 row = get_row(i,table2);
	 if (table2.header[KEY2_].type == 'I') {
	    get_table_element(KEY2_,row,table2,&ival,&n);
	    if (ival == kval) rowid = i;
	 } else {
	    if (table2.header[KEY2_].count==1) {
	       get_table_element(KEY2_,row,table2,&cval,&n);
	       if (memcmp(&cval,keyval1,sizeof(ival))==0) rowid = i;
	    } else {
	       tval = (char*)get_table_element(KEY2_,row,table2,NULL,&n);
	       if (strcmp(tval,(char *)keyval1)==0) rowid = i;
	    }
	 }
	 free_row(row,table2);
	 if (rowid > 0) break;
      }

   } else {   /* Binary search */

      memcpy(&kval,keyval1,sizeof(kval));
      rowid = vpf_binary_search( kval, KEY2_, table2 );

   }

   return rowid;
}


/**************************************************************************
 *
 *N  related_rows
 *
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    Return the list of related rows of table2 based upon the value of
 *    table 1's key.
 *    Supported data types - I and T<n>.
 *    Binary search supported only for data type I. (column must be sorted)
 *    Thematic index used, if present on key column.
 *    NOTE: A sequential search operation will search the entire
 *          table ...zzz...
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels  DOS Turbo C
 *E
 *************************************************************************/
linked_list_type related_rows( void *keyval1,
		       vpf_table_type table2, char *key2,
		       int sort_flag,
		       ThematicIndex *idx )
{
   linked_list_type rowlist;
   set_type rowset;
   ossim_int32 rowid, i, ival, kval, n, start,end;
   row_type row = 0;
   int KEY2_;
   char cval, *tval;

   rowlist = ll_init();

   if (ossim_strcasecmp(key2,"ID")==0) {
      memcpy( &rowid, keyval1, sizeof(rowid) );
      ll_insert(&rowid,sizeof(rowid),rowlist);
      return rowlist;
   }

   KEY2_ = table_pos(key2,table2);

   if ((table2.header[KEY2_].type != 'I')&&
       (table2.header[KEY2_].type != 'T')) return rowlist;

   if ((table2.header[KEY2_].type == 'I')&&
       (table2.header[KEY2_].count != 1)) return rowlist;

   if ((table2.header[KEY2_].type == 'T')&&(sort_flag)) sort_flag = 0;

   if (idx) {
      if (idx->fp) {
	 rowset = search_thematic_index(idx,(char *)keyval1);
	 start = set_min(rowset);
	 end = set_max(rowset);
	 for (i=start;i<=end;i++)
	    if (set_member(i,rowset)) {
	       ll_insert(&i,sizeof(i),ll_last(rowlist));
	    }
	 set_nuke(&rowset);
	 return rowlist;
      }
   }

   if (!sort_flag) {   /* Sequential search */

      for (i=1;i<=table2.nrows;i++) {
	 row = get_row(i,table2);
	 if (table2.header[KEY2_].type == 'I') {
	    get_table_element(KEY2_,row,table2,&ival,&n);
	    if (memcmp(&ival,keyval1,sizeof(ival))==0)
	       ll_insert(&i,sizeof(i),ll_last(rowlist));
	 } else {
	    if (table2.header[KEY2_].count==1) {
	       get_table_element(KEY2_,row,table2,&cval,&n);
	       if (memcmp(&cval,keyval1,sizeof(ival))==0)
		  ll_insert(&i,sizeof(i),ll_last(rowlist));
	    } else {
	       tval = (char*)get_table_element(KEY2_,row,table2,NULL,&n);
	       if (strcmp(tval,(char *)keyval1)==0)
		  ll_insert(&i,sizeof(i),ll_last(rowlist));
	    }
	 }
	 free_row(row,table2);
      }

   } else {   /* Binary search */

      memcpy(&kval,keyval1,sizeof(kval));
      rowid = vpf_binary_search( kval, KEY2_, table2 );

      if (rowid > 0) {
	 ll_insert(&rowid,sizeof(rowid),ll_last(rowlist));
	 i = rowid-1L;
	 do {
	    get_row(i,table2);
	    get_table_element(KEY2_,row,table2,&ival,&n);
	    if (ival == kval)
	       ll_insert(&i,sizeof(i),ll_last(rowlist));
	    i--;
	 } while ((ival==kval)&&(i>0));
	 i = rowid+1L;
	 do {
	    get_row(i,table2);
	    get_table_element(KEY2_,row,table2,&ival,&n);
	    if (ival == kval)
	       ll_insert(&i,sizeof(i),ll_last(rowlist));
	    i++;
	 } while ((ival==kval)&&(i<=table2.nrows));
      }

   }

   return rowlist;
}


/**************************************************************************
 *
 *N  vpf_nullify_table
 *
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    Nullify the given VPF table structure.
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels  DOS Turbo C
 *E
 *************************************************************************/
void vpf_nullify_table( vpf_table_type *table )
{
   strcpy(table->name,"");
   table->path = NULL;
   table->nfields = 0;
   strcpy(table->description,"");
   strcpy(table->narrative,"");
   table->header = NULL;
   table->xfp = NULL;
   table->index = NULL;
   table->xstorage = (storage_type)0;
   table->fp = NULL;
   table->nrows = 0;
   table->row = NULL;
   table->reclen = 0;
   table->ddlen = 0;
   table->defstr = NULL;
   table->storage = (storage_type)0;
   table->mode = (file_mode)0;
   table->status = CLOSED;
}


/**************************************************************************
 *
 *N  select_feature_class_relate
 *
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    Set up the relationships between features and primitives or between
 *    primitives and features (one way only) for a specified feature class.
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels  DOS Turbo C
 *E
 *************************************************************************/
fcrel_type select_feature_class_relate( int fcnum,
					library_type *library,
					char *start_table,
					char *end_table )
{
   int storage, cov;
   vpf_table_type fcs;
   ossim_int32 i;
   char path[255], covpath[255];
   position_type p;
   vpf_relate_struct rcell;
   fcrel_type fcrel;

   fcrel.nchain = 0;
   fcrel.table = NULL;
   fcrel.relate_list = NULL;

   cov = library->fc[fcnum].coverage;
   strcpy(covpath,library->cover[cov].path);
   rightjust(covpath);
   sprintf( path, "%sfcs", covpath );

   /* Feature Class Schema table */
   fcs = vpf_open_table( path, disk, "rb", NULL );

   fcrel.relate_list = fcs_relate_list( library->fc[fcnum].name,
					start_table,end_table,
					fcs );

   if (ll_empty(fcrel.relate_list)) {
      ll_reset(fcrel.relate_list);
      displaymessage("ERROR in feature class relationship!",
		     start_table,end_table,NULL);
      return fcrel;
   }

   /* Find the number of tables in the relate chain */
   p = ll_first(fcrel.relate_list);
   fcrel.nchain = 0;
   while (!ll_end(p)) {
      fcrel.nchain++;
      p = ll_next(p);
   }
   /* Allow for last table2 */
   fcrel.nchain++;

   fcrel.table = (vpf_table_type *)
		  vpfmalloc((fcrel.nchain+1)*
			     sizeof(vpf_table_type));

   for (i=0;i<fcrel.nchain+1;i++)
      vpf_nullify_table( &(fcrel.table[i]) );


   p = ll_first(fcrel.relate_list);
   for (i=0;i<fcrel.nchain-1;i++) {

      ll_element(p,&rcell);

      /** Can't open primitive table - may be several under tile **/
      /** directories.  Open all others **/
      if (!is_primitive(rcell.table1)) {

	 sprintf(path,"%s%s",covpath,rcell.table1);
	 if (is_join(rcell.table1))
	    storage = ram;
	 else
	    storage = disk;

	 fcrel.table[i] = vpf_open_table(path,(storage_type)storage,"rb",NULL);

      }

      if (!ll_end(p)) p = ll_next(p);
   }

   /* End of relate chain */
   i = fcrel.nchain-1;
   if (!is_primitive(rcell.table2)) {

      sprintf(path,"%s%s",covpath,rcell.table2);
      storage = disk;

      fcrel.table[i] = vpf_open_table(path,(storage_type)storage,"rb",NULL);

   }


   vpf_close_table( &fcs );

   return fcrel;
}


/**************************************************************************
 *
 *N  fc_row_number
 *
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    Given the starting row of a feature class relationship, return the
 *    row number of the table at the end of the feature class relate
 *    chain.
 *    If your relate goes from the feature to the primitive, this will
 *    return the primitive id for the given feature row.
 *    If your relate goes from the primitive to the feature, this will
 *    return the feature id of the given primitive row.
 *
 *    Currently only supports relates on 'I' or 'K' fields.
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels  DOS Turbo C
 *E
 *************************************************************************/
ossim_int32 fc_row_number( row_type row, fcrel_type fcrel, ossim_int32 tile )
{
   row_type relrow;
   ossim_int32 count;
   ossim_int32 i, rownum, keyval;
   id_triplet_type triplet_keyval;
   int KEY1_, KEY_;
   position_type p;
   vpf_relate_struct rcell;

   p = ll_first(fcrel.relate_list);
   ll_element(p,&rcell);
   KEY1_ = table_pos(rcell.key1,fcrel.table[0]);

   get_table_element(0,row,fcrel.table[0],&rownum,&count);

   if (KEY1_ == 0) {     /* "ID" */
      keyval = rownum;
   } else {
      switch (fcrel.table[0].header[KEY1_].type) {
	 case 'I':
	    get_table_element(KEY1_,row,fcrel.table[0],&keyval,&count);
	    break;
	 case 'K':
	    get_table_element(KEY1_,row,fcrel.table[0],&triplet_keyval,
			      &count);
	    keyval = triplet_keyval.exid;
	    if (tile != triplet_keyval.tile) {
	       return -2;
	    }
	    break;
	 default:
	    keyval = 0;
	    break;
      }
   }

   p = ll_first(fcrel.relate_list);
   for (i=1;i<(fcrel.nchain-1);i++) {
      /* Relate through Join table(s) */
      rownum = related_row(&keyval,fcrel.table[i],rcell.key2,0);
      relrow = get_row(rownum,fcrel.table[i]);

      p = ll_next(p);
      ll_element(p,&rcell);
      KEY_ = table_pos(rcell.key1,fcrel.table[i]);

      if (KEY_ == 0) {     /* "ID" */
	 keyval = rownum;
      } else {
	 switch (fcrel.table[i].header[KEY_].type) {
	 case 'I':
	    get_table_element(KEY_,relrow,fcrel.table[i],&keyval,&count);
	    break;
	 case 'K':
	    get_table_element(KEY_,relrow,fcrel.table[i],&triplet_keyval,
			      &count);
	    keyval = triplet_keyval.exid;
	    if (tile != triplet_keyval.tile) {
	       return -2;
	    }
	    break;
	 default:
	    keyval = 0;
	    break;
	 }
      }

      free_row(relrow,fcrel.table[i]);
   }

   if (ossim_strcasecmp(rcell.key2,"ID")==0)
      rownum = keyval;
   else
      rownum = related_row(&keyval,fcrel.table[i],rcell.key2,0);

   return rownum;
}


/**************************************************************************
 *
 *N  fc_row_numbers
 *
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    Given the starting row of a feature class relationship, return the
 *    list of row numbers of the table at the end of the feature class
 *    relate chain.
 *    If your relate goes from the feature to the primitive, this will
 *    return the primitive ids for the given feature row.
 *    If your relate goes from the primitive to the feature, this will
 *    return the feature ids of the given primitive row.
 *
 *    Currently only supports relates on 'I' or 'K' fields.
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels  DOS Turbo C
 *E
 *************************************************************************/
linked_list_type fc_row_numbers( row_type row,
				 fcrel_type fcrel,
				 ossim_int32 tile,
				 ThematicIndex *idx )
{
   row_type relrow;
   ossim_int32 count;
   ossim_int32 n, rownum, keyval;
   id_triplet_type triplet_keyval;
   int KEY1_, KEY_;
   position_type p, prow, pkey;
   vpf_relate_struct rcell;
   linked_list_type rowlist, keylist, templist;

   p = ll_first(fcrel.relate_list);
   ll_element(p,&rcell);
   KEY1_ = table_pos(rcell.key1,fcrel.table[0]);

   get_table_element(0,row,fcrel.table[0],&rownum,&count);

   if (KEY1_ == 0) {     /* "ID" */
      keyval = rownum;
   } else {
      switch (fcrel.table[0].header[KEY1_].type) {
	 case 'I':
	    get_table_element(KEY1_,row,fcrel.table[0],&keyval,&count);
	    break;
	 case 'K':
	    get_table_element(KEY1_,row,fcrel.table[0],&triplet_keyval,
			      &count);
	    keyval = triplet_keyval.exid;
	    if (tile != triplet_keyval.tile) {
	       keyval = -2;
	    }
	    break;
	 default:
	    keyval = 0;
	    break;
      }
   }

   keylist = ll_init();
   ll_insert(&keyval,sizeof(keyval),keylist);

   n = 0;

   p = ll_first(fcrel.relate_list);
   for (n=1;n<(fcrel.nchain-1);n++) {

      /* Relate through Join table(s) */

      rowlist = ll_init();
      pkey = ll_first(keylist);
      while (!ll_end(pkey)) {
	 ll_element(pkey,&keyval);
	 templist = related_rows(&keyval,fcrel.table[n],rcell.key2,0,NULL);
	 prow = ll_first(templist);
	 while (!ll_end(prow)) {
	    ll_element(prow,&rownum);
	    if (!ll_locate(&rownum,rowlist))
	       ll_insert(&rownum,sizeof(rownum),ll_last(rowlist));
	    prow = ll_next(prow);
	 }
	 ll_reset(templist);
	 pkey = ll_next(pkey);
      }
      ll_reset(keylist);

      p = ll_next(p);
      ll_element(p,&rcell);
      KEY_ = table_pos(rcell.key1,fcrel.table[n]);

      keylist = ll_init();
      prow = ll_first(rowlist);
      while (!ll_end(prow)) {
	 ll_element(prow,&rownum);
	 relrow = get_row(rownum,fcrel.table[n]);

	 if (KEY_ == 0) {     /* "ID" */
	    keyval = rownum;
	 } else {
	    switch (fcrel.table[n].header[KEY_].type) {
	    case 'I':
	       get_table_element(KEY_,relrow,fcrel.table[n],&keyval,&count);
	       break;
	    case 'K':
	       get_table_element(KEY_,relrow,fcrel.table[n],&triplet_keyval,
				 &count);
	       keyval = triplet_keyval.exid;
	       if (tile != triplet_keyval.tile) {
		  keyval = -2;
	       }
	       break;
	    default:
	       keyval = 0;
	       break;
	    }
	 }
	 if (keyval > 0)
	    ll_insert(&keyval,sizeof(keyval),ll_last(keylist));
	 prow = ll_next(prow);
	 free_row(relrow,fcrel.table[n]);
      }
      ll_reset(rowlist);
   }

   rowlist = ll_init();
   p = ll_first(keylist);
   while (!ll_end(p)) {
      ll_element(p,&keyval);
      templist = related_rows(&keyval,fcrel.table[n],rcell.key2,0,idx);
      prow = ll_first(templist);
      while (!ll_end(prow)) {
	 ll_element(prow,&rownum);
	 if (!ll_locate(&rownum,rowlist))
	    ll_insert(&rownum,sizeof(rownum),ll_last(rowlist));
	 prow = ll_next(prow);
      }
      ll_reset(templist);
      p = ll_next(p);
   }
   ll_reset(keylist);

   return rowlist;
}


/**************************************************************************
 *
 *N  deselect_feature_class_relate
 *
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    Clear out a previously allocated feature class relate structure
 *    from memory.
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels  DOS Turbo C
 *E
 *************************************************************************/
void deselect_feature_class_relate( fcrel_type *fcrel )
{
   register int i;

   if (fcrel->nchain > 0) {
      for (i=0;i<fcrel->nchain;i++) {
	 if (fcrel->table[i].status == OPENED) {
	    vpf_close_table(&(fcrel->table[i]));
	 }
      }
      free(fcrel->table);
      ll_reset(fcrel->relate_list);
   }
   fcrel->nchain = 0;
}

