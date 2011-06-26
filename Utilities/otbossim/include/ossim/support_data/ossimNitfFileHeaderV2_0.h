//*******************************************************************
//
//  License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfFileHeaderV2_0.h 18413 2010-11-11 19:56:22Z gpotts $
#ifndef ossimNitfFileHeaderV2_0_HEADER
#define ossimNitfFileHeaderV2_0_HEADER

#include <iosfwd>
#include <ossim/support_data/ossimNitfFileHeaderV2_X.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimString.h>
#include <ossim/support_data/ossimNitfTagInformation.h>

class ossimNitfRegisteredTag;

class ossimNitfImageInfoRecordV2_0
{
public:
   friend std::ostream& operator <<(std::ostream& out,
                                    const ossimNitfImageInfoRecordV2_0 &data);

   ossim_uint32 getHeaderLength()const;
   ossim_uint64 getImageLength()const;
   ossim_uint64 getTotalLength()const;
   void setSubheaderLength(ossim_uint32 length);
   void setImageLength(ossim_uint64 length);

   /*!
    * Is a 6 byte numeric 0-999999
    */
   char theImageSubheaderLength[7];

   /*!
    * Is a 10 byte numeric 0-999999999
    */
   char theImageLength[11];
};

struct ossimNitfSymbolInfoRecordV2_0
{
public:
   friend std::ostream& operator <<(std::ostream& out,
                               const ossimNitfSymbolInfoRecordV2_0 &data);

   ossim_int32 getHeaderLength()const;
   ossim_int32 getImageLength()const;
   ossim_int32 getTotalLength()const;
   
   /*!
    * Is a 4 byte numeric 0-9999
    */
   char theSymbolSubheaderLength[5];

   /*!
    * Is a 6 byte numeric 0-999999
    */
   char theSymbolLength[7];
};

struct ossimNitfLabelInfoRecordV2_0
{
public:
   friend std::ostream& operator <<(std::ostream& out,
                               const ossimNitfLabelInfoRecordV2_0 &data);

   ossim_int32 getHeaderLength()const;
   ossim_int32 getImageLength()const;
   ossim_int32 getTotalLength()const;
   
   /*!
    * Is a 4 byte numeric 0-9999
    */
   char theLabelSubheaderLength[5];

   /*!
    * Is a 3 byte numeric 0-320
    */
   char theLabelLength[4];
};

struct ossimNitfTextInfoRecordV2_0
{
public:
   friend std::ostream& operator <<(std::ostream& out,
                               const ossimNitfTextInfoRecordV2_0 &data);

   ossim_int32 getHeaderLength()const;
   ossim_int32 getImageLength()const;
   ossim_int32 getTotalLength()const;
   /*!
    * Is a 4 byte numeric 0-9999
    */
   char theTextSubheaderLength[5];

   /*!
    * Is a 5 byte numeric 0-99999
    */
   char theTextLength[6];
};

struct ossimNitfDataExtSegInfoRecordV2_0
{
public:
   friend std::ostream& operator <<(std::ostream& out,
                               const ossimNitfDataExtSegInfoRecordV2_0 &data);

   ossim_int32 getHeaderLength()const;
   ossim_int32 getImageLength()const;
   ossim_int32 getTotalLength()const;
   /*!
    * Is a 4 byte numeric 0-9999
    */
   char theDataExtSegSubheaderLength[5];

   /*!
    * Is a 9 byte numeric 0-999999999
    */
   char theDataExtSegLength[10];
};

struct ossimNitfResExtSegInfoRecordV2_0
{
public:
   friend std::ostream& operator <<(std::ostream& out,
                               const ossimNitfResExtSegInfoRecordV2_0 &data);

   /*!
    * Is a 4 byte numeric 0-9999
    */
   char theResExtSegSubheaderLength[5];

   /*!
    * Is a 7 byte numeric 0-9999999
    */
   char theResExtSegLength[8];
};

class OSSIMDLLEXPORT ossimNitfFileHeaderV2_0 : public ossimNitfFileHeaderV2_X
{
public:
   
   ossimNitfFileHeaderV2_0();
   virtual ~ossimNitfFileHeaderV2_0();

   /**
    * @brief parse method.
    * @return This method returns void but will throw an std::exception in
    * certain instances if the stream goes bad on seeks.
    */
   virtual void parseStream(std::istream &in);
   
   virtual void writeStream(std::ostream &out);
   
   /**
    * @brief print method that outputs a key/value type format adding prefix
    * to keys.
    */
   virtual std::ostream& print(std::ostream& out,
                               const std::string& prefix=std::string()) const;
   
   virtual bool isEncrypted()const;
   virtual ossim_int32 getNumberOfImages()const;
   virtual ossim_int32 getNumberOfLabels()const;
   virtual ossim_int32 getNumberOfSymbols()const;
   virtual ossim_int32 getNumberOfGraphics()const;
   virtual ossim_int32 getNumberOfTextSegments()const;
   virtual ossim_int32 getNumberOfDataExtSegments()const;
   virtual ossim_int32 getHeaderSize()const;
   virtual ossim_int64 getFileSize()const;
   virtual ossimString getSecurityClassification()const;
   
   virtual const char* getDateTime()const;
   virtual const char* getVersion()const;

   virtual ossimDrect getImageRect()const;
  
   virtual void addImageInfoRecord(const ossimNitfImageInfoRecordV2_0& recordInfo);
   virtual void replaceImageInfoRecord(ossim_uint32 i, const ossimNitfImageInfoRecordV2_0& recordInfo);

   virtual ossimNitfImageHeader*  getNewImageHeader(ossim_uint32 imageNumber,
                                                    std::istream& in)const;
   virtual ossimNitfSymbolHeader* getNewSymbolHeader(ossim_uint32 symbolNumber,
                                                     std::istream& in)const;
   virtual ossimNitfLabelHeader* getNewLabelHeader(ossim_uint32 labelNumber,
                                                   std::istream& in)const;
   virtual ossimNitfTextHeader*   getNewTextHeader(ossim_uint32 textNumber,
                                                   std::istream& in)const;
   virtual ossimNitfDataExtensionSegment* getNewDataExtensionSegment(
      ossim_uint32 dataExtNumber, std::istream& in)const;
   
   virtual ossimNitfImageHeader*  allocateImageHeader()const;
   virtual ossimNitfSymbolHeader* allocateSymbolHeader()const;
   virtual ossimNitfLabelHeader*  allocateLabelHeader()const;
   virtual ossimNitfTextHeader*   allocateTextHeader()const;
   virtual ossimNitfDataExtensionSegment *allocateDataExtSegment()const;

   virtual void setCodeWords(const ossimString& codeWords);
   virtual void setControlAndHandling(const ossimString& controlAndHandling);
   virtual void setReleasingInstructions(const ossimString& releasingInstructions);
   virtual void setClassificationAuthority(const ossimString& classAuth);
   virtual void setSecurityControlNumber(const ossimString& controlNo);
   virtual void setOriginatorsName(const ossimString& originatorName);
   virtual void setOriginatorsPhone(const ossimString& origniatorPhone);

   virtual ossimString getCodeWords()const;
   virtual ossimString getControlAndHandling()const;
   virtual ossimString getReleasingInstructions()const;
   virtual ossimString getClassificationAuthority()const;
   virtual ossimString getSecurityControlNumber()const;
   virtual ossimString getOriginatorsName()const;
   virtual ossimString getOriginatorsPhone()const;

   ossimString getComplianceLevel()const;
   ossimString getSecurityDowngrade()const;
   ossimString getDowngradingEvent()const;

   void setComplianceLevel(const ossimString& complianceLevel);
   void setSecurityDowngrade(const ossimString& securityDowngrade);
   void setDowngradingEvent(const ossimString& downgradeEvent);

   virtual void setFileLength(ossim_uint64 fileLength);
   virtual void setHeaderLength(ossim_uint64 headerLength);
   /**
    * Properties of a NITF 2.0 Header file. See MIL-STD-2500A for details.
    *
    * To access these, pass the constant to getProperty().
    */
   static const ossimString FSDWNG_KW;
   static const ossimString FSDEVT_KW;

   virtual void setProperty(ossimRefPtr<ossimProperty> property);
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;

   
   virtual bool saveState(ossimKeywordlist& kwl, const ossimString& prefix="")const;
TYPE_DATA
private:

   struct ossimNitfDisplayInfo
   {
   public:
      ossimNitfDisplayInfo(const ossimString &filePartType,
                           ossim_int32 displayLevel,
                           ossim_int32 indexIntoList)
         :theFilePartType(filePartType),
          theDisplayLevel(displayLevel),
          theIndex(indexIntoList)
         {
         }
      
      /*!
       * Since NITF already has unique ids for the file
       * part types we will just use them.
       *
       *  IM  indicates image part
       *  LA  indicates a label part
       *  SY   indicates a symbol part
       */
      ossimString theFilePartType;

      /*!
       * This is the display level of the renderable data.  Note the
       * display level is like a painters algorithm.  The lowest level
       * is drawn first followed by the next level.
       */
      ossim_int32        theDisplayLevel;

      /*!
       * This is an index into the array that has all the offsets
       * pre-computed.
       */
      ossim_int32        theIndex;
      
   };

   void clearFields();
   /**
    * Sets the number of image records in the NITF 2.0 File Header.
    *
    * @param num
    *        The number of image records for the entire NITF 2.0 file.
    *
    * @throw std::out_of_range
    */
   void setNumberOfImageInfoRecords(ossim_uint64 num);
   
  /*!
    * This method will be used to setup information about the file.
    * Example: NITF files have display levels where the lowest number is the
    * back most image and the highes number is the front most image.  We
    * are not guranteed that they will be in order in the file so we must
    * parse the file and sort the images.  The images include image data,
    * symbols and labels.  Each of these are renderable.  The lowest
    * display level should have the larges rect and this will be used
    * to send back to the user an image rect.  During rasterization we must
    * know the order to draw the data.
    */
   void precomputeValues();
   
   void readImageInfoRecords(std::istream &in);
   void readSymbolInfoRecords(std::istream &in);
   void readLabelInfoRecords(std::istream &in);
   void readTextFileInfoRecords(std::istream &in);
   void readDataExtSegInfoRecords(std::istream &in);
   void readResExtSegInfoRecords(std::istream &in);
   /*!
    * If the header was parsed this method will initialize the offsets
    * to whare all data resides within the file.  Example:  NITF files
    * have several sections (Image, Symbol, Labels, Text and Extended data) and
    * each of these sections have subsections.  The offsets stored are just offsets
    * from the start of the file to each major section and eqach subsection.
    */
   void initializeAllOffsets();

   /*!
    * This method will be called after the header is read
    * in and the offsets have been initialized.  It will
    * sort in order all renderable data based on their
    * display level.
    */
   void initializeDisplayLevels(std::istream& in);

   void insertIntoDisplayInfoList(const ossimNitfDisplayInfo &displayInformation);

   
   /*!
    * This is the current file we are operating on.
    */
   ossimFilename theFilename;
   
   /*!
    * This will be pre-computed when the header file is opened.  This is computed
    * after we have found the display order.  We will use the rectangle of the
    * smallest display number.
    */
   ossimDrect theImageRect;
   
   /*!
    * This will hold some pre-computed information about the display
    * order of the images.  It will be populated by scanning the
    * file and sorting display levels.
    */
   std::vector<ossimNitfDisplayInfo>             theDisplayInformationList;
   
   /*!
    * This will hold pre-computed offsets to the start of all image headers
    * and start to their data.
    */
   std::vector<ossimNitfImageOffsetInformation>  theImageOffsetList;

   /*!
    * This will hold pre-computed offsets to the start of all symbol headers
    * and start to their data.
    */
   std::vector<ossimNitfSymbolOffsetInformation> theSymbolOffsetList;

   /*!
    * This will hold pre-computed offsets to the start of all Label headers
    * and start to their data.
    */
   std::vector<ossimNitfLabelOffsetInformation>  theLabelOffsetList;

   /*!
    * This will hold pre-computed offsets to the start of all text headers
    * and start to their data.
    */
   std::vector<ossimNitfTextOffsetInformation>   theTextOffsetList;

   /*!
    * This will hold pre-coputed offsets to the start of all the data
    * extension segments.
    */
   std::vector<ossimNitfDataExtSegOffsetInformation> theDataExtSegOffsetList;



   
   // Note: these are work variables and not part of the
   // ossimNitf header.  These variables will be used to quickly
   // access various parts of the file.
   /*!
    * This is not part of the NITF file header.  This will be
    * computed since somethimes the header size will not be
    * given in the header field.  This value will hold a count of
    * all the bytes read for the header portion.  This can be used
    * to skip past the header data.
    */
   ossim_int32 theHeaderSize;
   
   /*!
    * FSCODE: This is a 40 byte Alphanumeric field
    */
   char theCodewords[41]; 

   /*!
    * FSCTLH: This is a 40 byte Alphanumeric field.
    */
   char theControlAndHandling[41]; 

   /*!
    * FSREL: This is a 40 byte Alphanumeric field.
    */
   char theReleasingInstructions[41]; 

   /*!
    * FSCAUT: This is a 20 byte alphanumeric field.
    */ 
   char theClassificationAuthority[21];

   /*!
    * FSCTLN: This is a 20 byte Alphanumeric field.
    */
   char theSecurityControlNumber[21]; 

   /*!
    * FSDWNG:
    *
    * This is a 6 byte Alphanumeric.  If this field
    * is not blank then it will have the form:
    *
    *   YYMMDD
    *
    *   Where YY is the last 2 chars of the year and will be
    *            00 to 59 represents 2000 to 2059
    *            60 to 99 represents 1960 to 1999
    *
    *
    *   if the field is 999999 then the Originating agency field
    *   is required. If te field is 999998 then the downgrade event
    *   is required
    */
   char theSecurityDowngrade[7];

   /**
    * FSDEVT:
    * This is a conditional field and is a 40 byte
    * Alphanumeric field.  This field exists if the
    * security downgrade is 999998.
    */
   char theDowngradingEvent[41];

   /**
    * ONAME:
    *
    * This is a 27 byte Alphanumeric number
    */
   char theOriginatorsName[28];

   /*!
    * OPHONE:
    * This is an 18 byte Alphanumeric value. 
    */
   char theOriginatorsPhone[19]; 

   /*!
    * FL:
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
    * HL:
    *
    * This is a reqired field.
    *
    * Will hold the header length. The values
    * range up to 999998 and a value of
    * 999999 indicates that the length of the
    * header was not available upon creation.
    */
   char theHeaderLength[7];

   /**
    * NUMI:
    *
    * This is a 3 byte field that specifies the number
    * of images in the file.  0-999
    */
   char theNumberOfImageInfoRecords[4];

   /*!
    * This information is conditional on the number of image
    * info records field.  If the field is greater than 0 then
    * the record information is read in up to the indicated number
    */
   std::vector<ossimNitfImageInfoRecordV2_0> theNitfImageInfoRecords;

   /*!
    * This is a 3 byte numeric 0-999.
    */

   char theNumberOfSymbolInfoRecords[4];
   /*!
    * This information is conditional on the number of symbol
    * info records field.  If the field is greater than 0 then
    * the record information is read in up to the indicated number
    */
   std::vector<ossimNitfSymbolInfoRecordV2_0> theNitfSymbolInfoRecords;

   /**
    * NUML
    *
    * This is a 3 byte numeric 0-999.
    */

   char theNumberOfLabelInfoRecords[4];
   
   /*!
    * This information is conditional on the number of label
    * info records field.  If the field is greater than 0 then
    * the record information is read in up to the indicated number
    */
   std::vector<ossimNitfLabelInfoRecordV2_0> theNitfLabelInfoRecords;

   /**
    * NUMT:
    *
    * This is a 3 byte numeric 0-999.
    */
   char theNumberOfTextFileInfoRecords[4];

   /*!
    * This information is conditional on the number of text file
    * info records field.  If the field is greater than 0 then
    * the record information is read in up to the indicated number
    */
   std::vector<ossimNitfTextInfoRecordV2_0> theNitfTextInfoRecords;

   /**
    * NUMDES:
    *
    * This is a 3 byte numeric 0-999.
    */
   char theNumberOfDataExtSegInfoRecords[4];

   /*!
    * This information is conditional on the number of 
    * info records field.  If the field is greater than 0 then
    * the record information is read in up to the indicated number
    */
   std::vector<ossimNitfDataExtSegInfoRecordV2_0> theNitfDataExtSegInfoRecords;

   /**
    * NUMRES:
    *
    * This is a 3 byte numeric 0-999. Reserved Extension Segments
    */
   char theNumberOfResExtSegInfoRecords[4];

   /*!
    * This information is conditional on the number of Res Ext Seg
    * info records field.  If the field is greater than 0 then
    * the record information is read in up to the indicated number
    */
   std::vector<ossimNitfResExtSegInfoRecordV2_0> theNitfResExtSegInfoRecords;

   /**
    * UDHDL:
    * 
    * This is a required field and is a 5 byte numeric ranging
    * from 0-99999.  This is 0 if there is no data.
    */
   char theUserDefinedHeaderDataLength[6];

   /*!
    * This is a conditional field.  It will exist if the
    * user defined header length is not 0.  If it exists
    * then it may contain an overflow into one of the data
    * extension segments above and will hold the number of that
    * segment.
    */
   char theUserDefinedHeaderOverflow[4];

   /**
    * XHDL:
    *
    * This is a required field.  It is 5 bytes long and
    * ranges from 0-99999.  0 means there is no data
    */
   char theExtendedHeaderDataLength[6];

   /*!
    * This is a required field.  It is 5 bytes long and
    * ranges from 0-99999.  0 means there is no data
    */
   char theExtendedHeaderOverflow[4];
};

#endif
