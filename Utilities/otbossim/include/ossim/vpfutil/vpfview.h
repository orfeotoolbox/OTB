/* VPFVIEW.H */

#ifndef __VPFVIEW_H__
#define __VPFVIEW_H__

#include <stdlib.h> /* to pick up min/max... */

#ifdef __cplusplus
extern "C" {
#endif
   
#include <ossim/vpfutil/set.h>
#ifdef __MSDOS__
#  include "gui.h"
#endif
#include <ossim/vpfutil/vpftable.h>
#include <ossim/vpfutil/linklist.h>

#ifdef __MSDOS__
typedef unsigned char VPF_BOOLEAN;
#else
typedef unsigned VPF_BOOLEAN;
#if (!defined(_MSC_VER))
#  ifndef stricmp
#    define stricmp(a,b) ossim_strcasecmp(a,b)
#  endif
#  ifndef strcmpi
#    define strcmpi(a,b) ossim_strcasecmp(a, b)
#  endif
#  ifndef strncmpi
#    define strncmpi(a,b,c) ossim_strncasecmp(a,b,c)
#  endif
#  ifndef ltoa
#    define ltoa(l,s,len) sprintf(s,"%d",l)
#  endif
#  ifndef itoa
#    define itoa(i,s,len) sprintf(s,"%d",i)
#  endif 
#endif
   
#endif

typedef char color_type;


/* Degree-minutes-seconds type */
typedef struct {
   int   degrees;
   int   minutes;
   float seconds;
} dms_type;


/* Geographic extent */
typedef struct {
   float x1, y1, x2, y2;
} extent_type;

/* Currently supported VPF versions */
typedef enum { VPF_0_7, VPF_0_8 } vpf_version_type;


/* VPF database internal structure */
typedef struct {
   char name[9];      /* Name of the VPF database */
   char *path;        /* DOS path name to the database */
   char **library;    /* Array of library names within the database */
   int  nlibraries;   /* Number of libraries in the database */
   char *producer;    /* Producer of the database - from header table */
   vpf_version_type version; /* VPF Version of the database */
   int byte_order;    /* Byte order of the database */
} database_type;


/* VPF coverage internal structure */
typedef struct {
   char name[9];        /* Name of the coverage */
   char *description;   /* Description of the coverage */
   char *path;          /* DOS path name to the coverage */
   int  topolevel;      /* Topology level of the coverage */
} coverage_type;


/* VPF feature types */
typedef enum { VPF_LINE=1, VPF_AREA, VPF_ANNO, VPF_POINT, VPF_COMPLEX_FEATURE=6 } vpf_feature_type;

/* VPF primitive types */
typedef enum { VPF_EDGE=1, VPF_FACE, VPF_TEXT, VPF_ENTITY_NODE, VPF_CONNECTED_NODE }
   vpf_primitive_type;


/* Units of measure */
typedef enum { VPF_UNKNOWN_UNITS, VPF_METERS, VPF_SECONDS, VPF_FEET, VPF_INCHES,
	       VPF_KILOMETERS, VPF_OTHER_UNITS, VPF_DEC_DEGREES } units_type;


/* VPF feature class internal structure */
typedef struct {
   char name[40];       /* Name of the feature class */
   char *description;   /* Description of the feature class */
   int  coverage;       /* Containing coverage number */
   char *table;         /* Feature table name */
   vpf_primitive_type primclass; /* Primitive class */
   char cprim[6];       /* Array of primclasses if primclass=COMPLEX */
   set_type view_set;   /* Themes of the view in this feature class */
} feature_class_type;


/* VPF library internal structure */
typedef struct {
   char name[9];            /* Name of the library */
   char database[9];        /* Name of the containing database */
   char *path;              /* DOS path name to the library */
   char *description;       /* Library description */
   extent_type extent;      /* Map extent of the library */
   char security[15];       /* Security classification of the library */
   coverage_type *cover;    /* Coverages within the library */
   int ncover;              /* Number of coverages in the library */
   feature_class_type *fc;  /* Feature classes within the library */
   int nfc;                 /* Number of feature classes in the library */
   long int ntiles;         /* Number of tiles in the library */
   set_type tile_set;       /* Set of 'active' tiles in the library */
   double tileheight;       /* 'Height' of tile 1 in the library */
   VPF_BOOLEAN dec_degrees;     /* Coordinates in decimal degrees? */
   char *prjname;           /* Projection name (if not dec_degrees) */
   units_type units;        /* Units of measure (if not dec_degrees) */
   vpf_version_type version; /* VPF Version of the database */
} library_type;


/* A theme is a single entry for a view of the database.  It can be */
/* thought of as a stored query with a description and symbology.   */
/* Each theme is associated with a feature class.                   */
typedef struct {
   char *description;              /* Description of the theme */
   char *fc;                       /* Feature class name for the theme */
   int  fcnum;                     /* Theme's feature class number */
   int  primclass;                 /* Primitive class of the theme */
   char *text_col;                 /* Column to be displayed as text */
   char *expression;               /* Query expression */
   int  point_color, point_symbol; /* Point color and symbol */
   int  line_color, line_symbol;   /* Line color and symbol */
   int  area_color, area_symbol;   /* Area color and symbol */
   int  text_color, text_symbol;   /* Text color and symbol */
} theme_type;


/* View structure.  Each view is associated with a particular database */
/* and a particular library within that datbase.                       */
typedef struct {
   char database[9];         /* Database name */
   char library[9];          /* Library name */
   char name[9];             /* View name */
   int  nthemes;             /* Number of themes in the view */
   theme_type *theme;        /* Array of themes */
   set_type selected;        /* Set of themes selected for display */
   set_type displayed;       /* Set of displayed themes */
   linked_list_type sellist; /* List of selected themes (ordered) */
} view_type;


/* Map environment information */
typedef struct {
   extent_type mapextent;           /* Current map extent */
   VPF_BOOLEAN     mapchanged;          /* Flag - has anything changed? */
   VPF_BOOLEAN     mapdisplayed;        /* Flag - has the map been displayed? */
   VPF_BOOLEAN     user_escape;         /* Flag - has the user hit escape? */
   VPF_BOOLEAN     study_area_selected; /* Flag - study area selected? */
   VPF_BOOLEAN     latlongrid;          /* Flag - lat-lon grid to be displayed?*/
   VPF_BOOLEAN     scale_bar;           /* Flag - scale bar to be displayed? */
#ifdef __MSDOS__
   window_type scale_bar_window;    /* Window containing the scale bar */
#endif
   int         maptop;              /* Top y location of the map */
   int         mapbottom;           /* Bottom y location of the map */
   int         projection;          /* Current projection number */
#ifdef __MSDOS__
   void far    (*forward_proj)();   /* Forward projection function pointer*/
   void far    (*inverse_proj)();   /* Inverse projection function pointer*/
#endif
} map_environment_type;


/* Output devices */
#define SCREEN     1
#define POSTSCRIPT 2  /* Black and white */
#define COLORPS    3
#define PRINTER    4


/* Functions: */

#include <ossim/vpfutil/vpfmisc.h>

set_type query_table( char *query, vpf_table_type table );
#ifdef __cplusplus
}
#endif

#endif
