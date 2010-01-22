//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimAnnotationFontObject.cpp 15766 2009-10-20 12:37:09Z gpotts $

#include <ossim/imaging/ossimAnnotationFontObject.h>
#include <ossim/font/ossimFontFactoryRegistry.h>

RTTI_DEF1(ossimAnnotationFontObject, "ossimAnnotationFontObject", ossimAnnotationObject);

ossimAnnotationFontObject::ossimAnnotationFontObject()
   :ossimAnnotationObject(),
    theFont(ossimFontFactoryRegistry::instance()->getDefaultFont()),
    thePosition(0,0),
    theString(""),
    theRotation(0.0),
    theHorizontalScale(0.0),
    theVerticalScale(0.0),
    theHorizontalShear(0.0),
    theVerticalShear(0.0)
{
   setFontInfo();
   theBoundingRect.makeNan();
}

ossimAnnotationFontObject::ossimAnnotationFontObject(const ossimIpt& upperLeft,
                                                     const ossimString& s,
                                                     const ossimIpt& pixelSize,
                                                     double rotation,
                                                     const ossimDpt& scale,
                                                     const ossimDpt& shear,
                                                     unsigned char r,
                                                     unsigned char g,
                                                     unsigned char b)
   :ossimAnnotationObject(r,g,b),
    theFont(ossimFontFactoryRegistry::instance()->getDefaultFont()),
    theString(s),
    thePixelSize(pixelSize),
    theRotation(rotation),
    theHorizontalScale(scale.x),
    theVerticalScale(scale.y),
    theHorizontalShear(shear.x),
    theVerticalShear(shear.y)
{
   setFontInfo();
   thePosition = upperLeft;
   computeBoundingRect();
#if 0
   if (theFont)
   {
      theFont->getBoundingBox(theBoundingRect);
   }
   theBoundingRect += upperLeft;
   thePosition = theBoundingRect.ul();
#endif
}

ossimAnnotationFontObject::ossimAnnotationFontObject(const ossimAnnotationFontObject& rhs)
   :ossimAnnotationObject(rhs),
    thePosition(rhs.thePosition),
    theString(rhs.theString),
    thePixelSize(rhs.thePixelSize),
    theRotation(rhs.theRotation),
    theHorizontalScale(rhs.theHorizontalScale),
    theVerticalScale(rhs.theVerticalScale),
    theHorizontalShear(rhs.theHorizontalShear),
    theVerticalShear(rhs.theVerticalShear),
    theBoundingRect(rhs.theBoundingRect)
{
   theFont = rhs.theFont;
   setFontInfo();
}


ossimAnnotationFontObject::~ossimAnnotationFontObject()
{
   theFont = 0;
}

void ossimAnnotationFontObject::draw(ossimRgbImage& anImage)const
{
   if(theBoundingRect.hasNans())
   {
      return;
   }
   if (!theFont)
   {
      return;
   }
   
   if(!thePixelSize.x || !thePixelSize.y)
   {
      return; // (DAH) we have to have some demension to write!
   }

   ossimRefPtr<ossimImageData> destination = anImage.getImageData();
   
   if(destination.valid())
   {
      ossimIrect destRect = anImage.getImageData()->getImageRectangle();
      
      ossimIpt shift(-theBoundingRect.ul().x,
                     -theBoundingRect.ul().y);

      destRect += shift;
      ossimIrect boundingRect = theBoundingRect + shift;
      
      if(boundingRect.intersects(destRect))
      {
         setFontInfo();
 
         ossimIrect fontBufferRect;
         ossimIrect boundingFontBox;
         
         theFont->setClippingBox(destRect);
         theFont->getBoundingBox(boundingFontBox);

         const ossim_uint8* srcBuf = theFont->rasterize();
         if (!srcBuf)
         {
            return;
         }
         
         theFont->getBufferRect(fontBufferRect);
         ossimIrect clipRect = boundingRect.clipToRect(fontBufferRect);
         
         long clipHeight = clipRect.height();
         long clipWidth  = clipRect.width();
         
         long destinationOffset   = (long)(((clipRect.ul().y - destRect.ul().y)* destRect.width()) +
                                           (clipRect.ul().x - destRect.ul().x));
         long fontBufferOffset    = (long)(((clipRect.ul().y - fontBufferRect.ul().y)*fontBufferRect.width()) +
                                           (clipRect.ul().x - fontBufferRect.ul().x));

         
         long destinationIndex = destinationOffset;
         long srcIndex         = fontBufferOffset;
         long num_bands        = destination->getNumberOfBands();
         ossim_int32 s_width = (ossim_int32)fontBufferRect.width();
         ossim_int32 d_width = (ossim_int32)destRect.width();
         
         num_bands = num_bands > 3? 3:num_bands;
         
         unsigned char colorArray[3];
         long band = 0;
         colorArray[0] = theRed;
         colorArray[1] = theGreen;
         colorArray[2] = theBlue;
         ossim_uint8** destBuf = new ossim_uint8*[num_bands];
         for(band = 0; band < num_bands;++band)
         {
            destBuf[band] = static_cast<ossim_uint8*>(destination->getBuf(band));
            if (!destBuf[band])
            {
               delete [] destBuf;
               return;
            }
         }
         
         for(long line = 0; line < clipHeight; ++line)
         {
            for(long col = 0; col < clipWidth; ++col)
            {
               if(srcBuf[srcIndex + col])
               {
                  for (band=0; band<num_bands; ++band)
                  {
                     *(destBuf[band] + destinationIndex + col) = colorArray[band];
                  }
               }
            }
            srcIndex         += s_width;
            destinationIndex += d_width;
         }
         delete [] destBuf;
      }
   }
}

std::ostream& ossimAnnotationFontObject::print(std::ostream& out)const
{
   if(theFont.valid())
   {
      out << "Family:          " << theFont->getFamilyName() << endl
          << "Style:           " << theFont->getStyleName()  << endl;
   }
   out << "String:             " << theString << endl
       << "Upper Left Position:    " << thePosition << endl
       << "Rotation:           " << theRotation << endl
       << "Horizontal shear:   " << theHorizontalShear << endl
       << "Vertical shear:     " << theVerticalShear << endl
       << "Vertical scale:     " << theVerticalScale << endl
       << "Horizontal scale:   " << theHorizontalScale << endl
       << "Bounding rect:      " << theBoundingRect << endl;
   return out;
}

void ossimAnnotationFontObject::getBoundingRect(ossimDrect& rect)const
{
   rect =  theBoundingRect;
}

void ossimAnnotationFontObject::computeBoundingRect()
{
   setFontInfo();
   if(theFont.valid())
   {
      ossimIrect textRect;
      theFont->getBoundingBox(textRect);
      ossim_int32 w = textRect.width();
      ossim_int32 h = textRect.height();
      ossim_int32 ulx = thePosition.x;
      ossim_int32 uly = thePosition.y;
      theBoundingRect = ossimIrect(ulx,
                                   uly,
                                   ulx + w - 1,
                                   uly + h - 1);
   }
}

bool ossimAnnotationFontObject::isPointWithin(const ossimDpt& imagePoint)const
{
   return theBoundingRect.pointWithin(imagePoint);
}

void ossimAnnotationFontObject::setFont(ossimFont* font)
{
    theFont = font;

   if(!theFont)
   {
      theFont = ossimFontFactoryRegistry::instance()->getDefaultFont();
   }
}

void ossimAnnotationFontObject::setCenterPosition(const ossimIpt& position)
{
   ossimDpt pt = theBoundingRect.midPoint();
   ossimIpt delta = position-pt;
   theBoundingRect += delta;
   thePosition = theBoundingRect.ul();
}

void ossimAnnotationFontObject::setUpperLeftPosition(const ossimIpt& position)
{
   ossimDpt pt = theBoundingRect.ul();
   ossimIpt delta = position-pt;
   theBoundingRect += delta;
   thePosition = theBoundingRect.ul();
}

void ossimAnnotationFontObject::setFontInfo()const
{
   if(theFont.valid())
   {
      theFont->setString(theString);
      theFont->setRotation(theRotation);
      theFont->setScale(theHorizontalScale, theVerticalScale);
      theFont->setHorizontalVerticalShear(theHorizontalShear, theVerticalShear);
      theFont->setPixelSize(thePixelSize.x,
                            thePixelSize.y);
   }
}

void ossimAnnotationFontObject::setString(const ossimString& s)
{
   theString = s;
}

ossimString ossimAnnotationFontObject::getString()const
{
   return theString;
}

ossimAnnotationObject* ossimAnnotationFontObject::getNewClippedObject(const ossimDrect& rect)const
{
   if(intersects(rect))
   {
      return (ossimAnnotationObject*)dup();
   }
   
   return (ossimAnnotationObject*)NULL;
}

bool ossimAnnotationFontObject::intersects(const ossimDrect& rect)const
{
   return rect.intersects(theBoundingRect);
}

void ossimAnnotationFontObject::setPointSize(const ossimIpt& size)
{
   thePixelSize = size;
   setFontInfo();
   if (theFont.valid())
   {
      theFont->getBoundingBox(theBoundingRect);
   }
   theBoundingRect += (thePosition - theBoundingRect.ul());
   thePosition = theBoundingRect.ul();         
}

void ossimAnnotationFontObject::setRotation(double rotation)
{
   theRotation = rotation;
   setFontInfo();
   if (theFont.valid())
   {
      theFont->getBoundingBox(theBoundingRect);
   }
   theBoundingRect += (thePosition - theBoundingRect.ul());
   thePosition = theBoundingRect.ul();         
}

void ossimAnnotationFontObject::setScale(const ossimDpt& scale)
{
   theHorizontalScale = scale.x;
   theVerticalScale   = scale.y;
   setFontInfo();
   if (theFont.valid())
   {
      theFont->getBoundingBox(theBoundingRect);
   }
   theBoundingRect += (thePosition - theBoundingRect.ul());
   thePosition = theBoundingRect.ul();         
}

void ossimAnnotationFontObject::setShear(const ossimDpt& shear)
{
   theHorizontalShear = shear.x;
   theVerticalShear   = shear.y;
   setFontInfo();
   if (theFont.valid())
   {
      theFont->getBoundingBox(theBoundingRect);
   }
   theBoundingRect += (thePosition - theBoundingRect.ul());
   thePosition = theBoundingRect.ul();         
}

void ossimAnnotationFontObject::setGeometryInformation(const ossimFontInformation& info)
{
   thePixelSize       = info.thePointSize;
   theRotation        = info.theRotation;
   theHorizontalScale = info.theScale.x;
   theVerticalScale   = info.theScale.y;
   theHorizontalShear = info.theShear.x;
   theVerticalShear   = info.theShear.y;
   
   setFontInfo();
   if (theFont.valid())
   {
      theFont->getBoundingBox(theBoundingRect);
   }
   theBoundingRect += (thePosition - theBoundingRect.ul());
   thePosition = theBoundingRect.ul();         
}

void ossimAnnotationFontObject::applyScale(double x, double y)
{
   thePosition.x = ossim::round<int>(thePosition.x *x);
   thePosition.y = ossim::round<int>(thePosition.y *y);
   
   setFontInfo();
   computeBoundingRect();

}

ossimObject* ossimAnnotationFontObject::dup()const
{
   return new ossimAnnotationFontObject(*this);
}
