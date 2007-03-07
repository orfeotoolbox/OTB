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
// $Id: ossimRpfFrameFileIndexRecord.cpp,v 1.1 2003/04/02 20:45:00 gpotts Exp $
#include "ossimRpfFrameFileIndexRecord.h"
#include "base/misc/ossimEndian.h"
#include <string.h> // for memset
#include "base/common/ossimErrorCodes.h"

ostream& operator <<(ostream& out,
                     const ossimRpfFrameFileIndexRecord& data)
{
   data.print(out);
   
   return out;
}

ossimRpfFrameFileIndexRecord::ossimRpfFrameFileIndexRecord()
{
   clearFields();
}

ossimErrorCode ossimRpfFrameFileIndexRecord::parseStream(istream& in, ossimByteOrder byteOrder)
{
   if(in)
   {
      ossimEndian anEndian;

      clearFields();
            
      in.read((char*)&theBoundaryRectRecordNumber, 2);
      in.read((char*)&theLocationRowNumber, 2);
      in.read((char*)&theLocationColumnNumber, 2);
      in.read((char*)&thePathnameRecordOffset, 4);
      in.read((char*)theFilename, 12);
      in.read((char*)theGeographicLocation, 6);
      in.read((char*)&theSecurityClassification, 1);
      in.read((char*)theFileSecurityCountryCode, 2);
      in.read((char*)theFileSecurityReleaseMarking, 2);

      if(anEndian.getSystemEndianType() != byteOrder)
      {
         anEndian.swap(theBoundaryRectRecordNumber);
         anEndian.swap(theLocationRowNumber);
         anEndian.swap(theLocationColumnNumber);
         anEndian.swap(thePathnameRecordOffset);
      }
   }
   else
   {
      return ossimErrorCodes::OSSIM_ERROR;
   }
   
   return ossimErrorCodes::OSSIM_OK;
}

void ossimRpfFrameFileIndexRecord::print(ostream& out)const
{
   out << "theBoundaryRectRecordNumber:      " << theBoundaryRectRecordNumber << endl
       << "theLocationRowNumber:             " << theLocationRowNumber << endl
       << "theLocationColumnNumber:          " << theLocationColumnNumber << endl
       << "thePathnameRecordOffset:          " << thePathnameRecordOffset << endl
       << "theFilename:                      " << theFilename << endl
       << "theGeographicLocation:            " << theGeographicLocation << endl
       << "theSecurityClassification:        " << theSecurityClassification << endl
       << "theFileSecurityCountryCode:       " << theFileSecurityCountryCode << endl
       << "theFileSecurityReleaseMarking:    " << theFileSecurityReleaseMarking;
}

void ossimRpfFrameFileIndexRecord::clearFields()
{
   theBoundaryRectRecordNumber = 0;
   theLocationRowNumber = 0;
   theLocationColumnNumber= 0;
   thePathnameRecordOffset = 0;
   memset(theFilename, ' ', 12);
   memset(theGeographicLocation, ' ', 6);
   theSecurityClassification = ' ';;
   memset(theFileSecurityCountryCode, ' ', 2);
   memset(theFileSecurityReleaseMarking, ' ', 2);

   theFilename[12] = '\0';
   theGeographicLocation[6] = '\0';
   theFileSecurityCountryCode[2] = '\0';
   theFileSecurityReleaseMarking[2] = '\0';
}

