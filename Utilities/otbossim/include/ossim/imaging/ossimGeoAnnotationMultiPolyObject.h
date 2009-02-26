//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description:
//
//*************************************************************************
// $Id: ossimGeoAnnotationMultiPolyObject.h 13349 2008-07-30 15:34:34Z dburken $
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

   virtual ~ossimGeoAnnotationMultiPolyObject();
   virtual void transform(ossimProjection* projection);

   /**
    * @brief Transforms from geographic to image space for a
    * reduced resolution data set (rrds).
    *
    * This will transform any world points to line sample; then, convert any
    * line sample to the correct rrds point.
    *
    * @param model The model to use for transformation.
    *
    * @param rrds Reduced resolution data set to use.
    */
   virtual void transform(const ossimImageProjectionModel& model,
                          ossim_uint32 rrds);

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
   std::vector<ossimGeoPolygon>    theMultiPolygon;
   ossimDrect                      theBoundingRect;
   bool                            theFillEnabled;
   ossimAnnotationMultiPolyObject* theProjectedPolyObject;
   
   void allocateProjectedPolygon();
TYPE_DATA
};

#endif
