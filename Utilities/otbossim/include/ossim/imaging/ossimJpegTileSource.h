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
// Contains class declaration for JpegTileSource.
// JpegTileSource is derived from ImageHandler which is derived from
// TileSource.
//*******************************************************************
//  $Id: ossimJpegTileSource.h 16860 2010-03-11 15:28:09Z gpotts $

#ifndef ossimJpegTileSource_HEADER
#define ossimJpegTileSource_HEADER

#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimAppFixedTileCache.h>

class  ossimImageData;

class OSSIM_DLL ossimJpegTileSource : public ossimImageHandler
{
public:

   ossimJpegTileSource();

   ossimJpegTileSource(const ossimKeywordlist& kwl,
                       const char* prefix=0);

   ossimJpegTileSource(const char* jpeg_file);


   virtual ossimString getShortName() const;
   virtual ossimString getLongName()  const;

   /**
    *  Returns a pointer to a tile given an origin representing the upper
    *  left corner of the tile to grab from the image.
    *  Satisfies pure virtual from TileSource class.
    */
   virtual ossimRefPtr<ossimImageData> getTile(const  ossimIrect& rect,
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
   virtual ossim_uint32 getNumberOfOutputBands()const;

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
    */
   virtual ossim_uint32 getImageTileWidth() const;

   /**
    * Returns the tile width of the image or 0 if the image is not tiled.
    * Note: this is not the same as the ossimImageSource::getTileWidth which
    * returns the output tile width which can be different than the internal
    * image tile width on disk.
    */
   virtual ossim_uint32 getImageTileHeight() const;

   bool isValidRLevel(ossim_uint32 reduced_res_level) const;


   bool isOpen()const;
   /**
    *  Returns true if the image_file can be opened and is a valid tiff file.
    */
   bool open(const ossimFilename& jpeg_file);
   virtual void close();

   /**
    * @brief Gets a property for matching name.
    * @param name The name of the property to get.
    * @return Returns property matching "name".
    */
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   
   /**
    * @brief Gets a list of property names available.
    * @param propertyNames The list to push back names to.
    */
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;
protected:
   virtual ~ossimJpegTileSource();
	class PrivateData;
   /**
    *  Returns true if no errors initializing object.
    *
    *  Notes:
    *  - Callers of this method must ensure "theTiffPtr" data member
    *    is initialized.
    *  - This method was added to consolidate object initialization code
    *    between constructor and public open method.
    */
   virtual bool open();

   void allocate();
   void destroy();
   void restart();

   /**
    * @note this method assumes that setImageRectangle has been called on
    * theTile.
    */
   void fillTile(const ossimIrect& clip_rect, ossimImageData* tile);

   ossimRefPtr<ossimImageData>  theTile;
   ossimRefPtr<ossimImageData>  theCacheTile;
   ossim_uint8*                 theLineBuffer;
   FILE*                        theFilePtr;
   ossimIrect                   theBufferRect;
   ossimIrect                   theImageRect;
   ossim_uint32                 theNumberOfBands;
   ossimIpt                     theCacheSize;

   PrivateData*                 thePrivateData;

   ossimAppFixedTileCache::ossimAppFixedCacheId theCacheId;

TYPE_DATA
};

#endif
