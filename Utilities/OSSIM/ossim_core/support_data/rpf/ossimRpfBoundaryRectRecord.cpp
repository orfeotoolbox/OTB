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
// $Id: ossimRpfBoundaryRectRecord.cpp,v 1.1 2003/04/02 20:45:00 gpotts Exp $
#include "ossimRpfBoundaryRectRecord.h"
#include <string.h> // for memset
#include "base/misc/ossimEndian.h"
#include "base/common/ossimErrorCodes.h"

ostream& operator <<(ostream& out,
                     const ossimRpfBoundaryRectRecord& data)
{
   out << setiosflags(ios::fixed)
       << setprecision(12)
       << "theProductDataType:             " << data.theProductDataType << endl
       << "theCompressionRatio:            " << data.theCompressionRatio << endl
       << "theScale:                       " << data.theScale << endl
       << "theZone:                        " << data.theZone << endl
       << "theProducer:                    " << data.theProducer << endl
       << data.theCoverage << endl
       << "theNumberOfFramesNorthSouth:    " << data.theNumberOfFramesNorthSouth << endl
       << "theNumberOfFramesEastWest:      " << data.theNumberOfFramesEastWest;
      
   return out;
}

ossimErrorCode ossimRpfBoundaryRectRecord::parseStream(istream& in, ossimByteOrder byteOrder)
{
   if(in)
   {
      ossimEndian anEndian;
      clearFields();
      
      in.read((char*)&theProductDataType, 5);
      in.read((char*)&theCompressionRatio, 5);
      in.read((char*)&theScale, 12);
      in.read((char*)&theZone, 1);
      in.read((char*)&theProducer, 5);
      
      theCoverage.parseStream(in, byteOrder);
      
      in.read((char*)&theNumberOfFramesNorthSouth, 4);
      in.read((char*)&theNumberOfFramesEastWest, 4);

      if(anEndian.getSystemEndianType() != byteOrder)
      {
         anEndian.swap(theNumberOfFramesNorthSouth);
         anEndian.swap(theNumberOfFramesEastWest);
      }
   }
   else
   {
      return ossimErrorCodes::OSSIM_ERROR;
   }

   return ossimErrorCodes::OSSIM_OK;
}

void ossimRpfBoundaryRectRecord::clearFields()
{
   memset(theProductDataType, ' ', 5);
   memset(theCompressionRatio, ' ', 5);
   memset(theScale, ' ', 12);
   memset(theProducer, ' ', 5);
   theNumberOfFramesNorthSouth = 0;
   theNumberOfFramesEastWest = 0;
   theCoverage.clearFields();

   theProductDataType[5] = '\0';
   theCompressionRatio[5] = '\0';
   theScale[12] = '\0';
   theProducer[5] = '\0';
   theZone = ' ';
}
