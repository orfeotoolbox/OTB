//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  Garrett Potts
//
// Description:
//
// 
//*******************************************************************
//  $Id$
#ifndef ossimBuckeyeSensor_HEADER
#define ossimBuckeyeSensor_HEADER
#include "ossimSensorModel.h"
#include "ossimSmacCallibrationSystem.h"
#include "ossimUtmProjection.h"

/**
 * The Buckey sensor has a set of simple orientation paramters.  For this model we are using the
 * orientation paramters found in the MetaData_GSTI.txt file typically found in the directory where the
 * imagery sits.
 *
 * There are 2 forms the class takes for keyword lists.  The first allows one to point
 * to a file containing the frame numbers and all the orientation paramters. You will still
 * need to define the Camera information and callibrartion paramters.  The pixel_size, 
 * focal_length and principal point are all in millimeters. 
 *
 * <pre>
 * EXAMPLE 1:
 *
 * type: ossimBuckeyeSensor
 * framemeta_gsti: /data/buckey/01-31@09/01-31@09/10-39-48/FrameMeta_GSTI.txt
 * frame_number: 10001
 * pixel_size: (0.0068,0.0068)
 * focal_length: 211.0211
 * principal_point: (-0.1853,1.2428)
 * smac_radial: -.2165e-3 .4230e-7 -.1652e-11 .2860e-19 .5690e-26
 * smac_decent: -.1483e-6 .1558e-6 -.1464e-18 .1233e-38
 * rect: 0 0 7239 5432
 *
 *
 * EXAMPLE 2:
 * roll: 0.074405
 * pitch: 2.849851
 * yaw: 269.517649
 * platform_position: (34.927014,69.353684,3757.12,WGE)
 * pixel_size: (0.0068,0.0068)
 * focal_length: 211.0211
 * principal_point: (-0.1853,1.2428)
 * smac_radial: -.2165e-3 .4230e-7 -.1652e-11 .2860e-19 .5690e-26
 * smac_decent: -.1483e-6 .1558e-6 -.1464e-18 .1233e-38
 * rect: 0 0 7239 5432
 */
class OSSIM_DLL ossimBuckeyeSensor : public ossimSensorModel
{
public:
   ossimBuckeyeSensor();
   ossimBuckeyeSensor(const ossimBuckeyeSensor& src)
   :ossimSensorModel(src),
   m_compositeMatrix(src.m_compositeMatrix),
   m_compositeMatrixInverse(src.m_compositeMatrixInverse),
   m_roll(src.m_roll),
   m_pitch(src.m_pitch),
   m_yaw(src.m_yaw),
   m_principalPoint(src.m_principalPoint), // in millimeters
   m_pixelSize(src.m_pixelSize),      // in millimeters
   m_focalLength(src.m_focalLength),    // in millimeters
   m_ecefPlatformPosition(src.m_ecefPlatformPosition),
   m_platformPosition(src.m_platformPosition),
   m_lensDistortion(src.m_lensDistortion)
   {
   }
   virtual ossimObject* dup()const
   {
      return new ossimBuckeyeSensor(*this);
   }
   
   virtual void imagingRay(const ossimDpt& image_point,
                           ossimEcefRay&   image_ray) const;

   virtual void lineSampleHeightToWorld(const ossimDpt& image_point,
                                        const double&   heightEllipsoid,
                                        ossimGpt&       worldPoint) const;
   void lineSampleToWorld(const ossimDpt& image_point,
                          ossimGpt&       gpt) const;
   
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
   
   void setLensDistortion(ossimSmacCallibrationSystem* distortion)
   {
      m_lensDistortion = distortion;
   }
   virtual void initAdjustableParameters();
   virtual bool loadState(const ossimKeywordlist& kwl, const char* prefix=0);
   virtual bool saveState(ossimKeywordlist& kwl, const char* prefix=0)const;
   
protected:
   NEWMAT::Matrix m_compositeMatrix;
   NEWMAT::Matrix m_compositeMatrixInverse;
   double         m_roll;
   double         m_pitch;
   double         m_yaw;
   ossimDpt       m_principalPoint; // in millimeters
   ossimDpt       m_pixelSize;      // in millimeters
   double         m_focalLength;    // in millimeters
   ossimEcefPoint m_ecefPlatformPosition;
   ossimGpt       m_platformPosition;
   
   ossimRefPtr<ossimSmacCallibrationSystem> m_lensDistortion;
   
TYPE_DATA
};

#endif
