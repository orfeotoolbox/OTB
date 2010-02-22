//*******************************************************************
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Ken Melero
// 
// Description:  This class gives access to the Volume Header Label
//               (VOL) of a DTED Level 1 file.
//
//********************************************************************
// $Id: ossimDtedVol.h 16104 2009-12-17 18:09:59Z gpotts $

#ifndef ossimDtedVol_H
#define ossimDtedVol_H

#include <iosfwd>

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimErrorStatusInterface.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimRefPtr.h>

class ossimProperty;

class OSSIM_DLL ossimDtedVol : public ossimErrorStatusInterface
{
public:
   ossimDtedVol(const ossimFilename& dted_file="",
                ossim_int32 offset=0);
   ossimDtedVol(std::istream& in);

   enum
   {
      VOL_LENGTH            = 80,
      VOL_ONE_LABEL_1       =  4,
      VOL_REEL_NUMBER       =  5,
      VOL_REEL_ACCESS       = 11,
      VOL_SPACE             = 12,
      VOL_ACCOUNT_NUMBER    = 38,
      VOL_BLANKS            = 52,
      VOL_ONE_LABEL_2       = 80,
      FIELD1_SIZE           = 3,
      FIELD2_SIZE           = 1,
      FIELD3_SIZE           = 6,
      FIELD4_SIZE           = 1,
      FIELD5_SIZE           = 26,
      FIELD6_SIZE           = 14,
      FIELD7_SIZE           = 28,
      FIELD8_SIZE           = 1
   };
   
   // The Recognition Sentinel signifies if the VOL record exists.
   ossimString getRecognitionSentinel() const;
   ossimString getReelNumber()           const;
   ossimString getAccountNumber()        const;
   ossim_int32 startOffset()             const;
   ossim_int32 stopOffset()              const;
   
   friend OSSIM_DLL std::ostream& operator<<( std::ostream& os,
                                              const ossimDtedVol& vol);
   
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
private:
   // Do not allow...
   ossimDtedVol(const ossimDtedVol& source);
   const ossimDtedVol& operator=(const ossimDtedVol& rhs);

   char theRecSen[FIELD1_SIZE+1];
   char theField2[FIELD2_SIZE+1];
   char theReelNumber[FIELD3_SIZE+1];
   char theField4[FIELD4_SIZE+1];
   char theField5[FIELD5_SIZE+1];
   char theAccountNumber[FIELD6_SIZE+1];
   char theField7[FIELD7_SIZE+1];
   char theField8[FIELD8_SIZE+1];
  
   ossim_int32 theStartOffset;
   ossim_int32 theStopOffset;
};

#endif
