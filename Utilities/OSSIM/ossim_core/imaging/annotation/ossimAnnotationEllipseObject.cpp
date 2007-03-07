//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See LICENSE.txt file in the top level directory.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimAnnotationEllipseObject.cpp,v 1.9 2005/12/07 19:08:55 dburken Exp $
#include <imaging/annotation/ossimAnnotationEllipseObject.h>
#include <imaging/ossimRgbImage.h>
#include <base/common/ossimCommon.h>

RTTI_DEF1(ossimAnnotationEllipseObject,
          "ossimAnnotationEllipseObject",
          ossimAnnotationObject)
   
ossimAnnotationEllipseObject::ossimAnnotationEllipseObject(
   const ossimDpt& center,
   const ossimDpt& widthHeight,
   bool enableFill,
   ossim_uint8 r,
   ossim_uint8 g,
   ossim_uint8 b,
   ossim_uint8 thickness)
   : ossimAnnotationObject(r, g, b, thickness),
     theCenter(center),
     theWidthHeight(widthHeight),
     theFillEnabled(enableFill),
     theBoundingRect(0,0,0,0)
{
   computeBoundingRect();
}

ossimAnnotationEllipseObject::ossimAnnotationEllipseObject(
   const ossimAnnotationEllipseObject& rhs)
   :
   theCenter(rhs.theCenter),
   theWidthHeight(rhs.theWidthHeight),
   theFillEnabled(rhs.theFillEnabled),
   theBoundingRect(rhs.theBoundingRect)
{
}

ossimObject* ossimAnnotationEllipseObject::dup()const
{
   return new ossimAnnotationEllipseObject(*this);
}

ossimAnnotationEllipseObject::~ossimAnnotationEllipseObject()
{
}

void ossimAnnotationEllipseObject::applyScale(double x, double y)
{
   theCenter.x      *= x;
   theCenter.y      *= y;
   theWidthHeight.x *= x;
   theWidthHeight.y *= y;
   theBoundingRect  *= ossimDpt(x,y);
}



void ossimAnnotationEllipseObject::draw(ossimRgbImage& anImage)const
{
   anImage.setDrawColor(theRed, theGreen, theBlue);
   anImage.setThickness(theThickness);

   if(theBoundingRect.intersects(anImage.getImageData()->getImageRectangle()))
   {
      if(theFillEnabled)
      {
//         anImage.drawFilledArc(irint(theCenter.x),
//                               irint(theCenter.y),
//                               irint(theWidthHeight.x),
//                               irint(theWidthHeight.y),
//                               0,
//                               360);
         anImage.drawFilledArc((int)(theCenter.x),
                               (int)(theCenter.y),
                               (int)(theWidthHeight.x),
                               (int)(theWidthHeight.y),
                               0,
                               360);
      }
      else
      {
//         anImage.drawArc(irint(theCenter.x),
//                         irint(theCenter.y),
//                         irint(theWidthHeight.x),
//                         irint(theWidthHeight.y),
//                         0,
//                         360);
         anImage.drawArc((int)(theCenter.x),
                         (int)(theCenter.y),
                         (int)(theWidthHeight.x),
                         (int)(theWidthHeight.y),
                         0,
                         360);
      }
   }
}

bool ossimAnnotationEllipseObject::intersects(const ossimDrect& rect)const
{
   return rect.intersects(theBoundingRect);
}

ossimAnnotationObject* ossimAnnotationEllipseObject::getNewClippedObject(
   const ossimDrect& rect)const
{
   if(intersects(rect))
   {
      return (ossimAnnotationObject*)dup();
   }
   
   return (ossimAnnotationEllipseObject*)NULL;
}

std::ostream& ossimAnnotationEllipseObject::print(std::ostream& out)const
{
   ossimAnnotationObject::print(out);
   out << endl;
   out << "ellipse_center:        " << theCenter << endl
       << "ellipse_height:        " << theWidthHeight.y << endl
       << "ellipse_width:         " << theWidthHeight.x << endl
       << "fill_enabled:          " << theFillEnabled << endl
       << "ellipse_bounding_rect: " << theBoundingRect;
   return out;
}

void ossimAnnotationEllipseObject::getBoundingRect(ossimDrect& rect)const
{
   rect = theBoundingRect;
}

void ossimAnnotationEllipseObject::computeBoundingRect()
{
   theBoundingRect = ossimDrect(theCenter.x - theWidthHeight.x,
                                theCenter.y - theWidthHeight.y,
                                theCenter.x + theWidthHeight.x,
                                theCenter.y + theWidthHeight.y);
}

bool ossimAnnotationEllipseObject::isPointWithin(const ossimDpt& point)const
{
  return theBoundingRect.pointWithin(point);
}

void ossimAnnotationEllipseObject::setCenterWidthHeight(const ossimDpt& center,
                                                        const ossimDpt& widthHeight)
{
   theCenter          = center;
   theWidthHeight     = widthHeight;

   computeBoundingRect();
}

void ossimAnnotationEllipseObject::setFill(bool enabled)
{
   theFillEnabled = enabled;
}

bool ossimAnnotationEllipseObject::getFill() const
{
   return theFillEnabled;
}

bool ossimAnnotationEllipseObject::saveState(ossimKeywordlist& kwl,
                                             const char* prefix)const
{
   return ossimAnnotationObject::saveState(kwl, prefix);
}

bool ossimAnnotationEllipseObject::loadState(const ossimKeywordlist& kwl,
                                                const char* prefix)
{
   return ossimAnnotationObject::loadState(kwl, prefix);
}
