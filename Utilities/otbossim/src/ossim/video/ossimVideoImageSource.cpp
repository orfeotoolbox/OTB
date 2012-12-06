//**************************************************************************************************
//                          OSSIM -- Open Source Software Image Map
//
// LICENSE: See top level LICENSE.txt file.
//
// AUTHOR: Oscar Kramer, SPADAC Inc.
//
// DESCRIPTION: Defines pure virtual base class for individual video frame image handler 
// 
//**************************************************************************************************
//  $Id: ossimVideoImageSource.cpp 2695 2011-06-08 21:12:15Z oscar.kramer $

#include <ossim/video/ossimVideoImageSource.h>
#include <ossim/video/ossimVideoSource.h>
#include <ossim/video/ossimVideoGeometry.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/imaging/ossimImageGeometry.h>

RTTI_DEF1(ossimVideoImageSource, "ossimVideoImageSource", ossimImageSource)

   //*************************************************************************************************
//! Default Constructor
//*************************************************************************************************
ossimVideoImageSource::ossimVideoImageSource()
: m_frameTime (-1.0)
{
}

//*************************************************************************************************
//! Constructor used when accessing a common video object (versus instantiating a new video for
//! each frame.
//*************************************************************************************************
ossimVideoImageSource::ossimVideoImageSource(ossimVideoSource* /* video */, 
                                             const double& frame_time_seconds)
:  m_frameTime (-1.0)
{
   initialize();
   setFrameTime(frame_time_seconds);
}

//*************************************************************************************************
//! Common initialization code for all construction methods.
//*************************************************************************************************
void ossimVideoImageSource::initialize()
{
   ossimVideoSource* video = dynamic_cast<ossimVideoSource*>(getInput());
   if (!video)
      return;

   // Establish the frame rect:
   ossimIpt frame_size (video->frameSize());
   m_frameRect = ossimIrect(0, 0, frame_size.x-1, frame_size.y-1);
  	
   // Initialize the tile data buffer:
   m_tile = ossimImageDataFactory::instance()->create(this, this);
   m_tile->setWidth(frame_size.x);

   m_tile->setHeight(frame_size.y);
   m_tile->initialize();
}

#if 0
/*
GARRETT: this is from when I derived this class from ossimImageHandler instead of ossimImageSource.
I'm leaving these out but it may be convenient to allow OSSIM to consider a video as a multi-entry
file. Give some thought to the merits of deriving from ossimImageHandler. What about reader factory 
and registry? If a user specifies a video file, should the image handler registry pick it up? 
*/
//*************************************************************************************************
// Indirectly sets the frame time given a frame index from the start frame.
//! Returns false if frame time is outside of allowable range.
//*************************************************************************************************
bool ossimVideoImageSource::setCurrentEntry(ossim_uint32 frame_number)
{
   if (!m_video)
      return false;

   ossim_float64 frame_rate = m_video->videoFrameRate();
   ossim_float64 frameTime =  frame_number / frame_rate;
   return setFrameTime(frameTime);
}

//*************************************************************************************************
// Converts frame time to frame index using frame rate:
//*************************************************************************************************
ossim_uint32 ossimVideoImageSource::getCurrentEntry() const
{
   if (!m_video)
      return 0;

   ossim_float64 frame_rate = m_video->videoFrameRate();
   if (frame_rate == 0.0)
      return 0;

   ossim_uint32 current_frame = m_frameTime/frame_rate;
   return current_frame;
}

//*************************************************************************************************
// Determines total number of frames using frame rate and video duration:
//*************************************************************************************************
ossim_uint32 ossimVideoImageSource::getNumberOfEntries() const
{
   if (!m_video)
      return 0;

   ossim_float64 frames_per_sec = m_video->videoFrameRate();
   if (frames_per_sec == 0.0)
      return 0;

   ossim_float64 duration = m_video->duration();

   ossim_uint32 num_frames = (ossim_uint32) (duration * frames_per_sec);
   return num_frames;
}
#endif

//*************************************************************************************************
// Sets the time stamp (in seconds from start of video) for the frame of interest.
//! Returns false if frame time is outside of allowable range.
//*************************************************************************************************
bool ossimVideoImageSource::setFrameTime(const double& frameTime)
{
   ossimVideoSource* video = dynamic_cast<ossimVideoSource*>(getInput());
   if (!video)
      return false;

   // If the requested frame number corresponds with the current active frame, we're done:
   if (frameTime == m_frameTime)
      return true;

   // Check that it is not past the last frame. Use video geometry's duration as it is more accurate
   m_frameTime = frameTime;
   if (m_frameTime > video->duration())
   {
      m_frameTime = 0;
      return false;
   }

   // Seek to that frame:
   video->seek(m_frameTime, ossimVideoSource::SEEK_ABSOLUTE);

   return true;
}

//*************************************************************************************************
// Returns number of lines per video frame.
//*************************************************************************************************
ossim_uint32 ossimVideoImageSource::getNumberOfLines(ossim_uint32 resLevel) const
{
   // Only full-image res level = 0 presently supported:
   if (resLevel != 0)
      return 0;

   return m_frameRect.height();
}

//*************************************************************************************************
// Returns number of lines per video frame.
//*************************************************************************************************
ossim_uint32 ossimVideoImageSource::getNumberOfSamples(ossim_uint32 resLevel) const
{
   // Only full-image res level = 0 presently supported:
   if (resLevel != 0)
      return 0;

   return m_frameRect.width();
}

//*************************************************************************************************
// Returns the populated geometry for this frame
//*************************************************************************************************
ossimRefPtr<ossimImageGeometry> ossimVideoImageSource::getImageGeometry()
{
   ossimVideoSource* video = dynamic_cast<ossimVideoSource*>(getInput());
   if (!video)
      return NULL;

   ossimRefPtr<ossimVideoGeometry> video_geometry = video->getVideoGeometry();
   if (!video_geometry.valid())
      return NULL;

   return video_geometry->getImageGeometry(m_frameTime);
}

//*************************************************************************************************
// From ossimConnectableObject base class. Returns true if object is an ossimVideoSource.
//*************************************************************************************************
bool ossimVideoImageSource::canConnectMyInputTo(ossim_int32 inputIndex,
                                                const ossimConnectableObject* object) const
{
   return ( (!object || PTR_CAST(ossimVideoSource, object)) && (inputIndex == 0) );
}

//*************************************************************************************************
//! Video frames have no overviews, only full-res.
//*************************************************************************************************
void ossimVideoImageSource::getDecimationFactor(ossim_uint32 resLevel, ossimDpt& result) const
{
   result = ossimDpt(0.0, 0.0);
   if (resLevel == 0)
      result = ossimDpt(1.0, 1.0);
}

//*************************************************************************************************
//! Video frames have no overviews, only full-res.
//*************************************************************************************************
void ossimVideoImageSource::getDecimationFactors(std::vector<ossimDpt>& decimations) const
{
   decimations.clear();
   decimations.push_back(ossimDpt(1.0, 1.0));
}
