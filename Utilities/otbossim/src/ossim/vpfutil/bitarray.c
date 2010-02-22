/******************************************************************************
 *
 *N  Module BITARRAY.C
 *
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    Bit manipulation of virtual 2-dimensional arrays.  These arrays are
 *    stored in a 1-dimensional bit array (MAP).
 *    Some notes on problems with this code:
 *     1. all bit mapping must be with your mapextents-2.  ie, if you
 *        declare a bitarray 200 by 200, only 198 by 198 is usable.
 *     2. when drawing/manipulating lines, you must use a delta of 0 for
 *        the x or y.  If you do not, the line will not be straight.  In
 *        other words, use coordinate geometry.
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    N/A
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Thomas Wood  October, 1990, Kernighan/Richie C
 *    Updated August 1991, ANSI C
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions:
 *F
 *    void SETROW( ArrayOfBits MAP, int XMIN, int XMAX, int Y );
 *    void CLEARROW( ArrayOfBits MAP, int XMIN, int XMAX, int Y );
 *    void TOGGLEROW( ArrayOfBits MAP, int XMIN, int XMAX, int Y );
 *    void SETCOLUMN( ArrayOfBits MAP, int X, in YMIN, int YMAX );
 *    void CLEARCOLUMN( ArrayOfBits MAP, int X, int YMIN, int YMAX );
 *    void TOGGLECOLUMN(ArrayOfBits MAP, int X, int YMIN, int YMAX );
 *    ArrayOfBits MakeArrayOfBits( int deltaX, int deltaY );
 *    int FreeArrayOfBits( ArrayOfBits map );
 *    int PointGetBit( ArrayOfBits map, IPoint point );
 *    int PointSetBit( ArrayOfBits map, IPoint at, int value );
 *    int LineSetBit( ArrayOfBits map, ISegment seg, int value );
 *    int LineXorBit( ArrayOfBits map, ISegment seg, int value );
 *    int TrapezoidXorBit( ArrayOfBits map, Isegment seg, int value );
 *****************************************************************************/

#ifndef __MSDOS__
#ifndef _WIN32
#include <unistd.h>
#endif
#endif
#include <ossim/vpfutil/xmemory.h>
#include <ossim/vpfutil/polygrf.h>
#include <stdlib.h>


/******************************************************************************
 *
 *N  SETROW
 *
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    Set bits in row Y of 2-d array MAP from XMIN to XMAX.
 *    This procedure sets bits in MAP based on XMIN, XMAX, and Y.  If XMIN and
 *    XMAX are equal, it is assumed that there is a 1bit array in MAP, and this
 *    bit is set to ~(255>>(8-(XMIN MOD 8))) && ~(255<<(1+(XMAX MOD 8))).
 *    The index is calculated by: Y*MAP.rowcount+ (XMAX >> 3) or
 *    Y*MAP.rowcount +(XMIN >> 3).  Note that >> is rightshift the bits,
 *    no carry.
 *    If XMAX and XMIN are different values, a for loop is used to go from
 *    the above index values, moving a string of 1's into each location,
 *    except for the first location, which gets ~(255>>8-(XMIN MOD 8)) and the
 *    last location, which gets ~(255<<1+(XMAX MOD 8)).  As the name suggests
 *    a row is set referenced by y.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    MAP <in/out>  == (ArrayOfBits) 2-d array to be modified
 *    XMIN <input>  == (int) bottom of refence
 *    XMAX <input>  == (int) top of reference
 *    Y    <input>  == (int) row to be modified
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Thomas Wood  October 1990, Kernighan&Ritchie C
 *    Updated August 1991, ANSI C
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Macros Called:
 *M
 *    LEFTBITS(INDEX)   BITARRAY.C
 *    RIGHTBITS(INDEX)  BITARRAY.C
 *E
 *****************************************************************************/

void  SETROW( ArrayOfBits MAP,
             int         XMIN,
             int         XMAX,
             int         Y )
{
  register int start /*= (((Y)*(MAP.rowcount))+((XMIN)>>3))*/;
  register int end   /*= (((Y)*(MAP.rowcount))+((XMAX)>>3))*/;
  register int sbit  /*= ((XMIN)%8)*/;
  register int ebit  /*= ((XMAX)%8)*/;

  if (XMIN < 0) XMIN = 0;
  start = (((Y)*(MAP.rowcount))+((XMIN)>>3));
  if (XMAX > MAP.bounding.x) XMAX = MAP.bounding.x-1;
  end = (((Y)*(MAP.rowcount))+((XMAX)>>3));
  sbit = ((XMIN) % 8);
  ebit = ((XMAX) % 8);
  if ((0 > Y) || (Y > MAP.bounding.y) || (MAP.array == NULL) || (XMAX < XMIN)) return;

  if (start == end)
    MAP.array[start] |= ((RIGHTBITS(sbit))&LEFTBITS(ebit));
  else
    {
      MAP.array[start] |= (RIGHTBITS(sbit));
      for(start++;start<end;start++)
	MAP.array[start] |= LEFTBITS(7);
      MAP.array[start] |= LEFTBITS(ebit);
    }
}

/*****************************************************************************
 *
 *N  CLEARROW
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    Erases bits in row Y of 2-d array MAP from XMIN to XMAX.
 *    This procedure clears bits in MAP based on XMIN, XMAX, and Y.  If XMIN
 *    and XMAX are equal, it is assumed that there is a 1 bit array in MAP,
 *    and this bit is set to:
 *    ~(~(255>>(8-(XMIN MOD 8))) && ~(255<<(1+(XMAX MOD 8)))).  The index is
 *    calculated as in the previous procedure.
 *    If XMAX and XMIN are different values, a for loop is used to go from
 *    the above index values, moving a string of 0's into each location.  The
 *    first location gets ~(~(255>>(8-(XMIN MOD 8)))) and the last location
 *    gets ~(~(255<<1+(XMAX MOD 8))).  As the name suggest, a row is erased
 *    referenced by y.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    MAP <in/out>  == (ArrayOfBits) 2-d array of bits to be modified
 *    XMIN <input>  == (int) bottom of reference
 *    XMAX <input>  == (int) top of reference
 *    Y    <input>  == (int) row to be modified in the 2 d array
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Thomas Wood  October 1990, Kernighan&Ritchie C
 *    Updated August 1991, ANSI C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Macros Called:
 *M
 *    LEFTBITS(INDEX)   BITARRAY.C
 *    RIGHTBITS(INDEX)  BITARRAY.C
 *E
 ****************************************************************************/
void  CLEARROW( ArrayOfBits MAP,
               int         XMIN,
               int  	   XMAX,
	       int 	   Y )
{
   register int start /*= (((Y)*(MAP.rowcount))+((XMIN)>>3))*/;
   register int end   /*= (((Y)*(MAP.rowcount))+((XMAX)>>3))*/;
   register int sbit  /*= ((XMIN)%8)*/;
   register int ebit  /*= ((XMAX)%8)*/;

   if (XMIN < 0) XMIN=0;
   start = (((Y)*(MAP.rowcount))+((XMIN)>>3));
   if (XMAX > MAP.bounding.x) XMAX = MAP.bounding.x-1;
   end = (((Y)*(MAP.rowcount))+((XMAX)>>3));
   sbit = ((XMIN)%8);
   ebit = ((XMAX)%8);
   if ((0 > Y) || (Y > MAP.bounding.y) || (MAP.array == NULL) || (XMAX < XMIN)) return;

   if (start == end)
      MAP.array[start] &= ~((RIGHTBITS(sbit))&LEFTBITS(ebit));
   else
   {
      MAP.array[start] &= ~(RIGHTBITS(sbit));
      for(start++;start<end;start++)
      {
         //---
         // Fixes:  warning: overflow in implicit constant conversion (drb)
         // ~LEFTBITS(7) == 0
         //---
         // MAP.array[start] &= ~LEFTBITS(7);
         MAP.array[start] &= 0;
      }
      MAP.array[start] &= ~LEFTBITS(ebit);
   }
}

/*****************************************************************************
 *
 *N  TOGGLEROW
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose
 *P
 *    This procedure is used for the exclusive or-ing (XOR) of the input MAP
 *    vs the mask of 1's.   Specifically, row Y is modified from bits XMIN to
 *    YMAX.
 *    The index is created as in the previous 2 procedures.  The mask is MAP
 *    XOR a string of 1's.
 *    Essentially, this turns on every bit that was turned off, and turns off
 *    every bit that was turned on.  As the name suggests, the index is a row
 *    referenced by y.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    MAP <in/out>  == (ArrayOfBits) 2 dimensional array of bits to be modified
 *    XMIN <input>  == (int) low bit to begin modification
 *    XMAX <input>  == (int) high bit to end modification
 *    Y    <input>  == (int) row to be modified in the 2-d array
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Thomas Wood  October, 1990, Kernighan&Ritchie C
 *    Updated August 1991, ANSI C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Macros Called:
 *M
 *    LEFTBITS(INDEX)  BITARRAY.C
 *    RIGHTBITS(INDEX) BITARRAY.C
 *E
 ****************************************************************************/
void TOGGLEROW( ArrayOfBits MAP,
	       int         XMIN,
	       int         XMAX,
	       int         Y )
{
  register int start /*= (((Y)*(MAP.rowcount))+((XMIN)>>3))*/;
  register int end   /*= (((Y)*(MAP.rowcount))+((XMAX)>>3))*/;
  register int sbit  /*= ((XMIN)%8)*/;
  register int ebit  /*= ((XMAX)%8)*/;

  if (XMIN <= 0) XMIN = 0;
  start = (((Y)*(MAP.rowcount))+((XMIN)>>3));
  if (XMAX > MAP.bounding.x) XMAX = MAP.bounding.x-1;
  end = (((Y)*(MAP.rowcount))+((XMAX)>>3));
  sbit = ((XMIN)%8);
  ebit = ((XMAX)%8);
  if ((0 > Y) || (Y > MAP.bounding.y) || (MAP.array == NULL) || (XMAX < XMIN)) return;

  if (start == end)
    MAP.array[start] ^= ((RIGHTBITS(sbit))&LEFTBITS(ebit));
  else                                                      
    {                                             
      MAP.array[start] ^= (RIGHTBITS(sbit));   
      for(start++;start<end;start++)              
	MAP.array[start] ^= LEFTBITS(7);
      MAP.array[start] ^= LEFTBITS(ebit);         
    }                                             
}

/*****************************************************************************
 *
 *N  SETCOLUMN
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    Sets bits in a column from MAP referenced by X, from bits YMIN to YMAX.
 *    More detailed:
 *    A mask is ORed with each bit from YMIN to YMAX, using the same indexing
 *    calculations as above.  The mask is generated as: (1<(X MOD 8)).
 *    As the name suggests, the index is a column referenced by X.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    MAP  <inout> == (ArrayOfBits) two dimensional array of bits to be modified
 *    X    <input> == (int) column to be modified in the 2-d array
 *    YMIN <input> == (int) low bit to begin modification
 *    YMAX <input> == (int) high bit to end modification
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *   History:
 *H
 *    Thomas Wood  October 1990, Original Version Kernghan&Ritchie C
 *    Updated August 1991, ANSI C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *    None
 *E
 ****************************************************************************/
void SETCOLUMN( ArrayOfBits MAP,
	       int         X,
	       int         YMIN,
	       int 	   YMAX )
{
  register int yymax  /*= YMAX*/;
  register int yymin  /*= YMIN*/;
  register int start  /*= (((yymin)*(MAP.rowcount))+((X)>>3))*/;
  register int mask   = (1 << ((X) % 8));

  if (YMIN < 0) YMIN = 0;
  yymin = YMIN;
  start = (((yymin)*(MAP.rowcount))+((X)>>3));
  if (YMAX > MAP.bounding.y) YMAX = MAP.bounding.y;
  yymax = YMAX;
  if ((0 > X) || (X > MAP.bounding.x) || (MAP.array == NULL) || (YMAX < YMIN)) return;

  MAP.array[start] |= mask;
  start += MAP.rowcount;                          
                                                  
  for(;yymin<yymax; yymin++,start += MAP.rowcount)             
    MAP.array[start] |= mask;
  /* this for statement executes YMAX-YMIN times (thats how many bits there
     are).  The index for the 2-d array is MAP.rowcount * 1,2,3,... YMAX-YMIN.
     This is only a virtual 2-d array, it is actually a 1-d array        */
}

/*****************************************************************************
 *
 *N  CLEARCOLUMN
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    Ands a mask with the column referenced by x, between bit YMIN and YMAX.
 *    The mask is ~(1<<(X MOD 8)).  Basically, this erases whatever was there.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    MAP  <inout> == (ArrayOfBits) 2-d array to be modified
 *    X    <input> == (int) column to be modified
 *    YMIN <input> == (int) starting bit
 *    YMAX <input> == (int) ending bit
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Thomas Wood  October 1990  Original Version Kernighan&Ritchie C
 *    Updated August 1991, ANSI C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions/Macros Called:
 *FM
 *    None
 *E
 ****************************************************************************/
void CLEARCOLUMN( ArrayOfBits MAP,
		 int 	     X,
		 int 	     YMIN,
		 int 	     YMAX ) 
{                                                 
  register int yymax /*= YMAX*/;
  register int yymin /*= YMIN*/;
  register int start /*= (((yymin)*(MAP.rowcount))+((X)>>3))*/;
  register int mask  = ~(1<<((X)%8));

  if (YMIN < 0) YMIN = 0;
  yymin = YMIN;
  start = (((yymin) * (MAP.rowcount)) + ((X) >> 3));
  if (YMAX > MAP.bounding.y) YMAX = MAP.bounding.y;
  yymax = YMAX;
  if ((0 > X) || (X > MAP.bounding.x) || (MAP.array == NULL) || (YMAX < YMIN)) return;

  MAP.array[start] &= mask;
  start += MAP.rowcount;                          
                                                  
  for(;yymin<yymax; yymin++,start += MAP.rowcount)             
    MAP.array[start] &= mask;                     
}

/******************************************************************************
 *
 *N  TOGGLECOLUMN
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    MAP  <inout> == (ArrayOfBits) 2-d array to be modified
 *    X    <input> == (int) column to be modified
 *    YMIN <input> == (int) starting bit
 *    YMAX <input> == (int) ending bit
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    XORs bits in column X of 2-d array MAP from bits YMIN to YMAX.
 *    Mask is 1's, index is YMIN through YMAX.
 *E
 *****************************************************************************/
void TOGGLECOLUMN( ArrayOfBits MAP,
		  int 	      X,
		  int 	      YMIN,
		  int 	      YMAX )
{
  register int yymax /*= YMAX*/;
  register int yymin /*= YMIN*/;
  register int start /*= (((yymin)*(MAP.rowcount))+((X)>>3))*/;
  register int mask  = (1<<((X)%8));

  if (YMIN < 0) YMIN = 1;
  yymin = YMIN;
  start = (((yymin) * (MAP.rowcount)) + ((X) >> 3));
  if (YMAX > MAP.bounding.y) YMAX = MAP.bounding.y;
  yymax = YMAX;
  if ((0 > X) || (X > MAP.bounding.x) || (MAP.array == NULL) || (YMAX < YMIN)) return;

  MAP.array[start] ^= mask;
  start += MAP.rowcount;
                                                  
  for(;yymin<yymax; yymin++,start += MAP.rowcount)
    MAP.array[start] ^= mask;                     
}

/******************************************************************************
 *
 *N  MakeArrayOfBits
 *
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    Defines a MAP based on deltaX and deltaY.  The rowcount is:
 *    (deltaX+8) >> 3.  The array allocation is:
 *    (unsigned char *) calloc((deltaY+1), rowcount).
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    deltaX          <input>  == (int) x width of 2-d bit array
 *    deltaY          <input>  == (int) y width of 2-d bit array
 *    MakeArrayOfBits <output> == (ArrayOfBits) empty 2-d bit array
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Thomas Wood  October 1990  Original Version Kernighan&Ritchie C
 *    Updated August 1991, ANSI C
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions/Macros Called:
 *FM
 *    Xmark()     XMEMORY.H
 *    Xclear()    XMEMORY.H
 *    Xcalloc(nelem, elsize)
 *E
 *****************************************************************************/
ArrayOfBits MakeArrayOfBits( int deltaX,
			     int deltaY )
{
  ArrayOfBits value;
  int temp;

  temp = deltaX + 1;
  temp += 7;

  value.rowcount = temp >> 3;

  value.bounding.x = deltaX;
  value.bounding.y = deltaY;

  Xmark();

  value.array = (unsigned char *)Xcalloc( (deltaY + 1), value.rowcount);

  Xclear();

  return value;
}

/******************************************************************************
 *
 *N  FreeArrayOfBits
 *
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    Erases a map, then returns 0.
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    map             <input>  == (ArrayOfBits) map to be erased
 *    FreeArrayOfBits <output> == (int) value of map
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Thomas Wood  October 1990, Original Version Kernighan&Ritchie C
 *    Updated August 1991, ANSI C
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Macros Called:
 *FM
 *    Xfree
 *E
 *****************************************************************************/
int FreeArrayOfBits ( ArrayOfBits map )
{
  Xfree(map.array);
  return 0;
}


/******************************************************************************
 *
 *N  PointGetBit
 *
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    Tells the user whether the bit at point(x,y) is set to 0 or 1.(error
 *    checking: will return 0 if the point sent is outside of the range of
 *    the map).
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    map         <input>  == (ArrayOfBits) 2-d array to be checked
 *    point       <input>  == (IPoint) coordinated to check
 *    PointGetBit <output> == (int) results of check
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Thomas Wood  October 1990,  Original Version Kernighan&Ritchie C
 *    Updated August 1991, ANSI C
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions/Modules Called:
 *FM
 *    OUTISDE(MAP,POINT)    BITARRAY.C
 *    GETBIT(MAP,POINT)     BITARRAY.C
 *E
 *****************************************************************************/
int PointGetBit ( ArrayOfBits map,
		  IPoint      point )
{
  if (map.array == NULL) return 0;
  if (OUTSIDE(map,point)) return 0;
  return GETBIT(map,point);
}

/*****************************************************************************
 *
 *N  PointSetBit
 *
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    Sets a bit to value at point at in map.  value must be 1 or 0
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    map         <input>  == (ArrayOfBits) 2-d array to be modified
 *    at          <input>  == (IPoint) coordinates to set set
 *    value       <input>  == (int) value to set coordinates to
 *    PointSetBit <output> == (int) value that coordinates were set to
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Thomas Wood  October 1990, Original Version Kernighan&Ritchie C
 *    Updated August 1991, ANSI C
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions/Modules Called:
 *FM
 *    OUTSIDE(MAP,POINT)     BITARRAY.C
 *    SETBIT(MAP,POINT)      BITARRAY.C
 *    CLEARBIT(MAP,POINT)    BITARRAY.C
 *E
 *****************************************************************************/
int PointSetBit ( ArrayOfBits map,
		  IPoint      at,
		  int         value)
{
  if (map.array == NULL) return 0;
  if (OUTSIDE(map,at)) return 0;
  if (value)
    return SETBIT(map,at);
  return CLEARBIT(map,at);
}


/******************************************************************************
 *
 *N  LineSetBit
 *
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    Sets bit in a line with coordinates from set in map.  Lines can be
 *    sloping, vertical, or horizontal, but the beginning coordinates
 *    must be less than the ending coordinates, and all coordinates should
 *    be within map (there is error checking for this, basically nothing is
 *    done).  Sets the bits for value=1, clears the bits for value=0.
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    map        <inout>  == (ArrayOfBits) 2-d array to be modified
 *    seg        <input>  == (ISegment) line coordinates(to,from) for the line
 *    value      <input>  == (int) 0 to clear bits, 1 to set bits
 *    LineSetBit <output> == (int) value that bits were set to
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Thomas Wood  October 1990, Original Version Kernighan&Ritchie C
 *    Updated August 1991, ANSI C
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions/Macros Called:
 *FM
 *    OUTSIDE(MAP,POINT)        BITARRAY.C
 *    SETBIT(MAP,POINT)         BITARRAY.C
 *    CLEARBIT(MAP,POINT)       BITARRAY.C
 *    MAXIMUM(X,Y)	        BITARRAY.C
 *    MINIMUM(X,Y)		BITARRAY.C
 *    OUTSIDE_X(MAP,Value)      BITARRAY.C
 *    OUTSIDE_Y(MAP,Value)      BITARRAY.C
 *    FILLBRESSEG(bseg,seg)	BITARRAY.C
 *    STEP(bseg)    		BITARRAY.C
 *    SETROW(ArrayOfBits MAP, int XMIN, int XMAX, int Y )   BITARRAY.C
 *    CLEARROW(ArrayOfBits MAP, int XMIN, int XMAX, int Y ) BITARRAY.C
 *    SETCOLUMN(ArrayOfBits MAP, int X, int YMIN, int YMAX ) BITARRAY.C
 *    CLEARCOLUMN(ArrayOfBits MAP, int X, int YMIN, int YMAX ) BITARRAY.C
 *E
 ******************************************************************************/
int LineSetBit ( ArrayOfBits map,
		 ISegment    seg,
		 int         value )
{
  BresSegment bseg;
  unsigned int from,to;

  if (map.array == NULL) return 0;
  if ((!seg.delta.x)&&(!seg.delta.y))
    {
      if (OUTSIDE(map,seg.origin)) return 0;
      if (value) SETBIT(map,seg.origin); else CLEARBIT(map,seg.origin);
      return 0;
    }
  /* horizontal line */
  if (seg.delta.y == 0)
    {
      if (OUTSIDE_Y(map,seg.origin.y)) return 0;
      if (seg.delta.x < 0)
	{
	  from = MAXIMUM(seg.origin.x + seg.delta.x, 0);
	  to   = MINIMUM(seg.origin.x, map.bounding.x);
	}
      else
	{
	  from = MAXIMUM(seg.origin.x, 0);
	  to   = MINIMUM(seg.origin.x + seg.delta.x, map.bounding.x);
	}
      if (from > to) return 0;
      if (from == to)
      {
	if (value) { seg.origin.x = from; SETBIT(map,seg.origin); }
	else     { seg.origin.x = from; CLEARBIT(map,seg.origin); }
      }
      if (value) 
      { 
         SETROW(map,from,to,seg.origin.y); 
      }
      else     
      { 
         CLEARROW(map,from,to,seg.origin.y); 
      }
      return 0;
    }
  /* vertical line */
  if (seg.delta.x == 0)
    {
      if (OUTSIDE_X(map,seg.origin.x)) return 0;
      if (seg.delta.y < 0)
	{
	  from = MAXIMUM(seg.origin.y + seg.delta.y ,0);
	  to   = MINIMUM(seg.origin.y, map.bounding.y);
	}
      else
	{
	  from = MAXIMUM(seg.origin.y, 0);
	  to   = MINIMUM(seg.origin.y + seg.delta.y, map.bounding.y);
	}
      if (from > to) return 0;
      if (from == to)
      {
         if (value) { seg.origin.y = from; SETBIT(map,seg.origin); }
         else     { seg.origin.y = from; CLEARBIT(map,seg.origin); }
      }
      if (value) { SETCOLUMN(map,seg.origin.x,from,to); }
      else    {  CLEARCOLUMN(map,seg.origin.x,from,to); }
      return 0;
    }

  /* sloping line */

  FILLBRESSEG(bseg,seg);
  for(;bseg.maximum;)
    {
      if (!OUTSIDE(map,bseg.at))
      {
	if (value)
        { 
           SETBIT(map,bseg.at); 
        }
        else
        {
           CLEARBIT(map,bseg.at);
        }
      }
      STEP(bseg);
    }
  if (!OUTSIDE(map,bseg.at))
  {
     if (value) SETBIT(map,bseg.at); 
     else CLEARBIT(map,bseg.at);
  }
  return 0;
}


/******************************************************************************
 *
 *N  LineXorBit
 *
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    If value=1, an XOR is done on all bits(actually, just a toggle of off bits
 *    to on and on bits to off.  If value=0, nothing is done
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    map        <inout>  == (ArrayOfBits) 2-d array to be modified
 *    seg        <input>  == (ISegment) line coords for modification
 *    value      <input>  == (int) tell whether to XOR or not
 *    LineXorBit <output> == (int) tell whether XORed or not
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Thomas Wood  October 1990, Kernighan&Ritchie C
 *    Updated August 1991, ANSI C
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions/Macros Called:
 *FM
 *    OUTSIDE(MAP,POINT)                BITARRAY.C
 *    SETBIT(MAP,POINT)                 BITARRAY.C
 *    MAXIMUM(x,y)                      BITARRAY.C
 *    MINIMUM(x,y)                      BITARRAY.C
 *    OUTSIDE_Y(MAP,value)              BITARRAY.C
 *    OUTSIDE_X(MAP,value)              BITARRAY.C
 *    FILLBRESSEG(bseg,seg)             BITARRAY.C
 *    STEP(bseg)                        BITARRAY.C
 *    SETROW(ArrayOfBits MAP, int XMIN, int XMAX, int Y)   BITARRAY.C
 *    SETCOLUMN(ArrayOfBits MAP, int X, int YMIN, int YMAX) BITARRAY.C
 *E
 ******************************************************************************/
int LineXorBit ( ArrayOfBits map,
		 ISegment    seg,
		 int         value )
{
  BresSegment bseg;
  unsigned int from,to;

  if (!value) return 0;
  if (map.array == NULL) return 0;
  if ((!seg.delta.x)&&(!seg.delta.y))
    {
      if (OUTSIDE(map,seg.origin)) return 0;
      TOGGLEBIT(map,seg.origin);
      return 0;
    }

  if (seg.delta.y == 0)
    {
      if (OUTSIDE_Y(map,seg.origin.y)) return 0;
      if (seg.delta.x < 0)
	{
	  from = MAXIMUM(seg.origin.x + seg.delta.x, 0);
	  to   = MINIMUM(seg.origin.x, map.bounding.x);
	}
      else
	{
	  from = MAXIMUM(seg.origin.x, 0);
	  to   = MINIMUM(seg.origin.x + seg.delta.x, map.bounding.x);
	}
      if (from > to) return 0;
      if (from == to) { seg.origin.x = from; TOGGLEBIT(map,seg.origin); }
      TOGGLEROW(map,from,to,seg.origin.y);
      return 0;
    }

  if (seg.delta.x == 0)
    {
      if (OUTSIDE_X(map,seg.origin.x)) return 0;
      if (seg.delta.y < 0)
	{
	  from = MAXIMUM(seg.origin.y + seg.delta.y ,0);
	  to   = MINIMUM(seg.origin.y, map.bounding.y);
	}
      else
	{
	  from = MAXIMUM(seg.origin.y, 0);
	  to   = MINIMUM(seg.origin.y + seg.delta.y, map.bounding.y);
	}
      if (from > to) return 0;
      if (from == to) { seg.origin.y = from; TOGGLEBIT(map,seg.origin); }
      TOGGLECOLUMN(map,seg.origin.x,from,to);
      return 0;
    }

  /* sloping line */
  
  FILLBRESSEG(bseg,seg);
  for(;bseg.maximum;)
    {
      if (!OUTSIDE(map,bseg.at))
	TOGGLEBIT(map,bseg.at);
      STEP(bseg);
    }
  if (!OUTSIDE(map,bseg.at))
    TOGGLEBIT(map,bseg.at);
  return 0;
}


/******************************************************************************
 *
 *N  TrapezoidXorBit
 *
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    Turns off any bit that was on, turns on any bit that was of in the line
 *    designated by coordinates in seg.  This is only done if value=1, else
 *    0 is returned and nothing is done
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    map             <inout>  == (ArrayOfBits) 2-d array to be modified
 *    seg             <input>  == (ISegment) line designated by 2 coords
 *    value           <input>  == (int) if 0, returns 0, else swaps bits
 *    TrapezoidXorBit <output> == (int) returns 0
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Thomas Wood  October 1990  Original Version Kernighan&Ritchie C
 *    Updated August 1991 ANSI C
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions/Macros Called:
 *FM
 *    OUTSIDE_X(MAP,value)		BITARRAY.C
 *    MINUMUM(x,y)			BITARRAY.C
 *    MAXIMUM(x,y)			BITARRAY.C
 *    FILLBRESSEG(bseg,seg)		BITARRAY.C
 *    TOGGLEROW(ArrayOfBits MAP,int XMIN,int XMAX,int Y)  BITARRAY.C
 *    TOGGLECOLUMN(ArrayOfBits MAP,int X,int YMIN,int YMAX)  BITARRAY.C
 *E
 *****************************************************************************/
int TrapezoidXorBit( ArrayOfBits map,
                     ISegment    seg,
                     int         value )
{
  BresSegment bseg;
  int xmin,xmax,to /*,temp*/;

  if (map.array == NULL) return 0;
  if (!value) return 0;

  if (seg.delta.x == 0)
    {
      if (OUTSIDE_X(map,seg.origin.x)) return 0;
      if (seg.delta.y < 0)
	to = MINIMUM(seg.origin.y, map.bounding.y);
      else
	to = MINIMUM(seg.origin.y + seg.delta.y, map.bounding.y);
      if  (0 > to) return 0;
      TOGGLECOLUMN(map,seg.origin.x,0,to);
      return 0;
    }

  if (seg.delta.y == 0)
    {
      if (seg.origin.y < 0) return 0;
      if (seg.origin.y > map.bounding.y)
	to = map.bounding.y;
      else
	to = seg.origin.y;
      if (seg.delta.x < 0)
	{
	  xmin = MAXIMUM(seg.origin.x + seg.delta.x, 0);
	  xmax = MINIMUM(seg.origin.x, map.bounding.x);
	}
      else
	{
/*	  temp = seg.origin.x + seg.delta.x;*/
	  xmin = MAXIMUM(seg.origin.x, 0);
	  xmax = MINIMUM(seg.origin.x + seg.delta.x, map.bounding.x);
	}
      if (xmin > xmax) return 0;
      for(; 0 <= to; to--)
	{ TOGGLEROW(map,xmin,xmax,to); }
      return 0;
    }
  
  /* sloping line */
  FILLBRESSEG(bseg,seg);

  for(;bseg.maximum;)
    {
      if (bseg.toggle)
	{
	  if ((bseg.Eprim>=0) &&
	      ((bseg.at.y>=0) &&
	       ((bseg.at.x>=0) &&(bseg.at.x<=map.bounding.x))))
	    {
	      to = MINIMUM(bseg.at.y, map.bounding.y);
	      { TOGGLECOLUMN(map,bseg.at.x,0,to); }
	    }
	}
      else
	if ((bseg.at.y>=0) &&
	    ((bseg.at.x>=0)&&(bseg.at.x<=map.bounding.x)))
	  {
	    to = MINIMUM(bseg.at.y, map.bounding.y);
	    { TOGGLECOLUMN(map,bseg.at.x,0,to); }
	  }

      STEP(bseg);
    }
  if ((bseg.at.y>=0) &&
      ((bseg.at.x>=0)&&(bseg.at.x<=map.bounding.x)))
    {
      to = MINIMUM(bseg.at.y, map.bounding.y);
      { TOGGLECOLUMN(map,bseg.at.x,0,to); }
    }
  return 0;
}

