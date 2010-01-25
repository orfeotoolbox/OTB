//*******************************************************************
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Eric Hirschorn
//
// Description:
//
// Contains class declaration for VirtualImageTiffWriter.
//
//*******************************************************************
//  $Id: ossimVirtualImageTiffWriter.h 11683 2007-09-07 17:25:30Z gpotts $
#ifndef ossimVirtualImageTiffWriter_HEADER
#define ossimVirtualImageTiffWriter_HEADER

#include <ossim/base/ossimRefPtr.h>
#include <ossim/projection/ossimMapProjectionInfo.h>
#include <ossim/imaging/ossimVirtualImageWriter.h>

class OSSIMDLLEXPORT ossimVirtualImageTiffWriter : public ossimVirtualImageWriter
{
public:

   ossimVirtualImageTiffWriter( const ossimFilename& filename = ossimFilename(),
                                ossimImageHandler* inputSource = 0,
                                bool overviewFlag = false );

   virtual ~ossimVirtualImageTiffWriter();

   virtual void setOutputTileSize( const ossimIpt& tileSize );

   virtual ossimIpt getOutputTileSize()const;

   /**
    *  Sets the compression quality for use when using a compression type
    *  of COMPRESSION_JPEG.
    *
    *  @param quality Range 1 to 100 with 100 being best quality.
    */
   virtual void setCompressionQuality( ossim_int32 quality );

   /**
    *  Compression type can be JPEG, PACKBITS, or ZIP/DEFLATE
    */
   virtual void setCompressionType( const ossimString& type );

   /**
    * @brief Returns the overview type associated with the given 
    * filter resampler type.
    *
    * Called from ossimVirtualImageBuilder.
    * 
    * Currently handled types are:
    * "ossim_virtual_tiff_nearest" and "ossim_virtual_tiff_box"
    *
    * @param type a resampler filter type.
    *
    * @return the overview type.
    */
   static ossimString getOverviewType( 
      ossimFilterResampler::ossimFilterResamplerType type );

   /**
    * @brief Returns the filter resampler type associated with the given 
    * overview type.
    *
    * Called from ossimVirtualImageBuilder.
    * 
    * Currently handled types are:
    * "ossim_virtual_tiff_nearest" and "ossim_virtual_tiff_box"
    *
    * @param type This should be the string representing the type.  This method
    * will do nothing if type is not handled and return false.
    *
    * @return the filter resampler type.
    */
   static ossimFilterResampler::ossimFilterResamplerType 
      getResamplerType( const ossimString& type );

   /**
    * @brief Identifies whether or not the overview type is handled by
    * this writer.
    *
    * Called from ossimVirtualImageBuilder.
    * 
    * Currently handled types are:
    * "ossim_virtual_tiff_nearest" and "ossim_virtual_tiff_box"
    *
    * @param type This should be the string representing the type.  This method
    * will do nothing if type is not handled and return false.
    *
    * @return true if type is handled, false if not.
    */
   static bool isOverviewTypeHandled( const ossimString& type );

   /**
    * @brief Method to populate class supported types.
    * Called from ossimVirtualImageBuilder.
    * @param typeList List of ossimStrings to add to.
    */
   static void getTypeNameList( std::vector<ossimString>& typeList );

protected:

   /**
    *  @brief Method to initialize output file name from image handler.
    *  @return true on success, false on error.
    */
   virtual bool initializeOutputFilenamFromHandler();

   /**
    *  Set the metadata tags for the appropriate resLevel.
    *  Level zero is the full resolution image.
    *
    *  @param rrds_level The current reduced res level.
    *  @param outputRect The dimensions (zero based) of res set.
    */
   virtual bool setTags( ossim_int32 resLevel,
                         const ossimIrect& outputRect ) const;

   /**
    *  Opens a tiff file for a single output frame for writing.
    *
    *  @param resLevel The zero-based resolution level of the frame
    *  @param row The zero-based row at which the frame is located
    *  @param col The zero-based column at which the frame is located
    *  @return true on success, false on error.
    */
   bool openTiff( int resLevel, int row, int col );

   /**
    *  Close the currently open tiff file.
    *  @return true on success, false on error.
    */
   bool closeTiff();

   /**
    *  Renames the current frame from the temporary name
    *  it carries during writing to the final name. I.e.
    *  the .tmp at the end of the name is removed.
    */
   void renameTiff();

   /**
    *  Closes the current frame file.
    */
   void flushTiff();

   /**
    *  Copy user-selected individual frames of the full resolution 
    *  image data to the output virtual image.
    *  @return true on success, false on error.
    */
   virtual bool writeR0Partial();

   /**
    *  Copy all of the full resolution image data to the output 
    *  virtual image.
    *  @return true on success, false on error.
    */
   virtual bool writeR0Full();

   /**
    *  Write user-selected individual frames of the reduced resolution 
    *  image data to the output virtual image.
    *
    *  @param resLevel The reduced resolution level to write.
    *  @return true on success, false on error.
    */
   virtual bool writeRnPartial( ossim_uint32 resLevel );

   /**
    *  Write all of the reduced resolution image data at the given 
    *  resolution level to the output virtual image.
    *
    *  @param resLevel The reduced resolution level to write.
    *  @return true on success, false on error.
    */
   virtual bool writeRnFull( ossim_uint32 resLevel );

   TIFF*                               theTif;
   ossimRefPtr<ossimMapProjectionInfo> theProjectionInfo;
   ossimFilename                       theCurrentFrameName;
   ossimFilename                       theCurrentFrameNameTmp;

TYPE_DATA
};

#endif /* End of "#ifndef ossimVirtualImageTiffWriter_HEADER" */
