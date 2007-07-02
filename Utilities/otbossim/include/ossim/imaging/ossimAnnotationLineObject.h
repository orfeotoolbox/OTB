//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts
// Description:
//
//*************************************************************************
// $Id: ossimAnnotationLineObject.h 9968 2006-11-29 14:01:53Z gpotts $
#ifndef ossimAnnotationLineObject_HEADER
#define ossimAnnotationLineObject_HEADER
#include <ossim/imaging/ossimAnnotationObject.h>
#include <ossim/base/ossimIpt.h>

class OSSIMDLLEXPORT ossimAnnotationLineObject : public ossimAnnotationObject
{
public:
   ossimAnnotationLineObject();
   ossimAnnotationLineObject(const ossimIpt& start,
                             const ossimIpt& end,
                             unsigned char r=255,
                             unsigned char g=255,
                             unsigned char b=255,
                             long thickness=1);
   
   ossimAnnotationLineObject(long x1,
                             long y1,
                             long x2,
                             long y2,
                             unsigned char r=255,
                             unsigned char g=255,
                             unsigned char b=255,
                             long thickness=1);

   ossimAnnotationLineObject(const ossimAnnotationLineObject& rhs);
       
   virtual ossimObject* dup()const;
   
   virtual void applyScale(double x, double y);

   virtual ossimAnnotationObject* getNewClippedObject(const ossimDrect& rect)const;

   virtual bool intersects(const ossimDrect& rect)const;
   
   virtual void draw(ossimRgbImage& anImage)const;
   virtual std::ostream& print(std::ostream& out)const;
   virtual void getBoundingRect(ossimDrect& rect)const;
   virtual bool isPointWithin(const ossimDpt& imagePoint)const;
   virtual void computeBoundingRect();

   void setLine(const ossimDpt& start, const ossimDpt& end);
   void getLine(ossimDpt& start, ossimDpt& end);

protected:
   ossimIpt theStart;
   ossimIpt theEnd;
   ossimDrect theBoundingRect;

TYPE_DATA
};

#endif
