//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimGeoAnnotationBitmap.cpp 17195 2010-04-23 17:32:18Z dburken $

#include <ossim/imaging/ossimGeoAnnotationBitmap.h>
#include <ossim/projection/ossimProjection.h>
#include <ossim/projection/ossimImageProjectionModel.h>
#include <ossim/base/ossimException.h>
#include <ossim/base/ossimIrect.h>

RTTI_DEF1(ossimGeoAnnotationBitmap,
          "ossimGeoAnnotationBitmap",
          ossimGeoAnnotationObject)
   
ossimGeoAnnotationBitmap::ossimGeoAnnotationBitmap(
   const ossimGpt& center,
   ossimRefPtr<ossimImageData> imageData,
   unsigned char r,
   unsigned char g,
   unsigned char b)
   :ossimGeoAnnotationObject(r, g, b),
    theCenterPoint(center),
    theProjectedPoint(0,0),
    theImageData(NULL)
{
   if(imageData.valid() &&
      (imageData->getScalarType()==OSSIM_UCHAR))
   {
      theImageData = imageData;
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimGeoAnnotationBitmap::ossimGeoAnnotationBitmap\n"
         << "Invalid image data passed to ossimGeoAnnotationBitmap "
         << "constructor" << endl;
   }
}

ossimGeoAnnotationBitmap::ossimGeoAnnotationBitmap(
   const ossimGeoAnnotationBitmap& rhs)
   :
   ossimGeoAnnotationObject(rhs),
   theCenterPoint(theCenterPoint),
   theProjectedPoint(theProjectedPoint),
   theImageData(rhs.theImageData)
{
}

ossimGeoAnnotationBitmap::~ossimGeoAnnotationBitmap()
{
}

ossimObject* ossimGeoAnnotationBitmap::dup()const
{
   return new ossimGeoAnnotationBitmap(*this);
}

bool ossimGeoAnnotationBitmap::intersects(const ossimDrect& rect) const
{
   if(theImageData.valid())
   {
      return theImageData->getImageRectangle().intersects(rect);
   }
   
   return false;
}

ossimGeoAnnotationBitmap* ossimGeoAnnotationBitmap::getNewClippedObject(
   const ossimDrect& /* rect */)const
{
   ossimGeoAnnotationBitmap* result = (ossimGeoAnnotationBitmap*)dup();
   
   ossimNotify(ossimNotifyLevel_WARN)
      << "ossimGeoAnnotationBitmap::getNewClippedObject WRNING: "
      << "not implemented" << std::endl;
   
   return result;
}

void ossimGeoAnnotationBitmap::applyScale(double /* x */, double /* y */)
{
     ossimNotify(ossimNotifyLevel_WARN)
      << "ossimGeoAnnotationBitmap::applyScale WRNING: not implemented"
      << std::endl; 
}


std::ostream& ossimGeoAnnotationBitmap::print(std::ostream& out)const
{
   out << "center:    " << theCenterPoint << endl;
   return out;
}

void ossimGeoAnnotationBitmap::draw(ossimRgbImage& anImage)const
{
   ossimRefPtr<ossimImageData> destination = anImage.getImageData();

   if(!destination)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimGeoAnnotationBitmap::draw\n"
         << "ERROR: can't draw annotation bitmap to NULL buffer" << endl;
      
      return;
   }
   ossimIrect destinationRect = destination->getImageRectangle();
   ossimIrect srcRect         = theImageData->getImageRectangle();
   if(!srcRect.intersects(destinationRect))
   {
      return;
   }
   ossimIrect clipRect        = srcRect.clipToRect(destinationRect);
   
   long clipHeight = (long)clipRect.height();
   long clipWidth  = (long)clipRect.width();


   if(clipRect.width() != 1.0 &&
      clipRect.height() != 1.0)
   {         
      long destinationOffset   = (long)(((clipRect.ul().y - destinationRect.ul().y)* destinationRect.width()) +
                                        (clipRect.ul().x - destinationRect.ul().x));
      long srcOffset   = (long)(((clipRect.ul().y - srcRect.ul().y)*srcRect.width()) +
                         (clipRect.ul().x - srcRect.ul().x));
      long destinationIndex = destinationOffset;
      long srcIndex         = srcOffset;
      long num_bands        = destination->getNumberOfBands();
      ossim_int32 s_width = (ossim_int32)srcRect.width();
      ossim_int32 d_width = (ossim_int32)destinationRect.width();
      
      num_bands = num_bands > 3? 3:num_bands;
      
      const ossim_uint8* imageDataBuf = static_cast<const ossim_uint8*>(theImageData->getBuf());
      unsigned char colorArray[3];
      colorArray[0] = theRed;
      colorArray[1] = theGreen;
      colorArray[2] = theBlue;
      for (long band=0; band<num_bands; ++band)
      {
         ossim_uint8* destinationBand = static_cast<ossim_uint8*>(destination->getBuf(band));
         destinationIndex       = destinationOffset;
         srcIndex               = srcOffset;
         for(long line = 0; line < clipHeight; ++line)
         {
            for(long col = 0; col < clipWidth; ++col)
            {
               if(imageDataBuf[srcIndex + col])
               {
                  destinationBand[destinationIndex + col] = colorArray[band];
               }
            }
            srcIndex += s_width;
            destinationIndex += d_width;
         }
      } 
   }
}

void ossimGeoAnnotationBitmap::getBoundingRect(ossimDrect& rect)const
{
   rect = ossimDrect(0,0,0,0);

   if(theImageData.valid())
   {
      rect = theImageData->getImageRectangle();
   }
}

void ossimGeoAnnotationBitmap::transform(ossimImageGeometry* projection)
{
   if(projection)
   {
      projection->worldToLocal(theCenterPoint, theProjectedPoint);
      theProjectedPoint = ossimIpt(theProjectedPoint);
      if(theImageData.valid())
      {
         ossimDpt origin(theProjectedPoint.x - theImageData->getWidth()/2.0,
                         theProjectedPoint.y - theImageData->getHeight()/2.0);
         
         theImageData->setOrigin(origin);
      }
   }
}

void ossimGeoAnnotationBitmap::setImageData(
   ossimRefPtr<ossimImageData>& imageData)
{
   theImageData = imageData;
}

void ossimGeoAnnotationBitmap::computeBoundingRect()
{
   // nothing to be done since we don't support
   // rotated bitmaps yet.
}
