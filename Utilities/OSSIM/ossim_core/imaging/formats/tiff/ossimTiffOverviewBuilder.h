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
// Contains class declaration for TiffOverviewBuilder.
//
//*******************************************************************
//  $Id: ossimTiffOverviewBuilder.h,v 1.14 2005/08/29 18:47:40 gpotts Exp $

#ifndef ossimTiffOverviewBuilder_HEADER
#define ossimTiffOverviewBuilder_HEADER

#include <base/common/ossimConstants.h>
#include <base/common/ossimSource.h>
#include <base/common/ossimErrorStatusInterface.h>
#include <base/common/ossimProcessInterface.h>
#include <base/common/events/ossimConnectableObjectListener.h>
#include <imaging/resample/ossimFilterResampler.h>
#include <base/data_types/ossimFilename.h>
#include <tiffio.h>
#include <tiff.h>

class ossimImageHandler;
class ossimFilename;
class ossimConnectableObject;

class OSSIM_DLL ossimTiffOverviewBuilder
   :
      public ossimSource,
      public ossimProcessInterface,
      public ossimConnectableObjectListener
{
public:

   enum
   {
      DEFAULT_COMPRESS_QUALITY=75
   };
   
   ossimTiffOverviewBuilder(ossimImageHandler* imageHanderler);
   
   ~ossimTiffOverviewBuilder();

   enum Quadrant
   {
      UL,
      UR,
      LL,
      LR
   };

   /**
    * Supports BOX or NEAREST NEIGHBOR.  When indexed you should probably use nearest neighbor
    */ 
   void setResampleType(ossimFilterResampler::ossimFilterResamplerType resampleType);
   
   /*!
    *  Builds overview file and sets "theOutputFile" to that of
    *  the overview_file.
    *
    *  @param overview_file The overview file name to output.
    *
    *  @param copy_all  If set to true the entire image will be
    *  copied.  This can be used to convert an image to a tiled tif.
    *  
    *  @return true on success, false on error.
    */
   bool buildOverview(const ossimFilename& overview_file,
                      bool copy_all=false);

   /*!
    * Calls buildOverview.  This method uses "theOutputFile" for the file
    * name.
    *
    * If the copy_all flag is set the entire image will be copied.  This can
    * be used to convert an image to a tiled tif.
    *
    * @return true on success, false on error.
    *
    * @note If setOutputFile was not called the output name will be derived
    * from the image name.  If image was "foo.tif" the overview file will
    * be "foo.ovr".
    */
   virtual bool execute();

   /*!
    *  Sets the compression type to use when building overviews.
    *  
    *  @param compression_type Current supported types:
    * - COMPRESSION_JPEG
    * - COMPRESSION_LZW
    * - COMPRESSION_DEFLATE
    * - COMPRESSION_PACKBITS
    * - COMPRESSION_NONE (default)
    */
   void setCompressionType(uint16 compression_type);

   /*!
    *  Sets the compression quality for use when using a compression type
    *  of COMPRESSION_JPEG.
    *
    *  @param quality Range 1 to 100 with 100 being best quality.
    */
   void setJpegCompressionQuality(ossim_int32 quality);

   /*!
    * @return ossimObject* to this object.
    */
   virtual ossimObject* getObject();

   /*!
    * @return const ossimObject* to this object.
    */
   virtual const ossimObject* getObject() const;

   /*!
    * @return The output filename.  This will be derived from the image
    * handlers filename unless the method buildOverview has been called which
    * takes a filename.
    */
   ossimFilename getOutputFile() const;

   /*!
    * Sets the output filename.
    */
   void  setOutputFile(const ossimFilename& file);

   /*!
    * @return true if input is an image handler.
    */
   virtual bool canConnectMyInputTo(ossim_int32 index,
                                    const ossimConnectableObject* obj) const;
   
private:
   
   /*!
    *  Returns the number of reduced resolution data sets to get the smallest
    *  dimension >= 32 && < 64.
    */
  ossim_int32 getNumberOfSets() const;

   /*!
    *  Copy the full resolution image data to the output tif image.
    */
  bool writeR0(TIFF* tif);

   /*!
    *  Write the first reduced resolution data set to the tif file.
    */
   bool writeR1(TIFF* tif);

   /*!
    *  Write reduced resolution data set to the tif file.  "rrds_level" must
    *  be greater than 1 for this method as it uses subsequent rrds_levels
    *  from the tif file.
    */
   bool writeRn(TIFF* tif,
                const ossimFilename& overview_file,
                ossim_int32 rrds_level);

   /*!
    *  Resamples a patch of data.
    */
   void resampleTile(void*     source_tile,
                     void*     output_tile,
                     Quadrant quadrant,
		     double    nullPix);

   template <class T> void  resampleTile(T* source_tile,
                                         T* output_tile,
                                         Quadrant quadrant,
                                         T  nullPixel);
   
   /*!
    *  Set the tiff tags for the appropriate rrds_level.  Level zero is the
    *  full resolution image.
    */
   bool setTags(TIFF* tif, ossim_int32 rrds_level) const;

   // Disallow these...
   ossimTiffOverviewBuilder(const ossimTiffOverviewBuilder& source);
   ossimTiffOverviewBuilder& operator=(const ossimTiffOverviewBuilder& rhs); 

   ossimImageHandler* theImageHandler;
   ossimFilename      theOutputFile;
   ossim_uint8*       theNullDataBuffer;
   ossim_int32        theBytesPerPixel;
   ossim_int32        theBitsPerSample;
   ossim_int32        theTileWidth;
   ossim_int32        theTileHeight;
   ossim_int32        theTileSizeInBytes;
   ossim_int32        theSampleFormat;
   ossim_int32        theCurrentTiffDir;
   ossim_uint16       theTiffCompressType;
   ossim_int32        theJpegCompressQuality;
   ossimFilterResampler::ossimFilterResamplerType theResampleType;
   vector<double>      theNullPixelValues;
TYPE_DATA   
};
   
#endif
