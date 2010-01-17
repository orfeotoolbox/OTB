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
// $Id: ossimGeoAnnotationBitmap.h 15766 2009-10-20 12:37:09Z gpotts $
#ifndef ossimGeoAnnotationBitmap_HEADER
#define ossimGeoAnnotationBitmap_HEADER
#include <ossim/imaging/ossimGeoAnnotationObject.h>

class OSSIMDLLEXPORT ossimGeoAnnotationBitmap: public ossimGeoAnnotationObject
{
public:

   ossimGeoAnnotationBitmap(const ossimGpt& center= ossimGpt(0,0,0),
                            ossimRefPtr<ossimImageData> imageData=0,
                            unsigned char r = 255,
                            unsigned char g = 255,
                            unsigned char b = 255);
   
   ossimGeoAnnotationBitmap(const ossimGeoAnnotationBitmap& rhs);
   
   virtual ossimObject* dup()const;

   virtual bool intersects(const ossimDrect& rect) const;

   virtual ossimGeoAnnotationBitmap* getNewClippedObject(const ossimDrect& rect)const;

   virtual void applyScale(double x, double y);

   virtual std::ostream& print(std::ostream& out)const;
   virtual void draw(ossimRgbImage& anImage)const;
   virtual void getBoundingRect(ossimDrect& rect)const;
   virtual void transform(ossimImageGeometry* projection);

   virtual void setImageData(ossimRefPtr<ossimImageData>& imageData);
   virtual void computeBoundingRect();

protected:
   virtual ~ossimGeoAnnotationBitmap();
   
   ossimGpt                    theCenterPoint;
   ossimDpt                    theProjectedPoint;
   ossimRefPtr<ossimImageData> theImageData;

TYPE_DATA
};

#endif
