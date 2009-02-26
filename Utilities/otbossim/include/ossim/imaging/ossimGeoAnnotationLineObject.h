//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks)
// Description:
//
//*************************************************************************
// $Id: ossimGeoAnnotationLineObject.h 13349 2008-07-30 15:34:34Z dburken $
#ifndef ossimGeoAnnotationLineObject_HEADER
#define ossimGeoAnnotationLineObject_HEADER
#include <ossim/base/ossimGpt.h>
#include <ossim/imaging/ossimGeoAnnotationObject.h>

class OSSIMDLLEXPORT ossimAnnotationLineObject;

class ossimGeoAnnotationLineObject : public ossimGeoAnnotationObject
{
public:
   ossimGeoAnnotationLineObject(const ossimGpt& start,
                                const ossimGpt& end,
                                unsigned char r=255,
                                unsigned char g=255,
                                unsigned char b=255,
                                long thickness=1);
   ossimGeoAnnotationLineObject(const ossimGeoAnnotationLineObject& rhs);
   virtual ~ossimGeoAnnotationLineObject();

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
   virtual bool intersects(const ossimDrect& rect)const;
   virtual ossimAnnotationObject* getNewClippedObject(const ossimDrect& rect)const;
   virtual void draw(ossimRgbImage& anImage)const;
   virtual void getBoundingRect(ossimDrect& rect)const;
   virtual void computeBoundingRect();

protected:
   ossimAnnotationLineObject* theProjectedLineObject;
   ossimGpt                   theStart;
   ossimGpt                   theEnd;

TYPE_DATA
};

#endif
