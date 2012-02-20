//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  Garrett Potts
//
// Description:
//
// Sonoma
//*******************************************************************
//  $Id$
#include <ossim/projection/ossimIpodSensor.h>
#include <ossim/projection/ossimUtmProjection.h>
#include <ossim/base/ossimException.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimLsrRay.h>
#include <ossim/base/ossimLsrSpace.h>
#include <ossim/base/ossimMatrix4x4.h>
#include <ossim/base/ossimCsvFile.h>
#include <ossim/elevation/ossimElevManager.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/matrix/newmatio.h>

static ossimTrace traceDebug("ossimIpodSensor:debug");

RTTI_DEF1(ossimIpodSensor, "ossimIpodSensor", ossimSensorModel);

ossimIpodSensor::ossimIpodSensor()
{
   initAdjustableParameters();
   theSensorID = "Ipod";
}

void ossimIpodSensor::imagingRay(const ossimDpt& image_point,
                                   ossimEcefRay&   image_ray) const
{

   ossimDpt f1 (image_point - theRefImgPt);
   f1.x *= m_pixelSize.x;
   f1.y *= -m_pixelSize.y; 
   ossimDpt film (f1 - m_principalPoint);
   
   // once we have them we can add distortion fixes 
   // here on the film
   //
   
   
   // now orient the local film point to the ECEF axes
   // in a north east down lsr space.
   //
   ossimColumnVector3d rc_body(film.y, film.x, m_focalLength + computeParameterOffset(6));  
   ossimColumnVector3d rc_ecef = m_ecef2NedInverse*m_air2Ned*rc_body;
   image_ray.setOrigin(m_adjustedPlatformPosition);
   ossimEcefVector     ecf_ray_dir (rc_ecef);
   image_ray.setDirection(ecf_ray_dir);

}

void ossimIpodSensor::lineSampleHeightToWorld(const ossimDpt& lineSampPt,
                                     const double&   heightEllipsoid,
                                     ossimGpt&       worldPt) const
{
   if (!insideImage(lineSampPt))
   {
      worldPt.makeNan();
      //       worldPoint = extrapolate(image_point, heightEllipsoid);
   }
   else
   {
      //***
      // First establish imaging ray from image point:
      //***
      ossimEcefRay ray;
      imagingRay(lineSampPt, ray);
      ossimEcefPoint Pecf (ray.intersectAboveEarthEllipsoid(heightEllipsoid));
      worldPt = ossimGpt(Pecf);
   }
}


void ossimIpodSensor::updateModel()
{

   m_adjustedPlatformPosition = ossimGpt(m_platformPositionEllipsoid.latd()   + computeParameterOffset(1),
                                         m_platformPositionEllipsoid.lond()   + computeParameterOffset(0),
                                         m_platformPositionEllipsoid.height() + computeParameterOffset(5));
   
   double r = ossim::degreesToRadians(m_roll  + computeParameterOffset(2));
   double p = ossim::degreesToRadians(m_pitch + computeParameterOffset(3) );
   double y = ossim::degreesToRadians(m_yaw   + computeParameterOffset(4));
   NEWMAT::Matrix rollM   = ossimMatrix3x3::create(1, 0, 0,
                                                    0, cos(-r), sin(-r),
                                                    0, -sin(-r), cos(-r));
   NEWMAT::Matrix pitchM  = ossimMatrix3x3::create(cos(-p), 0, -sin(-p),
                                                    0,      1, 0,
                                                    sin(-p), 0, cos(-p));
   NEWMAT::Matrix yawM    = ossimMatrix3x3::create(cos(-y), sin(-y), 0,
                                                -sin(-y), cos(-y), 0,
                                                0,0,1); 


   

   NEWMAT::Matrix camAM   = ossimMatrix3x3::create(0,1,0,-1,0,0,0,0,1); //90 deg rotation @ 3- for camera orientation on copter

   NEWMAT::Matrix camM   = ossimMatrix3x3::create(-1,0,0,0,-1,0,0,0,1); //180 deg rotation @ 3- for corner alignment (mirroring)

   m_air2Ned = camM*rollM*pitchM*yawM*camAM;
   
   double sin_lat = ossim::sind(m_adjustedPlatformPosition.latd());
   double cos_lat = ossim::cosd(m_adjustedPlatformPosition.latd());
   double sin_lon = ossim::sind(m_adjustedPlatformPosition.lond());
   double cos_lon = ossim::cosd(m_adjustedPlatformPosition.lond());
   m_ecef2Ned = (ossimMatrix3x3::create(0,0,1,0,1,0,-1,0,0)*
                 ossimMatrix3x3::create(cos_lat, 0, sin_lat, 0,1,0, -sin_lat,0,cos_lat)*
                 ossimMatrix3x3::create(cos_lon,sin_lon,0, -sin_lon, cos_lon, 0, 0, 0, 1));
   m_ecef2NedInverse = m_ecef2Ned.t();
   double adjustedFocal = m_focalLength + computeParameterOffset(6);
   m_fovIntrack    = (atan((theImageSize.y*theRefImgPt.y)/adjustedFocal));
   m_fovCrossTrack = (atan((theImageSize.x*theRefImgPt.x)/adjustedFocal));
   
   
   lineSampleToWorld(theRefImgPt, theRefGndPt);
   
   theBoundGndPolygon.resize(4);
   ossim_float64 w = theImageClipRect.width()*2.0;
   ossim_float64 h = theImageClipRect.height()*2.0;
   ossimGpt gpt;
   
   lineSampleToWorld(theImageClipRect.ul()+ossimDpt(-w, -h), gpt);
   theBoundGndPolygon[0] = gpt;
   lineSampleToWorld(theImageClipRect.ur()+ossimDpt(w, -h), gpt);
   theBoundGndPolygon[1] = gpt;
   lineSampleToWorld(theImageClipRect.lr()+ossimDpt(w, h), gpt);
   theBoundGndPolygon[2] = gpt;
   lineSampleToWorld(theImageClipRect.ll()+ossimDpt(-w, h), gpt);
   theBoundGndPolygon[3] = gpt;
   try{
   	computeGsd();
   }
   catch(...)
   {
   }

}

void ossimIpodSensor::initAdjustableParameters()
{
   resizeAdjustableParameterArray(7);
   
   setAdjustableParameter(0, 0.0);
   setParameterDescription(0, "lon_offset");
   setParameterUnit(0, "degrees");
   
   setAdjustableParameter(1, 0.0);
   setParameterDescription(1, "lat_offset");
   setParameterUnit(1, "degrees");
   
   setAdjustableParameter(2, 0.0);
   setParameterDescription(2, "roll");
   setParameterUnit(2, "degrees");
   
   setAdjustableParameter(3, 0.0);
   setParameterDescription(3, "pitch");
   setParameterUnit(3, "degrees");
   
   setAdjustableParameter(4, 0.0);
   setParameterDescription(4, "yaw");
   setParameterUnit(4, "degrees");
   
   setAdjustableParameter(5, 0.0);
   setParameterDescription(5, "Altitude delta");
   setParameterUnit(5, "meters");
   
   setAdjustableParameter(6, 0.0);
   setParameterDescription(6, "focal length delta");
   setParameterUnit(6, "meters");
   
   setParameterSigma(0, .001);
   setParameterSigma(1, .001);
   setParameterSigma(2, 10);
   setParameterSigma(3, 10);
   setParameterSigma(4, 10);
   setParameterSigma(5, 100);
   setParameterSigma(6, .04);
   
}

bool ossimIpodSensor::loadState(const ossimKeywordlist& kwl, const char* prefix)
{
   if(getNumberOfAdjustableParameters() < 1)
   {
      initAdjustableParameters();
   }
   theGSD.makeNan();
   theRefImgPt.makeNan();
   ossimSensorModel::loadState(kwl, prefix);
   ossimString image_size = kwl.find(prefix, "image_size");
   ossimString pixel_size = kwl.find(prefix, "pixel_size");
   ossimString principal_point = kwl.find(prefix, "principal_point");
   ossimString focal_length = kwl.find(prefix, "focal_length");
   ossimString roll;
   ossimString pitch;
   ossimString yaw;
   ossimString platform_position;
   m_roll    = 0;
   m_pitch   = 0;
   m_yaw     = 0;
   roll      = kwl.find(prefix, "roll"); 
   pitch     = kwl.find(prefix, "pitch"); 
   yaw       = kwl.find(prefix, "yaw"); 
   platform_position = kwl.find(prefix, "platform_position");
   bool result = (!pixel_size.empty()&&
                  !principal_point.empty()&&
                  !focal_length.empty()&&
                  !platform_position.empty());
   if(!image_size.empty())
   {
      theImageSize.toPoint(image_size);
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
   if(!focal_length.empty())
   {
      m_focalLength = focal_length.toDouble();
   }
   if(!pixel_size.empty())
   {
      m_pixelSize.toPoint(pixel_size);
   }
   if(!roll.empty())
   {
      m_roll = roll.toDouble();
   }
   if(!pitch.empty())
   {
      m_pitch = pitch.toDouble();
   }
   if(!yaw.empty())
   {
      m_yaw   = yaw.toDouble();
   }
   if(!principal_point.empty())
   {
      m_principalPoint.toPoint(principal_point);
   }
   if(!platform_position.empty())
   {
      m_platformPosition.toPoint(platform_position);
      m_platformPositionEllipsoid = m_platformPosition;
      double offset = ossimGeoidManager::instance()->offsetFromEllipsoid(m_platformPosition);
      if(!ossim::isnan(offset))
      {
         m_platformPositionEllipsoid.height(m_platformPosition.height() + offset);
      }
   }
   updateModel();
   
   if(theGSD.isNan())
   {
      try
      {
         // This will set theGSD and theMeanGSD. Method throws ossimException.
         computeGsd();
      }
      catch (const ossimException& e)
      {
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimIpodSensor::loadState Caught Exception:\n"
            << e.what() << std::endl;
         }
      }
   }
   
   return result;
}

bool ossimIpodSensor::saveState(ossimKeywordlist& kwl, const char* prefix)const
{
   ossimSensorModel::saveState(kwl, prefix);
   kwl.add(prefix, "roll", ossimString::toString(m_roll), true);
   kwl.add(prefix, "pitch", ossimString::toString(m_pitch), true);
   kwl.add(prefix, "yaw", ossimString::toString(m_yaw), true);
   kwl.add(prefix, "principal_point", m_principalPoint.toString(), true);
   kwl.add(prefix, "pixel_size", m_pixelSize.toString(), true);
   kwl.add(prefix, "platform_position",m_platformPosition.toString() ,true);
   kwl.add(prefix, "focal_length", ossimString::toString(m_focalLength) ,true);
   kwl.add(prefix, "image_size", theImageSize.toString() ,true);
   
   return true;
}


