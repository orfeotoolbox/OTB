#ifndef _VPFMISC_
#define _VPFMISC_
#ifdef __cplusplus
extern "C" {
#endif

#define graphic_mode()\
	( (getgraphmode() < 0) ? 0 : 1)

#include <ossim/vpfutil/vpfview.h>

void *vpfmalloc( unsigned long size );

void vpffree( void *ptr );
void displaymessage(char*, ...);

#ifdef __MSDOS__
void get_display_position( int *x, int *y, window_type window );
#endif

void displayinfo( char *text[],
		  int  nlines );

void no_study_area(void);

void no_map_displayed(void);

void display_message(char *str);

dms_type float_to_dms( double coord );

double dms_to_float( dms_type coord );

char *dms_string( dms_type coord, int seconds );

void out_dms( dms_type coord, int x, int y, int dir, int seconds );

int strpos( char *str, char ch );

VPF_BOOLEAN fwithin( float x,
	     float y,
	     extent_type extent );

VPF_BOOLEAN contained( extent_type extent1,
	       extent_type extent2 );

void memleft(void);

#ifdef __MSDOS__
window_type info_window( char *text );
#endif

#if !defined(__MSDOS__) && !defined(_MSC_VER) && !defined(__BORLANDC__)
#  ifndef _MSC_VER
char* strupr(char* str);
#  endif
#endif

float distance( double lat1, double lon1, double lat2, double lon2,
		int units );

char *rightjust( char *str );

char *leftjust( char *str );

VPF_BOOLEAN displayerror( char *text[],
		      int  nlines );

int printer_ready(void);

int printer_ok(void);

int is_primitive( char *name );

int is_simple_feature( char *name );

int is_complex_feature( char *name );

int is_feature( char *name );

int feature_type( char *name );

int is_join( char *name );

int primitive_class( char *name );

/**
 * Written to replace non-portable "strcasecmp".
 *
 * @return An integer -1, 0, or 1 if s1 is found, respectively, to be less
 * than, to match, or be greater than s2.
 */
int ossim_strcasecmp(const char *s1, const char *s2);

/**
 * Written to replace non-portable "strncasecmp".  Similar to
 * ossim_strcasecmp,  except  it  only  compares  the first n characters.
 *
 * @return An integer -1, 0, or 1 if s1 is found, respectively, to be less
 * than, to match, or be greater than s2.
 */
int ossim_strncasecmp(const char *s1, const char *s2, unsigned int n);

#ifdef __cplusplus
}
#endif

#endif
