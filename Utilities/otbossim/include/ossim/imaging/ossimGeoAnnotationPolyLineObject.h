//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimGeoAnnotationPolyLineObject.h 9094 2006-06-13 19:12:40Z dburken $
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
   virtual ~ossimGeoAnnotationPolyLineObject();
   virtual ossimObject* dup()const
      {
         return new ossimGeoAnnotationPolyLineObject(*this);
      }
   
   virtual void applyScale(double x,
                           double y);

   virtual void transform(ossimProjection* projection);
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
   vector<ossimGpt>                thePolygon;
   ossimAnnotationMultiLineObject* theProjectedMultiLineObject;

TYPE_DATA
};

#endif
