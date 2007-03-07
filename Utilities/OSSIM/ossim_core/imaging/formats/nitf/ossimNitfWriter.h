//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  See top level LICENSE.txt file.
//
// Author:  Garrett Potts
//
//*******************************************************************
//  $Id: ossimNitfWriter.h,v 1.17 2006/01/06 16:46:46 dburken Exp $
#ifndef ossimNitfWriter_HEADER
#define ossimNitfWriter_HEADER

#include <fstream>
#include <imaging/formats/ossimImageFileWriter.h>
#include <base/data_types/ossimKeywordlist.h>
#include <projections/map_projections/ossimMapProjectionInfo.h>
#include <base/common/ossimRgbLutDataObject.h>
#include <base/common/ossimRefPtr.h>
#include <support_data/nitf/ossimNitfFileHeaderV2_1.h>
#include <support_data/nitf/ossimNitfImageHeaderV2_1.h>

class ossimProjection;

class OSSIM_DLL ossimNitfWriter : public ossimImageFileWriter
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
   virtual ossimRefPtr<ossimProperty> getProperty(
      const ossimString& name)const;

   /**
    * @param propertyNames Array to populate with property names.
    *
    * @note The following names are handled:
    *  file_header
    *  image_header
    *  enable_rpcb_tag
    *  enable_blocka_tag
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
    *
    * Keywords saved by saveState:
    * 
    * enable_rpcb_tag: true
    *
    * enable_blocka_tag: true
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
    *
    * Keywords picked up by loadState:
    * 
    * enable_rpcb_tag: true
    *
    * enable_blocka_tag: true
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

   /**
    * Populates tags with geometry info from projection.  Will write an
    * rpcb tag if theEnableRpcbTagFlag if set to true.
    */
   void writeGeometry();

   /** Currently disabled... */
   // virtual void addStandardTags();

   /**
    * Adds the BLOCKA tag.
    *
    * @param mapInfo ossimMapProjectionInfo to use to set tag with.
    *
    * @note Currently only used with map projected images.
    */
   void addBlockaTag(ossimMapProjectionInfo& mapInfo);

   /**
    * Adds the RPC00B tag.
    *
    * @param rect Requested rectangle of image to write.
    *
    * @param proj The output projection.
    */
   void addRpcbTag(const ossimIrect& rect,
                   ossimRefPtr<ossimProjection> proj);


   std::ofstream* theOutputStream;
   
   ossimRefPtr<ossimNitfFileHeaderV2_1>  theFileHeader;
   ossimRefPtr<ossimNitfImageHeaderV2_1> theImageHeader;

   /** If true user wants to set RPC00B tag. (DEFAULT = false) */
   bool theEnableRpcbTagFlag;

   /**
    * If true user wants to set BLOCKA tag. (DEFAULT = true)
    * Currently only valid for map projected images.
    */
   bool theEnableBlockaTagFlag;
   
TYPE_DATA   
};

#endif /* #ifndef ossimNitfWriter_HEADER */
