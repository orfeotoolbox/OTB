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
// $Id: ossimRpfFrameFileIndexRecord.h,v 1.3 2004/03/19 18:31:47 gpotts Exp $
#ifndef ossimRpfFrameFileIndexRecord_HEADER
#define ossimRpfFrameFileIndexRecord_HEADER
#include <iostream>
using namespace std;
#include "base/common/ossimConstants.h" // ossiByteOrder and RPF constants
#include "base/context/ossimErrorContext.h" // for ossimErrorCode
#include "base/data_types/ossimFilename.h"

class ossimRpfFrameFileIndexRecord
{
public:
   friend ostream& operator <<(ostream& out,
                               const ossimRpfFrameFileIndexRecord& data);
   
   ossimRpfFrameFileIndexRecord();
   ossimErrorCode parseStream(istream& in, ossimByteOrder byteOrder);
   void clearFields();
   void print(ostream& out)const;
   
   ossim_uint16 getBoundaryRecNumber()const{return theBoundaryRectRecordNumber;}
   ossim_uint16  getLocationRowNumber()const{return theLocationRowNumber;}
   ossim_uint16  getLocationColNumber()const{return theLocationColumnNumber;}
   ossim_uint32  getPathnameRecordOffset()const{return thePathnameRecordOffset;}
   ossimFilename getFilename()const{return ossimFilename(theFilename);}
   
private:
   ossim_uint16 theBoundaryRectRecordNumber;
   ossim_uint16 theLocationRowNumber;
   ossim_uint16 theLocationColumnNumber;
   ossim_uint32 thePathnameRecordOffset;

   /*!
    * This is a 12 byte asci field.
    */
   char           theFilename[13];

   /*!
    * this is a 6 byte asci field.
    */
   char           theGeographicLocation[7];

   char           theSecurityClassification;

   /*!
    * is a 2 byte field.
    */
   char           theFileSecurityCountryCode[3];

   /*!
    * This is a 2 byte field.
    */
   char           theFileSecurityReleaseMarking[3];
};

#endif
