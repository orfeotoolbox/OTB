//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks)
// Description:
//
//*************************************************************************
// $Id: ossimGeoAnnotationLineObject.h 9968 2006-11-29 14:01:53Z gpotts $
#ifndef ossimGeoAnnotationLineObject_HEADER
#define ossimGeoAnnotationLineObject_HEADER
#include <ossim/base/ossimGpt.h>
#include <ossim/imaging/ossimGeoAnnotationObject.h>

class OSSIMDLLEXPORT ossimAnnotationLineObject;

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
   virtual ~ossimGeoAnnotationLineObject();

   virtual ossimObject* dup()const
      {
         return new ossimGeoAnnotationLineObject(*this);
      }

   virtual void applyScale(double x, double y);
   virtual void transform(ossimProjection* projection);
   virtual std::ostream& print(std::ostream& out)const;
   virtual bool intersects(const ossimDrect& rect)const;
   virtual ossimAnnotationObject* getNewClippedObject(const ossimDrect& rect)const;
   virtual void draw(ossimRgbImage& anImage)const;
   virtual void getBoundingRect(ossimDrect& rect)const;
   virtual void computeBoundingRect();

protected:
   ossimAnnotationLineObject* theProjectedLineObject;
   ossimGpt                   theStart;
   ossimGpt                   theEnd;

TYPE_DATA
};

#endif
