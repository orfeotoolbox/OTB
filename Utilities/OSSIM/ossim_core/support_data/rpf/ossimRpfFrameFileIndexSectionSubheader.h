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
// $Id: ossimRpfFrameFileIndexSectionSubheader.h,v 1.2 2003/06/15 19:01:34 dburken Exp $
#ifndef ossimRpfFrameFileIndexSectionSubheader_HEADER
#define ossimRpfFrameFileIndexSectionSubheader_HEADER
#include <iostream>
using namespace std;

#include "base/common/ossimConstants.h" // ossiByteOrder and RPF constants
#include "base/context/ossimErrorContext.h" // for ossimErrorCode

class ossimRpfFrameFileIndexSectionSubheader
{
public:
   friend ostream& operator <<(ostream& out,
                               const ossimRpfFrameFileIndexSectionSubheader& data);
   ossimRpfFrameFileIndexSectionSubheader();
   virtual ~ossimRpfFrameFileIndexSectionSubheader(){}

   ossimErrorCode parseStream(istream& in,
                              ossimByteOrder byteOrder);
   virtual void print(ostream& out)const;
   ossim_uint32 getOffset()const{return theIndexTableOffset;}
   ossim_uint32 getNumberOfIndexRecords()const{return theNumberOfIndexRecords;}
   ossim_uint16 getNumberOfPathnameRecords()const{return theNumberOfPathnameRecords;}
   ossim_uint16 getIndexRecordLength()const{return theIndexRecordLength;}

   void clearFields();
private:
   char          theHighestSecurityClassification;
   ossim_uint32  theIndexTableOffset;
   ossim_uint32  theNumberOfIndexRecords;
   ossim_uint16  theNumberOfPathnameRecords;
   ossim_uint16  theIndexRecordLength;
};

#endif
