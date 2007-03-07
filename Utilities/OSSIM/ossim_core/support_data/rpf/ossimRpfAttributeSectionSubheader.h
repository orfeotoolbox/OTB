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
// $Id: ossimRpfAttributeSectionSubheader.h,v 1.4 2005/08/22 14:58:35 gpotts Exp $
#ifndef ossimRpfAttributeSectionSubheader_HEADER
#define ossimRpfAttributeSectionSubheader_HEADER
#include <iostream>
using namespace std;

#include "base/common/ossimConstants.h" // ossiByteOrder and RPF constants
#include "base/context/ossimErrorContext.h" // for ossimErrorCode

class ossimRpfAttributeSectionSubheader
{
public:
   friend ostream& operator <<(ostream& out,
                               const ossimRpfAttributeSectionSubheader& data);
   ossimRpfAttributeSectionSubheader();

   virtual ~ossimRpfAttributeSectionSubheader(){}
   ossimErrorCode parseStream(istream& in, ossimByteOrder byteOrder);
   virtual void print(ostream& out)const;
   ossim_uint16 getNumberOfAttributeOffsetRecords()const
      {
         return theNumberOfAttributeOffsetRecords;
      }
   ossim_uint16 getAttributeOffsetTableOffset()const
      {
         return theAttributeOffsetTableOffset;
      }

   ossim_uint64 getSubheaderStart()const;
   ossim_uint64 getSubheaderEnd()const;
   
private:
   void clearFields();
   
   ossim_uint16 theNumberOfAttributeOffsetRecords;
   ossim_uint16 theNumberOfExplicitArealCoverageRecords;
   ossim_uint32 theAttributeOffsetTableOffset;
   ossim_uint16 theAttribteOffsetRecordLength;


   mutable ossim_uint64 theAttributeSectionSubheaderStart;
   mutable ossim_uint64 theAttributeSectionSubheaderEnd;
};

#endif
