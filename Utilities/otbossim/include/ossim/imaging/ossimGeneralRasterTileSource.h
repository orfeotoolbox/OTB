//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
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
//  $Id: ossimGeneralRasterTileSource.h 11181 2007-06-07 19:57:14Z dburken $

#ifndef ossimGeneralRasterTileSource_HEADER
#define ossimGeneralRasterTileSource_HEADER

  // #include <fstream>
#include <ossim/base/ossimIoStream.h>
  //using namespace std;

#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimGeneralRasterInfo.h>

class  ossimImageData;

class OSSIM_DLL ossimGeneralRasterTileSource : public ossimImageHandler
{
public:

   ossimGeneralRasterTileSource();

   virtual ~ossimGeneralRasterTileSource();

   virtual ossimString getShortName()const;
   virtual ossimString getLongName()const;
   virtual ossimString className()const;
   
   /**
    *  Returns a pointer to a tile given an origin representing the upper
    *  left corner of the tile to grab from the image.
    *  Satisfies pure virtual from TileSource class.
    */
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& tile_rect,
                                               ossim_uint32 resLevel=0);
   
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

protected:
   /**
    *  Methods return true on succes false on error.
    */
   virtual bool fillBuffer(const ossimIpt& origin);
   virtual bool fillBIP(const ossimIpt& origin);
   virtual bool fillBIL(const ossimIpt& origin);
   virtual bool fillBSQ(const ossimIpt& origin);
   virtual bool fillBsqMultiFile(const ossimIpt& origin);

   virtual bool initializeHandler();
   virtual void checkBuffer(const ossimIrect& rect);
   ossimRefPtr<ossimImageData>  theTile;
   ossim_uint8*                 theBuffer;
   ossimInterleaveType          theBufferInterleave;
   std::vector<ossimRefPtr<ossimIFStream> > theFileStrList;
//   vector<istream*>            theFileStrList;   
   ossimGeneralRasterInfo       theImageData;
   ossimIrect                   theBufferRect;
   bool                         theSwapBytesFlag;
   ossim_uint32                 theBufferSizeInPixels;

TYPE_DATA
};

#endif

