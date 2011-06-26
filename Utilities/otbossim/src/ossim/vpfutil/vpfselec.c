/*************************************************************************
 *
 *N  Module VPFSELEC - VPF SELECTED FEATURES
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This module contains functions for selecting VPF features and
 *     primitives.
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
 *    Barry Michaels   Nov 1991                           DOS Turbo C
 *                     Feb 1992 - Optimized for CD-ROM performance.
 *E
 *************************************************************************/

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


#ifdef __MSDOS__
#include <graphics.h>
#endif
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#ifdef __MSDOS__
#include <sys\stat.h>
#include <dos.h>
#else
#include <sys/stat.h>
#include <limits.h>
#endif
#include <ossim/vpfutil/vpfview.h>
#include <ossim/vpfutil/vpfprim.h>
#include <ossim/vpfutil/vpfrelat.h>
#include <ossim/vpfutil/mapgraph.h>
#include <ossim/vpfutil/projectn.h>
#include <ossim/vpfutil/vpftidx.h>
#include <ossim/vpfutil/vpfselec.h>
#include <ossim/vpfutil/vpfdraw.h>


extern char home[255];

/* Defined in VPFSPX.C */
set_type spatial_index_search( char *fname,
			       float x1, float y1, float x2, float y2 );


#ifdef __MSDOS__
/*************************************************************************
 *
 *N  read_selected_features
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    This function reads a saved selection set for a given theme of the
 *    specified view.
 *
 *    NOTE:  This function has "special knowledge" about the structure of
 *    a set.  If that structure is changed, this function must account
 *    for those changes.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *     view       <input>==(view_type *) view structure.
 *     themenum   <input>==(int) theme number.
 *     return    <output>==(set_type) set of selected features.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   May 1991                           DOS Turbo C
 *E
 *************************************************************************/
set_type read_selected_features( view_type *view, int themenum )
{
   set_type selset;
   char path[255], *num = "          ", byte;
   FILE *fp;
   register int i;

   /* Determine path name from theme number */
   strcpy(path,home);
   strcat(path,view->name);
   strcat(path,"\\sel");
   num = itoa(themenum,num,10);
   for (i=0;i<(4-strlen(num));i++)
      strcat(path,"0");
   strcat(path,num);

   if (access(path,0) != 0) {
      selset.size = 0;
      selset.buf = NULL;
      return selset;
   }

   /* Read the set */
   if ((fp = fopen(path,"rb")) == NULL) {
      selset.size = 0;
      selset.buf = NULL;
      return selset;
   }
   fread( &selset.size, sizeof(ossim_int32), 1, fp );
   selset.buf = (char *)malloc( ((selset.size/8L)+1L) * sizeof(char) );
   if (selset.buf) {
      fread( selset.buf, sizeof(char), (selset.size/8L)+1L, fp );
      selset.diskstorage = 0;
   } else {
      selset.diskstorage = 1;
      /* Can speed this up by buffering */
      selset.fp = tmpfile();
      if (!selset.fp) {
	 selset.diskstorage=0;
	 selset.size=0;
	 selset.buf=NULL;
	 return selset;
      }
      fread(&byte,1,1,selset.fp);
      while (!feof(fp)) {
	 fwrite(&byte,1,1,selset.fp);
	 fread(&byte,1,1,fp);
      }
   }
   fclose(fp);


   return selset;
}


/*************************************************************************
 *
 *N  save_selected_features
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    This function saves the selection set for a given theme of the
 *    specified view to a file in the view directory.
 *
 *    NOTE:  This function has "special knowledge" about the structure of
 *    a set.  If that structure is changed, this function must account
 *    for those changes.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *     view     <input>==(view_type *) view structure.
 *     themenum <input>==(int) theme number.
 *     selset   <input>==(set_type) set of selected features.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   May 1991                           DOS Turbo C
 *E
 *************************************************************************/
void save_selected_features( view_type *view, int themenum,
			     set_type selset )
{
   char path[80], *num="    ", byte;
   FILE *fp;
   register int i;

   /* Determine the path name from the theme number */
   strcpy(path,home);
   strcat(path,view->name);
   strcat(path,"\\sel");
   num = itoa(themenum,num,10);
   for (i=0;i<(4-strlen(num));i++)
      strcat(path,"0");
   strcat(path,num);

   /* Write the set */
   fp = fopen(path,"wb");
   if (!fp) return;
   fwrite( &(selset.size), sizeof(ossim_int32), 1, fp );
   if (!selset.diskstorage) {
      fwrite( selset.buf, sizeof(char), (selset.size/8L)+1L, fp );
   } else {
      rewind(selset.fp);
      fread(&byte,1,1,selset.fp);
      while (!feof(selset.fp)) {
	 fwrite(&byte,1,1,fp);
	 fread(&byte,1,1,selset.fp);
      }
   }
   fclose(fp);
}


/*************************************************************************
 *
 *N  get_selected_features
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    This function gets the selection set for a given theme of the
 *    specified view, either by querying the table, or by reading a
 *    previously saved selection set file.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *     view     <input>==(view_type *) view structure.
 *     themenum <input>==(int) theme number.
 *     library  <input>==(library-type) VPF library structure.
 *     return  <output>==(set_type) set of selected features.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   May 1991                           DOS Turbo C
 *E
 *************************************************************************/
set_type get_selected_features( view_type *view, int themenum,
				library_type library )
{
   set_type selset;
   vpf_table_type ft;
   register int i;

   /* Read a selection set, if present */
   if (strcmp(view->name,"") != 0) {
      selset = read_selected_features( view, themenum );
      if (selset.size > 0) return selset;
   }

   /* No selection set... */

   /* Find the feature class for the theme */
   for (i=0;i<library.nfc;i++)
      if (strcasecmp(library.fc[i].name,view->theme[themenum].fc)==0) break;
   if (i>=library.nfc) {
      display_message("Invalid theme in view");
      return selset;
   }

   /* Query the feature table */
   ft = vpf_open_table( library.fc[i].table, disk, "rb", NULL );
   selset = query_table( view->theme[themenum].expression, ft );
   vpf_close_table( &ft );

   /* Save the selection set so we don't have to query again */
   if (strcmp(view->name,"") != 0) {
      save_selected_features( view, themenum, selset );
   }

   return selset;
}



/*************************************************************************
 *
 *N  bounding_select
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    This function reads the bounding rectangle table to weed out the
 *    local primitives.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    path      <input> == (char *) path to the bounding rectangle table.
 *    mapextent <input> == (extent_type) map extent to compare.
 *    dec_degrees <input> == (int) flag to indicate if data is in decimal
 *                                 degrees.
 *    bounding_select <output> == (set_type) set of bounding rectangle ids.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   May 1991                           DOS Turbo C
 *E
 *************************************************************************/
set_type bounding_select( char *path, extent_type mapextent,
			  int dec_degrees )
{
   vpf_table_type table;
   set_type set;
   ossim_int32 i, count;
   extent_type box, pextent;
   double x1,y1,x2,y2;
   row_type row;
   int XMIN_,YMIN_,XMAX_,YMAX_;

   /* Project all extents to plate-carree for cartesian comparisons */
   /* (decimal degree coordinate systems) */

   x1 = mapextent.x1; y1 = mapextent.y1;
   x2 = mapextent.x2; y2 = mapextent.y2;
   if (dec_degrees) {
      set_plate_carree_parameters( central_meridian(x1,x2), 0.0, 1.0 );
      pcarree_xy(&x1,&y1);
      pcarree_xy(&x2,&y2);
   }
   pextent.x1 = x1; pextent.y1 = y1;
   pextent.x2 = x2; pextent.y2 = y2;

   table = vpf_open_table(path,disk,"rb",NULL);
   XMIN_ = table_pos("XMIN",table);
   YMIN_ = table_pos("YMIN",table);
   XMAX_ = table_pos("XMAX",table);
   YMAX_ = table_pos("YMAX",table);
   set = set_init(table.nrows+1);
   for (i=1;i<=table.nrows;i++) {
      row = read_next_row(table);
      get_table_element(XMIN_,row,table,&box.x1,&count);
      get_table_element(YMIN_,row,table,&box.y1,&count);
      get_table_element(XMAX_,row,table,&box.x2,&count);
      get_table_element(YMAX_,row,table,&box.y2,&count);
      free_row(row,table);

      x1 = box.x1; y1 = box.y1;
      x2 = box.x2; y2 = box.y2;
      if (dec_degrees) {
	 pcarree_xy(&x1,&y1);
	 pcarree_xy(&x2,&y2);
      }
      box.x1 = x1; box.y1 = y1;
      box.x2 = x2; box.y2 = y2;

      if ( contained(box,pextent) || contained(pextent,box) ) {
	 set_insert(i,set);
      }
   }
   vpf_close_table(&table);

   return set;
}



/*************************************************************************
 *
 *N  tile_thematic_index_name
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    This function determines the name of the tile thematic index for
 *    the given table.  If the table does not have a "TILE_ID" field,
 *    the name is set to null.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *     table    <input>==(vpf_table_type) opened VPF table.
 *     path    <output>==(char *) name of the thematic index file.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   May 1991                           DOS Turbo C
 *E
 *************************************************************************/
void tile_thematic_index_name( vpf_table_type table,
			       char *path )
{
   int i;

   i = table_pos("TILE_ID",table);
   if (i<0) {
      strcpy(path,"");
   } else {
      /* Thematic index name from the table header */
      sprintf(path,"%s%s",table.path,table.header[i].tdx);
   }
}
#endif
/*************************************************************************
 *
 *N  completely_within
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    This function determines whether extent1 is completely within
 *    extent2.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *     extent1 <input>==(extent_type) first extent to compare.
 *     extent2 <input>==(extent_type) second extent to compare.
 *     return  <output>==(int) 1 if extent1 is completely within extent2,
 *                             0 if not.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   May 1991                           DOS Turbo C
 *E
 *************************************************************************/
int completely_within( extent_type extent1, extent_type extent2 )
{
   if (extent1.x1 < extent2.x1) return 0;
   if (extent1.y1 < extent2.y1) return 0;
   if (extent1.x2 > extent2.x2) return 0;
   if (extent1.y2 > extent2.y2) return 0;
   return 1;
}

#ifdef __MSDOS__
/*************************************************************************
 *
 *N  get_selected_primitives
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    This function determines all of the selected primitive rows from
 *    the selected features.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *     view     <input> == (view_type *) view structure.
 *     themenum <input> == (int) theme number.
 *     library  <input> == (library_type *) VPF library structure.
 *     mapenv   <input> == (map_environment_type *) map environment.
 *     status  <output> == (int *) status of the function:
 *                         1 if completed, 0 if user escape.
 *     get_selected_primitives <output> == (set_type *) array of sets, each position
 *                         representing the set of primitives for the
 *                         corresponding tile in the tileref.aft table.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   May 1991                           DOS Turbo C
 *E
 *************************************************************************/
set_type *get_selected_primitives( view_type *view,
				   int themenum,
				   library_type *library,
				   map_environment_type *mapenv,
				   int *status )
{
   int fcnum, finished, found, cov, tilecover, TILEPATH_, degree;
   int feature, prim;
   vpf_table_type tile_table;
   row_type row;
   char *ptable[] = {"","edg","fac","txt","end","cnd"};
   char *spxname[] = {"","esi","fsi","tsi","nsi","csi"};
   char *brname[] = {"","ebr","fbr","tbr","nbr","cbr"};
   set_type feature_rows, primitive_rows, tile_features;
   set_type *primitives;
   ossim_int32 prim_rownum, count, tile;
   register ossim_int32 i,j, pclass, start,end;
   char path[255], covpath[255], tiledir[255], *buf, str[255];
   fcrel_type fcrel;
   linked_list_type primlist;
   position_type p;
   vpf_relate_struct rcell;
   ThematicIndex idx;

   primitives = (set_type *)vpfmalloc((library->ntiles+1)*sizeof(set_type));
   for (i=0;i<=library->ntiles;i++) {
      primitives[i].size = 0;
      primitives[i].buf = NULL;
   }

   fcnum = view->theme[themenum].fcnum;

   feature_rows = get_selected_features( view, themenum, *library );

   /* Open the tile reference table, if present */
   sprintf(path,"%stileref\\tileref.aft",library->path);
   if (access(path,0) != 0) {
      tilecover = FALSE;
   } else {
      tile_table = vpf_open_table(path,disk,"rb",NULL);
      TILEPATH_ = table_pos("TILE_NAME",tile_table);
      tilecover = TRUE;
   }

   for (pclass=EDGE;pclass<=CONNECTED_NODE;pclass++) {

      if ((pclass != library->fc[fcnum].primclass) &&
	  (library->fc[fcnum].primclass != COMPLEX_FEATURE)) continue;

      /* Set up the feature class table relate chain.        */
      /* The feature table is fcrel.table[0].                */
      /* The primitive table is the last table in the chain: */
      /*    fcrel.table[ fcrel.nchain-1 ].                   */

      j = 0;
      for (i=0;i<strlen(library->fc[fcnum].table);i++)
	 if (library->fc[fcnum].table[i] == '\\') j = i+1;
      strcpy( str, &(library->fc[fcnum].table[j]));
      fcrel = select_feature_class_relate(fcnum, library, str,
					  ptable[pclass]);
      feature = 0;
      prim = fcrel.nchain-1;

      p = ll_previous(ll_last(fcrel.relate_list),fcrel.relate_list);
      ll_element(p,&rcell);
      degree = rcell.degree;

      /*** 'Tile' number 1 is the universe polygon for the tileref cover ***/
      for (tile = 2; tile <= library->ntiles; tile++ ) {

	 if (!set_member(tile,library->tile_set)) continue;

	 if (tilecover) {
	    row = get_row(tile,tile_table);
	    buf = (char *)get_table_element(TILEPATH_,row,tile_table,
					     NULL,&count);
	    free_row(row,tile_table);
	    strcpy(tiledir,buf);
	    rightjust(tiledir);
	    strcat(tiledir,"\\");
	    free(buf);
	 } else {
	    strcpy(tiledir,"");
	 }

	 cov = library->fc[fcnum].coverage;
	 strcpy( covpath, library->cover[cov].path );

	 finished = 1;
	 found = 0;

	 /* Open primitive table in tile */
	 sprintf(path,"%s%s%s",covpath,tiledir,ptable[pclass]);
	 if (access(path,0) != 0) continue;
	 found = 1;
	 fcrel.table[prim] = vpf_open_table(path,disk,"rb",NULL);

	 if (primitives[tile].size > 0) {
	    printf("Oops!  size = %ld\n",primitives[tile].size);
	    exit(1);
	 }
	 primitives[tile] = set_init(fcrel.table[prim].nrows+1);

	 /* Get the set of primitive rows within the map extent */

	 /* Look for spatial index file */
	 primitive_rows.size = 0;
	 if (mapenv->projection == PLATE_CARREE &&
	     mapenv->mapextent.x1 < mapenv->mapextent.x2) {
	    sprintf(path,"%s%s%s",covpath,tiledir,spxname[pclass]);
	    if ((access(path,0)==0)&&(fcrel.table[prim].nrows > 20)) {

	       primitive_rows = spatial_index_search(path,
			  mapenv->mapextent.x1,mapenv->mapextent.y1,
			  mapenv->mapextent.x2,mapenv->mapextent.y2);

	    } else {
	       /* Next best thing - bounding rectangle table */
	       sprintf(path,"%s%s%s",covpath,tiledir,brname[pclass]);
	       if (access(path,0)==0) {
		  primitive_rows = bounding_select(path,mapenv->mapextent,
						   library->dec_degrees);
	       }
	    }
	 } /* projection check */

	 if (primitive_rows.size == 0) {
	    /* Search through all the primitives */
	    primitive_rows=set_init(fcrel.table[prim].nrows+1);
	    set_on(primitive_rows);
	 }

	 tile_thematic_index_name(fcrel.table[feature], path);
	 if ( (strcmp(path,"") != 0) && (access(path,4)==0) ) {
	    /* Tile thematic index for feature table present, */
	    tile_features = read_thematic_index( path, (char *)&tile );
	 } else {
	    tile_features = set_init(fcrel.table[feature].nrows+1);
	    set_on(tile_features);
	 }

	 idx.fp = NULL;
	 i = table_pos(rcell.key2,fcrel.table[prim]);
	 if (i >= 0) {
	    if (fcrel.table[prim].header[i].tdx) {
	       sprintf(path,"%s%s",fcrel.table[prim].path,
				fcrel.table[prim].header[i].tdx);
	       if (access(path,0)==0)
		  idx = open_thematic_index(path);
	    }
	    if (fcrel.table[prim].header[i].keytype == 'U') degree = R_ONE;
	    if (fcrel.table[prim].header[i].keytype == 'N') degree = R_MANY;
	    if (fcrel.table[prim].header[i].keytype == 'P') degree = R_ONE;
	 }

	 finished = 1;

	 start = set_min(tile_features);
	 end = set_max(tile_features);

	 fseek(fcrel.table[feature].fp,
	       index_pos(start,fcrel.table[feature]),
	       SEEK_SET);

	 for (i=start;i<=end;i++) {

	    row = read_next_row(fcrel.table[feature]);

	    if (!set_member( i, feature_rows )) {
	       free_row(row,fcrel.table[feature]);
	       continue;
	    }

	    if (!set_member( i, tile_features )) {
	       free_row(row,fcrel.table[feature]);
	       continue;
	    }

	    if (degree == R_ONE) {
	       prim_rownum = fc_row_number( row, fcrel, tile );
	       primlist = NULL;
	       p = NULL;
	    } else {
	       primlist = fc_row_numbers( row, fcrel, tile, &idx );
	    }

	    free_row( row, fcrel.table[feature] );

	    if (!primlist) {
	       if ((prim_rownum<1)||
		   (prim_rownum>fcrel.table[prim].nrows))
		  continue;
	       if (set_member( prim_rownum, primitive_rows )) {
		  set_insert(prim_rownum,primitives[tile]);
	       }
	    } else {
	       p = ll_first(primlist);
	       while (!ll_end(p)) {
		  ll_element(p,&prim_rownum);
		  if ((prim_rownum<1)||
		      (prim_rownum>fcrel.table[prim].nrows))
		     continue;
		  if (set_member( prim_rownum, primitive_rows )) {
		     set_insert(prim_rownum,primitives[tile]);
		  }
		  p = ll_next(p);
	       }
	    }

	    if (kbhit()) {
	       if (getch()==27) {
		  *status = 0;
		  finished = 0;
		  break;
	       }
	    }
	 }

	 if (idx.fp) close_thematic_index(&idx);

	 vpf_close_table(&(fcrel.table[prim]));

	 set_nuke(&primitive_rows);

	 set_nuke(&tile_features);

	 if (set_empty(primitives[tile])) {
	    set_nuke(&primitives[tile]);
	    primitives[tile].size = 0;
	    primitives[tile].buf = NULL;
	 }

	 if (!finished) {
	    *status = 0;
	    break;
	 }

      }

      if (!finished) {
	 *status = 0;
	 deselect_feature_class_relate( &fcrel );
	 break;
      }

      if (found) *status = 1;

      if (kbhit()) {
	 if (getch()==27) {
	    *status = 0;
	    deselect_feature_class_relate( &fcrel );
	    break;
	 }
      }

      deselect_feature_class_relate( &fcrel );
   }

   set_nuke(&feature_rows);

   if (tilecover) {
      vpf_close_table(&tile_table);
   }

   return primitives;
}



/*************************************************************************
 *
 *N  get_selected_tile_primitives
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    This function determines all of the selected primitive rows from
 *    the selected features of a given tile.
 *
 *    This function expects a feature class relationship structure that
 *    has been successfully created with select_feature_class_relate()
 *    from the feature table to the primitive.  The primitive table in
 *    the feature class relate structure must have been successfully
 *    opened for the given tile.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *     library   <input>==(library_type *) VPF library structure.
 *     fcnum     <input>==(int) feature class number of the feature table.
 *     fcrel     <input>==(fcrel_type) feature class relate structure.
 *     feature_rows <input>==(set_type) set of selected features.
 *     mapenv    <input>==(map_environment_type *) map environment.
 *     tile      <input>==(ossim_int32) tile number.
 *     tiledir   <input>==(char *) path to the tile directory.
 *     status   <output>==(int *) status of the function:
 *                         1 if completed, 0 if user escape.
 *     return   <output>==(set_type) set of primitives for the features
 *                         in the corresponding tile.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   May 1991                           DOS Turbo C
 *E
 *************************************************************************/
set_type get_selected_tile_primitives( library_type *library,
				       int fcnum,
				       fcrel_type fcrel,
				       set_type feature_rows,
				       map_environment_type *mapenv,
				       ossim_int32 tile,
				       char *tiledir,
				       int *status )
{
   int cov, degree;
   int feature, prim;
   row_type row;
   char *spxname[] = {"","esi","fsi","tsi","nsi","csi"};
   char *brname[] = {"","ebr","fbr","tbr","nbr","cbr"};
   set_type primitive_rows, tile_features;
   set_type primitives;
   ossim_int32 prim_rownum;
   register ossim_int32 i,pclass, start,end;
   char path[255], covpath[255];
   linked_list_type primlist;
   position_type p;
   vpf_relate_struct rcell;
   ThematicIndex idx;

   feature = 0;
   prim = fcrel.nchain-1;

   /* Assume that fcrel.table[prim] has been opened */

   primitives.size = 0;
   primitives.buf = NULL;

   cov = library->fc[fcnum].coverage;
   strcpy( covpath, library->cover[cov].path );

   p = ll_previous(ll_last(fcrel.relate_list),fcrel.relate_list);
   ll_element(p,&rcell);
   degree = rcell.degree;

   for (pclass=EDGE;pclass<=CONNECTED_NODE;pclass++) {

      if ((pclass != library->fc[fcnum].primclass) &&
	  (library->fc[fcnum].primclass != COMPLEX_FEATURE)) continue;

      primitives = set_init(fcrel.table[prim].nrows+1);

      /* Get the set of primitive rows within the map extent */

      /* Look for the spatial index file to weed out primitives in the */
      /* given tile but outside of the viewing area.  If a projection  */
      /* other than plate-carree (rectangular) is on, or if the extent */
      /* crosses the meridian, the quick check is no longer valid.     */

      primitive_rows.size = 0;

      if (mapenv->projection == PLATE_CARREE &&
	  mapenv->mapextent.x1 < mapenv->mapextent.x2) {
	 sprintf(path,"%s%s%s",covpath,tiledir,spxname[pclass]);
	 /* 20 (below) is a fairly arbitrary cutoff of the number of */
	 /* primitives that make a spatial index search worth while. */
	 if ((access(path,0)==0)&&(fcrel.table[prim].nrows > 20)) {
	    primitive_rows = spatial_index_search(path,
			  mapenv->mapextent.x1,mapenv->mapextent.y1,
			  mapenv->mapextent.x2,mapenv->mapextent.y2);
	 } else {
	    /* Next best thing - bounding rectangle table */
	    sprintf(path,"%s%s%s",covpath,tiledir,brname[pclass]);
	    if ((access(path,0)==0)&&(fcrel.table[prim].nrows > 20)) {
	       primitive_rows = bounding_select(path,mapenv->mapextent,
						library->dec_degrees);
	    }
	 }
      }
      if (primitive_rows.size == 0) {
	 /* Search through all the primitives */
	 primitive_rows=set_init(fcrel.table[prim].nrows+1);
	 set_on(primitive_rows);
      }

      if (strcmp(tiledir,"") != 0) {
	 tile_thematic_index_name(fcrel.table[feature], path);
	 if ((strcmp(path,"")!=0) && (access(path,4)==0)) {
	    /* Tile thematic index for feature table present, */
	    tile_features = read_thematic_index( path, (char *)&tile );
	 } else {
	    tile_features = set_init(fcrel.table[feature].nrows+1);
	    set_on(tile_features);
	 }
      } else {
	 tile_features = set_init(fcrel.table[feature].nrows+1);
	 set_on(tile_features);
      }
      set_delete(0,tile_features);

      idx.fp = NULL;
      i = table_pos(rcell.key2,fcrel.table[prim]);
      if (i >= 0) {
	 if (fcrel.table[prim].header[i].tdx) {
	    sprintf(path,"%s%s",fcrel.table[prim].path,
				fcrel.table[prim].header[i].tdx);
	    if (access(path,0)==0)
	       idx = open_thematic_index(path);
	 }
	 if (fcrel.table[prim].header[i].keytype == 'U') degree = R_ONE;
	 if (fcrel.table[prim].header[i].keytype == 'N') degree = R_MANY;
	 if (fcrel.table[prim].header[i].keytype == 'P') degree = R_ONE;
      }

      start = set_min(tile_features);
      end = set_max(tile_features);

      /* It turns out to be MUCH faster off of a CD-ROM to */
      /* read each row and discard unwanted ones than to   */
      /* forward seek past them.  It's about the same off  */
      /* of a hard disk.				      */

      fseek(fcrel.table[feature].fp,index_pos(start,fcrel.table[feature]),
	    SEEK_SET);

      for (i=start;i<=end;i++) {

	 row = read_next_row(fcrel.table[feature]);

	 if (!set_member( i, feature_rows )) {
	    free_row(row,fcrel.table[feature]);
	    continue;
	 }
	 if (!set_member( i, tile_features )) {
	    free_row(row,fcrel.table[feature]);
	    continue;
	 }

	 if (degree == R_ONE) {
	    prim_rownum = fc_row_number( row, fcrel, tile );
	    primlist = NULL;
	    p = NULL;
	 } else {
	    primlist = fc_row_numbers( row, fcrel, tile, &idx );
	 }

	 free_row( row, fcrel.table[feature] );

	 if (!primlist) {
	    if ((prim_rownum<1)||(prim_rownum>fcrel.table[prim].nrows))
	       continue;
	    if (set_member( prim_rownum, primitive_rows )) {
	       set_insert(prim_rownum,primitives);
	    }
	 } else {
	    p = ll_first(primlist);
	    while (!ll_end(p)) {
	       ll_element(p,&prim_rownum);
	       if ((prim_rownum<1)||
		   (prim_rownum>fcrel.table[prim].nrows))
		  continue;
	       if (set_member( prim_rownum, primitive_rows )) {
		  set_insert(prim_rownum,primitives);
	       }
	       p = ll_next(p);
	    }
	 }

	 if (primlist) ll_reset(primlist);

	 if (kbhit()) {
	    if (getch()==27) {
	       *status = 0;
	       break;
	    }
	 }
      }

      set_nuke(&primitive_rows);

      set_nuke(&tile_features);

      if (idx.fp) close_thematic_index(&idx);

      *status = 1;

      if (kbhit()) {
	 if (getch()==27) {
	    *status = 0;
	    break;
	 }
      }
   }

   return primitives;
}
#endif

#if __MSDOS__
static ossim_int32 filesize( char *file )
{
   struct stat s;
   stat(file,&s);
   return (ossim_int32)s.st_size;
}

static ossim_int32 available_space( char *drive )
{
   struct dfree disktable;
   int disknum;

   if (!drive) return 0;

   disknum=toupper(drive[0])-'A'+1;
   getdfree(disknum,&disktable);
   return (ossim_int32) disktable.df_avail *
	  (ossim_int32) disktable.df_sclus *
	  (ossim_int32) disktable.df_bsec;
}
#endif

#ifdef __MSDOS__
/*************************************************************************
 *
 *N  draw_selected_features
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    This function draws the selected features from a specified feature
 *    class based upon a query (either an expression or the pre-compiled
 *    results of an expression).
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *     view     <inout>==(view_type *) view structure.
 *     themenum <input>==(int) theme number.
 *     library  <input>==(library_type *) VPF library structure.
 *     mapenv   <input>==(map_environment_type *) map environment structure.
 *     return  <output>==(int) completion status:
 *                                1 if completed successfully,
 *                                0 if an error occurred.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   August 1991                        DOS Turbo C
 *E
 *************************************************************************/
int draw_selected_features( view_type *view,
			    int themenum,
			    library_type *library,
			    map_environment_type *mapenv )
{
   int status, fcnum, finished, cov, tilecover, TILEPATH_, prim;
   int outline, color1, color2, color3, color4;
   vpf_table_type rngtable,edgtable,fbrtable, tile_table;
   row_type row;
   char *ptable[] = {"","edg","fac","txt","end","cnd"};
   register ossim_int32 i, j, p, pclass, tile;
   int display_order[] = {FACE,EDGE,ENTITY_NODE,CONNECTED_NODE,TEXT};
   register ossim_int32 starttile, endtile, startprim, endprim;
   ossim_int32 count;
   char path[255], covpath[255], tiledir[255], *buf, str[255];
   char *drive, rngpath[255],edgpath[255],edxpath[255],fbrpath[255];
   boolean rng_rdisk,edg_rdisk,fbr_rdisk;
   set_type primitives, feature_rows;
   fcrel_type fcrel;
   window_type info;
   struct viewporttype vp;

   getviewsettings(&vp);

   fcnum = view->theme[themenum].fcnum;

   sprintf(path,"%stileref\\tileref.aft",library->path);
   if (access(path,0) != 0) {
      tilecover = FALSE;
   } else {
      tile_table = vpf_open_table(path,disk,"rb",NULL);
      TILEPATH_ = table_pos("TILE_NAME",tile_table);
      tilecover = TRUE;
   }

   feature_rows = get_selected_features( view, themenum, *library );

   for (p=0;p<5;p++) {
      pclass = display_order[p];

      if ((pclass != library->fc[fcnum].primclass) &&
	  (library->fc[fcnum].primclass != COMPLEX_FEATURE)) continue;

      if ((library->fc[fcnum].primclass == COMPLEX_FEATURE) &&
	  (!library->fc[fcnum].cprim[pclass])) continue;

      /* Set up the feature class table relate chain.        */
      /* The feature table is fcrel.table[0].                */
      /* The primitive table is the last table in the chain: */
      /*    fcrel.table[ fcrel.nchain-1 ].                   */

      j = 0;
      for (i=0;i<strlen(library->fc[fcnum].table);i++)
	 if (library->fc[fcnum].table[i] == '\\') j = i+1;
      strcpy( str, &(library->fc[fcnum].table[j]));
      fcrel = select_feature_class_relate(fcnum, library, str,
					  ptable[pclass]);
      prim = fcrel.nchain-1;

      /*** 'Tile' number 1 is the universe polygon for
	   the tileref cover ***/
      starttile = set_min(library->tile_set);
      if (starttile < 2) starttile = 2;
      endtile = set_max(library->tile_set);
      if (endtile < 2) endtile = 2;

      for (tile = starttile; tile <= endtile; tile++ ) {

	 if (!set_member(tile,library->tile_set)) continue;

	 if (tilecover) {
	    row = get_row(tile,tile_table);
	    buf = (char *)get_table_element(TILEPATH_,row,tile_table,
					     NULL,&count);
	    free_row(row,tile_table);
	    strcpy(tiledir,buf);
	    rightjust(tiledir);
	    strcat(tiledir,"\\");
	    free(buf);
	 } else {
	    strcpy(tiledir,"");
	 }

	 cov = library->fc[fcnum].coverage;
	 strcpy( covpath, library->cover[cov].path );

	 finished = TRUE;

	 sprintf(path,"%s%s%s",covpath,tiledir,ptable[pclass]);

	 if (access(path,0) != 0) continue;
	 fcrel.table[prim] = vpf_open_table(path,disk,"rb",NULL);

	 info = info_window("Searching...");

	 primitives = get_selected_tile_primitives( library,
						    fcnum, fcrel,
						    feature_rows,
						    mapenv,
						    tile, tiledir,
						    &status );
	 delete_window(&info);
	 setviewport(vp.left,vp.top,vp.right,vp.bottom,vp.clip);

	 /* Reset plate-carree parameters (changed in  */
	 /* get_selected_tile_primitives() )           */
	 if (mapenv->projection == PLATE_CARREE)
	    set_plate_carree_parameters( central_meridian(
					 mapenv->mapextent.x1,
					 mapenv->mapextent.x2),
					 0.0, 1.0 );

	 if (primitives.size < 1) {
	    vpf_close_table(&fcrel.table[prim]);
	    continue;
	 }

	 if (!status) {
	    set_nuke(&primitives);
	    vpf_close_table(&fcrel.table[prim]);
	    break;
	 }

	 if (pclass == FACE) {
	    /* Must also open RNG, EDG, and FBR for drawing faces. */
	    /* If a RAM disk is specified, copy these to it and open */
	    /* them there. */
	    rng_rdisk = FALSE;
	    edg_rdisk = FALSE;
	    fbr_rdisk = FALSE;
	    drive = getenv("TMP");
	    buf = (char *)vpfmalloc(255);

	    sprintf(path,"%s%srng",covpath,tiledir);
	    strcpy(rngpath,path);
	    if (drive && filesize(path) < available_space(drive)) {
	       sprintf(rngpath,"%s\\RNG",drive);
	       sprintf(buf,"COPY %s %s > NUL",path,rngpath);
	       system(buf);
	       rng_rdisk = TRUE;
	    }
	    rngtable = vpf_open_table(rngpath,disk,"rb",NULL);

	    sprintf(path,"%s%sedg",covpath,tiledir);
	    strcpy(edgpath,path);
            sprintf(edxpath,"%s%sedx",covpath,tiledir);
	    if (drive &&
	       (filesize(path)+filesize(edxpath))<available_space(drive)) {
	       sprintf(edgpath,"%s\\EDG",drive);
	       sprintf(buf,"COPY %s %s > NUL",path,edgpath);
	       system(buf);
	       sprintf(edxpath,"%s\\EDX",drive);
	       sprintf(buf,"COPY %s%sedx %s > NUL",covpath,tiledir,edxpath);
	       system(buf);
	       edg_rdisk = TRUE;
	    }
	    edgtable = vpf_open_table(edgpath,disk,"rb",NULL);

	    sprintf(path,"%s%sfbr",covpath,tiledir);
	    strcpy(fbrpath,path);
	    if (drive && filesize(path) < available_space(drive)) {
	       sprintf(fbrpath,"%s\\FBR",drive);
	       sprintf(buf,"COPY %s %s > NUL",path,fbrpath);
	       system(buf);
	       fbr_rdisk = TRUE;
	    }
	    fbrtable = vpf_open_table(fbrpath,disk,"rb",NULL);

	    free(buf);
	 }

	 finished = 1;

	 startprim = set_min(primitives);
	 endprim = set_max(primitives);

	 /* It turns out to be MUCH faster off of a CD-ROM to */
	 /* read each row and discard unwanted ones than to   */
	 /* forward seek past them.  It's about the same off  */
	 /* of a hard disk.				      */

	 fseek(fcrel.table[prim].fp,
	       index_pos(startprim,fcrel.table[prim]),
	       SEEK_SET);

	 for (i=startprim;i<=endprim;i++) {

	    row = read_next_row(fcrel.table[prim]);

	    if (set_member( i, primitives )) {
	       /* Draw the primitive */
	       switch (pclass) {
		  case EDGE:
		     finished = draw_edge_row(row,fcrel.table[prim]);
		     break;
		  case ENTITY_NODE:
		  case CONNECTED_NODE:
		     finished = draw_point_row(row,fcrel.table[prim]);
		     break;
		  case FACE:
		     gpgetlinecolor( &outline );
		     gpgetpattern( &color1, &color2, &color3, &color4 );
		     hidemousecursor();
		     draw_face_row( row,fcrel.table[prim],
				    rngtable, edgtable, fbrtable,
				    outline,
				    color1, color2, color3, color4 );
		     showmousecursor();
		     finished = 1;
		     if (kbhit()) {
			if (getch()==27) finished = 0;
		     }
		     break;
		  case TEXT:
		     finished = draw_text_row(row,fcrel.table[prim]);
		     break;
	       }
	    }

	    free_row(row,fcrel.table[prim]);

	    if (!finished) {
	       status = 0;
	       break;
	    }
	 }

	 if (pclass == FACE) {
	    vpf_close_table(&rngtable);
	    if (rng_rdisk) remove(rngpath);
	    vpf_close_table(&edgtable);
	    if (edg_rdisk) {
	       remove(edgpath);
	       remove(edxpath);
	    }
	    vpf_close_table(&fbrtable);
	    if (fbr_rdisk) remove(fbrpath);
	 }

	 vpf_close_table(&fcrel.table[prim]);

	 set_nuke(&primitives);

	 if (!finished) {
	    status = 0;
	    break;
	 }

      }

      if (!finished) {
	 status = 0;
	 deselect_feature_class_relate( &fcrel );
	 break;
      }

      status = 1;

      if (kbhit()) {
	 if (getch()==27) {
	    status = 0;
	    deselect_feature_class_relate( &fcrel );
	    break;
	 }
      }

      deselect_feature_class_relate(&fcrel);
   }

   if (tilecover) {
      vpf_close_table(&tile_table);
   }

   set_nuke(&feature_rows);

   return status;
}
#endif


