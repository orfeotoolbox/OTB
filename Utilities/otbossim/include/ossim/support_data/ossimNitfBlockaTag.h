//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: BLOCKA tag class declaration.
//
// See document STDI-0002 Table 8-9 for more info.
// 
// http://164.214.2.51/ntb/baseline/docs/stdi0002/final.pdf
//
//----------------------------------------------------------------------------
// $Id: ossimNitfBlockaTag.h 19682 2011-05-31 14:21:20Z dburken $
#ifndef ossimNitfBlockaTag_HEADER
#define ossimNitfBlockaTag_HEADER
#include <ossim/support_data/ossimNitfRegisteredTag.h>

class ossimDpt;

class OSSIM_DLL ossimNitfBlockaTag : public ossimNitfRegisteredTag
{
public:
   enum 
   {
      BLOCK_INSTANCE_SIZE = 2,
      N_GRAY_SIZE         = 5,
      L_LINES_SIZE        = 5,
      LAYOVER_ANGLE_SIZE  = 3,
      SHADOW_ANGLE_SIZE   = 3,
      FIELD_6_SIZE        = 16, // reserved
      FRLC_LOC_SIZE       = 21,
      LRLC_LOC_SIZE       = 21,
      LRFC_LOC_SIZE       = 21,
      FRFC_LOC_SIZE       = 21,
      FIELD_11_SIZE       = 5,  // reserved
      //                -----
      //                  123
   };
   
   /** default constructor */
   ossimNitfBlockaTag();

   /** @return "BLOCKA" as an ossimString. */
   virtual std::string getRegisterTagName() const;
  
   /**
    * Parse method.
    *
    * @param in Stream to parse.
    */
   virtual void parseStream(std::istream& in);
   
   /**
    * Write method.
    *
    * @param out Stream to write to.
    */
   virtual void writeStream(std::ostream& out);
   
   /** @return The size of this record in bytes (123) within a nitf file. */
   virtual ossim_uint32 getSizeInBytes()const;
   
   /**
    * Clears all string fields within the record to some default nothingness.
    */
   virtual void clearFields();

   /**
    * @brief Print method that outputs a key/value type format
    * adding prefix to keys.
    * @param out Stream to output to.
    * @param prefix Prefix added to key like "image0.";
    */
   virtual std::ostream& print(std::ostream& out,
                               const std::string& prefix) const;

   /** @param block number to set block instance to.  Valid range 01 to 99. */
   void setBlockInstance(ossim_uint32 block);

   
   /** @return theBlockInstance as a string. */
   ossimString getBlockInstance() const;
   
   /**
    * @param grayCount Count of gray fill.
    *
    * Valid range:  0 to 99999
    */
   void setNGray(ossim_uint32 grayCount);
   
   /** @return theNGray as a string. */
   ossimString getNGray() const;
   
   /**
    * @param lines Number of rows.
    *
    * Valid range:  0 to 99999
    */
   void setLLines(ossim_uint32 lines);
   
   /** @return theLLines as a string. */
   ossimString getLLines() const;
   
   /**
    * @param angle Value to set theLayoverAngle to.
    *
    * Valid range: 000 to 359 or spaces
    */
   void setLayoverAngle(ossim_uint32 angle);
   
   /** @return theLayoverAngle as a string. */
   ossimString getLayoverAngle() const;
   
   /**
    * @param angle Value to set theLayoverAngle to.
    *
    * Valid range: 000 to 359 or spaces
    */
   void setShadowAngle(ossim_uint32 angle);
   
   /** @return theLayoverAngle as a string. */
   ossimString getShadowAngle() const;
   
   /**
    * Convert latitude and logitude to theFrlcLoc string.
    * 
    * @param pt Ground point of first row, last column.
    *
    * @param longitude Longitude portion in decimal degrees.
    */
   void setFrlcLoc(const ossimDpt& pt);
   
   /** @return theFrlcLoc as a string. */
   ossimString getFrlcLoc() const;
   
   /** @param pt Ground point to initialize with location. */
   void getFrlcLoc(ossimDpt& pt) const;

   /**
    * Convert latitude and logitude to theLrlcLoc string.
    * 
    * @param pt Ground point of last row, last column.
    *
    * @param longitude Longitude portion in decimal degrees.
    */
   void setLrlcLoc(const ossimDpt& pt);
   
   /** @return theLrlcLoc as a string. */
   ossimString getLrlcLoc() const;
   
   /** @param pt Ground point to initialize with location. */
   void getLrlcLoc(ossimDpt& pt) const;

      /**
    * Convert latitude and logitude to theLrfcLoc string.
    * 
    * @param pt Ground point of last row, first column.
    *
    * @param longitude Longitude portion in decimal degrees.
    */
   void setLrfcLoc(const ossimDpt& pt);
   
   /** @return theLrfcLoc as a string. */
   ossimString getLrfcLoc() const;
   
   /** @param pt Ground point to initialize with location. */
   void getLrfcLoc(ossimDpt& pt) const;

      /**
    * Convert latitude and logitude to theFrfcLoc string.
    * 
    * @param pt Ground point of first row, first column.
    *
    * @param longitude Longitude portion in decimal degrees.
    */
   void setFrfcLoc(const ossimDpt& pt);
   
   /** @return theFrfcLoc as a string. */
   ossimString getFrfcLoc() const;
   
   /** @param pt Ground point to initialize with location. */
   void getFrfcLoc(ossimDpt& pt) const;
   
protected:

   /**
    * Parses location string and initializes pt with result.
    *
    * @param locationString String to parse.
    *
    * @param pt Point to initialize.
    */
   void converLocStringToPt(const ossimString& locationString,
                            ossimDpt& pt) const;
                            
   
   /**
    * FIELD: BLOCK_INSTANCE
    *
    * required 2 byte field
    *
    * 01 to 99
    * 
    * Block number of the image.
    */
   char theBlockInstance[BLOCK_INSTANCE_SIZE+1];
   
   /**
    * FIELD: N_GRAY
    *
    * required 5 byte field
    *
    * 00000 to 99999
    *
    * SAR The number of gray fill pixels.
    *
    * EO-IR: 00000
    */
   char theNGray[N_GRAY_SIZE+1];
   
   /**
    * FIELD: L_LINES
    *
    * required 5 byte field
    *
    * 00000 to 99999
    *
    * Row count.
    */
   char theLLines[L_LINES_SIZE+1];
   
   /**
    * FIELD: LAYOVER_ANGLE
    *
    * required 3 byte field
    *
    * 000 to 359 or spaces
    * 
    * SAR: Angle between the first row of pixels and the layover direction in
    * the image, measured in a clockwise direction.
    *
    * EO-IR: spaces
    */
   char theLayoverAngle[LAYOVER_ANGLE_SIZE+1];
   
   /**
    * FIELD: SHADOW_ANGLE
    *
    * required 3 byte field
    *
    * 000 to 359 or spaces
    *
    * SAR: Angle between the first row of pixels and the radar shadow in
    * the image, measured in a clockwase direction.
    *
    * EO-IR: spaces
    */
   char theShadowAngle[SHADOW_ANGLE_SIZE+1];
   
   /**
    * FIELD: FIELD_6
    *
    * Reserved field of 16 spaces.
    */
   char theField6[FIELD_6_SIZE+1];
   
   /**
    * FIELD: FRLC_LOC
    *
    * required 21 byte field
    *
    * XDDMMSS.SSYDDDMMSS.SS
    * OR
    * +-dd.dddddd+-ddd.dddddd
    * OR
    * 21 spaces
    *
    * Location of the first row, last column of the image block.
    */
   char theFrlcLoc[FRLC_LOC_SIZE+1];
   
   /**
    * FIELD: LRLC_LOC
    *
    * required 21 byte field
    *
    * XDDMMSS.SSYDDDMMSS.SS
    * OR
    * +-dd.dddddd+-ddd.dddddd
    * OR
    * 21 spaces
    *
    * Location of the last row, last column of the image block.
    */
   char theLrlcLoc[LRLC_LOC_SIZE+1];
   
   /**
    * FIELD: LRFC_LOC
    *
    * required 21 byte field
    *
    * XDDMMSS.SSYDDDMMSS.SS
    * OR
    * +-dd.dddddd+-ddd.dddddd
    * OR
    * 21 spaces
    *
    * Location of the last row, first column of the image block.
    */
   char theLrfcLoc[LRFC_LOC_SIZE+1];
   
   /**
    * FIELD: FRFC_LOC
    *
    * required 21 byte field
    *
    * XDDMMSS.SSYDDDMMSS.SS
    * OR
    * +-dd.dddddd+-ddd.dddddd
    * OR
    * 21 spaces
    *
    * Location of the first row, first column of the image block.
    */
   char theFrfcLoc[FRFC_LOC_SIZE+1];
   
   /**
    * FIELD: FIELD_11
    *
    * Reserved field of 5 bytes containing: 010.0 
    */
   char theField11[FIELD_11_SIZE+1];
   
TYPE_DATA   
};

#endif /* End of "#ifndef ossimNitfBlockaTag_HEADER" */
