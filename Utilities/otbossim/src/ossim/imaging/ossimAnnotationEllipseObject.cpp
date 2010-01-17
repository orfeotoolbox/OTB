//*******************************************************************
//
// License:  See LICENSE.txt file in the top level directory.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimAnnotationEllipseObject.cpp 13964 2009-01-14 16:30:07Z gpotts $
#include <ossim/imaging/ossimAnnotationEllipseObject.h>
#include <ossim/imaging/ossimRgbImage.h>
#include <ossim/base/ossimCommon.h>

RTTI_DEF1(ossimAnnotationEllipseObject,
          "ossimAnnotationEllipseObject",
          ossimAnnotationObject)
   
ossimAnnotationEllipseObject::ossimAnnotationEllipseObject(
   const ossimDpt& center,
   const ossimDpt& widthHeight,
   ossim_float64 azimuthInDegrees,
   bool enableFill,
   ossim_uint8 r,
   ossim_uint8 g,
   ossim_uint8 b,
   ossim_uint8 thickness)
   : ossimAnnotationObject(r, g, b, thickness),
     theCenter(center),
     theWidthHeight(widthHeight),
     theAzimuthInDegrees(azimuthInDegrees),
     theFillEnabled(enableFill),
     theDrawAxesFlag(false),
     theBoundingRect(0,0,0,0)
{
   computeBoundingRect();
}

ossimAnnotationEllipseObject::ossimAnnotationEllipseObject(
   const ossimAnnotationEllipseObject& rhs)
   :ossimAnnotationObject(rhs),
   theCenter(rhs.theCenter),
   theWidthHeight(rhs.theWidthHeight),
   theAzimuthInDegrees(rhs.theAzimuthInDegrees),
   theFillEnabled(rhs.theFillEnabled),
   theDrawAxesFlag(rhs.theDrawAxesFlag),
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
   computeBoundingRect();
}



void ossimAnnotationEllipseObject::draw(ossimRgbImage& anImage)const
{
   anImage.setDrawColor(theRed, theGreen, theBlue);
   anImage.setThickness(theThickness);

   if(theBoundingRect.intersects(anImage.getImageData()->getImageRectangle()))
   {
      if(theFillEnabled)
      {
         anImage.drawFilledEllipse((int)(theCenter.x),
                                   (int)(theCenter.y),
                                   (int)(theWidthHeight.x),
                                   (int)(theWidthHeight.y),
                                   theAzimuthInDegrees*RAD_PER_DEG);
      }
      else
      {
         anImage.drawEllipse((int)(theCenter.x),
                             (int)(theCenter.y),
                             (int)(theWidthHeight.x),
                             (int)(theWidthHeight.y),
                             theAzimuthInDegrees*RAD_PER_DEG,
                             theDrawAxesFlag); 
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
   out << "ellipse_center:        " << theCenter
       << "\nellipse_height:        " << theWidthHeight.y
       << "\nellipse_width:         " << theWidthHeight.x
       << "\nfill_enabled:          " << theFillEnabled
       << "\ndraw_axes:             " << theDrawAxesFlag
       << "\nellipse_bounding_rect: " << theBoundingRect;
   return out;
}

void ossimAnnotationEllipseObject::getBoundingRect(ossimDrect& rect)const
{
   rect = theBoundingRect;
}

void ossimAnnotationEllipseObject::computeBoundingRect()
{
   ossim_float64 maxDimension =
      ossim::max(theWidthHeight.x, theWidthHeight.y)/2.0 + 1.0;
   theBoundingRect = ossimDrect(theCenter.x - maxDimension,
                                theCenter.y - maxDimension,
                                theCenter.x + maxDimension,
                                theCenter.y + maxDimension);
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

void ossimAnnotationEllipseObject::setAzimuth(ossim_float64 azimuth)
{
   theAzimuthInDegrees = azimuth;
}
   
ossim_float64 ossimAnnotationEllipseObject::getAzimuth() const
{
   return theAzimuthInDegrees;
}

void ossimAnnotationEllipseObject::setFillFlag(bool enabled)
{
   theFillEnabled = enabled;
}

bool ossimAnnotationEllipseObject::getFillFlag() const
{
   return theFillEnabled;
}

void ossimAnnotationEllipseObject::setDrawAxesFlag(bool flag)
{
   theDrawAxesFlag = flag;
}

bool ossimAnnotationEllipseObject::getDrawAxesFlag() const
{
   return theDrawAxesFlag;
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
