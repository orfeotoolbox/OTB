//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author:  David Burken
// 
// Description:
//
// Abstract class for rendering vector data.  Derived classes should implement
// the "rasterizeVectorData" data method.
//               
//*******************************************************************
// $Id: ossimVectorRenderer.h 15766 2009-10-20 12:37:09Z gpotts $
#ifndef ossimVectorRenderer_HEADER
#define ossimVectorRenderer_HEADER

#include <ossim/imaging/ossimImageSourceFilter.h>
#include <ossim/base/ossimDrect.h>
#include <ossim/base/ossimDpt3d.h>

class ossimImageViewTransform;
class ossimImageData;

class ossimVectorRenderer : public ossimImageSourceFilter
{
public:
   ossimVectorRenderer();
   ossimVectorRenderer(ossimImageSource* inputSource,
                       ossimImageViewTransform *transform = NULL);
      
   /**
    * Renders the vector data to a data object.  The area which is
    * imaged is the area of interest.  The scale is determined by the output
    * view projection of the ImageViewTransform.
    */
   virtual ossimImageData* renderVectorData() const = 0;

   /**
    * Returns vector of ossimDpt3d points representing the data object at
    * the index passed in.  Implementors should set an error and return an
    * empty vector if the index is out of range.
    */
   virtual vector<ossimDpt3d> getObject(ossim_uint32 index) const = 0;
   
   /**
    * Method to save the state of an object to a keyword list.
    * Return true if ok or false on error.
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;
   
   /**
    * Method to the load (recreate) the state of an object from a keyword
    * list.  Return true if ok or false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);

   /**
    * Sets the ImageViewTransform to be used for converting vector data.
    */
   virtual void setImageViewTransform(ossimImageViewTransform* transform);

   /**
    * Sets the area of interest.  This will be used by the renderVectorData.
    */
   virtual void setAreaOfInterest(const ossimDrect& aoi);

   /**
    * Returns the current area of interest as an ossimDrect.
    */
   virtual ossimDrect getAreaOfInterest() const;

   /**
    * Returns the full bounding rectangle of current vector data loaded.
    */
   virtual ossimDrect getBoundingRect(long resLevel=0)const;

   /**
    * Returns the number of vector objects in the area of interest.
    */
   virtual ossim_int32 getNumberOfAoiObjects() const;

   /**
    * Returns the total number of vector objects in the full bounding
    * rectangle.
    */
   virtual ossim_int32 getNumberOfObjects() const;

protected:
   virtual ~ossimVectorRenderer();
   
   /**
    * Sets the bounding rectangle of current vector data loaded.
    */
   virtual void setBoundingRect(ossimDrect& rect);

   /**
    * Sets the number of vector objects in the area of interest.
    */
   virtual void setNumberOfAoiObjects(ossim_int32 number_of_objects);
 
   /**
    * Sets the number of vector objects in the full bounding rectangle.
    */
   virtual void setNumberOfObjects(ossim_int32 number_of_objects);
   
private:

   ossimImageViewTransform*  theImageViewTransform;
   ossimDrect                theBoundingRect;
   ossimDrect                theAreaOfInterest;
   ossim_int32               theNumberOfAoiObjects;
   ossim_int32               theNumberOfObjects;
   
TYPE_DATA
};

#endif
