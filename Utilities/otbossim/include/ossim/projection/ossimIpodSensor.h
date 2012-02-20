//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  Garrett Potts
//
// Description:
//
// SkyBox
//*******************************************************************
//  $Id$
#ifndef ossimIpodSensor_HEADER
#define ossimIpodSensor_HEADER
#include "ossimSensorModel.h"
#include "ossimUtmProjection.h"
#include <ossim/base/ossimDpt3d.h>
class OSSIM_DLL ossimIpodSensor : public ossimSensorModel
{
public:
   ossimIpodSensor();
   ossimIpodSensor(const ossimIpodSensor& src)
   :ossimSensorModel(src),
   m_air2Ned(src.m_air2Ned),
   m_ecef2Ned(src.m_ecef2Ned),
   m_ecef2NedInverse(src.m_ecef2NedInverse),
   m_roll(src.m_roll),
   m_pitch(src.m_pitch),
   m_yaw(src.m_yaw),
   m_principalPoint(src.m_principalPoint), // in meters
   m_pixelSize(src.m_pixelSize),      // in meters
   m_focalLength(src.m_focalLength),    // in meters
   m_ecefPlatformPosition(src.m_ecefPlatformPosition),
   m_platformPosition(src.m_platformPosition),
   m_platformPositionEllipsoid(src.m_platformPositionEllipsoid),
   m_fovIntrack(src.m_fovIntrack),
   m_fovCrossTrack(src.m_fovCrossTrack),
   m_adjustedPlatformPosition(src.m_adjustedPlatformPosition)
   {
   }
   virtual ossimObject* dup()const
   {
      return new ossimIpodSensor(*this);
   }
   
   virtual void imagingRay(const ossimDpt& image_point,
                           ossimEcefRay&   image_ray) const;
   
   void lineSampleHeightToWorld(const ossimDpt& image_point,
                                const double&   heightEllipsoid,
                                ossimGpt&       worldPoint) const;
      //   virtual void worldToLineSample(const ossimGpt& world_point,
   //                                  ossimDpt&       image_point) const;
   virtual void updateModel();
   
   void setRollPitchYaw(double r, double p, double y)
   {
      m_roll  = r;
      m_pitch = p;
      m_yaw   = y;
   }
   void setFocalLength(double value)
   {
      m_focalLength = value;
   }
   void setPlatformPosition(const ossimGpt& value)
   {
      m_platformPosition     = value;
      m_ecefPlatformPosition = value;
   }
   
   void setPrincipalPoint(const ossimDpt& value)
   {
      m_principalPoint = value;
   }
   void setPixelSize(const ossimDpt& value)
   {
      m_pixelSize = value;
   }
   inline virtual bool useForward()const {return true;} //!ground to image faster (you don't need DEM) //TBC
   
   virtual void initAdjustableParameters();
   virtual bool loadState(const ossimKeywordlist& kwl, const char* prefix=0);
   virtual bool saveState(ossimKeywordlist& kwl, const char* prefix=0)const;
   
protected:
   NEWMAT::Matrix m_air2Ned;
   NEWMAT::Matrix m_ecef2Ned;
   NEWMAT::Matrix m_ecef2NedInverse;

   NEWMAT::Matrix m_composite;
   NEWMAT::Matrix m_compositeInverse;

   double         m_roll;
   double         m_pitch;
   double         m_yaw;
   ossimDpt       m_principalPoint; // in meters
   ossimDpt       m_pixelSize;      // in meters
   double         m_focalLength;    // in meters
   ossimEcefPoint m_ecefPlatformPosition;
   ossimGpt       m_platformPosition;
   ossimGpt       m_platformPositionEllipsoid;
   double         m_fovIntrack;
   double         m_fovCrossTrack;
   
   ossimGpt       m_adjustedPlatformPosition;
   TYPE_DATA
};

#endif

