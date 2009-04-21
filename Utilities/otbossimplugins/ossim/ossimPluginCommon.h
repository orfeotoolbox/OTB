//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: Common code for this plugin.
// 
//----------------------------------------------------------------------------
// $Id$

#ifndef ossimPluginCommon_HEADER
#define ossimPluginCommon_HEADER 1

#include <string>

class CivilDateTime;

namespace ossim
{
   /**
    * @note This code was moved out of ossimRadarSat2Model.
    * 
    * @brief Converts date from radarsat format to model format.
    *
    * From:  yyyy-mm-ddThh:mm:ss(.s+)zzzz (where zzzz is the timeZone).
    * To: yyyymmddhhmmssddd
    *
    * @param utcString Input date string.
    *
    * @param outputDate Output date string.
    *
    * @return true on success false on error.
    */
   bool UtcDateTimeStringToCivilDate(const std::string& utcString,
                                     CivilDateTime& outputDate);

} // matches: namespace ossim

#endif /* matches: #ifndef ossimPluginCommon_HEADER */
