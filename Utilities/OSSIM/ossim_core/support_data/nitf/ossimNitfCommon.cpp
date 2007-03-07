//----------------------------------------------------------------------------
// Copyright (c) 2005, David Burken, all rights reserved.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: Utility class for global nitf methods.
//
//----------------------------------------------------------------------------
// $Id: ossimNitfCommon.cpp,v 1.4 2005/10/17 19:21:55 gpotts Exp $

#include <sstream>
#include <iomanip>

#include <support_data/nitf/ossimNitfCommon.h>
#include <base/common/ossimTrace.h>
#include <base/context/ossimNotifyContext.h>
#include <base/data_types/ossimString.h>
#include <base/data_types/ossimDpt.h>

static const ossimTrace traceDebug(ossimString("ossimNitfCommon:debug"));

                                   
ossimNitfCommon::ossimNitfCommon(){}

ossimNitfCommon::~ossimNitfCommon(){}

ossimString ossimNitfCommon::convertToScientificString(
   const ossim_float64& aValue,
   ossim_uint32 size)
{
   // Precision cannot hit 0 for this to work...
   if ( ((aValue < 0.0) && (size < 8)) ||
        ((aValue >= 0.0) && (size < 7)) )
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimNitfCommon::convertToScientificString DEBUG:"
            << "\nsize range error!"
            << std::endl;
      }
      return ossimString();
   }

   //--
   // Set the precision to account for size with 1.xxxxxE+01
   //---
   ossim_uint32 precision = size - 6;
   
   if (aValue < 0.0)
   {
      --precision;
   }
   
   std::ostringstream s1;
   s1 << std::setiosflags(std::ios_base::scientific|std::ios_base::internal)
      << std::setfill('0')
      // << setw(size)
      << std::setprecision(precision)
      << aValue;
   
   ossimString result = s1.str();

   // Upcase the "e".
   result.upcase();
   
   if (traceDebug())
   {
      if (result.size() != size)
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimNitfCommon::convertToScientificString DEBUG:"
            << "\nIncorrect output size!"
            << "\nValue:  " << aValue
            << "\nString: " << result
            << std::endl;
      }
   }
   return result;
}

ossimString ossimNitfCommon::convertToDoubleString(
   const ossim_float64& aValue,
   ossim_uint32 precision,
   ossim_uint32 size)
{
   ossim_uint32 width = size;
   
   std::ostringstream s1;
   s1 << std::setiosflags(std::ios_base::fixed|
                     std::ios_base::internal)
      << std::setprecision(precision)
      << std::setfill('0')
      << std::setw(width)
      << aValue;
   
   ossimString result = s1.str();

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimNitfCommon::convertToDoubleString DEBUG:"
         << "\nresult: " << result << std::endl;
      
      if (result.size() != size)
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimNitfCommon::convertToDoubleString DEBUG:"
            << "\nIncorrect output size!"
            << std::endl;
      }
   }
   return result;
}

ossimString ossimNitfCommon::convertToUIntString(ossim_uint32 aValue,
                                                ossim_uint32 size)
{
   ossim_uint32 width = size;
   
   std::ostringstream s1;
   s1 << std::setiosflags(std::ios_base::fixed|
                     std::ios_base::internal)
      << std::setfill('0')
      << std::setw(width)
      << aValue;
   
   ossimString result = s1.str();

   if (traceDebug())
   {
      if (result.size() != size)
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimNitfCommon::convertToUIntString DEBUG:"
            << "\nIncorrect output size!"
            << "\nValue:  " << aValue
            << "\nString: " << result
            << std::endl;
      }
   }
   return result;   
}

ossimString ossimNitfCommon::convertToIntString(ossim_int32 aValue,
                                                ossim_uint32 size)
{
   ossim_uint32 width = size;
   
   std::ostringstream s1;
   s1 << std::setiosflags(std::ios_base::fixed|
                     std::ios_base::internal)
      << std::setfill('0')
      << std::setw(width)
      << aValue;
   
   ossimString result = s1.str();

   if (traceDebug())
   {
      if (result.size() != size)
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimNitfCommon::convertToIntString DEBUG:"
            << "\nIncorrect output size!"
            << "\nValue:  " << aValue
            << "\nString: " << result
            << std::endl;
      }
   }
   return result;   
}

ossimString ossimNitfCommon::convertToDdLatLonLocString(const ossimDpt& pt,
                                                        ossim_uint32 precision)
{
   ossimString lat;
   ossimString lon;
   ossim_uint32 latSize = precision+4; // size = precision + "-33."
   ossim_uint32 lonSize = precision+5; // size = precision + "-122."
   
   if (pt.y >= 0.0)
   {
      lat = "+";
      --latSize;
   }
   if (pt.x >= 0.0)
   {
      lon = "+";
      --lonSize;
   }
   lat += convertToDoubleString(pt.y,
                                precision,
                                latSize);
   lon += convertToDoubleString(pt.x,
                                precision,
                                lonSize);
   ossimString result = lat+lon;

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimNitfCommon::convertToDdLatLonLocString DEBUG:"
         << "\nresult: " << result
         << std::endl;
   }

   return result;
}
