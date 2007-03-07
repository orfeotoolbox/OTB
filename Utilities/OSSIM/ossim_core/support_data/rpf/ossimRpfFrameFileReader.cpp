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
// Description: Rpf support class
// 
//********************************************************************
// $Id: ossimRpfFrameFileReader.cpp,v 1.3 2003/06/05 14:40:06 gpotts Exp $
#include "ossimRpfFrameFileReader.h"
#include "ossimRpfHeader.h"
#include "ossimRpfImageDescriptionSubheader.h"
#include "ossimRpfMaskSubsection.h"
#include "ossimRpfCompressionSectionSubheader.h"
#include "ossimRpfAttributeSectionSubheader.h"
#include "support_data/nitf/ossimNitfFile.h"
#include "support_data/nitf/ossimNitfTagInformation.h"
#include "support_data/nitf/ossimNitfFileHeader.h"
#include "base/common/ossimErrorCodes.h"

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
   const ossimNitfFileHeader* nitfFileHeader = nitfFile->getHeader();
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
