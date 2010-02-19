//*******************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Ken Melero
// 
// Description:  This class gives access to the Data Set Identification
//               (DSI) of a DTED Level 1 file.
//
//********************************************************************
// $Id: ossimDtedDsi.h 16104 2009-12-17 18:09:59Z gpotts $

#ifndef ossimDtedDsi_H
#define ossimDtedDsi_H
#include <iosfwd>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimErrorStatusInterface.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimRefPtr.h>

class ossimProperty;

class OSSIM_DLL ossimDtedDsi : public ossimErrorStatusInterface
{
public:
   ossimDtedDsi(const ossimFilename& dted_file="", ossim_int32 offset=0);
   ossimDtedDsi(std::istream& in);

   enum
   {
      DSI_LENGTH = 648,
      DSI_SECURITY_CODE = 4,
      DSI_SECURITY_CONTROL = 5,
      DSI_SECURITY_HANDLING = 7,
      DSI_RESERVED_1 = 34,
      DSI_DMA_SERIES = 60,
      DSI_RESERVED_2 = 65,
      DSI_RESERVED_3 = 80,
      DSI_DATA_EDITION = 88,
      DSI_MATCH_MERGE_VERSION = 90,
      DSI_MAINTENANCE_DATE = 91,
      DSI_MATCH_MERGE_DATE = 95,
      DSI_MAINTENANCE_CODE = 99,
      DSI_PRODUCER_CODE = 103,
      DSI_RESERVED_4 = 111,
      DSI_PRODUCT_SPEC_STOCK_NUMBER = 127, 
      DSI_PRODUCT_SPEC_NUMBER = 136,
      DSI_PRODUCT_SPEC_DATE = 138,
      DSI_VERTICAL_DATUM = 142,
      DSI_HORIZ_DATUM = 145,
      DSI_DIGITIZING_SYSTEM = 150,
      DSI_COMPILATION_DATE = 160,
      DSI_RESERVED_5 = 164,
      DSI_LAT_ORIGIN = 186,
      DSI_LON_ORIGIN = 195,
      DSI_LAT_SW = 205,
      DSI_LON_SW = 212,
      DSI_LAT_NW = 220,
      DSI_LON_NW = 227,
      DSI_LAT_NE = 235,
      DSI_LON_NE = 242,
      DSI_LAT_SE = 250,
      DSI_LON_SE = 257,
      DSI_ORIENTATION = 265,
      DSI_LAT_INTERVAL = 274,
      DSI_LON_INTERVAL = 278,
      DSI_NUM_LAT_LINES = 282,
      DSI_NUM_LON_LINES = 286,
      DSI_PARTIAL_CELL_INDICATOR = 290,
      DSI_RESERVED = 292,
      FIELD1_SIZE           = 3,
      FIELD2_SIZE           = 1,
      FIELD3_SIZE           = 2,
      FIELD4_SIZE           = 27,
      FIELD5_SIZE           = 26,
      FIELD6_SIZE           = 5,
      FIELD7_SIZE           = 15,
      FIELD8_SIZE           = 8,
      FIELD9_SIZE           = 2,
      FIELD10_SIZE          = 1,
      FIELD11_SIZE          = 4,
      FIELD12_SIZE          = 4,
      FIELD13_SIZE          = 4,
      FIELD14_SIZE          = 8,
      FIELD15_SIZE          = 16,
      FIELD16_SIZE          = 9,
      FIELD17_SIZE          = 2,
      FIELD18_SIZE          = 4,
      FIELD19_SIZE          = 3,
      FIELD20_SIZE          = 5,
      FIELD21_SIZE          = 10,
      FIELD22_SIZE          = 4,
      FIELD23_SIZE          = 22,
      FIELD24_SIZE          = 9,
      FIELD25_SIZE          = 10,
      FIELD26_SIZE          = 7,
      FIELD27_SIZE          = 8,
      FIELD28_SIZE          = 7,
      FIELD29_SIZE          = 8,
      FIELD30_SIZE          = 7,
      FIELD31_SIZE          = 8,
      FIELD32_SIZE          = 7,
      FIELD33_SIZE          = 8,
      FIELD34_SIZE          = 9,
      FIELD35_SIZE          = 4,
      FIELD36_SIZE          = 4,
      FIELD37_SIZE          = 4,
      FIELD38_SIZE          = 4,
      FIELD39_SIZE          = 2,
      FIELD40_SIZE          = 101,
      FIELD41_SIZE          = 100,
      FIELD42_SIZE          = 156
   };

   // The Recognition Sentinel signifies if the DSI record exists.
   ossimString recognitionSentinel() const;

   ossimString securityCode()           const;
   ossimString productLevel()           const;
   ossimString edition()                const;
   ossimString matchMergeVersion()      const;
   ossimString maintanenceDate()        const;
   ossimString matchMergeDate()         const;
   ossimString maintenanceCode()        const;
   ossimString producerCode()           const;
   ossimString productStockSpecNumber() const;
   ossimString productSpecNumber()      const;
   ossimString productSpecDate()        const;
   ossimString verticalDatum()          const;
   ossimString horizontalDatum()        const;
   ossimString compilationDate()        const;
   ossimString latOrigin()              const;
   ossimString lonOrigin()              const;
   ossimString latSW()                  const;
   ossimString lonSW()                  const;
   ossimString latNW()                  const;
   ossimString lonNW()                  const;
   ossimString latNE()                  const;
   ossimString lonNE()                  const;
   ossimString latSE()                  const;
   ossimString lonSE()                  const;

   ossimString orientation()    const;
   ossimString latInterval()    const;
   ossimString lonInterval()    const;
   ossim_int32 numLatPoints()   const;
   ossim_int32 numLonLines()    const;
   ossim_int32 cellIndicator()  const;
   ossim_int32 startOffset()    const;
   ossim_int32 stopOffset()     const;
   
   friend OSSIM_DLL std::ostream& operator<<( std::ostream& os,
                                              const ossimDtedDsi& dsi);

   /**
    * @brief print method that outputs a key/value type format adding prefix
    * to keys.
    * @param out String to output to.
    * @param prefix This will be prepended to key.
    * e.g. Where prefix = "nitf." and key is "file_name" key becomes:
    * "nitf.file_name:"
    * @return output stream.
    */
   std::ostream& print(std::ostream& out,
                       const std::string& prefix) const;   
   
   void parse(std::istream& in);

   /**
    * @brief Gets a property for name.
    * @param name Property name to get.
    * @return ossimRefPtr<ossimProperty> Note that this can be empty if
    * property for name was not found.
    */
   ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   
   /**
    * @brief Adds this class's properties to list.
    * @param propertyNames list to append to.
    */
   void getPropertyNames(std::vector<ossimString>& propertyNames)const;

private:
   // Do not allow...
   ossimDtedDsi(const ossimDtedDsi& source);
   const ossimDtedDsi& operator=(const ossimDtedDsi& rhs);

   
   char theRecSen[FIELD1_SIZE+1];
   char theSecurityCode[FIELD2_SIZE+1];
   char theField3[FIELD3_SIZE+1];
   char theField4[FIELD4_SIZE+1];
   char theProductLevel[FIELD6_SIZE+1];
   char theField7[FIELD7_SIZE+1];
   char theField8[FIELD8_SIZE+1];
   char theEdition[FIELD9_SIZE+1];
   char theMatchMergeVersion[FIELD10_SIZE+1];
   char theMaintenanceDate[FIELD11_SIZE+1];
   char theMatchMergeDate[FIELD12_SIZE+1];
   char theMaintenanceCode[FIELD13_SIZE+1];
   char theProducerCode[FIELD14_SIZE+1];
   char theField15[FIELD15_SIZE+1];
   char theProductStockSpecNumber[FIELD16_SIZE+1];
   char theProductSpecNumber[FIELD17_SIZE+1];
   char theProductSpecDate[FIELD18_SIZE+1];
   char theVerticalDatum[FIELD19_SIZE+1];
   char theHorizontalDatum[FIELD20_SIZE+1];
   char theField21[FIELD21_SIZE+1];
   char theCompilationDate[FIELD22_SIZE+1];
   char theField23[FIELD23_SIZE+1];
   char theLatOrigin[FIELD24_SIZE+1];
   char theLonOrigin[FIELD25_SIZE+1];
   char theLatSW[FIELD26_SIZE+1];
   char theLonSW[FIELD27_SIZE+1];
   char theLatNW[FIELD28_SIZE+1];
   char theLonNW[FIELD29_SIZE+1];
   char theLatNE[FIELD30_SIZE+1];
   char theLonNE[FIELD31_SIZE+1];
   char theLatSE[FIELD32_SIZE+1];
   char theLonSE[FIELD33_SIZE+1];
   char theOrientation[FIELD34_SIZE+1];
   char theLatInterval[FIELD35_SIZE+1];
   char theLonInterval[FIELD36_SIZE+1];
   char theNumLatPoints[FIELD37_SIZE+1];
   char theNumLonLines[FIELD38_SIZE+1];
   char theCellIndicator[FIELD39_SIZE+1];
   char theField40[FIELD40_SIZE+1];
   char theField41[FIELD41_SIZE+1];
   char theField42[FIELD42_SIZE+1];
   
   ossim_int32       theStartOffset;
   ossim_int32       theStopOffset;
};

#endif
