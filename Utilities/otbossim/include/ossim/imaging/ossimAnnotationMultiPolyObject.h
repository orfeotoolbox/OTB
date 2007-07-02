//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts
// Description:
//
//*************************************************************************
// $Id: ossimAnnotationMultiPolyObject.h 9968 2006-11-29 14:01:53Z gpotts $
#ifndef ossimAnnotationMultiPolyObject_HEADER
#define ossimAnnotationMultiPolyObject_HEADER
#include <ossim/imaging/ossimAnnotationObject.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimPolygon.h>

class OSSIMDLLEXPORT ossimAnnotationMultiPolyObject : public ossimAnnotationObject
{
public:
   ossimAnnotationMultiPolyObject();
   ossimAnnotationMultiPolyObject(const vector<ossimPolygon>& multiPoly,
                                  bool enableFill,
                                  unsigned char r,
                                  unsigned char g,
                                  unsigned char b,
                                  long thickness);
   
   ossimAnnotationMultiPolyObject(const ossimAnnotationMultiPolyObject& rhs)
      : ossimAnnotationObject(rhs),
        theMultiPolygon(rhs.theMultiPolygon),
        theBoundingRect(rhs.theBoundingRect),
        theFillEnabled(rhs.theFillEnabled)
      {
      }
   virtual ossimObject* dup()const
      {
         return new ossimAnnotationMultiPolyObject(*this);
      }
   virtual ~ossimAnnotationMultiPolyObject();
   virtual void applyScale(double x,
                           double y);
   virtual void draw(ossimRgbImage& anImage)const;
   virtual bool intersects(const ossimDrect& rect)const;
   
   virtual ossimAnnotationObject* getNewClippedObject(const ossimDrect& rect)const;
   virtual std::ostream& print(std::ostream& out)const;
   virtual void getBoundingRect(ossimDrect& rect)const;
   virtual void clear();
   virtual void addPolygon(const ossimPolygon& poly)
      {
         theMultiPolygon.push_back(poly);
      }
   virtual void addPoint(ossim_uint32 polygonIndex,
                         const ossimDpt& pt)
      {
         if(polygonIndex < theMultiPolygon.size())
         {
            theMultiPolygon[polygonIndex].addPoint(pt);
         }
      }
   
   virtual void setMultiPolygon(const vector<ossimPolygon>& multiPoly)
      {
         theMultiPolygon = multiPoly;
         computeBoundingRect();
      }
   virtual void computeBoundingRect();
   virtual bool isPointWithin(const ossimDpt& imagePoint)const;
   
   const std::vector<ossimPolygon>& getMultiPolygon()const{return theMultiPolygon;}
   std::vector<ossimPolygon>& getMultiPolygon(){return theMultiPolygon;}
   void setFillFlag(bool flag)
      {
         theFillEnabled = flag;
      }
protected:
   vector<ossimPolygon> theMultiPolygon;
   ossimDrect           theBoundingRect;
   bool                 theFillEnabled;

TYPE_DATA
};

#endif
