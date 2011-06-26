//*******************************************************************
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
// 
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfImageHeader.h 18413 2010-11-11 19:56:22Z gpotts $
#ifndef ossimNitfImageHeader_HEADER
#define ossimNitfImageHeader_HEADER

#include <vector>

#include <ossim/base/ossimNBandLutDataObject.h>
#include <ossim/base/ossimObject.h>
#include <ossim/base/ossimPropertyInterface.h>
#include <ossim/support_data/ossimNitfTagInformation.h>
#include <ossim/support_data/ossimNitfCompressionHeader.h>
#include <ossim/support_data/ossimNitfImageBand.h>

class ossimString;
class ossimDpt;
class ossimIpt;
class ossimIrect;

class OSSIMDLLEXPORT ossimNitfImageHeader : public ossimObject,
                                            public ossimPropertyInterface
{
public:
   ossimNitfImageHeader();
   virtual ~ossimNitfImageHeader();

   virtual bool getTagInformation(ossimNitfTagInformation& tagInfo,
                                  ossim_uint32 idx) const;
   
   virtual bool getTagInformation(ossimNitfTagInformation& tagInfo,
                                  const ossimString& tagName) const;

   ossimRefPtr<ossimNitfRegisteredTag> getTagData(const ossimString& tagName);
   
   const ossimRefPtr<ossimNitfRegisteredTag> getTagData(
      const ossimString& tagName) const;
   
   virtual ossim_uint32 getNumberOfTags()const;

   virtual void addTag(const ossimNitfTagInformation& tag);
   virtual void removeTag(const ossimString& tagName);
   
   virtual void parseStream(std::istream &in)= 0;
   virtual void writeStream(std::ostream &out)= 0;

   virtual bool isCompressed()const=0;
   virtual bool isEncrypted()const=0;

   virtual ossimString getCompressionCode()const=0;
   virtual ossimString getCompressionRateCode()const=0;
   virtual ossim_uint32 getDisplayLevel()const=0;
   virtual ossim_uint32 getAttachmentLevel()const=0;
   
   virtual ossim_int32 getNumberOfRows()const=0;
   virtual ossim_int32 getNumberOfBands()const=0;
   virtual ossim_int32 getNumberOfCols()const=0;
   virtual ossim_int32 getNumberOfBlocksPerRow()const=0;
   virtual ossim_int32 getNumberOfBlocksPerCol()const=0;
   virtual ossim_int32 getNumberOfPixelsPerBlockHoriz()const=0;
   virtual ossim_int32 getNumberOfPixelsPerBlockVert()const=0;
   virtual ossimString getImageId()const=0;
   virtual ossimString getTitle()const=0;
   virtual ossim_int32 getActualBitsPerPixelPerBand()const=0;
   virtual ossim_int32 getBitsPerPixelPerBand()const=0;
   virtual ossimIrect  getImageRect()const=0;
   /**
    * Blocks might not be the same size as the significant rows and cols used
    * by the getImageRect.
    */
   virtual ossimIrect   getBlockImageRect()const=0;
   virtual ossimString  getIMode()const=0;
   virtual ossim_uint64 getDataLocation()const=0;

   /** @return The IDATIM field unparsed. */
   virtual ossimString  getImageDateAndTime() const=0;
   
   virtual ossimString  getAcquisitionDateMonthDayYear(ossim_uint8 separationChar='-')const=0;
   virtual ossimString  getSecurityClassification()const=0;
   virtual ossimString  getCategory()const=0;
   virtual ossimString  getImageSource()const=0;
   virtual ossimString  getRepresentation()const=0;
   virtual ossimString  getCoordinateSystem()const=0;
   virtual ossimString  getGeographicLocation()const=0;
   virtual ossimString  getPixelValueType()const=0;

   /** @return IMAG field */
   virtual ossimString  getImageMagnification()const=0;

   /**
    * @brief Gets the ILOC field as an ossimIpt.
    * @param loc Initialized with the field.
    * @note: Do not confuse with "data location".
    */
   virtual void getImageLocation(ossimIpt& loc)const=0;

   /**
    * @brief Convenience method to get the decimation factor as a double from
    * the string returned be the getImageMagnification() method.
    *
    * @param result Initialized with the decimation factor as a double.
    * Will return an ossim::nan() if something bad happens like the string is
    * empty so callers should check result with ossim::isnan(result).
    */
   virtual void getDecimationFactor(ossim_float64& result) const;
   
   virtual bool hasBlockMaskRecords()const=0;
   virtual bool hasPadPixelMaskRecords()const=0;
   virtual bool hasTransparentCode()const=0;
   virtual ossim_uint32 getTransparentCode()const=0;
   virtual ossim_uint32 getBlockMaskRecordOffset(ossim_uint32 blockNumber,
                                                 ossim_uint32 bandNumber)const=0;
   
   virtual ossim_uint32 getPadPixelMaskRecordOffset(ossim_uint32 blockNumber,
                                                    ossim_uint32 bandNumber)const=0;
   virtual const ossimRefPtr<ossimNitfCompressionHeader> getCompressionHeader()const=0;
   virtual const ossimRefPtr<ossimNitfImageBand> getBandInformation(ossim_uint32 idx)const=0;
   bool hasLut()const;
   ossimRefPtr<ossimNBandLutDataObject> createLut(ossim_uint32 bandIdx)const;
   virtual bool isSameAs(const ossimNitfImageHeader* hdr) const;
   
   virtual void setProperty(ossimRefPtr<ossimProperty> property);
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;

   virtual ossim_uint32 getTotalTagLength()const;

   /**
    * @brief print method that outputs a key/value type format adding prefix
    * to keys.
    */
   virtual std::ostream& printTags(
      std::ostream& out, const std::string& prefix=std::string()) const;
   
   /**
    * @brief Populates keyword list with metadata.
    * expample:  country_code: US
    * 
    * @param kwl Keywordlist to populate with metadata.
    *
    * @param prefix Optional prefix to put in front of key, like: "image1.".
    */
   virtual void getMetadata(ossimKeywordlist& kwl,
                            const char* prefix=0) const;

   /**
    * @brief pure virtual print method that outputs a key/value type format
    * adding prefix to keys.
    */
   virtual std::ostream& print(std::ostream& out,
                               const std::string& prefix) const = 0;

   virtual bool saveState(ossimKeywordlist& kwl, const ossimString& prefix="")const;
protected:

   /**
    * Method to check tie point to see if it will be truncated in field
    * IGEOLO which has only arc second resolution.
    * 
    * Spits out warning if tie does not evenly divide by an arc second.
    *
    * @param tie Tie point to check.
    */
   void checkForGeographicTiePointTruncation(const ossimDpt& tie) const;
   
   std::vector<ossimNitfTagInformation> theTagList;

TYPE_DATA
};

#endif
