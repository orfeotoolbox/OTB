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
//
// Description:
//
//*******************************************************************
//  $Id: ossimGeoidNgsHeader.h,v 1.2 2005/11/04 17:50:35 gpotts Exp $
#ifndef ossimGeoidNgsHeader_HEADER
#define ossimGeoidNgsHeader_HEADER
#include "base/data_types/ossimFilename.h"

#include <iostream>

class OSSIMDLLEXPORT ossimGeoidNgsHeader
{
public:
   friend std::ostream& operator << (std::ostream &out, const ossimGeoidNgsHeader &data);
   
   ossimGeoidNgsHeader();
   ossimGeoidNgsHeader(const ossimFilename &fileName,
                       ossimByteOrder byteOrder=OSSIM_LITTLE_ENDIAN);

   bool initialize(const ossimFilename &fileName,
                   ossimByteOrder byteOrder=OSSIM_LITTLE_ENDIAN);
   
   double southernMostLat()const{return theSouthernMostLatitude;}
   double westernMostLon()const{return theWesternMostLongitude;}
   double latDelta()const{return theLatDelta;}
   double lonDelta()const{return theLonDelta;}
   int    rows()const{return theRows;}
   int    cols()const{return theCols;}
   int    dataType()const{return theDataType;}
   bool   pointWithin(double lat, double lon)const;
   const ossimFilename& filename()const{return theFilename;}
   
   int headerSize()const{return 44;}
   int dataTypeSize()const{return theDataType==1?4:0;}

   double getHeightDelta(double lat,
                         double lon)const;
private:
   ossimFilename theFilename;
   ossimByteOrder theByteOrder;
   double theSouthernMostLatitude;

   /*!
    * Important:  The geoid.bin files expresses this as a possitive value.
    *
    * example: -90.0 is 270 degrees.
    */
   double theWesternMostLongitude;

   /*!
    * Specifies the spacing of the latitude direction.
    */
   double theLatDelta;

   /*!
    * longitude spacing.
    */
   double theLonDelta;

   /*!
    * theRows specifies the number of lat increments
    */
   int    theRows;

   /*!
    * theCols specifies the number of lon increments
    */
   int    theCols;

   /*!
    * Specifies the datatype.  if this is 1 then the data that follows
    * is 4 byte floats.
    */
   int    theDataType;
};

#endif
