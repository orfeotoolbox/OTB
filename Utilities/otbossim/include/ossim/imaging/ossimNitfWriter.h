//*******************************************************************
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Garrett Potts
//
//*******************************************************************
//  $Id: ossimNitfWriter.h 22420 2013-09-26 16:42:56Z gpotts $
#ifndef ossimNitfWriter_HEADER
#define ossimNitfWriter_HEADER

#include <iosfwd>
#include <ossim/imaging/ossimNitfWriterBase.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/projection/ossimMapProjectionInfo.h>
#include <ossim/base/ossimRgbLutDataObject.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/support_data/ossimNitfFileHeaderV2_1.h>
#include <ossim/support_data/ossimNitfImageHeaderV2_1.h>
#include <ossim/support_data/ossimNitfTextHeaderV2_0.h>
#include <ossim/support_data/ossimNitfTextHeaderV2_1.h>
#include <ossim/support_data/ossimNitfDataExtensionSegmentV2_1.h>
#include <ossim/base/ossimIoStream.h>

class ossimProjection;

class OSSIM_DLL ossimNitfWriter : public ossimNitfWriterBase
{
public:
   ossimNitfWriter(const ossimFilename& filename=ossimFilename(""),
                   ossimImageSource* inputSource = (ossimImageSource*)NULL);
   virtual ~ossimNitfWriter();
   virtual bool isOpen()const;
   virtual bool open();
   virtual void close();
   
   /**
    * void getImageTypeList(std::vector<ossimString>& imageTypeList)const
    *
    * Appends this writer image types to list "imageTypeList".
    *
    * This writer has the following types:
    * nitf_block_band_separate
    * nitf_block_band_sequential
    *
    * @param imageTypeList stl::vector<ossimString> list to append to.
    */   
   virtual void getImageTypeList(std::vector<ossimString>& imageTypeList)const;
   
   virtual void setProperty(ossimRefPtr<ossimProperty> property);
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;

   /**
    * @param propertyNames Array to populate with property names.
    *
    * @note The following names are handled:
    *  file_header
    *  image_header
    *  des_header
    */
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;
   
   void addRegisteredTag(ossimRefPtr<ossimNitfRegisteredTag> registeredTag, bool unique = true);
   void addRegisteredTag(ossimRefPtr<ossimNitfRegisteredTag> registeredTag, bool unique,
      const ossim_uint32& ownerIndex, const ossimString& tagType);

   /**
    * Saves the state of the writer to kwl with prefix then calls
    * base class ossimImageFileWriter::saveState
    *
    * @param kwl Keyword list to save to.
    *
    * @param prefix Usually something like: "object2."

    * @return true on success, false on failure.
    */
   virtual bool saveState(ossimKeywordlist& kwl, const char* prefix=0) const;

   /**
    * Initializes the state of the writer from kwl with prefix then calls
    * base class ossimImageFileWriter::loadState
    *
    * @param kwl Keyword list to initialize from.
    *
    * @param prefix Usually something like: "object2."

    * @return true on success, false on failure.
    */
   virtual bool loadState(const ossimKeywordlist& kwl, const char* prefix=0);

   virtual bool addTextToNitf(std::string& inputText);

   /** @brief Gets the block size. */
   virtual void getTileSize(ossimIpt& size) const;

   /**
    * Sets the nitf output block size.  Must be divisible by 16.
    *
    * Note:  This is not called setBlockSize just because there was already
    * a virtual setTileSize.
    *
    * @param tileSize Block size.
    */
   virtual void setTileSize(const ossimIpt& tileSize);

   /**
    * Get the image header used for export.
    *
    * @warning Many values will be overwritten at export time.  This includes the 
    *          ossimNitfImageBandV2_1, if the number of bands on the image header
    *          does not match that on the writer.
    *
    * @return The image header.
    */
   virtual ossimNitfImageHeaderV2_1 *getImageHeader();

   /**
    * Get the file header used for export.
    *
    * @warning Many values will be overwritten at export time.
    *
    * @return The file header.
    */
   virtual ossimNitfFileHeaderV2_1 *getFileHeader();

   /**
    * Add a DES to the file.
    *
    * @param des The DES to add to the file.
    */
   virtual void addDataExtensionSegment(const ossimNitfDataExtensionSegmentV2_1& des, bool allowTreOverflow);
protected:
   
   /**
    * @return true on success false on error.
    */
   virtual bool writeFile();
   
   /**
    * write out block band separate
    * 
    * @return true on success and false on error
    */
   virtual bool writeBlockBandSeparate();
   
   /**
    * Outputs in band sequential format.  Band 1 is followed by band
    * 2, ... etc.
    */
   virtual bool writeBlockBandSequential();

   /** Currently disabled... */
   // virtual void addStandardTags();

   ossimOFStream64*                      m_outputStream;
   ossimRefPtr<ossimNitfFileHeaderV2_1>  m_fileHeader;
   ossimRefPtr<ossimNitfImageHeaderV2_1> m_imageHeader;
   std::vector<ossimNitfDataExtensionSegmentV2_1> m_dataExtensionSegments;
   ossimRefPtr<ossimNitfTextHeaderV2_1>  m_textHeader;
   std::string                           m_textEntry;
   ossimIpt                              m_blockSize;

TYPE_DATA 
private:
   void takeOverflowTags(bool useFileHeader, bool userDefinedTags);

};

#endif /* #ifndef ossimNitfWriter_HEADER */
