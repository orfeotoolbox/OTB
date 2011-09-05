//*************************************************************************************************
// OSSIM -- Open Source Software Image Map
//
// LICENSE: See top level LICENSE.txt file.
//
//  AUTHOR: Oscar Kramer
//
//*************************************************************************************************
//  $Id: ossimVideoImageSource.h 2695 2011-06-08 21:12:15Z oscar.kramer $
#ifndef ossimVideoImageSource_HEADER
#define ossimVideoImageSource_HEADER

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/imaging/ossimImageSource.h>
#include <ossim/imaging/ossimImageSource.h>

class ossimVideoSource;
class ossimVideoGeometry;

//*************************************************************************************************
//  CLASS DESCRIPTION: 
//! Contains class declaration for ossimVideoImageSource. This is the ossimImageSource-derived
//! class representing a single frame in a video stream.
//*************************************************************************************************
class OSSIMDLLEXPORT ossimVideoImageSource : public ossimImageSource
{
public:
   //! Sets the time stamp (in seconds from start of video) for the frame of interest. This will 
   //! necessarily change the image geometry. 
   //! Returns false if frame time is outside of allowable range.
   virtual bool setFrameTime(const double& t);

   //! Changes the frame to the new index (frame count from start of video). This will necessarily
   //! change the image geometry. 
   //! Returns false if frame count is outside of allowable range.
   //virtual bool setFrameNumber(ossim_uint32 frame_number);

   virtual ossim_uint32 getNumberOfLines(ossim_uint32 resLevel = 0) const;
   virtual ossim_uint32 getNumberOfSamples(ossim_uint32 resLevel = 0) const;
   virtual ossim_uint32 getImageTileWidth() const { return getNumberOfSamples(); }
   virtual ossim_uint32 getImageTileHeight() const { return getNumberOfLines(); }

   //! Currently assuming all videos are 8-bit RGB.
   virtual ossim_uint32    getNumberOfInputBands()  const { return 3; } 
   virtual ossim_uint32    getNumberOfOutputBands() const { return 3; } 
   virtual ossimScalarType getOutputScalarType()    const { return OSSIM_UINT8; }

   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& rect, ossim_uint32 resLevel=0)=0;

   virtual ossimRefPtr<ossimImageGeometry> getImageGeometry();

   //! From ossimConnectableObject base class. Returns true if object is an ossimVideoSource.
   virtual bool canConnectMyInputTo(ossim_int32 index, const ossimConnectableObject* object) const;

   //! Video frames have no overviews, only full-res.
   virtual ossim_uint32 getNumberOfDecimationLevels() const { return 1; }
   virtual void getDecimationFactor(ossim_uint32 resLevel, ossimDpt& result) const;
   virtual void getDecimationFactors(std::vector<ossimDpt>& decimations) const;

protected:
   //! Constructor used when accessing a common video object (versus instantiating a new video for
   //! each frame.
   ossimVideoImageSource(ossimVideoSource* video, 
                          const double& frame_time_seconds = 0);

   ossimVideoImageSource();
   virtual void initialize();

   double                        m_frameTime;  //!< seconds from start of video
   ossimIrect                    m_frameRect; //!< Always (0,0) based
   ossimRefPtr<ossimImageData>   m_tile;

   TYPE_DATA
};

#endif
