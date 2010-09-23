//*******************************************************************
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author: David Burken
//
// Description:
//
// Contains class declaration of ossimDtedTileSource.  The ossimDtedTileSource
// class is derived from ossimImageHandler and is intended to be an image
// handler for DTED files.
//
//********************************************************************
// $Id: ossimDtedTileSource.h 17932 2010-08-19 20:34:35Z dburken $

#ifndef ossimDtedTileSource_HEADER
#define ossimDtedTileSource_HEADER

#include <fstream>

#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/support_data/ossimDtedVol.h>
#include <ossim/support_data/ossimDtedHdr.h>
#include <ossim/support_data/ossimDtedUhl.h>
#include <ossim/support_data/ossimDtedDsi.h>
#include <ossim/support_data/ossimDtedAcc.h>

class ossimImageData;

class OSSIM_DLL ossimDtedTileSource : public ossimImageHandler
{
public:
      
   ossimDtedTileSource();

   enum
   {
      DATA_RECORD_OFFSET_TO_POST = 8,  // bytes
      DATA_RECORD_CHECKSUM_SIZE  = 4,  // bytes
      POST_SIZE                  = 2,  // bytes
      NULL_PIXEL                 = -32767
   };
   
   virtual ossimString getShortName()const;
   virtual ossimString getLongName()const;

   /**
    *  Returns true if "theImageFile" can be opened and is a valid adrg file.
    */
   virtual bool open();
   virtual void close();

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
   virtual ossim_uint32 getNumberOfInputBands() const;
   
   virtual ossim_uint32 getNumberOfOutputBands()const;
   
   /**
    *  Returns the number of lines in the image.
    *  Satisfies pure virtual requirement from ImageHandler class.
    */
   virtual ossim_uint32 getNumberOfLines(
      ossim_uint32 reduced_res_level = 0) const;
   
   /**
    *  Returns the number of samples in the image.  
    *  Satisfies pure virtual requirement from ImageHandler class.
    */
   virtual ossim_uint32 getNumberOfSamples(
      ossim_uint32 reduced_res_level = 0) const;

   virtual double getNullPixelValue(ossim_uint32 band=0)const;
   virtual double getMinPixelValue(ossim_uint32 band=0)const;
   virtual double getMaxPixelValue(ossim_uint32 band=0)const;

   /**
    * Returns the zero based image rectangle for the reduced resolution data
    * set (rrds) passed in.  Note that rrds 0 is the highest resolution rrds.
    */
   virtual ossimIrect getImageRectangle(
      ossim_uint32 reduced_res_level = 0) const;
   
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
   
   //! Returns the image geometry object associated with this tile source or NULL if not defined.
   //! The geometry contains full-to-local image transform as well as projection (image-to-world)
   virtual ossimRefPtr<ossimImageGeometry> getImageGeometry();
   
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
    * Initializes spacing to the post spacing of the cell.
    *
    * @param postSpacing ossimDpt to initialize with spacing where
    * postspacing.x = longitude interval and postspacing.y = latitude interval.
    */
   void getPostSpacing(ossimDpt& postSpacing) const;

   /**
    * @brief Gets a property for name.
    * @param name Property name to get.
    * @return ossimRefPtr<ossimProperty> Note that this can be empty if
    * property for name was not found.
    */
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   /**
    * @brief Adds this class's properties to list.
    * @param propertyNames list to append to.
    */
   virtual void getPropertyNames(
      std::vector<ossimString>& propertyNames)const;

   /**
    * @brief Sets the min/max.
    *
    * looks for an omd or statistics file for min/max.  If not found
    * calls gatherStatistics method.  Overrides ossimImageHandler::loadMetaData
    * called by completeOpen.
    */
   virtual void loadMetaData();  
   
protected:
   virtual ~ossimDtedTileSource();

   // Disallow operator= and copy constrution...
   const ossimDtedTileSource& operator=(const  ossimDtedTileSource& rhs);
   ossimDtedTileSource(const ossimDtedTileSource&);

   /**
    * Allocated the tiles.
    */
   void allocate();

   /**
    *  Returns true on success, false on error.
    */
   bool fillBuffer(const ossimIrect& tile_rect,
                   const ossimIrect& clip_rect,
                   ossimImageData* tile);

   
   /**
    * @brief Scans for min and max.
    * @param writeStatsFile If true and statistics file does not exists it
    * will write one; else, no file will be written.
    */
   void gatherStatistics(bool writeStatsFile);
   
   ossim_sint16 convertSignedMagnitude(ossim_uint16& s) const;
   
   ossimRefPtr<ossimImageData> theTile;
   mutable std::ifstream      theFileStr;
   ossim_uint32               theTileWidth;
   ossim_uint32               theTileHeight;   
   ossim_uint32               theNumberOfLines;
   ossim_uint32               theNumberOfSamps;
   ossim_uint32               theOffsetToFirstDataRecord;
   ossim_uint32               theDataRecordSize;
   ossim_sint16               theMinHeight;
   ossim_sint16               theMaxHeight;
   ossimDpt                   thePostSpacing;
   
   ossimDtedVol m_vol;
   ossimDtedHdr m_hdr;
   ossimDtedUhl m_uhl;
   ossimDtedDsi m_dsi;
   ossimDtedAcc m_acc;
   /**
    *  DTED data is always stored as BIG_ENDIAN.  This flag indicates that
    *  byte swapping is needed (LITTLE_ENDIAN machine).
    */
   bool  theSwapBytesFlag;    

TYPE_DATA
};

inline ossim_sint16 ossimDtedTileSource::convertSignedMagnitude(ossim_uint16& s) const
{
   // DATA_VALUE_MASK 0x7fff = 0111 1111 1111 1111
   // DATA_SIGN_MASK  0x8000 = 1000 0000 0000 0000
   
   // First check to see if the bytes need swapped.
   s = (theSwapBytesFlag ? ( ((s&0x00ff) << 8) | ((s&0xff00) >> 8) ) : s);

   // If the sign bit is set, mask it out then multiply by negative one.
   if (s & 0x8000)
   {
      return (static_cast<ossim_sint16>(s & 0x7fff) * -1);
   }
   
   return static_cast<ossim_sint16>(s);
}

#endif
