//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// LICENSE: LGPL
//
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfFileHeader.h,v 1.11 2005/08/19 18:59:45 gpotts Exp $
#ifndef ossimNitfFileHeader_HEADER
#define ossimNitfFileHeader_HEADER
#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <base/data_types/ossimDrect.h>
#include <base/data_types/ossimFilename.h>
#include <base/common/ossimObject.h>
#include <base/common/ossimPropertyInterface.h>
#include <base/property/ossimProperty.h>
#include "ossimNitfTagInformation.h"

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

   ossimNitfImageOffsetInformation(long headerOffset,
                                   long dataOffset)
      :theImageHeaderOffset(headerOffset),
       theImageDataOffset(dataOffset)
      {
      }
   
   long theImageHeaderOffset;
   long theImageDataOffset;
};


class OSSIMDLLEXPORT ossimNitfSymbolOffsetInformation : public ossimObject
{
public:
   ossimNitfSymbolOffsetInformation(long headerOffset,
                                    long dataOffset)
      :theSymbolHeaderOffset(headerOffset),
       theSymbolDataOffset(dataOffset)
      {
      }
   
   long theSymbolHeaderOffset;
   long theSymbolDataOffset;
};

class OSSIMDLLEXPORT ossimNitfLabelOffsetInformation : public ossimObject
{
public:
   ossimNitfLabelOffsetInformation(long headerOffset,
                                   long dataOffset)
      :theLabelHeaderOffset(headerOffset),
       theLabelDataOffset(dataOffset)
      {
      }
   
   long theLabelHeaderOffset;
   long theLabelDataOffset;
};

class OSSIMDLLEXPORT ossimNitfTextOffsetInformation : public ossimObject
{
public:
   ossimNitfTextOffsetInformation(long headerOffset,
                                  long dataOffset)
      :theTextHeaderOffset(headerOffset),
       theTextDataOffset(dataOffset)
      {
      }
   
   long theTextHeaderOffset;
   long theTextDataOffset;
   
};

class OSSIMDLLEXPORT ossimNitfDataExtSegOffsetInformation : public ossimObject
{
public:
   ossimNitfDataExtSegOffsetInformation(long headerOffset,
                                        long dataOffset)
      :theDataExtSegHeaderOffset(headerOffset),
       theDataExtSegDataOffset(dataOffset)
      {
      }
   
   long theDataExtSegHeaderOffset;
   long theDataExtSegDataOffset;   
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
   virtual long getNumberOfImages()const=0;
   virtual long getNumberOfLabels()const=0;
   virtual long getNumberOfSymbols()const=0;
   virtual long getNumberOfGraphics()const=0;
   virtual long getNumberOfTextSegments()const=0;
   virtual long getNumberOfDataExtSegments()const=0;
   virtual long getHeaderSize()const=0;
   
   virtual bool  getTagInformation(ossimNitfTagInformation& tag,
                                   int idx)const;

   virtual int getNumberOfTags()const;

   virtual bool getTag(ossimNitfTagInformation& tagInfo,
                       const ossimString& tagName)const;
   
   virtual long getFileSize()const=0;
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
   virtual ossimNitfImageHeader* getNewImageHeader(long imageNumber,
                                                   std::istream& in)const=0;
   virtual ossimNitfSymbolHeader* getNewSymbolHeader(long symbolNumber,
                                                    std::istream& in)const=0;
   virtual ossimNitfLabelHeader* getNewLabelHeader(long labelNumber,
                                                   std::istream& in)const=0;
   virtual ossimNitfTextHeader* getNewTextHeader(long textNumber,
                                                std::istream& in)const=0;
   virtual ossimNitfDataExtensionSegment* getNewDataExtensionSegment(long dataExtNumber,
                                                                     std::istream& in)const=0;
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
