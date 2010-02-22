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
#include <ossim/projection/ossimBuckeyeSensor.h>
#include <ossim/projection/ossimUtmProjection.h>
#include <ossim/base/ossimException.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimLsrRay.h>
#include <ossim/base/ossimLsrSpace.h>
#include <ossim/base/ossimMatrix4x4.h>
#include <ossim/base/ossimCsvFile.h>
#include <ossim/elevation/ossimElevManager.h>
static ossimTrace traceDebug("ossimBuckeyeSensor:debug");

RTTI_DEF1(ossimBuckeyeSensor, "ossimBuckeyeSensor", ossimSensorModel);

ossimBuckeyeSensor::ossimBuckeyeSensor()
{
   m_lensDistortion = new ossimSmacCallibrationSystem;
   initAdjustableParameters();
}
void ossimBuckeyeSensor::imagingRay(const ossimDpt& image_point,
                                   ossimEcefRay&   image_ray) const
{
   ossimDpt f1 ((image_point) - theRefImgPt);
   f1.x *= m_pixelSize.x;
   f1.y *= -m_pixelSize.y;
   ossimDpt film (f1 - m_principalPoint);
   if (m_lensDistortion.valid())
   {
      ossimDpt filmOut;
      m_lensDistortion->undistort(film, filmOut);
      film = filmOut;
   }
   
   ossimColumnVector3d cam_ray_dir (film.x,
                                    film.y,
                                    -(m_focalLength+computeParameterOffset(6)));
   ossimEcefVector     ecf_ray_dir (m_compositeMatrix*cam_ray_dir);
   ecf_ray_dir = ecf_ray_dir*(1.0/ecf_ray_dir.magnitude());
   
   image_ray.setOrigin(m_ecefPlatformPosition);
   image_ray.setDirection(ecf_ray_dir);
}

void ossimBuckeyeSensor::lineSampleHeightToWorld(const ossimDpt& image_point,
                                                 const double&   heightEllipsoid,
                                                 ossimGpt&       worldPoint) const
{
   if (!insideImage(image_point))
   {
      worldPoint.makeNan();
      worldPoint = extrapolate(image_point, heightEllipsoid);
   }
   else
   {
      //***
      // First establish imaging ray from image point:
      //***
      ossimEcefRay ray;
      imagingRay(image_point, ray);
      ossimEcefPoint Pecf (ray.intersectAboveEarthEllipsoid(heightEllipsoid));
      worldPoint = ossimGpt(Pecf);
   }
   
}

void ossimBuckeyeSensor::lineSampleToWorld(const ossimDpt& image_point,
                                          ossimGpt&       gpt) const
{
   if(image_point.hasNans())
   {
      gpt.makeNan();
      return;
   }
   //***
   // Determine imaging ray and invoke elevation source object's services to
   // intersect ray with terrain model:
   //***
   ossimEcefRay ray;
   imagingRay(image_point, ray);
   ossimElevManager::instance()->intersectRay(ray, gpt);
   
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "image_point = " << image_point << std::endl;
      ossimNotify(ossimNotifyLevel_DEBUG) << "ray = " << ray << std::endl;
      ossimNotify(ossimNotifyLevel_DEBUG) << "gpt = " << gpt << std::endl;
   }
   
   if (traceDebug())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimSensorModel::lineSampleToWorld: returning..." << std::endl;
   return;
   
}

#if 0
void ossimBuckeyeSensor::worldToLineSample(const ossimGpt& world_point,
                                          ossimDpt&       image_point) const
{
#if 0
   if((theBoundGndPolygon.getNumberOfVertices() > 0)&&
      (!theBoundGndPolygon.hasNans()))
   {
      if (!(theBoundGndPolygon.pointWithin(world_point)))
      {
         image_point.makeNan();
         //          image_point = extrapolate(world_point);
         return;
      }         
   }
#endif
   ossimEcefPoint g_ecf(world_point);
   ossimEcefVector ecfRayDir(g_ecf - m_ecefPlatformPosition);
   ossimColumnVector3d camRayDir (m_compositeMatrixInverse*ecfRayDir.data());
   
   
   double scale = -m_focalLength/camRayDir[2];
   ossimDpt film (scale*camRayDir[0], scale*camRayDir[1]);
   
   if (m_lensDistortion.valid())
   {
      ossimDpt filmOut;
      m_lensDistortion->distort(film, filmOut);
      film = filmOut;
   }
   
   ossimDpt f1(film + m_principalPoint);
   ossimDpt p1(f1.x/m_pixelSize.x,
               -f1.y/m_pixelSize.y);
   
   ossimDpt p0 (p1.x + theRefImgPt.x,
                p1.y + theRefImgPt.y);
   
   image_point = p0;
}
#endif
void ossimBuckeyeSensor::updateModel()
{
   ossimGpt gpt;
   ossimGpt wgs84Pt;
   double metersPerDegree = wgs84Pt.metersPerDegree().y;
   double degreePerMeter = 1.0/metersPerDegree;
   double latShift = computeParameterOffset(1)*degreePerMeter;
   double lonShift = computeParameterOffset(0)*degreePerMeter;
   
   gpt = m_platformPosition;
   double height = gpt.height();
   gpt.height(height + computeParameterOffset(5));
   gpt.latd(gpt.latd() + latShift);
   gpt.lond(gpt.lond() + lonShift);
   
   m_ecefPlatformPosition = gpt;
   ossimLsrSpace lsrSpace(m_ecefPlatformPosition, m_yaw+computeParameterOffset(4));
   
   NEWMAT::Matrix platformLsrMatrix = lsrSpace.lsrToEcefRotMatrix();
   NEWMAT::Matrix orientationMatrix = (                                      
                                       ossimMatrix3x3::createRotationXMatrix(m_pitch+computeParameterOffset(3), OSSIM_LEFT_HANDED)
                                       *ossimMatrix3x3::createRotationYMatrix(m_roll+computeParameterOffset(2), OSSIM_LEFT_HANDED)
            //                           ossimMatrix3x3::createRotationYMatrix(m_pitch+computeParameterOffset(2), OSSIM_RIGHT_HANDED)
            //                           *ossimMatrix3x3::createRotationXMatrix(m_roll+computeParameterOffset(3), OSSIM_RIGHT_HANDED)
                                      );
   
   m_compositeMatrix         = platformLsrMatrix*orientationMatrix;
   m_compositeMatrixInverse  = m_compositeMatrix.i();
   
   
   theBoundGndPolygon.resize(4);
   ossim_float64 w = theImageClipRect.width()*2.0;
   ossim_float64 h = theImageClipRect.height()*2.0;
   lineSampleToWorld(theImageClipRect.ul()+ossimDpt(-w, -h), gpt);
   theBoundGndPolygon[0] = gpt;
   lineSampleToWorld(theImageClipRect.ur()+ossimDpt(w, -h), gpt);
   theBoundGndPolygon[1] = gpt;
   lineSampleToWorld(theImageClipRect.lr()+ossimDpt(w, h), gpt);
   theBoundGndPolygon[2] = gpt;
   lineSampleToWorld(theImageClipRect.ll()+ossimDpt(-w, h), gpt);
   theBoundGndPolygon[3] = gpt;
   lineSampleToWorld(theRefImgPt, theRefGndPt);
}

void ossimBuckeyeSensor::initAdjustableParameters()
{
   resizeAdjustableParameterArray(7);
   
   setAdjustableParameter(0, 0.0);
   setParameterDescription(0, "x_offset");
   setParameterUnit(0, "meters");
   
   setAdjustableParameter(1, 0.0);
   setParameterDescription(1, "y_offset");
   setParameterUnit(1, "meters");
   
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
   setParameterUnit(6, "millimeters");
   
   setParameterSigma(0, 10);
   setParameterSigma(1, 10);
   setParameterSigma(2, 1);
   setParameterSigma(3, 1);
   setParameterSigma(4, 5);
   setParameterSigma(5, 100);
   setParameterSigma(6, 50);
}

bool ossimBuckeyeSensor::loadState(const ossimKeywordlist& kwl, const char* prefix)
{
   if(getNumberOfAdjustableParameters() < 1)
   {
      initAdjustableParameters();
   }
   theGSD.makeNan();
   theRefImgPt.makeNan();
   ossimSensorModel::loadState(kwl, prefix);
   if(theRefImgPt.hasNans())
   {
      theRefImgPt = theImageClipRect.midPoint();
   }
   ossimString framemeta_gsti = kwl.find(prefix, "framemeta_gsti");
   ossimString frame_number = kwl.find(prefix, "frame_number");
   ossimString pixel_size = kwl.find(prefix, "pixel_size");
   ossimString principal_point = kwl.find(prefix, "principal_point");
   ossimString focal_length = kwl.find(prefix, "focal_length");
   ossimString smac_radial = kwl.find(prefix, "smac_radial");
   ossimString smac_decent = kwl.find(prefix, "smac_decent");
   ossimString roll;
   ossimString pitch;
   ossimString yaw;
   ossimString platform_position;
   m_roll    = 0;
   m_pitch   = 0;
   m_yaw     = 0;
   if(!framemeta_gsti.empty()&&
      !frame_number.empty())
   {
      ossimCsvFile csv(" \t"); // we will use tab or spaces as seaparator
      if(csv.open(framemeta_gsti))
      {
         if(csv.readHeader())
         {
            ossimRefPtr<ossimCsvFile::Record> record;
            bool foundFrameNumber = false;
            while( ((record = csv.nextRecord()).valid()) && !foundFrameNumber)
            {
               if( (*record)["Frame#"] == frame_number)
               {
                  foundFrameNumber = true;
                  roll = (*record)["Roll(deg)"];
                  pitch = (*record)["Pitch(deg)"];
                  yaw = (*record)["Yaw(deg)"];
                  platform_position = "(" + (*record)["Lat(deg)"] + "," 
                                          + (*record)["Lon(deg)"]+ ","
                                          + (*record)["HAE(m)"] + ",WGE)";
               }
            }
         }
      }
   }
   else
   {
      roll = kwl.find(prefix, "roll"); 
      pitch = kwl.find(prefix, "pitch"); 
      yaw = kwl.find(prefix, "yaw"); 
      platform_position = kwl.find(prefix, "platform_position");
   }
   bool result = (!pixel_size.empty()&&
                  !principal_point.empty()&&
                  !focal_length.empty()&&
                  !platform_position.empty());
   
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
   }
   m_lensDistortion = 0;
   if(!smac_radial.empty()&&
      !smac_decent.empty())
   {
      std::vector<ossimString> radial;
      std::vector<ossimString> decent;
      smac_radial.split(radial, " ");
      smac_decent.split(decent, " ");
      if((radial.size() == 5)&&
         (decent.size() == 4))
      {
         m_lensDistortion = new ossimSmacCallibrationSystem(radial[0].toDouble(),
                                                            radial[1].toDouble(),
                                                            radial[2].toDouble(),
                                                            radial[3].toDouble(),
                                                            radial[4].toDouble(),
                                                            decent[0].toDouble(),
                                                            decent[1].toDouble(),
                                                            decent[2].toDouble(),
                                                            decent[3].toDouble());
      }
   }
   theImageSize = ossimDpt(theImageClipRect.width(),
                           theImageClipRect.height());
   
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
               << "ossimBuckeyeSensor::loadState Caught Exception:\n"
               << e.what() << std::endl;
         }
      }
   }
   
   return result;
}

bool ossimBuckeyeSensor::saveState(ossimKeywordlist& kwl, const char* prefix)const
{
   ossimSensorModel::saveState(kwl, prefix);
   kwl.add(prefix, "roll", ossimString::toString(m_roll), true);
   kwl.add(prefix, "pitch", ossimString::toString(m_pitch), true);
   kwl.add(prefix, "yaw", ossimString::toString(m_yaw), true);
   kwl.add(prefix, "principal_point", m_principalPoint.toString(), true);
   kwl.add(prefix, "pixel_size", m_pixelSize.toString(), true);
   if(m_lensDistortion.valid())
   {
      kwl.add(prefix, "smac_radial",
              ossimString::toString(m_lensDistortion->symmetricRadialDistortionCoefficients()[0], 20) + " " + 
              ossimString::toString(m_lensDistortion->symmetricRadialDistortionCoefficients()[1], 20) + " " +
              ossimString::toString(m_lensDistortion->symmetricRadialDistortionCoefficients()[2], 20) + " " +
              ossimString::toString(m_lensDistortion->symmetricRadialDistortionCoefficients()[3], 20) + " " +
              ossimString::toString(m_lensDistortion->symmetricRadialDistortionCoefficients()[4], 20)
              ,true);
      kwl.add(prefix, "smac_decent",
              ossimString::toString(m_lensDistortion->decenteringDistortionCoefficients()[0], 20) + " " + 
              ossimString::toString(m_lensDistortion->decenteringDistortionCoefficients()[1], 20) + " " +
              ossimString::toString(m_lensDistortion->decenteringDistortionCoefficients()[2], 20) + " " +
              ossimString::toString(m_lensDistortion->decenteringDistortionCoefficients()[3], 20)
              ,true);
   }
   kwl.add(prefix, "platform_postion",m_platformPosition.toString() ,true);
   kwl.add(prefix, "focal_length", ossimString::toString(m_focalLength) ,true);
   
   return true;
}

