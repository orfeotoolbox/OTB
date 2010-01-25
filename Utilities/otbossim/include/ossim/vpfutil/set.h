#if !defined(__SET__)

#define __SET__

/* SET.H */

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif
/* A set is represented as an array of characters with each character */
/* holding 8 bits of the set. */
typedef struct {
   char *buf;
   long int size;
   char diskstorage;
   FILE *fp;
} set_type;


/* Functions: */

set_type set_init( long int n );

int  set_empty( set_type set );

void set_insert( long int element,
		 set_type set );

void set_delete( long int element,
		 set_type set );

int set_member( long int element,
		set_type set );

long int set_min( set_type set );

long int set_max( set_type set );

long int  num_in_set( set_type set );

/* SET_ON and SET_OFF are only valid if the set is in memory */
#define SET_ON(set) memset(set.buf,255,(set.size>>3L)+1L)
#define SET_OFF(set) memset(set.buf,0,(set.size>>3L)+1L)

/* set_on and set_off are valid for all sets */
void set_on( set_type set );

void set_off( set_type set );

int  set_equal( set_type a,
		set_type b );

void set_assign( set_type *a,
		 set_type b );

set_type set_union( set_type a,
		    set_type b );

set_type set_intersection( set_type a,
			   set_type b );

set_type set_difference( set_type a,
			 set_type b );

void set_nuke( set_type *set );

#ifdef __cplusplus
}
#endif

#endif
