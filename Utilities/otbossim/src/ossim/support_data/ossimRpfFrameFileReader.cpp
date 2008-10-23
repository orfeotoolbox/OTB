//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// LICENSE:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
// Description: Rpf support class
// 
//********************************************************************
// $Id: ossimRpfFrameFileReader.cpp 9094 2006-06-13 19:12:40Z dburken $
#include <ossim/support_data/ossimRpfFrameFileReader.h>
#include <ossim/support_data/ossimRpfHeader.h>
#include <ossim/support_data/ossimRpfImageDescriptionSubheader.h>
#include <ossim/support_data/ossimRpfMaskSubsection.h>
#include <ossim/support_data/ossimRpfCompressionSectionSubheader.h>
#include <ossim/support_data/ossimRpfAttributeSectionSubheader.h>
#include <ossim/support_data/ossimNitfFile.h>
#include <ossim/support_data/ossimNitfTagInformation.h>
#include <ossim/support_data/ossimNitfFileHeader.h>
#include <ossim/base/ossimErrorCodes.h>

#ifndef NULL
#include <stddef.h>
#endif
ostream& operator<<(ostream& out,
                    const ossimRpfFrameFileReader& data)
{
   data.print(out);

   return out;
}


ossimRpfFrameFileReader::ossimRpfFrameFileReader()
   :theRpfHeader(NULL),
    theFilename("")
{
}

ossimRpfFrameFileReader::~ossimRpfFrameFileReader()
{
   if(theRpfHeader)
   {
      delete theRpfHeader;
      theRpfHeader = NULL;
   }
}


ossimErrorCode ossimRpfFrameFileReader::parseFile(const ossimFilename& fileName)
{
   ifstream in(fileName, ios::in | ios::binary);
   ossimNitfFile *nitfFile = new ossimNitfFile;

   if(!in)
   {
      return ossimErrorCodes::OSSIM_ERROR;
   }
   clearAll();
   nitfFile->parseFile(fileName);
   const ossimRefPtr<ossimNitfFileHeader> nitfFileHeader =
      nitfFile->getHeader();
   if(!nitfFileHeader)
   {
      delete nitfFile;
      nitfFile = NULL;
      
      return ossimErrorCodes::OSSIM_ERROR;
   }
   if(theRpfHeader)
   {
      delete theRpfHeader;
      theRpfHeader = NULL;
   }
   ossimNitfTagInformation info; 
   nitfFileHeader->getTag(info, "RPFHDR");
   // we no longer need access to the nitf header.  We got what we needed
   delete nitfFile;
   nitfFile = NULL;
   theFilename = fileName;
   if(info.getTagName() == "RPFHDR")
   {      
      theRpfHeader = new ossimRpfHeader;
      
      // set the get pointer for the stream to the start
      // of the Rpf header data
      in.seekg(info.getTagDataOffset());
      
      // now get the header data.  We do not need to pass in the byte order.
      // this is grabbed from the first byte of the stream.  To see this,
      // Look at the RpfHeader implementation.
      if(theRpfHeader->parseStream(in)==ossimErrorCodes::OSSIM_OK)
      {
      }
      else
      {
         delete theRpfHeader;
         theRpfHeader = NULL;
      }
   }
   else
   {
      return ossimErrorCodes::OSSIM_ERROR;
   }
   
   if(theRpfHeader)
   {
   }
   else
   {
      return ossimErrorCodes::OSSIM_ERROR;
   }
   
   return ossimErrorCodes::OSSIM_OK;
}

void ossimRpfFrameFileReader::print(ostream& out)const
{
   if(theRpfHeader)
   {
      out << *theRpfHeader << endl;
   }
}

void ossimRpfFrameFileReader::clearAll()
{
   theFilename = "";
}
