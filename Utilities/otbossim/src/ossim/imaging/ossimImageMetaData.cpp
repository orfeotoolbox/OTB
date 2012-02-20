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
//  $Id: ossimImageMetaData.cpp 20265 2011-11-18 12:19:39Z gpotts $
#include <vector>
#include <algorithm>
#include <ossim/imaging/ossimImageMetaData.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimScalarTypeLut.h>
#include <ossim/base/ossimTrace.h>

static ossimTrace traceDebug("ossimImageMetaData:debug");

ossimImageMetaData::ossimImageMetaData()
   :theNullPixelArray(0),
    theMinPixelArray(0),
    theMaxPixelArray(0),
    theScalarType(OSSIM_SCALAR_UNKNOWN),
    theNumberOfBands(0)
{
}

ossimImageMetaData::ossimImageMetaData(ossimScalarType aType,
				       ossim_uint32 numberOfBands)
   :theNullPixelArray(0),
    theMinPixelArray(0),
    theMaxPixelArray(0),
    theScalarType(aType),
    theNumberOfBands(numberOfBands)
{
   if(theNumberOfBands)
   {
      theNullPixelArray = new double[theNumberOfBands];
      theMinPixelArray  = new double[theNumberOfBands];
      theMaxPixelArray  = new double[theNumberOfBands];
      
      setDefaultsForArrays();
   }
}

ossimImageMetaData::ossimImageMetaData(const ossimImageMetaData& rhs)
:theNullPixelArray(0),
theMinPixelArray(0),
theMaxPixelArray(0),
theMinValuesValidFlag(false),
theMaxValuesValidFlag(false),
theNullValuesValidFlag(false),
theScalarType(OSSIM_SCALAR_UNKNOWN),
theNumberOfBands(0)
{
   assign(rhs);
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
   theNumberOfBands = 0;
}

void ossimImageMetaData::clearBandInfo()
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

void ossimImageMetaData::assign(const ossimImageMetaData& rhs)
{
   if(this != &rhs)
   {
      clear();
      theNumberOfBands = rhs.getNumberOfBands();
      theScalarType    = rhs.getScalarType();
      
      if(theNumberOfBands)
      {
         theNullPixelArray = new double[theNumberOfBands];
         theMinPixelArray  = new double[theNumberOfBands];
         theMaxPixelArray  = new double[theNumberOfBands];
         
         std::copy(rhs.getNullPixelArray(),
                   rhs.getNullPixelArray()+theNumberOfBands,
                   theNullPixelArray);
         std::copy(rhs.getMinPixelArray(),
                   rhs.getMinPixelArray()+theNumberOfBands,
                   theMinPixelArray);
         std::copy(rhs.getMaxPixelArray(),
                   rhs.getMaxPixelArray()+theNumberOfBands,
                   theMaxPixelArray);
      }
   }
}

bool ossimImageMetaData::loadState(const ossimKeywordlist& kwl,
				   const char* prefix)
{
   static const char MODULE[] = "ossimImageMetaData::loadState";

   if (traceDebug())
   {
      CLOG << " Entered..."
           << "\nkwl:\n" << kwl
           << "\nprefix:  " << (prefix?prefix:"") << std::endl;
   }
   
   bool result = true;
   clear();

   ossim_int32 scalar =
      ossimScalarTypeLut::instance()->getEntryNumber(kwl, prefix);
   if (scalar == ossimLookUpTable::NOT_FOUND)
   {
      theScalarType = OSSIM_SCALAR_UNKNOWN;

      ossimString lookupStr = kwl.find( prefix, "radiometry" );
      if ( lookupStr.empty() == false )
      {
         if ( lookupStr.contains("8-bit") )
         {
            theScalarType = OSSIM_UINT8;
         }
         else
         if ( lookupStr.contains("11-bit") )
         {
            theScalarType = OSSIM_USHORT11;
         }
         else
         if ( lookupStr.contains("16-bit unsigned") )
         {
            theScalarType =  OSSIM_UINT16;
         }
         else
         if ( lookupStr.contains("16-bit signed") )
         {
            theScalarType =  OSSIM_SINT16;
         }
         else
         if ( lookupStr.contains("32-bit unsigned") )
         {
            theScalarType =  OSSIM_UINT32;
         }
         else
         if ( lookupStr.contains("float") )
         {
            theScalarType =  OSSIM_FLOAT32;
         }
         else
         if ( lookupStr.contains("normalized float") )
         {
            theScalarType =  OSSIM_FLOAT32;
         }
         else
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << MODULE
               << "\nERROR: Unrecognized pixel scalar type description: " 
               <<  lookupStr
               << std::endl;
         }
      }
   }
   else
   {
      theScalarType = static_cast<ossimScalarType>(scalar);
   }

   const char* kwlLookup =
      kwl.find(prefix, ossimKeywordNames::NUMBER_BANDS_KW);
   if (kwlLookup)
   {
      theNumberOfBands = ossimString(kwlLookup).toInt();
      setNumberOfBands(theNumberOfBands);
   }
  
   loadBandInfo(kwl, prefix);

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "\ntheScalarType:  " << theScalarType
         << "\ntheNumberOfBands: " << theNumberOfBands
         << "\ntheMinValuesValidFlag: " << theMinValuesValidFlag
         << "\ntheMaxValuesValidFlag: " << theMaxValuesValidFlag
         << "\ntheNullValuesValidFlag: " << theNullValuesValidFlag;
          
      for (ossim_uint32 band = 0; band < theNumberOfBands; ++band)
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "\ntheMinValuesValidFlag[" << band << "]: "
            << theMinPixelArray[band]
            << "\ntheMaxValuesValidFlag[" << band << "]: "
            << theMaxPixelArray[band]
            << "\ntheNullValuesValidFlag[" << band << "]: "
            << theNullPixelArray[band];
      }
      ossimNotify(ossimNotifyLevel_DEBUG) << std::endl;
   }
   

   if(!theNumberOfBands)
   {
      result = false;
   }
   return result;
}

void ossimImageMetaData::loadBandInfo(const ossimKeywordlist& kwl,
                                      const char* prefix)
{
   ossimString copyPrefix;
   if (prefix)
   {
      copyPrefix = prefix;
   }
   
   ossimString regExpression =  ossimString("^(") + copyPrefix + "band[0-9]+.)";
   std::vector<ossimString> keys =
      kwl.getSubstringKeyList( regExpression );
   
   ossim_uint32 numberOfBands = (ossim_uint32)keys.size();

   theMinValuesValidFlag  = true;
   theMaxValuesValidFlag  = true;
   theNullValuesValidFlag = true;

   if(!numberOfBands)
   {
      theMinValuesValidFlag  = false;
      theMaxValuesValidFlag  = false;
      theNullValuesValidFlag = false;
      return;
   }
   if((!theNumberOfBands)||
      (theNumberOfBands != numberOfBands))
   {
      setNumberOfBands(numberOfBands);
   }
   
   int offset = (int)(copyPrefix+"band").size();
   int idx = 0;
   std::vector<int> theNumberList(numberOfBands);
   for(idx = 0; idx < (int)theNumberList.size();++idx)
     {
       ossimString numberStr(keys[idx].begin() + offset,
			     keys[idx].end());
       theNumberList[idx] = numberStr.toInt();
     }
   std::sort(theNumberList.begin(), theNumberList.end());
   for(idx=0;idx < (int)theNumberList.size();++idx)
   {
      ossimString newPrefix = copyPrefix;
      newPrefix += ossimString("band");
      newPrefix += ossimString::toString(theNumberList[idx]);
      newPrefix += ossimString(".");

      const char* minValue = kwl.find(newPrefix.c_str(),
                                      ossimKeywordNames::MIN_VALUE_KW);
      const char* maxValue = kwl.find(newPrefix.c_str(),
                                      ossimKeywordNames::MAX_VALUE_KW);
      const char* nullValue = kwl.find(newPrefix.c_str(),
                                       ossimKeywordNames::NULL_VALUE_KW);

      if(minValue)
      {
         theMinPixelArray[idx] = ossimString(minValue).toDouble();
      }
      else
      {
         theMinValuesValidFlag = false;
         theMinPixelArray[idx] = ossim::defaultMin(theScalarType);
      }
      if(maxValue)
      {
         theMaxPixelArray[idx] = ossimString(maxValue).toDouble();
      }
      else
      {
         theMaxValuesValidFlag = false;
         theMaxPixelArray[idx] = ossim::defaultMax(theScalarType);
      }
      if(nullValue)
      {
         theNullPixelArray[idx] = ossimString(nullValue).toDouble();
      }
      else
      {
         theNullValuesValidFlag = false;
         theNullPixelArray[idx] = ossim::defaultNull(theScalarType);
      }
   }
}

bool ossimImageMetaData::saveState(ossimKeywordlist& kwl,
				   const char* prefix)const
{
   if(theNullPixelArray&&theMinPixelArray&&theMaxPixelArray)
   {
      ossim_int32 band = 0;
      
      for (band=0; band<(ossim_int32)theNumberOfBands; ++band)
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
      kwl.add(prefix,
	      ossimKeywordNames::NUMBER_BANDS_KW,
	      (ossim_int32)theNumberOfBands,
	      true);
      kwl.add(prefix,
	      ossimKeywordNames::SCALAR_TYPE_KW,
	      (ossimScalarTypeLut::instance()->getEntryString(theScalarType)),
	      true);
   }
   
   return true;
}

ossim_uint32 ossimImageMetaData::getNumberOfBands()const
{
   return theNumberOfBands;
}

void ossimImageMetaData::setScalarType(ossimScalarType aType)
{
   theScalarType = aType;
}

ossimScalarType ossimImageMetaData::getScalarType()const
{
   return theScalarType;
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

const ossimImageMetaData& ossimImageMetaData::operator=(
   const ossimImageMetaData& rhs)
{
   assign(rhs);
   
   return *this;
}

bool ossimImageMetaData::isValid()const
{
   return (theNumberOfBands > 0);
//    return ((theNumberOfBands>0)&&
//            (theScalarType != OSSIM_SCALAR_UNKNOWN));
}
