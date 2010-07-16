//*************************************************************************
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
//*************************************************************************
//$Id: ossimGeoAnnotationFontObject.cpp 17108 2010-04-15 21:08:06Z dburken $

#include <sstream>

#include <ossim/imaging/ossimGeoAnnotationFontObject.h>
#include <ossim/imaging/ossimAnnotationFontObject.h>
#include <ossim/projection/ossimProjection.h>
#include <ossim/projection/ossimImageProjectionModel.h>
#include <ossim/font/ossimFont.h>
#include <ossim/font/ossimFontFactoryRegistry.h>
#include <ossim/base/ossimException.h>
#include <ossim/base/ossimNotifyContext.h>

RTTI_DEF1(ossimGeoAnnotationFontObject,
          "ossimGeoAnnotationFontObject",
          ossimGeoAnnotationObject);


ossimGeoAnnotationFontObject::ossimGeoAnnotationFontObject()
   :
   ossimGeoAnnotationObject(),
   theCenterGround(),
   theFont(0),
   theFontInfo(),
   theAnnotationFontObject(0)
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
   
   theAnnotationFontObject->setFont(theFont.get());
   
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
   theFont(0),
   theFontInfo(),
   theAnnotationFontObject(0)
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
   theFont(rhs.theFont.valid()?(ossimFont*)rhs.theFont->dup():(ossimFont*)0),
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
   theAnnotationFontObject->setFont(theFont.get());
}

ossimGeoAnnotationFontObject::~ossimGeoAnnotationFontObject()
{
   theAnnotationFontObject = 0;
   theFont = 0;
}

ossimObject* ossimGeoAnnotationFontObject::dup()const
{
   return new ossimGeoAnnotationFontObject(*this);
}


void ossimGeoAnnotationFontObject::setFont(ossimFont* font)
{
   theFont = font;

   theAnnotationFontObject->setFont(theFont.get());
}

ossimFont* ossimGeoAnnotationFontObject::getFont()
{
   return theFont.get();
}

void ossimGeoAnnotationFontObject::setPointSize(const ossimIpt& size)
{
   if(theAnnotationFontObject.valid())
   {
      theAnnotationFontObject->setPointSize(size);
   }
}

void ossimGeoAnnotationFontObject::setRotation(double rotation)
{
   if(theAnnotationFontObject.valid())
   {
      theAnnotationFontObject->setRotation(rotation);
   }
}

void ossimGeoAnnotationFontObject::setScale(const ossimDpt& scale)
{
   if(theAnnotationFontObject.valid())
   {
      theAnnotationFontObject->setScale(scale);
   }
}

void ossimGeoAnnotationFontObject::setShear(const ossimDpt& shear)
{
   if(theAnnotationFontObject.valid())
   {
      theAnnotationFontObject->setShear(shear);
   }
}

void ossimGeoAnnotationFontObject::setColor(ossim_uint8 r,
                                            ossim_uint8 g,
                                            ossim_uint8 b)
{
   if(theAnnotationFontObject.valid())
   {
      theAnnotationFontObject->setColor(r, g, b);
   }
}

void ossimGeoAnnotationFontObject::transform(ossimImageGeometry* projection)
{
   if(projection)
   {
      ossimDpt ipt;

      projection->worldToLocal(theCenterGround, ipt);
      theAnnotationFontObject->setCenterPosition(ipt);
      theAnnotationFontObject->computeBoundingRect();
   }
}

void ossimGeoAnnotationFontObject::setCenterGround(const ossimGpt& gpt)
{
   theCenterGround = gpt;
}

ossimAnnotationFontObject* ossimGeoAnnotationFontObject::getFontObject()
{
   return theAnnotationFontObject.get();
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
   const ossimDrect& /* rect */)const
{
   return (ossimAnnotationObject*)0;
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
   ossimRefPtr<ossimFont> f = ossimFontFactoryRegistry::instance()->
      createFont(theFontInfo);
   if (f.valid())
   {
      theFont = f;
      theAnnotationFontObject->setFont(theFont.get());
   }

   theAnnotationFontObject->setGeometryInformation(theFontInfo);
   
   return status;
}
