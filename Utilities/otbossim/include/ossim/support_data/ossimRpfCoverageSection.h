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
// $Id: ossimRpfCoverageSection.h 16997 2010-04-12 18:53:48Z dburken $
#ifndef ossimRpfCoverageSection_HEADER
#define ossimRpfCoverageSection_HEADER

#include <iosfwd>

#include <ossim/base/ossimConstants.h>
#include <ossim/support_data/ossimRpfConstants.h>
#include <ossim/base/ossimErrorCodes.h>

class ossimRpfCoverageSection
{
public:
   friend std::ostream& operator <<(std::ostream &out,
                                    const ossimRpfCoverageSection &data);
   
   ossimRpfCoverageSection();
   ossimRpfCoverageSection(const ossimRpfCoverageSection& obj);
   const ossimRpfCoverageSection& operator=(const ossimRpfCoverageSection& rhs);
   
   ~ossimRpfCoverageSection(){}

   ossimErrorCode parseStream(std::istream &in, ossimByteOrder byteOrder);

   /**
    * @brief Write method.
    *
    * @param out Stream to write to.
    */
   void writeStream(std::ostream& out);


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

   ossim_float64 getUlLat()const{return theUpperLeftLat;}
   ossim_float64 getUlLon()const{return theUpperLeftLon;}
   ossim_float64 getLlLat()const{return theLowerLeftLat;}
   ossim_float64 getLlLon()const{return theLowerLeftLon;}
   ossim_float64 getLrLat()const{return theLowerRightLat;}
   ossim_float64 getLrLon()const{return theLowerRightLon;}
   ossim_float64 getUrLat()const{return theUpperRightLat;}
   ossim_float64 getUrLon()const{return theUpperRightLon;}

   ossim_float64 getVerticalResolution()const{return theVerticalResolution;}
   ossim_float64 getHorizontalResolution()const{return theHorizontalResolution;}

   /*!
    * This indicates the lat increment per pixel in degrees.
    */
   ossim_float64 getVerticalInterval()const{return theVerticalInterval;}

   /*!
    * This indicates the lon increment per pixel in degrees.
    */
   ossim_float64 getHorizontalInterval()const{return theHorizontalInterval;}

   /** @brief Sets theUpperLeftLat to value. */
   void setUlLat(ossim_float64 value);

   /** @brief Sets theUpperLeftLon to value. */
   void setUlLon(ossim_float64 value);

   /** @brief Sets theLowerLeftLat to value. */
   void setLlLat(ossim_float64 value);

   /** @brief Sets theLowerLeftLon to value. */
   void setLlLon(ossim_float64 value);
   
   /** @brief Sets theLowerRightLat to value. */
   void setLrLat(ossim_float64 value);
   
   /** @brief Sets theLowerRightLon to value. */
   void setLrLon(ossim_float64 value);

   /** @brief Sets theUpperRightLat to value. */
   void setUrLat(ossim_float64 value);

   /** @brief Sets theUpperRightLon to value. */
   void setUrLon(ossim_float64 value);

   /** @brief Sets theVerticalResolution to value. */
   void setVerticalResolution(ossim_float64 value);

   /** @brief Sets theHorizontalResolution to value. */
   void setHorizontalResolution(ossim_float64 value);
   
   /** @brief Sets theVerticalInterval to value. */
   void setVerticalInterval(ossim_float64 value);
   
   /** @brief Sets theHorizontalInterval to value. */
   void setHorizontalInterval(ossim_float64 value);
   
private:   
   /*!
    * 8 byte ossim_float64 and is the upper left lat or
    * North West portion of the image.
    */
   ossim_float64 theUpperLeftLat;

   /*!
    * 8 byte ossim_float64 and is the upper left lon
    * (North west)
    */
   ossim_float64 theUpperLeftLon;

   ossim_float64 theLowerLeftLat;
   ossim_float64 theLowerLeftLon;

   ossim_float64 theUpperRightLat;
   ossim_float64 theUpperRightLon;

   ossim_float64 theLowerRightLat;
   ossim_float64 theLowerRightLon;

   ossim_float64 theVerticalResolution;
   ossim_float64 theHorizontalResolution;

   ossim_float64 theVerticalInterval;
   ossim_float64 theHorizontalInterval;
};

#endif
