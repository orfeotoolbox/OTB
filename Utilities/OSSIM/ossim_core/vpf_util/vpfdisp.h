
/*******************/
/*   vpfdispl.h    */
/*******************/

#ifndef __vpfdisp_h__
#define __vpfdisp_h__

#include "vpfview.h"
#ifdef __cplusplus
extern "C" {
#endif


void vpf_display_record( row_type row,
			 vpf_table_type table,
			 FILE *fp );


void display_attributes( int fc,
			 long int row,
			 library_type *library,
			 FILE *fp );


void format_date( date_type date, char *fmtdate );

#ifdef __cplusplus
}
#endif
#endif   /* #ifndef __vpfdisp_h__  */
