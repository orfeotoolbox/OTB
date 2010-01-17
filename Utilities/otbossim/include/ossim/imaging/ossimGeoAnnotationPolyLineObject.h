//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimGeoAnnotationPolyLineObject.h 15766 2009-10-20 12:37:09Z gpotts $
#ifndef ossimGeoAnnotationPolyLineObject_HEADER
#define ossimGeoAnnotationPolyLineObject_HEADER
#include <ossim/base/ossimGpt.h>
#include <ossim/imaging/ossimGeoAnnotationObject.h>

class ossimAnnotationMultiLineObject;

class OSSIMDLLEXPORT ossimGeoAnnotationPolyLineObject : public ossimGeoAnnotationObject
{
public:
   ossimGeoAnnotationPolyLineObject(const vector<ossimGpt>& groundPts,
                                     ossim_uint8 r=255,
                                     ossim_uint8 g=255,
                                     ossim_uint8 b=255,
                                     ossim_uint8 thickness=1);
   ossimGeoAnnotationPolyLineObject(const ossimGeoAnnotationPolyLineObject& rhs);
   virtual ossimObject* dup()const;

   virtual void applyScale(double x, double y);

   virtual void transform(ossimImageGeometry* projection);
   
   virtual std::ostream& print(std::ostream& out)const;
   virtual void draw(ossimRgbImage& anImage)const;
   virtual ossimAnnotationObject* getNewClippedObject(const ossimDrect& rect)const;
   virtual bool intersects(const ossimDrect& rect)const;
   
   virtual void getBoundingRect(ossimDrect& rect)const;
   virtual void computeBoundingRect();

   /**
    * Sets the color of thePolygon. RGB range 0 to 255
    *
    * @param r DN value for red.
    * @param g DN value for green.
    * @param b DN value for blue.
    */
   virtual void setColor(ossim_uint8 r,
                         ossim_uint8 g,
                         ossim_uint8 b);
   /**
    * Sets the pen thickness of thePolygon. range 0 to 255
    *
    * @param thickness Pen thickness.
    */
   virtual void setThickness(ossim_uint8 thickness);

protected:
   virtual ~ossimGeoAnnotationPolyLineObject();
   
   std::vector<ossimGpt>           thePolygon;
   ossimAnnotationMultiLineObject* theProjectedMultiLineObject;

TYPE_DATA
};

#endif
