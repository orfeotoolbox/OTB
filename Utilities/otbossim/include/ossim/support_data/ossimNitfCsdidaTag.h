//----------------------------------------------------------------------------
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: CSDIDA tag class declaration.
//
// Dataset Indentification TRE.
//
// See document STDI-0006-NCDRD Table 3.3-14 for more info.
// 
//----------------------------------------------------------------------------
// $Id

#ifndef ossimNitfCsdidaTag_HEADER
#define ossimNitfCsdidaTag_HEADER 1
#include <ossim/support_data/ossimNitfRegisteredTag.h>

class OSSIM_DLL ossimNitfCsdidaTag : public ossimNitfRegisteredTag
{
public:
   /** @brief default constructor */
   ossimNitfCsdidaTag();

   /** @brief destructor */
   virtual ~ossimNitfCsdidaTag();

   /**
    * @brief Method to return tag name.
    * @return "CSDIDA" as a std::string.
    */
   virtual std::string getRegisterTagName()const;

   /** @brief Method to parse data from stream. */
   virtual void parseStream(std::istream& in);

   /** @brief Method to write data to stream. */
   virtual void writeStream(std::ostream& out);

   /** @return Byte size of this tag. */
   virtual ossim_uint32 getSizeInBytes()const;

   /** @brief Method to clear all fields including null terminating. */
   virtual void clearFields();

   /**
    * @brief Print method that outputs a key/value type format
    * adding prefix to keys.
    * @param out Stream to output to.
    * @param prefix Prefix added to key like "image0.";
    */
   virtual std::ostream& print(std::ostream& out,
                               const std::string& prefix) const;
   
protected:

   /**
    * FIELD: DAY
    * 
    * 2 byte field BCS-N
    *
    * 01 TO 31 (UTC)
    */
   char theDay[3];

   /**
    * FIELD: MONTH
    *
    * 3 byte field BCS-A
    *
    * JAN to DEC (UTC)
    */
   char theMonth[4];


   /**
    * FIELD: YEAR
    *
    * 4 byte field BCS-N
    * 
    * 0000 to 9999 (UTC)
    */
   char theYear[5];

   /**
    * FIELD: PLATFORM CODE
    *
    * 2 byte field BCS-A
    * 
    * QB, IK, OV, WV ...
    */
   char thePlatformCode[3];

   /**
    * FIELD: VEHICLE ID
    *
    * 2 byte field BCS-N
    * 
    * 00 TO 99
    */
   char theVehicleId[3];

   /**
    * FIELD: PASS
    *
    * 2 byte field BCS-N
    * 
    * 01 to 99
    */
   char thePass[3];

   /**
    * FIELD: OPERATION
    *
    * 3 byte field BCS-N
    * 
    * 001 to 999
    */
   char theOperation[4];

   /**
    * FIELD: SENSOR_ID
    * 
    * 2 byte field BCS-A
    * 
    * AA = pan only, GA = multispectral and pan sharpened only, NA =
    * panchroomatic & multispectral together.
    */
   char theSensorId[3];

   /**
    * FIELD: PRODUCT_ID
    *
    * 2 byte field BCS-A
    * 
    * See table 2.1-7 
    */
   char theProductId[3];

   /**
    * FIELD: Reserved
    * 
    * 4 bytes BCS-A
    *
    * fill "00  "
    */
   char theReservedField1[5];

   /**
    * FIELD: TIME
    *
    * 14 byte field BCS-N
    * 
    * Image start time (UTC) YYYYMMDDhhmmss Corresponds to ACQUISITION_DATE
    * in STDI-0002
    */
   char theTime[15];

   /**
    * FIELD: PROCESS_TIME
    *
    * 14 byte field BCS-N
    * 
    * Image start time (UTC) YYYYMMDDhhmmss
    */
   char theProcessTime[15];

   /** FIELD: Reserved
    *
    * 2 bytes BCS-N
    * 
    * Fill "00"
    */
   char theReservedField2[3];

   /**
    * FIELD: Reserved
    *
    * 2 bytes BCS-N 
    *
    * Fill "01"
    */
   char theReservedField3[3];

   /**
    * FIELD: Reserved
    *
    * 1 byte BCS-A
    *
    * Fill "N"
    */
   char theReservedField4[1];

   /**
    * FIELD: Reserved
    *
    * 1 byte BCS-A
    *
    * BCS-A "N"
    */
   char theReservedField5[1];

   /**
    * FIELD: SOFTWARE_VERSION_NUMBER
    *
    * 10 byte field BCS-A
    * 
    * Software version used.
    */
   char theSoftwareVersionNumber[11]; 
   
TYPE_DATA   
};

#endif /* matches #ifndef ossimNitfCsdidaTag_HEADER */
