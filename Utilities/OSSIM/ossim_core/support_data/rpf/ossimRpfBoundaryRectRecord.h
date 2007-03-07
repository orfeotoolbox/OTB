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
// $Id: ossimRpfBoundaryRectRecord.h,v 1.2 2003/06/15 19:01:34 dburken Exp $
#ifndef ossimRpfBoundaryRectRecord_HEADER
#define ossimRpfBoundaryRectRecord_HEADER
#include <iostream>
#include <iomanip>
using namespace std;

#include "base/common/ossimConstants.h" // ossiByteOrder and RPF constants
#include "base/context/ossimErrorContext.h" // for ossimErrorCode
#include "base/data_types/ossimString.h"
#include "ossimRpfCoverageSection.h"

class ossimRpfBoundaryRectRecord
{
public:
   friend ostream& operator <<(ostream& out,
                               const ossimRpfBoundaryRectRecord& data);
   ossimErrorCode parseStream(istream& in, ossimByteOrder byteOrder);
   void clearFields();

   ossimString getProductType()const{return ossimString(theProductDataType);}
   ossimString getScale()const{return ossimString(theScale);}
   ossimString getCompressionRatio()const{return ossimString(theCompressionRatio);}
   
   ossim_uint32 getNumberOfFramesVertical()const{ return theNumberOfFramesNorthSouth;}
   ossim_uint32 getNumberOfFramesHorizontal()const{ return theNumberOfFramesEastWest;}

   const ossimRpfCoverageSection& getCoverage()const{return theCoverage;}
   char getZone()const{return theZone;}

private:
   /*!
    * a five byte asci field
    */
   char theProductDataType[6];

   /*!
    * five byte asci field
    */
   char theCompressionRatio[6];

   /*!
    * 12 byte asci field.
    */
   char theScale[13];

   /*!
    *
    */
   char theZone;

   /*!
    * is a 5 byte asci field.
    */
   char theProducer[6];

   ossimRpfCoverageSection theCoverage;

   /*!
    * Four byte fields.
    */
   ossim_uint32 theNumberOfFramesNorthSouth;

   /*!
    * Four byte fields.
    */
   ossim_uint32 theNumberOfFramesEastWest;   
};

#endif
