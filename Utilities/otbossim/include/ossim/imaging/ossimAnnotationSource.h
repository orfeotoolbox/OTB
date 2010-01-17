//*******************************************************************
//
// License:  See LICENSE.txt file in the top level directory.
//
// Author: Garrett Potts
//
// Description:
//
// Class to annotate or draw things on tiles like text, ellipses and so on.
//
// Notes:
// 1) The output scalar type of this object is ALWAYS 8 bit or OSSIM_UINT8 so
//    if your input connection is something other than 8 bit it will be
//    remapped to 8 bit.
// 2) This can handle any number of input bands; however, it will never draw
//    to more than three.  So if you have an input connection of four bands
//    the fourth band will not be drawn to.
//
//*************************************************************************
// $Id: ossimAnnotationSource.h 15766 2009-10-20 12:37:09Z gpotts $
#ifndef ossimAnnotationSource_HEADER
#define ossimAnnotationSource_HEADER

#include <vector>
#include <ossim/imaging/ossimImageSourceFilter.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimDrect.h>
#include <ossim/imaging/ossimRgbImage.h>
#include <ossim/imaging/ossimAnnotationObject.h>

class ossimKeywordlist;

class OSSIMDLLEXPORT ossimAnnotationSource : public ossimImageSourceFilter
{
public:
   typedef std::vector<ossimRefPtr<ossimAnnotationObject> > AnnotationObjectListType;
   ossimAnnotationSource(ossimImageSource* inputSource=0);
   virtual ~ossimAnnotationSource();

   virtual ossim_uint32 getNumberOfOutputBands() const;

   virtual ossimScalarType getOutputScalarType() const;

   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& tileRect,
                                               ossim_uint32 resLevel = 0);
   
   /*!
    * Will allow this source to initialize its data objects.  If it needs to
    * it should call its connection to get what it needs.
    */
   virtual void initialize();

   /*!
    * Will return the bounding rectangle.
    */
   virtual ossimIrect getBoundingRect(ossim_uint32 resLevel=0)const;

   /*!
    * @param bands This can be any number of bands; however, the
    * annotator will only write to a max of 3, so if you have a 4 band
    * image the fourth band will not be annotated.
    */
   virtual void setNumberOfBands(ossim_uint32 bands);
   
   /*!
    * Will add an object to the list.
    * it will own the object added.
    */
   virtual bool addObject(ossimAnnotationObject* anObject);
   
   /*!
    * Will delete an object from the list.
    * Note: the passed in pointer is no long
    *       valid.
    */
   virtual bool deleteObject(ossimAnnotationObject* anObject);

   /*!
    * Saves the current state of this object.
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;

   /*!
    * Method to the load (recreate) the state of an object from a keyword
    * list.  Return true if ok or false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);

   virtual void computeBoundingRect();

   /*!
    * Calls the isPointWithin all Annotation
    * Objects.  Note:  Do not delete these objects.
    */
   AnnotationObjectListType pickObjects(const ossimDpt& imagePoint);

   /*!
    * Note: do not delete the objects returned.
    */
   AnnotationObjectListType pickObjects(const ossimDrect& imageRect);

   const AnnotationObjectListType& getObjectList()const;

   AnnotationObjectListType& getObjectList();

   /*!
     Clears theAnnotationObjectList vector.  All elements are deleted.
   */
   void deleteAll();
   
   virtual void drawAnnotations(ossimRefPtr<ossimImageData> tile);
   
protected:
   
   void allocate(const ossimIrect& rect);
   void destroy();
   
   ossimAnnotationSource(const ossimAnnotationSource& rhs):ossimImageSourceFilter(rhs){}

   /*!
    * What is the size of the image.  This class
    * is an image producer and it needs to know about
    * its bounds. We will default the rectangle to an
    * empty region.
    */
   ossimDrect theRectangle;
   
   /*!
    * The output band can be set to 1 or 3 for
    * grey or RGB scale. We will default this to
    * a 1 band image.
    */
   ossim_uint32 theNumberOfBands;

   /*!
    * This has all the routines we need for drawing
    * lines, circles and polygons.
    */
   ossimRefPtr<ossimRgbImage> theImage;
   
   ossimRefPtr<ossimImageData> theTile;

   /*!
    * These will be all your primitives in
    * Example: lines, circles, text ...
    */
   AnnotationObjectListType theAnnotationObjectList;   

TYPE_DATA
};

#endif /* #ifndef ossimAnnotationSource_HEADER */

