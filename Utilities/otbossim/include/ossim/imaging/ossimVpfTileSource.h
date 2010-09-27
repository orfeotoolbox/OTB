//----------------------------------------------------------------------------
// Copyright (C) 2004 Garrett Potts, all rights reserved.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Garrett Potts
//
// $Id: ossimVpfTileSource.h 17932 2010-08-19 20:34:35Z dburken $
//----------------------------------------------------------------------------
#ifndef ossimVpfTileSource_HEADER
#define ossimVpfTileSource_HEADER

#include <ossim/imaging/ossimVpfAnnotationSource.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimViewInterface.h>


class OSSIMDLLEXPORT ossimVpfTileSource : public ossimImageHandler,
                                          public ossimViewInterface
{
public:
   
   ossimVpfTileSource();

   virtual void close();

   /**
    *  @return Returns true on success, false on error.
    *
    *  @note This method relies on the data member ossimImageData::theImageFile
    *  being set.  Callers should do a "setFilename" prior to calling this
    *  method or use the ossimImageHandler::open that takes a file name and an
    *  entry index.
    */   
   virtual bool open();

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
    * Note:  The full res image is counted as a data set so an image with no
    *        reduced resolution data set will have a count of one.
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
                          const char* prefix);
   
   //! Returns the image geometry object associated with this tile source or NULL if non defined.
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

   /*!
    * Returns the tile width of the image or 0 if the image is not tiled.
    * Note: this is not the same as the ossimImageSource::getTileWidth which
    * returns the output tile width which can be different than the internal
    * image tile width on disk.
    */
   virtual ossim_uint32 getImageTileHeight() const;

   virtual bool isOpen()const;
   
   virtual double getNullPixelValue(ossim_uint32 band=0)const;

   virtual double getMinPixelValue(ossim_uint32 band=0)const;
      
   virtual double getMaxPixelValue(ossim_uint32 band=0)const;

   virtual ossimObject* getView();

   virtual const ossimObject* getView()const;
   
   virtual bool setView(ossimObject*  baseObject);

   void getAllFeatures(std::vector<ossimVpfAnnotationFeatureInfo*>& featureList);
  
   void setAllFeatures(std::vector<ossimVpfAnnotationFeatureInfo*>& featureList);

   void transformObjects();

   void computeBoundingRect();

protected:
   virtual ~ossimVpfTileSource();
   ossimRefPtr<ossimVpfAnnotationSource> m_AnnotationSource;
   
   
TYPE_DATA
};

#endif
