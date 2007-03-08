//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Description:
// 
// Contains class definition with common error codes and methods to go
// from code to string and string to code.
//
//*************************************************************************
// $Id: ossimErrorCodes.cpp 9963 2006-11-28 21:11:01Z gpotts $

#include <ossim/base/ossimErrorCodes.h>
#include <ossim/base/ossimString.h>

const ossimErrorCode ossimErrorCodes::OSSIM_OK                   = 0;
const ossimErrorCode ossimErrorCodes::OSSIM_NO_ERROR             = 0;
const ossimErrorCode ossimErrorCodes::OSSIM_WARNING              = -1;
const ossimErrorCode ossimErrorCodes::OSSIM_ERROR                = 1;
const ossimErrorCode ossimErrorCodes::OSSIM_ERROR_STD_PARALLEL_1 = 2; 
const ossimErrorCode ossimErrorCodes::OSSIM_LAT_ERROR            = 3;
const ossimErrorCode ossimErrorCodes::OSSIM_LON_ERROR            = 4;
const ossimErrorCode ossimErrorCodes::OSSIM_NORTHING_ERROR       = 5;
const ossimErrorCode ossimErrorCodes::OSSIM_ORIGIN_LAT_ERROR     = 6;
const ossimErrorCode ossimErrorCodes::OSSIM_CENT_MER_ERROR       = 7;
const ossimErrorCode ossimErrorCodes::OSSIM_A_ERROR              = 8;
const ossimErrorCode ossimErrorCodes::OSSIM_B_ERROR              = 9;
const ossimErrorCode ossimErrorCodes::OSSIM_A_LESS_B_ERROR       = 10;
const ossimErrorCode ossimErrorCodes::OSSIM_FIRST_STDP_ERROR     = 11;
const ossimErrorCode ossimErrorCodes::OSSIM_SEC_STDP_ERROR       = 12;
const ossimErrorCode ossimErrorCodes::OSSIM_FIRST_SECOND_ERROR   = 13;
const ossimErrorCode ossimErrorCodes::OSSIM_HEMISPHERE_ERROR     = 14;
const ossimErrorCode ossimErrorCodes::OSSIM_EASTING_ERROR        = 15;
const ossimErrorCode ossimErrorCodes::OSSIM_RADIUS_ERROR         = 16;
const ossimErrorCode ossimErrorCodes::OSSIM_ORIGIN_LON_ERROR     = 17;
const ossimErrorCode ossimErrorCodes::OSSIM_ORIENTATION_ERROR    = 18;
const ossimErrorCode ossimErrorCodes::OSSIM_SCALE_FACTOR_ERROR   = 19;
const ossimErrorCode ossimErrorCodes::OSSIM_ZONE_ERROR           = 20;
const ossimErrorCode ossimErrorCodes::OSSIM_ZONE_OVERRIDE_ERROR  = 21;
const ossimErrorCode ossimErrorCodes::OSSIM_INVALID_FILE_ERROR   = 22;
const ossimErrorCode ossimErrorCodes::OSSIM_OPEN_FILE_ERROR      = 23;
const ossimErrorCode ossimErrorCodes::OSSIM_WRITE_FILE_ERROR     = 24;
const ossimErrorCode ossimErrorCodes::OSSIM_ERROR_UNKNOWN        = OSSIM_INT_NAN;

ossimErrorCodes* ossimErrorCodes::theInstance = NULL;

ossimErrorCodes::ossimErrorCodes()
{
}
   
ossimErrorCodes::ossimErrorCodes(const ossimErrorCodes& /* rhs */ )
{
}

const ossimErrorCodes& ossimErrorCodes::operator=(const ossimErrorCodes&)
{
  return *this;
}

ossimErrorCodes::~ossimErrorCodes()
{
   if (theInstance)
   {
      delete theInstance;
      theInstance = NULL;
   }
}

ossimErrorCodes* ossimErrorCodes::instance()
{
   if (!theInstance)
   {
      theInstance = new ossimErrorCodes();
   }

   return theInstance;
}

ossimErrorCode ossimErrorCodes::getErrorCode(const ossimString& error_string) const
{
   // Upcase...
   ossimString str;
   str = str.upcase(error_string);

   if (str == "OSSIM_OK")
   {
      return ossimErrorCodes::OSSIM_OK;
   }
   else if (str == "OSSIM_NO_ERROR")
   {
      return ossimErrorCodes::OSSIM_NO_ERROR;
   }
   else if (str == "OSSIM_WARNING")
   {
      return ossimErrorCodes::OSSIM_WARNING;
   }
   else if (str == "OSSIM_ERROR")
   {
      return ossimErrorCodes::OSSIM_ERROR;
   }
   else if (str == "OSSIM_ERROR_STD_PARALLEL_1")
   {
      return ossimErrorCodes::OSSIM_ERROR_STD_PARALLEL_1;
   }
   else if (str == "OSSIM_LAT_ERROR")
   {
      return ossimErrorCodes::OSSIM_LAT_ERROR;
   }
   else if (str == "OSSIM_LON_ERROR")
   {
      return ossimErrorCodes::OSSIM_LON_ERROR;
   }
   else if (str == "OSSIM_NORTHING_ERROR")
   {
      return ossimErrorCodes::OSSIM_NORTHING_ERROR;
   }
   else if (str == "OSSIM_ORIGIN_LAT_ERROR")
   {
      return ossimErrorCodes::OSSIM_ORIGIN_LAT_ERROR;
   }
   else if (str == "OSSIM_CENT_MER_ERROR")
   {
      return ossimErrorCodes::OSSIM_CENT_MER_ERROR;
   }
   else if (str == "OSSIM_A_ERROR")
   {
      return ossimErrorCodes::OSSIM_B_ERROR;
   }
   else if (str == "OSSIM_A_LESS_B_ERROR")
   {
      return ossimErrorCodes::OSSIM_A_LESS_B_ERROR;
   }
   else if (str == "OSSIM_FIRST_STDP_ERROR")
   {
      return ossimErrorCodes::OSSIM_FIRST_STDP_ERROR;
   }
   else if (str == "OSSIM_SEC_STDP_ERROR")
   {
      return ossimErrorCodes::OSSIM_SEC_STDP_ERROR;
   }
   else if (str == "OSSIM_FIRST_SECOND_ERROR")
   {
      return ossimErrorCodes::OSSIM_FIRST_SECOND_ERROR;
   }
   else if (str == "OSSIM_HEMISPHERE_ERROR")
   {
      return ossimErrorCodes::OSSIM_HEMISPHERE_ERROR;
   }
   else if (str == "OSSIM_EASTING_ERROR")
   {
      return ossimErrorCodes::OSSIM_EASTING_ERROR;
   }
   else if (str == "OSSIM_RADIUS_ERROR")
   {
      return ossimErrorCodes::OSSIM_RADIUS_ERROR;
   }
   else if (str == "OSSIM_ORIGIN_LON_ERROR")
   {
      return ossimErrorCodes::OSSIM_ORIGIN_LON_ERROR;
   }
   else if (str == "OSSIM_ORIENTATION_ERROR")
   {
      return ossimErrorCodes::OSSIM_ORIENTATION_ERROR;
   }
   else if (str == "OSSIM_SCALE_FACTOR_ERROR")
   {
      return ossimErrorCodes::OSSIM_SCALE_FACTOR_ERROR;
   }
   else if (str == "OSSIM_ZONE_ERROR")
   {
      return ossimErrorCodes::OSSIM_ZONE_ERROR;
   }
   else if (str == "OSSIM_ZONE_OVERRIDE_ERROR")
   {
      return ossimErrorCodes::OSSIM_ZONE_OVERRIDE_ERROR;
   }
   else if (str == "OSSIM_INVALID_FILE_ERROR")
   {
      return ossimErrorCodes::OSSIM_INVALID_FILE_ERROR;
   }
   else if (str == "OSSIM_OPEN_FILE_ERROR")
   {
      return ossimErrorCodes::OSSIM_OPEN_FILE_ERROR;
   }
   else if (str == "OSSIM_WRITE_FILE_ERROR")
   {
      return ossimErrorCodes::OSSIM_WRITE_FILE_ERROR;
   }

   return OSSIM_ERROR_UNKNOWN;
}

ossimString ossimErrorCodes::getErrorString(ossimErrorCode error_code) const
{
   if(error_code == OSSIM_OK)
   {
      // No difference between OSSIM_NO_ERROR and OSSIM_OK.
      return ossimString("OSSIM_OK");
   }
   if(error_code == OSSIM_WARNING)
   {
      return ossimString("OSSIM_WARNING");
   }
   if(error_code == OSSIM_ERROR)
   {
      return ossimString("OSSIM_ERROR");
   }
   if(error_code == OSSIM_ERROR_STD_PARALLEL_1)
   {
      return ossimString("OSSIM_ERROR_STD_PARALLEL_1");
   }
   if(error_code == OSSIM_LAT_ERROR)
   {
      return ossimString("OSSIM_LAT_ERROR");
   }
   if(error_code == OSSIM_LON_ERROR)
   {
      return ossimString("OSSIM_LON_ERROR");
   }
   if(error_code == OSSIM_NORTHING_ERROR)
   {
      return ossimString("OSSIM_NORTHING_ERROR");
   }
   if(error_code == OSSIM_ORIGIN_LAT_ERROR)
   {
      return ossimString("OSSIM_ORIGIN_LAT_ERROR");
   }
   if(error_code == OSSIM_CENT_MER_ERROR)
   {
      return ossimString("OSSIM_CENT_MER_ERROR");
   }
   if(error_code == OSSIM_A_ERROR)
   {
      return ossimString("OSSIM_A_ERROR");
   }
   if(error_code == OSSIM_B_ERROR)
   {
      return ossimString("OSSIM_B_ERROR");
   }
   if(error_code == OSSIM_A_LESS_B_ERROR)
   {
      return ossimString("OSSIM_A_LESS_B_ERROR");
   }
   if(error_code == OSSIM_FIRST_STDP_ERROR)
   {
      return ossimString("OSSIM_FIRST_STDP_ERROR");
   }
   if(error_code == OSSIM_SEC_STDP_ERROR)
   {
      return ossimString("OSSIM_SEC_STDP_ERROR");
   }
   if(error_code == OSSIM_FIRST_SECOND_ERROR)
   {
      return ossimString("OSSIM_FIRST_SECOND_ERROR");
   }
   if(error_code == OSSIM_HEMISPHERE_ERROR)
   {
      return ossimString("OSSIM_HEMISPHERE_ERROR");
   }
   if(error_code == OSSIM_EASTING_ERROR)
   {
      return ossimString("OSSIM_EASTING_ERROR");
   }
   if(error_code == OSSIM_RADIUS_ERROR)
   {
      return ossimString("OSSIM_RADIUS_ERROR");
   }
   if(error_code == OSSIM_ORIGIN_LON_ERROR)
   {
      return ossimString("OSSIM_ORIGIN_LON_ERROR");
   }
   if(error_code == OSSIM_ORIENTATION_ERROR)
   {
      return ossimString("OSSIM_ORIENTATION_ERROR");
   }
   if(error_code == OSSIM_SCALE_FACTOR_ERROR)
   {
      return ossimString("OSSIM_SCALE_FACTOR_ERROR");
   }
   if(error_code == OSSIM_ZONE_ERROR)
   {
      return ossimString("OSSIM_ZONE_ERROR");
   }
   if(error_code == OSSIM_ZONE_OVERRIDE_ERROR)
   {
      return ossimString("OSSIM_ZONE_OVERRIDE_ERROR");
   }
   if(error_code == OSSIM_INVALID_FILE_ERROR)
   {
      return ossimString("OSSIM_INVALID_FILE_ERROR");
   }
   if(error_code == OSSIM_OPEN_FILE_ERROR)
   {
      return ossimString("OSSIM_OPEN_FILE_ERROR");
   }
   if(error_code == OSSIM_WRITE_FILE_ERROR)
   {
      return ossimString("OSSIM_WRITE_FILE_ERROR");
   }

   return ossimString("OSSIM_ERROR_UNKNOWN");
}
