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
// Contains class declaration for ossimVirtualImageWriter
//*******************************************************************
//  $Id: ossimVirtualImageWriter.h 11181 2007-06-07 19:57:14Z dburken $

#ifndef ossimVirtualImageWriter_HEADER
#define ossimVirtualImageWriter_HEADER

#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimOutputSource.h>
#include <ossim/base/ossimProcessInterface.h>
#include <ossim/base/ossimConnectableObjectListener.h>
#include <ossim/base/ossimStdOutProgress.h>
#include <ossim/imaging/ossimFilterResampler.h>
#include <ossim/imaging/ossimImageSourceSequencer.h>
#include <ossim/imaging/ossimImageFileWriter.h>
#include <ossim/imaging/ossimImageHandler.h>

#define OSSIM_DEFAULT_FRAME_HEIGHT  ((ossim_int32)128)
#define OSSIM_DEFAULT_FRAME_WIDTH   ((ossim_int32)128)

// Helper struct for organizing RPF frames.
struct InputFrameInfo
{
   ossimFilename name;
   ossim_uint32  entry;
   ossim_uint32  row;
   ossim_uint32  col;

   // output frame indices
   // calculated during execute()
   std::vector<ossim_int32> xRangeMin;
   std::vector<ossim_int32> xRangeMax;
   std::vector<ossim_int32> yRangeMin;
   std::vector<ossim_int32> yRangeMax;

   bool isValid( ossim_uint32 resLevel=0 ) 
   { return xRangeMin.size()>resLevel &&
            xRangeMax.size()>resLevel &&
            yRangeMin.size()>resLevel &&
            yRangeMax.size()>resLevel;
   }

};

/**
 * Base class for writers of virtual images.
 *
 * There is normally one subclass of this class for each format supported
 * for writing.  This class works closely with the ossimVirtualOverviewBuilder
 * class to provide it functionality for specific image formats.  Format 
 * specific ossimVirtualImageWriters are normally instantiated by the 
 * ossimVirtualImageBuilder::setOverviewType() method. 
 * ossimVirtualImageWriters should not be directly instantiated by 
 * application code.
 */
class OSSIMDLLEXPORT ossimVirtualImageWriter : public ossimOutputSource, 
                                               public ossimProcessInterface,
                                               public ossimConnectableObjectListener
{
public:

   ossimVirtualImageWriter( const ossimFilename& filename = ossimFilename(),
                            ossimImageHandler* inputSource=0,
                            bool overviewFlag = false );
   
   virtual ~ossimVirtualImageWriter();

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

   /**
    * Sets the output image tiling size if supported by the writer.  If not
    * supported this simply sets the sequencer(input) tile size.
    */
   virtual void setOutputTileSize( const ossimIpt& tileSize );

   ossimIpt getOutputTileSize() const;

   /**
    * Number of pixels on a side of the output frames of the
    * virtual image.
    */
   virtual void setOutputFrameSize( const ossimIpt& frameSize );

   virtual void initialize();

   /**
    * @brief Sets an arbitrary format image writer to output
    * the frame files.
    *
    * @param pWriter The frame writer instance.
    */
   void setFrameWriter( ossimImageFileWriter* pWriter );

   /**
    * @brief Retrieves arbitrary format image writer.
    *
    * return Returns the frame writer instance.
    */
   ossimImageFileWriter* getFrameWriter( void ) const;

   virtual void setOutputFile( const ossimFilename& file );
   virtual const ossimFilename& getOutputFile()const;

   virtual bool canConnectMyInputTo( ossim_int32 inputIndex,
                                     const ossimConnectableObject* object )const;

   /**
    * @return Returns theAreaOfInterest.
    */
   virtual ossimIrect getAreaOfInterest() const;

   virtual void setAreaOfInterest( const ossimIrect& inputAreaOfInterest );

   /**
    * Supports BOX or NEAREST NEIGHBOR.  
    * When indexed you should probably use nearest neighbor
    */ 
   void setResampleType( ossimFilterResampler::ossimFilterResamplerType t )
   { m_ResampleType = t; }

   ossimFilterResampler::ossimFilterResamplerType getResampleType() const 
   { return m_ResampleType; }

   /**
    * @brief Method to return the flag that identifies whether or not the
    * virtual image is an overview or not.
    * @return The overview flag.  If true the virtual image is an overview
    * that contains missing resolution levels of another image.
    */
   bool getOverviewFlag() const;

   /**
    * @brief Method to set the flag that identifies whether or not the
    * virtual image is an overview or not.
    * @param flag The overview flag. If true the virtual image is an overview
    * that contains missing resolution levels of another image.
    */
   void setOverviewFlag( bool flag );

   /**
    * @brief Method to return copy all flag.
    * @return The copy all flag.  If true all data will be written to the
    * overview including R0.
    */
   bool getCopyAllFlag() const;

   /**
    * @brief Sets theCopyAllFlag.
    * @param flag The flag. If true all data will be written to the
    * overview including R0.
    */
   void setCopyAllFlag( bool flag );

   /**
    * @return ossimObject* to this object.
    */
   virtual ossimObject* getObject();

   /**
    * @return const ossimObject* to this object.
    */
   virtual const ossimObject* getObject() const;

   /** 
    * Unused inherited function isOpen()
    */
   virtual bool isOpen() const { return false; }
   /** 
    * Unused inherited function open()
    */
   virtual bool open() { return false; }
   /** 
    * Unused inherited function close()
    */
   virtual void close() {}

   /**
    * @brief Sets the name of a frame for guiding a limited-scope virtual 
    * image update.
    *
    * @param name The name of an existing frame file of an RPF image.
    */
   void setDirtyFrame( const ossimString& name );

   /**
    *  Manages the writing of the virtual image file.
    *  @return true on success, false on error.
    */
   virtual bool execute();

protected:

   /**
    *  Copy the full resolution image data to the output image.
    *  @return true on success, false on error.
    */
   virtual bool writeR0();

   /**
    *  Write reduced resolution data set to the file.
    *  @return true on success, false on error.
    */
   virtual bool writeRn( ossim_uint32 resLevel );

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

   /**
    *  @brief Gets the zero based final reduced resolution data set.
    *
    *  @param startResLevel The starting reduced resolution level.
    *  @param bPartialBuild If true, do calculation assuming a partial build.
    *  @return the final reduced resolution data set 0 being full res.
    */
   virtual ossim_uint32 getFinalResLevel( ossim_uint32 startResLevel,
                                          bool bPartialBuild=false ) const;

   /**
    *  @brief Gets the zero based starting reduced resolution data set.
    *
    *  @return the starting reduced resolution data set 0 being full res.
    */
   virtual ossim_uint32 getStartingResLevel() const;

   /**
    *  @brief Method to initialize output file name from image handler.
    *  @return true on success, false on error.
    */
   virtual bool initializeOutputFilenamFromHandler();

   /**
    *  Set the header info into a keywordlist after the output
    *  frames have been output from the start to end resolution
    *  levels.
    *
    *  @param kwl A keywordlist where the header info is stored.
    *  @param begin The starting reduced resolution level.
    *  @param end The final reduced resolution level.
    *  @return true on success, false on error.
    */
   virtual bool saveHeaderInfo( ossimKeywordlist& kwl, 
                                ossim_uint32 begin,
                                ossim_uint32 end );

   /**
    *  Set the virtual image -specific information for
    *  a single image entry to the keywordlist.
    */
   void saveNativeKeywordEntry( ossimKeywordlist& kwl,
                                const ossimString& prefix, 
                                ossim_uint32 resLevelBegin,
                                ossim_uint32 resLevelEnd ) const;

   /**
    *  Set the geometry information for a single image entry
    *  to the keywordlist.
    */
   void saveGeometryKeywordEntry( ossimKeywordlist& kwl,
                                  const ossimString& prefix );
 
   /**
    *  Set general image information for a single image entry
    *  to the keywordlist.
    */
   void saveGeneralKeywordEntry( ossimKeywordlist& kwl, 
                                 const ossimString& prefix ) const;

   /**
    * @return true if the current entry of 'theImageHandler' represents
    * an external overview.
    */
   bool isExternalOverview() const;

   /**
    *  Calculates and returns the number of frames in x,y that the
    *  virtual image will contain at the requested resolution level.
    *
    *  @param resLevel The reduced resolution level.
    *  @return the number of frames in the x,y directions.
    */
   ossimIpt getNumberOfOutputFrames( ossim_uint32 resLevel=0 ) const;

   /**
    *  Calculates and returns the total number of frames that will be
    *  built at the requested resolution level.
    *
    *  @param resLevel The reduced resolution level.
    *  @param bPartialBuild If true, do calculation assuming a partial build.
    *  @return the number of frames in the x,y directions.
    */
   ossim_int32 getNumberOfBuiltFrames( ossim_uint32 resLevel=0,
                                       bool bPartialBuild=false ) const;

   /**
    *  Calculates and returns the number of tiles in x,y that the
    *  output image will contain at the requested resolution level.
    *
    *  @param resLevel The reduced resolution level.
    *  @return the number of tiles in the x,y directions.
    */
   ossimIpt getNumberOfOutputTiles( ossim_uint32 resLevel=0 ) const;

   /**
    *  Calculates and returns the number of tiles in x,y that a
    *  single frame of the virtual image will contain.
    *
    *  @return the number of tiles in x,y directions.
    */
   ossimIpt getNumberOfTilesPerOutputFrame() const;

   /**
    *  Finds information about the RPF frame of the given name.
    *
    *  @param name The name of an RPF frame file.
    *  @return a pointer to a InputFrameInfo struct.
    */
   InputFrameInfo* getInputFrameInfo( ossimFilename name ) const;

   /**
    * Returns true if the given output frame (frameX,frameY,resLevel) 
    * has already been generated by previous input RPF frames (i.e.
    * at less than idx in theInputFrameInfoQueue).
    */
   bool isFrameAlreadyDone( ossim_uint32 idx, ossim_uint32 resLevel,
                            ossim_int32 frameX, ossim_int32 frameY ) const;

   /**
    *  Sets the output frame name to the frame writer.
    *
    *  @param resLevel The zero-based resolution level of the frame
    *  @param row The zero-based row at which the frame is located
    *  @param col The zero-based column at which the frame is located
    *  @return true on success, false on error.
    */
   virtual bool setFrameName( int resLevel, int row, int col );

   /**
    *  Close the currently open image frame.
    */
   virtual void closeFrame();

   /**
    *  Renames the current frame from the temporary name
    *  it carries during writing to the final name. I.e.
    *  the .tmp at the end of the name is removed.
    */
   void renameFrame();

   /**
    *  Set writer parameters of the current frame for the given resLevel.
    *
    *  @param rrds_level The current reduced res level.
    *  @param outputRect The dimensions (zero based) of res set.
    */
   virtual bool setFrameParameters( ossim_int32 resLevel,
                                    const ossimIrect& outputRect );

   ossimFilename                                  m_OutputFile;
   ossimFilename                                  m_OutputFileTmp;
   ossimFilename                                  m_OutputSubdirectory;
   ossimString                                    m_VirtualWriterType;
   ossimString                                    m_MajorVersion;
   ossimString                                    m_MinorVersion;
   ossim_uint16                                   m_CompressType;
   ossim_int32                                    m_CompressQuality;
   ossimIpt                                       m_OutputTileSize;
   ossimIpt                                       m_OutputFrameSize;
   ossimIpt                                       m_InputFrameSize;
   ossim_int32                                    m_BytesPerPixel;
   ossim_int32                                    m_BitsPerSample;
   ossim_int32                                    m_TileSizeInBytes;
   std::vector<ossim_uint8>                       m_NullDataBuffer;
   bool                                           m_CopyAllFlag;
   bool                                           m_LimitedScopeUpdateFlag;
   bool                                           m_OverviewFlag;
   ossim_uint32                                   m_CurrentEntry;
   ossimStdOutProgress*                           m_ProgressListener;
   std::vector<ossimFilename>                     m_DirtyFrameList;
   std::vector<InputFrameInfo*>                   m_InputFrameInfoList;
   std::vector<InputFrameInfo*>                   m_InputFrameInfoQueue;
   ossimFilterResampler::ossimFilterResamplerType m_ResampleType;
   ossimRefPtr<ossimImageHandler>                 m_ImageHandler;
   ossimRefPtr<ossimImageSourceSequencer>         m_InputConnection;
   ossimRefPtr<ossimImageGeometry>                m_InputGeometry;
   ossimRefPtr<ossimProjection>                   m_InputProjection;
   ossimRefPtr<ossimImageFileWriter>              m_FrameWriter;
   ossimIrect                                     m_AreaOfInterest;
   ossimFilename                                  m_CurrentFrameName;
   ossimFilename                                  m_CurrentFrameNameTmp;
   ossimString                                    m_FrameExt;
   ossim_uint32                                   m_NumberVerticalFrames;
   ossim_uint32                                   m_NumberHorizontalFrames;

TYPE_DATA
};
#endif
