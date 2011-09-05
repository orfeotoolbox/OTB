//*************************************************************************************************
// OSSIM -- Open Source Software Image Map
//
// LICENSE: See top level LICENSE.txt file.
//
//  AUTHOR: Oscar Kramer
//
//*************************************************************************************************
//  $Id: ossimVideoGeometry.h 2689 2011-06-08 16:15:26Z oscar.kramer $
#ifndef ossimVideoGeometry_HEADER
#define ossimVideoGeometry_HEADER

#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimReferenced.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/video/ossimVideoSource.h>

class ossimPolygon;
class ossimDrect;
class ossimImageGeometry;
class ossimVideoSource;

//*************************************************************************************************
//  CLASS DESCRIPTION: 
//! Pure virtual base class for all video data. 
//*************************************************************************************************
class OSSIMDLLEXPORT ossimVideoGeometry : public ossimReferenced
{
public:
   //! Provides platform position in lat, lon, hgt given frame time in seconds since start of 
   //! sequence. Returns TRUE if point valid.
   virtual bool sensorPosition(const double& t, ossimGpt& gpt) const = 0;

   //! Provides platform attitude in degrees given frame time in seconds since start of sequence
   //! sequence. Returns TRUE if parameters valid. The coordinate system is defined by the derived 
   //! class (can represent for example Euler rotations about an ECF, ENU or roll, pitch, heading)
   virtual bool sensorAttitude(const double& t, 
                               double& X_deg, 
                               double& Y_deg, 
                               double& Z_deg) const = 0;

   //! Provides focal length in meters given frame time in seconds since start of
   //! sequence. Returns TRUE if parameters valid.
   virtual bool sensorFocalLength(const double& t, double& focal_len_meters) const = 0;

   //! Provides scene center position in lat, lon, hgt given frame time in seconds since start of 
   //! sequence. Returns TRUE if point valid.
   virtual bool frameCenter(const double& t, ossimGpt& centerGpt) const = 0;

   //! Provides scene corner points in lat, lon given frame time in seconds since start of sequence.
   //! Returns TRUE if polygon valid.
   virtual bool frameBoundingPoly(const double& t, ossimPolygon& geo_poly) const = 0;

   //! Provides the bounding rect that covers all frames in the full video.
   //! Returns TRUE if rect valid.
   virtual bool videoBoundingRect(ossimDrect& geo_rect) const = 0;

   //! Access method returns number of pixels in a frame.
   virtual const ossimIpt& frameSize() const = 0;

   //! Provides an image geometry object for frame at specified time in seconds since start of 
   //! sequence. Returns TRUE if point valid.
   virtual ossimRefPtr<ossimImageGeometry> getImageGeometry(const double& t) = 0;

protected:
   ossimVideoGeometry(ossimVideoSource* video) : m_video (video) {}
   ossimVideoGeometry() {}

   ossimRefPtr<ossimVideoSource> m_video;
};
#endif
