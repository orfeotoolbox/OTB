//*******************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Ken Melero
// 
// Description:  This class gives access to the File Header Label
//               (HDR) of a DTED Level 1 file.
//
//********************************************************************
// $Id: ossimDtedHdr.h 16104 2009-12-17 18:09:59Z gpotts $

#ifndef ossimDtedHdr_H
#define ossimDtedHdr_H
#include <iosfwd>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimErrorStatusInterface.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimRefPtr.h>

class ossimProperty;

class OSSIM_DLL ossimDtedHdr : public ossimErrorStatusInterface
{
public:
   ossimDtedHdr(const ossimFilename& dted_file="", ossim_int32 offset=0);
   ossimDtedHdr(std::istream& in);

   enum
   {
      HDR_LENGTH            = 80,
      HDR_ONE_LABEL_1       =  4,
      HDR_FILENAME          =  5,
      HDR_UNIVAC            = 22,
      HDR_REEL              = 28,
      HDR_FILE_NUM          = 32,
      HDR_GENERATION_NUM    = 36,
      HDR_VERSION_NUM       = 40,
      HDR_CREATION_DATE     = 42,
      HDR_EXPIRATION_DATE   = 48,
      HDR_ACCESS            = 54,
      HDR_BLOCK_COUNT       = 55,
      HDR_QUALIFIER         = 61,
      HDR_BLANKS            = 74,
      FIELD1_SIZE           = 3,
      FIELD2_SIZE           = 1,
      FIELD3_SIZE           = 17,
      FIELD4_SIZE           = 6,
      FIELD5_SIZE           = 4,
      FIELD6_SIZE           = 4,
      FIELD7_SIZE           = 4,
      FIELD8_SIZE           = 2,
      FIELD9_SIZE           = 6,
      FIELD10_SIZE          = 6,
      FIELD11_SIZE          = 1,
      FIELD12_SIZE          = 6,
      FIELD13_SIZE          = 13,
      FIELD14_SIZE          = 7
   };

   // The Recognition Sentinel signifies if the HDR record exists.
   ossimString recognitionSentinel() const;

   ossimString fileName()     const;
   ossimString version()      const;
   ossimString creationDate() const;
   ossim_int32 startOffset()  const;
   ossim_int32 stopOffset()   const;

   friend OSSIM_DLL std::ostream& operator<<( std::ostream& os,
                                              const ossimDtedHdr& hdr);

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
   ossimDtedHdr(const ossimDtedHdr& source);
   const ossimDtedHdr& operator=(const ossimDtedHdr& rhs);
   
   char theRecSen[FIELD1_SIZE+1];
   char theField2[FIELD2_SIZE+1];
   char theFilename[FIELD3_SIZE+1];
   char theField4[FIELD4_SIZE+1];
   char theField5[FIELD5_SIZE+1];
   char theField6[FIELD6_SIZE+1];
   char theVersion[FIELD7_SIZE+1];
   char theCreationDate[FIELD8_SIZE+1];
   char theField9[FIELD9_SIZE+1];
   char theField10[FIELD10_SIZE+1];
   char theField11[FIELD11_SIZE+1];
   char theField12[FIELD12_SIZE+1];
   char theField13[FIELD13_SIZE+1];
   char theField14[FIELD14_SIZE+1];
   
   ossim_int32 theStartOffset;
   ossim_int32 theStopOffset;
};

#endif
