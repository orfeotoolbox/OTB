//*******************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Description:
//
// Contains class declaration of ossimUsgsDemTileSource.
// The ossimUsgsDemTileSource class is derived from ossimImageHandler and
// is intended to be an image handler for USGS dem files.
//
//********************************************************************
// $Id: ossimUsgsDemTileSource.h 19640 2011-05-25 15:58:00Z oscarkramer $

#ifndef ossimUsgsDemTileSource_HEADER
#define ossimUsgsDemTileSource_HEADER

#include <ossim/imaging/ossimImageHandler.h>

class ossimImageData;
class ossimDemGrid;

class OSSIM_DLL ossimUsgsDemTileSource : public ossimImageHandler
{
public:
      
   ossimUsgsDemTileSource();

   virtual ossimString getShortName() const;
   virtual ossimString getLongName()  const;

   /**
    *  Returns true if the "theImageFile can be opened.
    *
    *  If the extension is not equal to ".dem" this will attempt to look for
    *  an ".omd" file with the keyword pair "dem_type:  usgs_dem" in it.
    *
    *  Optionally users can change the scalar type from signed 16 bit to
    *  float with the keyword pair "scalar_type: ossim_float32"
    */
   virtual bool open();

   virtual ossimRefPtr<ossimImageData> getTile(const  ossimIrect& tile_rect,
                                               ossim_uint32 resLevel=0);

   /**
    * Method to get a tile.   
    *
    * @param result The tile to stuff.  Note The requested rectangle in full
    * image space and bands should be set in the result tile prior to
    * passing.  It will be an error if:
    * result.getNumberOfBands() != this->getNumberOfOutputBands()
    *
    * @return true on success false on error.  If return is false, result
    *  is undefined so caller should handle appropriately with makeBlank or
    * whatever.
    */
   virtual bool getTile(ossimImageData* result, ossim_uint32 resLevel=0);
   
   /**
    *  Returns the number of bands in the image.
    *  Satisfies pure virtual requirement from ImageHandler class.
    */
   virtual ossim_uint32 getNumberOfInputBands()  const;
   virtual ossim_uint32 getNumberOfOutputBands() const;
   
   /**
    *  Returns the number of lines in the image.
    *  Satisfies pure virtual requirement from ImageHandler class.
    */
   virtual ossim_uint32 getNumberOfLines(ossim_uint32 reduced_res_level = 0) const;
   
   /**
    *  Returns the number of samples in the image.  
    *  Satisfies pure virtual requirement from ImageHandler class.
    */
   virtual ossim_uint32 getNumberOfSamples(ossim_uint32 reduced_res_level = 0) const;

   virtual double getNullPixelValue(ossim_uint32 band=0)const;
   virtual double getMinPixelValue(ossim_uint32 band=0)const;
   virtual double getMaxPixelValue(ossim_uint32 band=0)const;

   /**
    * Returns the zero based image rectangle for the reduced resolution data
    * set (rrds) passed in.  Note that rrds 0 is the highest resolution rrds.
    */
   virtual ossimIrect getImageRectangle(ossim_uint32 reduced_res_level = 0) const;
   
   /**
    * Method to save the state of an object to a keyword list.
    * Return true if ok or false on error.
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;
   
   /**
    * Method to the load (recreate) the state of an object from a keyword
    * list.  Return true if ok or false on error.
    *
    * Keywords picked up by this method:
    * dem_type: usgs_dem
    * scalar_type: ossim_sint16 (default)
    * or
    * scalar_type: ossim_float32
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);
   
   
   /**
    * Returns the output pixel type of the tile source.
    */
   virtual ossimScalarType getOutputScalarType() const;

   /**
    * Returns the width of the output tile.
    */
   virtual ossim_uint32 getTileWidth() const;

   /**
    * Returns the height of the output tile.
    */
   virtual ossim_uint32 getTileHeight() const;

   /**
    * Returns the tile width of the image or 0 if the image is not tiled.
    * Note: this is not the same as the ossimImageSource::getTileWidth which
    * returns the output tile width which can be different than the internal
    * image tile width on disk.
    */
   virtual ossim_uint32 getImageTileWidth() const;

   /**
    * Returns the tile width of the image or 0 if the image is not tiled.
    * Note: this is not the same as the ossimImageSource::getTileWidth which
    * returns the output tile width which can be different than the internal
    * image tile width on disk.
    */
   virtual ossim_uint32 getImageTileHeight() const;   

   virtual bool isOpen()const;
   
   /**
    * Override base-class method to make sure the internal geometry is explored before extending.
    */
   virtual ossimRefPtr<ossimImageGeometry> getImageGeometry();

protected:

   /**
    * @brief Initializes theGeometry from USGS DEM header.
    *
    * This is called by base ossimImageHandler::getImageGeometry if
    * theGeometry is not set.  External callers should not go through this
    * method but call "getImageGeometry" instead for efficiency reasons.
    * 
    * @return Pointer to geometry or null if header not found.
    */
   virtual ossimRefPtr<ossimImageGeometry> getInternalImageGeometry() const;
   
   virtual ~ossimUsgsDemTileSource();
   // Disallow operator= and copy constrution...
   const ossimUsgsDemTileSource& operator=(const  ossimUsgsDemTileSource& rhs);
   ossimUsgsDemTileSource(const ossimUsgsDemTileSource&);
   
   /**
    *  Returns true on success, false on error.
    */
   template <class T> bool fillBuffer(T, // dummy template variable
                                      const ossimIrect& tile_rect,
                                      const ossimIrect& clip_rect,
                                      ossimImageData* tile);

   void gatherStatistics();

   ossimDemGrid*               theDem;
   ossimRefPtr<ossimImageData> theTile;
   ossim_float64               theNullValue;
   ossim_float64               theMinHeight;
   ossim_float64               theMaxHeight;
   bool                        theFeetFlag;  // elevation units = feet
   bool                        theIsDemFlag;

   /**
    * This can be either OSSIM_SINT16 or OSSIM_FLOAT32.  Default = OSSIM_SINT16
    * Can be changed via ".omd" file keyword:
    */
   ossimScalarType             theScalarType;
   
TYPE_DATA
};

#endif  // #ifndef ossimUsgsDemTileSource_HEADER
