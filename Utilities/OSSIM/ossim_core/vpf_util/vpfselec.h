/* VPFSELEC.H - SELECT VPF FEATURES */

#ifndef __VPFSELEC_H__

#define __VPFSELEC_H__ 1

#ifdef __cplusplus
extern "C" {
#endif

set_type read_selected_features( view_type *view, int themenum );

void save_selected_features( view_type *view, int themenum,
			     set_type selset );

set_type get_selected_features( view_type *view, int themenum,
				library_type library );

set_type bounding_select( char *path, extent_type mapextent,
			  int dec_degrees );

#ifndef __MSDOS__
int completely_within( extent_type extent1, extent_type extent2 );
#endif

set_type *get_selected_primitives( view_type *view, int themenum,
				   library_type *library,
				   map_environment_type *mapenv,
				   int *status );

int draw_selected_features( view_type *view,
			    int themenum,
			    library_type *library,
			    map_environment_type *mapenv );


#ifdef __cplusplus
}
#endif

#endif
