//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
// 
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfFile.cpp 9094 2006-06-13 19:12:40Z dburken $

#include <fstream>
#include <iostream>
#include <iomanip>

#include <ossim/support_data/ossimNitfFile.h>
#include <ossim/support_data/ossimNitfFileHeader.h>
#include <ossim/support_data/ossimNitfImageHeader.h>
#include <ossim/support_data/ossimNitfFileHeaderV2_0.h>
#include <ossim/support_data/ossimNitfFileHeaderV2_1.h>
#include <ossim/support_data/ossimNitfImageHeaderV2_0.h>
#include <ossim/support_data/ossimNitfImageHeaderV2_1.h>
#include <ossim/support_data/ossimNitfTagFactoryRegistry.h>
#include <ossim/support_data/ossimNitfRegisteredTag.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimNotifyContext.h>



// Static trace for debugging
static ossimTrace traceDebug("ossimNitfFile:debug");

std::ostream& operator <<(std::ostream& out, const ossimNitfFile& data)
{
   return data.print(out);
}

std::ostream& ossimNitfFile::print(std::ostream& out) const
{
   out << "ossimNitfFile::print"
       << setiosflags(std::ios::left) << std::setw(24)
       << "\ntheFilename:" << theFilename << std::endl;
   
   if(theNitfFileHeader.valid())
   {
      out << *(theNitfFileHeader.get()) << std::endl;

      int n = 0;
      n = theNitfFileHeader->getNumberOfImages();
      int idx = 0;

      for(idx = 0; idx < n;++idx)
      {
         ossimNitfImageHeader* ih = getNewImageHeader(idx);
         out << "IMAGE -----> " << idx << std::endl;
         if(ih)
         {
            out << *ih << std::endl;
            delete ih;
            ih = 0;
         }
      }
   }

   return out;
}

ossimNitfFile::ossimNitfFile()
   : theFilename(""),
     theNitfFileHeader(NULL)
{
}

ossimNitfFile::~ossimNitfFile()
{
   theNitfFileHeader = NULL;
}

bool ossimNitfFile::parseFile(const ossimFilename& file)
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "DEBUG ossimNitfFile::parseFile: "
         << "endtered......"
         << std::endl;
   }
   std::ifstream in(file.c_str(), std::ios::in|std::ios::binary);
   if (in.fail())
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "DEBUG ossimNitfFile::parseFile: "
            << "Could not open file:  " << file.c_str()
            << "\nReturning..." << std::endl;
      }
      return false;
   }
      
   if(theNitfFileHeader.valid())
   {
      theNitfFileHeader = NULL;
   }

   char temp[10];
   in.read(temp, 9);
   in.seekg(0, std::ios::beg);
   temp[9] ='\0';
   
   theFilename = file;

   ossimString s(temp);
   if(s == "NITF02.00")
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "DEBUG: NITF Version 2.0"
            << std::endl;
      }
      theNitfFileHeader = new ossimNitfFileHeaderV2_0;
   }
   else if ( (s == "NITF02.10") || (s == "NSIF01.00") )
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "DEBUG: NITF Version 2.1"
            << std::endl;
      }
      theNitfFileHeader = new ossimNitfFileHeaderV2_1;
   }
   else
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "DEBUG ossimNitfFile::parseFile: "
            << "Not an NITF file!"
            << std::endl;
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "DEBUG ossimNitfFile::parseFile: returning...........false"
            << "endtered......"
            << std::endl;
      }
      return false;
   }

   if(theNitfFileHeader.valid())
   {
      theNitfFileHeader->parseStream(in);
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "DEBUG ossimNitfFile::parseFile: returning...........true"
         << std::endl;
   }
   return true;
}

const ossimRefPtr<ossimNitfFileHeader> ossimNitfFile::getHeader() const
{
   return theNitfFileHeader;
}

ossimIrect ossimNitfFile::getImageRect()const
{
   if(theNitfFileHeader.valid())
   {
      return theNitfFileHeader->getImageRect();
   }

   return ossimIrect(ossimIpt(0,0), ossimIpt(0,0));
}

ossimNitfImageHeader* ossimNitfFile::getNewImageHeader(long imageNumber)const
{
   if(theNitfFileHeader.valid())
   {
      std::ifstream in(theFilename.c_str(), std::ios::in|std::ios::binary);

      return theNitfFileHeader->getNewImageHeader(imageNumber,
                                                  in);
      in.close();
   }
   
   return NULL;
}

ossimNitfSymbolHeader* ossimNitfFile::getNewSymbolHeader(long symbolNumber)const
{
   if(theNitfFileHeader.valid())
   {
      std::ifstream in(theFilename.c_str(), std::ios::in|std::ios::binary);

      return theNitfFileHeader->getNewSymbolHeader(symbolNumber,
                                                   in);
      in.close();
   }
   
   return NULL;
}

ossimNitfLabelHeader* ossimNitfFile::getNewLabelHeader(long labelNumber)const
{
   if(theNitfFileHeader.valid())
   {
      std::ifstream in(theFilename.c_str(), std::ios::in|std::ios::binary);

      return theNitfFileHeader->getNewLabelHeader(labelNumber,
                                                  in);
      in.close();
   }
   
   return NULL;
}

ossimNitfTextHeader* ossimNitfFile::getNewTextHeader(long textNumber)const
{
   if(theNitfFileHeader.valid())
   {
      std::ifstream in(theFilename.c_str(), std::ios::in|std::ios::binary);

      return theNitfFileHeader->getNewTextHeader(textNumber,
                                                 in);
      in.close();
   }
   
   return NULL;
}

ossimNitfDataExtensionSegment* ossimNitfFile::getNewDataExtensionSegment(long dataExtNumber)const
{
   if(theNitfFileHeader.valid())
   {
      std::ifstream in(theFilename.c_str(), std::ios::in|std::ios::binary);

      return theNitfFileHeader->getNewDataExtensionSegment(dataExtNumber,
                                                           in);
      in.close();
   }
   
   return NULL;
}

ossimString ossimNitfFile::getVersion()const
{
   if(theNitfFileHeader.valid())
   {
      return ossimString(theNitfFileHeader->getVersion());
   }
   
   return ossimString("");
}

