//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  See top level LICENSE.txt file.
//
// Author:  Garrett Potts
//
//*******************************************************************
//  $Id: ossimUnitTypeLut.cpp 10378 2007-01-26 14:27:21Z gpotts $

#include <ossim/base/ossimUnitTypeLut.h>

ossimUnitTypeLut* ossimUnitTypeLut::theInstance = NULL;

static const int TABLE_SIZE = 17;

ossimUnitTypeLut* ossimUnitTypeLut::instance()
{
   if (!theInstance)
   {
      theInstance = new ossimUnitTypeLut;
   }
   return theInstance;
}

ossimUnitTypeLut::ossimUnitTypeLut()
  :ossimLookUpTable(TABLE_SIZE)
{
   theTable[0].theKey    = OSSIM_UNIT_UNKNOWN;
   theTable[0].theValue  = "unknown";
   theTable[1].theKey    = OSSIM_METERS;
   theTable[1].theValue  = "meters";
   theTable[2].theKey    = OSSIM_FEET;
   theTable[2].theValue  = "feet";
   theTable[3].theKey    = OSSIM_US_SURVEY_FEET;
   theTable[3].theValue  = "us_survey_feet";
   theTable[4].theKey    = OSSIM_DEGREES;
   theTable[4].theValue  = "degrees";
   theTable[5].theKey    = OSSIM_RADIANS;
   theTable[5].theValue  = "radians";
   theTable[6].theKey    = OSSIM_NAUTICAL_MILES;
   theTable[6].theValue  = "nautical_miles";
   theTable[7].theKey    = OSSIM_SECONDS;
   theTable[7].theValue  = "seconds";
   theTable[8].theKey    = OSSIM_MINUTES;
   theTable[8].theValue  = "minutes";
   theTable[9].theKey    = OSSIM_PIXEL;
   theTable[9].theValue  = "pixel";
   theTable[10].theKey   = OSSIM_MILES;
   theTable[10].theValue = "miles";
   theTable[11].theKey   = OSSIM_MILLIMETERS;
   theTable[11].theValue = "millimeters";
   theTable[12].theKey   = OSSIM_MICRONS;
   theTable[12].theValue = "microns";
   theTable[13].theKey   = OSSIM_CENTIMETERS;
   theTable[13].theValue = "centimeters";
   theTable[14].theKey   = OSSIM_YARDS;
   theTable[14].theValue = "yards";
   theTable[15].theKey   = OSSIM_INCHES;
   theTable[15].theValue = "inches";
   theTable[16].theKey   = OSSIM_KILOMETERS;
   theTable[16].theValue = "kilometers";
}

ossimUnitTypeLut::~ossimUnitTypeLut()
{
}
ossim_int32 ossimUnitTypeLut::getEntryNumber(const char* entry_string,
                                             bool case_insensitive) const
{
   ossim_int32 result
      = ossimLookUpTable::getEntryNumber(entry_string, case_insensitive);
   if(result < 0)
   {
      return (ossim_int32)OSSIM_UNIT_UNKNOWN;
   }
   
   return result;
}

ossim_int32 ossimUnitTypeLut::getEntryNumber(const ossimKeywordlist& kwl,
                                             const char* prefix,
                                             bool case_insensitive) const
{
   ossim_int32 result =
      ossimLookUpTable::getEntryNumber(kwl, prefix, case_insensitive);
   if(result < 0)
   {
      return (ossim_int32)OSSIM_UNIT_UNKNOWN;
   }
   return result;
}

ossimKeyword ossimUnitTypeLut::getKeyword() const
{
   return ossimKeyword(ossimKeywordNames::UNITS_KW, "");
}
