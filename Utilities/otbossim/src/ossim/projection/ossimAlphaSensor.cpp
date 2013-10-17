//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  Dave Hicks
//
// Description:  Alpha Sensor Base Class
//
//*******************************************************************
//  $Id$
#include <ossim/projection/ossimAlphaSensor.h>
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

static ossimTrace traceExec ("ossimAlphaSensor:exec");
static ossimTrace traceDebug("ossimAlphaSensor:debug");

RTTI_DEF1(ossimAlphaSensor, "ossimAlphaSensor", ossimSensorModel);

enum
{
   PARAM_ADJ_LON_OFFSET   = 0,
   PARAM_ADJ_LAT_OFFSET = 1,
   PARAM_ADJ_ALTITUDE_OFFSET = 2, 
   PARAM_ADJ_ROLL_OFFSET = 3,
   PARAM_ADJ_PITCH_OFFSET = 4,
   PARAM_ADJ_HDG_OFFSET = 5,
   PARAM_ADJ_FOCAL_LENGTH_OFFSET = 6,
   PARAM_ADJ_COUNT = 7
};

ossimAlphaSensor::ossimAlphaSensor()
   :
   m_rollBias(0.0),
   m_pitchBias(0.0),
   m_headingBias(0.0),
   m_fov(0.0),
   m_slitRot(0.0),
   m_focalLength(0.0),
   m_adjustedFocalLength(0.0)
{

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimAlphaSensor::ossimAlphaSensor DEBUG:" << std::endl;
   }
   m_cam2Platform.ReSize(3,3);
   m_cam2Platform = 0.0;
   
   initAdjustableParameters();
}

ossimAlphaSensor::ossimAlphaSensor(const ossimAlphaSensor& src)
   :
   ossimSensorModel(src),
   m_rollBias(src.m_rollBias),
   m_pitchBias(src.m_pitchBias),
   m_headingBias(src.m_headingBias),
   m_fov(src.m_fov),
   m_slitRot(src.m_slitRot),
   m_focalLength(src.m_focalLength),
   m_rollPoly(src.m_rollPoly),
   m_pitchPoly(src.m_pitchPoly),
   m_headingPoly(src.m_headingPoly),
   m_lonPoly(src.m_lonPoly),
   m_latPoly(src.m_latPoly),
   m_altPoly(src.m_altPoly),
   m_scanPoly(src.m_scanPoly),
   m_cam2Platform(src.m_cam2Platform),
   m_adjustedFocalLength(src.m_adjustedFocalLength)
{
}

ossimObject* ossimAlphaSensor::dup()const
{
   return new ossimAlphaSensor(*this);
}

void ossimAlphaSensor::lineSampToWorld(const ossimDpt& imagePoint,
                                             ossimGpt& worldPt) const
{
   ossimEcefRay ray;
   imagingRay(imagePoint, ray);
   ossimElevManager::instance()->intersectRay(ray, worldPt);
}  

void ossimAlphaSensor::lineSampleHeightToWorld(const ossimDpt& imagePoint,
                                               const double&   heightEllipsoid,
                                                     ossimGpt& worldPt) const
{
   ossimEcefRay ray;
   imagingRay(imagePoint, ray);
   ossimEcefPoint pecf(ray.intersectAboveEarthEllipsoid(heightEllipsoid));
   worldPt = ossimGpt(pecf);

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimAlphaSensorHSI::lineSampleHeightToWorld DEBUG:" << std::endl;
      ossimNotify(ossimNotifyLevel_DEBUG) << "  imagePoint = " << imagePoint << std::endl;
      ossimNotify(ossimNotifyLevel_DEBUG) << "  heightEllipsoid = " << heightEllipsoid << std::endl;
      ossimNotify(ossimNotifyLevel_DEBUG) << "  ray = " << ray << std::endl;
      ossimNotify(ossimNotifyLevel_DEBUG) << "  worldPt = " << worldPt << std::endl;
   }
}

void ossimAlphaSensor::setFov(const double fov)
{
   m_fov = fov;
   m_focalLength = (theImageSize.x/2)/tan((m_fov/DEG_PER_RAD)/2);
}

void ossimAlphaSensor::setRollBias(const double rollBias)
{
   m_rollBias = rollBias;
}

void ossimAlphaSensor::setPitchBias(const double pitchBias)
{
   m_pitchBias = pitchBias;
}

void ossimAlphaSensor::setHeadingBias(const double headingBias)
{
   m_headingBias = headingBias;
}

void ossimAlphaSensor::setSlitRot(const double slitRot)
{
   m_slitRot = slitRot;
}

void ossimAlphaSensor::setRollPoly(const std::vector< ossim_float64 > rollPoly)
{
   m_rollPoly = rollPoly;
}

void ossimAlphaSensor::setPitchPoly(const std::vector< ossim_float64 > pitchPoly)
{
   m_pitchPoly = pitchPoly;
}

void ossimAlphaSensor::setHeadingPoly(const std::vector< ossim_float64 > headingPoly)
{
   m_headingPoly = headingPoly;
}

void ossimAlphaSensor::setLonPoly(const std::vector< ossim_float64 > lonPoly)
{
   m_lonPoly = lonPoly;
}

void ossimAlphaSensor::setLatPoly(const std::vector< ossim_float64 > latPoly)
{
   m_latPoly = latPoly;
}

void ossimAlphaSensor::setAltPoly(const std::vector< ossim_float64 > altPoly)
{
   m_altPoly = altPoly;
}

void ossimAlphaSensor::setScanPoly(const std::vector< ossim_float64 > scanPoly)
{
   m_scanPoly = scanPoly;
}

void ossimAlphaSensor::getPositionOrientation(const ossim_float64& line,
                                                    ossimEcefPoint& pos,
                                                    NEWMAT::Matrix& cam2EcfRot) const
{
   // Platform position
   pos = getCameraPosition(line);

   // Local platform orientation
   NEWMAT::Matrix platform2Local = getPlatform2LocalRot(line);

   // Form local<-ECF matrix
   ossimGpt posG = ossimGpt(pos);
   NEWMAT::Matrix local2Ecf = formLLAmat(posG.latr(), posG.lonr(), 0.0);

   // Form full ECF<-camera matrix
   cam2EcfRot = local2Ecf.t() * platform2Local * m_cam2Platform;
}

ossimEcefPoint ossimAlphaSensor::getCameraPosition(const ossim_float64& line) const
{
   // Interpolate position at given line number
   ossim_float64 lat = evalPoly(m_latPoly, line);
   ossim_float64 lon = evalPoly(m_lonPoly, line);
   ossim_float64 alt = evalPoly(m_altPoly, line);
   ossimGpt cameraPositionEllipsoid(lat, lon, alt);

   // Update adjusted position
   double deltap = computeParameterOffset(PARAM_ADJ_LAT_OFFSET)/
      cameraPositionEllipsoid.metersPerDegree().y;
   double deltal = computeParameterOffset(PARAM_ADJ_LON_OFFSET)/
      cameraPositionEllipsoid.metersPerDegree().x;
   
   ossimGpt adjustedCameraPosition = ossimGpt(cameraPositionEllipsoid.latd()   + deltap,
                                              cameraPositionEllipsoid.lond()   + deltal,
                                              cameraPositionEllipsoid.height() + computeParameterOffset(PARAM_ADJ_ALTITUDE_OFFSET));

   ossimEcefPoint pos(adjustedCameraPosition);

   return pos;
}

NEWMAT::Matrix ossimAlphaSensor::getPlatform2LocalRot(const ossim_float64& line) const
{
   // Interpolate orientation at given line number
   ossim_float64 roll = evalPoly(m_rollPoly, line);
   ossim_float64 pitch = evalPoly(m_pitchPoly, line);
   ossim_float64 heading = evalPoly(m_headingPoly, line);

   // Form orientation matrix
   NEWMAT::Matrix rmat = formHPRmat(-roll, -pitch, -heading);

   return rmat;
}

ossim_float64 ossimAlphaSensor::evalPoly(const std::vector<ossim_float64>& polyCoef,
                                         const ossim_float64& line) const
{
   int nCoef = polyCoef.size();

   ossim_float64 result = polyCoef[nCoef-1];

   if (nCoef > 1)
   {
      for(int i=nCoef-2; i >= 0 ; --i)
          result = result * line + polyCoef[i];
   }

   return result;
}

void ossimAlphaSensor::updateModel()
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimAlphaSensor::updateModel DEBUG:" << std::endl;
   }

   // Apply bias corrections
   double r = ossim::degreesToRadians(m_rollBias    + computeParameterOffset(PARAM_ADJ_ROLL_OFFSET));
   double p = ossim::degreesToRadians(m_pitchBias   + computeParameterOffset(PARAM_ADJ_PITCH_OFFSET));
   double h = ossim::degreesToRadians(m_headingBias + computeParameterOffset(PARAM_ADJ_HDG_OFFSET));
   
   // Form bias rotation matrix
   m_cam2Platform = formHPRmat(r, p, h);
   
   // Apply focal length correction
   m_focalLength = (theImageSize.x/2)/tan((m_fov/DEG_PER_RAD)/2);
   m_adjustedFocalLength = m_focalLength + computeParameterOffset(PARAM_ADJ_FOCAL_LENGTH_OFFSET);
}

void ossimAlphaSensor::initAdjustableParameters()
{
   if (traceExec())
      ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimAlphaSensor::initAdjustableParameters: returning..." << std::endl;
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
   
   setAdjustableParameter(PARAM_ADJ_ROLL_OFFSET, 0.0);
   setParameterDescription(PARAM_ADJ_ROLL_OFFSET, "roll_offset");
   setParameterUnit(PARAM_ADJ_ROLL_OFFSET, "degrees");
   setParameterSigma(PARAM_ADJ_ROLL_OFFSET, 10);
   
   setAdjustableParameter(PARAM_ADJ_PITCH_OFFSET, 0.0);
   setParameterDescription(PARAM_ADJ_PITCH_OFFSET, "pitch_offset");
   setParameterUnit(PARAM_ADJ_PITCH_OFFSET, "degrees");
   setParameterSigma(PARAM_ADJ_PITCH_OFFSET, 10);
   
   setAdjustableParameter(PARAM_ADJ_HDG_OFFSET, 0.0);
   setParameterDescription(PARAM_ADJ_HDG_OFFSET, "heading_offset");
   setParameterUnit(PARAM_ADJ_HDG_OFFSET, "degrees");
   setParameterSigma(PARAM_ADJ_HDG_OFFSET, 5);
   
   setAdjustableParameter(PARAM_ADJ_FOCAL_LENGTH_OFFSET, 0.0);
   setParameterDescription(PARAM_ADJ_FOCAL_LENGTH_OFFSET, "focal_length_offset");
   setParameterUnit(PARAM_ADJ_FOCAL_LENGTH_OFFSET, "pixels");
   setParameterSigma(PARAM_ADJ_FOCAL_LENGTH_OFFSET, 20.0);   
}

bool ossimAlphaSensor::loadState(const ossimKeywordlist& kwl, const char* prefix)
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimAlphaSensor::loadState DEBUG:" << std::endl;
   }

   theGSD.makeNan();
   theRefImgPt.makeNan();
   ossimSensorModel::loadState(kwl, prefix);

   ossimString fov = kwl.find(prefix, "fov");
   ossimString number_samples = kwl.find(prefix, "number_samples");
   ossimString number_lines = kwl.find(prefix, "number_lines");
   ossimString rollBias = kwl.find(prefix, "roll_bias"); 
   ossimString pitchBias = kwl.find(prefix, "pitch_bias"); 
   ossimString headingBias = kwl.find(prefix, "heading_bias"); 
   ossimString slitRot = kwl.find(prefix, "slit_rotation"); 

   ossim_uint32 pcount;
   const char* lookup;

   // Roll polynomial
   m_rollPoly.clear();
   pcount = kwl.numberOf("roll_poly_coeff");
   if (pcount>0)
   {
      ossim_uint32 found = 0;
      ossim_uint32 count = 0;
      while ( (found < pcount) && (count < 100) )
      {
         ossimString kw = "roll_poly_coeff";
         kw += ossimString::toString(count);;
         lookup = kwl.find(prefix, kw.c_str());
         if (lookup)
         {
            ++found;
            m_rollPoly.push_back(ossimString::toFloat64(lookup));
         }
         ++count;
      }
   }
   else
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "DEBUG ossimAlphaSensor::loadState() roll_poly_coeff lookup failure..."
            << std::endl;
      }
      return false;
   }

   // Pitch polynomial
   m_pitchPoly.clear();
   pcount = kwl.numberOf("pitch_poly_coeff");
   if (pcount>0)
   {
      ossim_uint32 found = 0;
      ossim_uint32 count = 0;
      while ( (found < pcount) && (count < 100) )
      {
         ossimString kw = "pitch_poly_coeff";
         kw += ossimString::toString(count);;
         lookup = kwl.find(prefix, kw.c_str());
         if (lookup)
         {
            ++found;
            m_pitchPoly.push_back(ossimString::toFloat64(lookup));
         }
         ++count;
      }
   }
   else
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "DEBUG ossimAlphaSensor::loadState() pitch_poly_coeff lookup failure..."
            << std::endl;
      }
      return false;
   }

   // Heading polynomial
   m_headingPoly.clear();
   pcount = kwl.numberOf("heading_poly_coeff");
   if (pcount>0)
   {
      ossim_uint32 found = 0;
      ossim_uint32 count = 0;
      while ( (found < pcount) && (count < 100) )
      {
         ossimString kw = "heading_poly_coeff";
         kw += ossimString::toString(count);;
         lookup = kwl.find(prefix, kw.c_str());
         if (lookup)
         {
            ++found;
            m_headingPoly.push_back(ossimString::toFloat64(lookup));
         }
         ++count;
      }
   }
   else
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "DEBUG ossimAlphaSensor::loadState() heading_poly_coeff lookup failure..."
            << std::endl;
      }
      return false;
   }

   // Latitude polynomial
   m_latPoly.clear();
   pcount = kwl.numberOf("lat_poly_coeff");
   if (pcount>0)
   {
      ossim_uint32 found = 0;
      ossim_uint32 count = 0;
      while ( (found < pcount) && (count < 100) )
      {
         ossimString kw = "lat_poly_coeff";
         kw += ossimString::toString(count);;
         lookup = kwl.find(prefix, kw.c_str());
         if (lookup)
         {
            ++found;
            m_latPoly.push_back(ossimString::toFloat64(lookup));
         }
         ++count;
      }
   }
   else
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "DEBUG ossimAlphaSensor::loadState() lat_poly_coeff lookup failure..."
            << std::endl;
      }
      return false;
   }

   // Longitude polynomial
   m_lonPoly.clear();
   pcount = kwl.numberOf("lon_poly_coeff");
   if (pcount>0)
   {
      ossim_uint32 found = 0;
      ossim_uint32 count = 0;
      while ( (found < pcount) && (count < 100) )
      {
         ossimString kw = "lon_poly_coeff";
         kw += ossimString::toString(count);;
         lookup = kwl.find(prefix, kw.c_str());
         if (lookup)
         {
            ++found;
            m_lonPoly.push_back(ossimString::toFloat64(lookup));
         }
         ++count;
      }
   }
   else
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "DEBUG ossimAlphaSensor::loadState() lon_poly_coeff lookup failure..."
            << std::endl;
      }
      return false;
   }

   // Altitude polynomial
   m_altPoly.clear();
   pcount = kwl.numberOf("alt_poly_coeff");
   if (pcount>0)
   {
      ossim_uint32 found = 0;
      ossim_uint32 count = 0;
      while ( (found < pcount) && (count < 100) )
      {
         ossimString kw = "alt_poly_coeff";
         kw += ossimString::toString(count);;
         lookup = kwl.find(prefix, kw.c_str());
         if (lookup)
         {
            ++found;
            m_altPoly.push_back(ossimString::toFloat64(lookup));
         }
         ++count;
      }
   }
   else
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "DEBUG ossimAlphaSensor::loadState() alt_poly_coeff lookup failure..."
            << std::endl;
      }
      return false;
   }

   // Scan angle polynomial
   m_scanPoly.clear();
   pcount = kwl.numberOf("scan_poly_coeff");
   if (pcount>0)
   {
      ossim_uint32 found = 0;
      ossim_uint32 count = 0;
      while ( (found < pcount) && (count < 100) )
      {
         ossimString kw = "scan_poly_coeff";
         kw += ossimString::toString(count);;
         lookup = kwl.find(prefix, kw.c_str());
         if (lookup)
         {
            ++found;
            m_scanPoly.push_back(ossimString::toFloat64(lookup));
         }
         ++count;
      }
   }
   else
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "DEBUG ossimAlphaSensor::loadState() scan_poly_coeff lookup failure..."
            << std::endl;
      }
      return false;
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
   if(!fov.empty())
   {
      m_fov = fov.toDouble();
   }
   if(!rollBias.empty())
   {
      m_rollBias = rollBias.toDouble();
   }
   if(!pitchBias.empty())
   {
      m_pitchBias = pitchBias.toDouble();
   }
   if(!headingBias.empty())
   {
      m_headingBias = headingBias.toDouble();
   }
   if(!slitRot.empty())
   {
      m_slitRot = slitRot.toDouble();
   }
   
   updateModel();
   
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimAlphaSensor::loadState complete..." << std::endl;
   }
   
   return true;
}

bool ossimAlphaSensor::saveState(ossimKeywordlist& kwl, const char* prefix)const
{
   ossimSensorModel::saveState(kwl, prefix);
   kwl.add(prefix, "roll_bias", ossimString::toString(m_rollBias), true);
   kwl.add(prefix, "pitch_bias", ossimString::toString(m_pitchBias), true);
   kwl.add(prefix, "heading_bias", ossimString::toString(m_headingBias), true);
   kwl.add(prefix, "fov", ossimString::toString(m_fov) ,true);
   kwl.add(prefix, "slit_rotation", ossimString::toString(m_slitRot) ,true);
   kwl.add(prefix, "image_size", theImageSize.toString() ,true);

   ossim_uint32 i;
   for (i = 0; i < m_rollPoly.size(); ++i)
   {
      ossimString kw = "roll_poly_coeff";
      kw += ossimString::toString(i);
      kwl.add(prefix, kw, m_rollPoly[i]);
   }
   for (i = 0; i < m_pitchPoly.size(); ++i)
   {
      ossimString kw = "pitch_poly_coeff";
      kw += ossimString::toString(i);
      kwl.add(prefix, kw, m_pitchPoly[i]);
   }
   for (i = 0; i < m_headingPoly.size(); ++i)
   {
      ossimString kw = "heading_poly_coeff";
      kw += ossimString::toString(i);
      kwl.add(prefix, kw, m_headingPoly[i]);
   }
   for (i = 0; i < m_lonPoly.size(); ++i)
   {
      ossimString kw = "lon_poly_coeff";
      kw += ossimString::toString(i);
      kwl.add(prefix, kw, m_lonPoly[i]);
   }
   for (i = 0; i < m_latPoly.size(); ++i)
   {
      ossimString kw = "lat_poly_coeff";
      kw += ossimString::toString(i);
      kwl.add(prefix, kw, m_latPoly[i]);
   }
   for (i = 0; i < m_altPoly.size(); ++i)
   {
      ossimString kw = "alt_poly_coeff";
      kw += ossimString::toString(i);
      kwl.add(prefix, kw, m_altPoly[i]);
   }
   for (i = 0; i < m_scanPoly.size(); ++i)
   {
      ossimString kw = "scan_poly_coeff";
      kw += ossimString::toString(i);
      kwl.add(prefix, kw, m_scanPoly[i]);
   }

   return true;
}


// Form ARINC 705 standard {heading/pitch/roll} rotation matrix
// Rotates local<-body
NEWMAT::Matrix ossimAlphaSensor::formHPRmat(const ossim_float64& r,
                                            const ossim_float64& p,
                                            const ossim_float64& h)const
{
   ossim_float64 cp = cos(p);
   ossim_float64 sp = sin(p);
   ossim_float64 ch = cos(h);
   ossim_float64 sh = sin(h);
   ossim_float64 cr = cos(r);
   ossim_float64 sr = sin(r);

   NEWMAT::Matrix rollM   = ossimMatrix3x3::create(  1,  0,  0,
                                                     0, cr,-sr,
                                                     0, sr, cr);

   NEWMAT::Matrix pitchM  = ossimMatrix3x3::create( cp,  0, sp,
                                                     0,  1,  0,
                                                   -sp,  0, cp);

   NEWMAT::Matrix hdgM    = ossimMatrix3x3::create( ch,-sh,  0,
                                                    sh, ch,  0,
                                                     0,  0,  1);

   NEWMAT::Matrix body2LocalRot = hdgM * pitchM * rollM;

   return body2LocalRot;
}


// Form local <- ECF rotation matrix
NEWMAT::Matrix ossimAlphaSensor::formLLAmat(const ossim_float64& lat,
                                            const ossim_float64& lon,
                                            const ossim_float64& az)const
{
   ossim_float64 cp = cos(lat);
   ossim_float64 sp = sin(lat);
   ossim_float64 cl = cos(lon);
   ossim_float64 sl = sin(lon);
   ossim_float64 ca = cos(az);
   ossim_float64 sa = sin(az);

   NEWMAT::Matrix ecf2LocalRot(3,3);
   ecf2LocalRot(1,1) = -sl*sa - sp*cl*ca;
   ecf2LocalRot(1,2) =  cl*sa - sp*sl*ca;
   ecf2LocalRot(1,3) =  cp*ca;
   ecf2LocalRot(2,1) =  sl*ca - sp*cl*sa;
   ecf2LocalRot(2,2) = -cl*ca - sp*sl*sa;
   ecf2LocalRot(2,3) =  cp*sa;
   ecf2LocalRot(3,1) =  cp*cl;
   ecf2LocalRot(3,2) =  cp*sl;
   ecf2LocalRot(3,3) =  sp;

   return ecf2LocalRot;
}
