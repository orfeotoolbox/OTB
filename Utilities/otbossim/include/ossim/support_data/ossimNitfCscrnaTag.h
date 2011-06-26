//----------------------------------------------------------------------------
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: CSCRNA tag class declaration.
//
// Corner Footprint TRE.
//
// See document STDI-0006-NCDRD Table 3.2-13 for more info.
// 
//----------------------------------------------------------------------------
// $Id

#ifndef ossimNitfCscrnaTag_HEADER
#define ossimNitfCscrnaTag_HEADER 1
#include <ossim/support_data/ossimNitfRegisteredTag.h>

class OSSIM_DLL ossimNitfCscrnaTag : public ossimNitfRegisteredTag
{
public:
   /** @brief default constructor */
   ossimNitfCscrnaTag();

   /** @brief destructor */
   virtual ~ossimNitfCscrnaTag();

   /**
    * @brief Method to return tag name.
    * @return "CSCRNA" as an std::string.
    */
   virtual std::string getRegisterTagName()const;

   /** @brief Method to parse data from stream. */
   virtual void parseStream(std::istream& in);

   /** @brief Method to write data to stream. */
   virtual void writeStream(std::ostream& out);

   /** @return Byte size of this tag, 109 without CETAG and CEL fields. */
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

   //---
   // NOTE:  Spec says corner points are project from image plane to
   // reference ellipsoid NOT ellipsoid + height.
   //---

   /**
    * FIELD: PREDICT_CORNERS
    * 
    * 1 byte field BCS-A
    *
    * Y = predicted, N = actual
    */
   char thePredictedCornerFlag[2];

   /**
    * FIELD: ULCRN_LAT
    *
    * 9 byte field BCS-N Degrees
    *
    * -90.000000 ti +90.000000
    */
   char theUlLat[10];

   /**
    * FIELD: ULCRN_LON
    *
    * 10 byte field BCS-N Degrees
    *
    * -179.999999 ti +180.000000
    */
   char theUlLon[11];

   /**
    * FIELD: ULCRN_HT
    *
    * 8 byte field BCS-N Meters
    *
    * Height referenced to the reference ellipsoid.
    *
    * -00610.0 to +10668.0
    */
   char theUlHt[9];

    /**
    * FIELD: URCRN_LAT
    *
    * 9 byte field BCS-N Degrees
    *
    * -90.000000 ti +90.000000
    */
   char theUrLat[10];

   /**
    * FIELD: URCRN_LON
    *
    * 10 byte field BCS-N Degrees
    *
    * -179.999999 ti +180.000000
    */
   char theUrLon[11];

   /**
    * FIELD: URCRN_HT
    *
    * 8 byte field BCS-N Meters
    *
    * Height referenced to the reference ellipsoid.
    *
    * -00610.0 to +10668.0
    */
   char theUrHt[9];

   /**
    * FIELD: LRCRN_LAT
    *
    * 9 byte field BCS-N Degrees
    *
    * -90.000000 ti +90.000000
    */
   char theLrLat[10];

   /**
    * FIELD: LRCRN_LON
    *
    * 10 byte field BCS-N Degrees
    *
    * -179.999999 ti +180.000000
    */
   char theLrLon[11];

   /**
    * FIELD: LRCRN_HT
    *
    * 8 byte field BCS-N Meters
    *
    * Height referenced to the reference ellipsoid.
    *
    * -00610.0 to +10668.0
    */
   char theLrHt[9];

   /**
    * FIELD: LLCRN_LAT
    *
    * 9 byte field BCS-N Degrees
    *
    * -90.000000 ti +90.000000
    */
   char theLlLat[10];

   /**
    * FIELD: LLCRN_LON
    *
    * 10 byte field BCS-N Degrees
    *
    * -179.999999 ti +180.000000
    */
   char theLlLon[11];

   /**
    * FIELD: LLCRN_HT
    *
    * 8 byte field BCS-N Meters
    *
    * Height referenced to the reference ellipsoid.
    *
    * -00610.0 to +10668.0
    */
   char theLlHt[9];
   
TYPE_DATA   
};

#endif /* matches #ifndef ossimNitfCscrnaTag_HEADER */
