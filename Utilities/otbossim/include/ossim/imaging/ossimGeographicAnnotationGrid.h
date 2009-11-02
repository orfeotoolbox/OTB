#ifndef ossimGeographicAnnotationGrid_HEADER
#define ossimGeographicAnnotationGrid_HEADER
#include <ossim/imaging/ossimAnnotationObject.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimGrect.h>
#include <ossim/base/ossimDrect.h>

class ossimMapProjection;

class ossimGeographicAnnotationGrid : public ossimAnnotationObject
{
public:
   ossimGeographicAnnotationGrid();

   virtual void draw(ossimRgbImage& anImage)const;

   /*!
    * We need to know what projection the view
    * and we need to know the bounding rect for
    * the view in geographic space.  The bounding
    * rect is there for an area of interest.
    */
   virtual void setViewProjectionInformation(ossimMapProjection* projection,
                                             const ossimGrect& boundingGroundRect);

   virtual void getBoundingRect(ossimDrect& rect)const;
   virtual void computeBoundingRect();
   virtual std::ostream& print(std::ostream& out)const;

protected:
   /*!
    * Holds the view projection.
    */
   ossimRefPtr<ossimMapProjection> theViewProjection;

   ossimGrect       theGroundRect;

   /*!
    * This will be computed based on the bounding
    * ground and projection.
    */
   ossimDrect       theBoundingRect;
   
   /*!
    * Will hold the spacing of the grid cells in
    * geographic lat lon change.
    */
   double theDeltaLatSpacing;
   double theDeltaLonSpacing;
};

#endif
