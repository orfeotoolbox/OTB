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
// $Id: ossimRpfAttributeSectionSubheader.cpp,v 1.3 2005/08/22 14:58:35 gpotts Exp $
#include "ossimRpfAttributeSectionSubheader.h"
#include "base/misc/ossimEndian.h"
#include "base/common/ossimErrorCodes.h"

ostream& operator <<(ostream& out,
                     const ossimRpfAttributeSectionSubheader& data)
{
   data.print(out);
   
   return out;
}

ossimRpfAttributeSectionSubheader::ossimRpfAttributeSectionSubheader()
{
   clearFields();
}

ossimErrorCode ossimRpfAttributeSectionSubheader::parseStream(istream& in,
                                                              ossimByteOrder byteOrder)
{
   theAttributeSectionSubheaderStart = 0;
   theAttributeSectionSubheaderEnd = 0;
   if(in)
   {
      theAttributeSectionSubheaderStart = in.tellg();
      in.read((char*)&theNumberOfAttributeOffsetRecords, 2);
      in.read((char*)&theNumberOfExplicitArealCoverageRecords, 2);
      in.read((char*)&theAttributeOffsetTableOffset, 4);
      in.read((char*)&theAttribteOffsetRecordLength, 2);
      ossimEndian anEndian;

      if(anEndian.getSystemEndianType() != byteOrder)
      {
         anEndian.swap(theNumberOfAttributeOffsetRecords);
         anEndian.swap(theNumberOfExplicitArealCoverageRecords);
         anEndian.swap(theAttributeOffsetTableOffset);
         anEndian.swap(theAttribteOffsetRecordLength);
      }
      theAttributeSectionSubheaderEnd = in.tellg();
   }
   else
   {
      return ossimErrorCodes::OSSIM_ERROR;
   }
   
   return ossimErrorCodes::OSSIM_OK;
}

void ossimRpfAttributeSectionSubheader::print(ostream& out)const
{
   out << "theNumberOfAttributeOffsetRecords:         " << theNumberOfAttributeOffsetRecords << endl
       << "theNumberOfExplicitArealCoverageRecords:   " << theNumberOfExplicitArealCoverageRecords << endl
       << "theAttributeOffsetTableOffset:             " << theAttributeOffsetTableOffset << endl
       << "theAttribteOffsetRecordLength:             " << theAttribteOffsetRecordLength;
}

void ossimRpfAttributeSectionSubheader::clearFields()
{
   theNumberOfAttributeOffsetRecords       = 0;
   theNumberOfExplicitArealCoverageRecords = 0;
   theAttributeOffsetTableOffset           = 0;
   theAttribteOffsetRecordLength           = 0;
   
   theAttributeSectionSubheaderStart       = 0;
   theAttributeSectionSubheaderEnd         = 0;
}

ossim_uint64 ossimRpfAttributeSectionSubheader::getSubheaderStart()const
{
   return theAttributeSectionSubheaderStart;
}

ossim_uint64 ossimRpfAttributeSectionSubheader::getSubheaderEnd()const
{
   return theAttributeSectionSubheaderEnd;
}
