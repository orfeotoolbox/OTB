//-------------------------------------------------------------------
// License:  See top level LICENSE.txt file.
//
// Author:  Garrett Potts
//
// Description:
//
// Contains class declaration for ossimNotify
//-------------------------------------------------------------------
//  $Id: ossimNotify.h 21458 2012-08-07 20:40:35Z dburken $
#ifndef ossimNotify_HEADER
#define ossimNotify_HEADER 1

#include <ossim/base/ossimErrorCodes.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimFilename.h>
#include <iostream>
#include <fstream>

/**
  * Notification level enumeration.  This specifies the
  * level of the mesage being written.
  */
enum ossimNotifyLevel
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
enum ossimNotifyFlags
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

/** @brief Returns the log filename if set. */
/* OSSIMDLLEXPORT const char* ossimGetLogFilename(); */
OSSIMDLLEXPORT void ossimGetLogFilename(ossimFilename& logFile);


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
                                    const char *fmtString=0, ...);

/**
 * This is for general warnings and information feedback
 *
 * @param fmtString a C printf() style formatting string used (with the
 * following arguments) to prepare an error message.
 */
OSSIMDLLEXPORT void  ossimSetInfo( const char *className,
                                   const char *fmtString=0, ...);

#endif /* #ifndef ossimNotify_HEADER */
