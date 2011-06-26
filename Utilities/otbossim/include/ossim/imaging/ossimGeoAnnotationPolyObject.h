//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimGeoAnnotationPolyObject.h 19734 2011-06-06 23:45:36Z dburken $
#ifndef ossimGeoAnnotationPolyObject_HEADER
#define ossimGeoAnnotationPolyObject_HEADER

#include <vector>
#include <ossim/base/ossimGpt.h>
#include <ossim/imaging/ossimGeoAnnotationObject.h>
#include <ossim/imaging/ossimAnnotationPolyObject.h>
 
class OSSIMDLLEXPORT ossimGeoAnnotationPolyObject : public ossimGeoAnnotationObject
{
public:
   enum ossimPolyType
   {
     OSSIM_POLY_EXTERIOR_RING  = 0,
     OSSIM_POLY_INTERIOR_RING = 1
   };

   ossimGeoAnnotationPolyObject(bool enableFill = false,
                                ossim_uint8 r=255,
                                ossim_uint8 g=255,
                                ossim_uint8 b=255,
                                ossim_uint8 thickness=1);

   ossimGeoAnnotationPolyObject(const std::vector<ossimGpt>& groundPts,
                                bool enableFill = false,
                                ossim_uint8 r=255,
                                ossim_uint8 g=255,
                                ossim_uint8 b=255,
                                ossim_uint8 thickness=1);

   ossimGeoAnnotationPolyObject(const ossimGeoAnnotationPolyObject& rhs);


   virtual ossimObject* dup()const;

   virtual void applyScale(double x, double y);

   virtual void transform(ossimImageGeometry* projection);

   virtual std::ostream& print(std::ostream& out)const;

   virtual void draw(ossimRgbImage& anImage)const;

   virtual ossimAnnotationObject* getNewClippedObject(
      const ossimDrect& rect)const;

   virtual bool intersects(const ossimDrect& rect)const;
   
   virtual void getBoundingRect(ossimDrect& rect)const;

   virtual void computeBoundingRect();

   virtual const std::vector<ossimGpt>& getPolygon()const;
   virtual void setPolygon(const std::vector<ossimGpt>& poly);

   virtual void setFillFlag(bool flag);

   virtual void setColor(ossim_uint8 r,
                         ossim_uint8 g,
                         ossim_uint8 b);
   
   virtual void setThickness(ossim_uint8 thickness);

   /**
    * Saves the current state of this object.
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;

   /**
    * Method to the load (recreate) the state of an object from a keyword
    * list.  Return true if ok or false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);

   virtual std::vector<ossimGpt> getPoints() { return thePolygon; }

   virtual ossimPolyType getPolyType () { return m_PolyType; }

   virtual void setPolyType(ossimPolyType polyType) { m_PolyType = polyType; }
   
protected:
   virtual ~ossimGeoAnnotationPolyObject();

   std::vector<ossimGpt>      thePolygon;
   ossimRefPtr<ossimAnnotationPolyObject> theProjectedPolyObject;
   ossimPolyType              m_PolyType;

TYPE_DATA
};

#endif
