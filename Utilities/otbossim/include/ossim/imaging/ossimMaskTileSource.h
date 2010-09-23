//*******************************************************************
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Mingjie Su
//
// Description:
//
// Contains class declaration for ossimMaskTileSource.
// ossimMaskTileSource is derived from ossimGeneralRasterTileSource
//*******************************************************************
//  $Id: ossimMaskTileSource.h 1185 2010-07-28 18:22:03Z ming.su $

#ifndef ossimMaskTileSource_HEADER
#define ossimMaskTileSource_HEADER

#include <ossim/imaging/ossimImageHandler.h>

class OSSIM_DLL ossimMaskTileSource : public ossimImageHandler
{
public:

   ossimMaskTileSource();

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
  
   /**
    * Override base getXXXXPixValue methods since the null/min/max can be set
    * to something different.  Currently returns the same value for all bands.
    */
   virtual double getNullPixelValue(ossim_uint32 band=0)const;
   virtual double getMinPixelValue(ossim_uint32 band=0)const;
   virtual double getMaxPixelValue(ossim_uint32 band=0)const;

   void setImageBoundingRect(ossimIrect rect);

   bool isSupportedExtension();

   static const ossim_uint32  DEFAULT_CHAR_SIZE;
   
protected:
   virtual ~ossimMaskTileSource();
   /**
    * @note this method assumes that setImageRectangle has been called on
    * theTile.
    */
  template <class T>
  void fillTile(T, // dummy template variable
                const ossimIrect& tile_rect, 
                ossimImageData* tile,
                ossim_uint32 resLevel);

  ossim_uint32 getPixelValue(ossim_uint32 index,
                             ossim_uint32 samplePos,
                             ossim_uint32 linePos,
                             ossim_uint32 pixelOffset);

   ossimRefPtr<ossimImageData>  theTile;
   ossim_uint32                 m_numberOfSamples;
   ossim_uint32                 m_numberOfLines;
   char*                        m_buffer;
   ossim_uint32                 m_bufferSize;
  
TYPE_DATA
};

#endif

