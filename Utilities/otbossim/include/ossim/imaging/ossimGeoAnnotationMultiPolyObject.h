//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description:
//
//*************************************************************************
// $Id: ossimGeoAnnotationMultiPolyObject.h 9968 2006-11-29 14:01:53Z gpotts $
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
   virtual ossimObject* dup()const
      {
         return new ossimGeoAnnotationMultiPolyObject(*this);
      }
   virtual ~ossimGeoAnnotationMultiPolyObject();
   virtual void transform(ossimProjection* projection);
   virtual void applyScale(double x,
                                double y);
   virtual void draw(ossimRgbImage& anImage)const;
   virtual bool intersects(const ossimDrect& rect)const;
   
   virtual ossimAnnotationObject* getNewClippedObject(const ossimDrect& rect)const;
   virtual std::ostream& print(std::ostream& out)const;
   virtual void getBoundingRect(ossimDrect& rect)const;
   virtual void addPoint(ossim_uint32 polygonIndex,
                         const ossimGpt& pt)
      {
         if(polygonIndex < theMultiPolygon.size())
         {
            theMultiPolygon[polygonIndex].addPoint(pt);

            // we will have to reset the projected polygon
            if(theProjectedPolyObject)
            {
               delete theProjectedPolyObject;
               theProjectedPolyObject = NULL;
            }
         }
      }
   
   virtual void setMultiPolygon(const vector<ossimGeoPolygon>& multiPoly)
      {
         theMultiPolygon = multiPoly;
         if(theProjectedPolyObject)
         {
            delete theProjectedPolyObject;
            theProjectedPolyObject = NULL;
         }
      }
   virtual void setColor(unsigned char r,
                         unsigned char g,
                         unsigned char b)
      {
         ossimAnnotationObject::setColor(r, g, b);
         if(theProjectedPolyObject)
         {
            theProjectedPolyObject->setColor(r, g, b);
         }
      }
   
   virtual void setThickness(ossim_uint8 thickness)
      {
         ossimAnnotationObject::setThickness(thickness);
         if(theProjectedPolyObject)
         {
            theProjectedPolyObject->setThickness(thickness);
         }
      }
   virtual void computeBoundingRect();
   virtual bool isPointWithin(const ossimDpt& imagePoint)const;
   virtual void setFillFlag(bool flag);
  
   const std::vector<ossimGeoPolygon>& getMultiPolygon()const{return theMultiPolygon;}
   std::vector<ossimGeoPolygon>& getMultiPolygon(){return theMultiPolygon;}
   
protected:
   vector<ossimGeoPolygon> theMultiPolygon;
   ossimDrect              theBoundingRect;
   bool                   theFillEnabled;
   ossimAnnotationMultiPolyObject* theProjectedPolyObject;
   
   void allocateProjectedPolygon();
TYPE_DATA
};

#endif
