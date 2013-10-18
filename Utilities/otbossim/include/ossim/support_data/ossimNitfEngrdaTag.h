//----------------------------------------------------------------------------
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: ENGRDA - Engineering Data tag class declaration.
//
// See document STDI-0002 (version 3), Appendix N for more info.
// 
//----------------------------------------------------------------------------
// $Id

#ifndef ossimNitfEngrdaTag_HEADER
#define ossimNitfEngrdaTag_HEADER 1

#include <string>
#include <vector>

#include <ossim/support_data/ossimNitfRegisteredTag.h>

class OSSIM_DLL ossimNitfEngrdaTag : public ossimNitfRegisteredTag
{
public:

   enum 
   {
      RESRC_SIZE   = 20,
      RECNT_SIZE   = 3,
      ENGLN_SIZE   = 2,
      ENGMTXC_SIZE = 4,
      ENGMTXR_SIZE = 4,
      ENGTYP_SIZE  = 1,
      ENGDTS_SIZE  = 1,
      ENGDATU_SIZE = 2,
      ENGDATC_SIZE = 8,
      TMP_BUF_SIZE = 128
   };
   
   /** @brief default constructor */
   ossimNitfEngrdaTag();

   /** @brief destructor */
   virtual ~ossimNitfEngrdaTag();

   /**
    * @brief Method to return tag name.
    * @return "ENGRDA" as a std::string.
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
                               const std::string& prefix=std::string()) const;
   
protected:

   /** @brief Container class for an Engineering Data Element Record. */
   class ossimEngDataElement
   {
   public:
      /** ENGLBL - label */
      std::string theEngLbl;

      /** ENGMTXC - column count */
      ossim_uint16 theEngMtxC;

      /** ENGMTXR - row count */
      ossim_uint16 theEngMtxR;

      /** ENGTYP - data type */
      ossim_int8  theEngTyp;

      /** ENGDTS - data size in bytes */
      ossim_uint8 theEngDts;

      /** ENGDATU - units */
      std::string theEngDatU;

      /** ENGDATA - data */
      std::vector<ossim_uint8> theEngDat;
   };

   /**
    * @brief Method to print out a ossimEngDataElement object.
    * @param out Stream to print to.
    * @param element The element to print.
    * @param elIndex The index of the element array index.
    * @param prefix  The prefix to add to key.
    */
   std::ostream& printData(std::ostream& out,
                           const ossimEngDataElement& element,
                           ossim_uint32 elIndex,
                           const std::string& prefix) const;

   /**
    * @brief Method to convert value to a string, padding with zero's,
    * left justified.
    * @param v The value to convert.
    * @param w The width of the string field.
    * @param s The string to stuff.
    */
   template <class T> void getValueAsString(T v,
                                            ossim_uint16 w,
                                            std::string& s) const;

   /** Type R = Required Type <R> = BCS spaces allowed for entire field */
   
   /**
    * FIELD: RESRC
    *
    * TYPE: R
    * 
    * 20 byte field
    *
    * Unique Source System Name.
    */
   char theReSrc[RESRC_SIZE+1];
   
   /**
    * FIELD: RECNT
    *
    * TYPE: R
    * 
    * 20 byte field
    *
    * Unique Source System Name.
    */
   char theReCnt[RECNT_SIZE+1];

   std::vector<ossimEngDataElement> theData;

   ossim_uint32 theTreLength;
   
TYPE_DATA   
};

#endif /* matches #ifndef ossimNitfEngrdaTag_HEADER */
