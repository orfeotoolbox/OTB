//*******************************************************************
// Copyright (C) 2002 ImageLinks Inc. 
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
// Author:  David Burken  (dburken@imagelinks.com)
// 
// Description:
// 
// Contains class declaration with common error codes and methods to go
// from code to string and string to code.
//
//*************************************************************************
// $Id: ossimErrorCodes.h,v 1.5 2004/05/17 13:57:22 dburken Exp $

#ifndef ossimErrorCodes_HEADER
#define ossimErrorCodes_HEADER

#include "ossimConstants.h"

class OSSIMDLLEXPORT ossimString;

/*!
 *  class ossimErrorCodes
 *  Contains class declaration with common error codes and methods to go
 *  from code to string and string to code.
 */
class OSSIMDLLEXPORT ossimErrorCodes
{
public:

   ~ossimErrorCodes();

   /*!
    *  Returns a pointer to "theInstance".
    */
   static ossimErrorCodes* instance();

   /*!
    *  Returns the error code from a string.  Returns OSSIM_ERROR_UNKNOWN if
    *  the string does not match.
    */
   ossimErrorCode getErrorCode(const ossimString& error_string) const;

   /*!
    *  Returns the string matching the error code.
    *  Returns "OSSIM_ERROR_UNKNOWN" if code is not valid.
    */
   ossimString getErrorString(ossimErrorCode error_code) const;
   
   static const ossimErrorCode OSSIM_OK;
   static const ossimErrorCode OSSIM_NO_ERROR;
   static const ossimErrorCode OSSIM_WARNING;
   static const ossimErrorCode OSSIM_ERROR;
   static const ossimErrorCode OSSIM_ERROR_STD_PARALLEL_1; 
   static const ossimErrorCode OSSIM_LAT_ERROR;
   static const ossimErrorCode OSSIM_LON_ERROR;
   static const ossimErrorCode OSSIM_NORTHING_ERROR;
   static const ossimErrorCode OSSIM_ORIGIN_LAT_ERROR;
   static const ossimErrorCode OSSIM_CENT_MER_ERROR;
   static const ossimErrorCode OSSIM_A_ERROR;
   static const ossimErrorCode OSSIM_B_ERROR;
   static const ossimErrorCode OSSIM_A_LESS_B_ERROR;
   static const ossimErrorCode OSSIM_FIRST_STDP_ERROR;
   static const ossimErrorCode OSSIM_SEC_STDP_ERROR;
   static const ossimErrorCode OSSIM_FIRST_SECOND_ERROR;
   static const ossimErrorCode OSSIM_HEMISPHERE_ERROR;
   static const ossimErrorCode OSSIM_EASTING_ERROR;
   static const ossimErrorCode OSSIM_RADIUS_ERROR;
   static const ossimErrorCode OSSIM_ORIGIN_LON_ERROR;
   static const ossimErrorCode OSSIM_ORIENTATION_ERROR;
   static const ossimErrorCode OSSIM_SCALE_FACTOR_ERROR;
   static const ossimErrorCode OSSIM_ZONE_ERROR;
   static const ossimErrorCode OSSIM_ZONE_OVERRIDE_ERROR;
   static const ossimErrorCode OSSIM_INVALID_FILE_ERROR;
   static const ossimErrorCode OSSIM_OPEN_FILE_ERROR;
   static const ossimErrorCode OSSIM_WRITE_FILE_ERROR;
   static const ossimErrorCode OSSIM_ERROR_UNKNOWN;
   
protected:
   // Only allow instantiation through the "instance()" method.
   ossimErrorCodes();
   ossimErrorCodes(const ossimErrorCodes& rhs);
   const ossimErrorCodes& operator=(const ossimErrorCodes &rhs);

   static ossimErrorCodes* theInstance;
};

#endif
