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
// $Id: ossimNitfFileHeaderV2_1.h 20123 2011-10-11 17:55:44Z dburken $
#ifndef ossimNitfFileHeaderV2_1_HEADER
#define ossimNitfFileHeaderV2_1_HEADER

#include <iosfwd>

#include <ossim/support_data/ossimNitfFileHeaderV2_X.h>
#include <ossim/base/ossimDate.h>
#include <ossim/base/ossimKeywordlist.h>

class OSSIMDLLEXPORT ossimNitfImageInfoRecordV2_1
{
public:
   friend std::ostream& operator <<(std::ostream& out,
                                    const ossimNitfImageInfoRecordV2_1 &data);
   
   ossim_uint32 getHeaderLength()const;
   ossim_uint64 getImageLength()const;
   ossim_uint64 getTotalLength()const;
   
   void setSubheaderLength(ossim_uint32 length);
   void setImageLength(ossim_uint64 length);
   
   /**
    * Is a 6 byte numeric 000439-999999
    */
   char theImageSubheaderLength[7];
   
   /**
    * Is a 10 byte numeric 0000000001-9999999999
    */
   char theImageLength[11];
};


struct ossimNitfGraphicInfoRecordV2_1
{
public:
   friend std::ostream& operator <<(std::ostream& out,
                                    const ossimNitfGraphicInfoRecordV2_1 &data);
   
   /**
    * Is a 4 byte numeric 0258-9999
    */
   char theGraphicSubheaderLength[5];
   
   /**
    * Is a 6 byte numeric 0000001-9999999
    */
   char theGraphicLength[7];
};


class OSSIMDLLEXPORT ossimNitfTextFileInfoRecordV2_1
{
public:
   friend std::ostream& operator<<(std::ostream& out,
                                   const ossimNitfTextFileInfoRecordV2_1 &data);


   ossim_uint32 getHeaderLength()const;
   ossim_uint32 getTextLength()const;
   ossim_uint32 getTotalLength()const;

   void setSubheaderLength(ossim_uint32 length);
   void setTextLength(ossim_uint64 length);
   
   /**
    * Is a 4 byte numeric 0282-9999
    */
   char theTextFileSubheaderLength[5];

   /**
    * Is a 5 byte numeric 0-99999
    */
   char theTextFileLength[6];
};

struct ossimNitfDataExtSegInfoRecordV2_1
{
public:
   friend std::ostream& operator<<(std::ostream& out,
                                   const ossimNitfDataExtSegInfoRecordV2_1 &data);

   /**
    * Is a 4 byte numeric 200-9999
    */
   char theDataExtSegSubheaderLength[5];

   /**
    * Is a 9 byte numeric 0-999999999
    */
   char theDataExtSegLength[10];
};

struct ossimNitfResExtSegInfoRecordV2_1
{
public:
   friend std::ostream& operator<<(std::ostream& out,
                                   const ossimNitfResExtSegInfoRecordV2_1 &data);

   /**
    * Is a 4 byte numeric 0-9999
    */
   char theResExtSegSubheaderLength[5];

   /**
    * Is a 7 byte numeric 0-9999999
    */
   char theResExtSegLength[8];
};

class OSSIMDLLEXPORT ossimNitfFileHeaderV2_1 : public ossimNitfFileHeaderV2_X
{
public:
   
   ossimNitfFileHeaderV2_1();
   virtual ~ossimNitfFileHeaderV2_1();
   virtual void parseStream(std::istream &in);
   virtual void writeStream(std::ostream &out);
   virtual bool isEncrypted()const;
   virtual ossim_int32 getNumberOfImages()const;
   virtual ossim_int32 getNumberOfLabels()const;
   virtual ossim_int32 getNumberOfSymbols()const;
   virtual ossim_int32 getNumberOfGraphics()const;
   virtual ossim_int32 getNumberOfTextSegments()const;
   virtual ossim_int32 getNumberOfDataExtSegments()const;
   virtual ossim_int32 getHeaderSize()const;
   virtual ossim_int64 getFileSize()const;
   virtual const char* getVersion()const;
   virtual const char* getDateTime()const;
   virtual ossimDrect getImageRect()const;

   virtual void addImageInfoRecord(const ossimNitfImageInfoRecordV2_1& recordInfo);
   virtual void addTextInfoRecord(const ossimNitfTextFileInfoRecordV2_1& recordInfo);
	virtual void addDataExtSegInfoRecord(const ossimNitfDataExtSegInfoRecordV2_1& recordInfo);

   virtual void replaceImageInfoRecord(int i, const ossimNitfImageInfoRecordV2_1& recordInfo);
   
   virtual ossimNitfImageHeader*  getNewImageHeader(ossim_uint32 imageNumber,
                                                    std::istream& in)const;
   virtual ossimNitfSymbolHeader* getNewSymbolHeader(ossim_uint32 symbolNumber,
                                                     std::istream& in)const;
   virtual ossimNitfLabelHeader* getNewLabelHeader(ossim_uint32 labelNumber,
                                                   std::istream& in)const;
   virtual ossimNitfTextHeader* getNewTextHeader(ossim_uint32 textNumber,
                                                   std::istream& in)const;
   virtual ossimNitfDataExtensionSegment* getNewDataExtensionSegment(
      ossim_uint32 dataExtNumber, std::istream& in)const;

   virtual ossimNitfImageHeader*  allocateImageHeader()const;
   virtual ossimNitfSymbolHeader* allocateSymbolHeader()const;
   virtual ossimNitfLabelHeader*  allocateLabelHeader()const;
   virtual ossimNitfTextHeader*   allocateTextHeader()const;
   virtual ossimNitfDataExtensionSegment *allocateDataExtSegment()const;

   virtual ossimString getSecurityClassificationSys()const;
   virtual ossimString getCodeWords()const;
   virtual ossimString getControlAndHandling()const;
   virtual ossimString getReleasingInstructions()const;
   virtual ossimString getDeclassificationType()const;
   virtual ossimString getDeclassificationDate()const;
   virtual ossimString getDeclassificationExemption()const;
   virtual ossimString getDowngrade()const;
   virtual ossimString getDowngradingDate()const;
   virtual ossimString getClassificationText()const;
   virtual ossimString getClassificationAuthorityType()const;
   virtual ossimString getClassificationAuthority()const;
   virtual ossimString getClassificationReason()const;
   virtual ossimString getSecuritySourceDate()const;
   virtual ossimString getSecurityControlNumber()const;
   virtual void getBackgroundColor(ossim_uint8& r,
                                   ossim_uint8& g,
                                   ossim_uint8& b)const;
   virtual ossimString getOriginatorsName()const;
   virtual ossimString getOriginatorsPhone()const;
   
   virtual void setFileLength(ossim_uint64 fileLength);
   virtual void setHeaderLength(ossim_uint64 headerLength);
   
   virtual void setSecurityClassificationSys(const ossimString& value);
   virtual void setCodeWords(const ossimString& codeWords);
   virtual void setControlAndHandling(const ossimString& controlAndHandling);
   virtual void setReleasingInstructions(const ossimString& releasingInstructions);
   virtual void setDeclassificationType(const ossimString& declassType);
   virtual void setDeclassificationDate(const ossimLocalTm& declassDate);
   virtual void setDeclassificationDate(const ossimString& declassDate);
   virtual void setDeclassificationExemption(const ossimString& exemption);
   virtual void setDowngrade(const ossimString& downgrade);
   virtual void setDowngradingDate(const ossimLocalTm& date);
   virtual void setDowngradingDate(const ossimString& date);
   virtual void setClassificationText(const ossimString& classificationText);
   virtual void setClassificationAuthorityType(const ossimString& authorityType);
   virtual void setClassificationAuthority(const ossimString& authorityType);
   virtual void setClassificationReason(const ossimString& reason);
   virtual void setSecuritySourceDate(const ossimLocalTm& date);
   virtual void setSecuritySourceDate(const ossimString& date);
   virtual void setSecurityControlNumber(const ossimString& number);
   virtual void setFileBackgroundColor(ossim_uint8 r,
                                       ossim_uint8 g,
                                       ossim_uint8 b);
   virtual void setOriginatorsName(const ossimString& name);
   virtual void setOriginatorsPhone(const ossimString& phone);
   
   virtual void setProperty(ossimRefPtr<ossimProperty> property);
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;

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
    * Properties of a NITF 2.1 Header file. See MIL-STD-2500B for details.
    *
    * To access these, pass the constant to getProperty().
    */
   static const ossimString FSCLSY_KW;
   static const ossimString FSDCTP_KW;
   static const ossimString FSDCDT_KW;
   static const ossimString FSDCXM_KW;
   static const ossimString FSDG_KW;
   static const ossimString FSDGDT_KW;
   static const ossimString FSCLTX_KW;
   static const ossimString FSCATP_KW;
   static const ossimString FSCRSN_KW;
   static const ossimString FSSRDT_KW;
   static const ossimString FBKGC_KW;
   
   void clearFields();

   /**
    * @brief print method that outputs a key/value type format adding prefix
    * to keys.
    */
   virtual std::ostream& print(std::ostream& out,
                               const std::string& prefix=std::string()) const;
   
private:
   /**
    * Sets the number of image records in the NITF 2.1 File Header.
    *
    * @param num
    *        The number of image records for the entire NITF 2.1 file.
    *
    * @throw std::out_of_range
    */
   void setNumberOfImageInfoRecords(ossim_uint64 num);
   void setNumberOfTextInfoRecords(ossim_uint64 num);
   void setNumberOfGraphicInfoRecords(ossim_uint64 num);
   void setNumberOfDataExtSegInfoRecords(ossim_uint64 num);

   void readImageInfoRecords(std::istream &in);
   void readGraphicInfoRecords(std::istream &in);
   void readTextFileInfoRecords(std::istream &in);
   void readDataExtSegInfoRecords(std::istream &in);
   void readResExtSegInfoRecords(std::istream &in);

   /**
    * If the header was parsed this method will initialize the offsets
    * to whare all data resides within the file.  Example:  NITF files
    * have several sections (Image, Symbol, Labels, Text and Extended data) and
    * each of these sections have subsections.  The offsets stored are just offsets
    * from the start of the file to each major section and eqach subsection.
    */
   void initializeAllOffsets();
   
   // Note: these are work variables and not part of the
   // Nitf header.  These variables will be used to quickly
   // access various parts of the file.
   /**
    * This is not part of the NITF file header.  This will be
    * computed since somethimes the header size will not be
    * given in the header field.  This value will hold a count of
    * all the bytes read for the header portion.  This can be used
    * to skip past the header data.
    */
   ossim_uint32 theHeaderSize;

   /**
    * This will be pre-computed when the header file is opened.  This is
    * computed after we have found the display order.  We will use the
    * rectangle of the smallest display number.
    */
   ossimDrect theImageRect;
   
   /**
    * This will hold pre-computed offsets to the start of all image headers
    * and start to their data.
    */
   std::vector<ossimNitfImageOffsetInformation>  theImageOffsetList;
   
   // START Of header variables
   /**
    * Field: FSCLSY
    * 
    * Is a 2 byte field.  Required but can be blank:
    *
    */
   char theSecurityClassificationSys[3];

   /**
    * Field: FSCODE
    * 
    * This is a 11 byte Alphanumeric field
    */
   char theCodewords[12]; 

   /**
    * Field: FSCTLH
    * 
    * This is a 2 byte Alphanumeric field.
    */
   char theControlAndHandling[3]; 

   /**
    * Field: FSREL
    * 
    * This is a 20 byte Alphanumeric field.
    */
   char theReleasingInstructions[21]; 

   /**
    * Field: FSDCTP
    * 
    * This is a 2 byte alphanumeric field.
    * 
    * DD    declassify on a specific date
    * DE    declassify upon occurrence of an event
    * GD    downgrade to a specified level on a specific date)
    * GE    downgrade to a specified level upon occurrence of an event)
    * O     OADR
    * X     exempt from automatic declassification
    *
    * NOTE: If this field is all BCS spaces (0x20), it shall imply that no
    *       file security declassification or
    */ 
   char theDeclassificationType[3];

   /**
    * Field: FSDCDT
    * 
    * This is a 8 byte field and has the form
    * CCYYMMDD where:
    *
    * CC is the firs 2 chars of the year 00-99
    * YY is the second 2 chars of the year 00-99
    * MM is the month 01-12
    * DD is the day 01-31
    */
   char theDeclassificationDate[9]; 

   /**
    * Field: FSDCXM
    * 
    * Is a 4 byte field
    */
   char theDeclassificationExemption[5];
   
   /**
    * Field: FSDG
    * 
    * Can be blank.  Is a 1 byte field valid
    * values are:
    *
    * S = secret
    * C = Confidential
    * R = Restricted
    * 0x20 or BCS spaces.
    *
    * if spaces then security downgrading does not apply.
    */
   char theDowngrade[2];

   /**
    * Field: FSDGDT
    * 
    * Is an 8 byte field of the form
    * CCYYMMDD.
    */
   char theDowngradingDate[9];

   /**
    * Field: FSCLTX
    * 
    * Is a 43 byte field and can contain BCS spaces (0x20).
    */
   char theClassificationText[44];

   /**
    * Field: FSCATP
    *
    * Values can be:
    * 
    * O  Original classification authority
    * D  derivative from a single source
    * M  derivative from multiple sources
    * 
    * Is a 1 byte field and can contain BCS spaces (0x20).
    */
   char theClassificationAuthorityType[2];

   /**
    * Field: FSCAUT
    * 
    * Is a 40 byte field and can contain BCS spaces (0x20).
    */
   char theClassificationAuthority[41];

   /**
    * Field: FSCRSN
    * 
    * Is a 1 byte field and can contain BCS spaces (0x20).
    *
    * Valid values are A-G
    */
   char theClassificationReason[2];

   /**
    * Field: FSSRDT
    * 
    * Is a 8 byte field of the form CCYYMMDD and can be
    * BCS space (0x20).
    */
   char theSecuritySourceDate[9];

   /**
    * Field: FSCTLN
    * 
    * Is a 15 byte field and can be blank (0x20)
    */
   char theSecurityControlNumber[16];
   
   /**
    * Field: FBKGC
    * 
    * Required.  Is a 3 byte field and is reuqired.  Note:  This
    * field is binary and has range
    *
    *      (0x00-0xff, 0x00-0xff, 0x00-0xff)
    *
    * each byte can range from 0 to 255 binary
    */
   ossim_uint8 theFileBackgroundColor[3];
   
   /**
    * Field: ONAME
    * 
    * This is a 24 byte Alphanumeric number
    */
   char theOriginatorsName[25];

   /**
    * Field: OPHONE
    * 
    * This is an 18 byte Alphanumeric value. 
    */
   char theOriginatorsPhone[19]; 

   /**
    * Field: FL
    * 
    * This is a required field.
    *
    * Is a 12 byte number of the form
    * 000000000388-999999999998 indicates the
    * file length and a value of 999999999999
    * indicates that the file length was not
    * available.
    */
   char theFileLength[13];

   /**
    * Field: HL
    * 
    * This is a reqired field.
    *
    * ranges from 000388-999999.
    */
   char theHeaderLength[7];

   /**
    * Field: NUMI
    * 
    * This is a 3 byte field that specifies the number
    * of images in the file.  0-999
    */
   char theNumberOfImageInfoRecords[4];

   /**
    * This information is conditional on the number of image
    * info records field.  If the filed is greater than 0 then
    * the record information is read in up t the indicated number
    */
   std::vector<ossimNitfImageInfoRecordV2_1> theNitfImageInfoRecords;

   /**
    * Field: NUMS
    * 
    * This info is required and ranges from 000-999.
    */
   char theNumberOfGraphicInfoRecords[4];

   /**
    * This is an optional field and will depend on
    * theNumberOfGraphicInfoRecords.  If 0 then does not exist.
    */
   std::vector<ossimNitfGraphicInfoRecordV2_1> theNitfGraphicInfoRecords;

   /**
    * Field: NUMX
    * 
    * Is a 3 byte field and is reserved for future use.
    */
   char theReservedForFutureUse1[4];
   
   /**
    * Field: NUMT
    * 
    * This is a 3 byte numeric 0-999.
    */
   char theNumberOfTextFileInfoRecords[4];

   /**
    * This information is conditional on the number of text file
    * info records field.  If the filed is greater than 0 then
    * the record information is read in up t the indicated number
    */
   std::vector<ossimNitfTextFileInfoRecordV2_1> theNitfTextFileInfoRecords;

   /**
    * Field: NUMDES
    * 
    * This is a 3 byte numeric 0-999.
    */
   char theNumberOfDataExtSegInfoRecords[4];

   /**
    * This information is conditional on the number of 
    * info records field.  If the filed is greater than 0 then
    * the record information is read in up t the indicated number
    */
   std::vector<ossimNitfDataExtSegInfoRecordV2_1> theNitfDataExtSegInfoRecords;

   /**
    * Field: NUMRES
    * 
    * This is a 3 byte numeric 0-999. Reserved Extension Segments
    */
   char theNumberOfResExtSegInfoRecords[4];

   /**
    * This information is conditional on the number of Res Ext Seg
    * info records field.  If the filed is greater than 0 then
    * the record information is read in up t the indicated number
    */
   std::vector<ossimNitfResExtSegInfoRecordV2_1> theNitfResExtSegInfoRecords;

   /**
    * Field: UDHDL
    * 
    * This is a required field and is a 5 byte numeric ranging
    * from 0-99999.  This is 0 if there is no data.  Valid values
    * are 00000 or 00003-99999
    */
   char theUserDefinedHeaderDataLength[6];

   /**
    * Field: UDHOFL
    * 
    * This exists if theUserDefinedHeaderDataLength is not 0.
    */
   char theUserDefinedHeaderOverflow[4];

   /**
    * Field: XHDL
    * 
    * This is a required field.  It is 5 bytes long and
    * ranges from 0-99999.  0 means there is no data
    */
   char theExtendedHeaderDataLength[6];

   /**
    * Field: XHDLOFL
    * 
    * This is conditional on theExtendedHeaderDataLength.  If
    * that field is 0 then this does not exist.  This is a 3
    * byte field.
    */
   char theExtendedHeaderDataOverflow[4];

TYPE_DATA
};

#endif
