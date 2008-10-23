#ifndef vpfinit_HEADER
#define vpfinit_HEADER
/* VPFINIT.H */
#ifdef __cplusplus
extern "C" {
#endif

void vpfinit( database_type *database, library_type *library,
	      view_type *view, map_environment_type *mapenv );

void free_database( database_type *database );

void free_library( library_type *library );

void free_view( view_type *view );

void vpfreset( database_type *database, library_type *library,
	       view_type *view, map_environment_type *mapenv );

void vpfexit( database_type *database, library_type *library,
	      view_type *view, map_environment_type *mapenv );

database_type init_database( char *dbpath, VPF_BOOLEAN *ok );

int pick_library( database_type database, char *name );

library_type init_library( database_type database, char *libname,
			   VPF_BOOLEAN *ok );

view_type init_view( char *viewname, library_type library );

void set_viewing_area( library_type *library,
		       map_environment_type *mapenv,
		       extent_type extent );

void set_default_area( library_type *library,
		       map_environment_type *mapenv,
		       float x, float y );

#ifdef __cplusplus
}
#endif

#endif
