//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description:
//
// Contains class declaration for ossimIrect.
// Container class for four integer points representing a rectangle.
//
//*******************************************************************
//  $Id: ossimIrect.h 22197 2013-03-12 02:00:55Z dburken $

#ifndef ossimIrect_HEADER
#define ossimIrect_HEADER 1
#include <iosfwd>
#include <vector>

#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimCommon.h>

//***
// NOTE:  A word on corner points...
//
// There is the concept of "pixel is area" and "pixel is point".
// - Pixel is area means the (x,y) pixel coordinate refers to the upper left
//   corner of the pixel, NOT the center of the pixel.
// - Pixel is point means the (x,y) pixel coordinate refers to the center
//   of the pixel, NOT the upper left corner.
//
// For the uniformity purposes, all pixel points  should be in the
// "pixel is point" form; therefore, the (x,y) point should represent the
// CENTER of the pixel.
//***


//***
// Forward class declarations.
//***
class ossimDrect;
class ossimKeywordlist;

//*******************************************************************
// CLASS:  ossimIrect
//*******************************************************************

class OSSIMDLLEXPORT ossimIrect
{
public:
   enum
   {
      UPPER_LEFT  = 1,
      LOWER_LEFT  = 2,
      LOWER_RIGHT = 4,
      UPPER_RIGHT = 8
   };

   ossimIrect()
      :
         theUlCorner(0, 0),
         theUrCorner(0, 0),
         theLrCorner(0, 0),
         theLlCorner(0, 0),
         theOrientMode(OSSIM_LEFT_HANDED)
      {}

   ossimIrect(ossimIpt ul_corner,
              ossimIpt lr_corner,
              ossimCoordSysOrientMode mode=OSSIM_LEFT_HANDED)
      :
         theUlCorner(ul_corner),
         theUrCorner(lr_corner.x, ul_corner.y),
         theLrCorner(lr_corner),
         theLlCorner(ul_corner.x, lr_corner.y),
         theOrientMode(mode)
      {}

   ossimIrect(ossim_int32 ul_corner_x,
              ossim_int32 ul_corner_y,
              ossim_int32 lr_corner_x,
              ossim_int32 lr_corner_y,
              ossimCoordSysOrientMode mode=OSSIM_LEFT_HANDED)
      :
         theUlCorner(ul_corner_x, ul_corner_y),
         theUrCorner(lr_corner_x, ul_corner_y),
         theLrCorner(lr_corner_x, lr_corner_y),
         theLlCorner(ul_corner_x, lr_corner_y),
         theOrientMode(mode)
      {}
   
   ossimIrect(const ossimIrect& rect)
      :
         theUlCorner(rect.ul()),
         theUrCorner(rect.ur()),
         theLrCorner(rect.lr()),
         theLlCorner(rect.ll()),
         theOrientMode(rect.orientMode())
      {}

   ossimIrect(const ossimDrect& rect);

   /*!
    * Must compute a bounding rect given these image
    * points.
    */
   ossimIrect(const std::vector<ossimIpt>& points,
              ossimCoordSysOrientMode mode=OSSIM_LEFT_HANDED);

   ossimIrect(const ossimIpt& p1,
              const ossimIpt& p2,
              const ossimIpt& p3,
              const ossimIpt& p4,
              ossimCoordSysOrientMode=OSSIM_LEFT_HANDED);

   //! Constructs an Irect surrounding the specified point, and of specified size.
   ossimIrect(const ossimIpt& center, 
              ossim_uint32    size_x, 
              ossim_uint32    size_y,
              ossimCoordSysOrientMode mode=OSSIM_LEFT_HANDED);

   /** destructor */
   ~ossimIrect();
   
   inline const ossimIrect& operator=  (const ossimIrect& rect);
   const ossimIrect&        operator=  (const ossimDrect& rect);
   inline bool              operator!= (const ossimIrect& rect) const;
   inline bool              operator== (const ossimIrect& rect) const;

   friend ossimIrect operator*(double scalar, const ossimIrect& rect)
      {
         return ossimIrect((int)floor(rect.theUlCorner.x*scalar),
                           (int)floor(rect.theUlCorner.y*scalar),
                           (int)ceil(rect.theUlCorner.x*scalar+rect.width()*scalar-1),
                           (int)ceil(rect.theUlCorner.y*scalar+rect.height()*scalar-1),
                           rect.theOrientMode);
      }

   const ossimIrect& operator*=(ossim_int32 scalar)
   {
      *this = ossimIrect((theUlCorner.x*scalar),
                         (theUlCorner.y*scalar),
                         (theUlCorner.x*scalar+width()*scalar-1),
                         (theUlCorner.y*scalar+height()*scalar-1),
                         theOrientMode);
      return *this;
   }
   
   ossimIrect operator*(ossim_int32 scalar)const
   {
      return ossimIrect((theUlCorner.x*scalar),
                        (theUlCorner.y*scalar),
                        (theUlCorner.x*scalar+width()*scalar-1),
                        (theUlCorner.y*scalar+height()*scalar-1),
                        theOrientMode);
   }
   
   const ossimIrect& operator *=(double scalar)
      {
         *this = ossimIrect((int)floor(theUlCorner.x*scalar),
                            (int)floor(theUlCorner.y*scalar),
                            (int)ceil(theUlCorner.x*scalar+width()*scalar-1),
                            (int)ceil(theUlCorner.y*scalar+height()*scalar-1),
                            theOrientMode);
         return *this;
      }
   
   ossimIrect operator *(double scalar)const
      {
         return ossimIrect((int)floor(theUlCorner.x*scalar),
                           (int)floor(theUlCorner.y*scalar),
                           (int)ceil(theUlCorner.x*scalar+width()*scalar-1),
                           (int)ceil(theUlCorner.y*scalar+height()*scalar-1),
                           theOrientMode);
      }

   const ossimIrect& operator *=(const ossimDpt& scalar)
      {
         *this = ossimIrect((int)floor(theUlCorner.x*scalar.x),
                            (int)floor(theUlCorner.y*scalar.y),
                            (int)ceil(theUlCorner.x*scalar.x+width()*scalar.x-1),
                            (int)ceil(theUlCorner.y*scalar.y+height()*scalar.y-1),
                            theOrientMode);
         return *this;
      }
   
   ossimIrect operator *(const ossimDpt& scalar)const
      {
         return  ossimIrect((int)floor(theUlCorner.x*scalar.x),
                            (int)floor(theUlCorner.y*scalar.y),
                            (int)ceil(theUlCorner.x*scalar.x+width()*scalar.x-1),
                            (int)ceil(theUlCorner.y*scalar.y+height()*scalar.y-1),
                            theOrientMode);
      }
   const ossimIrect& operator +=(const ossimIpt& shift)
      {
         *this = ossimIrect(theUlCorner.x+shift.x,
                            theUlCorner.y+shift.y,
                            theLrCorner.x+shift.x,
                            theLrCorner.y+shift.y,
                            theOrientMode);
         return *this;
      }
   
   const ossimIrect& operator -=(const ossimIpt& shift)
      {
         *this = ossimIrect(theUlCorner.x-shift.x,
                            theUlCorner.y-shift.y,
                            theLrCorner.x-shift.x,
                            theLrCorner.y-shift.y,
                            theOrientMode);
         return *this;
      }
   
   ossimIrect operator +(const ossimIpt& shift)const
      {
         return ossimIrect(theUlCorner.x+shift.x,
                           theUlCorner.y+shift.y,
                           theLrCorner.x+shift.x,
                           theLrCorner.y+shift.y,
                           theOrientMode);
      }

   ossimIrect operator -(const ossimIpt& shift)const
      {
         return ossimIrect(theUlCorner.x-shift.x,
                           theUlCorner.y-shift.y,
                           theLrCorner.x-shift.x,
                           theLrCorner.y-shift.y,
                           theOrientMode);
      }


   const ossimIrect& operator +=(const ossimDpt& shift)
      {
         *this = ossimIrect((int)floor(theUlCorner.x+shift.x),
                            (int)floor(theUlCorner.y+shift.y),
                            (int)ceil(theUlCorner.x+shift.x+width()-1),
                            (int)ceil(theUlCorner.y+shift.y+height()-1),
                            theOrientMode);
         return *this;
      }
   
   const ossimIrect& operator -=(const ossimDpt& shift)
      {
         *this = ossimIrect((int)floor(theUlCorner.x-shift.x),
                            (int)floor(theUlCorner.y-shift.y),
                            (int)ceil(theUlCorner.x-shift.x+width()-1),
                            (int)ceil(theUlCorner.y-shift.y+height()-1),
                            theOrientMode);
         return *this;
      }
   
   ossimIrect operator +(const ossimDpt& shift)const
      {
         return ossimIrect((int)floor(theUlCorner.x+shift.x),
                           (int)floor(theUlCorner.y+shift.y),
                           (int)ceil(theUlCorner.x+shift.x+width()-1),
                           (int)ceil(theUlCorner.y+shift.y+height()-1),
                           theOrientMode);
      }

   ossimIrect operator -(const ossimDpt& shift)const
      {
         return ossimIrect((int)floor(theUlCorner.x-shift.x),
                           (int)floor(theUlCorner.y-shift.y),
                           (int)ceil(theUlCorner.x-shift.x+width()-1),
                           (int)ceil(theUlCorner.y-shift.y+height()-1),
                           theOrientMode);
         return *this;
      }

   
   const ossimIpt& ul() const { return theUlCorner; }
   const ossimIpt& ur() const { return theUrCorner; }
   const ossimIpt& lr() const { return theLrCorner; }
   const ossimIpt& ll() const { return theLlCorner; }

   const ossimIrect& changeOrientationMode(ossimCoordSysOrientMode mode)
   {
      // if we are already in the orientation then return
      //
      if(mode == theOrientMode) return *this;
      if(mode == OSSIM_LEFT_HANDED)
      {
         // we must be right handed so change to left handed
         *this = ossimIrect(theUlCorner.x,
                            theLlCorner.y,
                            theLrCorner.x,
                            theUlCorner.y,
                            OSSIM_LEFT_HANDED);
      }
      else
      {
         // we must be left handed so change to RIGHT handed
         *this = ossimIrect(theUlCorner.x,
                            theLlCorner.y,
                            theLrCorner.x,
                            theUlCorner.y,
                            OSSIM_RIGHT_HANDED);
      }
      theOrientMode = mode;
      
      return *this;
   }
   void getBounds(ossim_int32& minx, ossim_int32& miny,
                  ossim_int32& maxx, ossim_int32& maxy)const
      {
         minx = theUlCorner.x;
         maxx = theLrCorner.x;
         if(theOrientMode == OSSIM_LEFT_HANDED)
         {
            miny = theUlCorner.y;
            maxy = theLrCorner.y;
         }
         else
         {
            maxy = theUlCorner.y;
            miny = theLrCorner.y;
         }
      }

   /*!
    * Initializes center_point with center of the rectangle.
    * Makes center_point nan if this rectangle has nans.
    */
   void getCenter(ossimDpt& center_point) const;
   
   void makeNan()
   {
      theUlCorner.makeNan();
      theLlCorner.makeNan();
      theLrCorner.makeNan();
      theUrCorner.makeNan();
   }
   
   bool hasNans()const{ return (theUlCorner.hasNans() ||
                                theLlCorner.hasNans() ||
                                theLrCorner.hasNans() ||
                                theUrCorner.hasNans());}
   
   bool isNan()const{ return (theUlCorner.isNan() &&
                              theLlCorner.isNan() &&
                              theLrCorner.isNan() &&
                              theUrCorner.isNan());}

   //***
   // This class supports both left and right-handed coordinate systems. For
   // both, the positive x-axis extends to the "right".
   //***
   ossimCoordSysOrientMode orientMode() const { return theOrientMode; }
   void setOrientMode(ossimCoordSysOrientMode mode) { theOrientMode = mode; }

   void stretchToTileBoundary(const ossimIpt& tileWidthHeight);

   /**
    * @return ossimString representing ossimIrect.
    *
    * Format:  ( 30, -90, 512, 512, [LH|RH] )
    *            -x- -y-  -w-  -h-   -Right or left handed-
    *
    * where:
    *     x and y are origins either upper left if LEFT HANDED (LH) or
    *                                lower left if RIGHT HANDED (RH)
    *     w and h are width and height respectively
    *     The last value is LH or RH to indicate LeftHanded or RightHanded
    *    
    */
   ossimString toString()const;
   
   
   /**
    * expected Format:  form 1: ( 30, -90, 512, 512, [LH|RH] )
    *                            -x- -y-  -w-  -h-   -Right or left handed-
    * 
    *                   form 2: ( 30, -90, 512, 512)
    *                            -x- -y-  -w-  -h-
    *
    * NOTE: Form 2 assumes Left handed were x,y is origin upper left and y positive down.
    *
    * This method starts by doing a "makeNan" on rect. 
    *
    * @param rectString String to initialize from.
    * @return true or false to indicate successful parsing.
    */
   bool toRect(const ossimString& rectString);
   

   const ossimIrect& expand(const ossimIpt& padding);

   //! Guarantees that this rect will be at least w X h big. If smaller than specified, the 
   //! corresponding side will be stretched equally in + and - direction to meet required size.
   //! Returns TRUE if resizing occurred.
   bool insureMinimumSize(const ossimIpt& width_height);

   ossim_uint32 area()const
      {
         return width()*height();
      }

   /*!
    * Sets the upper left corner to "pt".  Adjusts the remaining corners
    * accordingly.
    */
   inline void set_ul(const ossimIpt& pt);

   /*!
    * Sets the upper right corner to "pt".  Adjusts the remaining corners
    * accordingly.
    */
   inline void set_ur(const ossimIpt& pt);

   /*!
    * Sets the lower right corner to "pt".  Adjusts the remaining corners
    * accordingly.
    */
   inline void set_lr(const ossimIpt& pt);

   /*!
    * Sets the lower left corner to "pt".  Adjusts the remaining corners
    * accordingly.
    */
   inline void set_ll(const ossimIpt& pt);

   /*!
    * Sets the upper left x.  Adjusts the remaining corners accordingly.
    */
   inline void set_ulx(ossim_int32 x);

   /*!
    * Sets the upper left y.  Adjusts the remaining corners accordingly.
    */
   inline void set_uly(ossim_int32 y);

   /*!
    * Sets the upper right x.  Adjusts the remaining corners accordingly.
    */
   inline void set_urx(ossim_int32 x);

   /*!
    * Sets the upper right y.  Adjusts the remaining corners accordingly.
    */
   inline void set_ury(ossim_int32 y);

   /*!
    * Sets the lower right x.  Adjusts the remaining corners accordingly.
    */
   inline void set_lrx(ossim_int32 x);

   /*!
    * Sets the lower right y.  Adjusts the remaining corners accordingly.
    */
   inline void set_lry(ossim_int32 y);

   /*!
    * Sets the lower left x.  Adjusts the remaining corners accordingly.
    */
   inline void set_llx(ossim_int32 x);

   /*!
    * Sets the lower left y.  Adjusts the remaining corners accordingly.
    */
   inline void set_lly(ossim_int32 y);

   /*!
    * Returns true if "pt" falls within rectangle.  Fall on an edge is also
    * considered to be within.
    */
   inline bool pointWithin(const ossimIpt& pt) const;

   /*!
    * Returns true if any portion of an input rectangle "rect" intersects
    * "this" rectangle.  
    */
   bool intersects(const ossimIrect& rect) const;

   /*!
    * Returns true if "this" rectangle is contained completely within
    * the input rectangular "rect".
    */
   bool completely_within(const ossimIrect& rect) const;

   ossimCoordSysOrientMode orientationMode()const{return theOrientMode;}
   /*!
    * Returns the height of the rectangle.
    */
   ossim_uint32 height() const
   {
      ossim_int32 h = theLlCorner.y - theUlCorner.y;
      if (h < 0)
      {
         h = -h;
      }
      return static_cast<ossim_uint32>( h + 1 );
   }

   /*!
    * Returns the width of a rectangle.
    */
   ossim_uint32 width()  const
   {
      ossim_int32 w = theLrCorner.x - theLlCorner.x;
      if (w < 0)
      {
         w = -w;
      }
      return static_cast<ossim_uint32>( w + 1 );
   }

   ossimIpt size() const { return ossimIpt(width(), height()); }
 
   ossimIrect clipToRect(const ossimIrect& rect)const;

   inline ossimIpt midPoint()const;

   void print(std::ostream& os) const;

   ossimIrect combine(const ossimIrect& rect)const;
   
   friend OSSIMDLLEXPORT std::ostream& operator<<(std::ostream& os,
                                                  const ossimIrect& rect);

   bool saveState(ossimKeywordlist& kwl,
                  const char* prefix=0)const;
   bool loadState(const ossimKeywordlist& kwl,
                  const char* prefix=0);
private:

   //***
   // Private data members representing the rectangle corners.
   //***
   ossimIpt theUlCorner;
   ossimIpt theUrCorner;
   ossimIpt theLrCorner;
   ossimIpt theLlCorner;

   ossimCoordSysOrientMode  theOrientMode;
};

//*******************************************************************
// Inline Method: ossimIrect::operator=(ossimIrect)
//*******************************************************************
inline const ossimIrect& ossimIrect::operator=(const ossimIrect& rect)
{
   if (this != &rect)
   {
      theUlCorner   = rect.ul();
      theUrCorner   = rect.ur();
      theLrCorner   = rect.lr();
      theLlCorner   = rect.ll();
      theOrientMode = rect.theOrientMode;

      if(rect.hasNans())
      {
         makeNan();
      }
   }

   return *this;
}

//*******************************************************************
// Inline Method: ossimIrect::operator!=
//*******************************************************************
inline bool ossimIrect::operator!=(const ossimIrect& rect) const
{
   return ( (theUlCorner   != rect.ul()) ||
            (theUrCorner   != rect.ur()) ||
            (theLrCorner   != rect.lr()) ||
            (theLlCorner   != rect.ll()) ||
            (theOrientMode != rect.theOrientMode));
}

//*******************************************************************
// Inline Method: ossimIrect::operator==
//*******************************************************************
inline bool ossimIrect::operator==(const ossimIrect& rect) const
{
   return ( (theUlCorner   == rect.ul()) &&
            (theUrCorner   == rect.ur()) &&
            (theLrCorner   == rect.lr()) &&
            (theLlCorner   == rect.ll()) &&
            (theOrientMode == rect.theOrientMode) );
}

//*******************************************************************
// Inline Method: ossimIrect::set_ul
//*******************************************************************
inline void ossimIrect::set_ul(const ossimIpt& pt)
{
   if(pt.hasNans())
   {
      makeNan();
   }
   else
   {
      theUlCorner   = pt;
      theUrCorner.y = pt.y;
      theLlCorner.x = pt.x;
   }
}

//*******************************************************************
// Inline Method: ossimIrect::set_ur
//*******************************************************************
inline void ossimIrect::set_ur(const ossimIpt& pt)
{
   if(pt.hasNans())
   {
      makeNan();
   }
   else
   {
      theUrCorner   = pt;
      theUlCorner.y = pt.y;
      theLrCorner.x = pt.x;
   }
}

//*******************************************************************
// Inline Method: ossimIrect::set_lr
//*******************************************************************
inline void ossimIrect::set_lr(const ossimIpt& pt)
{
   if(pt.hasNans())
   {
      makeNan();
   }
   else
   {
      theLrCorner   = pt;
      theUrCorner.x = pt.x;
      theLlCorner.y = pt.y;
   }
}

//*******************************************************************
// Inline Method: ossimIrect::set_ll
//*******************************************************************
inline void ossimIrect::set_ll(const ossimIpt& pt)
{
   if(pt.hasNans())
   {
      makeNan();
   }
   else
   {
      theLlCorner   = pt;
      theUlCorner.x = pt.x;
      theLrCorner.y = pt.y;
   }
}

//*******************************************************************
// Inline Method: ossimIrect::set_ulx
//*******************************************************************
inline void ossimIrect::set_ulx(ossim_int32 x)
{
   theUlCorner.x = x;
   theLlCorner.x = x;
}

//*******************************************************************
// Inline Method: ossimIrect::set_uly
//*******************************************************************
inline void ossimIrect::set_uly(ossim_int32 y)
{
   theUlCorner.y = y;
   theUrCorner.y = y;
}

//*******************************************************************
// Inline Method: ossimIrect::set_urx
//*******************************************************************
inline void ossimIrect::set_urx(ossim_int32 x)
{
   theUrCorner.x = x;
   theLrCorner.x = x;
}

//*******************************************************************
// Inline Method: ossimIrect::set_ury
//*******************************************************************
inline void ossimIrect::set_ury(ossim_int32 y)
{
   theUrCorner.y = y;
   theUlCorner.y = y;
}

//*******************************************************************
// Inline Method: ossimIrect::set_lrx
//*******************************************************************
inline void ossimIrect::set_lrx(ossim_int32 x)
{
   theLrCorner.x = x;
   theUrCorner.x = x;
}

//*******************************************************************
// Inline Method: ossimIrect::set_lry
//*******************************************************************
inline void ossimIrect::set_lry(ossim_int32 y)
{
   theLrCorner.y = y;
   theLlCorner.y = y;
}

//*******************************************************************
// Inline Method: ossimIrect::set_llx
//*******************************************************************
inline void ossimIrect::set_llx(ossim_int32 x)
{
   theLlCorner.x = x;
   theUlCorner.x = x;
}

//*******************************************************************
// Inline Method: ossimIrect::set_lly
//*******************************************************************
inline void ossimIrect::set_lly(ossim_int32 y)
{
   theLlCorner.y = y;
   theLrCorner.y = y;
}

//*******************************************************************
// Inline Method: ossimIrect::pointWithin
//*******************************************************************
inline bool ossimIrect::pointWithin(const ossimIpt& pt) const
{
   if(hasNans())
   {
      return false;
   }
   if (theOrientMode == OSSIM_LEFT_HANDED)
      return ((pt.x >= ul().x) &&
              (pt.x <= ur().x) &&
              (pt.y >= ul().y) &&
              (pt.y <= ll().y));
   else
      return ((pt.x >= ul().x) &&
              (pt.x <= ur().x) &&
              (pt.y <= ul().y) &&
              (pt.y >= ll().y));
}

//*******************************************************************
// Inline Method: ossimIrect::midPoint
//*******************************************************************
inline ossimIpt ossimIrect::midPoint()const
{
   if(hasNans())
   {
      return ossimIpt(OSSIM_INT_NAN, OSSIM_INT_NAN);
   }
   double x = (ul().x + ur().x + ll().x + lr().x) * 0.25;
   double y = (ul().y + ur().y + ll().y + lr().y) * 0.25;
   
   return ossimIpt(ossim::round<int>(x),
                   ossim::round<int>(y));
}

#endif
