//*******************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimImageSource.h 21637 2012-09-06 21:17:57Z dburken $
#ifndef ossimImageSource_HEADER
#define ossimImageSource_HEADER 1

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimSource.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/imaging/ossimImageGeometry.h>

class ossimDpt;

class OSSIMDLLEXPORT ossimImageSource : public ossimSource
{
public:
   ossimImageSource(ossimObject* owner = 0);
   ossimImageSource(ossimObject* owner,
                    ossim_uint32 inputListSize,
                    ossim_uint32 outputListSize,
                    bool inputListIsFixedFlag=true,
                    bool outputListIsFixedFlag=true);
   virtual ~ossimImageSource();

   /**
    * @return from origin out to tile_width-1, and tile_height-1.
    */
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIpt& origin,
                                               ossim_uint32 resLevel=0);
   
  /**
   * @return the requested region of interest
   */
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& rect,
                                               ossim_uint32 resLevel=0);

   /**
    * Method to get a tile.   
    *
    * @param result The tile to stuff.  Note The requested rectangle in full
    * image space and bands should be set in the result tile prior to
    * passing.
    *
    * @return true on success false on error.  If return is false, result
    *  is undefined so caller should handle appropriately with makeBlank or
    * whatever.
    *
    * @note Derived classes should override this method to most efficiently
    * stuff result.
    */
   virtual bool getTile(ossimImageData* result, ossim_uint32 resLevel=0);
   
  /**
   * For RTTI support. overrides ossimImageSource
   * get object to return the correct casted base pointer
   * for RTTI casting
   */
   virtual ossimObject* getObject() { return this; }
   
   /**
    * For RTTI support. overrides ossimImageSource
    * get object to return the correct casted base pointer
    * for RTTI casting
    */
   virtual const ossimObject* getObject() const { return this; }

 
  /**
   * Will return the decimation factor for the given resolution 
   * level.  the decimation is the scale from Resolution 0 or full 
   * res.  Usually this is a power of 2 decimation where
   * the decimation result is 1.0/2^resoltion.
   */
   virtual void getDecimationFactor(ossim_uint32 resLevel,
                                    ossimDpt& result)const;
  
   /**
    * Will return an array of all decimations for each resolution level.
    */
   virtual void getDecimationFactors(std::vector<ossimDpt>& decimations) const;

   /**
    * Will return the number of resolution levels.  Note: resolution
    * level 0 is included in the return count.
    */
   virtual ossim_uint32 getNumberOfDecimationLevels() const;
   
   /*!
    * Returns the number of bands available from the input.
    */
   virtual ossim_uint32 getNumberOfInputBands() const = 0;
   
   /**
    * Returns the number of bands in a tile returned from this TileSource.
    */
   virtual ossim_uint32 getNumberOfOutputBands() const;

   /**
    * @brief Initializes bandList.
    * 
    * This implementation initializes to the zero based order of input bands.
    * Derived classes that are band selectors should override.
    *
    * @param bandList Initialized by this.
    */
   virtual void getOutputBandList( std::vector<ossim_uint32>& bandList ) const;

   /**
    * This will be used to query the output pixel type of the tile source.
    * Please ignore the argument.  It will soon be removed.
    */
   virtual ossimScalarType getOutputScalarType() const;

   /**
    * Returns the default processing tile width
    */
   virtual ossim_uint32 getTileWidth() const;

   /**
    * Returns the default processing tile height
    */
   virtual ossim_uint32 getTileHeight() const;

   /**
    * Each band has a null pixel associated with it.  The null pixel 
    * represents an invalid value.
    */ 
   virtual double getNullPixelValue(ossim_uint32 band=0)const;
   
   /**
    * Returns the min pixel of the band.
    */
   virtual double getMinPixelValue(ossim_uint32 band=0)const;
   
   /**
    * Returns the max pixel of the band.
    */
   virtual double getMaxPixelValue(ossim_uint32 band=0)const;
   
   /**
    * This will return the bounding rect of the source.  We can have several
    * sources which are in a chain to modify the bounding image rect.
    * lets say you are next to an image handler then it will return the
    * bounding rect for that image.  If you are at the right side of a
    * resampler then you will get a bounding rect along the image view plane.
    * This is going to be a very import method for both image writers,
    * mosaics or anything that needs to operate only within the bounds of an
    * image.
    */
   virtual ossimIrect getBoundingRect(ossim_uint32 resLevel=0) const;
   
   /**
    * @brief Gets the bounding rectangle of the source.
    *
    * This default implementation calls the getBoundingRect interface that
    * returns a rectangle.  Derived classes should override as needed.
    * 
    * @param rect Initialized with bounding rectangle by this.
    * @param resLevel Reduced resolution level if applicable.
    */
   virtual void getBoundingRect(ossimIrect& rect,
                                ossim_uint32 resLevel=0) const;
   
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
    * ordering specifies how the vertices should be arranged.
    * valid image vertices is basically the tightly fit convex hull
    * of the image.  Usually an image has NULL values and are
    * internally not upright rectangular.  This can cause
    * problems some spatial filters.
    *
    * The default implementation is to return the bounding rect.
    */
   virtual void getValidImageVertices(
      std::vector<ossimIpt>& validVertices,
      ossimVertexOrdering ordering=OSSIM_CLOCKWISE_ORDER,
      ossim_uint32 resLevel=0)const;

   /**
    * Returns the image geometry object associated with this tile source or
    * NULL if not defined. The geometry contains full-to-local image transform
    * as well as projection (image-to-world). Default implementation returns
    * the image geometry object associated with the next  
    * (left) input source (if any) connected to this source in the chain, or
    * NULL.
    */
   virtual ossimRefPtr<ossimImageGeometry> getImageGeometry();

   /**
    * Default implementation sets geometry of the first input to the geometry
    * specified.
    */
   virtual void setImageGeometry(const ossimImageGeometry* geom);
       
   /** Default method to call input's saveImageGeometry. */
   virtual void saveImageGeometry() const;
   
   /** Default method to call input's saveImageGeometry. */
   virtual void saveImageGeometry(const ossimFilename& geometry_file) const;
   
   virtual void initialize()=0;
   
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   virtual void setProperty(ossimRefPtr<ossimProperty> property);
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;

   /**
    * @return Flag indicating the data contains pallete indexes.
    *
    * This should be replaced by a getPhotoInterpretation().
    */
   virtual bool isIndexedData() const;
   
protected:

   ossimImageSource (const ossimImageSource& rhs);
   const ossimImageSource& operator= (const ossimImageSource&);

private:

TYPE_DATA
};

#endif /* #ifndef ossimImageSource_HEADER */
