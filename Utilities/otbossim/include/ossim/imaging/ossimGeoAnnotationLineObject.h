//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks)
// Description:
//
//*************************************************************************
// $Id: ossimGeoAnnotationLineObject.h 15766 2009-10-20 12:37:09Z gpotts $
#ifndef ossimGeoAnnotationLineObject_HEADER
#define ossimGeoAnnotationLineObject_HEADER
#include <ossim/base/ossimGpt.h>
#include <ossim/imaging/ossimGeoAnnotationObject.h>

class ossimAnnotationLineObject;

class ossimGeoAnnotationLineObject : public ossimGeoAnnotationObject
{
public:
   ossimGeoAnnotationLineObject(const ossimGpt& start,
                                const ossimGpt& end,
                                unsigned char r=255,
                                unsigned char g=255,
                                unsigned char b=255,
                                long thickness=1);
   ossimGeoAnnotationLineObject(const ossimGeoAnnotationLineObject& rhs);

   virtual ossimObject* dup()const;

   virtual void applyScale(double x, double y);
   virtual void transform(ossimImageGeometry* projection);

   virtual std::ostream& print(std::ostream& out)const;
   virtual bool intersects(const ossimDrect& rect)const;
   virtual ossimAnnotationObject* getNewClippedObject(const ossimDrect& rect)const;
   virtual void draw(ossimRgbImage& anImage)const;
   virtual void getBoundingRect(ossimDrect& rect)const;
   virtual void computeBoundingRect();

protected:
   virtual ~ossimGeoAnnotationLineObject();
   
   ossimAnnotationLineObject* theProjectedLineObject;
   ossimGpt                   theStart;
   ossimGpt                   theEnd;

TYPE_DATA
};

#endif
