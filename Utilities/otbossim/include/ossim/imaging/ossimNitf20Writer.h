//*******************************************************************
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Garrett Potts
//
//*******************************************************************
//  $Id: ossimNitfWriter.h 9256 2006-07-14 15:28:19Z dburken $
#ifndef ossimNitf20Writer_HEADER
#define ossimNitf20Writer_HEADER

#include <iosfwd>
#include <ossim/imaging/ossimImageFileWriter.h>
#include <ossim/imaging/ossimNitfWriterBase.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/projection/ossimMapProjectionInfo.h>
#include <ossim/base/ossimRgbLutDataObject.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/support_data/ossimNitfFileHeaderV2_0.h>
#include <ossim/support_data/ossimNitfImageHeaderV2_0.h>

class ossimProjection;

class OSSIM_DLL ossimNitf20Writer : public ossimNitfWriterBase
{
public:
   ossimNitf20Writer(const ossimFilename& filename=ossimFilename(""),
                   ossimImageSource* inputSource = (ossimImageSource*)NULL);
   virtual ~ossimNitf20Writer();
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
   virtual ossimRefPtr<ossimProperty> getProperty(
      const ossimString& name)const;

   /**
    * @param propertyNames Array to populate with property names.
    *
    * @note The following names are handled:
    *  file_header
    *  image_header
    */
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;
   
   void addRegisteredTag(ossimRefPtr<ossimNitfRegisteredTag> registeredTag);

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

   void addTags();

   std::ofstream* theOutputStream;
   
   ossimRefPtr<ossimNitfFileHeaderV2_0>  theFileHeader;
   ossimRefPtr<ossimNitfImageHeaderV2_0> theImageHeader;

   /** If true user wants to set RPC00B tag. (DEFAULT = false) */
   bool theEnableRpcbTagFlag;

   /**
    * If true user wants to set BLOCKA tag. (DEFAULT = true)
    * Currently only valid for map projected images.
    */
   bool theEnableBlockaTagFlag;

   /**
    *
    * If true this will enable searching the input connnection for another NITF handler and 
    * bring the fields to this writers output fields and will maintin as many field values as possible
    *
    */
   bool theCopyFieldsFlag;
   
TYPE_DATA   
};

#endif /* #ifndef ossimNitfWriter_HEADER */
