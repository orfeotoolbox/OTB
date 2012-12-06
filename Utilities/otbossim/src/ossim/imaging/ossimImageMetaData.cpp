//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  See top level LICENSE.txt file.
// 
// Author:  Garrett Potts
//
// Description:
//
// Contains class definition for ossimImageMetaData.
// 
//*******************************************************************
//  $Id: ossimImageMetaData.cpp 21745 2012-09-16 15:21:53Z dburken $

#include <ossim/imaging/ossimImageMetaData.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimScalarTypeLut.h>
#include <ossim/base/ossimTrace.h>
#include <algorithm>
#include <iostream>
#include <vector>

static ossimTrace traceDebug("ossimImageMetaData:debug");

ossimImageMetaData::ossimImageMetaData()
   :theNullPixelArray(0),
    theMinPixelArray(0),
    theMaxPixelArray(0),
    theMinValuesValidFlag(false),
    theMaxValuesValidFlag(false),
    theNullValuesValidFlag(false),
    theScalarType(OSSIM_SCALAR_UNKNOWN),
    theBytesPerPixel(0),
    theNumberOfBands(0)
{
}

ossimImageMetaData::ossimImageMetaData(ossimScalarType aType,
				       ossim_uint32 numberOfBands)
   :theNullPixelArray(0),
    theMinPixelArray(0),
    theMaxPixelArray(0),
    theMinValuesValidFlag(false),
    theMaxValuesValidFlag(false),
    theNullValuesValidFlag(false),
    theScalarType(aType),
    theBytesPerPixel(0),
    theNumberOfBands(numberOfBands)
{
   if(theNumberOfBands)
   {
      theNullPixelArray = new double[theNumberOfBands];
      theMinPixelArray  = new double[theNumberOfBands];
      theMaxPixelArray  = new double[theNumberOfBands];
      
      setDefaultsForArrays();
   }
   theBytesPerPixel = ossim::scalarSizeInBytes( aType );
}

ossimImageMetaData::ossimImageMetaData(const ossimImageMetaData& rhs)
   :
   theNullPixelArray(0),
   theMinPixelArray(0),
   theMaxPixelArray(0),
   theMinValuesValidFlag(rhs.theMinValuesValidFlag),
   theMaxValuesValidFlag(rhs.theMaxValuesValidFlag),
   theNullValuesValidFlag(rhs.theNullValuesValidFlag),
   theScalarType(rhs.theScalarType),
   theBytesPerPixel(rhs.theBytesPerPixel),
   theNumberOfBands(rhs.theNumberOfBands)
{
   if(theNumberOfBands)
   {
      theNullPixelArray = new double[theNumberOfBands];
      theMinPixelArray  = new double[theNumberOfBands];
      theMaxPixelArray  = new double[theNumberOfBands];
      
      std::copy(rhs.theNullPixelArray,
                rhs.theNullPixelArray+theNumberOfBands,
                theNullPixelArray);
      std::copy(rhs.theMinPixelArray,
                rhs.theMinPixelArray+theNumberOfBands,
                theMinPixelArray);
      std::copy(rhs.theMaxPixelArray,
                rhs.theMaxPixelArray+theNumberOfBands,
                theMaxPixelArray);
   }
}

const ossimImageMetaData& ossimImageMetaData::operator=( const ossimImageMetaData& rhs )
{
   if( this != &rhs )
   {
      clear();
      theMinValuesValidFlag  = rhs.theMinValuesValidFlag;
      theMaxValuesValidFlag  = rhs.theMaxValuesValidFlag;
      theNullValuesValidFlag = rhs.theNullValuesValidFlag;
      theNumberOfBands       = rhs.theNumberOfBands;
      theScalarType          = rhs.theScalarType;
      theBytesPerPixel       = rhs.theBytesPerPixel;
      if(theNumberOfBands)
      {
         theNullPixelArray = new double[theNumberOfBands];
         theMinPixelArray  = new double[theNumberOfBands];
         theMaxPixelArray  = new double[theNumberOfBands];
         
         std::copy(rhs.theNullPixelArray,
                   rhs.theNullPixelArray+theNumberOfBands,
                   theNullPixelArray);
         std::copy(rhs.theMinPixelArray,
                   rhs.theMinPixelArray+theNumberOfBands,
                   theMinPixelArray);
         std::copy(rhs.theMaxPixelArray,
                   rhs.theMaxPixelArray+theNumberOfBands,
                   theMaxPixelArray);
      }
   }
   return *this;
}

ossimImageMetaData::~ossimImageMetaData()
{
  clear();
}

void ossimImageMetaData::clear()
{
   if(theNullPixelArray)
   {
      delete [] theNullPixelArray;
      theNullPixelArray = 0;
   }
   if(theMinPixelArray)
   {
      delete [] theMinPixelArray;
      theMinPixelArray = 0;
   }
   if(theMaxPixelArray)
   {
      delete [] theMaxPixelArray;
      theMaxPixelArray = 0;
   }
   theScalarType    = OSSIM_SCALAR_UNKNOWN;
   theBytesPerPixel = 0;
   theNumberOfBands = 0;
}

void ossimImageMetaData::setDefaultsForArrays()
{
   ossim_uint32 i=0;
   
   for(i = 0; i < theNumberOfBands; ++i)
   {
      theNullPixelArray[i] = ossim::defaultNull(theScalarType);
      theMinPixelArray[i]  = ossim::defaultMin(theScalarType);
      theMaxPixelArray[i]  = ossim::defaultMax(theScalarType);
   }
}

void ossimImageMetaData::setNumberOfBands(ossim_uint32 numberOfBands)
{
   if(theNullPixelArray)
   {
      delete [] theNullPixelArray;
      theNullPixelArray = 0;
   }
   if(theMinPixelArray)
   {
      delete [] theMinPixelArray;
      theMinPixelArray = 0;
   }
   if(theMaxPixelArray)
   {
      delete [] theMaxPixelArray;
      theMaxPixelArray = 0;
   }
   
   theNumberOfBands = numberOfBands;

   if(theNumberOfBands)
   {
      theNullPixelArray = new double[theNumberOfBands];
      theMinPixelArray  = new double[theNumberOfBands];
      theMaxPixelArray  = new double[theNumberOfBands];
   
      setDefaultsForArrays();
   }
}

bool ossimImageMetaData::loadState(const ossimKeywordlist& kwl, const char* prefix)
{
   bool result = false; // return status
   
   // Clear the object:
   clear();

   std::string pfx = ( prefix ? prefix : "" );
   
   ossim_uint32 bands = getBandCount( kwl, pfx );
   if ( bands )
   {
      result = true;

      setNumberOfBands( bands );
      
      theMinValuesValidFlag  = true;
      theMaxValuesValidFlag  = true;
      theNullValuesValidFlag = true;

      updateMetaData( kwl, pfx ); // Initializes the rest of object.
   }
   else
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << " ERROR:\n"
            << "Required keyword not found:  " << ossimKeywordNames::NUMBER_BANDS_KW << std::endl;
      }
   }
   
   if (traceDebug())
   {
      print( ossimNotify(ossimNotifyLevel_DEBUG) );
   }

   return result;
}

bool ossimImageMetaData::saveState(ossimKeywordlist& kwl,
				   const char* prefix)const
{
   if ( theNumberOfBands )
   {
      ossim_uint32 band = 0;
      
      for (band=0; band < theNumberOfBands; ++band)
      {
         ossimString kwMin = (ossimKeywordNames::BAND_KW +
                              ossimString::toString(band+1) + "." +
                              ossimKeywordNames::MIN_VALUE_KW);
         ossimString kwMax = (ossimKeywordNames::BAND_KW +
                              ossimString::toString(band+1) + "." +
                              ossimKeywordNames::MAX_VALUE_KW);
         ossimString kwNull = (ossimKeywordNames::BAND_KW +
                               ossimString::toString(band+1) + "." +
                               ossimKeywordNames::NULL_VALUE_KW);
         
         kwl.add(prefix,
                 kwMin.c_str(),
                 theMinPixelArray[band],
                 true);
         kwl.add(prefix,
                 kwMax.c_str(),
                 theMaxPixelArray[band],
                 true);
         kwl.add(prefix,
                 kwNull.c_str(),
                 theNullPixelArray[band],
                 true);
      }
      // std::cout << "added bands ******************" << std::endl;
      kwl.add(prefix,
	      ossimKeywordNames::NUMBER_BANDS_KW,
	      theNumberOfBands,
	      true);
      kwl.add(prefix,
	      ossimKeywordNames::SCALAR_TYPE_KW,
	      (ossimScalarTypeLut::instance()->getEntryString(theScalarType)),
	      true);

      kwl.add( prefix, "bytes_per_pixel", theBytesPerPixel, true );
   }
   
   return true;
}

ossim_uint32 ossimImageMetaData::getNumberOfBands()const
{
   return theNumberOfBands;
}

void ossimImageMetaData::setScalarType(ossimScalarType aType)
{
   theScalarType    = aType;
   theBytesPerPixel = ossim::scalarSizeInBytes( aType );
}

ossimScalarType ossimImageMetaData::getScalarType()const
{
   return theScalarType;
}

ossim_uint32 ossimImageMetaData::getBytesPerPixel() const
{
   return theBytesPerPixel;
}

void ossimImageMetaData::setMinPix(ossim_uint32 band, double pix)
{
   if((band < theNumberOfBands)&&(theMinPixelArray))
   {
      theMinPixelArray[band] = pix;
   }
}

void ossimImageMetaData::setMaxPix(ossim_uint32 band, double pix)
{
   if((band < theNumberOfBands) && theMaxPixelArray)
   {
      theMaxPixelArray[band] = pix;
   }
}

void ossimImageMetaData::setNullPix(ossim_uint32 band, double pix)
{
   if((band < theNumberOfBands)&&(theNullPixelArray))
   {
      theNullPixelArray[band] = pix;
   }
}

double ossimImageMetaData::getMinPix(ossim_uint32 band)const
{
   if(theNumberOfBands&&theMinPixelArray)
   {
      ossim_uint32 i = ossim::min(band, (ossim_uint32)(theNumberOfBands-1));
      
      return theMinPixelArray[i];
   }
   return ossim::defaultMin(theScalarType);
}

double ossimImageMetaData::getMaxPix(ossim_uint32 band)const
{
   if(theNumberOfBands&&theMaxPixelArray)
   {
      ossim_uint32 i = ossim::min(band, (ossim_uint32)(theNumberOfBands-1));
      
      return theMaxPixelArray[i];
   }
   return ossim::defaultMax(theScalarType);
}

double ossimImageMetaData::getNullPix(ossim_uint32 band)const
{
   if(theNumberOfBands&&theNullPixelArray)
   {
      ossim_uint32 i = ossim::min(band, (ossim_uint32)(theNumberOfBands-1));
      
      return theNullPixelArray[i];
   }
   return ossim::defaultNull(theScalarType);
}

const double* ossimImageMetaData::getMinPixelArray()const
{
   return theMinPixelArray;
}
const double* ossimImageMetaData::getMaxPixelArray()const
{
   return theMaxPixelArray;
}

const double* ossimImageMetaData::getNullPixelArray()const
{
   return theNullPixelArray;
}

void ossimImageMetaData::setMinValuesValid(bool flag)
{
   theMinValuesValidFlag = flag;
}

void ossimImageMetaData::setMaxValuesValid(bool flag)
{
   theMaxValuesValidFlag = flag;
}

void ossimImageMetaData::setNullValuesValid(bool flag)
{
   theNullValuesValidFlag = flag;
}

bool ossimImageMetaData::getMinValuesValidFlag()const
{
   return theMinValuesValidFlag;
}

bool ossimImageMetaData::getMaxValuesValidFlag()const
{
   return theMaxValuesValidFlag;
}

bool ossimImageMetaData::getNullValuesValidFlag()const
{
   return theNullValuesValidFlag;
}

bool ossimImageMetaData::isValid()const
{
   return (theNumberOfBands > 0);
//    return ((theNumberOfBands>0)&&
//            (theScalarType != OSSIM_SCALAR_UNKNOWN));
}

void ossimImageMetaData::updateMetaData(
   const ossimKeywordlist& kwl, const std::string& prefix )
{
   ossimString value;
   std::string key;
   
   // Get the bands:
   if ( !theNumberOfBands )
   {
      ossim_uint32 bands = getBandCount( kwl, prefix );
      if ( bands )
      {
         setNumberOfBands( bands );
      }
   }

   if ( theNumberOfBands )
   {
      //---
      // See if bands are zero or one based.  Bands are ONE based in the "omd"
      // file! If band0 is present assume zero based.
      //---

      ossimString regExpression = std::string("^(") + prefix + std::string("band0+.)");
      std::vector<ossimString> keys = kwl.getSubstringKeyList( regExpression );

      ossim_uint32 startBand = (keys.size() ? 0 : 1);

      ossim_uint32 limit = theNumberOfBands+startBand; // For for loop:
      
      std::string bs = "band"; // band sting
      std::string base;
      
      for ( ossim_uint32 band = startBand; band < limit; ++band )
      {
         base = bs + ossimString::toString( band ).string() + std::string(".") ;

         // Min:
         key = base + std::string(ossimKeywordNames::MIN_VALUE_KW);
         value =  kwl.findKey( prefix, key );
         
         if ( value.size() )
         {
            theMinPixelArray[ band - startBand  ] = value.toFloat64();
         }

         // Max:
         key = base + std::string(ossimKeywordNames::MAX_VALUE_KW);
         value =  kwl.findKey( prefix, key );
         if ( value.size() )
         {
            theMaxPixelArray[ band - startBand ] = value.toFloat64();
         }

         // Null:
         key = base + std::string(ossimKeywordNames::NULL_VALUE_KW);
         value =  kwl.findKey( prefix, key );
         if ( value.size() )
         {
            theNullPixelArray[ band - startBand ] = value.toFloat64();
         }
      }  
   }

   // Scalar, only look for if not set.
   if ( theScalarType == OSSIM_SCALAR_UNKNOWN )
   {
      std::string key = ossimKeywordNames::SCALAR_TYPE_KW; // "scalar_type"
      value.string() = kwl.findKey( prefix, key );
      if ( value.empty() )
      {
         key = "radiometry";
         value.string() = kwl.findKey( prefix, key );
      }
      if ( value.size() )
      {
         theScalarType = ossimScalarTypeLut::instance()->getScalarTypeFromString( value );
      }
      
      if ( theScalarType != OSSIM_SCALAR_UNKNOWN )
      {
         theBytesPerPixel = ossim::scalarSizeInBytes( theScalarType );
      }
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimImageMetaData::updateMetaData DEBUG"
         << *this << std::endl;
   }
}

std::ostream& ossimImageMetaData::print(std::ostream& out) const
{
   ossimKeywordlist kwl;
   saveState( kwl, 0 );
   out << kwl << std::endl;
   return out;
}

std::ostream& operator<<(std::ostream& out, const ossimImageMetaData& obj)
{
   return obj.print( out );
}

ossim_uint32 ossimImageMetaData::getBandCount(const ossimKeywordlist& kwl,
                                              const std::string& prefix) const
{
   ossim_uint32 result = 0;

   ossimString value;
   value.string() = kwl.findKey( prefix, std::string(ossimKeywordNames::NUMBER_BANDS_KW) );
   if ( value.size() )
   {
      result = value.toUInt32();
   }
   else
   {
      value.string() = std::string("^(") + prefix + std::string("band[0-9]+.)");
      std::vector<ossimString> keys = kwl.getSubstringKeyList( value );
      result = static_cast<ossim_uint32>( keys.size() );
   }
   return result;   
}
