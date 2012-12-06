//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description:
//
// Contains class definition for ossimIrect.
// 
//*******************************************************************
//  $Id: ossimIrect.cpp 21560 2012-08-30 12:09:03Z gpotts $

#include <ostream>
#include <sstream>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimDrect.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimKeywordNames.h>

// nonstandard versions that use operator>, so they behave differently
// than std:::min/max and ossim::min/max.  kept here for now for that
// reason.
#ifndef MAX
#  define MAX(x,y) ((x)>(y)?(x):(y))
#  define MIN(x,y) ((x)>(y)?(y):(x))
#endif

ossimIrect::ossimIrect(const ossimDrect& rect)
   :
      theUlCorner(rect.ul()),
      theUrCorner(rect.ur()),
      theLrCorner(rect.lr()),
      theLlCorner(rect.ll()),
      theOrientMode(rect.orientMode())
{}

ossimIrect::ossimIrect(const std::vector<ossimIpt>& points,
                       ossimCoordSysOrientMode mode)
   :
      theOrientMode (mode)
{
   if(points.size())
   {
      ossim_uint32 index;
      ossim_int32  minx = points[0].x;
      ossim_int32  miny = points[0].y;
      ossim_int32  maxx = points[0].x;
      ossim_int32  maxy = points[0].y;
            
      // find the bounds
      for(index = 1; index < points.size();index++)
      {
         minx = ossim::min(minx, points[index].x);
         miny = ossim::min(miny, points[index].y);
         maxx = ossim::max(maxx, points[index].x);
         maxy = ossim::max(maxy, points[index].y);
         
      }
      if(theOrientMode == OSSIM_LEFT_HANDED)
      {
         *this = ossimIrect(minx, miny, maxx, maxy, mode);
      }
      else
      {
         *this = ossimIrect(minx,maxy, maxx, miny, mode);
      }
   }
   else
   {
      makeNan();
   }
}

ossimIrect::ossimIrect(const ossimIpt& p1,
                       const ossimIpt& p2,
                       const ossimIpt& p3,
                       const ossimIpt& p4,
                       ossimCoordSysOrientMode mode)
   :theOrientMode(mode)
{
   if(p1.hasNans()||p2.hasNans()||p3.hasNans()||p4.hasNans())
   {
      makeNan();
   }
   else
   {
      ossim_int32 minx, miny;
      ossim_int32 maxx, maxy;
      
      minx = ossim::min( p1.x, ossim::min(p2.x, ossim::min(p3.x, p4.x)));
      miny = ossim::min( p1.y, ossim::min(p2.y, ossim::min(p3.y, p4.y)));
      maxx = ossim::max( p1.x, ossim::max(p2.x, ossim::max(p3.x, p4.x)));
      maxy = ossim::max( p1.y, ossim::max(p2.y, ossim::max(p3.y, p4.y)));
      
      if(theOrientMode == OSSIM_LEFT_HANDED)
      {
         *this = ossimIrect(minx, miny, maxx, maxy, mode);
      }
      else
      {
         *this = ossimIrect(minx,maxy, maxx, miny, mode);
      }
   }
}

ossimIrect::~ossimIrect()
{
}

//*******************************************************************
//! Constructs an Irect surrounding the specified point, and of specified size.
//*******************************************************************
ossimIrect::ossimIrect(const ossimIpt& center, 
                       ossim_uint32    size_x, 
                       ossim_uint32    size_y,
                       ossimCoordSysOrientMode mode)
: theOrientMode (mode)
{
   ossim_int32 minx = center.x - size_x/2;
   ossim_int32 maxx = minx + size_x - 1;

   ossim_int32 miny = center.y - size_y/2;
   ossim_int32 maxy = miny + size_y - 1;

   if(mode == OSSIM_LEFT_HANDED)
      *this = ossimIrect(minx, miny, maxx, maxy, mode);
   else
      *this = ossimIrect(minx,maxy, maxx, miny, mode);
}


//*************************************************************************************************
//! Guarantees that this rect will be at least w X h big. If smaller than specified, the 
//! corresponding side will be stretched equally in + and - direction to meet required size.
//! Returns TRUE if resizing occurred.
//*************************************************************************************************
bool ossimIrect::insureMinimumSize(const ossimIpt& width_height)
{
   ossimIpt ul (theUlCorner);
   ossimIpt lr (theLrCorner);
   bool resized = false;

   int dx = width_height.x - width();
   int dy = width_height.y - height();

   if (dx > 0)
   {
      dx = (int) ceil((double)dx/2.0);
      ul.x -= dx;
      lr.x += dx;
      resized = true;
   }

   if (dy > 0)
   {
      resized = true;
      if(theOrientMode == OSSIM_LEFT_HANDED)
      {
         dy = (int) ceil((double)dy/2.0);
         ul.y -= dy;
         lr.y += dy;
      }
      else
      {
         dy = (int) ceil((double)dy/2.0);
         ul.y += dy;
         lr.y -= dy;
      }
   }

   if (resized)
      *this = ossimIrect(ul, lr, theOrientMode);

   return resized;
}

//*******************************************************************
// Public Method:
//*******************************************************************
bool ossimIrect::intersects(const ossimIrect& rect) const
{
   if(rect.hasNans() || hasNans())
   {
      return false;
   }
   if (theOrientMode != rect.theOrientMode)
      return false;
   
   ossim_int32  ulx = ossim::max(rect.ul().x,ul().x);
   ossim_int32  lrx = ossim::min(rect.lr().x,lr().x);
   ossim_int32  uly, lry;
   bool rtn=false;
   if (theOrientMode == OSSIM_LEFT_HANDED)
   {
      uly  = ossim::max(rect.ul().y,ul().y);
      lry  = ossim::min(rect.lr().y,lr().y);
      rtn = ((ulx <= lrx) && (uly <= lry));
   }
   else
   {
      uly  = ossim::max(rect.ll().y,ll().y);
      lry  = ossim::min(rect.ur().y,ur().y);
      rtn = ((ulx <= lrx) && (uly <= lry));
   }
   
   return (rtn);
}

void ossimIrect::stretchToTileBoundary(const ossimIpt& tileWidthHeight)
{
   ossimIpt ul;
   ossimIpt lr;

   if(theOrientMode == OSSIM_LEFT_HANDED)
   {
      ul.x = theUlCorner.x;
      if( (theUlCorner.x % tileWidthHeight.x) != 0)
      {
         ul.x = ((ul.x / tileWidthHeight.x))*tileWidthHeight.x;
         if(ul.x > theUlCorner.x)
         {
            ul.x -= tileWidthHeight.x;
         }
      }
      ul.y = theUlCorner.y;
      if( (theUlCorner.y % tileWidthHeight.y) != 0)
      {
         ul.y = ((ul.y / tileWidthHeight.y))*tileWidthHeight.y;
         if(ul.y > theUlCorner.y)
         {
            ul.y -= tileWidthHeight.y;
         }
      }
      ossim_int32 w = (theLrCorner.x - ul.x) + 1;
      ossim_int32 h = (theLrCorner.y - ul.y) + 1;
      
      ossim_int32 nw = (w / tileWidthHeight.x)*tileWidthHeight.x;
      ossim_int32 nh = (h / tileWidthHeight.y)*tileWidthHeight.y;
      
      if(w%tileWidthHeight.x)
      {
         nw += tileWidthHeight.x;
      }
      if(h%tileWidthHeight.y)
      {
         nh += tileWidthHeight.y;
      }
           
      lr.x = ul.x + (nw-1);
      lr.y = ul.y + (nh-1);
   }
   else
   {
      ul.x = theUlCorner.x;
      ul.y = theUlCorner.y;
      if( (theUlCorner.x%tileWidthHeight.x)!= 0)
      {
         ul.x = ((ul.x/ tileWidthHeight.x))*tileWidthHeight.x;
         if(ul.x > theUlCorner.x)
         {
            ul.x -= tileWidthHeight.x;
         }
      }
      if( (theUlCorner.y%tileWidthHeight.y)!=0 )
      {
         ul.y = ((ul.y / tileWidthHeight.y))*tileWidthHeight.y;
         if(ul.y < theUlCorner.y)
         {
            ul.y += tileWidthHeight.y;
         }
      }
      ossim_int32 w = theLrCorner.x - ul.x;
      if (w < 0)
      {
         w = -w;
      }
      w += 1;
      ossim_int32 h = theLrCorner.y - ul.y;
      if (h < 0)
      {
         h = -h;
      }
      h += 1;

      ossim_int32 nw = (w / tileWidthHeight.x)*tileWidthHeight.x;
      ossim_int32 nh = (h / tileWidthHeight.y)*tileWidthHeight.y;
      
      if(w%tileWidthHeight.x)
      {
         nw += tileWidthHeight.x;
      }
      if(h%tileWidthHeight.y)
      {
         nh += tileWidthHeight.y;
      }
           
      lr.x = ul.x + (nw-1);
      lr.y = ul.y - (nh-1);
  }

   *this = ossimIrect(ul, lr, theOrientMode);
}


const ossimIrect& ossimIrect::expand(const ossimIpt& padding)
{
   theUlCorner.x -= padding.x;
   theUrCorner.x += padding.x;
   theLrCorner.x += padding.x;
   theLlCorner.x -= padding.x;
   if(theOrientMode == OSSIM_LEFT_HANDED)
   {
      theUlCorner.y -= padding.y;
      theUrCorner.y -= padding.y;
      theLrCorner.y += padding.y;
      theLlCorner.y += padding.y;
   }
   else
   {
      theUlCorner.y += padding.y;
      theUrCorner.y += padding.y;
      theLrCorner.y -= padding.y;
      theLlCorner.y -= padding.y;
   }
   
   return *this;
}

ossimString ossimIrect::toString()const
{
   ossimString result="(";
   
   if(theOrientMode == OSSIM_LEFT_HANDED)
   {
      ossimIpt origin = ul();
      result += (ossimString::toString(origin.x) + ",");
      result += (ossimString::toString(origin.y) + ",");
      result += (ossimString::toString(width()) + ",");
      result += (ossimString::toString(height()) + ",");
      result += "LH";
   }
   else 
   {
      ossimIpt origin = ll();
      result += (ossimString::toString(origin.x) + ",");
      result += (ossimString::toString(origin.y) + ",");
      result += (ossimString::toString(width()) + ",");
      result += (ossimString::toString(height()) + ",");
      result += "RH";
   }

   result += ")";
   return result;
}

bool ossimIrect::toRect(const ossimString& rectString)
{
   bool result = false;
   makeNan();
   
   std::istringstream in(rectString);
   ossim::skipws(in);
   char charString[2];
   charString[1] = '\0';
   ossimString interior;
   if(in.peek() == '(')
   {
      in.ignore();
      while((in.peek() != ')')&&
            (in.peek() != '\n') &&
            in.good())
      {
         charString[0] = in.get();
         interior += charString;
      }
      if(in.peek() == ')')
      {
         result = true;
      }
   }
   if(result)
   {
      std::vector<ossimString> splitArray;
      interior.split(splitArray, ",");
      
      // assume left handed
      if(splitArray.size() >= 4)
      {
         ossim_int64 x = splitArray[0].toInt64();
         ossim_int64 y = splitArray[1].toInt64();
         ossim_int64 w = splitArray[2].toInt64();
         ossim_int64 h = splitArray[3].toInt64();
         ossimString orientation = "lh";
         if(splitArray.size() == 5)
         {
            orientation = splitArray[4].downcase();
         }
         if(orientation == "lh")
         {
            // origin upper left
            *this = ossimIrect(x,y,x + (w-1), y+h-1, OSSIM_LEFT_HANDED);
         }
         else 
         {
            // origin lower left
            *this = ossimIrect(x,y+(h-1),x + (w-1), y, OSSIM_RIGHT_HANDED);
         }
         
      }
      else
      {
         result = false;
      }

   }
   return result;
}

//*******************************************************************
// Public Method:
//*******************************************************************
bool ossimIrect::completely_within(const ossimIrect& rect) const
{
   if(rect.hasNans() || hasNans())
   {
      return false;
   }
   
   if (theOrientMode != rect.theOrientMode)
      return false;
   
   /*  --------------
       |     1      |
       | ---------- |
       | |        | |
       | |        | |
       | |   2    | |
       | |        | |
       | |        | |
       | ---------- |
       |            |
       --------------  */

   bool rtn = true;
   
   if (theUlCorner.x < rect.ul().x)
      rtn = false;
   
   else if (theLrCorner.x > rect.lr().x)
      rtn = false;
   
   else if (theOrientMode == OSSIM_LEFT_HANDED)
   {
      if (theUlCorner.y < rect.ul().y)
         rtn = false;
   
      else if (theLrCorner.y > rect.lr().y)
         rtn = false;
   }
   
   else
   {
      if (theUlCorner.y > rect.ul().y)
         rtn = false;
   
      else if (theLrCorner.y < rect.lr().y)
         rtn = false;
   }

   return rtn;
}

//*******************************************************************
// Public Method:
//*******************************************************************
void ossimIrect::print(std::ostream& os) const
{
   os << toString();
}

//*******************************************************************
// friend function: operator<<
//*******************************************************************
std::ostream& operator<<(std::ostream& os, const ossimIrect& rect)
{
   rect.print(os);

   return os;
}

//*******************************************************************
//  Method: ossimIrect::clipToRect
//*******************************************************************
ossimIrect ossimIrect::clipToRect(const ossimIrect& rect)const
{   
   if (theOrientMode != rect.theOrientMode)
      return (*this);

   int x0 = MAX(rect.ul().x, ul().x);
   int x1 = MIN(rect.lr().x, lr().x);
   int y0, y1;

   if(!this->intersects(rect))
   {
      return ossimIrect(OSSIM_INT_NAN,
         OSSIM_INT_NAN,
         OSSIM_INT_NAN,
         OSSIM_INT_NAN);

   }
   if (theOrientMode == OSSIM_LEFT_HANDED)
   {
      y0 = MAX(rect.ul().y, ul().y);
      y1 = MIN(rect.lr().y, lr().y);

      if( (x1 < x0) || (y1 < y0) )
         return ossimIrect(ossimIpt(0,0), ossimIpt(0,0), theOrientMode);
      else
         return ossimIrect(x0, y0, x1, y1, theOrientMode);
   }
   else
   {
      y1 = MIN(rect.ul().y,ul().y);
      y0 = MAX(rect.lr().y,lr().y);

      if((x1 < x0) || (y1 < y0))
         return ossimIrect(ossimIpt(0,0), ossimIpt(0,0), theOrientMode);
      else
         return ossimIrect(x0, y1, x1, y0, theOrientMode);
   }
}

//*******************************************************************
//  Returns the minimum bounding rect that includes this and arg rect.
//*******************************************************************
ossimIrect ossimIrect::combine(const ossimIrect& rect) const
{   
   // If any rect has NANs, it is assumed uninitialized, so assign the result to just the other
   if (hasNans()) 
      return rect;
   if(rect.hasNans())
      return *this;

   if (theOrientMode != rect.theOrientMode)
      return(*this);

   ossimIpt ulCombine;
   ossimIpt lrCombine;

   if(theOrientMode == OSSIM_LEFT_HANDED)
   {
      ulCombine.x = ((ul().x <= rect.ul().x)?ul().x:rect.ul().x);
      ulCombine.y = ((ul().y <= rect.ul().y)?ul().y:rect.ul().y);
      lrCombine.x = ((lr().x >= rect.lr().x)?lr().x:rect.lr().x);
      lrCombine.y = ((lr().y >= rect.lr().y)?lr().y:rect.lr().y);
   }
   else
   {
      ulCombine.x = ((ul().x <= rect.ul().x)?ul().x:rect.ul().x);
      ulCombine.y = ((ul().y >= rect.ul().y)?ul().y:rect.ul().y);
      lrCombine.x = ((lr().x >= rect.lr().x)?lr().x:rect.lr().x);
      lrCombine.y = ((lr().y <= rect.lr().y)?lr().y:rect.lr().y);
   }

   return ossimIrect(ulCombine, lrCombine, theOrientMode);
}

//*******************************************************************
// Inline Method: ossimIrect::operator=(const ossimDrect& rect)
//*******************************************************************
const ossimIrect& ossimIrect::operator=(const ossimDrect& rect)
{
   theUlCorner   = rect.ul();
   theUrCorner   = rect.ur();
   theLrCorner   = rect.lr();
   theLlCorner   = rect.ll();
   theOrientMode = rect.orientMode();
   
   return *this;
}

bool ossimIrect::saveState(ossimKeywordlist& kwl,
                           const char* prefix)const
{
   kwl.add(prefix,
           ossimKeywordNames::TYPE_KW,
           "ossimIrect",
           true);

   kwl.add(prefix, "rect", toString());
 #if 0  
   if(hasNans())
   {
      kwl.add(prefix,
              "ul_x",
              "nan",
              true);
      kwl.add(prefix,
              "ul_y",
              "nan",
              true);
      kwl.add(prefix,
              "lr_x",
              "nan",
              true);
      kwl.add(prefix,
              "lr_y",
              "nan",
              true);
   }
   else
   {
      kwl.add(prefix,
              "ul_x",
              theUlCorner.x,
              true);
      kwl.add(prefix,
              "ul_y",
              theUlCorner.y,
              true);
      kwl.add(prefix,
              "lr_x",
              theLrCorner.x,
              true);
      kwl.add(prefix,
              "lr_y",
              theLrCorner.y,
              true);
   }
#endif
   return true;
}

bool ossimIrect::loadState(const ossimKeywordlist& kwl,
                           const char* prefix)
{
  makeNan();
  const char* ulx = kwl.find(prefix, "ul_x");
  const char* uly = kwl.find(prefix, "ul_y");
  const char* lrx = kwl.find(prefix, "lr_x");
  const char* lry = kwl.find(prefix, "lr_y");
  const char* rect = kwl.find(prefix, "rect");

  if(ulx&&uly&&lrx&&lry)
  {
    if( (ossimString(ulx).trim().upcase() != "NAN") &&
        (ossimString(uly).trim().upcase() != "NAN") &&
        (ossimString(lrx).trim().upcase() != "NAN") &&
        (ossimString(lry).trim().upcase() != "NAN"))
    {
      *this = ossimIrect(ossimString(ulx).toInt32(),
                         ossimString(uly).toInt32(),
                         ossimString(lrx).toInt32(),
                         ossimString(lry).toInt32());
    }
  }
  else if(rect)
  {
      toRect(rect);
  }
   
   return true;
}

void ossimIrect::getCenter(ossimDpt& center_point) const
{
   if (hasNans())
   {
      center_point.makeNan();
      return;
   }

   double d = (theUlCorner.x + theUrCorner.x + theLrCorner.x + theLlCorner.x);
   center_point.x = d / 4.0;

   d = (theUlCorner.y + theUrCorner.y + theLrCorner.y + theLlCorner.y);
   center_point.y = d / 4.0;
}
