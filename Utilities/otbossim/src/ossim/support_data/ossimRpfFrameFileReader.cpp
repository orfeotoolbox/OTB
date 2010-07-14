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
// $Id: ossimRpfFrameFileReader.cpp 16997 2010-04-12 18:53:48Z dburken $

#include <fstream>
#include <ostream>

#include <ossim/support_data/ossimRpfFrameFileReader.h>
#include <ossim/support_data/ossimRpfHeader.h>
#include <ossim/support_data/ossimNitfFile.h>
#include <ossim/support_data/ossimNitfTagInformation.h>
#include <ossim/support_data/ossimNitfFileHeader.h>

std::ostream& operator<<(std::ostream& out, const ossimRpfFrameFileReader& data)
{
   return data.print(out);
}

ossimRpfFrameFileReader::ossimRpfFrameFileReader()
   :theRpfHeader(0),
    theFilename("")
{
}

ossimRpfFrameFileReader::~ossimRpfFrameFileReader()
{
   theRpfHeader = 0;
}

ossimErrorCode ossimRpfFrameFileReader::parseFile(const ossimFilename& fileName)
{
   std::ifstream in(fileName, ios::in | ios::binary);
   ossimRefPtr<ossimNitfFile> nitfFile = new ossimNitfFile;

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
      nitfFile = 0;
      return ossimErrorCodes::OSSIM_ERROR;
   }

   ossimNitfTagInformation info; 
   nitfFileHeader->getTag(info, "RPFHDR");
   // we no longer need access to the nitf header.  We got what we needed
   nitfFile = 0;
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
      theRpfHeader->parseStream(in); // ==ossimErrorCodes::OSSIM_OK)
      if ( in.fail() )
      {
         theRpfHeader = 0;
      }
   }
   else
   {
      return ossimErrorCodes::OSSIM_ERROR;
   }
   
   if( !theRpfHeader.valid() )
   {
      return ossimErrorCodes::OSSIM_ERROR;
   }
   
   return ossimErrorCodes::OSSIM_OK;
}

std::ostream& ossimRpfFrameFileReader::print(std::ostream& out)const
{
   if( theRpfHeader.valid() )
   {
      theRpfHeader->print(out, std::string(""));
   }
   return out;
}

const ossimRpfHeader* ossimRpfFrameFileReader::getRpfHeader()const
{
   return theRpfHeader.get();
}

void ossimRpfFrameFileReader::clearAll()
{
   theRpfHeader = 0;
   theFilename = "";
}
