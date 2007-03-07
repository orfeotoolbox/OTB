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
// $Id: ossimNitfCommon.h,v 1.2 2005/10/12 15:35:07 dburken Exp $
#ifndef ossimNitfCommon_HEADER
#define ossimNitfCommon_HEADER

#include <base/common/ossimConstants.h>

class ossimString;
class ossimDpt;

/** class ossimNitfCommon for global utility methods */   
class OSSIM_DLL ossimNitfCommon
{
public:
   ossimNitfCommon();
   ~ossimNitfCommon();

   /**
    * This method takes a value and a size(or width) and converts it to a
    * scientific notation sting in the format: ±0.999999E±9
    *
    * @note If the value is positive, currently there is no + added to the
    * front.
    *
    * @param aValue Value to convert to string.
    *
    * @param size total size of the string.  This must be at least 7 if
    * aValue is positive and at least 8 if aValue is negative; else,
    * no action is taken and an empty string is returned.
    */
   static ossimString convertToScientificString(const ossim_float64& aValue,
                                                ossim_uint32 size);

   /**
    * This method takes a value and converts to a string of size with
    * requested precision with a '0' fill.
    *
    * Format example: 00925.00
    * 
    * @param aValue Value to convert to string.
    *
    * @param precision The floating point precision.
    *
    * @param size total size of the string.
    */
   static ossimString convertToDoubleString(const ossim_float64& aValue,
                                            ossim_uint32 precision,
                                            ossim_uint32 size);

   /**
    * This method takes a value and converts to a string of size with
    * with a '0' fill.
    *
    * Format example: 00925

    * @param aValue Value to convert to string.
    *
    * @param size total size of the string.
    */
   static ossimString convertToUIntString(ossim_uint32 aValue,
                                          ossim_uint32 size);
   
   /**
    * Just like convertToUIntString only takes a signed value.
    * 
    * This method takes a value and converts to a string of size with
    * with a '0' fill.
    *
    * Format example: 00925

    * @param aValue Value to convert to string.
    *
    * @param size total size of the string.
    */
   static ossimString convertToIntString(ossim_int32 aValue,
                                         ossim_uint32 size);

   /**
    * Converts pt to a decimal degrees latitude longitude location string.
    *
    * Format example: +-dd.dddddd+-ddd.dddddd
    *
    * @param pt Point to convert where pt.x = longitude and pt.y = latitude.
    *
    * @param  precision The floating point precision. This will determine the
    * size of the string.
    */
   static ossimString convertToDdLatLonLocString(const ossimDpt& pt,
                                                 ossim_uint32 precision);

   

}; // End of class ossimNitfCommon.
   
#endif /* #ifndef ossimNitfCommon_HEADER */
