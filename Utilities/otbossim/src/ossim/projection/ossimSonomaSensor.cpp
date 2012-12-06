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
#include <ossim/projection/ossimSonomaSensor.h>
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

static ossimTrace traceDebug("ossimSonomaSensor:debug");

RTTI_DEF1(ossimSonomaSensor, "ossimSonomaSensor", ossimSensorModel);

ossimSonomaSensor::ossimSonomaSensor()
{
   m_utmProjection = new ossimUtmProjection;
   theSensorID = "Sonoma";
   m_mount = ossimMatrix4x4::createIdentity();
   m_mountInverse = m_mount;
   initAdjustableParameters();
}

void ossimSonomaSensor::imagingRay(const ossimDpt& /* image_point */,
                                   ossimEcefRay&   /* image_ray */) const
{
#if 0
   ossimColumnVector3d v(image_point.x,image_point.y,1.0);
   ossimColumnVector3d v2(0,0,0);
   v2 = m_compositeMatrix*v2;
   v = m_compositeMatrix*(m_mount*(m_pixelToCamera*v));
   ossimEcefVector vec = ossimEcefPoint(v[0], v[1], v[2]) - ossimEcefPoint(v2[0], v2[1], v2[2]); //ossimEcefPoint(v[0], v[1], v[2]) - m_ecefPlatformPosition;
   vec.normalize();
   image_ray.setOrigin(v2);
   image_ray.setDirection(vec);
   
   std::cout << "image ====== " << image_point << std::endl;
   std::cout << "V2 ====== " << v2 << std::endl;
   std::cout << "v ====== " << v << std::endl;
   std::cout << "platform ====== " << m_ecefPlatformPosition << std::endl;
#endif
#if 0
   ossimDpt f1 ((image_point) - theRefImgPt);
   f1.x *= m_pixelSize.x;
   f1.y *= -m_pixelSize.y;
   ossimDpt film (f1 - m_principalPoint);
 
   
   ossimColumnVector3d cam_ray_dir (film.x,
                                    film.y,
                                    -(m_focalLength+computeParameterOffset(6)));
   
   ossimEcefVector     ecf_ray_dir (m_compositeMatrix*m_mount*cam_ray_dir);
   ecf_ray_dir = ecf_ray_dir*(1.0/ecf_ray_dir.magnitude());
   
   image_ray.setOrigin(m_ecefPlatformPosition);
   image_ray.setDirection(ecf_ray_dir);
#endif
}

void ossimSonomaSensor::lineSampleHeightToWorld(const ossimDpt& image_point,
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
      ossimColumnVector3d origin = m_compositeMatrix*ossimColumnVector3d(0,0,0);
      ossimColumnVector3d v = m_compositeMatrix*(m_mountInverse*(m_pixelToCamera*ossimColumnVector3d(image_point.x, image_point.y, 1.0)));
      ossimDpt3d rayOrigin(origin[0], origin[1], origin[2]);
      ossimDpt3d rayDirection(v[0]-origin[0],
                              v[1]-origin[1],
                              v[2]-origin[2]);
      rayDirection = rayDirection/rayDirection.length();
      ossimDpt3d result;
      double h = heightEllipsoid;
      
      if(ossim::isnan(heightEllipsoid))
      {
         h = 0.0;
      }
      
      intersectRayWithHeight(*(m_utmProjection.get()), result, rayOrigin, rayDirection, h);
               
      worldPoint = m_utmProjection->inverse(ossimDpt(result.x, result.y));
      worldPoint.height(result.z);
      //***
      // First establish imaging ray from image point:
      //***
//      ossimEcefRay ray;
//      imagingRay(image_point, ray);
//      ossimEcefPoint Pecf (ray.intersectAboveEarthEllipsoid(heightEllipsoid));
//      worldPoint = ossimGpt(Pecf);
   }
   
}
void ossimSonomaSensor::lineSampleToWorld(const ossimDpt& image_point,
                                                ossimGpt&       gpt) const
{
   if(image_point.hasNans())
   {
      gpt.makeNan();
      return;
   }
   
   ossimColumnVector3d origin = m_compositeMatrix*ossimColumnVector3d(0,0,0);
   ossimColumnVector3d v = m_compositeMatrix*(m_mountInverse*(m_pixelToCamera*ossimColumnVector3d(image_point.x, image_point.y, 1.0)));
   ossimDpt3d rayOrigin(origin[0], origin[1], origin[2]);
   ossimDpt3d rayDirection(v[0]-origin[0],
                           v[1]-origin[1],
                           v[2]-origin[2]);
   rayDirection = rayDirection/rayDirection.length();
   ossimDpt3d result;
   // ossim_uint32 iterIdx = 0;
   intersectRay(*(m_utmProjection.get()), result, rayOrigin, rayDirection);
   gpt = m_utmProjection->inverse(ossimDpt(result.x, result.y));
   gpt.height(result.z);
   
#if 0
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
#endif   
   if (traceDebug())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimSensorModel::lineSampleToWorld: returning..." << std::endl;
   return;
   
}

void ossimSonomaSensor::updateModel()
{
   ossimGpt gpt = m_platformPositionEllipsoid;
   m_ecefPlatformPosition = gpt;
   double r = ossim::degreesToRadians(m_roll);
   double p = ossim::degreesToRadians(-(90+m_pitch));
   double y = ossim::degreesToRadians(m_heading);
   
   NEWMAT::Matrix rollM = ossimMatrix4x4(cos(r), -sin(r), 0.0, 0.0,
                                         sin(r), cos(r), 0.0, 0.0,
                                         0.0, 0.0, 1.0, 0.0,
                                         0.0, 0.0, 0.0, 1.0).getData();
   NEWMAT::Matrix pitchM = ossimMatrix4x4(1.0, 0.0, 0.0, 0.0,    
                                          0.0, cos(p), -sin(p), 0.0,
                                          0.0, sin(p), cos(p), 0.0,
                                          0.0, 0.0, 0.0, 1.0).getData();
   NEWMAT::Matrix headingM = ossimMatrix4x4(cos(y), -sin(y), 0.0, 0.0,
                                            sin(y), cos(y), 0.0, 0.0,
                                            0, 0, 1.0, 0.0,
                                            0.0, 0.0, 0.0, 1.0).getData();
   
   m_utmProjection->setZone(ossimUtmProjection::computeZone(m_platformPositionEllipsoid));
   m_utmProjection->setHemisphere(m_platformPositionEllipsoid.latd()>=0.0?'N':'S');
   m_pixelToCamera = ossimMatrix3x3::create(m_pixelSize.x/m_focalLength, 0.0, -(m_pixelSize.x/m_focalLength)*theImageSize.x/2,
                                            0.0, -m_pixelSize.x/m_focalLength,   (m_pixelSize.x/m_focalLength)*theImageSize.y/2,
                                            0.0, 0.0,-1.0);
   ossimDpt utm = m_utmProjection->forward(m_platformPositionEllipsoid);
   
   m_compositeMatrixInverse = rollM * pitchM * headingM * ossimMatrix4x4::createTranslationMatrix(-utm.x, -utm.y, -m_platformPositionEllipsoid.height());
   m_compositeMatrix = m_compositeMatrixInverse.i();
   
#if 0
   ossimGpt gpt = m_platformPositionEllipsoid;
   m_ecefPlatformPosition = gpt;
   
   double r = ossim::degreesToRadians(m_roll);
   double p = ossim::degreesToRadians(-(90+m_pitch));
   double y = ossim::degreesToRadians(m_heading);
   
   NEWMAT::Matrix rollM = ossimMatrix4x4(cos(r), -sin(r), 0.0, 0.0,
                                         sin(r), cos(r), 0.0, 0.0,
                                         0.0, 0.0, 1.0, 0.0,
                                         0.0, 0.0, 0.0, 1.0).getData();
   NEWMAT::Matrix pitchM = ossimMatrix4x4(1.0, 0.0, 0.0, 0.0,    
                                          0.0, cos(p), -sin(p), 0.0,
                                          0.0, sin(p), cos(p), 0.0,
                                          0.0, 0.0, 0.0, 1.0).getData();
   NEWMAT::Matrix headingM = ossimMatrix4x4(cos(y), -sin(y), 0.0, 0.0,
                                            sin(y), cos(y), 0.0, 0.0,
                                            0, 0, 1.0, 0.0,
                                            0.0, 0.0, 0.0, 1.0).getData();
   ossimLsrSpace lsrSpace(m_ecefPlatformPosition);//,m_heading+computeParameterOffset(4));

   
   NEWMAT::Matrix platformLsrMatrix4x4 = ossimMatrix4x4::createIdentity();
#endif
#if 0  
   // now lets create a UTM axis by creating a derivative at the center
   // by shift over a few pixels and subtracting
   //

   m_utmProjection.setZone(ossimUtmProjection::computeZone(m_platformPositionEllipsoid));
   m_utmProjection.setHemisphere(m_platformPositionEllipsoid.latd()>=0.0?'N':'S');
   m_utmProjection.setMetersPerPixel(ossimDpt(1.0,1.0));
   ossimDpt midPt  = m_utmProjection.forward(m_ecefPlatformPosition);
   ossimDpt rPt    = midPt + ossimDpt(10, 0.0);
   ossimDpt uPt    = midPt + ossimDpt(0.0, 10.0);
   ossimGpt wMidPt = m_utmProjection.inverse(midPt);
   ossimGpt wRPt   = m_utmProjection.inverse(rPt);
   ossimGpt wUPt   = m_utmProjection.inverse(uPt);
   
   ossimEcefPoint ecefMidPt = wMidPt;
   ossimEcefPoint ecefRPt   = wRPt;
   ossimEcefPoint ecefUPt   = wUPt;
   
   ossimEcefVector east  = ecefRPt-ecefMidPt;
   ossimEcefVector north = ecefUPt-ecefMidPt;
   east.normalize();
   north.normalize();
   
   // now use the lsr space constructors to construct an orthogonal set of axes
   //
   lsrSpace = ossimLsrSpace(m_ecefPlatformPosition,
                            east,
                            north,
                            0);
#endif
   
   
#if 0
   NEWMAT::Matrix platformLsrMatrix = lsrSpace.ecefToLsrRotMatrix();

   platformLsrMatrix4x4[0][0] = platformLsrMatrix[0][0];
   platformLsrMatrix4x4[1][0] = platformLsrMatrix[1][0];
   platformLsrMatrix4x4[2][0] = platformLsrMatrix[2][0];
   platformLsrMatrix4x4[0][1] = platformLsrMatrix[0][1];
   platformLsrMatrix4x4[1][1] = platformLsrMatrix[1][1];
   platformLsrMatrix4x4[2][1] = platformLsrMatrix[2][1];
   platformLsrMatrix4x4[0][2] = platformLsrMatrix[0][2];
   platformLsrMatrix4x4[1][2] = platformLsrMatrix[1][2];
   platformLsrMatrix4x4[2][2] = platformLsrMatrix[2][2];
  
   m_compositeMatrix   = (rollM*pitchM*headingM*platformLsrMatrix4x4).i();
   m_compositeMatrixInverse = m_compositeMatrix.i();
//   m_compositeMatrix = ((rollM*pitchM*headingM)*platformLsrMatrix4x4).t()*m_mount;
//   m_compositeMatrixInverse         = m_compositeMatrix.i();
#endif
   
#if 0
   platformLsrMatrix4x4[0][0] = platformLsrMatrix[0][0];
   platformLsrMatrix4x4[1][0] = platformLsrMatrix[1][0];
   platformLsrMatrix4x4[2][0] = platformLsrMatrix[2][0];
   platformLsrMatrix4x4[0][1] = platformLsrMatrix[0][1];
   platformLsrMatrix4x4[1][1] = platformLsrMatrix[1][1];
   platformLsrMatrix4x4[2][1] = platformLsrMatrix[2][1];
   platformLsrMatrix4x4[0][2] = platformLsrMatrix[0][2];
   platformLsrMatrix4x4[1][2] = platformLsrMatrix[1][2];
   platformLsrMatrix4x4[2][2] = platformLsrMatrix[2][2];
   NEWMAT::Matrix m = (platformLsrMatrix4x4*(ossimMatrix4x4::createRotationZMatrix(r)*ossimMatrix4x4::createRotationXMatrix(p)*ossimMatrix4x4::createRotationZMatrix(y)))*
                       ossimMatrix4x4::createTranslationMatrix(-m_ecefPlatformPosition.x(), -m_ecefPlatformPosition.y(), -m_ecefPlatformPosition.z());
   m_compositeMatrixInverse = m;
   m_compositeMatrix = m.i();
   NEWMAT::Matrix pixelToCamera = NEWMAT::Matrix(3, 3);
   double f  = m_focalLength/m_pixelSize.x;
   double w  = theImageClipRect.width();
   double h  = theImageClipRect.height();
   pixelToCamera << f << 0 << (-f*w/2.0) << 0  << -f << (f*h/2.0) << 0 << 0 <<-1;
   pixelToCamera = pixelToCamera.t();
   m_pixelToCamera = pixelToCamera;
#endif
   theBoundGndPolygon.resize(0);
#if 0
   theBoundGndPolygon.resize(4);
   ossim_float64 w2 = theImageClipRect.width()*2;
   ossim_float64 h2 = theImageClipRect.height()*2;
   lineSampleToWorld(theImageClipRect.ul()+ossimDpt(-w2, -h2), gpt);
//   std::cout << "gpt1 = " << gpt << std::endl;
   theBoundGndPolygon[0] = gpt;
   lineSampleToWorld(theImageClipRect.ur()+ossimDpt(w2, -h2), gpt);
//   std::cout << "gpt2 = " << gpt << std::endl;
   theBoundGndPolygon[1] = gpt;
   lineSampleToWorld(theImageClipRect.lr()+ossimDpt(w2, h2), gpt);
   theBoundGndPolygon[2] = gpt;
//   std::cout << "gpt3 = " << gpt << std::endl;
   lineSampleToWorld(theImageClipRect.ll()+ossimDpt(-w2, h2), gpt);
   theBoundGndPolygon[3] = gpt;
#endif
//   std::cout << "gpt4 = " << gpt << std::endl;
   lineSampleToWorld(theRefImgPt, theRefGndPt);
   
   
//   ossimColumnVector3d v(0.0,0.0,1.0);
//   v = m*(m_mountInverse*(m_pixelToCamera*v));
//   ossimEcefVector vec = ossimEcefPoint(v[0], v[1], v[2]) - m_ecefPlatformPosition;
//   vec.normalize();
   
//   ossimEcefRay   image_ray;
   
//   image_ray.setOrigin(m_ecefPlatformPosition);
//   image_ray.setDirection(vec);
//   ossimEcefPoint Pecf (image_ray.intersectAboveEarthEllipsoid(0.0));
   
   computeGsd();
}

void ossimSonomaSensor::initAdjustableParameters()
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
   setParameterDescription(4, "heading");
   setParameterUnit(4, "degrees");
   
   setAdjustableParameter(5, 0.0);
   setParameterDescription(5, "Altitude delta");
   setParameterUnit(5, "meters");
   
   setAdjustableParameter(6, 0.0);
   setParameterDescription(6, "focal length delta");
   setParameterUnit(6, "meters");
   
   setParameterSigma(0, 10);
   setParameterSigma(1, 10);
   setParameterSigma(2, 1);
   setParameterSigma(3, 1);
   setParameterSigma(4, 5);
   setParameterSigma(5, 100);
   setParameterSigma(6, 50);
   
}

bool ossimSonomaSensor::loadState(const ossimKeywordlist& kwl, const char* prefix)
{
   theGSD.makeNan();
   theRefImgPt.makeNan();
   ossimSensorModel::loadState(kwl, prefix);
   if(getNumberOfAdjustableParameters() < 7)
   {
      initAdjustableParameters();
   }
   if(theRefImgPt.hasNans())
   {
      theRefImgPt = theImageClipRect.midPoint();
   }
   ossimString mount           = kwl.find(prefix, "mount");
   ossimString pixel_size      = kwl.find(prefix, "pixel_size");
   ossimString principal_point = kwl.find(prefix, "principal_point");
   ossimString focal_length    = kwl.find(prefix, "focal_length");
   ossimString roll;
   ossimString pitch;
   ossimString heading;
   ossimString platform_position;
   m_roll    = 0;
   m_pitch   = 0;
   m_heading     = 0;
   roll = kwl.find(prefix, "roll"); 
   pitch = kwl.find(prefix, "pitch"); 
   heading = kwl.find(prefix, "heading"); 
   platform_position = kwl.find(prefix, "platform_position");
   bool result = (!pixel_size.empty()&&
                  !principal_point.empty()&&
                  !focal_length.empty()&&
                  !platform_position.empty());
   if(!mount.empty())
   {
      mount = mount.trim();
      std::vector<ossimString> values;
      
      mount.split(values, " ");
      bool valid = true;
      if(values.size() == 9)
      {
         m_mount = ossimMatrix3x3::createIdentity();
      }
      else if((values.size() == 16) ||
              (values.size() == 12))
      {
         m_mount = ossimMatrix4x4::createIdentity();
      }
      else 
      {
         m_mount = ossimMatrix4x4::createIdentity();
         m_mountInverse = m_mount;
         valid = false;
      }

      if(valid)
      {
         ossim_uint32 idx = 0;
         ossim_int32 row = -1;
         for(idx = 0; idx < values.size(); ++idx)
         {
            if(idx%4 == 0) ++row;
            m_mount[row][idx%4] = values[idx].toDouble();
         }
         m_mountInverse = m_mount.i();
      }
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
   if(!heading.empty())
   {
      m_heading   = heading.toDouble();
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
            << "ossimSonomaSensor::loadState Caught Exception:\n"
            << e.what() << std::endl;
         }
      }
   }
   
   return result;
}

bool ossimSonomaSensor::saveState(ossimKeywordlist& kwl, const char* prefix)const
{
   ossimSensorModel::saveState(kwl, prefix);
   
   ossimString mount;
   ossim_int32 rowIdx = 0;
   ossim_int32 colIdx = 0;

   for(rowIdx = 0; rowIdx < m_mount.Nrows(); ++rowIdx)
   {
      for(colIdx = 0; colIdx < m_mount.Ncols(); ++ colIdx)
      {
         mount += (ossimString::toString(m_mount[rowIdx][colIdx]) + " ");
      }
   }
   
   kwl.add(prefix, "mount", mount.trim(), true);
   kwl.add(prefix, "roll", ossimString::toString(m_roll), true);
   kwl.add(prefix, "pitch", ossimString::toString(m_pitch), true);
   kwl.add(prefix, "heading", ossimString::toString(m_heading), true);
   kwl.add(prefix, "principal_point", m_principalPoint.toString(), true);
   kwl.add(prefix, "pixel_size", m_pixelSize.toString(), true);
   kwl.add(prefix, "platform_position",m_platformPosition.toString() ,true);
   kwl.add(prefix, "focal_length", ossimString::toString(m_focalLength) ,true);
   
   return true;
}


bool ossimSonomaSensor::intersectRay(const ossimMapProjection& proj, ossimDpt3d& result, ossimDpt3d& origin, ossimDpt3d& dir)const
{
   ossimPlane plane(0.0, 0.0, 1.0, 0.0);
   
   static const double CONVERGENCE_THRESHOLD = 0.0001; // meters
   static const int    MAX_NUM_ITERATIONS    = 50;
   
   double          h; // height above ellipsoid
   bool            intersected;
   ossimDpt3d  prev_intersect_pt (origin);
   ossimDpt3d  new_intersect_pt;
   double          distance;
   bool            done = false;
   int             iteration_count = 0;
   
   if(dir.hasNans()) 
   {
      result.makeNan();
      return false;
   }
   
   ossimGpt gpt = proj.inverse(ossimDpt(origin.x, origin.y));
   //
   // Loop to iterate on ray intersection with variable elevation surface:
   //
   do
   {
      //
      // Intersect ray with ellipsoid inflated by h_ellips:
      //
      h = ossimElevManager::instance()->getHeightAboveEllipsoid(gpt);
      
      if ( ossim::isnan(h) ) h = 0.0;
      plane.setOffset(-h);
      intersected = plane.intersect(new_intersect_pt, origin, dir);
      if (!intersected)
      {
         result.makeNan();
         done = true;
      }
      else
      {
         //
         // Assign the ground point to the latest iteration's intersection
         // point:
         //
         gpt =  proj.inverse(ossimDpt(new_intersect_pt.x, new_intersect_pt.y));
         gpt.height(new_intersect_pt.z);
         result = new_intersect_pt;
         //
         // Determine if convergence achieved:
         //
         distance = (new_intersect_pt - prev_intersect_pt).length();
         if (distance < CONVERGENCE_THRESHOLD)
         {
            done = true;
         }
         else
         {
            prev_intersect_pt = new_intersect_pt;
         }
      }
      
      iteration_count++;
      
   } while ((!done) && (iteration_count < MAX_NUM_ITERATIONS));
   
   return intersected;
}

bool ossimSonomaSensor::intersectRayWithHeight(const ossimMapProjection& /* proj */, ossimDpt3d& result, ossimDpt3d& origin, ossimDpt3d& dir, double h)const
{
   ossimPlane plane(0.0, 0.0, 1.0, -h);
   bool intersected = plane.intersect(result, origin, dir);
  
   
   return intersected;
}

