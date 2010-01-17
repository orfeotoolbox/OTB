//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks)
// Description:
//
//*************************************************************************
// $Id: ossimAnnotationMultiPolyLineObject.h 15766 2009-10-20 12:37:09Z gpotts $
#ifndef ossimAnnotationMultiPolyLineObject_HEADER
#define ossimAnnotationMultiPolyLineObject_HEADER

#include <ossim/imaging/ossimAnnotationObject.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimPolyLine.h>

class OSSIMDLLEXPORT ossimAnnotationMultiPolyLineObject : public ossimAnnotationObject
{
public:
   ossimAnnotationMultiPolyLineObject();
   ossimAnnotationMultiPolyLineObject(const vector<ossimPolyLine>& multiPoly,
                                  unsigned char r,
                                  unsigned char g,
                                  unsigned char b,
                                  long thickness);
   
   ossimAnnotationMultiPolyLineObject(const ossimAnnotationMultiPolyLineObject& rhs);

   virtual ossimObject* dup()const;

   virtual void applyScale(double x,
                           double y);
   virtual void draw(ossimRgbImage& anImage)const;
   virtual bool intersects(const ossimDrect& rect)const;
   
   virtual ossimAnnotationObject* getNewClippedObject(const ossimDrect& rect)const;
   virtual std::ostream& print(std::ostream& out)const;
   virtual void getBoundingRect(ossimDrect& rect)const;
   virtual void addPolyLine(const ossimPolyLine& poly);
   virtual void addPoint(ossim_uint32 polygonIndex, const ossimDpt& pt);
   
   virtual void setMultiPolyLine(const vector<ossimPolyLine>& multiPoly);
   virtual void computeBoundingRect();
   virtual bool isPointWithin(const ossimDpt& imagePoint)const;
   
   const std::vector<ossimPolyLine>& getMultiPolyLine()const;
   std::vector<ossimPolyLine>& getMultiPolyLine();
protected:
   virtual ~ossimAnnotationMultiPolyLineObject();
   vector<ossimPolyLine> theMultiPolyLine;
   ossimDrect           theBoundingRect;

TYPE_DATA
};

#endif
