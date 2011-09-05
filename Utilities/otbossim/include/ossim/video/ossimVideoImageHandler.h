//*************************************************************************************************
//                          OSSIM -- Open Source Software Image Map
//
// LICENSE: See top level LICENSE.txt file.
//
//  AUTHOR: Oscar Kramer, GeoEye Analytics.
//
//*************************************************************************************************
//  $Id$
#ifndef ossimVideoImageHandler_HEADER
#define ossimVideoImageHandler_HEADER

#include <ossim/video/ossimVideoHandler.h>
#include <ossim/video/ossimVideoGeometry.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/base/ossimRefPtr.h>

//*************************************************************************************************
//  CLASS DESCRIPTION: 
//! OSSIM Image handler for video frames.
//
//! Notes on video handling in OSSIM:
//! - Currently, video is treated as a single file with multiple entries. Each entry corresponds
//!   to a single video frame. The number of entries = the number of frames.
//*************************************************************************************************
class OSSIMDLLEXPORT ossimVideoImageHandler : public ossimImageHandler
{
public:
   virtual bool open() = 0;
   virtual bool isOpen() const = 0;
   virtual void close();
   
   //! Common initialization code for all construction methods.
   virtual void initialize() = 0;

   // Returns list of all frames available in the video.
   virtual void getEntryList(std::vector<ossim_uint32>& entryList) const;

      //! Sets the currently active frame number as count from start of video
   //! Returns false if frame count is outside of allowable range.
   virtual bool setCurrentEntry(ossim_uint32 frame_number);

   virtual ossim_uint32 getCurrentEntry() const;
   virtual ossim_uint32 getNumberOfEntries() const;

   virtual ossim_uint32 getNumberOfLines(ossim_uint32 resLevel = 0) const;
   virtual ossim_uint32 getNumberOfSamples(ossim_uint32 resLevel = 0) const;
   virtual ossim_uint32 getImageTileWidth() const { return getNumberOfSamples(); }
   virtual ossim_uint32 getImageTileHeight() const { return getNumberOfLines(); }

   //! Currently assuming all videos are RGB.
   virtual ossim_uint32    getNumberOfInputBands()  const { return 3; } 
   virtual ossim_uint32    getNumberOfOutputBands() const { return 3; } 
   virtual ossimScalarType getOutputScalarType()    const { return OSSIM_UINT8; }

   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& rect, ossim_uint32 resLevel=0) = 0;

   virtual ossimRefPtr<ossimImageGeometry> getImageGeometry();

   //**********************************************************************************************
   // New methods declared first in this class:
   //**********************************************************************************************
   ossimRefPtr<ossimVideoHandler>  getVideo()         const { return m_video;         }
   ossimRefPtr<ossimVideoGeometry> getVideoGeometry() const { return m_videoGeometry; }

protected:
   ossimVideoImageHandler();

   ossimRefPtr<ossimVideoHandler>   m_video;
   ossimRefPtr<ossimVideoGeometry>  m_videoGeometry;
   int                              m_currentFrameNumber;  //!< This is the currently active entry
   ossimRefPtr<ossimImageData>      m_tile;

   TYPE_DATA
};

#endif
