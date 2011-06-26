
/* VPFRELATE.H   -   Functions supporting relates between VPF tables */
		     /* One to many relates */
#ifndef vpfrelat_HEADER
#define vpfrelat_HEADER
#include <ossim/vpfutil/vpftidx.h>
#ifdef __cplusplus
extern "C" {
#endif

#define R_ONE 1
#define R_MANY 2

typedef struct {
   char table1[40];
   char key1[40];
   char table2[40];
   char key2[40];
   int degree;
} vpf_relate_struct;

typedef struct {
   int nchain;
   vpf_table_type *table;
   linked_list_type relate_list;
} feature_class_relate_type, fcrel_type;


linked_list_type fcs_relate_list( char *fcname, char *start_table,
				  char *end_table, vpf_table_type fcs );

ossim_int32 related_row( void *keyval1,
		      vpf_table_type table2, char *key2,
		      int sort_flag );

linked_list_type related_rows( void *keyval1,
		       vpf_table_type table2, char *key2,
		       int sort_flag,
		       ThematicIndex *idx );


fcrel_type select_feature_class_relate( int fcnum, library_type *library,
					char *start_table, char *end_table );

ossim_int32 fc_row_number( row_type row, fcrel_type fcrel, ossim_int32 tile );

linked_list_type fc_row_numbers( row_type row,
				 fcrel_type fcrel,
				 ossim_int32 tile,
				 ThematicIndex *idx );

void deselect_feature_class_relate( fcrel_type *fcrel );

#ifdef __cplusplus
}
#endif

#endif
