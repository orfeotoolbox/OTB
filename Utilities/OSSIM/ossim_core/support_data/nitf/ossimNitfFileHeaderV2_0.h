//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License 
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software 
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfFileHeaderV2_0.h,v 1.7 2005/08/19 18:59:45 gpotts Exp $
#ifndef ossimNitfFileHeaderV2_0_HEADER
#define ossimNitfFileHeaderV2_0_HEADER
#include "ossimNitfFileHeader.h"
#include "base/data_types/ossimString.h"
#include "ossimNitfTagInformation.h"

class ossimNitfRegisteredTag;

class ossimNitfImageInfoRecordV2_0
{
public:
   friend ostream& operator <<(ostream& out,
                               const ossimNitfImageInfoRecordV2_0 &data);

   long getHeaderLength()const{return ossimString(theImageSubheaderLength).toLong();}
   long getImageLength()const{return ossimString(theImageLength).toLong();}
   long getTotalLength()const{return (getHeaderLength() + getImageLength()); }
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
   friend ostream& operator <<(ostream& out,
                               const ossimNitfSymbolInfoRecordV2_0 &data);

   long getHeaderLength()const{return ossimString(theSymbolSubheaderLength).toLong();}
   long getImageLength()const{return ossimString(theSymbolLength).toLong();}
   long getTotalLength()const{return (getHeaderLength() + getImageLength()); }
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
   friend ostream& operator <<(ostream& out,
                               const ossimNitfLabelInfoRecordV2_0 &data);

   long getHeaderLength()const{return ossimString(theLabelSubheaderLength).toLong();}
   long getImageLength()const{return ossimString(theLabelLength).toLong();}
   long getTotalLength()const{return (getHeaderLength() + getImageLength()); }
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
   friend ostream& operator <<(ostream& out,
                               const ossimNitfTextInfoRecordV2_0 &data);

   long getHeaderLength()const{return ossimString(theTextSubheaderLength).toLong();}
   long getImageLength()const{return ossimString(theTextLength).toLong();}
   long getTotalLength()const{return (getHeaderLength() + getImageLength()); }
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
   friend ostream& operator <<(ostream& out,
                               const ossimNitfDataExtSegInfoRecordV2_0 &data);

   long getHeaderLength()const{return ossimString(theDataExtSegSubheaderLength).toLong();}
   long getImageLength()const{return ossimString(theDataExtSegLength).toLong();}
   long getTotalLength()const{return (getHeaderLength() + getImageLength()); }
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
   friend ostream& operator <<(ostream& out,
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

class OSSIMDLLEXPORT ossimNitfFileHeaderV2_0 : public ossimNitfFileHeader
{
public:
   
   ossimNitfFileHeaderV2_0();
   virtual ~ossimNitfFileHeaderV2_0();
   virtual void parseStream(std::istream &in);
   virtual void writeStream(std::ostream &out);
   virtual std::ostream& print(std::ostream& out)const;
   virtual bool isEncrypted()const;
   virtual long getNumberOfImages()const;
   virtual long getNumberOfLabels()const;
   virtual long getNumberOfSymbols()const;
   virtual long getNumberOfGraphics()const;
   virtual long getNumberOfTextSegments()const;
   virtual long getNumberOfDataExtSegments()const;
   virtual long getHeaderSize()const;
   virtual long getFileSize()const;
   virtual ossimString getSecurityClassification()const;
   
   virtual const char* getDateTime()const;
   virtual const char* getVersion()const;

   virtual ossimDrect getImageRect()const;

   virtual ossimNitfImageHeader*  getNewImageHeader(long imageNumber,
                                                    std::istream& in)const;
   virtual ossimNitfSymbolHeader* getNewSymbolHeader(long symbolNumber,
                                                     std::istream& in)const;
   virtual ossimNitfLabelHeader* getNewLabelHeader(long labelNumber,
                                                   std::istream& in)const;
   virtual ossimNitfTextHeader*   getNewTextHeader(long textNumber,
                                                   std::istream& in)const;
   virtual ossimNitfDataExtensionSegment* getNewDataExtensionSegment(long dataExtNumber,
                                                                     std::istream& in)const;
   virtual ossimNitfImageHeader*  allocateImageHeader()const;
   virtual ossimNitfSymbolHeader* allocateSymbolHeader()const;
   virtual ossimNitfLabelHeader*  allocateLabelHeader()const;
   virtual ossimNitfTextHeader*   allocateTextHeader()const;
   virtual ossimNitfDataExtensionSegment *allocateDataExtSegment()const;

TYPE_DATA
private:

   struct ossimNitfDisplayInfo
   {
   public:
      ossimNitfDisplayInfo(const ossimString &filePartType,
                           long displayLevel,
                           long indexIntoList)
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
      long        theDisplayLevel;

      /*!
       * This is an index into the array that has all the offsets
       * pre-computed.
       */
      long        theIndex;
      
   };

   void clearFields();
   
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
   vector<ossimNitfDisplayInfo>             theDisplayInformationList;
   
   /*!
    * This will hold pre-computed offsets to the start of all image headers
    * and start to their data.
    */
   vector<ossimNitfImageOffsetInformation>  theImageOffsetList;

   /*!
    * This will hold pre-computed offsets to the start of all symbol headers
    * and start to their data.
    */
   vector<ossimNitfSymbolOffsetInformation> theSymbolOffsetList;

   /*!
    * This will hold pre-computed offsets to the start of all Label headers
    * and start to their data.
    */
   vector<ossimNitfLabelOffsetInformation>  theLabelOffsetList;

   /*!
    * This will hold pre-computed offsets to the start of all text headers
    * and start to their data.
    */
   vector<ossimNitfTextOffsetInformation>   theTextOffsetList;

   /*!
    * This will hold pre-coputed offsets to the start of all the data
    * extension segments.
    */
   vector<ossimNitfDataExtSegOffsetInformation> theDataExtSegOffsetList;



   
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
   long theHeaderSize;
   
   /*!
    * Required field for NITF and is a 9 byte value
    *
    * format is NITFNN.NN where NN.NN is the version
    * number. The valid version number for this class
    * is NITF02.00
    */
   char  theFileTypeVersion[10]; // nine bytes

   /*!
    * Required field.  Value ranges from 1-99
    */
   char  theComplianceLevel[3];  // 2 bytes

   /*!
    * Not required.  Reserved for future use.
    */
   char  theSystemType[5];      // four bytes

   /*!
    * Reuqired field for NITF.  Is a 10 byte
    * Alphanumeric
    */
   char  theOriginatingStationId[11]; // ten bytes

   /*!
    * Has the form DDHHMMSSZMONYY and is a required field
    *
    * where DD is the Day
    *       HH is the hour
    *       MM is the minute
    *       SS is the seconds
    *       Z  is just a required character
    *       MON is first 3 chars of the month
    *       YY  is the year
    */
   char  theDateTime[15];  // 14 bytes

   /*!
    * Is a required field and is an 80 byte
    * Alphanumeric value
    */
   char  theFileTitle[81]; // 80 bytes

   /*!
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

   /*!
    * This is a 40 byte Alphanumeric field
    */
   char theCodewords[41]; 

   /*!
    * This is a 40 byte Alphanumeric field.
    */
   char theControlAndHandling[41]; 

   /*!
    * This is a 40 byte Alphanumeric field.
    */
   char theReleasingInstructions[41]; 

   /*!
    * This is a 20 byte alphanumeric field.
    */ 
   char theClassificationAuthority[21];

   /*!
    * This is a 20 byte Alphanumeric field.
    */
   char theSecurityControlNumber[21]; 

   /*!
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

   /*!
    * This is a conditional field and is a 40 byte
    * Alphanumeric field.  This field exists if the
    * security downgrade is 999998.
    */
   char theDowngradingEvent[41];

   /*!
    * This is the copy number and is a 5 byte
    * numeric of the form 0-99999
    */
   char theMessageCopyNumber[6];

   /*!
    * This is the total number of copies that exist.
    * This is a 5 byte numeric of the form 0-99999.
    */
   char theNumberOfCopies[6];

   /*!
    * This is a reqired field and is a 1 byte numeric
    * where 0 is not encrypted and 1 is encrypted.
    */
   char theEncryption[2];

   /*!
    * This is a 27 byte Alphanumeric number
    */
   char theOriginatorsName[28];

   /*!
    * This is an 18 byte Alphanumeric value. 
    */
   char theOriginatorsPhone[19]; 

   /*!
    * This is a required field.
    *
    * Is a 12 byte number of the form
    * 000000000388-999999999998 indicates the
    * file length and a value of 999999999999
    * indicates that the file length was not
    * available.
    */
   char theNitfFileLength[13];

   /*!
    * This is a reqired field.
    *
    * Will hold the header length. The values
    * range up to 999998 and a value of
    * 999999 indicates that the length of the
    * header was not available upon creation.
    */
   char theNitfHeaderLength[7];

   /*!
    * This is a 3 byte field that specifies the number
    * of images in the file.  0-999
    */
   char theNumberOfImageInfoRecords[4];

   /*!
    * This information is conditional on the number of image
    * info records field.  If the field is greater than 0 then
    * the record information is read in up to the indicated number
    */
   vector<ossimNitfImageInfoRecordV2_0> theNitfImageInfoRecords;

   /*!
    * This is a 3 byte numeric 0-999.
    */

   char theNumberOfSymbolInfoRecords[4];
   /*!
    * This information is conditional on the number of symbol
    * info records field.  If the field is greater than 0 then
    * the record information is read in up to the indicated number
    */
   vector<ossimNitfSymbolInfoRecordV2_0> theNitfSymbolInfoRecords;

   /*!
    * This is a 3 byte numeric 0-999.
    */

   char theNumberOfLabelInfoRecords[4];
   
   /*!
    * This information is conditional on the number of label
    * info records field.  If the field is greater than 0 then
    * the record information is read in up to the indicated number
    */
   vector<ossimNitfLabelInfoRecordV2_0> theNitfLabelInfoRecords;

   /*!
    * This is a 3 byte numeric 0-999.
    */
   char theNumberOfTextFileInfoRecords[4];

   /*!
    * This information is conditional on the number of text file
    * info records field.  If the field is greater than 0 then
    * the record information is read in up to the indicated number
    */
   vector<ossimNitfTextInfoRecordV2_0> theNitfTextInfoRecords;

   /*!
    * This is a 3 byte numeric 0-999.
    */
   char theNumberOfDataExtSegInfoRecords[4];

   /*!
    * This information is conditional on the number of 
    * info records field.  If the field is greater than 0 then
    * the record information is read in up to the indicated number
    */
   vector<ossimNitfDataExtSegInfoRecordV2_0> theNitfDataExtSegInfoRecords;

   /*!
    * This is a 3 byte numeric 0-999. Reserved Extension Segments
    */
   char theNumberOfResExtSegInfoRecords[4];

   /*!
    * This information is conditional on the number of Res Ext Seg
    * info records field.  If the field is greater than 0 then
    * the record information is read in up to the indicated number
    */
   vector<ossimNitfResExtSegInfoRecordV2_0> theNitfResExtSegInfoRecords;

   /*!
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

   /*!
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
