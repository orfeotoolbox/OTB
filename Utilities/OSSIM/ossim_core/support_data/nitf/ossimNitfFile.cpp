//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
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
// $Id: ossimNitfFile.cpp,v 1.7 2005/08/19 18:59:45 gpotts Exp $
#include <support_data/nitf/ossimNitfFile.h>
#include <support_data/nitf/ossimNitfFileHeader.h>
#include <support_data/nitf/ossimNitfImageHeader.h>
#include <support_data/nitf/ossimNitfFileHeaderV2_0.h>
#include <support_data/nitf/ossimNitfFileHeaderV2_1.h>
#include <support_data/nitf/ossimNitfImageHeaderV2_0.h>
#include <support_data/nitf/ossimNitfImageHeaderV2_1.h>
#include <support_data/nitf/ossimNitfTagFactoryRegistry.h>
#include <support_data/nitf/ossimNitfRegisteredTag.h>
#include <base/common/ossimTrace.h>
#include <base/data_types/ossimFilename.h>
#include <base/context/ossimNotifyContext.h>

#include <fstream>
using namespace std;

#ifndef NULL
#include <stddef.h>
#endif

//***
// Static trace for debugging
//***
static ossimTrace traceDebug("ossimNitfFile:debug");

std::ostream& operator <<(std::ostream& out, const ossimNitfFile& data)
{
   return data.print(out);
}

std::ostream& ossimNitfFile::print(std::ostream& out) const
{
   if(theNitfFileHeader)
   {
      out << *(theNitfFileHeader)<<endl;

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
   if(theNitfFileHeader)
   {
      delete theNitfFileHeader;
      theNitfFileHeader = NULL;
   }
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
   ifstream in(file.c_str(), ios::in|ios::binary);
   if (in.fail())
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimNitfFile::parseFile: "
                                             << "Could not open file:  " << file.c_str()
                                             << "\nReturning..." << std::endl;
      }
      return false;
   }
      
   if(theNitfFileHeader)
   {
      delete theNitfFileHeader;
      theNitfFileHeader = NULL;
   }

   char temp[10];
   in.read(temp, 9);
   in.seekg(0, ios::beg);
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

   if(theNitfFileHeader)
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

const ossimNitfFileHeader* ossimNitfFile::getHeader()const
{
   return theNitfFileHeader;
}

ossimIrect ossimNitfFile::getImageRect()const
{
   if(theNitfFileHeader)
   {
      return theNitfFileHeader->getImageRect();
   }

   return ossimIrect(ossimIpt(0,0), ossimIpt(0,0));
}

ossimNitfImageHeader* ossimNitfFile::getNewImageHeader(long imageNumber)const
{
   if(theNitfFileHeader)
   {
      ifstream in(theFilename.c_str(), ios::in|ios::binary);

      return theNitfFileHeader->getNewImageHeader(imageNumber,
                                                  in);
      in.close();
   }
   
   return NULL;
}

ossimNitfSymbolHeader* ossimNitfFile::getNewSymbolHeader(long symbolNumber)const
{
   if(theNitfFileHeader)
   {
      ifstream in(theFilename.c_str(), ios::in|ios::binary);

      return theNitfFileHeader->getNewSymbolHeader(symbolNumber,
                                                   in);
      in.close();
   }
   
   return NULL;
}

ossimNitfLabelHeader* ossimNitfFile::getNewLabelHeader(long labelNumber)const
{
   if(theNitfFileHeader)
   {
      ifstream in(theFilename.c_str(), ios::in|ios::binary);

      return theNitfFileHeader->getNewLabelHeader(labelNumber,
                                                  in);
      in.close();
   }
   
   return NULL;
}

ossimNitfTextHeader* ossimNitfFile::getNewTextHeader(long textNumber)const
{
   if(theNitfFileHeader)
   {
      ifstream in(theFilename.c_str(), ios::in|ios::binary);

      return theNitfFileHeader->getNewTextHeader(textNumber,
                                                 in);
      in.close();
   }
   
   return NULL;
}

ossimNitfDataExtensionSegment* ossimNitfFile::getNewDataExtensionSegment(long dataExtNumber)const
{
   if(theNitfFileHeader)
   {
      ifstream in(theFilename.c_str(), ios::in|ios::binary);

      return theNitfFileHeader->getNewDataExtensionSegment(dataExtNumber,
                                                           in);
      in.close();
   }
   
   return NULL;
}

ossimString ossimNitfFile::getVersion()const
{
   if(theNitfFileHeader)
   {
      return ossimString(theNitfFileHeader->getVersion());
   }
   
   return ossimString("");
}

