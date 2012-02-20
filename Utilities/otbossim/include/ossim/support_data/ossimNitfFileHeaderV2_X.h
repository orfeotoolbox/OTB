//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
// 
//----------------------------------------------------------------------------
// $Id: ossimNitfFileHeaderV2_X.h 20123 2011-10-11 17:55:44Z dburken $

#ifndef ossimNitfFileHeaderV2_X_HEADER
#define ossimNitfFileHeaderV2_X_HEADER
#include <ossim/base/ossimConstants.h>
#include <ossim/support_data/ossimNitfFileHeader.h>

class ossimKeywordlist;
class ossimLocalTm;

class OSSIM_DLL ossimNitfFileHeaderV2_X : public ossimNitfFileHeader
{
public:
   ossimNitfFileHeaderV2_X();

   static ossimString formatDate(const ossimString& version,
                                 const ossimLocalTm& d);

   virtual void setCodeWords(const ossimString& codeWords) = 0;
   virtual void setControlAndHandling(const ossimString& controlAndHandling) = 0;
   virtual void setReleasingInstructions(const ossimString& releasingInstructions) = 0;
   virtual void setClassificationAuthority(const ossimString& classAuth) = 0;
   virtual void setSecurityControlNumber(const ossimString& controlNo) = 0;
   virtual void setOriginatorsName(const ossimString& originatorName) = 0;
   virtual void setOriginatorsPhone(const ossimString& origniatorPhone) = 0;

   virtual ossimString getCodeWords()const = 0;
   virtual ossimString getControlAndHandling()const = 0;
   virtual ossimString getReleasingInstructions()const = 0;
   virtual ossimString getClassificationAuthority()const = 0;
   virtual ossimString getSecurityControlNumber()const = 0;
   virtual ossimString getOriginatorsName()const = 0;
   virtual ossimString getOriginatorsPhone()const = 0;

   virtual ossimString getSecurityClassification()const;
   void setComplexityLevel(const ossimString& level);
   void setSystemType(const ossimString& systemType);
   void setOriginatingStationId(const ossimString& originationId);

   /**
    * @brief Sets field FDT.
    *
    * Version 2.0 format: DDHHMMSSZMONYY
    * Version 2.1 format: CCYYMMDDhhmmss
    *
    * @param ossimLocalTm Local time.  Note that this will be converted to
    * GMT(ZULU).
    */
   void setDate(const ossimLocalTm& d);

   
   /**
    * @brief Sets field FDT.
    *
    * @param d Date string to copy to FDT field.
    *
    * @note No conversions performed. 
    */
   void setDate(const ossimString& d);

   /**
    * @brief Sets field FDT to current time converted to GMT(ZULU).
    *
    * Version 2.0 format: DDHHMMSSZMONYY
    * Version 2.1 format: CCYYMMDDhhmmss
    */
   virtual void setDate();

   void setTitle(const ossimString& title);
   void setFileSecurityClassification(const ossimString& securityClassification);
   void setCopyNumber(const ossimString& copyNumber);
   void setNumberOfCopies(const ossimString& numberOfCopies);
   void setEncryption(const ossimString& encryption);

   ossimString getComplexityLevel()const;
   ossimString getSystemType()const;
   ossimString getOriginatingStationId()const;
   ossimString getDate()const;
   ossimString getTitle()const;
   ossimString getCopyNumber()const;
   ossimString getNumberOfCopies()const;
   ossimString getEncryption()const;

   virtual void setProperty(ossimRefPtr<ossimProperty> property);
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;

   bool saveState(ossimKeywordlist& kwl, const ossimString& prefix="")const;

   /*!
    * @brief Method to set fields from a keyword list.
    *
    * This is not a true loadState as it does not lookup/initialize all class
    * members.  This was added to allow defaults, e.g OSTAID, to be set via a
    * site configuration file.
    * Code does not return false if a field(key) is not found..
    *
    * @return true if ok or false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);
   
   /**
    * Properties of a NITF 2.x Header file. See MIL-STD-2500A or
    * MIL-STD-2500B for details.
    *
    * To access these, pass the constant to getProperty().
    */
   static const ossimString FILE_TYPE_KW;
   static const ossimString FHDR_KW;
   static const ossimString VERSION_KW;
   static const ossimString CLEVEL_KW;
   static const ossimString STYPE_KW;
   static const ossimString OSTAID_KW;
   static const ossimString FDT_KW;
   static const ossimString FTITLE_KW;
   static const ossimString FSCLAS_KW;
   static const ossimString FSCODE_KW;
   static const ossimString FSCTLH_KW;
   static const ossimString FSREL_KW;
   static const ossimString FSCAUT_KW;
   static const ossimString FSCTLN_KW;
   static const ossimString FSCOP_KW;
   static const ossimString FSCPYS_KW;
   static const ossimString ENCRYP_KW;
   static const ossimString ONAME_KW;
   static const ossimString OPHONE_KW;

protected:

   /**
    * Field: FHDR
    * Required field for NITF and is a 9 byte value
    *
    * format is NITFNN.NN where NN.NN is the version
    * number. The valid version number for this class
    * is NITF02.10.
    *
    * NOTE: This field may contain NSIF01.00 which is also the same as NITF02.10
    * 
    */
   char  theFileTypeVersion[10]; // nine bytes

   /**
    * Field: CLEVEL
    * 
    * Required field. 2 byte field ranges from 01-99
    */
   char  theComplexityLevel[3];  // 2 bytes

   /**
    * Field: STYPE
    * 
    * Required field. 4 bytes
    */
   char  theSystemType[5];      // four bytes

   /**
    * Field: OSTAID
    * 
    * Is required 10 byte field and can't be blank
    * 
    */
   char  theOriginatingStationId[11]; // ten bytes

   /**
    * Field: FDT
    * 
    * for NITF 2.1 format we have
    *
    * Has the form CCYYMMDDhhmmss and is a required field
    *
    * where CC is the first two digits of the year 00-99
    *       YY is the last 2 digits of the year 00-99
    *       MM is the month 01-12
    *       DD is the day 01-31
    *       hh is the hour 00-23
    *       mm is the minutes 00-59
    *       ss is the seconds 00-59
    *
    * For NITF 2.0 format we have
    *
    * DDHHMMSSZMONYY
    *
    * where  DD is the day
    *        HH is the hour
    *        MM is the minutes
    *        SS is the seconds
    *        MON is the first 3 characters of the month
    *        YY is the 2 digit year
    */
   char  theDateTime[15];  // 14 bytes

   /**
    * Field: FTITLE
    * 
    * Is a required field and is an 80 byte
    * Alphanumeric value this can be all BCS spaces.
    */
   char  theFileTitle[81]; // 80 bytes

   /**
    * Field: FSCLAS
    * 
    * This is a reqired field and is 1 byte long and
    * can contain one of the following:
    *
    *  T      Top secret
    *  S      Secret
    *  C      Confidential
    *  R      Restricted
    *  U      Unclassified
    */
   char theSecurityClassification[2];

   /**
    * Field: FSCOP
    * 
    * This is the copy number and is a 5 byte
    * numeric of the form 00000-99999.  This field
    * is required.
    */
   char theCopyNumber[6];

   /**
    * Field: FSCOPYS
    * 
    * This is the total number of copies that exist.
    * This is a 5 byte numeric of the form 00000-99999.  This
    * field is required.
    */
   char theNumberOfCopies[6];

   /**
    * Field: ENCRYP
    * 
    * This is a reqired field and is a 1 byte numeric
    * where 0 is not encrypted and 1 is encrypted.  This field
    * is reuqired.
    */
   char theEncryption[2];

TYPE_DATA
};

#endif

