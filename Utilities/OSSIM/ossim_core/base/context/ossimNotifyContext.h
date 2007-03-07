//-------------------------------------------------------------------
// Copyright (C) 2000 ImageLinks Inc.
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
// Author:  Garrett Potts (gpotts@imagelinks.com)
//
// Description:
//
// Contains class declaration for ossimErrorContext.
//-------------------------------------------------------------------
//  $Id: ossimNotifyContext.h,v 1.6 2004/04/08 13:44:27 gpotts Exp $
#ifndef ossimNotifyContext_HEADER
#define ossimNotifyContext_HEADER
#include <iostream>
#include <fstream>
#include <base/common/ossimErrorCodes.h>
#include <base/common/ossimConstants.h>
#include <base/data_types/ossimFilename.h>

extern "C"
{
/**
  * Notification level enumeration.  This specifies the
  * level of the mesage being written.
  */
OSSIMDLLEXPORT enum ossimNotifyLevel
{
   ossimNotifyLevel_ALWAYS = 0,
   ossimNotifyLevel_FATAL  = 1,
   ossimNotifyLevel_WARN   = 2,
   ossimNotifyLevel_NOTICE = 3,
   ossimNotifyLevel_INFO   = 4,
   ossimNotifyLevel_DEBUG  = 5
};

/**
 * Flags to allow the user to turn off certain notification levels
 * 
 */
OSSIMDLLEXPORT enum ossimNotifyFlags
{
   ossimNotifyFlags_NONE   = 0,
   ossimNotifyFlags_FATAL  = 1,
   ossimNotifyFlags_WARN   = 2,
   ossimNotifyFlags_NOTICE = 4,
   ossimNotifyFlags_INFO   = 8,
   ossimNotifyFlags_DEBUG  = 16,
   ossimNotifyFlags_ALL    = (ossimNotifyFlags_FATAL|
                              ossimNotifyFlags_WARN|
                              ossimNotifyFlags_NOTICE|
                              ossimNotifyFlags_INFO|
                              ossimNotifyFlags_DEBUG)
};

OSSIMDLLEXPORT void ossimSetDefaultNotifyHandlers();

OSSIMDLLEXPORT void ossimSetNotifyStream(std::ostream* outputStream,
                                         ossimNotifyFlags whichLevelsToRedirect=ossimNotifyFlags_ALL);

OSSIMDLLEXPORT std::ostream* ossimGetNotifyStream(ossimNotifyLevel whichLevel);

OSSIMDLLEXPORT bool ossimIsReportingEnabled();

OSSIMDLLEXPORT std::ostream& ossimNotify(ossimNotifyLevel level = ossimNotifyLevel_WARN);

/**
 *
 */
OSSIMDLLEXPORT void ossimSetLogFilename(const ossimFilename& filename);


/**
 *
 */
OSSIMDLLEXPORT void ossimDisableNotify(ossimNotifyFlags notifyLevel=ossimNotifyFlags_ALL);

/**
 *
 */
OSSIMDLLEXPORT void ossimEnableNotify(ossimNotifyFlags notifyLevel=ossimNotifyFlags_ALL);


OSSIMDLLEXPORT void ossimSetNotifyFlag(ossimNotifyFlags notifyFlags);
OSSIMDLLEXPORT void ossimPushNotifyFlags();
OSSIMDLLEXPORT void ossimPopNotifyFlags();
OSSIMDLLEXPORT ossimNotifyFlags ossimGetNotifyFlags();


/**
 * 
 */
OSSIMDLLEXPORT void  ossimSetError( const char *className,
                                    ossim_int32 error,
                                    const char *fmtString, ...);

/**
 * This is for general warnings and information feedback
 *
 * @param fmtString a C printf() style formatting string used (with the
 * following arguments) to prepare an error message.
 */
OSSIMDLLEXPORT void  ossimSetInfo( const char *className,
                                   const char *fmtString, ...);
}
#endif
