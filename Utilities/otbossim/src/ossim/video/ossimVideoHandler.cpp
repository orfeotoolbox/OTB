//**************************************************************************************************
//                          OSSIM -- Open Source Software Image Map
//
// LICENSE: See top level LICENSE.txt file.
//
// AUTHOR: Oscar Kramer, SPADAC Inc.
//
// DESCRIPTION: Interface for all ossim Video types
// 
//**************************************************************************************************
//  $Id: ossimVideoHandler.cpp 2644 2011-05-26 15:20:11Z oscar.kramer $

#include <ossim/video/ossimVideoHandler.h>

RTTI_DEF1(ossimVideoHandler, "ossimVideoHandler", ossimVideoSource)

//*************************************************************************************************
// Protected constructor. This is a pure virtual base class with protected default constructor
//*************************************************************************************************
ossimVideoHandler::ossimVideoHandler()
   :
   m_currentFrameTime (0),
   m_videoDuration (0),
   m_frameRate (ossim::nan())
{ }

//*************************************************************************************************
// Protected constructor. This is a pure virtual base class with protected default constructor
//*************************************************************************************************
void ossimVideoHandler::rewind()
{
   seek(0, SEEK_ABSOLUTE);
}

//*************************************************************************************************
//! Returns TRUE if video file named is successfully opened.
//*************************************************************************************************
bool ossimVideoHandler::open(const ossimFilename& videoFile)
{
   m_videoFilename = videoFile;
   return open();
}

