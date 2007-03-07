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
// $Id: ossimRpfFrameFileIndexSectionSubheader.cpp,v 1.1 2003/04/02 20:45:00 gpotts Exp $
#include "ossimRpfFrameFileIndexSectionSubheader.h"
#include "base/misc/ossimEndian.h"
#include "base/common/ossimErrorCodes.h"

ostream& operator <<(ostream& out,
                     const ossimRpfFrameFileIndexSectionSubheader& data)
{
   data.print(out);
   
   return out;
}

ossimRpfFrameFileIndexSectionSubheader::ossimRpfFrameFileIndexSectionSubheader()
{
   clearFields();
}

ossimErrorCode ossimRpfFrameFileIndexSectionSubheader::parseStream(istream& in, ossimByteOrder byteOrder)
{
   if(in)
   {
      ossimEndian anEndian;

      in.read((char*)&theHighestSecurityClassification, 1);
      in.read((char*)&theIndexTableOffset, 4);
      in.read((char*)&theNumberOfIndexRecords, 4);
      in.read((char*)&theNumberOfPathnameRecords, 2);
      in.read((char*)&theIndexRecordLength, 2);

      if(anEndian.getSystemEndianType() != byteOrder)
      {
         anEndian.swap(theIndexTableOffset);
         anEndian.swap(theNumberOfIndexRecords);
         anEndian.swap(theNumberOfPathnameRecords);
         anEndian.swap(theIndexRecordLength);
      }
   }
   else
   {
      return ossimErrorCodes::OSSIM_ERROR;
   }
   
   return ossimErrorCodes::OSSIM_OK;
}

void ossimRpfFrameFileIndexSectionSubheader::print(ostream& out)const
{
   out << "theHighestSecurityClassification:  " << theHighestSecurityClassification << endl
       << "theIndexTableOffset:               " << theIndexTableOffset << endl
       << "theNumberOfIndexRecords:           " << theNumberOfIndexRecords << endl
       << "theNumberOfPathnameRecords:        " << theNumberOfPathnameRecords << endl
       << "theIndexRecordLength:              " << theIndexRecordLength;
}

void ossimRpfFrameFileIndexSectionSubheader::clearFields()
{
   theHighestSecurityClassification = ' ';
   theIndexTableOffset              = 0;
   theNumberOfIndexRecords          = 0;
   theNumberOfPathnameRecords       = 0;
   theIndexRecordLength             = 0;
}
