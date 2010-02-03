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
// Contains class declaration for VirtualOverviewBuilder.
//
//*******************************************************************
//  $Id: ossimVirtualOverviewBuilder.h 14780 2009-06-25 19:32:32Z dburken $

#ifndef ossimVirtualOverviewBuilder_HEADER
#define ossimVirtualOverviewBuilder_HEADER

#include <vector>
#include <ossim/imaging/ossimFilterResampler.h>
#include <ossim/imaging/ossimOverviewBuilderBase.h>

class ossimImageHandler;
class ossimFilename;
class ossimConnectableObject;
class ossimStdOutProgress;

class OSSIM_DLL ossimVirtualOverviewBuilder : public ossimOverviewBuilderBase
{
public:

   enum WriterType
   {
      WRITER_TYPE_TIFF,
      WRITER_TYPE_UNKNOWN
   };

   /** default constructor */
   ossimVirtualOverviewBuilder();

   /** virtual destructor */
   virtual ~ossimVirtualOverviewBuilder();

   /**
    * Supports BOX or NEAREST NEIGHBOR.  
    * When indexed you should probably use nearest neighbor
    */ 
   void setResampleType( ossimFilterResampler::ossimFilterResamplerType resampleType );

   /**
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
   bool buildOverview( const ossimFilename& overview_file,
                       bool copy_all=false );

   /**
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
    * @return The output filename.  This will be derived from the image
    * handlers filename unless the method buildOverview has been called which
    * takes a filename.
    */
   ossimFilename getOutputFile() const;

   /**
    * @return true if input is an image handler.
    */
   virtual bool canConnectMyInputTo( ossim_int32 index,
                                     const ossimConnectableObject* obj ) const;

   /**
    * @brief Sets the input to the builder. Satisfies pure virtual from
    * ossimOverviewBuilderBase.
    *
    * @param imageSource The input to the builder.
    * @return True on successful initialization, false on error.
    */
   virtual bool setInputSource( ossimImageHandler* imageSource );
   
   /**
    * @brief Sets the output filename.
    * Satisfies pure virtual from ossimOverviewBuilderBase.
    * @param file The output file name.
    */
   virtual void setOutputFile( const ossimFilename& file );

   void setOutputTileSize( const ossimIpt& tileSize );

   /*
    * Number of pixels on a side of the output frames of the
    * virtual overview.
    */
   void setOutputFrameSize( const ossimIpt& frameSize );

   /**
    * @brief Sets the overview output type.
    *
    * Satisfies pure virtual from ossimOverviewBuilderBase.
    * 
    * @param type This should be the string representing the type.  This method
    * will do nothing if type is not handled and return false.
    *
    * @return true if type is handled, false if not.
    */
   virtual bool setOverviewType( const ossimString& type );

   /**
    * @brief Gets the overview type.
    * Satisfies pure virtual from ossimOverviewBuilderBase.
    * @return The overview output type as a string.
    */
   virtual ossimString getOverviewType() const;

   /**
    * @brief Method to populate class supported types.
    * Satisfies pure virtual from ossimOverviewBuilderBase.
    * @param typeList List of ossimStrings to add to.
    */
   virtual void getTypeNameList( std::vector<ossimString>& typeList )const;

   /**
    * @brief Method to set properties.
    * @param prop Property to set.
    *
    * @note Currently supported property:
    * name=levels, value should be list of levels separated by a comma with
    * no spaces. Example: "2,4,8,16,32,64"
    */
   virtual void setProperty( ossimRefPtr<ossimProperty> prop );

   /**
    * @brief Method to populate the list of property names.
    * @param propNames List to populate.  This does not clear the list
    * just adds to it.
    */
   virtual void getPropertyNames( std::vector<ossimString>& propNames )const;

   /**
    * @brief Sets the name of a frame for guiding a limited-scope virtual 
    * overview update.
    *
    * @param name The name of an existing frame file of an RPF image.
    */
   void setDirtyFrame( const ossimString& name );

   static const char* OUTPUT_FRAME_SIZE_KW;

private:

   // Disallow these...
   ossimVirtualOverviewBuilder( const ossimVirtualOverviewBuilder& source );
   ossimVirtualOverviewBuilder& operator=( const ossimVirtualOverviewBuilder& rhs ); 

   ossimRefPtr<ossimImageHandler>                 theImageHandler;
   bool                                           theOwnsImageHandlerFlag;
   ossimFilename                                  theOutputFile;
   ossimIpt                                       theOutputTileSize;
   ossimIpt                                       theOutputFrameSize;
   ossimFilterResampler::ossimFilterResamplerType theResamplerType;
   bool                                           theCopyAllFlag;
   ossimString                                    theCompressType;
   ossim_int32                                    theCompressQuality;
   ossimStdOutProgress*                           theProgressListener;
   WriterType                                     theWriterType;
   std::vector<ossimString>                       theDirtyFrameList;

TYPE_DATA   
};
   
#endif
