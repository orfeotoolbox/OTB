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
//  $Id: ossimVideoSource.cpp 2695 2011-06-08 21:12:15Z oscar.kramer $

#include <ossim/video/ossimVideoSource.h>
#include <ossim/video/ossimVideoGeometry.h>
#include <ossim/video/ossimVideoImageHandler.h>
#include <ossim/video/ossimVideoImageSource.h>

RTTI_DEF1(ossimVideoSource, "ossimVideoSource", ossimSource)

   //*************************************************************************************************
// Protected constructor. This is a pure virtual base class with protected default constructor
//*************************************************************************************************
ossimVideoSource::ossimVideoSource()
{ }

//*************************************************************************************************
// Rewind command is passed down the chain until a video handler can process it.
//*************************************************************************************************
void ossimVideoSource::rewind()
{
   ossimVideoSource* inter = PTR_CAST(ossimVideoSource, getInput(0));
   if (inter)
      inter->rewind();
}

//*************************************************************************************************
//! Seeks to the frame at time specified, according to the seekType (relative to active frame or
//! absolute time from start of video). Returns TRUE if successful. 
//*************************************************************************************************
bool ossimVideoSource::seek(ossim_float64 reference_time_sec, SeekType seekType)
{
   ossimVideoSource* inter = PTR_CAST(ossimVideoSource, getInput(0));
   if (inter)
      return inter->seek(reference_time_sec, seekType);
   
   return false;
}

//*************************************************************************************************
//! Seeks to start of next frame. Returns TRUE if successful.
//*************************************************************************************************
bool ossimVideoSource::nextFrame()
{
   ossimVideoSource* inter = PTR_CAST(ossimVideoSource, getInput(0));
   if (inter)
      return inter->nextFrame();

   return false;
}

//*************************************************************************************************
//! Total length of video in seconds.
//*************************************************************************************************
ossim_float64 ossimVideoSource::duration() const 
{
   ossimVideoSource* inter = PTR_CAST(ossimVideoSource, getInput(0));
   if (inter)
      return inter->duration();

   return 0.0;
}

//*************************************************************************************************
//! Time in seconds from start of video of the current active frame, or NAN if no active frame.
//*************************************************************************************************
ossim_float64 ossimVideoSource::currentFrameTime() const
{
   ossimVideoSource* inter = PTR_CAST(ossimVideoSource, getInput(0));
   if (inter)
      return inter->currentFrameTime();

   return 0.0;
}

//*************************************************************************************************
//! Nominal frames per second.
//*************************************************************************************************
ossim_float64 ossimVideoSource::videoFrameRate() const
{
   ossimVideoSource* inter = PTR_CAST(ossimVideoSource, getInput(0));
   if (inter)
      return inter->videoFrameRate();

   return 0.0;
}

//*************************************************************************************************
//! Access to video geometry object.
//*************************************************************************************************
const ossimRefPtr<ossimVideoGeometry> ossimVideoSource::getVideoGeometry() const
{
   ossimVideoSource* inter = PTR_CAST(ossimVideoSource, getInput(0));
   if (inter)
      return inter->getVideoGeometry();

   return NULL;
}

//*************************************************************************************************
//! Access to current active frame.
//*************************************************************************************************
const ossimRefPtr<ossimVideoImageHandler> ossimVideoSource::getCurrentFrame() const
{
   //STUB
   return NULL;
}

//*************************************************************************************************
//! Total number of frames in video.
//*************************************************************************************************
ossim_uint32 ossimVideoSource::getNumberOfFrames() const
{
   // Base class implementation just computes number of frames from duration:
   ossim_uint32 num_frames = (ossim_uint32) computeFrameNumber(duration());
   return num_frames;
}

//*************************************************************************************************
//! Utility method for computing frame number from frame time.
//*************************************************************************************************
int ossimVideoSource::computeFrameNumber(double seconds_from_start) const
{
   double frame_rate = videoFrameRate();
   if (frame_rate > 0.0)
      return (int) seconds_from_start*frame_rate;
   // else
   return -1;
}

//*************************************************************************************************
//! Utility method for computing frame time from frame number.
//*************************************************************************************************
double ossimVideoSource::computeFrameTime(ossim_uint32 frame_count)  const
{
   double frame_rate = videoFrameRate();
   if (frame_rate > 0.0)
      return frame_count/frame_rate;
   // else
   return ossim::nan();
}

//*************************************************************************************************
//! Connections only to other video sources are allowed.
//*************************************************************************************************
bool ossimVideoSource::canConnectMyInputTo(ossim_int32 index, 
                                           const ossimConnectableObject* object) const
{
   if (object && (index == 0) && PTR_CAST(ossimVideoSource, object))
      return true;
   // else
   return false;
}

//*************************************************************************************************
//! Presently can only connect into an image chain via an ossimVideoImageSource bridge, or
//! to another video source for video chains. Returns TRUE if connection possible.
//*************************************************************************************************
bool ossimVideoSource::canConnectMyOutputTo(ossim_int32 index, 
                                            const ossimConnectableObject* object) const
{
   if (!object || (index != 0))
      return false;

   if (PTR_CAST(ossimVideoSource, object))
      return true;

   if (PTR_CAST(ossimVideoImageSource, object))
      return true;

   if (PTR_CAST(ossimVideoImageHandler, object))
      return true;

   return false;
}
