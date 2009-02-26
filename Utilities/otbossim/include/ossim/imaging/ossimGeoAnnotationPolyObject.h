//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimGeoAnnotationPolyObject.h 13349 2008-07-30 15:34:34Z dburken $
#ifndef ossimGeoAnnotationPolyObject_HEADER
#define ossimGeoAnnotationPolyObject_HEADER

#include <vector>

#include <ossim/base/ossimGpt.h>
#include <ossim/imaging/ossimGeoAnnotationObject.h>
class ossimAnnotationPolyObject;

class OSSIMDLLEXPORT ossimGeoAnnotationPolyObject : public ossimGeoAnnotationObject
{
public:
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

   virtual ~ossimGeoAnnotationPolyObject();

   virtual ossimObject* dup()const;

   virtual void applyScale(double x, double y);

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
   
protected:
   std::vector<ossimGpt>      thePolygon;
   ossimAnnotationPolyObject* theProjectedPolyObject;

TYPE_DATA
};

#endif
