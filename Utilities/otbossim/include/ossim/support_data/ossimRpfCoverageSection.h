//*******************************************************************
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
// 
// Author: Garrett Potts
// 
// Description: Rpf support class
// 
//********************************************************************
// $Id: ossimRpfCoverageSection.h 14241 2009-04-07 19:59:23Z dburken $
#ifndef ossimRpfCoverageSection_HEADER
#define ossimRpfCoverageSection_HEADER

#include <iosfwd>

#include <ossim/base/ossimConstants.h>
#include <ossim/support_data/ossimRpfConstants.h>
#include <ossim/base/ossimErrorContext.h>

class ossimRpfCoverageSection
{
public:
   friend std::ostream& operator <<(std::ostream &out,
                                    const ossimRpfCoverageSection &data);
   
   ossimRpfCoverageSection();
   virtual ~ossimRpfCoverageSection(){}

   ossimErrorCode parseStream(std::istream &in, ossimByteOrder byteOrder);

   /**
    * @brief print method that outputs a key/value type format adding prefix
    * to keys.
    * @param out String to output to.
    * @param prefix This will be prepended to key.
    * e.g. Where prefix = "nitf." and key is "file_name" key becomes:
    * "nitf.file_name:"
    * @return output stream.
    */
   std::ostream& print(std::ostream& out,
                       const std::string& prefix=std::string()) const;
   
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

   double theVerticalResolution;

   double theHorizontalResolution;

   double theVerticalInterval;

   double theHorizontalInterval;
   
};

#endif
