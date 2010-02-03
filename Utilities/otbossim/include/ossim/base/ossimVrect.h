//*******************************************************************
//
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Garrett Potts
//
// Description:
//
// Contains class declaration for vrect.
// Container class for four double points representing a rectangle
// where y is up
// 
//*******************************************************************
//  $Id: ossimVrect.h 11955 2007-10-31 16:10:22Z gpotts $
#ifndef ossimVrect_HEADER
#define ossimVrect_HEADER
#include <iostream>

#include <ossim/base/ossimDpt.h>

class OSSIMDLLEXPORT ossimVrect
{
public:
   ossimVrect()
      :theUlCorner(0.0, 0.0),
       theLlCorner(0.0, 0.0),
       theLrCorner(0.0, 0.0),
       theUrCorner(0.0, 0.0)
      {}
   ossimVrect(const ossimDpt &ul,
         const ossimDpt &lr)
      :theUlCorner(ul),
       theLlCorner(ul.x, lr.y),
       theLrCorner(lr),
       theUrCorner(lr.x, ul.y)
      {}
   ossimVrect(double ul_corner_x,
         double ul_corner_y,
         double lr_corner_x,
         double lr_corner_y)
      :
         theUlCorner(ul_corner_x, ul_corner_y),
         theLlCorner(ul_corner_x, lr_corner_y),
         theLrCorner(lr_corner_x, lr_corner_y),
         theUrCorner(lr_corner_x, ul_corner_y)
      {}
   ossimVrect(const ossimVrect& rect)
      :
         theUlCorner(rect.ul()),
         theLlCorner(rect.ll()),
         theLrCorner(rect.lr()),
         theUrCorner(rect.ur())
      {}

   ~ossimVrect();

   const ossimVrect& operator=  (const ossimVrect& rect);
   bool         operator!= (const ossimVrect& rect) const;
   bool         operator== (const ossimVrect& rect) const;
   
   const ossimDpt &ul() const { return theUlCorner; }
   const ossimDpt &ur() const { return theUrCorner; }
   const ossimDpt &lr() const { return theLrCorner; }
   const ossimDpt &ll() const { return theLlCorner; }
   /*!
    * Returns the height of a rectangle.
    */
   ossim_float64 height() const
      { return fabs(theLlCorner.y - theUlCorner.y) + 1.0; }

   /*!
    * Returns the width of a rectangle.
    */
   ossim_float64 width()  const
      { return fabs(theLrCorner.x - theLlCorner.x) + 1.0; }
   
   inline ossimDpt midPoint()const;
   void print(std::ostream& os) const;
   
   ossimVrect combine(const ossimVrect& rect)const;
   inline ossimVrect clipToRect(const ossimVrect& rect)const;

   friend std::ostream& operator<<(std::ostream& os, const ossimVrect& rect);

private:
   ossimDpt theUlCorner;
   ossimDpt theLlCorner;
   ossimDpt theLrCorner;
   ossimDpt theUrCorner;
};

inline const ossimVrect& ossimVrect::operator=(const ossimVrect& rect)
{
   if (this != &rect)
   {
      theUlCorner = rect.ul();
      theUrCorner = rect.ur();
      theLrCorner = rect.lr();
      theLlCorner = rect.ll();
   }

   return *this;
}

inline bool ossimVrect::operator!=(const ossimVrect& rect) const
{
   return ( (theUlCorner != rect.ul()) ||
            (theUrCorner != rect.ur()) ||
            (theLrCorner != rect.lr()) ||
            (theLlCorner != rect.ll()) );
}

inline bool ossimVrect::operator==(const ossimVrect& rect) const
{
   return ( (theUlCorner == rect.ul()) &&
            (theUrCorner == rect.ur()) &&
            (theLrCorner == rect.lr()) &&
            (theLlCorner == rect.ll()) );
}


inline ossimVrect ossimVrect::clipToRect(const ossimVrect& rect)const
{
    ossim_float64     ulx, uly, lrx, lry;

    // XXX not replaced with std::max or ossim::max since the test is backward 
    //     here and will give a different answer in the case of nan.
    #define d_MAX(a,b)      (((a)>(b)) ? a : b)

    ulx = d_MAX(rect.ul().x,ul().x);
    uly = d_MAX(rect.ul().y,ul().y);

    #undef d_MAX

    lrx = std::min(rect.lr().x,lr().x);
    lry = std::min(rect.lr().y,lr().y);

    if( lrx <= ulx || lry <= uly )
        return ossimVrect(ossimDpt(0,0),ossimDpt(0,0));
    else
       return ossimVrect(ulx,uly,lrx,lry);
}

inline ossimDpt ossimVrect::midPoint()const
{
   return ossimDpt( (ul().x + ur().x + ll().x + lr().x)*.25,
               (ul().y + ur().y + ll().y + lr().y)*.25);
}

inline ossimVrect ossimVrect::combine(const ossimVrect& rect)const
{
   ossimDpt ulCombine;
   ossimDpt lrCombine;
   
   ulCombine.x = ((ul().x <= rect.ul().x)?ul().x:rect.ul().x);
   ulCombine.y = ((ul().y >= rect.ul().y)?ul().y:rect.ul().y);
   lrCombine.x = ((lr().x >= rect.lr().x)?lr().x:rect.lr().x);
   lrCombine.y = ((lr().y <= rect.lr().y)?lr().y:rect.lr().y);

   return ossimVrect(ulCombine, lrCombine);
}

#endif
