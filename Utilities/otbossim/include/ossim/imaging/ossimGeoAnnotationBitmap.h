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
// $Id: ossimGeoAnnotationBitmap.h 9094 2006-06-13 19:12:40Z dburken $
#ifndef ossimGeoAnnotationBitmap_HEADER
#define ossimGeoAnnotationBitmap_HEADER
#include <ossim/imaging/ossimGeoAnnotationObject.h>

class OSSIMDLLEXPORT ossimGeoAnnotationBitmap: public ossimGeoAnnotationObject
{
public:

   ossimGeoAnnotationBitmap(const ossimGpt& center= ossimGpt(0,0,0),
                            ossimRefPtr<ossimImageData> imageData=NULL,
                            unsigned char r = 255,
                            unsigned char g = 255,
                            unsigned char b = 255);
   
   ossimGeoAnnotationBitmap(const ossimGeoAnnotationBitmap& rhs);

   virtual ~ossimGeoAnnotationBitmap();
   
   virtual ossimObject* dup()const
      {
         return new ossimGeoAnnotationBitmap(*this);
      }
   virtual bool intersects(const ossimDrect& rect) const
      {
         if(theImageData.valid())
         {
            return theImageData->getImageRectangle().intersects(rect);
         }

         return false;
      }
   virtual ossimGeoAnnotationBitmap* getNewClippedObject(const ossimDrect& rect)const
      {
         ossimGeoAnnotationBitmap* result = (ossimGeoAnnotationBitmap*)dup();

         ossimNotify(ossimNotifyLevel_WARN) << "ossimGeoAnnotationBitmap::getNewClippedObject WRNING: not implemented" << std::endl;
         
         return result;
      }
   virtual void applyScale(double x,
                           double y)
      {
         
      }
   virtual std::ostream& print(std::ostream& out)const;
   virtual void draw(ossimRgbImage& anImage)const;
   virtual void getBoundingRect(ossimDrect& rect)const;
   virtual void transform(ossimProjection* projection);
   virtual void setImageData(ossimRefPtr<ossimImageData>& imageData);
   virtual void computeBoundingRect();

protected:
   ossimGpt                    theCenterPoint;
   ossimDpt                    theProjectedPoint;
   ossimRefPtr<ossimImageData> theImageData;

TYPE_DATA
};

#endif
