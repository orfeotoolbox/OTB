//**************************************************************************************************
//                          OSSIM -- Open Source Software Image Map
//
// LICENSE: See top level LICENSE.txt file.
//
// AUTHOR: Oscar Kramer, GeoEye Inc.
//
// DESCRIPTION: 
// 
//**************************************************************************************************
//  $Id: ossimVideoSource.h 2695 2011-06-08 21:12:15Z oscar.kramer $

#ifndef ossimVideoSource_HEADER
#define ossimVideoSource_HEADER

#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimSource.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimIpt.h>

class ossimVideoGeometry;
class ossimVideoImageHandler;

//*************************************************************************************************
//! Pure virtual base class for all video sources -- analogous to ossimImageSource. 
//*************************************************************************************************
class OSSIMDLLEXPORT ossimVideoSource : public ossimSource
{
public:
   
   enum SeekType
   {
      SEEK_NONE     = 0,
      SEEK_RELATIVE = 1,
      SEEK_ABSOLUTE = 2
   };

   //**********************************************************************************************
   // The following are first declared in ossimVideoSource (and derived types).
   // Generally implemented as pass-through calls to this objects video source input.
   //**********************************************************************************************
   //!  For rewinding all active streams to beginning.
   virtual void rewind();

   //! Seeks to the frame at time specified, according to the seekType (relative to active frame or
   //! absolute time from start of video). Returns TRUE if successful. 
   virtual bool seek(ossim_float64 reference_time_sec, SeekType seekType);

   //! Seeks to start of next frame. Returns TRUE if successful.
   virtual bool nextFrame();

   //! Total length of video in seconds.
   virtual ossim_float64 duration() const;

   //! Total number of frames in video.
   virtual ossim_uint32 getNumberOfFrames() const;

   //! Time in seconds from start of video of the current active frame, or NAN if no active frame.
   virtual ossim_float64 currentFrameTime() const;
   
   //! Nominal frames per second.
   virtual ossim_float64 videoFrameRate() const;
   
   //! Utility method for computing frame number from frame time.
   //! @return  Returns frame number corresponding to the time (sec) from start of video or -1 if
   //! time is outside valid window.
   int computeFrameNumber(double seconds_from_start) const;

   //! Utility method for computing frame time from frame number.
   //! @return  Returns frame time (sec) from start of video corresponding to the frame count, 
   //! or NaN if frame count is invalid.
   double computeFrameTime(ossim_uint32 frame_count)  const;

   //! Access to video geometry object.
   virtual const ossimRefPtr<ossimVideoGeometry> getVideoGeometry() const;

   //! Access to current active frame.
   virtual const ossimRefPtr<ossimVideoImageHandler> getCurrentFrame() const;

   //! Nominal size of a frame in pixels
   const ossimIpt& frameSize() const { return m_frameSize; }

   //**********************************************************************************************
   // The following are implementations of ossimConnectableObject virtuals:
   //**********************************************************************************************
   //! Connections only to other video sources are allowed.
   virtual bool canConnectMyInputTo(ossim_int32 index, const ossimConnectableObject* object) const;
   
   //! Presently can only connect into an image chain via an ossimVideoImageSource bridge, or
   //! to another video source for video chains. Returns TRUE if connection possible.
   virtual bool canConnectMyOutputTo(ossim_int32 index, const ossimConnectableObject* object) const;
   
protected:
   //! Default constructor.
   ossimVideoSource();

   ossimIpt m_frameSize;        //!< in pixels

   TYPE_DATA

};
#endif
