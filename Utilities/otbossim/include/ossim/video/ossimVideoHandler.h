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
//  $Id: ossimVideoHandler.h 2650 2011-05-27 16:53:41Z oscar.kramer $

#ifndef ossimVideoHandler_HEADER
#define ossimVideoHandler_HEADER

#include <ossim/base/ossimRefPtr.h>
#include <ossim/video/ossimVideoSource.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/video/ossimVideoGeometry.h>

//*************************************************************************************************
//! Pure virtual base class for all video file handlers -- analogous to ossimImageHandler. 
//*************************************************************************************************
class OSSIMDLLEXPORT ossimVideoHandler : public ossimVideoSource
{
public:
   
   //*************************************************************************************************
   // The following are first declared here in ossimVideoHandler
   //*************************************************************************************************
   //! Returns TRUE if video file named is successfully opened.
   virtual bool open(const ossimFilename& videoFile);
   
   //! Returns TRUE if video file at m_videoFilename is successfully opened.
   virtual bool open() = 0;

   //! Closes video file.
   virtual void close() = 0;

   //! Returns TRUE if video file is open.
   virtual bool isOpen() const = 0;

   //! Returns filename of previously opened video file.
   const ossimFilename& filename() const { return m_videoFilename; }

   //**********************************************************************************************
   // The following are implementations of ossimVideoSource virtuals:
   //**********************************************************************************************
   //!  For rewinding all active streams to beginning.
   virtual void rewind();

   //! Seeks to the frame at time specified, according to the seekType (relative to active frame or
   //! absolute time from start of video).
   virtual bool seek(ossim_float64 reference_time_sec, SeekType seekType) = 0;

   //! Access to video geometry object.
   virtual const ossimRefPtr<ossimVideoGeometry> getVideoGeometry() const {return m_videoGeometry;}

   //! Time in seconds from start of video of the current active frame, or NAN if no active frame.
   ossim_float64 currentFrameTime() const { return m_currentFrameTime; }
   
   //! Nominal frames per second.
   ossim_float64 videoFrameRate() const { return m_frameRate; };
   
   //! Total length of video in seconds.
   ossim_float64 duration() const { return m_videoDuration; }

   //**********************************************************************************************
   // The following are implementations of ossimConnectableObject virtuals:
   //**********************************************************************************************
   //! This object does not accept inputs.
   virtual bool canConnectMyInputTo(ossim_int32, const ossimConnectableObject*) const {return false;}

protected:
   //! Default constructor.
   ossimVideoHandler();

   ossimFilename                       m_videoFilename;
   ossimRefPtr<ossimVideoGeometry>     m_videoGeometry;
   ossim_float64                       m_currentFrameTime; //!< in seconds from start of video
   mutable ossim_float64               m_videoDuration;    //!< in seconds
   ossim_float64                       m_frameRate;        //!< in frames per second

   TYPE_DATA
};
#endif
