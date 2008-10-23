//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description: Rpf support class
// 
//********************************************************************
// $Id: ossimRpfBoundaryRectRecord.h 9967 2006-11-29 02:01:23Z gpotts $
#ifndef ossimRpfBoundaryRectRecord_HEADER
#define ossimRpfBoundaryRectRecord_HEADER
#include <iostream>
#include <iomanip>
using namespace std;

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimErrorContext.h>
#include <ossim/base/ossimString.h>
#include <ossim/support_data/ossimRpfCoverageSection.h>

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
