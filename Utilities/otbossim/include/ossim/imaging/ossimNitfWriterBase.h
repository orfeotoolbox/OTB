//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: OSSIM nitf writer base class to hold methods common to
// all nitf writers.
//
//----------------------------------------------------------------------------
// $Id: ossimNitfWriterBase.h 2981 2011-10-10 21:14:02Z david.burken $
#ifndef ossimNitfWriterBase_HEADER
#define ossimNitfWriterBase_HEADER

#include <ossim/imaging/ossimImageFileWriter.h>

class ossimFilename;
class ossimImageSourceSequencer;
class ossimMapProjectionInfo;
class ossimNitfFileHeaderV2_X;
class ossimNitfImageHeaderV2_X;
class ossimProjection;

/**
 * @brief OSSIM nitf writer base class to hold methods common to
 * all nitf writers.
 */
class OSSIM_DLL ossimNitfWriterBase : public ossimImageFileWriter
{
public:

   /** @brief default constructor */
   ossimNitfWriterBase();

   /**
    * @brief Constructor that takes filename and input source.
    * @param filename The output file name.
    * @param inputSource The input source to write to file.
    */
   ossimNitfWriterBase(const ossimFilename& filename,
                       ossimImageSource* inputSource);

   /* @brief virtual destructor */
   virtual ~ossimNitfWriterBase();

   /**
    * @brief Set the properties.
    * @param property The property to set.
    */
   virtual void setProperty(ossimRefPtr<ossimProperty> property);

   /**
    * @brief Gets a property.
    * @param The name of the property to get.
    * @return The property if found.
    */
   virtual ossimRefPtr<ossimProperty> getProperty(
      const ossimString& name)const;

   /**
    * @param propertyNames Array to populate with property names.
    *
    * @note The following names are handled:
    *  enable_rpcb_tag
    *  enable_blocka_tag
    */
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;
 

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

   /**
    * Returns a 3-letter extension from the image type descriptor 
    * (theOutputImageType) that can be used for image file extensions.
    *
    * @param imageType string representing image type.
    *
    * @return the 3-letter string extension.
    */
   virtual ossimString getExtension() const;

protected:

   /**
    * @brief Populates tags with geometry info from projection.  Will write an
    * rpcb tag if theEnableRpcbTagFlag if set to true.
    * @param hdr The header to write to.
    * @param seq The input connection or image source.
    */
   void writeGeometry(ossimNitfImageHeaderV2_X* hdr,
                      ossimImageSourceSequencer* seq);

   /**
    * @brief Adds the BLOCKA tag.
    *
    * @param mapInfo ossimMapProjectionInfo to use to set tag with.
    * @param hdr The header to write to.
    *
    * @note Currently only used with map projected images.
    */
   void addBlockaTag(ossimMapProjectionInfo& mapInfo,
                     ossimNitfImageHeaderV2_X* hdr);

   /**
    * @brief Adds the RPC00B tag.
    *
    * @param rect Requested rectangle of image to write.
    *
    * @param proj The output projection.
    */
   void addRpcbTag(const ossimIrect& rect,
                   ossimProjection* proj,
                   ossimNitfImageHeaderV2_X* hdr);

   /**
    * @brief Sets the complexity level of theFileHeader.
    *
    * @param endPosition This should be the end seek position of the file.
    */
   void setComplexityLevel(std::streamoff,
                           ossimNitfFileHeaderV2_X* hdr);

   /**
    * @brief Sets file header and image header defaults from config file if
    * found in preferences.
    */
   virtual void initializeDefaultsFromConfigFile(
      ossimNitfFileHeaderV2_X* fileHdr,
      ossimNitfImageHeaderV2_X* imgHdr );   

   /** @brief If true user wants to set RPC00B tag. (DEFAULT = false) */
   bool theEnableRpcbTagFlag;
   
   /**
    * @brief If true user wants to set BLOCKA tag. (DEFAULT = true)
    * Currently only valid for map projected images.
    */
   bool theEnableBlockaTagFlag;

   

private:

TYPE_DATA
};

#endif /* #ifndef ossimKakaduNitfVoid Writer_HEADER */
