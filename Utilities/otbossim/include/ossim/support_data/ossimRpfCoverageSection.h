//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description: Rpf support class
// 
//********************************************************************
// $Id: ossimRpfCoverageSection.h 9967 2006-11-29 02:01:23Z gpotts $
#ifndef ossimRpfCoverageSection_HEADER
#define ossimRpfCoverageSection_HEADER
#include <iostream>
using namespace std;

#include <ossim/base/ossimConstants.h>
#include <ossim/support_data/ossimRpfConstants.h>
#include <ossim/base/ossimErrorContext.h>

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
