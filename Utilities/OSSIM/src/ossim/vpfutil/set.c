/*************************************************************************
 *
 *N  Module SET.C
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This module contains functions that make up an abstract data type
 *     "set".  The data structures and algorithms herein allow programs
 *     to perform basic manipulations defined in the mathematics of set
 *     theory.  These operations are fundamental to relational database
 *     theory, as well.
 *
 *     This version allows sets too large to fit in memory to be swapped
 *     out to disk.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    N/A
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   July 1990                       DOS Turbo C
 *    Nov 1991 - Embedded bit manipulation routines instead of using a
 *               separate module (speed & size).
 *    Nov 1991 - Added disk swapping capabilities.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *    set_type set_init( long int n );
 *    int  set_empty( set_type set );
 *    void set_insert( long int element, set_type set );
 *    void set_delete( long int element, set_type set );
 *    int set_member( long int element, set_type set );
 *    long int set_min( set_type set );
 *    long int set_max( set_type set );
 *    int  num_in_set( set_type set );
 *    void set_on( set_type set );
 *    void set_off( set_type set );
 *    int  set_equal( set_type a, set_type b );
 *    void set_assign( set_type *a, set_type b );
 *    set_type set_union( set_type a, set_type b );
 *    set_type set_intersection( set_type a, set_type b );
 *    set_type set_difference( set_type a, set_type b );
 *    void set_nuke( set_type *set );
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    These functions are all ANSI C compatible.
 *E
 *************************************************************************/
#if defined(__MSDOS__) || defined(__MINGW32__)
#    include <dirent.h>
#    include <malloc.h>
#    include <mem.h>
#    include <dos.h>
#else
#ifndef _WIN32

#include <unistd.h>

#endif

#endif

#include <stdlib.h>
#include <ossim/vpfutil/set.h>

#include <stdio.h>
#include <string.h>
#ifndef TRUE
#define TRUE  1
#endif
#ifndef FALSE
#define FALSE 0
#endif

static char chkmask[] = {254,253,251,247,239,223,191,127};
static char setmask[] = {1,2,4,8,16,32,64,128};

#define BITSET(bit,byte)  ((byte | chkmask[bit]) ^ chkmask[bit])
#define SET_BIT(bit,byte)  (byte | setmask[bit])
#define UNSET_BIT(bit,byte)  (byte ^ setmask[bit])

#define NBYTES(set)  ((set.size>>3L) + 1L)

/* #define BOUNDSCHECK 1 */


static char set_byte( long int nbyte, set_type set )
{
   char byte;

   if ( (nbyte < 0) || (nbyte > NBYTES(set)) ) return 0;

   if (set.diskstorage) {
      fseek(set.fp,nbyte,SEEK_SET);
      fread(&byte,1,1,set.fp);
   } else {
      byte = set.buf[nbyte];
   }
   return byte;
}

#define SET_BYTE( nbyte, set, byte )\
   if ( (nbyte < 0) || (nbyte > NBYTES(set)) ) byte = 0;        \
   if (set.diskstorage) {					\
      fseek(set.fp,nbyte,SEEK_SET);                             \
      fread(&byte,1,1,set.fp);                                  \
   } else {                                                     \
      byte = set.buf[nbyte];                                    \
   }


/*************************************************************************
 *
 *N  set_off
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     Turns each element in the set 'off'.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    set <inout> == (set_type) set to be acted upon.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   Nov 1991                   DOS Turbo C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This function conforms to ANSI C standards.
 *E
 *************************************************************************/
void set_off( set_type set )
{
   register long int nbytes, nbyte, bufsize;
   char byte=0, *buf;

   if (set.diskstorage) {
      rewind(set.fp);
      nbytes = NBYTES(set);
      /* Use buffered reads and writes to set blocks of bits at a time */
      nbyte = 0;
      while (nbyte < nbytes) {
#ifdef __MSDOS__
	 bufsize = (long int)min(nbytes-nbyte,farcoreleft()/2L);
#else
	 bufsize = (long int)(nbytes-nbyte);
#endif
	 buf = (char *)malloc(bufsize);
	 memset(buf,byte,bufsize);
	 fwrite(buf,bufsize,1,set.fp);
	 free(buf);
	 nbyte += bufsize;
      }

      rewind(set.fp);

   } else {
      SET_OFF(set);
   }
}

/*************************************************************************
 *
 *N  set_on
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     Turns each element in the set 'on'.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    set <inout> == (set_type) set to be acted upon.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   Nov 1991                   DOS Turbo C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This function conforms to ANSI C standards.
 *E
 *************************************************************************/
void set_on( set_type set )
{
   register long int nbytes,i, bufsize, nbyte;
#if !defined(__MSDOS__)
   unsigned
#endif
   char byte=255, *buf;

   if (set.diskstorage) {
      rewind(set.fp);
      nbytes = NBYTES(set);
      /* Use buffered reads and writes to set blocks of bits */
      nbyte = 0;
      while (nbyte < nbytes) {
#ifdef __MSDOS__
	 bufsize = (long int)min(nbytes-nbyte,farcoreleft()/2L);
 	buf = (char*)malloc(bufsize);
#else
	 bufsize = (long int)(nbytes-nbyte);
	buf = (unsigned char*)malloc(bufsize);
#endif
	 memset(buf,byte,bufsize);
	 fwrite(buf,bufsize,1,set.fp);
	 free(buf);
	 nbyte += bufsize;
      }

   } else {
      /* Turn on all bits up through set.size. */
      /* All but the last byte. */
      memset(set.buf,byte,set.size>>3L);
      /* The valid bits of the last byte. */
      for (i=(set.size>>3L)*8L;i<=set.size;i++)
	 set_insert(i,set);
   }
}

#if __MSDOS__
static long int available_space( char *drive )
{
   struct dfree disktable;
   int disknum;

   if (!drive) return 0;
   disknum=toupper(drive[0])-'A'+1;
   getdfree(disknum,&disktable);
   return (long int) disktable.df_avail *
	  (long int) disktable.df_sclus *
	  (long int) disktable.df_bsec;
}
#endif

/*************************************************************************
 *
 *N  set_init
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     Initialize the set for 'n' elements.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    n       <input> == (long int) maximum number of elements in the set.
 *    return <output> == (set_type) initialized set.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   July 1990                  DOS Turbo C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This function conforms to ANSI C standards.
 *E
 *************************************************************************/
set_type set_init( long int n )
{
   set_type s;
   long int nbytes;

   s.size = n;
   nbytes = NBYTES(s);
#ifdef __MSDOS__
   if (nbytes < (farcoreleft()/2L)-4096L) {
#endif
      s.buf = (char *)malloc(nbytes);
      s.diskstorage = 0;
#ifdef __MSDOS__
   } else {
      s.diskstorage = 1;
      s.buf = (char *)malloc(20);
      /* If the user has specified a RAM disk, use it (faster) */
      if (getenv("TMP")  &&  nbytes < available_space(getenv("TMP")))
	 sprintf(s.buf,"%s\\%s",getenv("TMP"),tmpnam(NULL));
      else
	 tmpnam(s.buf);
      s.fp = fopen(s.buf,"w+b");
      if (!s.fp) {
	 perror("set_init: ");
	 exit(1);
      }
   }
#endif
   set_off(s);
   return s;
}

/*************************************************************************
 *
 *N  set_empty
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function returns TRUE if the given set is empty; else it
 *     returns FALSE.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    set     <input> == (set_type) set.
 *    return <output> == (int) TRUE[1] or FALSE[0].
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   July 1990                  DOS Turbo C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This function conforms to ANSI C standards.
 *E
 *************************************************************************/
int set_empty( set_type set )
{
   register long int i, nbytes;

   nbytes = NBYTES(set);
   for (i=0;i<nbytes;i++) {
      if (set_byte(i,set)) {
	 return FALSE;
      }
   }
   return TRUE;
}

/*************************************************************************
 *
 *N  set_insert
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function inserts the given element into the specified set.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    element <input> == (long int) element to insert into the set.
 *    set     <inout> == (set_type) set.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   July 1990                  DOS Turbo C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This function conforms to ANSI C standards.
 *E
 *************************************************************************/
void set_insert( long int element,
		 set_type set )
{
   long int nbyte,bit;
   char byte;

   if ((element<0)||(element>set.size)) {
#ifdef BOUNDSCHECK
printf("Invalid call to set_insert! (%ld, %ld)\n",
       element,set.size);
exit(1);
#endif
      return;
   }
   nbyte = element>>3L; /* element/8 */
   bit = element%8L;
   SET_BYTE(nbyte,set,byte);
   byte = SET_BIT(bit,byte);
   if (set.diskstorage) {
      fseek(set.fp,nbyte,SEEK_SET);
      fwrite(&byte,1,1,set.fp);
   } else {
      set.buf[nbyte] = byte;
   }
}

/*************************************************************************
 *
 *N  set_delete
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function deletes the given element from the specified set.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    element <input> == (long int) element to delete from the set.
 *    set     <inout> == (set_type) set.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   July 1990                  DOS Turbo C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *    void unset_bit()    BITSTUFF.C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This function conforms to ANSI C standards.
 *E
 *************************************************************************/
void set_delete( long int element,
		 set_type set )
{
   long int nbyte,bit;
   char byte;

   if ((element<0)||(element>set.size)) {
#ifdef BOUNDSCHECK
printf("Invalid call to set_delete!\n");
exit(1);
#endif
      return;
   }
   nbyte = element>>3L;  /* element/8 */
   bit = element%8L;
   SET_BYTE(nbyte,set,byte);
   if (!BITSET(bit,byte)) return;
   byte = UNSET_BIT(bit,byte);
   if (set.diskstorage) {
      fseek(set.fp,nbyte,SEEK_SET);
      fwrite(&byte,1,1,set.fp);
   } else {
      set.buf[nbyte] = byte;
   }
}

/*************************************************************************
 *
 *N  set_member
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function determines whether a given element is a member of
 *     the specified set.  It returns either TRUE or FALSE.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    element <input> == (long int) element to check in the set.
 *    set     <input> == (set_type) set.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   July 1990                  DOS Turbo C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *    void bitset()    BITSTUFF.C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This function conforms to ANSI C standards.
 *E
 *************************************************************************/
int set_member( long int element,
		set_type set )
{
   long int nbyte,bit;
   char byte;

   if ((element < 0)||(element > set.size)) return FALSE;
   nbyte = element>>3L;  /* element/8L */
   bit = element%8L;
   SET_BYTE(nbyte,set,byte);
   return BITSET(bit,byte);
}

/*************************************************************************
 *
 *N  set_min
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function returns the minimum element in the given set.
 *     If the set is empty, the return value is -1.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    set     <input> == (set_type) set.
 *    return <output> == (long int) minimum element in the set.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   July 1990                  DOS Turbo C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *    void bitset()    BITSTUFF.C
 *    int set_empty( set_type set )     SET.C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This function conforms to ANSI C standards.
 *E
 *************************************************************************/
long int set_min( set_type set )
{
   register long int nbyte, bit, element, nbytes, bufsize, i;
   char byte='\0', *buf, on;

   /* Find the first byte with a bit set */
   nbytes = NBYTES(set);

   if (set.diskstorage) {

      /* Set is stored on disk, use block reads to get set into */
      /* memory one chunk at a time */
      nbyte = 0;
      on = 0;
      rewind(set.fp);
      while (nbyte < nbytes) {
#ifdef __MSDOS__
	 bufsize = (long int)min(nbytes-nbyte,farcoreleft()/2L);
#else
	 bufsize = (long int)(nbytes-nbyte);
#endif
	 buf = (char *)malloc(bufsize);
	 fread(buf,bufsize,1,set.fp);
	 for (i=0;i<bufsize;i++,nbyte++) {
	    if (buf[i]) {
	       on = 1;
	       byte = buf[i];
	       break;
	    }
	 }
	 free(buf);
	 if (on) break;
      }

   } else {
      for (nbyte=0;nbyte<nbytes;nbyte++)
	 if (set.buf[nbyte]) {
	    byte = set.buf[nbyte];
	    break;
	 }
   }

   /* Now find the first bit set in the byte */
   element = nbyte*8L;
   for (bit=0; bit<8; bit++,element++) {
      if (element > set.size) return -1;
      if (BITSET(bit,byte)) return element;
   }
   return -1;
}

/*************************************************************************
 *
 *N  set_max
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function returns the maximum element in the given set.
 *     If the set is empty, the return value is 0.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    set     <input> == (set_type) set.
 *    return <output> == (long int) maximum element in the set.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   July 1990                  DOS Turbo C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This function conforms to ANSI C standards.
 *E
 *************************************************************************/
long int set_max( set_type set )
{
   register long int nbyte, bit, element, nbytes, bufsize, i;
   char byte='\0', *buf=0, on='\0';

   /* Find the last byte with a bit set */
   nbytes = NBYTES(set);

   if (set.diskstorage) {

      /* Set is stored on disk, use block reads to get set into */
      /* memory one chunk at a time */
      nbyte = nbytes-1L;
      on = 0;
      while (nbyte > 0) {
#ifdef __MSDOS__
	 bufsize = (long int)min(nbyte+1L,farcoreleft()/2L);
#else
	 bufsize = (long int)(nbyte+1L);
#endif
	 buf = (char *)malloc(bufsize);
	 fseek(set.fp,nbyte-bufsize+1L,SEEK_SET);
	 fread(buf,bufsize,1,set.fp);
	 for (i=bufsize-1L;i>0;i--,nbyte--) {
	    if (buf[i]) {
	       on = 1;
	       byte = buf[i];
	       break;
	    }
	 }
	 free(buf);
	 if (on) break;
      }
   } else {

      for (nbyte = nbytes-1; nbyte >= 0; nbyte--) {
	 if (set.buf[nbyte]) {
	    byte = set.buf[nbyte];
	    break;
	 }
      }
      if (nbyte < 0) return 0L;

   }

   /* Now find last bit set in the byte */
   element = nbyte*8L + 7L;
   for (bit=7; bit >= 0; bit--,element--) {
      if (BITSET(bit,byte)) {
	 return element;
      }
   }

   return 0L;
}

/*************************************************************************
 *
 *N  num_in_set
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function returns the number of elements in the given set.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    set     <input> == (set_type) set.
 *    return <output> == (long int) number of elements in the set.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   July 1990                  DOS Turbo C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *    void bitset()    BITSTUFF.C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This function conforms to ANSI C standards.
 *E
 *************************************************************************/
long int num_in_set( set_type set )
{
   register long int nbyte,bit,n=0, nbytes;
   char byte;

   nbytes = NBYTES(set);
   for (nbyte=0;nbyte<nbytes;nbyte++) {
      byte = set_byte(nbyte,set);
      if (byte) {
	 for (bit=0;bit<8;bit++)
	    if (BITSET(bit,byte)) n++;
      }
   }
   return n;
}

#if 0
/*************************************************************************
 *
 *N  set_equal
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function determines whether two sets are equal to each other.
 *     It returns TRUE or FALSE.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    a       <input> == (set_type) first set to compare.
 *    b       <input> == (set_type) second set to compare.
 *    return <output> == (int) TRUE if (a==b) or FALSE if (a!=b).
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   July 1990                  DOS Turbo C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This function conforms to ANSI C standards.
 *E
 *************************************************************************/
int  set_equal( set_type a,
		set_type b )
{
   long int i, nbytes;

   if (a.size != b.size) return FALSE;
   if ((!a.diskstorage)&&(!b.diskstorage)) {
      if (memcmp(a.buf,b.buf,NBYTES(a))==0)
	 return TRUE;
      else
	 return FALSE;
   } else {
      nbytes = NBYTES(a);
      for (i=0;i<nbytes;i++) {
	 if (set_byte(i,a) != set_byte(i,b)) return FALSE;
      }
      return TRUE;
   }
}
#endif

/*************************************************************************
 *
 *N  set_assign
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function assigns set a to be equal to set b.  If a and b are
 *     different sizes, the function will reallocate a to match b.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    a       <input> == (set_type *) set to be assigned.
 *    b       <input> == (set_type) set to assign to a.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   July 1990                  DOS Turbo C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This function conforms to ANSI C standards.
 *E
 *************************************************************************/
void set_assign( set_type *a,
		 set_type b )
{
   register long int nbytes, nbyte, bufsize;
   char *buf;

   nbytes = NBYTES(b);

   if (!a->diskstorage && !b.diskstorage) {
      if (a->size == b.size) {
	 memcpy(a->buf,b.buf,nbytes);
      } else {    /* a and b are different sizes */
	 a->buf = (char *)realloc(a->buf,nbytes);
	 if (a->buf==NULL) {
	    a->diskstorage = 1;
	    a->fp = tmpfile();
	    fwrite(b.buf,1,nbytes,a->fp);
	 } else {
	    memcpy(a->buf,b.buf,nbytes);
	 }
	 a->size = b.size;
      }
   } else {
      /* Either a or b or both are stored on disk */
      if (a->diskstorage && !b.diskstorage) {
	 rewind(a->fp);
	 fwrite(b.buf,1,nbytes,a->fp);
	 a->size = b.size;
      } else {
	 /* b.diskstorage must be true to get here */
	 if (!a->diskstorage) {
	    /* Force a->diskstorage */
	    a->diskstorage = 1;
	    if (a->size > 0) free(a->buf);
	    a->fp = tmpfile();
	 }

	 /* Block copy file from b.fp to a->fp */
	 nbyte = 0;
	 rewind(b.fp);
	 rewind(a->fp);
	 while (nbyte < nbytes) {
#ifdef __MSDOS__
	    bufsize = (long int)min(nbytes-nbyte,farcoreleft()/2L);
#else
	    bufsize = (long int)(nbytes-nbyte);
#endif
	    buf = (char *)malloc(bufsize);
	    fread(buf,bufsize,1,b.fp);
	    fwrite(buf,bufsize,1,a->fp);
	    free(buf);
	    nbyte += bufsize;
	 }

	 a->size = b.size;
      }
   }
}

#if 0
/*************************************************************************
 *
 *N  set_union
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     Return the set C such that C = (A U B).  C is initialized within
 *     this function, and should be nuked when no longer needed.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    a       <input> == (set_type) set to be unioned.
 *    b       <input> == (set_type) set to be unioned.
 *    return <output> == (set_type) (A U B).
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   July 1990                  DOS Turbo C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *    set_type set_init()   SET.C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This function conforms to ANSI C standards.
 *E
 *************************************************************************/
set_type set_union( set_type a,
		    set_type b )
{
   register long int i, nbytes;
   set_type c;
   char byte;

   c = set_init( (long int)max(a.size,b.size) );

   nbytes = NBYTES(c);

   for (i=0;i<nbytes;i++) {

      byte = set_byte(i,a) | set_byte(i,b);

      if (c.diskstorage) {
	 if (byte) {
	    fseek(c.fp,i,SEEK_SET);
	    fwrite(&byte,1,1,c.fp);
	 }
      } else {
	 c.buf[i] = byte;
      }

   }

   return c;
}
#endif


/*************************************************************************
 *
 *N  set_intersection
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     Return the set C such that C = (A o B).  C is initialized within
 *     this function, and should be nuked when no longer needed.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    a       <input> == (set_type) set to be intersectioned.
 *    b       <input> == (set_type) set to be intersectioned.
 *    return <output> == (set_type) (A o B).
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   July 1990                  DOS Turbo C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *    set_type set_init()   SET.C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This function conforms to ANSI C standards.
 *E
 *************************************************************************/
set_type set_intersection( set_type a,
			   set_type b )
{
   register long int i, nbytes;
   set_type c;
   char byte;
#ifdef __MSDOS__
   c = set_init( (long int)max(a.size,b.size) );
#else
   c = set_init( a.size > b.size ? a.size : b.size );
#endif

   if (c.diskstorage) rewind(c.fp);

   nbytes = NBYTES(c);
   for (i=0;i<nbytes;i++) {
      byte = set_byte(i,a) & set_byte(i,b);
      if (c.diskstorage)
	 fwrite(&byte,1,1,c.fp);
      else
	 c.buf[i] = byte;
   }

   return c;
}

#if 0
/*************************************************************************
 *
 *N  set_difference
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     Return the set C such that C = (A - B).  C is initialized within
 *     this function, and should be nuked when no longer needed.
 *
 *     NOTE:  This function can be sped up, if necessary, by direct
 *     manipulation of the bytes and bits rather than the abstract
 *     set function calls used presently.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    a       <input> == (set_type) set to subtract from.
 *    b       <input> == (set_type) set to be subtracted.
 *    return <output> == (set_type) (A - B).
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   July 1990                  DOS Turbo C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *    set_type set_init()   SET.C
 *    int set_member()      SET.C
 *    void set_insert()     SET.C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This function conforms to ANSI C standards.
 *E
 *************************************************************************/
set_type set_difference( set_type a,
			 set_type b )
{
   register long int i;
   set_type c;

   c = set_init( a.size );

   for (i=0;i<=a.size;i++) {
      if ( i > b.size ) {
	 if (set_member(i,a)) set_insert( i, c );
      } else {
	 if ((set_member(i,a)) && (!set_member(i,b))) set_insert(i,c);
      }
   }

   return c;
}
#endif

/*************************************************************************
 *
 *N  set_nuke
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     Nucleate a set from existence.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    set     <inout> == (set_type *) set to be nuked.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   July 1990                  DOS Turbo C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This function conforms to ANSI C standards.
 *E
 *************************************************************************/
void set_nuke( set_type *set )
{
   if (set->diskstorage) {
      fclose(set->fp);
      unlink(set->buf);
   }
   free( set->buf );
   set->size = -1;
   set->diskstorage = 0;
}
