//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimGeoAnnotationEllipseObject.cpp 15766 2009-10-20 12:37:09Z gpotts $

#include <sstream>

#include <ossim/imaging/ossimGeoAnnotationEllipseObject.h>
#include <ossim/projection/ossimProjection.h>
#include <ossim/projection/ossimImageProjectionModel.h>
#include <ossim/base/ossimException.h>
#include <ossim/base/ossimUnitTypeLut.h>
#include <ossim/base/ossimUnitConversionTool.h>

RTTI_DEF1(ossimGeoAnnotationEllipseObject,
          "ossimGeoAnnotationEllipseObject",
          ossimGeoAnnotationObject);
   
ossimGeoAnnotationEllipseObject::ossimGeoAnnotationEllipseObject(
   const ossimGpt& center,
   const ossimDpt& widthHeight,
   bool enableFill,
   ossim_uint8 r,
   ossim_uint8 g,
   ossim_uint8 b,
   ossim_uint8 thickness)
   :ossimGeoAnnotationObject(r, g, b, thickness),
    theProjectedEllipse(0),
    theCenter(center),
    theWidthHeight(widthHeight),
    theEllipseWidthHeightUnitType(OSSIM_PIXEL) // default to image space
{
   theProjectedEllipse = new ossimAnnotationEllipseObject(ossimDpt(0,0),
							  ossimDpt(0,0),
							  enableFill,
							  r,
							  g,
							  b,
							  thickness);
}

ossimGeoAnnotationEllipseObject::ossimGeoAnnotationEllipseObject(
   const ossimGeoAnnotationEllipseObject& rhs)
   :ossimGeoAnnotationObject(rhs),
    theProjectedEllipse(rhs.theProjectedEllipse.valid()?(ossimAnnotationEllipseObject*)rhs.theProjectedEllipse->dup():(ossimAnnotationEllipseObject*)0),
    theCenter(rhs.theCenter),
    theWidthHeight(rhs.theWidthHeight),
    theEllipseWidthHeightUnitType(rhs.theEllipseWidthHeightUnitType)
{
}

ossimGeoAnnotationEllipseObject::~ossimGeoAnnotationEllipseObject()
{
   if(theProjectedEllipse.valid())
   {
      theProjectedEllipse = 0;
   }
}

ossimObject* ossimGeoAnnotationEllipseObject::dup()const
{
   return new ossimGeoAnnotationEllipseObject(*this);
}

void ossimGeoAnnotationEllipseObject::applyScale(double x,
                                                 double y)
{
   if(theProjectedEllipse.valid()) theProjectedEllipse->applyScale(x, y);
   theCenter.lond(theCenter.lond()*x);
   theCenter.latd(theCenter.latd()*y);
   theWidthHeight .x *= x;
   theWidthHeight .y *= y;
}

std::ostream& ossimGeoAnnotationEllipseObject::print(std::ostream& out)const
{
   out << "center ground:                  " <<  theCenter << endl
       << "width height image space flag = " << theEllipseWidthHeightUnitType
       << endl
       << *theProjectedEllipse;
   return out;
}

void ossimGeoAnnotationEllipseObject::draw(ossimRgbImage& anImage)const
{
   if(theProjectedEllipse.valid())
   {
      theProjectedEllipse->draw(anImage);
   }
}

bool ossimGeoAnnotationEllipseObject::intersects(const ossimDrect& rect)const
{
   if(theProjectedEllipse.valid())
   {
      theProjectedEllipse->intersects(rect);
   }

   return false;
}

ossimAnnotationObject* ossimGeoAnnotationEllipseObject::getNewClippedObject(const ossimDrect& rect)const
{
   if(intersects(rect))
   {
      if(theProjectedEllipse.valid())
      {
         return theProjectedEllipse->getNewClippedObject(rect);
      }
   }
   
   return (ossimAnnotationObject*)0;
}

void ossimGeoAnnotationEllipseObject::getBoundingRect(ossimDrect& rect)const
{
   if(theProjectedEllipse.valid())
   {
      theProjectedEllipse->getBoundingRect(rect);
   }
   else
   {
      rect.makeNan();
   }
}

void ossimGeoAnnotationEllipseObject::computeBoundingRect()
{
   if(theProjectedEllipse.valid())
   {
      theProjectedEllipse->computeBoundingRect();
   }
}

void ossimGeoAnnotationEllipseObject::transform(ossimImageGeometry* projection)
{
   if(!projection)
   {
      return;
   }

   // Ellipse center, height and width in image space.
   ossimDpt projectedCenter;
   ossimDpt projectedWidthHeight;

   // first get the center projected
   projection->worldToLocal(theCenter, projectedCenter);

   getWidthHeightInPixels(projectedWidthHeight, projection);

   theProjectedEllipse->setCenterWidthHeight(projectedCenter,
                                             projectedWidthHeight);      
}

bool ossimGeoAnnotationEllipseObject::saveState(ossimKeywordlist& kwl,
                                                const char* prefix) const
{
   kwl.add(prefix, "center", theCenter.toString().c_str());
   kwl.add(prefix, "ellipse_width", theWidthHeight.x);
   kwl.add(prefix, "ellipse_height", theWidthHeight.y);
   kwl.add(prefix, "azimuth", getAzimuth());
   kwl.add(prefix, "fill", ossimString::toString(getFillFlag()));
   kwl.add(prefix, "draw_axes", ossimString::toString(getDrawAxesFlag()));
   kwl.add(prefix, "units",
           ossimUnitTypeLut::instance()->getEntryString(theEllipseWidthHeightUnitType).c_str());
   
   return ossimGeoAnnotationObject::saveState(kwl, prefix);
}

bool ossimGeoAnnotationEllipseObject::loadState(const ossimKeywordlist& kwl,
                                                const char* prefix)
{
   if (!theProjectedEllipse)
   {
      return false;
   }
   
   //---
   // Base class state must be called first to pick up colors...
   //---
   bool status =  ossimGeoAnnotationObject::loadState(kwl, prefix);
   if (status == true)
   {
      theProjectedEllipse->setColor(theRed, theGreen, theBlue);
      theProjectedEllipse->setThickness(theThickness);
   }

   const char* center         = kwl.find(prefix, "center");
   const char* ellipse_width  = kwl.find(prefix, "ellipse_width");
   const char* ellipse_height = kwl.find(prefix, "ellipse_height");
   const char* azimuth        = kwl.find(prefix, "azimuth");
   const char* fill           = kwl.find(prefix, "fill");
   const char* draw_axes      = kwl.find(prefix, "draw_axes");
   const char* units          = kwl.find(prefix, "units");
   
   if(units)
   {
      theEllipseWidthHeightUnitType =
         static_cast<ossimUnitType>(ossimUnitTypeLut::instance()->getEntryNumber(units));
      if (theEllipseWidthHeightUnitType == OSSIM_UNIT_UNKNOWN)
      {
         theEllipseWidthHeightUnitType = OSSIM_PIXEL;
      }
   }

   if (center)
   {
      std::istringstream is(center);
      is >> theCenter;
   }
   if(ellipse_width)
   {
      theWidthHeight.x = ossimString(ellipse_width).toDouble();
   }
   if(ellipse_height)
   {
      theWidthHeight.y = ossimString(ellipse_height).toDouble();
   }

   if (azimuth)
   {
      setAzimuth(ossimString(azimuth).toFloat64());
   }

   if (fill)
   {
      setFillFlag(ossimString(fill).toBool());
   }

   if (draw_axes)
   {
      setDrawAxesFlag(ossimString(draw_axes).toBool());
   }

   return status;
}

void ossimGeoAnnotationEllipseObject::setEllipseWidthHeightUnitType(
   ossimUnitType type)
{
   theEllipseWidthHeightUnitType = type;
}

void ossimGeoAnnotationEllipseObject::setWidthHeight(const ossimDpt& pt)
{
   theWidthHeight = pt;
}

void ossimGeoAnnotationEllipseObject::setAzimuth(ossim_float64 azimuth)
{
   if (theProjectedEllipse.valid())
   {
      theProjectedEllipse->setAzimuth(azimuth);
   }
}
   
ossim_float64 ossimGeoAnnotationEllipseObject::getAzimuth() const
{
   if (theProjectedEllipse.valid())
   {
      return theProjectedEllipse->getAzimuth();
   }
   return 0.0;
}

void ossimGeoAnnotationEllipseObject::getWidthHeightInPixels(
   ossimDpt& widthHeight, const ossimImageGeometry* projection) const
{
   switch (theEllipseWidthHeightUnitType)
   {
      case OSSIM_PIXEL:
      {
         widthHeight = theWidthHeight;

         break;
      }
      
      case OSSIM_DEGREES:
      case OSSIM_MINUTES:
      case OSSIM_SECONDS:
      {
         ossimGpt origin = projection->hasProjection()?projection->getProjection()->origin():ossimGpt();
         ossimDpt gsd    = projection->getMetersPerPixel();
         
         ossimUnitConversionTool uct(origin,
                                     theWidthHeight.x,
                                     theEllipseWidthHeightUnitType);

         double x = uct.getValue(OSSIM_METERS);

         uct.setValue(theWidthHeight.y, theEllipseWidthHeightUnitType);
         
         double y = uct.getValue(OSSIM_METERS);

         widthHeight.x = ceil(x/gsd.x);
         widthHeight.y = ceil(y/gsd.y);
         
         break;
      }

      case OSSIM_METERS:
      {
         ossimDpt gsd = projection->getMetersPerPixel();
         widthHeight.x = static_cast<ossim_int32>(theWidthHeight.x/gsd.x+0.5);
         widthHeight.y = static_cast<ossim_int32>(theWidthHeight.y/gsd.y+0.5);

         break;
      }

      default:
      {
         ossimDpt gsd = projection->getMetersPerPixel();
         
         ossimUnitConversionTool uct(theWidthHeight.x,
                                     theEllipseWidthHeightUnitType);

         double x = uct.getValue(OSSIM_METERS);
         
         uct.setValue(theWidthHeight.y, theEllipseWidthHeightUnitType);
         
         double y = uct.getValue(OSSIM_METERS);
         
         widthHeight.x = ceil(x/gsd.x);
         widthHeight.y = ceil(y/gsd.y);
         
         break;
      }
   }
}
void ossimGeoAnnotationEllipseObject::setColor(ossim_uint8 r,
                                               ossim_uint8 g,
                                               ossim_uint8 b)
{
   if(theProjectedEllipse.valid())
   {
      theProjectedEllipse->setColor(r, g, b);
   }
   ossimGeoAnnotationObject::setColor(r, g, b);
}

void ossimGeoAnnotationEllipseObject::setThickness(ossim_uint8 thickness)
{
   if(theProjectedEllipse.valid())
   {
      theProjectedEllipse->setThickness(thickness);
   }
}

void ossimGeoAnnotationEllipseObject::setFillFlag(bool flag)
{
   if(theProjectedEllipse.valid())
   {
      theProjectedEllipse->setFillFlag(flag);
   }
}

bool ossimGeoAnnotationEllipseObject::getFillFlag() const
{
   if (theProjectedEllipse.valid())
   {
      return theProjectedEllipse->getFillFlag();
   }
   return false;
}

void ossimGeoAnnotationEllipseObject::setDrawAxesFlag(bool flag)
{
   if (theProjectedEllipse.valid())
   {
      theProjectedEllipse->setDrawAxesFlag(flag);
   }
}

bool ossimGeoAnnotationEllipseObject::getDrawAxesFlag() const
{
   if (theProjectedEllipse.valid())
   {
      return theProjectedEllipse->getDrawAxesFlag();
   }
   return false;
}

void ossimGeoAnnotationEllipseObject::setCenter(const ossimGpt& gpt)
{
   theCenter = gpt;
}

void ossimGeoAnnotationEllipseObject::getCenter(ossimGpt& gpt) const
{
   gpt = theCenter;
}

