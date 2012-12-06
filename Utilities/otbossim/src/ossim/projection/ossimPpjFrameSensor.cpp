//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  Dave Hicks
//
// Description:  PPJ Frame Model
//
//*******************************************************************
//  $Id$
#include <ossim/projection/ossimPpjFrameSensor.h>
#include <ossim/base/ossimException.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimLsrRay.h>
#include <ossim/base/ossimLsrSpace.h>
#include <ossim/base/ossimMatrix4x4.h>
#include <ossim/elevation/ossimElevManager.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/matrix/newmatio.h>

static ossimTrace traceExec ("ossimPpjFrameSensor:exec");
static ossimTrace traceDebug("ossimPpjFrameSensor:debug");

RTTI_DEF1(ossimPpjFrameSensor, "ossimPpjFrameSensor", ossimSensorModel);

enum
{
   PARAM_ADJ_LON_OFFSET   = 0,
   PARAM_ADJ_LAT_OFFSET = 1,
   PARAM_ADJ_ALTITUDE_OFFSET =2, 
   // PARAM_ADJ_ROLL_OFFSET,
   // PARAM_ADJ_PITCH_OFFSET,
   // PARAM_ADJ_YAW_OFFSET,
   PARAM_ADJ_FOCAL_LENGTH_OFFSET=6,
   PARAM_ADJ_COUNT = 7
};

ossimPpjFrameSensor::ossimPpjFrameSensor()
   :
   m_ecef2Cam(),
   m_ecef2CamInverse(),
   m_principalPoint(0.0, 0.0),
   m_focalLengthX(0.0),
   m_focalLengthY(0.0),
   m_focalLength(0.0),
   m_ecefCameraPosition(),
   m_cameraPositionEllipsoid(),
   m_radialK1(0.0),
   m_radialK2(0.0),
   m_radialP1(0.0),
   m_radialP2(0.0),
   m_adjustedCameraPosition(),
   m_adjustedFocalLength(0.0),
   m_averageProjectedHeight(0.0)
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimPpjFrameSensor::ossimPpjFrameSensor DEBUG:" << std::endl;
   }
   initAdjustableParameters();
   theSensorID = "PpjFrame";
   m_ecef2Cam.ReSize(3,3);
   m_ecef2CamInverse.ReSize(3,3);

   std::fill(m_ecef2Cam.Store(), m_ecef2Cam.Store()+9, 0.0);
   std::fill(m_ecef2CamInverse.Store(), m_ecef2CamInverse.Store()+9, 0.0);
   m_ecef2Cam[0][0] = 1.0;
   m_ecef2Cam[1][1] = 1.0;
   m_ecef2Cam[2][2] = 1.0;
   m_ecef2CamInverse[0][0] = 1.0;
   m_ecef2CamInverse[1][1] = 1.0;
   m_ecef2CamInverse[2][2] = 1.0;
}

ossimPpjFrameSensor::ossimPpjFrameSensor(const ossimPpjFrameSensor& src)
   :
   ossimSensorModel(src),
   m_ecef2Cam(src.m_ecef2Cam),
   m_ecef2CamInverse(src.m_ecef2CamInverse),
   m_principalPoint(src.m_principalPoint),
   m_focalLengthX(src.m_focalLengthX),
   m_focalLengthY(src.m_focalLengthY),
   m_focalLength(src.m_focalLength),
   m_ecefCameraPosition(src.m_ecefCameraPosition),
   m_cameraPositionEllipsoid(src.m_cameraPositionEllipsoid),
   m_radialK1(src.m_radialK1),
   m_radialK2(src.m_radialK2),
   m_radialP1(src.m_radialP1),
   m_radialP2(src.m_radialP2),
   m_adjustedCameraPosition(src.m_adjustedCameraPosition),
   m_adjustedFocalLength(src.m_adjustedFocalLength),
   m_averageProjectedHeight(src.m_averageProjectedHeight)
{
}

ossimObject* ossimPpjFrameSensor::dup()const
{
   return new ossimPpjFrameSensor(*this);
}
   
void ossimPpjFrameSensor::setFocalLength(double focX, double focY)
{
   m_focalLengthX = focX;
   m_focalLengthY = focY;
   m_focalLength  = m_focalLengthX;
}

void ossimPpjFrameSensor::setCameraPosition(const ossimGpt& value)
{
   m_cameraPositionEllipsoid = value;
   m_ecefCameraPosition = value;
}
   
void ossimPpjFrameSensor::setPrincipalPoint(const ossimDpt& value)
{
   m_principalPoint = value;
}

void ossimPpjFrameSensor::setecef2CamMatrix(const NEWMAT::Matrix& value)
{
   m_ecef2Cam = value;
   m_ecef2CamInverse = m_ecef2Cam.t();
}

void ossimPpjFrameSensor::setAveragePrjectedHeight(double averageProjectedHeight)
{
   m_averageProjectedHeight = averageProjectedHeight;
}

void ossimPpjFrameSensor::imagingRay(const ossimDpt& imagePoint,
                                     ossimEcefRay& imageRay) const
{
   // Form camera frame LOS vector
   ossimColumnVector3d camLOS(imagePoint.x - m_principalPoint.x,
                              imagePoint.y - m_principalPoint.y,
                              m_adjustedFocalLength);   

   // Rotate to ECF
   ossimColumnVector3d ecfLOS = m_ecef2CamInverse * camLOS;
   imageRay.setOrigin(m_adjustedCameraPosition);
   ossimEcefVector ecfRayDir(ecfLOS);
   imageRay.setDirection(ecfRayDir);

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimPpjFrameSensor::imagingRay DEBUG:\n"
         << "  camLOS = " << camLOS << "\n"
         << "  m_adjustedPlatformPosition = " << m_adjustedCameraPosition << "\n"
         << "  imageRay = " << imageRay << "\n"
         << std::endl;
   }

}

void ossimPpjFrameSensor::lineSampleToWorld(const ossimDpt& imagePoint,
                                                ossimGpt& worldPt) const
{
   ossimEcefRay ray;
   imagingRay(imagePoint, ray);
   ossimElevManager::instance()->intersectRay(ray, worldPt, m_averageProjectedHeight);
}  

void ossimPpjFrameSensor::lineSampleHeightToWorld(const ossimDpt& imagePoint,
                                                  const double&   heightEllipsoid,
                                                        ossimGpt& worldPt) const
{
   ossimEcefRay ray;
   imagingRay(imagePoint, ray);
   double h = (ossim::isnan(heightEllipsoid)||ossim::almostEqual(heightEllipsoid, 0.0))?m_averageProjectedHeight:heightEllipsoid;
   ossimEcefPoint pecf(ray.intersectAboveEarthEllipsoid(h));
   worldPt = ossimGpt(pecf);

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "ossimPpjFrameSensor::lineSampleHeightToWorld DEBUG:" << std::endl;
      ossimNotify(ossimNotifyLevel_DEBUG) << "  imagePoint = " << imagePoint << std::endl;
      ossimNotify(ossimNotifyLevel_DEBUG) << "  heightEllipsoid = " << heightEllipsoid << std::endl;
      ossimNotify(ossimNotifyLevel_DEBUG) << "  ray = " << ray << std::endl;
      ossimNotify(ossimNotifyLevel_DEBUG) << "  worldPt = " << worldPt << std::endl;
   }
}

void ossimPpjFrameSensor::worldToLineSample(const ossimGpt& world_point,
                                             ossimDpt&       image_point) const
{   
   ossimGpt wpt = world_point;
   if(wpt.isHgtNan())
   {
      wpt.height(m_averageProjectedHeight);
   }
   ossimEcefPoint gnd_ecf(wpt);
   ossimEcefPoint cam_ecf(m_adjustedCameraPosition);
   ossimEcefVector ecfRay(gnd_ecf - cam_ecf);
   ossimColumnVector3d camRay(m_ecef2Cam*ecfRay.data());   
      
   double x = m_principalPoint.x + m_adjustedFocalLength*camRay[0]/camRay[2];
   double y = m_principalPoint.y + m_adjustedFocalLength*camRay[1]/camRay[2];

   ossimDpt p(x, y);
    
   image_point = p;
}

void ossimPpjFrameSensor::updateModel()
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimPpjFrameSensor::updateModel DEBUG:" << std::endl;
   }

   double deltap = computeParameterOffset(PARAM_ADJ_LAT_OFFSET)/
      m_cameraPositionEllipsoid.metersPerDegree().y;
   double deltal = computeParameterOffset(PARAM_ADJ_LON_OFFSET)/
      m_cameraPositionEllipsoid.metersPerDegree().x;
   
   m_adjustedCameraPosition = ossimGpt(m_cameraPositionEllipsoid.latd()   + deltap,
                                       m_cameraPositionEllipsoid.lond()   + deltal,
                                       m_cameraPositionEllipsoid.height() + computeParameterOffset(PARAM_ADJ_ALTITUDE_OFFSET));

   // TODO  Need to add correction matrix to accomodate orientation offsets.  It
   //       shouldn't be done in ECF frame.
   // double r = ossim::degreesToRadians(m_roll  + computeParameterOffset(PARAM_ADJ_ROLL_OFFSET));
   // double p = ossim::degreesToRadians(m_pitch + computeParameterOffset(PARAM_ADJ_PITCH_OFFSET) );
   // double y = ossim::degreesToRadians(m_yaw   + computeParameterOffset(PARAM_ADJ_YAW_OFFSET));
   // NEWMAT::Matrix rollM   = ossimMatrix3x3::create(1, 0, 0,
   //                                                 0, cos(r), sin(r),
   //                                                 0, -sin(r), cos(r));
   // NEWMAT::Matrix pitchM  = ossimMatrix3x3::create(cos(p), 0, -sin(p),
   //                                                 0,      1, 0,
   //                                                 sin(p), 0, cos(p));
   // NEWMAT::Matrix yawM    = ossimMatrix3x3::create(cos(y), sin(y), 0,
   //                                                 -sin(y), cos(y), 0,
   //                                                 0,0,1); 
   
   m_adjustedFocalLength = m_focalLength + computeParameterOffset(PARAM_ADJ_FOCAL_LENGTH_OFFSET);
   
     
   try
   {
      computeGsd();
   }
   catch(...)
   {
      
   }
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimPpjFrameSensor::updateModel complete..." << std::endl;
   }
   /*
   ossimGpt gpt;
   lineSampleToWorld(theImageClipRect.ul(),gpt);//+ossimDpt(-w, -h), gpt);
   theBoundGndPolygon[0] = gpt;
   lineSampleToWorld(theImageClipRect.ur(),gpt);//+ossimDpt(w, -h), gpt);
   theBoundGndPolygon[1] = gpt;
   lineSampleToWorld(theImageClipRect.lr(),gpt);//+ossimDpt(w, h), gpt);
   theBoundGndPolygon[2] = gpt;
   lineSampleToWorld(theImageClipRect.ll(),gpt);//+ossimDpt(-w, h), gpt);
   theBoundGndPolygon[3] = gpt;
   */
}

void ossimPpjFrameSensor::initAdjustableParameters()
{
   if (traceExec())
      ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimPpjFrameSensor::initAdjustableParameters: returning..." << std::endl;
   resizeAdjustableParameterArray(PARAM_ADJ_COUNT);
   
   setAdjustableParameter(PARAM_ADJ_LON_OFFSET, 0.0);
   setParameterDescription(PARAM_ADJ_LON_OFFSET, "lon_offset");
   setParameterUnit(PARAM_ADJ_LON_OFFSET, "meters");
   setParameterSigma(PARAM_ADJ_LON_OFFSET, 10);
   
   setAdjustableParameter(PARAM_ADJ_LAT_OFFSET, 0.0);
   setParameterDescription(PARAM_ADJ_LAT_OFFSET, "lat_offset");
   setParameterUnit(PARAM_ADJ_LAT_OFFSET, "meters");
   setParameterSigma(PARAM_ADJ_LAT_OFFSET, 10);
   
   setAdjustableParameter(PARAM_ADJ_ALTITUDE_OFFSET, 0.0);
   setParameterDescription(PARAM_ADJ_ALTITUDE_OFFSET, "altitude_offset");
   setParameterUnit(PARAM_ADJ_ALTITUDE_OFFSET, "meters");
   setParameterSigma(PARAM_ADJ_ALTITUDE_OFFSET, 10);
   
   // TODO  Add these back in when orientation angle offsets are fixed.
   // setAdjustableParameter(PARAM_ADJ_ROLL_OFFSET, 0.0);
   // setParameterDescription(PARAM_ADJ_ROLL_OFFSET, "roll_offset");
   // setParameterUnit(PARAM_ADJ_ROLL_OFFSET, "degrees");
   // setParameterSigma(PARAM_ADJ_ROLL_OFFSET, 10);
   
   // setAdjustableParameter(PARAM_ADJ_PITCH_OFFSET, 0.0);
   // setParameterDescription(PARAM_ADJ_PITCH_OFFSET, "pitch_offset");
   // setParameterUnit(PARAM_ADJ_PITCH_OFFSET, "degrees");
   // setParameterSigma(PARAM_ADJ_PITCH_OFFSET, 10);
   
   // setAdjustableParameter(PARAM_ADJ_YAW_OFFSET, 0.0);
   // setParameterDescription(PARAM_ADJ_YAW_OFFSET, "yaw_offset");
   // setParameterUnit(PARAM_ADJ_YAW_OFFSET, "degrees");
   // setParameterSigma(PARAM_ADJ_YAW_OFFSET, .04);
   
   setAdjustableParameter(PARAM_ADJ_FOCAL_LENGTH_OFFSET, 0.0);
   setParameterDescription(PARAM_ADJ_FOCAL_LENGTH_OFFSET, "focal_length_offset");
   setParameterUnit(PARAM_ADJ_FOCAL_LENGTH_OFFSET, "pixels");
   setParameterSigma(PARAM_ADJ_FOCAL_LENGTH_OFFSET, 20.0);   
}

bool ossimPpjFrameSensor::loadState(const ossimKeywordlist& kwl, const char* prefix)
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimPpjFrameSensor::loadState DEBUG:" << std::endl;
   }

   theGSD.makeNan();
   theRefImgPt.makeNan();
   ossimSensorModel::loadState(kwl, prefix);
   if(getNumberOfAdjustableParameters() < 1)
   {
      initAdjustableParameters();
   }
   ossimString principal_point = kwl.find(prefix, "principal_point");
   ossimString focal_length_x = kwl.find(prefix, "focal_length_x");
   ossimString focal_length_y = kwl.find(prefix, "focal_length_y");
   ossimString number_samples = kwl.find(prefix, "number_samples");
   ossimString number_lines = kwl.find(prefix, "number_lines");
   ossimString ecf_to_cam_row1 = kwl.find(prefix, "ecf_to_cam_row1");
   ossimString ecf_to_cam_row2 = kwl.find(prefix, "ecf_to_cam_row2");
   ossimString ecf_to_cam_row3 = kwl.find(prefix, "ecf_to_cam_row3");
   ossimString platform_position = kwl.find(prefix, "ecef_camera_position");
   ossimString averageProjectedHeight = kwl.find(prefix, "average_projected_height");

   // ossimString roll;
   // ossimString pitch;
   // ossimString yaw;
   // m_roll    = 0;
   // m_pitch   = 0;
   // m_yaw     = 0;
   // roll      = kwl.find(prefix, "roll"); 
   // pitch     = kwl.find(prefix, "pitch"); 
   // yaw       = kwl.find(prefix, "yaw"); 

   bool result = (!principal_point.empty()&&
                  !focal_length_x.empty()&&
                  !platform_position.empty()&&
                  !ecf_to_cam_row1.empty()&&
                  !ecf_to_cam_row2.empty()&&
                  !ecf_to_cam_row3.empty());
   if(!averageProjectedHeight.empty())
   {
      m_averageProjectedHeight = averageProjectedHeight.toDouble();
   }
   if(!number_samples.empty())
   {
      theImageSize = ossimIpt(number_samples.toDouble(), number_lines.toDouble());
      theRefImgPt = ossimDpt(theImageSize.x*.5, theImageSize.y*.5);
      theImageClipRect = ossimDrect(0,0,theImageSize.x-1, theImageSize.y-1);
   }
   if(theImageClipRect.hasNans())
   {
      theImageClipRect = ossimDrect(0,0,theImageSize.x-1,theImageSize.y-1);
   }
   if(theRefImgPt.hasNans())
   {
      theRefImgPt = theImageClipRect.midPoint();
   }
   if(!focal_length_x.empty())
   {
      m_focalLengthX = focal_length_x.toDouble();
      m_focalLength = m_focalLengthX;
   }
   if(!focal_length_y.empty())
   {
      m_focalLengthY = focal_length_y.toDouble();
   }

   std::vector<ossimString> row;
   if(!ecf_to_cam_row1.empty())
   {
      row = ecf_to_cam_row1.explode(" ");
      for (int i=0; i<3; ++i)
         m_ecef2Cam[0][i] = row[i].toDouble();
      row = ecf_to_cam_row2.explode(" ");
      for (int i=0; i<3; ++i)
         m_ecef2Cam[1][i] = row[i].toDouble();
      row = ecf_to_cam_row3.explode(" ");
      for (int i=0; i<3; ++i)
         m_ecef2Cam[2][i] = row[i].toDouble();
      m_ecef2CamInverse = m_ecef2Cam.t();
   }

   // if(!roll.empty())
   // {
   //    m_roll = roll.toDouble();
   // }
   // if(!pitch.empty())
   // {
   //    m_pitch = pitch.toDouble();
   // }
   // if(!yaw.empty())
   // {
   //    m_yaw   = yaw.toDouble();
   // }

   if(!principal_point.empty())
   {
      m_principalPoint.toPoint(principal_point);
   }
   if(!platform_position.empty())
   {
      m_ecefCameraPosition.toPoint(platform_position);
      m_cameraPositionEllipsoid = ossimGpt(m_ecefCameraPosition);
   }

   
   updateModel();
   
   if(theGSD.isNan())
   {
      try
      {
         computeGsd();
      }
      catch (const ossimException& e)
      {
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimPpjFrameSensor::loadState Caught Exception:\n"
            << e.what() << std::endl;
         }
      }
   }
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimPpjFrameSensor::loadState complete..." << std::endl;
   }
   
   return result;
}

bool ossimPpjFrameSensor::saveState(ossimKeywordlist& kwl, const char* prefix)const
{
   ossimSensorModel::saveState(kwl, prefix);
   // kwl.add(prefix, "roll", ossimString::toString(m_roll), true);
   // kwl.add(prefix, "pitch", ossimString::toString(m_pitch), true);
   // kwl.add(prefix, "yaw", ossimString::toString(m_yaw), true);
   kwl.add(prefix, "gsd", theGSD.toString(), true);
   kwl.add(prefix, "principal_point", m_principalPoint.toString(), true);
   kwl.add(prefix, "ecef_camera_position",m_ecefCameraPosition.toString() ,true);
   kwl.add(prefix, "focal_length_x", ossimString::toString(m_focalLengthX) ,true);
   kwl.add(prefix, "focal_length_y", ossimString::toString(m_focalLengthY) ,true);
   kwl.add(prefix, "image_size", theImageSize.toString() ,true);
   kwl.add(prefix, "average_projected_height", m_averageProjectedHeight, true);
   ossimString row1 = ossimString::toString(m_ecef2Cam[0][0]) + " " +
                      ossimString::toString(m_ecef2Cam[0][1]) + " " +
                      ossimString::toString(m_ecef2Cam[0][2]);
   kwl.add(prefix, "ecf_to_cam_row1", row1, true);
   ossimString row2 = ossimString::toString(m_ecef2Cam[1][0]) + " " +
                      ossimString::toString(m_ecef2Cam[1][1]) + " " +
                      ossimString::toString(m_ecef2Cam[1][2]);
   kwl.add(prefix, "ecf_to_cam_row2", row2, true);
   ossimString row3 = ossimString::toString(m_ecef2Cam[2][0]) + " " +
                      ossimString::toString(m_ecef2Cam[2][1]) + " " +
                      ossimString::toString(m_ecef2Cam[2][2]);
   kwl.add(prefix, "ecf_to_cam_row3", row3, true);

   return true;
}
