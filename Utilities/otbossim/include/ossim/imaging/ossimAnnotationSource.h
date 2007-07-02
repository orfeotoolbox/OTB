//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See LICENSE.txt file in the top level directory.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimAnnotationSource.h 9094 2006-06-13 19:12:40Z dburken $
#ifndef ossimAnnotationSource_HEADER
#define ossimAnnotationSource_HEADER
#include <list>
#include <ossim/imaging/ossimImageSourceFilter.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimKeywordlist.h>


class ossimRgbImage;
class ossimAnnotationObject;

class OSSIMDLLEXPORT ossimAnnotationSource : public ossimImageSourceFilter
{
public:
   ossimAnnotationSource(ossimImageSource* inputSource=NULL);
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
    * This can be 1 or 3 bands
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
   vector<ossimAnnotationObject*> pickObjects(const ossimDpt& imagePoint);

   /*!
    * Note: do not delete the objects returned.
    */
   vector<ossimAnnotationObject*> pickObjects(const ossimDrect& imageRect);

   const vector<ossimAnnotationObject*>& getObjectList()const;

   vector<ossimAnnotationObject*>& getObjectList();

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
   ossimRgbImage* theImage;
   
   ossimRefPtr<ossimImageData> theTile;

   /*!
    * These will be all your primitives in
    * Example: lines, circles, text ...
    */
   std::vector<ossimAnnotationObject*> theAnnotationObjectList;   

TYPE_DATA
};

#endif /* #ifndef ossimAnnotationSource_HEADER */

