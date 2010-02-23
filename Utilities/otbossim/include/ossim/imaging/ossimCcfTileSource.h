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
// Contains class declaration for CcfTileSource.
//
// A ccf (Chip Chunk Format) is a double tiled format.  Image data is
// arranged in chips and chunks.  Each chip is 32 x 32 pixels.  Each chunk
// is 8 x 8 chips for a single band image.  Bands are interleaved by chip
// so a rgb image has a red chip a green chip, then a blue chip which
// would make the chunk 24 x 8 chips.
//
//*******************************************************************
//  $Id: ossimCcfTileSource.h 16075 2009-12-10 15:46:43Z gpotts $

#ifndef ossimCcfTileSource_HEADER
#define ossimCcfTileSource_HEADER

#include <fstream>

#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimCcfHead.h>

class  ossimImageData;

class OSSIM_DLL ossimCcfTileSource : public ossimImageHandler
{
public:

   ossimCcfTileSource();
   
   virtual ossimString getShortName()const;
   virtual ossimString getLongName()const;

   /**
    *  @return Returns true on success, false on error.
    *
    *  @note This method relies on the data member ossimImageData::theImageFile
    *  being set.  Callers should do a "setFilename" prior to calling this
    *  method or use the ossimImageHandler::open that takes a file name and an
    *  entry index.
    */
   virtual bool open();
   
   virtual void close();
   
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
   virtual ossim_uint32 getNumberOfOutputBands()const;

   /**
     *  Returns the number of lines in the image.
     *  Satisfies pure virtual from ImageHandler class.
     */
   virtual ossim_uint32 getNumberOfLines(
      ossim_uint32 reduced_res_level = 0) const;
   
   /**
    *  Returns the number of samples in the image.  
    *  Satisfies pure virtual from ImageHandler class.
    */
   virtual ossim_uint32 getNumberOfSamples(
      ossim_uint32 reduced_res_level = 0) const;

   /**
    *  Returns the number of reduced resolution data sets (rrds).
    *  Note:  The full res image is counted as a data set so an image with no
    *         reduced resolution data set will have a count of one.
    */
   virtual ossim_uint32 getNumberOfDecimationLevels() const;
   
   /**
    *  Returns the zero based image rectangle for the reduced resolution data
    *  set (rrds) passed in.  Note that rrds 0 is the highest resolution rrds.
    */
   virtual ossimIrect getImageRectangle(
      ossim_uint32 reduced_res_level = 0) const;
   
   /**
    *  Set the output band list.  Use to set the number and order of output
    *  bands.  Will set an error if out of range.
    */
   bool setOutputBandList(const vector<ossim_uint32>& outputBandList);

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
    * @brief Gets a property for matching name.
    * @param name The name of the property to get.
    * @return Returns property matching "name".
    */
   virtual ossimRefPtr<ossimProperty> getProperty(
      const ossimString& name)const;
   
   /**
    * @brief Gets a list of property names available.
    * @param propertyNames The list to push back names to.
    */
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;
	
   
protected:
   virtual ~ossimCcfTileSource();

   void initVerticesFromHeader();
   
   // Disallow these...
   ossimCcfTileSource(const ossimCcfTileSource& source);
   ossimCcfTileSource& operator=(const ossimCcfTileSource& source); 
   
   /**
    *  Returns true on success, false on error.
    */
   bool fillBuffer(const  ossimIrect& tile_rect,
                   const  ossimIrect& clip_rect,
                   const  ossimIrect& image_rect,
                   ossim_uint32 reduced_res_level,
                   ossimImageData* tile);

   /**
    *  Returns true on success, false on error.
    */
   bool fillUshortBuffer(const  ossimIrect& tile_rect,
                         const  ossimIrect& clip_rect,
                         const  ossimIrect& image_rect,
                         ossim_uint32 reduced_res_level,
                         ossimImageData* tile);
   
   /**
    *  Returns true on success, false on error.
    */
   bool fillUcharBuffer(const  ossimIrect& tile_rect,
                        const  ossimIrect& clip_rect,
                        const  ossimIrect& image_rect,
                        ossim_uint32 reduced_res_level,
                        ossimImageData* tile);

   /**
    *  Adjust point to even 256 boundary.  Assumes 0,0 origin.
    */
   void adjustToStartOfChunk(ossimIpt& pt) const;

   /**
    *  Adjust point to even 32 boundary.  Assumes 0,0 origin.
    */
   void adjustToStartOfChip(ossimIpt& pt) const;

   bool isEcgGeom(const ossimFilename& filename)const;

   ossimCcfHead                theCcfHead;
   ossimRefPtr<ossimImageData> theTile;
   ossim_uint8*                theChipBuffer;
   ifstream*                   theFileStr;
   vector<ossim_uint32>        theOutputBandList;
   ossim_int32                 theByteOrder;


TYPE_DATA
};
   
#endif
