//*******************************************************************
// Copyright (C) 2001 ImageLinks Inc. 
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
// $Id: ossimDtedTileSource.h 9184 2006-06-22 19:27:59Z sbortman $

#ifndef ossimDtedTileSource_HEADER
#define ossimDtedTileSource_HEADER

#include <fstream>
using namespace std;

#include <ossim/imaging/ossimImageHandler.h>

class ossimImageData;

class OSSIM_DLL ossimDtedTileSource : public ossimImageHandler
{
public:
      
   ossimDtedTileSource();
   virtual ~ossimDtedTileSource();

   enum
   {
      DATA_RECORD_OFFSET_TO_POST = 8,  // bytes
      DATA_RECORD_CHECKSUM_SIZE  = 4,  // bytes
      POST_SIZE                  = 2,  // bytes
      NULL_PIXEL                 = -32767
   };
   
   virtual ossimString getShortName()const;
   virtual ossimString getLongName()const;
   virtual ossimString className()const;

   /**
    *  Returns true if "theImageFile" can be opened and is a valid adrg file.
    */
   virtual bool open();

   virtual ossimRefPtr<ossimImageData> getTile(const  ossimIrect& tile_rect,
                                               ossim_uint32 resLevel=0);
   
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
   
   /**
    *  Populates the keyword list with image geometry information.  This
    *  method is used to relay projection/model information to users.
    *  Returns true if geometry info is present, false if not.
    */
   virtual bool getImageGeometry(ossimKeywordlist& kwl,
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
    * Initializes spacing to the post spacing of the cell.
    *
    * @param postSpacing ossimDpt to initialize with spacing where
    * postspacing.x = longitude interval and postspacing.y = latitude interval.
    */
   void getPostSpacing(ossimDpt& postSpacing) const;
   
private:
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
                   const ossimIrect& clip_rect);

   void gatherStatistics();
   ossim_sint16 convertSignedMagnitude(ossim_uint16& s) const;
   
   ossimRefPtr<ossimImageData> theTile;
   mutable ifstream           theFileStr;
   ossim_uint32               theTileWidth;
   ossim_uint32               theTileHeight;   
   ossim_uint32               theNumberOfLines;
   ossim_uint32               theNumberOfSamps;
   ossim_uint32               theOffsetToFirstDataRecord;
   ossim_uint32               theDataRecordSize;
   ossim_sint16               theMinHeight;
   ossim_sint16               theMaxHeight;
   ossimDpt                   thePostSpacing;
   
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
