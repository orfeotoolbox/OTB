//**************************************************************************************************
//                          OSSIM -- Open Source Software Image Map
//
// LICENSE: See top level LICENSE.txt file.
//
// AUTHOR: Oscar Kramer, SPADAC Inc.
//
// DESCRIPTION: 
// 
//**************************************************************************************************
//  $Id$

#include <ossim/video/ossimVideoImageHandler.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/imaging/ossimImageGeometry.h>

RTTI_DEF1(ossimVideoImageHandler, "ossimVideoImageHandler", ossimImageHandler)

//*************************************************************************************************
// Constructor
//*************************************************************************************************
ossimVideoImageHandler::ossimVideoImageHandler() 
:  m_currentFrameNumber (-1)
{
}

//*************************************************************************************************
// Closes "image" by detaching from the video
//*************************************************************************************************
void ossimVideoImageHandler::close()
{
   m_video = 0;
   m_videoGeometry = 0;
}

//*************************************************************************************************
// Returns list of all frames available in the video.
//*************************************************************************************************
void ossimVideoImageHandler::getEntryList(std::vector<ossim_uint32>& entryList) const
{
   entryList.clear();
   ossim_uint32 num_frames = getNumberOfEntries();
   for (ossim_uint32 i=0; i<num_frames; ++i)
      entryList.push_back(i);
}

//*************************************************************************************************
// Indirectly sets the frame time given a frame index from the start frame.
//! Returns false if frame time is outside of allowable range.
//*************************************************************************************************
bool ossimVideoImageHandler::setCurrentEntry(ossim_uint32 frame_number)
{
   if (!m_video.valid())
      return false;

   ossim_float64 frameTime = m_video->computeFrameTime(frame_number);
   if (m_video->seek(frameTime, ossimVideoSource::SEEK_ABSOLUTE))
   {
      m_currentFrameNumber = frame_number;
      return true;
   }

   return false;
}

//*************************************************************************************************
// Converts frame time to frame index using frame rate:
//*************************************************************************************************
ossim_uint32 ossimVideoImageHandler::getCurrentEntry() const
{
   if (m_currentFrameNumber < 0 )
      return 0;

   return m_currentFrameNumber;
}

//*************************************************************************************************
// Determines total number of frames using frame rate and video duration:
//*************************************************************************************************
ossim_uint32 ossimVideoImageHandler::getNumberOfEntries() const
{
   if (!m_video.valid())
      return 0;

   ossim_uint32 num_frames = (ossim_uint32) m_video->computeFrameNumber(m_video->duration());
   return num_frames;
}

//*************************************************************************************************
// Returns number of lines per video frame.
//*************************************************************************************************
ossim_uint32 ossimVideoImageHandler::getNumberOfLines(ossim_uint32 resLevel) const
{
   // Only full-image res level = 0 presently supported:
   if (!m_video.valid() || (resLevel != 0))
      return 0;

   return m_video->frameSize().line;
}

//*************************************************************************************************
// Returns number of lines per video frame.
//*************************************************************************************************
ossim_uint32 ossimVideoImageHandler::getNumberOfSamples(ossim_uint32 resLevel) const
{
   // Only full-image res level = 0 presently supported:
   if (!m_video.valid() || (resLevel != 0))
      return 0;

   return m_video->frameSize().samp;
}

//*************************************************************************************************
// Returns Image Geometry for current frame.
//*************************************************************************************************
ossimRefPtr<ossimImageGeometry> ossimVideoImageHandler::getImageGeometry()
{
   if (theGeometry.valid())
      return theGeometry;

   // Only full-image res level = 0 presently supported:
   if (!m_video.valid() || !m_videoGeometry.valid())
      return NULL;

   theGeometry = m_videoGeometry->getImageGeometry(m_video->computeFrameTime(m_currentFrameNumber));
   return theGeometry;
}
