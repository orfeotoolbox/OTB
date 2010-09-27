
//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Mingjie Su
//
// Description:
//
// Contains class definition for the class "ossimMaskedImageHandler".
// ossimMaskedImageHandler is derived from ImageHandler which is derived from
// TileSource.
//
//*******************************************************************
//  $Id: ossimMaskedImageHandler.h 1242 2010-08-06 20:37:21Z david.burken $

#ifndef ossimMaskedImageHandler_HEADER
#define ossimMaskedImageHandler_HEADER

#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimMaskFilter.h>
#include <ossim/base/ossimIrect.h>

class ossimMaskTileSource;

class ossimMaskedImageHandler : public ossimImageHandler
{
public:
   ossimMaskedImageHandler();
   virtual ~ossimMaskedImageHandler();

   /**
    *  @return Returns true on success, false on error.
    *
    *  @note This method relies on the data member ossimImageData::theImageFile
    *  being set.  Callers should do a "setFilename" prior to calling this
    *  method or use the ossimImageHandler::open that takes a file name and an
    *  entry index.
    */      
   virtual bool open();

   virtual bool open(const ossimFilename& imageFile);

   virtual void close();

   /*!
    *  Returns a pointer to a tile given an origin representing the upper
    *  left corner of the tile to grab from the image.
    *  Satisfies pure virtual from TileSource class.
    */
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& tileRect,
                                               ossim_uint32 resLevel=0);

   /*!
    *  Returns the number of bands in the image.
    *  Satisfies pure virtual from ImageHandler class.
    */
   virtual ossim_uint32 getNumberOfInputBands() const;

    /*!
    * Returns the number of bands in a tile returned from this TileSource.
    * Note: we are supporting sources that can have multiple data objects.
    * If you want to know the scalar type of an object you can pass in the 
    */
   virtual ossim_uint32 getNumberOfOutputBands() const;
  
   /*!
    *  Returns the number of bands in the image.
    *  Satisfies pure virtual from ImageHandler class.
    */
   virtual ossim_uint32 getNumberOfLines(ossim_uint32 reduced_res_level = 0) const;

   /*!
    *  Returns the number of bands available from an image.
    *  Satisfies pure virtual from ImageHandler class.
    */
   virtual ossim_uint32 getNumberOfSamples(ossim_uint32 reduced_res_level = 0) const;

   /*!
    * Returns the number of reduced resolution data sets (rrds).
    * 
    * Note:  Shape files should never have reduced res sets so this method is
    * returns "8" to avoid the question of "Do you want to build res sets".
    */
   virtual ossim_uint32 getNumberOfDecimationLevels() const;

   /*!
    * Returns the zero based image rectangle for the reduced resolution data
    * set (rrds) passed in.  Note that rrds 0 is the highest resolution rrds.
    */
   virtual ossimIrect getImageRectangle(ossim_uint32 reduced_res_level = 0) const;

   /*!
    * Method to save the state of an object to a keyword list.
    * Return true if ok or false on error.
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;

   /*!
    * Method to the load (recreate) the state of an object from a keyword
    * list.  Return true if ok or false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);
   
   //! Returns the image geometry object associated with this tile source or NULL if non defined.
   //! The geometry contains full-to-local image transform as well as projection (image-to-world)
   virtual ossimRefPtr<ossimImageGeometry> getImageGeometry();

   /*!
    * Returns the output pixel type of the tile source.
    */
   virtual ossimScalarType getOutputScalarType() const;

   /*!
    * Returns the width of the output tile.
    */
   virtual ossim_uint32 getTileWidth() const;
   
   /*!
    * Returns the height of the output tile.
    */
   virtual ossim_uint32 getTileHeight() const;

   /*!
    * Returns the tile width of the image or 0 if the image is not tiled.
    * Note: this is not the same as the ossimImageSource::getTileWidth which
    * returns the output tile width which can be different than the internal
    * image tile width on disk.
    */
   virtual ossim_uint32 getImageTileWidth() const;

   virtual bool isOpen()const;

   /*!
    * Returns the tile width of the image or 0 if the image is not tiled.
    * Note: this is not the same as the ossimImageSource::getTileWidth which
    * returns the output tile width which can be different than the internal
    * image tile width on disk.
    */
   virtual ossim_uint32 getImageTileHeight() const;   

   virtual double getNullPixelValue(ossim_uint32 band=0)const;

   virtual double getMinPixelValue(ossim_uint32 band=0)const;
      
   virtual double getMaxPixelValue(ossim_uint32 band=0)const;

   virtual void setProperty(ossimRefPtr<ossimProperty> property);
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;

   /** @return The current entry number. */
   virtual ossim_uint32 getCurrentEntry()const;

   /**
    * @param entryIdx Entry number to select.
    * @return true if it was able to set the current entry and false otherwise.
    */
   virtual bool setCurrentEntry(ossim_uint32 entryIdx);

   void setInputImageSource(ossimRefPtr<ossimImageHandler> inputImageSource);

   bool isSupportedExtensions();

private:

   ossimRefPtr<ossimImageHandler>                   m_inputImageSource;
   ossimRefPtr<ossimMaskTileSource>                 m_maskImageSource;
   ossimRefPtr<ossimMaskFilter>                     m_maskFilter;
 
TYPE_DATA
};

#endif
