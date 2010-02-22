//----------------------------------------------------------------------------
// Copyright (c) 2005, David Burken, all rights reserved.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: Class declaration of ossimWatermarkFilter.
// Applies an image or watermark to image.  Positioning is based on mode.
// Density is base on alpha weight.
//
//----------------------------------------------------------------------------
// $Id: ossimWatermarkFilter.h 16276 2010-01-06 01:54:47Z gpotts $
#ifndef ossimWatermarkFilter_HEADER
#define ossimWatermarkFilter_HEADER

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/imaging/ossimImageSourceFilter.h>

/**
 * class ossimWatermarkFilter
 * Applies an image or watermark to image.
 * Positioning is based on mode.
 * Density is base on alpha weight.
 */
class OSSIMDLLEXPORT ossimWatermarkFilter : public ossimImageSourceFilter
{
public:

   enum WatermarkMode
   {
      UPPER_LEFT     = 0,
      UPPER_CENTER   = 1,
      UPPER_RIGHT    = 2,
      CENTER         = 3,
      LOWER_LEFT     = 4,
      LOWER_CENTER   = 5,
      LOWER_RIGHT    = 6,
      UNIFORM_DENSE  = 7,
      UNIFORM_SPARSE = 8,
      END            = 9 //< Number of modes.
   };
      
   /** contructor */
   ossimWatermarkFilter();

   /** @return Short name of filter. */
   virtual ossimString getShortName()   const;

   /** @return Long name of filter. */
   virtual ossimString getLongName()    const;

   /** @return Descriptive name of filter. */
   virtual ossimString getDescription() const;

   /**
    * @param tile_rect Rectangle to fill tile with.
    * 
    * @param resLevel Reduced resolution level to grab from.
    * 
    * @return ossimRefPtr<ossimImageData> This is tile that was filled with
    * tile_rect.
    *
    * @note Callers should check the ossimRefPtr::valid method.
    * The internal pointer of the ossimRefPtr<ossimImageData> can be
    * null if the tile_rect did not intersect the input connection's
    * bounding rectangle.
    */
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& tile_rect,
                                               ossim_uint32 resLevel=0);
   
   /**
    * Initializes state of the object from the input connection.
    */
   virtual void initialize();

   /**
    * Method to save the state of an object to a keyword list.
    * Return true if ok or false on error.
    *
    * Keywords picked saved by saveState
    * 
    * filename: my_colormap_file.jpg
    *
    * weight: .25
    * 
    * watermark_mode:  upper_left
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;

   /**
    * Method to the load (recreate) the state of the object from a keyword
    * list.
    *
    * @param kwl Keyword list to initialize from.
    *
    * @param prefix Usually something like: "object1."
    *
    * @return This method will alway return true as it is intended to be
    * used in conjuction with the set methods.
    *
    * Keywords picked up by loadState:
    * 
    * filename: my_colormap_file.jpg
    *
    * weight: .25
    * 
    * watermark_mode:  upper_left
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);

   /**
    * @param The property to get as a string like "watermark_mode".
    *
    * @return ossimRefPt<ossimProperty> holding pointer to ossimProperty
    * matching string or null pointer if string does not match.
    *
    * @note Internal pointer can be null so callers should check prior to
    * accessing like:
    * ossimRefPtr<ossimProperty> p = myFilter->getProperty(ossimString("foo"));
    * if (p.valid() == NULL)
    * {
    *    some_error;
    * }
    */
   virtual ossimRefPtr<ossimProperty> getProperty(
      const ossimString& name)const;

   /**
    * Set property.
    *
    * @param property Property to set if property->getName() matches a
    * property name of this object.
    */
   virtual void setProperty(ossimRefPtr<ossimProperty> property);

   /**
    * Adds this objects properties to the list.
    *
    * @param propertyNames Array to add to.
    *
    * @note This method does not clear propertyNames prior to adding it's
    * names.
    */
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;

   /**
    * @param list List initialized with watermark mode strings.
    *
    * @note This method clears the "list" prior to stuffing.
    */
   void getModeList(vector<ossimString>& list) const;

   /** @return theMode */
   ossimWatermarkFilter::WatermarkMode getMode() const;

   /** @return theMode as a string like "upper_left". */
   ossimString getModeString() const;

   /**
    * Sets theFilename to file.
    *
    * @param file The watermark image file.
    */
   void setFilename(const ossimFilename& file);

   /**
    * Sets the filter mode.
    *
    * @param mode String representing mode.
    */
   void setMode(const ossimString& mode);

   /**
    * Sets the weight.
    *
    * @param weight Weight to use for watermark pixels normalize between
    * 0.0 and 1.0.
    */
   void setWeight(ossim_float64 weight);
   
   /**
    * Print method.  Called by: friend operator<<
    * 
    * @return std::ostream&
    */
   virtual std::ostream& print(std::ostream& out) const;

protected:
   /** destructor */
   virtual ~ossimWatermarkFilter();

   /**
    * Allocates / recomputes things that are needed.  Clears theDirtyFlag
    * by setting it to false on success.
    *
    * @return true on success, false on error.
    */
   bool allocate();

   /**
    * Attempts to open "theFilename".  This will load "theWatermark" with
    * the image converting it to the scalar type of the input connection.
    *
    * @return true on success, false if no image handler is found for the
    * image or theInputConnection has not been established.
    */
   bool openWatermarkFile();
   
   // void verifyEnable();

   /**
    * Writes watermark(s) to theTile.
    * Performs theTile->validate() at the end if theTile was touched.
    */
   template <class T> void fill(T dummy);

   /**
    * Computes the bounding rectangles.  These are the rectangles in image
    * space of the input connection to paint the watermark(s) on.  The start of
    * each rectangle will be the start of where "theWatermark" is painted.
    */
   void getIntersectingRects(vector<ossimIrect>& rects);

   /**
    * Addes upper left watermark rectangle to rects if it intersects
    * theTile rect clipped to input bounding rect.
    *
    * @param rects Array to add rect to.
    */
   void getUpperLeftRect(vector<ossimIrect>& rects);
   
   /**
    * Addes upper center watermark rectangle to rects if it intersects
    * theTile rect clipped to input bounding rect.
    *
    * @param rects Array to add rect to.
    */
   void getUpperCenterRect(vector<ossimIrect>& rects);
   
   /**
    * Addes upper right watermark rectangle to rects if it intersects
    * theTile rect clipped to input bounding rect.
    *
    * @param rects Array to add rect to.
    */
   void getUpperRightRect(vector<ossimIrect>& rects);

   /**
    * Addes center watermark rectangle to rects if it intersects
    * theTile rect clipped to input bounding rect.
    *
    * @param rects Array to add rect to.
    */
   void getCenterRect(vector<ossimIrect>& rects);

   /**
    * Addes lower left watermark rectangle to rects if it intersects
    * theTile rect clipped to input bounding rect.
    *
    * @param rects Array to add rect to.
    */
   void getLowerLeftRect(vector<ossimIrect>& rects);

   /**
    * Addes lower center watermark rectangle to rects if it intersects
    * theTile rect clipped to input bounding rect.
    *
    * @param rects Array to add rect to.
    */
   void getLowerCenterRect(vector<ossimIrect>& rects);
   
   /**
    * Addes lower right watermark rectangle to rects if it intersects
    * theTile rect clipped to input bounding rect.
    *
    * @param rects Array to add rect to.
    */
   void getLowerRightRect(vector<ossimIrect>& rects);
   
   /**
    * Addes rectangles to rects if it intersects
    * theTile rect clipped to input bounding rect.
    *
    * This will repeat the watermark throughout the input bounding rect.
    *
    * @param rects Array to add rect to.
    */
   void getUniformDenseRects(vector<ossimIrect>& rects);
   
   /**
    * Addes rectangles to rects if it intersects
    * theTile rect clipped to input bounding rect.
    *
    * This will repeat the watermark sparsly throughout the input bounding
    * rect.
    * 
    * @param rects Array to add rect to.
    */
   void getUniformSparceRects(vector<ossimIrect>& rects);
   
   /** File name of watermark image. */
   ossimFilename      theFilename;

   /** Normalized between 0.0 and 1.0. */
   ossim_float64      theWatermarkWeight;

   /** The returned tile. */
   ossimRefPtr<ossimImageData> theTile;

   /** Tile storage for watermark image. */
   ossimRefPtr<ossimImageData> theWatermark;

   /** The number of watermark bands. */
   ossim_uint32 theWatermarkNumberOfBands;

   /** The filter mode. */
   WatermarkMode      theMode;

   /** The bounding rectangle of the input connection. */
   ossimIrect theInputBoundingRect;

   /** The number of input bands. */
   ossim_uint32 theInputNumberOfBands;

   /** The input scalar type. */
   ossimScalarType theInputScalarType;

   /**
    * Set in the initialize method this instructs the getTile that something
    * has changes and it need to call initialize.
    */
   bool theDirtyFlag;


TYPE_DATA
      
}; // End of class ossimWatermarkFilter.

#endif /* #ifndef ossimWatermarkFilter_HEADER */
