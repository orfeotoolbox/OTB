//*******************************************************************
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description:
//
// Contains class declaration for ossimGeneralRasterTileSource.
// ossimGeneralRasterTileSource is derived from ImageHandler which is
// derived from ossimTileSource.
//*******************************************************************
//  $Id: ossimGeneralRasterTileSource.h 21631 2012-09-06 18:10:55Z dburken $

#ifndef ossimGeneralRasterTileSource_HEADER
#define ossimGeneralRasterTileSource_HEADER 1


#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/base/ossimIoStream.h>
#include <ossim/imaging/ossimGeneralRasterInfo.h>
#include <vector>
  

class  ossimImageData;

class OSSIM_DLL ossimGeneralRasterTileSource : public ossimImageHandler
{
public:

   ossimGeneralRasterTileSource();

   virtual ossimString getShortName()const;
   virtual ossimString getLongName()const;
   
   /**
    *  Returns a pointer to a tile given an origin representing the upper
    *  left corner of the tile to grab from the image.
    *  Satisfies pure virtual from TileSource class.
    */
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& tile_rect,
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
    *  Satisfies pure virtual from ImageHandler class.
    */
   virtual ossim_uint32 getNumberOfInputBands() const;
   
   /**
    * Returns the number of bands in a tile returned from this TileSource.
    * Note: we are supporting sources that can have multiple data objects.
    * If you want to know the scalar type of an object you can pass in the 
    */
   virtual ossim_uint32 getNumberOfOutputBands() const;
   
   /**
    *  Returns the number of lines in the image.
    *  Satisfies pure virtual from ImageHandler class.
    */
   virtual ossim_uint32 getNumberOfLines(ossim_uint32 reduced_res_level = 0) const;
   
   /**
    *  Returns the number of samples in the image.  
    *  Satisfies pure virtual from ImageHandler class.
    */
   virtual ossim_uint32 getNumberOfSamples(ossim_uint32 reduced_res_level = 0) const;

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
    * Returns the output pixel type of the tile source.
    */
   virtual ossimScalarType getOutputScalarType() const;

   /**
    * Returns the width of the output tile.
    */
   virtual ossim_uint32    getTileWidth() const;
   
   /**
    * Returns the height of the output tile.
    */
   virtual ossim_uint32    getTileHeight() const;

   /**
    * Returns the tile width of the image or 0 if the image is not tiled.
    * Note: this is not the same as the ossimImageSource::getTileWidth which
    * returns the output tile width which can be different than the internal
    * image tile width on disk.
    * Note:  This handler will always return 0 for this method.
    */
   virtual ossim_uint32 getImageTileWidth() const;

   /**
    * Returns the tile width of the image or 0 if the image is not tiled.
    * Note: this is not the same as the ossimImageSource::getTileWidth which
    * returns the output tile width which can be different than the internal
    * image tile width on disk.
    * Note:  This handler will always return 0 for this method.
    */
   virtual ossim_uint32 getImageTileHeight() const;   
   
   bool isValidRLevel(ossim_uint32 reduced_res_level) const;
   
   virtual void close();
   virtual bool isOpen() const;
   virtual bool open();
   virtual bool open(const ossimGeneralRasterInfo& info);
   
   /**
    * Override base getXXXXPixValue methods since the null/min/max can be set
    * to something different.  Currently returns the same value for all bands.
    */
   virtual double getNullPixelValue(ossim_uint32 band=0)const;
   virtual double getMinPixelValue(ossim_uint32 band=0)const;
   virtual double getMaxPixelValue(ossim_uint32 band=0)const;

   virtual ossimRefPtr<ossimImageGeometry> getImageGeometry();

   /**
    * @brief Indicates whether or not the image handler can control output
    * band selection via the setOutputBandList method.
    *
    * Overrides: ossimImageHandler::isBandSelector
    *
    * @return true
    */
   virtual bool isBandSelector() const;

   /**
    * @brief If the image handler "isBandSeletor()" then the band selection
    * of the output chip can be controlled.
    *
    * Overrides: ossimImageHandler::setOutputBandList
    * 
    * @return true on success, false on error.
    */
   virtual bool setOutputBandList(const std::vector<ossim_uint32>& band_list);

   /** @brief Initializes bandList to the zero based order of output bands. */
   virtual void getOutputBandList(std::vector<ossim_uint32>& bandList) const;

protected:
   virtual ~ossimGeneralRasterTileSource();
   /**
    *  Methods return true on succes false on error.
    */
   virtual bool fillBuffer(const ossimIpt& origin, const ossimIpt& size);
   virtual bool fillBIP(const ossimIpt& origin, const ossimIpt& size);
   virtual bool fillBIL(const ossimIpt& origin, const ossimIpt& size);
   virtual bool fillBSQ(const ossimIpt& origin, const ossimIpt& size);
   virtual bool fillBsqMultiFile(const ossimIpt& origin, const ossimIpt& size);

   virtual ossimKeywordlist getHdrInfo(ossimFilename hdrFile);
   virtual ossimKeywordlist getXmlInfo(ossimFilename xmlFile);

   bool initializeHandler();
   
   ossimRefPtr<ossimImageData>              m_tile;
   ossim_uint8*                             m_buffer;
   ossim_uint8*                             m_lineBuffer;
   ossimInterleaveType                      m_bufferInterleave;
   std::vector<ossimRefPtr<ossimIFStream> > m_fileStrList;
   // std::vector< std::ifstream* >            m_fileStrList;   
   ossimGeneralRasterInfo                   m_rasterInfo;
   ossimIrect                               m_bufferRect;
   bool                                     m_swapBytesFlag;
   ossim_uint32                             m_bufferSizeInPixels;
   std::vector<ossim_uint32>                m_outputBandList;

private:
   
   /** @brief Allocates m_tile. */
   void allocateTile();

   /** @brief Allocates m_buffer */
   void allocateBuffer( const ossimImageData* tile );
   

TYPE_DATA
};

#endif /* #ifndef ossimGeneralRasterTileSource_HEADER */
