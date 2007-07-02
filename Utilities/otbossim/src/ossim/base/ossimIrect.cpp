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
//  $Id: ossimIrect.cpp 9094 2006-06-13 19:12:40Z dburken $

#include <ostream>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimDrect.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimKeywordNames.h>

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
         minx = ossimMin(minx, points[index].x);
         miny = ossimMin(miny, points[index].y);
         maxx = ossimMax(maxx, points[index].x);
         maxy = ossimMax(maxy, points[index].y);
         
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
      
      minx = ossimMin( p1.x, ossimMin(p2.x, ossimMin(p3.x, p4.x)));
      miny = ossimMin( p1.y, ossimMin(p2.y, ossimMin(p3.y, p4.y)));
      maxx = ossimMax( p1.x, ossimMax(p2.x, ossimMax(p3.x, p4.x)));
      maxy = ossimMax( p1.y, ossimMax(p2.y, ossimMax(p3.y, p4.y)));
      
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
   
   ossim_int32 ulx = MAX(rect.ul().x,ul().x);
   ossim_int32 lrx = MIN(rect.lr().x,lr().x);
   ossim_int32 uly, lry;
   bool   rtn;
   
   if (theOrientMode == OSSIM_LEFT_HANDED)
   {
      uly = MAX(rect.ul().y,ul().y);
      lry = MIN(rect.lr().y,lr().y);
      rtn = ((ulx <= lrx) && (uly <= lry));
   }
   else
   {
      uly = MIN(rect.ul().y,ul().y);
      lry = MAX(rect.lr().y,lr().y);
      rtn = ((ulx <= lrx) && (uly >= lry));
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
      ossim_int32 w = ossimAbs(theLrCorner.x - ul.x) + 1;
      ossim_int32 h = ossimAbs(theLrCorner.y - ul.y) + 1;

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
   os << theUlCorner << theLrCorner;
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

   return true;
}

bool ossimIrect::loadState(const ossimKeywordlist& kwl,
                           const char* prefix)
{
   const char* ulx = kwl.find(prefix, "ul_x");
   const char* uly = kwl.find(prefix, "ul_y");
   const char* lrx = kwl.find(prefix, "lr_x");
   const char* lry = kwl.find(prefix, "lr_y");

   if(ulx&&uly&&lrx&&lry)
   {
      if( (ossimString(ulx).trim().upcase() == "NAN") ||
          (ossimString(uly).trim().upcase() == "NAN") ||
          (ossimString(lrx).trim().upcase() == "NAN") ||
          (ossimString(lry).trim().upcase() == "NAN"))
      {
         makeNan();
      }
      else
      {
         *this = ossimIrect(ossimString(ulx).toInt32(),
                            ossimString(uly).toInt32(),
                            ossimString(lrx).toInt32(),
                            ossimString(lry).toInt32());
      }
   }
   else
   {
      makeNan();
      
      return false;
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
