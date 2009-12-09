//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description:
//
//*************************************************************************
// $Id: ossimGeoAnnotationMultiPolyObject.h 15766 2009-10-20 12:37:09Z gpotts $
#ifndef ossimGeoAnnotationMultiPolyObject_HEADER
#define ossimGeoAnnotationMultiPolyObject_HEADER
#include <ossim/imaging/ossimGeoAnnotationObject.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimGeoPolygon.h>
#include <ossim/imaging/ossimAnnotationMultiPolyObject.h>

class OSSIM_DLL ossimGeoAnnotationMultiPolyObject : public ossimGeoAnnotationObject
{
public:
   ossimGeoAnnotationMultiPolyObject();
   ossimGeoAnnotationMultiPolyObject(const vector<ossimGeoPolygon>& multiPoly,
                                     bool enableFill=false,
                                     unsigned char r=255,
                                     unsigned char g=255,
                                     unsigned char b=255,
                                     long thickness=1);
   
   ossimGeoAnnotationMultiPolyObject(const ossimGeoAnnotationMultiPolyObject& rhs);
   virtual ossimObject* dup()const;

   virtual void transform(ossimImageGeometry* projection);

   virtual void applyScale(double x,
                                double y);
   virtual void draw(ossimRgbImage& anImage)const;
   virtual bool intersects(const ossimDrect& rect)const;
   
   virtual ossimAnnotationObject* getNewClippedObject(const ossimDrect& rect)const;
   virtual std::ostream& print(std::ostream& out)const;
   virtual void getBoundingRect(ossimDrect& rect)const;
   virtual void addPoint(ossim_uint32 polygonIndex, const ossimGpt& pt);
   
   virtual void setMultiPolygon(const vector<ossimGeoPolygon>& multiPoly);

   virtual void setColor(unsigned char r, unsigned char g, unsigned char b);
   
   virtual void setThickness(ossim_uint8 thickness);

   virtual void computeBoundingRect();
   virtual bool isPointWithin(const ossimDpt& imagePoint)const;
   virtual void setFillFlag(bool flag);
  
   const std::vector<ossimGeoPolygon>& getMultiPolygon()const{return theMultiPolygon;}
   std::vector<ossimGeoPolygon>& getMultiPolygon(){return theMultiPolygon;}
   
protected:
   virtual ~ossimGeoAnnotationMultiPolyObject();

   std::vector<ossimGeoPolygon>    theMultiPolygon;
   ossimDrect                      theBoundingRect;
   bool                            theFillEnabled;
   ossimRefPtr<ossimAnnotationMultiPolyObject> theProjectedPolyObject;
   
   void allocateProjectedPolygon();
TYPE_DATA
};

#endif
