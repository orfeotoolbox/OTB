
//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// LICENSE: LGPL
//
// Author: Garrett Potts
//
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfFileHeader.h 10552 2007-02-28 20:35:07Z gpotts $
#ifndef ossimNitfFileHeader_HEADER
#define ossimNitfFileHeader_HEADER

#include <iosfwd>
#include <vector>
#include <iterator>
#include <ossim/base/ossimDrect.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimObject.h>
#include <ossim/base/ossimPropertyInterface.h>
#include <ossim/base/ossimProperty.h>
#include <ossim/support_data/ossimNitfTagInformation.h>

class ossimNitfImageHeader;
class ossimNitfSymbolHeader;
class ossimNitfTextHeader;
class ossimNitfLabelHeader;
class ossimNitfRegisteredTag;
class ossimNitfDataExtensionSegment;
class ossimNitfRegisteredTag;
// These structures are just data holders and will
// not be used directly by outside users.  This is for internal use
//
class OSSIM_DLL ossimNitfImageOffsetInformation : public ossimObject
{
public:
   virtual std::ostream& print(std::ostream& out) const;

   ossimNitfImageOffsetInformation(ossim_uint64 headerOffset,
                                   ossim_uint64 dataOffset)
      :theImageHeaderOffset(headerOffset),
       theImageDataOffset(dataOffset)
      {
      }
   
   ossim_uint64 theImageHeaderOffset;
   ossim_uint64 theImageDataOffset;
};


class OSSIMDLLEXPORT ossimNitfSymbolOffsetInformation : public ossimObject
{
public:
   ossimNitfSymbolOffsetInformation(ossim_uint64 headerOffset,
                                    ossim_uint64 dataOffset)
      :theSymbolHeaderOffset(headerOffset),
       theSymbolDataOffset(dataOffset)
      {
      }
   
   ossim_uint64 theSymbolHeaderOffset;
   ossim_uint64 theSymbolDataOffset;
};

class OSSIMDLLEXPORT ossimNitfLabelOffsetInformation : public ossimObject
{
public:
   ossimNitfLabelOffsetInformation(ossim_uint64 headerOffset,
                                   ossim_uint64 dataOffset)
      :theLabelHeaderOffset(headerOffset),
       theLabelDataOffset(dataOffset)
      {
      }
   
   ossim_uint64 theLabelHeaderOffset;
   ossim_uint64 theLabelDataOffset;
};

class OSSIMDLLEXPORT ossimNitfTextOffsetInformation : public ossimObject
{
public:
   ossimNitfTextOffsetInformation(ossim_uint64 headerOffset,
                                  ossim_uint64 dataOffset)
      :theTextHeaderOffset(headerOffset),
       theTextDataOffset(dataOffset)
      {
      }
   
   ossim_uint64 theTextHeaderOffset;
   ossim_uint64 theTextDataOffset;
   
};

class OSSIMDLLEXPORT ossimNitfDataExtSegOffsetInformation : public ossimObject
{
public:
   ossimNitfDataExtSegOffsetInformation(ossim_uint64 headerOffset,
                                        ossim_uint64 dataOffset)
      :theDataExtSegHeaderOffset(headerOffset),
       theDataExtSegDataOffset(dataOffset)
      {
      }
   
   ossim_uint64 theDataExtSegHeaderOffset;
   ossim_uint64 theDataExtSegDataOffset;   
};

class OSSIM_DLL ossimNitfFileHeader : public ossimObject,
                                      public ossimPropertyInterface
{
public:
   friend class ossimNitfFile;

   ossimNitfFileHeader();
   virtual ~ossimNitfFileHeader();

   virtual void parseStream(std::istream &in)= 0;
   virtual void writeStream(std::ostream &out)=0;
   
   virtual bool isEncrypted()const=0;
   virtual ossim_int32 getNumberOfImages()const=0;
   virtual ossim_int32 getNumberOfLabels()const=0;
   virtual ossim_int32 getNumberOfSymbols()const=0;
   virtual ossim_int32 getNumberOfGraphics()const=0;
   virtual ossim_int32 getNumberOfTextSegments()const=0;
   virtual ossim_int32 getNumberOfDataExtSegments()const=0;
   virtual ossim_int32 getHeaderSize()const=0;
   
   virtual bool  getTagInformation(ossimNitfTagInformation& tag,
                                   int idx)const;

   virtual int getNumberOfTags()const;

   virtual bool getTag(ossimNitfTagInformation& tagInfo,
                       const ossimString& tagName)const;
   
   virtual ossim_int32 getFileSize()const=0;
   virtual const char* getVersion()const=0;
   virtual const char* getDateTime()const=0;
   virtual ossimDrect getImageRect()const=0;
   virtual ossimString getSecurityClassification()const=0;
   
   bool hasImages()const;
   bool hasSymbols()const;
   bool hasGraphics()const;
   bool hasText()const;
   bool hasLabels()const;
   bool hasDataExtSegments()const;
   virtual ossimNitfImageHeader* getNewImageHeader(ossim_int32 imageNumber,
                                                   std::istream& in)const=0;
   virtual ossimNitfSymbolHeader* getNewSymbolHeader(ossim_int32 symbolNumber,
                                                    std::istream& in)const=0;
   virtual ossimNitfLabelHeader* getNewLabelHeader(ossim_int32 labelNumber,
                                                   std::istream& in)const=0;
   virtual ossimNitfTextHeader* getNewTextHeader(ossim_int32 textNumber,
                                                std::istream& in)const=0;
   virtual ossimNitfDataExtensionSegment* getNewDataExtensionSegment(
      ossim_int32 dataExtNumber, std::istream& in)const=0;
   virtual ossimNitfImageHeader*    allocateImageHeader()const=0;
   virtual ossimNitfSymbolHeader*   allocateSymbolHeader()const=0;
   virtual ossimNitfLabelHeader*    allocateLabelHeader()const=0;
   virtual ossimNitfTextHeader*     allocateTextHeader()const=0;
   virtual ossimNitfDataExtensionSegment *allocateDataExtSegment()const=0;

   virtual ossim_uint32 getTotalTagLength()const;


   virtual void setProperty(ossimRefPtr<ossimProperty> property);
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;

   
protected:
   std::vector<ossimNitfTagInformation> theTagList;
   
TYPE_DATA
};

#endif
