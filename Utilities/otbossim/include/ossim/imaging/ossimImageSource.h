//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimImageSource.h 9094 2006-06-13 19:12:40Z dburken $
#ifndef ossimImageSource_HEADER
#define ossimImageSource_HEADER

#include <ossim/base/ossimSource.h>
#include <ossim/base/ossimErrorContext.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimDrect.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/imaging/ossimImageSourceInterface.h>

class ossimDpt;

class OSSIMDLLEXPORT ossimImageSource : public ossimSource,
			                public ossimImageSourceInterface
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
   * For RTTI support. overrides ossimImageSourceInterface
   * get object to return the correct casted base pointer
   * for RTTI casting
   */
   virtual ossimObject* getObject() { return this; }
   
   /**
    * For RTTI support. overrides ossimImageSourceInterface
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
   virtual void getDecimationFactors(vector<ossimDpt>& decimations) const;

   /**
    * Will return the number of resolution levels.  Note: resolution
    * level 0 is included in the return count.
    */
   virtual ossim_uint32 getNumberOfDecimationLevels() const;
   
   /**
    * Returns the number of bands in a tile returned from this TileSource.
    */
   virtual ossim_uint32 getNumberOfOutputBands() const;

   /**
    * Initializes bandList to the zero based order of output bands.
    */
   virtual void getOutputBandList(std::vector<ossim_uint32>& bandList) const;

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
   virtual void getValidImageVertices(vector<ossimIpt>& validVertices,
                                      ossimVertexOrdering ordering=OSSIM_CLOCKWISE_ORDER,
                                      ossim_uint32 resLevel=0)const;

   /**
    * the default is to find the first input source that is of
    * type ossimImageSourceInterface and return its input geometry.
    */
   virtual bool getImageGeometry(ossimKeywordlist& kwl,
                                 const char* prefix=0);

   /**
    * Default method to call input's setImageGeometry.
    */
   virtual void setImageGeometry(const ossimKeywordlist& kwl);

   /**
    * Default method to call input's saveImageGeometry.
    */
   virtual void saveImageGeometry() const;
   
   /**
    * Default method to call input's saveImageGeometry.
    */
   virtual void saveImageGeometry(const ossimFilename& geometry_file) const;
   
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   virtual void setProperty(ossimRefPtr<ossimProperty> property);
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;

   
protected:
   ossimImageSource (const ossimImageSource& rhs);
   const ossimImageSource& operator= (const ossimImageSource&);

private:

TYPE_DATA
};

#endif /* #ifndef ossimImageSource_HEADER */
