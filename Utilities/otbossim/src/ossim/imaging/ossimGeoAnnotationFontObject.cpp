//*************************************************************************
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
//*************************************************************************
//$Id: ossimGeoAnnotationFontObject.cpp 11411 2007-07-27 13:53:51Z dburken $

#include <sstream>

#include <ossim/imaging/ossimGeoAnnotationFontObject.h>
#include <ossim/imaging/ossimAnnotationFontObject.h>
#include <ossim/projection/ossimProjection.h>
#include <ossim/font/ossimFont.h>
#include <ossim/font/ossimFontFactoryRegistry.h>
#include <ossim/base/ossimNotifyContext.h>

RTTI_DEF1(ossimGeoAnnotationFontObject,
          "ossimGeoAnnotationFontObject",
          ossimGeoAnnotationObject);


ossimGeoAnnotationFontObject::ossimGeoAnnotationFontObject()
   :
   ossimGeoAnnotationObject(),
   theCenterGround(),
   theFont(NULL),
   theFontInfo(),
   theAnnotationFontObject(NULL)
{
   theFont = (ossimFont*)ossimFontFactoryRegistry::instance()->
      getDefaultFont()->dup();
   if (!theFont)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimGeoAnnotationFontObject::ossimGeoAnnotationFontObject"
         << " WARNING: No font support..."
         << endl;
   }
   theAnnotationFontObject =
      new ossimAnnotationFontObject(ossimIpt(0,0),
                                    ossimString(""));
   
   theAnnotationFontObject->setFont(theFont, false);
   
}

ossimGeoAnnotationFontObject::ossimGeoAnnotationFontObject(
   const ossimGpt& location,
   const ossimString& s,
   const ossimIpt& pixelSize,
   double rotation,
   const ossimDpt& scale,
   const ossimDpt& shear,
   ossim_uint8 r,
   ossim_uint8 g,
   ossim_uint8 b)
   :
   ossimGeoAnnotationObject(r, g, b),
   theCenterGround(location),
   theFont(NULL),
   theFontInfo(),
   theAnnotationFontObject(NULL)
{
  ossimDpt pt;
  pt.makeNan();

  theAnnotationFontObject = new ossimAnnotationFontObject(pt,
							  s,
							  pixelSize,
							  rotation,
							  scale,
							  shear,
							  r,
							  g,
							  b);
}

ossimGeoAnnotationFontObject::ossimGeoAnnotationFontObject(
   const ossimGeoAnnotationFontObject& rhs)
   :
   ossimGeoAnnotationObject(rhs),
   theCenterGround(rhs.theCenterGround),
   theFont(rhs.theFont?(ossimFont*)rhs.theFont->dup():(ossimFont*)NULL),
   theFontInfo(),
   theAnnotationFontObject(
      new ossimAnnotationFontObject(
         ossimDpt(ossim::nan(), ossim::nan()),
         rhs.theAnnotationFontObject->theString,
         rhs.theAnnotationFontObject->thePixelSize,
         rhs.theAnnotationFontObject->theRotation,
         ossimDpt(rhs.theAnnotationFontObject->theHorizontalScale,
                  rhs.theAnnotationFontObject->theVerticalScale),
         ossimDpt(rhs.theAnnotationFontObject->theHorizontalShear,
                  rhs.theAnnotationFontObject->theVerticalShear),
         rhs.theRed,
         rhs.theGreen,
         rhs.theBlue))
{
   theAnnotationFontObject->setFont(theFont, false);
}

ossimGeoAnnotationFontObject::~ossimGeoAnnotationFontObject()
{
   if(theAnnotationFontObject)
   {
      delete theAnnotationFontObject;
      theAnnotationFontObject = NULL;
   }
   if(theFont)
   {
      delete theFont;
      theFont = NULL;
   }
}

ossimObject* ossimGeoAnnotationFontObject::dup()const
{
   return new ossimGeoAnnotationFontObject(*this);
}


void ossimGeoAnnotationFontObject::setFont(ossimFont* font)
{
   if(theFont)
   {
     delete theFont;
     theFont = NULL;
   }
   theFont = font;

   theAnnotationFontObject->setFont(theFont, false);
}

ossimFont* ossimGeoAnnotationFontObject::getFont()
{
   return theFont;
}

void ossimGeoAnnotationFontObject::setPointSize(const ossimIpt& size)
{
   if(theAnnotationFontObject)
   {
      theAnnotationFontObject->setPointSize(size);
   }
}

void ossimGeoAnnotationFontObject::setRotation(double rotation)
{
   if(theAnnotationFontObject)
   {
      theAnnotationFontObject->setRotation(rotation);
   }
}

void ossimGeoAnnotationFontObject::setScale(const ossimDpt& scale)
{
   if(theAnnotationFontObject)
   {
      theAnnotationFontObject->setScale(scale);
   }
}

void ossimGeoAnnotationFontObject::setShear(const ossimDpt& shear)
{
   if(theAnnotationFontObject)
   {
      theAnnotationFontObject->setShear(shear);
   }
}

void ossimGeoAnnotationFontObject::setColor(ossim_uint8 r,
                                            ossim_uint8 g,
                                            ossim_uint8 b)
{
   if(theAnnotationFontObject)
   {
      theAnnotationFontObject->setColor(r, g, b);
   }
}

void ossimGeoAnnotationFontObject::transform(ossimProjection* projection)
{
   if(projection)
   {
      ossimDpt ipt;

      projection->worldToLineSample(theCenterGround, ipt);
      theAnnotationFontObject->setPositionCenter(ipt);
      theAnnotationFontObject->computeBoundingRect();
   }
}

void ossimGeoAnnotationFontObject::setCenterGround(const ossimGpt& gpt)
{
   theCenterGround = gpt;
}

ossimAnnotationFontObject* ossimGeoAnnotationFontObject::getFontObject()
{
   return theAnnotationFontObject;
}

void ossimGeoAnnotationFontObject::draw(ossimRgbImage& anImage)const
{
   theAnnotationFontObject->draw(anImage);
}

void ossimGeoAnnotationFontObject::computeBoundingRect()
{
   theAnnotationFontObject->computeBoundingRect();
}

void ossimGeoAnnotationFontObject::getBoundingRect(ossimDrect& rect)const
{
   theAnnotationFontObject->getBoundingRect(rect);
}

ossimAnnotationObject* ossimGeoAnnotationFontObject::getNewClippedObject(
   const ossimDrect& rect)const
{
   return (ossimAnnotationObject*)NULL;
}

bool ossimGeoAnnotationFontObject::intersects(const ossimDrect& rect)const
{
   return theAnnotationFontObject->intersects(rect);
}

void ossimGeoAnnotationFontObject::applyScale(double x, double y)
{
   theAnnotationFontObject->applyScale(x,y);
}

bool ossimGeoAnnotationFontObject::saveState(ossimKeywordlist& kwl,
                                             const char* prefix) const
{
   kwl.add(prefix, "center", theCenterGround.toString().c_str());
   kwl.add(prefix, "text", theAnnotationFontObject->getString().c_str());
   theFontInfo.saveState(kwl, prefix);
   
   return ossimGeoAnnotationObject::saveState(kwl, prefix);
}

bool ossimGeoAnnotationFontObject::loadState(const ossimKeywordlist& kwl,
                                             const char* prefix)
{
   //---
   // Base class state must be called first to pick up colors...
   //---
   bool status =  ossimGeoAnnotationObject::loadState(kwl, prefix);

   theAnnotationFontObject->setColor(theRed, theGreen, theBlue);
   theAnnotationFontObject->setThickness(theThickness);
   
   const char* lookup;

   lookup = kwl.find(prefix, "text");
   if (lookup)
   {
      theAnnotationFontObject->setString(ossimString(lookup));
   }

   lookup = kwl.find(prefix, "center");
   if (lookup)
   {
      std::istringstream is(lookup);
      is >> theCenterGround;
   }

   // Get the font information.
   theFontInfo.loadState(kwl, prefix);

   // See if we can make a font.
   ossimFont* f = ossimFontFactoryRegistry::instance()->
      createFont(theFontInfo);
   if (f)
   {
      if (theFont)
      {
         delete theFont;
      }
      theFont = f;
      theAnnotationFontObject->setFont(theFont, false);
   }

   theAnnotationFontObject->setGeometryInformation(theFontInfo);
   
   return status;
}
