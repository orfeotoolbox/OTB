
/* POLYGRF.H */

#ifndef POLYGRF_H

#define POLYGRF_H


#include <ossim/vpfutil/protomas.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
  int x,y;
} IPoint;

typedef struct
{
  IPoint origin;
  IPoint delta;
} ISegment;

typedef struct
{
  IPoint at;
  int    Eprim,toggle,maximum;
  IPoint signs;
  struct {
    int inc,step;
    } errors;
} BresSegment;

typedef struct {
  IPoint origin;
  IPoint delta;
} IBox;

typedef struct
{
  struct
    {
      float x,y;
    } scale;
  IPoint offset;
} ITransform;

#define TRANSFORMSPACE 0
#define USERSPACE      1

typedef unsigned char Color;

typedef struct
{
  unsigned char type,width,height;
  union {
    long pad;
    unsigned char *list;
    struct {
      unsigned char ll,lr,ul,ur;
    } vpfPatternSmall;
  } vpfPatterData;
} vpfPattern;

typedef struct {
  IBox bounding;
  int depth;
  int rowcount;
  unsigned char *array;
} BitArray;

typedef struct {
  IPoint bounding;
  int rowcount;
  unsigned char *array;
} ArrayOfBits;

typedef struct
{
  ArrayOfBits  Bordermap,Maskmap;
  ITransform   transform;
  int          pointcount;
  IPoint       first;
  int          secx;
  int          llx;
  IPoint       turnpoint;
  IPoint       prev;
  Color        line;
  vpfPattern      fill;

  IBox         bounding;   /* Original bounding box on the screen */

} Shademap;

#define isBox(BOX) ((BOX.delta.x >= 0)&&(BOX.delta.y >= 0))



/********************************************************
 *  POLYGRF.C Definitions
 *******************************************************/
ISegment MakeISegment( IPoint from,
		       IPoint to );

IBox MakeIBox( IPoint from,
	       IPoint to );

ITransform MakeITransform( IBox from,
			   IBox to );


IPoint TranslateIPoint( ITransform transform,
		        IPoint a,
		        int flag );

ISegment TranslateISegment( ITransform transform,
			    ISegment a,
			    int flay );

IBox TranslateIBox( ITransform transform,
		    IBox a,
		    int flag );
/* _UNUSED_
IPoint Iintersect ( ISegment, ISegment );
float  Iturn      ( ISegment, IPoint );
*/

IBox AndIBox( IBox a,
	      IBox b );



/* OTHER DEFS. CANT FIND WHERE THEY ARE USED
ISegment ISegmentClipIBox ( IBox, ISegment );
BitArray MakeBitArray EXT3( int, int, int );

int FreeBitArray  EXT1( BitArray );

int GetBitPoint   EXT2( BitArray, ipoint );

int ClearBitArray EXT1( BitArray );
int SetBitArray   EXT2( BitArray, int );
int AndBitArray   EXT2( BitArray, int );
int OrBitArray    EXT2( BitArray, int );
int XorBitArray   EXT2( BitArray, int );

int SetBitPoint   EXT3( BitArray, IPoint, int );
int AndBitPoint   EXT3( BitArray, IPoint, int );
int OrBitPoint    EXT3( BitArray, IPoint, int );
int XorBitPoint   EXT3( BitArray, IPoint, int );

int SetBitLine    EXT3( BitArray, ISegment, int );
int AndBitLine    EXT3( BitArray, ISegment, int );
int OrBitLine     EXT3( BitArray, ISegment, int );
int XorBitLine    EXT3( BitArray, ISegment, int );

int SetBitWedge   EXT4( BitArray, ISegment, IPoint, int );
int AndBitWedge   EXT4( BitArray, ISegment, IPoint, int );
int OrBitWedge    EXT4( BitArray, ISegment, IPoint, int );
int XorBitWedge   EXT4( BitArray, ISegment, IPoint, int );

int SetBitBox     EXT3( BitArray, IBox, int );
int AndBitBox     EXT3( BitArray, IBox, int );
int OrBitBox      EXT3( BitArray, IBox, int );
int XorBitBox     EXT3( BitArray, IBox, int );

int SetBitMask    EXT4( BitArray, BitArray, IPoint, int );
int AndBitMask    EXT4( BitArray, BitArray, IPoint, int );
int OrBitMask     EXT4( BitArray, BitArray, IPoint, int );
int XorBitMask    EXT4( BitArray, BitArray, IPoint, int );
*/
/***********************************************************************
 *   end polygrf.c definitions
 **********************************************************************/


/***********************************************************************
 *   BITARRAY.C TYPES
 **********************************************************************/
#include <ossim/vpfutil/xmemory.h>

#define MINIMUM(x,y) (((x) > (y))? (y) : (x))
#define MAXIMUM(x,y) (((x) < (y))? (y) : (x))

#define OUTSIDE(MAP,VPF_POINT_STRUCT)\
  ((VPF_POINT_STRUCT.x < 0)||                      \
    (VPF_POINT_STRUCT.y < 0)||                      \
    (VPF_POINT_STRUCT.x > MAP.bounding.x)||   \
    (VPF_POINT_STRUCT.y > MAP.bounding.y))

#define OUTSIDE_X(MAP,value)  (((value)< 0)||((value)> MAP.bounding.x))
#define OUTSIDE_Y(MAP,value)  (((value)< 0)||((value)> MAP.bounding.y))

#define GETBIT(MAP,VPF_POINT_STRUCT) \
   (((MAP.array[(((VPF_POINT_STRUCT.y)*(MAP.rowcount))+((VPF_POINT_STRUCT.x)>>3))])&(1<<((VPF_POINT_STRUCT.x)%8)))? 1: 0)

#define SETBIT(MAP,VPF_POINT_STRUCT) \
   ((MAP.array[(((VPF_POINT_STRUCT.y)*(MAP.rowcount))+((VPF_POINT_STRUCT.x)>>3))]) |= (1<<((VPF_POINT_STRUCT.x)%8)))

#define CLEARBIT(MAP,VPF_POINT_STRUCT) \
   ((MAP.array[(((VPF_POINT_STRUCT.y)*(MAP.rowcount))+((VPF_POINT_STRUCT.x)>>3))]) &= ~(1<<((VPF_POINT_STRUCT.x)%8)))

#define TOGGLEBIT(MAP,VPF_POINT_STRUCT) \
   ((MAP.array[(((VPF_POINT_STRUCT.y)*(MAP.rowcount))+((VPF_POINT_STRUCT.x)>>3))]) ^= (1<<((VPF_POINT_STRUCT.x)%8)))

#define LEFTBITS(INDEX)  (255&(~(255<<(1+(INDEX)))))
#define RIGHTBITS(INDEX) (255&(~(255>>(8-(INDEX)))))

#define FILLBRESSEG(bseg,seg) \
{                                                  \
  int tempx,tempy;                                 \
  bseg.signs.x = (seg.delta.x < 0) ? -1: 1;        \
  bseg.signs.y = (seg.delta.y < 0) ? -1: 1;        \
  tempx = seg.delta.x * bseg.signs.x;              \
  tempy = seg.delta.y * bseg.signs.y;              \
  bseg.at = seg.origin;                            \
                                                   \
  if (tempy > tempx)                               \
    {                                              \
      bseg.Eprim        = (tempx<<1) - tempy;      \
      bseg.errors.inc   = (tempx<<1);              \
      bseg.errors.step  = (tempx<<1) - (tempy<<1); \
      bseg.maximum      = tempy;                   \
      bseg.toggle       = 1;                       \
    }                                              \
  else                                             \
    {                                              \
      bseg.Eprim        = (tempy<<1) - tempx;      \
      bseg.errors.inc   = (tempy<<1);              \
      bseg.errors.step  = (tempy<<1) - (tempx<<1); \
      bseg.maximum      = tempx;                   \
      bseg.toggle       = 0;                       \
    }                                              \
}


#define STEP(bseg) \
 if (bseg.maximum){                     \
   if(bseg.Eprim<0)                    \
     {                                 \
       bseg.Eprim += bseg.errors.inc;  \
       if (bseg.toggle)                \
         bseg.at.y  += bseg.signs.y;   \
       else                            \
         bseg.at.x  += bseg.signs.x;   \
       bseg.maximum--;                 \
     }                                 \
   else                                \
     {                                 \
       bseg.Eprim += bseg.errors.step; \
       bseg.at.x  += bseg.signs.x;     \
       bseg.at.y  += bseg.signs.y;     \
       bseg.maximum--;                 \
     }                                 \
   }





/***********************************************************************
 *   BITARRAY.C DEFINITIONS
 **********************************************************************/
void SETROW( ArrayOfBits MAP,
	     int	 XMIN,
	     int	 XMAX,
	     int	 Y );

void CLEARROW( ArrayOfBits MAP,
	       int	   XMIN,
	       int	   XMAX,
	       int	   Y );

void TOGGLEROW( ArrayOfBits MAP,
	        int	    XMIN,
	        int	    XMAX,
	        int	    Y );

void SETCOLUMN( ArrayOfBits MAP,
	        int	    X,
	        int 	    YMIN,
	        int	    YMAX );

void CLEARCOLUMN( ArrayOfBits MAP,
		  int	      X,
		  int 	      YMIN,
		  int	      YMAX );

void TOGGLECOLUMN( ArrayOfBits MAP,
		   int	       X,
		   int	       YMIN,
		   int	       YMAX );

ArrayOfBits MakeArrayOfBits( int deltaX,
			     int deltaY );

int FreeArrayOfBits( ArrayOfBits map );

int ClearArrayOfBits( ArrayOfBits map );

int PointGetBit( ArrayOfBits map,
		 IPoint point );

int PointSetBit( ArrayOfBits map,
                 IPoint      at,
		 int  	     value );

int PointAndBit( ArrayOfBits map,
		 IPoint	     at,
		 int 	     value );

int PointOrBit( ArrayOfBits map,
	        IPoint      at,
	        int	    value );

int PointXorBit( ArrayOfBits map,
		 IPoint      at,
		 int 	     value );

int LineSetBit( ArrayOfBits map,
	        ISegment    seg,
	        int 	    value );

int LineAndBit( ArrayOfBits map,
	        ISegment    seg,
	        int         value );

int LineOrBit( ArrayOfBits map,
	       ISegment    seg,
	       int 	   value );

int LineXorBit( ArrayOfBits map,
	        ISegment    seg,
	        int 	    value );

/* THESE PROCEDURES NOT DEFINED
int WedgeSetBit       EXT4( ArrayOfBits, ISegment, IPoint, int );
int WedgeAndBit       EXT4( ArrayOfBits, ISegment, IPoint, int );
int WedgeOrBit        EXT4( ArrayOfBits, ISegment, IPoint, int );
int WedgeXorBit       EXT4( ArrayOfBits, ISegment, IPoint, int );
*/

int BoxSetBit( ArrayOfBits map,
	       IBox        box,
	       int 	   value );

int BoxAndBit( ArrayOfBits map,
	       IBox        box,
	       int 	   value );

int BoxOrBit( ArrayOfBits map,
	      IBox	  box,
	      int 	  value );

int BoxXorBit( ArrayOfBits map,
	       IBox	   box,
	       int	   value );

int TrapezoidSetBit( ArrayOfBits map,
		     ISegment	 seg,
		     int	 value );

int TrapezoidAndBit( ArrayOfBits map,
		     ISegment	 seg,
		     int	 value );

int TrapezoidOrBit( ArrayOfBits map,
		    ISegment	seg,
		    int 	value );

int TrapezoidXorBit( ArrayOfBits map,
		     ISegment	 seg,
		     int	 value );

int SetArrayOfBits( ArrayOfBits map,
		    int 	value );

int AndArrayOfBits( ArrayOfBits map,
		    int		value );

int OrArrayOfBits( ArrayOfBits map,
		   ISegment    seg,
		   int	       value );

int XorArrayOfBits( ArrayOfBits map,
		    ISegment	seg,
		    int 	value);
/***********************************************************************
 * end of bitarray.c definitions
 **********************************************************************/


/***********************************************************************
 * Shademap.c definitions
 **********************************************************************/

#define SOLID   0
#define STRIPS  1
#define STRIPES 1
#define SIMPLE  2
/* #define COMPLEX_FEATURE 3 */

Shademap *OpenShade( IBox    bounding,
		     IBox    window,
		     Color   line,
		     vpfPattern fill );

int ModifyShade( Shademap *smap,
		 Color	  line,
		 vpfPattern  fill );

int WriteShade( Shademap *smap,
	        IPoint	 point );

int CloseLoop( Shademap *smap );

int CloseShade( Shademap *smap );

int OutShade( Shademap *smap );

vpfPattern MakePattern( int ll,
		     int lr,
		     int ul,
		     int ur );

/***************************************************************************
 * end shademap.c definitions
 **************************************************************************/
#ifdef __cplusplus
}
#endif

#endif

