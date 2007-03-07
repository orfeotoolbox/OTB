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
// $Id: ossimRpfCoverageSection.h,v 1.3 2003/05/13 11:31:10 dburken Exp $
#ifndef ossimRpfCoverageSection_HEADER
#define ossimRpfCoverageSection_HEADER
#include <iostream>
using namespace std;

#include "base/common/ossimConstants.h" // ossiByteOrder and RPF constants
#include "ossimRpfConstants.h" // ossiByteOrder and RPF constants
#include "base/context/ossimErrorContext.h" // for ossimErrorCode

class ossimRpfCoverageSection
{
public:
   friend ostream& operator <<(ostream &out,
                               const ossimRpfCoverageSection &data);
   
   ossimRpfCoverageSection();
   virtual ~ossimRpfCoverageSection(){}
   ossim_int32 parseStream(istream &in, ossimByteOrder byteOrder);
   virtual void print(ostream &out)const;
   void clearFields();

   bool isGeographicRectNull()const
      {
         return ((theUpperLeftLat  == OSSIM_RPF_ULONG_NULL) &&
                 (theUpperLeftLon  == OSSIM_RPF_ULONG_NULL) &&
                 (theLowerLeftLat  == OSSIM_RPF_ULONG_NULL) &&
                 (theLowerLeftLon  == OSSIM_RPF_ULONG_NULL) &&
                 (theLowerRightLat == OSSIM_RPF_ULONG_NULL) &&
                 (theLowerRightLon == OSSIM_RPF_ULONG_NULL) &&
                 (theUpperRightLat == OSSIM_RPF_ULONG_NULL) &&
                 (theUpperRightLon == OSSIM_RPF_ULONG_NULL));
      }
   bool isIntervalNull()const
      {
         return ((theVerticalInterval   == OSSIM_RPF_ULONG_NULL)&&
                 (theHorizontalInterval == OSSIM_RPF_ULONG_NULL));
      }
   double getUlLat()const{return theUpperLeftLat;}
   double getUlLon()const{return theUpperLeftLon;}
   double getLlLat()const{return theLowerLeftLat;}
   double getLlLon()const{return theLowerLeftLon;}
   double getLrLat()const{return theLowerRightLat;}
   double getLrLon()const{return theLowerRightLon;}
   double getUrLat()const{return theUpperRightLat;}
   double getUrLon()const{return theUpperRightLon;}

   double getVerticalResolution()const{return theVerticalResolution;}
   double getHorizontalResolution()const{return theHorizontalResolution;}

   /*!
    * This indicates the lat increment per pixel in degrees.
    */
   double getVerticalInterval()const{return theVerticalInterval;}

   /*!
    * This indicates the lon increment per pixel in degrees.
    */
   double getHorizontalInterval()const{return theHorizontalInterval;}
   
private:   
   /*!
    * 8 byte double and is the upper left lat or
    * North West portion of the image.
    */
   double theUpperLeftLat;

   /*!
    * 8 byte double and is the upper left lon
    * (North west)
    */
   double theUpperLeftLon;

   double theLowerLeftLat;
   double theLowerLeftLon;

   double theUpperRightLat;
   double theUpperRightLon;

   double theLowerRightLat;
   double theLowerRightLon;

   /*!
    *
    */
   double theVerticalResolution;

   double theHorizontalResolution;

   double theVerticalInterval;

   double theHorizontalInterval;
   
};

#endif
