//*******************************************************************
// Copyright (C) 2005 Garrett Potts
//
// LGPL
// 
// Author:  Garrett Potts
//
//*******************************************************************
//  $Id: ossimApplanixUtmModel.cpp 21214 2012-07-03 16:20:11Z dburken $
#include <sstream>
#include <ossim/projection/ossimApplanixUtmModel.h>
#include <ossim/base/ossimEllipsoid.h>
#include <ossim/base/ossimUnitConversionTool.h>
#include <ossim/base/ossimUnitTypeLut.h>
#include <ossim/base/ossimDatumFactory.h>
#include <ossim/base/ossimDatum.h>
#include <ossim/base/ossimLsrSpace.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimGeoidManager.h>
#include <ossim/projection/ossimUtmProjection.h>
#include <ossim/support_data/ossimApplanixEOFile.h>
#include <ossim/base/ossimMatrix4x4.h>
#include <ossim/elevation/ossimElevManager.h>
static ossimTrace traceDebug("ossimApplanixUtmModel:debug");

RTTI_DEF1(ossimApplanixUtmModel, "ossimApplanixUtmModel", ossimSensorModel);

#ifdef OSSIM_ID_ENABLED
static const char OSSIM_ID[] = "$Id: ossimApplanixUtmModel.cpp 21214 2012-07-03 16:20:11Z dburken $";
#endif

ossimApplanixUtmModel::ossimApplanixUtmModel()
   :theOmega(0.0),
    thePhi(0.0),
    theKappa(0.0),
    theBoreSightTx(0.0),
    theBoreSightTy(0.0),
    theBoreSightTz(0.0)
{
   theCompositeMatrix          = ossimMatrix4x4::createIdentity();
   theCompositeMatrixInverse   = ossimMatrix4x4::createIdentity();
   theFocalLength              = 55.0;
   thePixelSize = ossimDpt(.009, .009);
   theEcefPlatformPosition = ossimGpt(0.0,0.0, 1000.0);
   theGSD.x = 0.1524;
   theGSD.y = 0.1524;
   theMeanGSD = 0.1524;
   theLensDistortion = new ossimMeanRadialLensDistortion;
   initAdjustableParameters();

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimApplanixUtmModel::ossimApplanixUtmModel DEBUG:" << endl;
#ifdef OSSIM_ID_ENABLED
      ossimNotify(ossimNotifyLevel_DEBUG)<< "OSSIM_ID:  " << OSSIM_ID << endl;
#endif
   }
}

ossimApplanixUtmModel::ossimApplanixUtmModel(const ossimApplanixUtmModel& src)
   :ossimSensorModel(src),
    theCompositeMatrix(src.theCompositeMatrix),
    theCompositeMatrixInverse(src.theCompositeMatrixInverse),
    theOmega(src.theOmega),
    thePhi(src.thePhi),
    theKappa(src.theKappa),
    theBoreSightTx(src.theBoreSightTx),
    theBoreSightTy(src.theBoreSightTy),
    theBoreSightTz(src.theBoreSightTz),
    thePrincipalPoint(src.thePrincipalPoint),
    thePixelSize(src.thePixelSize),
    theFocalLength(src.theFocalLength),
    theEcefPlatformPosition(src.theEcefPlatformPosition),
    thePlatformPosition(src.thePlatformPosition),
    theUtmZone(src.theUtmZone),
    theUtmHemisphere(src.theUtmHemisphere),
    theUtmPlatformPosition(src.theUtmPlatformPosition)
{
   if(src.theLensDistortion.valid())
   {
      theLensDistortion = new ossimMeanRadialLensDistortion(*(src.theLensDistortion.get()));
   }
   else
   {
      theLensDistortion = new ossimMeanRadialLensDistortion();
   }
}

ossimObject* ossimApplanixUtmModel::dup()const
{
   return new ossimApplanixUtmModel(*this);
}

void ossimApplanixUtmModel::imagingRay(const ossimDpt& image_point,
                                    ossimEcefRay&   image_ray) const
{
//    if(traceDebug())
//    {
//       ossimNotify(ossimNotifyLevel_DEBUG) << "ossimApplanixUtmModel::imagingRay: ..... entered" << std::endl;
//    }
    ossimDpt f1 ((image_point) - theRefImgPt);
   f1.x *= thePixelSize.x;
   f1.y *= -thePixelSize.y;
   ossimDpt film (f1 - thePrincipalPoint);
//    if(traceDebug())
//    {
//       ossimNotify(ossimNotifyLevel_DEBUG) << "pixel size   = " << thePixelSize << std::endl;
//       ossimNotify(ossimNotifyLevel_DEBUG) << "principal pt = " << thePrincipalPoint << std::endl;
//       ossimNotify(ossimNotifyLevel_DEBUG) << "film pt      = " << film << std::endl;
//    }
   if (theLensDistortion.valid())
   {
      ossimDpt filmOut;
      theLensDistortion->undistort(film, filmOut);
      film = filmOut;
   }
   
   ossimColumnVector3d cam_ray_dir (film.x,
                                    film.y,
                                    -theFocalLength);
   ossimEcefVector     ecf_ray_dir (theCompositeMatrix*cam_ray_dir);
   ecf_ray_dir = ecf_ray_dir*(1.0/ecf_ray_dir.magnitude());
   
   image_ray.setOrigin(theAdjEcefPlatformPosition);
   image_ray.setDirection(ecf_ray_dir);
   
//    if(traceDebug())
//    {
//       ossimNotify(ossimNotifyLevel_DEBUG) << "ossimApplanixUtmModel::imagingRay: ..... leaving" << std::endl;
//    }
}

void ossimApplanixUtmModel::lineSampleToWorld(const ossimDpt& image_point,
                                               ossimGpt&       gpt) const
{
   if (traceDebug())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimApplanixEcefModel::lineSampleToWorld:entering..." << std::endl;
   
   if(image_point.hasNans())
   {
      gpt.makeNan();
      return;
   }
   //***
   // Extrapolate if image point is outside image:
   //***
  // if (!insideImage(image_point))
  // {
   //   gpt.makeNan();
//       gpt = extrapolate(image_point);
   //   return;
   //}

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

void ossimApplanixUtmModel::lineSampleHeightToWorld(const ossimDpt& image_point,
                                                 const double&   heightEllipsoid,
                                                 ossimGpt&       worldPoint) const
{
//   if (!insideImage(image_point))
//   {
      //worldPoint.makeNan();
//       worldPoint = extrapolate(image_point, heightEllipsoid);
//   }
//   else
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

void ossimApplanixUtmModel::worldToLineSample(const ossimGpt& world_point,
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
   ossimEcefVector ecfRayDir(g_ecf - theAdjEcefPlatformPosition);
   ossimColumnVector3d camRayDir (theCompositeMatrixInverse*ecfRayDir.data());
   
      
   double scale = -theFocalLength/camRayDir[2];
   ossimDpt film (scale*camRayDir[0], scale*camRayDir[1]);
      
   if (theLensDistortion.valid())
   {
      ossimDpt filmOut;
      theLensDistortion->distort(film, filmOut);
      film = filmOut;
    }
   
     ossimDpt f1(film + thePrincipalPoint);
    ossimDpt p1(f1.x/thePixelSize.x,
                -f1.y/thePixelSize.y);

    ossimDpt p0 (p1.x + theRefImgPt.x,
                 p1.y + theRefImgPt.y);
    
    image_point = p0;
}

void ossimApplanixUtmModel::updateModel()
{

   ossimGpt wgs84Pt;
   double metersPerDegree = wgs84Pt.metersPerDegree().x;
   double degreePerMeter = 1.0/metersPerDegree;
   double latShift = -computeParameterOffset(1)*theMeanGSD*degreePerMeter;
   double lonShift = computeParameterOffset(0)*theMeanGSD*degreePerMeter;

   ossimGpt gpt = thePlatformPosition;
//   gpt.height(0.0);
   double height = gpt.height();
   gpt.height(height + computeParameterOffset(5));
   gpt.latd(gpt.latd() + latShift);
   gpt.lond(gpt.lond() + lonShift);
   
   theAdjEcefPlatformPosition = gpt;
   ossimLsrSpace lsrSpace(theAdjEcefPlatformPosition);
   // ORIENT TO A UTM AXIS
   //
    NEWMAT::ColumnVector v(3);
   
    v[0] = 0.0;
    v[1] = 0.0;
    v[2] = 1.0;
    NEWMAT::ColumnVector v2 = lsrSpace.lsrToEcefRotMatrix()*v;
    ossimEcefVector zVector(v2[0], v2[1], v2[2]);
    zVector.normalize();
   
   // now lets create a UTM axis by creating a derivative at the center
   // by shift over a few pixels and subtracting
   //
   ossimUtmProjection utmProj;
   
   utmProj.setZone(theUtmZone);
   utmProj.setZone(theUtmHemisphere);
   utmProj.setMetersPerPixel(ossimDpt(1.0,1.0));
   ossimDpt midPt  = utmProj.forward(theAdjEcefPlatformPosition);
   ossimDpt rPt    = midPt + ossimDpt(10, 0.0);
   ossimDpt uPt    = midPt + ossimDpt(0.0, 10.0);
   ossimGpt wMidPt = utmProj.inverse(midPt);
   ossimGpt wRPt   = utmProj.inverse(rPt);
   ossimGpt wUPt   = utmProj.inverse(uPt);
   
   ossimEcefPoint ecefMidPt = wMidPt;
   ossimEcefPoint ecefRPt   = wRPt;
   ossimEcefPoint ecefUPt   = wUPt;
   
   ossimEcefVector east  = ecefRPt-ecefMidPt;
   ossimEcefVector north = ecefUPt-ecefMidPt;
   east.normalize();
   north.normalize();
   
   // now use the lsr space constructors to construct an orthogonal set of axes
   //
   lsrSpace = ossimLsrSpace(thePlatformPosition,
                            0,
                            north,
                            east.cross(north));
//   lsrSpace = ossimLsrSpace(thePlatformPosition);
   // DONE ORIENT TO UTM AXIS
   
   NEWMAT::Matrix platformLsrMatrix = lsrSpace.lsrToEcefRotMatrix();
   NEWMAT::Matrix orientationMatrix = (ossimMatrix3x3::createRotationXMatrix(theOmega+computeParameterOffset(2), OSSIM_LEFT_HANDED)*
                                       ossimMatrix3x3::createRotationYMatrix(thePhi+computeParameterOffset(3), OSSIM_LEFT_HANDED)*
                                       ossimMatrix3x3::createRotationZMatrix(theKappa+computeParameterOffset(4), OSSIM_LEFT_HANDED));
   
   theCompositeMatrix         = platformLsrMatrix*orientationMatrix;
   theCompositeMatrixInverse  = theCompositeMatrix.i();

//   theAdjEcefPlatformPosition = theEcefPlatformPosition; 

   theBoundGndPolygon.resize(4);
   // ossim_float64 w = theImageClipRect.width();//*2.0;
   // ossim_float64 h = theImageClipRect.height();//*2.0;
   
   lineSampleToWorld(theImageClipRect.ul(),gpt);//+ossimDpt(-w, -h), gpt);
   theBoundGndPolygon[0] = gpt;
   lineSampleToWorld(theImageClipRect.ur(),gpt);//+ossimDpt(w, -h), gpt);
   theBoundGndPolygon[1] = gpt;
   lineSampleToWorld(theImageClipRect.lr(),gpt);//+ossimDpt(w, h), gpt);
   theBoundGndPolygon[2] = gpt;
   lineSampleToWorld(theImageClipRect.ll(),gpt);//+ossimDpt(-w, h), gpt);
   theBoundGndPolygon[3] = gpt;
}

void ossimApplanixUtmModel::initAdjustableParameters()
{
   resizeAdjustableParameterArray(6);
   
   setAdjustableParameter(0, 0.0);
   setParameterDescription(0, "x_offset");
   setParameterUnit(0, "pixels");

   setAdjustableParameter(1, 0.0);
   setParameterDescription(1, "y_offset");
   setParameterUnit(1, "pixels");

   setAdjustableParameter(2, 0.0);
   setParameterDescription(2, "orientation x");
   setParameterUnit(2, "degrees");

   setAdjustableParameter(3, 0.0);
   setParameterDescription(3, "orientation y");
   setParameterUnit(3, "degrees");

   setAdjustableParameter(4, 0.0);
   setParameterDescription(4, "orientation z");
   setParameterUnit(4, "degrees");

   setAdjustableParameter(5, 0.0);
   setParameterDescription(5, "Altitude delta");
   setParameterUnit(5, "meters");

   
   
   setParameterSigma(0, 20.0);
   setParameterSigma(1, 20.0);
   setParameterSigma(2, .1);
   setParameterSigma(3, .1);
   setParameterSigma(4, .1);
   setParameterSigma(5, 50);
}

void ossimApplanixUtmModel::setPrincipalPoint(ossimDpt principalPoint)
{
   thePrincipalPoint = principalPoint;
}

void ossimApplanixUtmModel::setPixelSize(const ossimDpt& pixelSize)
{
   thePixelSize = pixelSize;
}

void ossimApplanixUtmModel::setImageRect(const ossimDrect& rect)
{
   theImageClipRect = rect;
   theRefImgPt = rect.midPoint();
}

void ossimApplanixUtmModel::setFocalLength(double focalLength)
{
   theFocalLength = focalLength;
}

void ossimApplanixUtmModel::setPlatformPosition(const ossimGpt& gpt)
{
   theRefGndPt             = gpt;
   theEcefPlatformPosition = gpt;
   updateModel();
   
}

bool ossimApplanixUtmModel::saveState(ossimKeywordlist& kwl,
                                   const char* prefix) const
{
   ossimSensorModel::saveState(kwl, prefix);
   
   kwl.add(prefix, "type", "ossimApplanixUtmModel", true);

   kwl.add(prefix, "omega", theOmega, true);
   kwl.add(prefix, "phi", thePhi, true);
   kwl.add(prefix, "kappa", theKappa, true);
   kwl.add(prefix, "bore_sight_tx", theBoreSightTx*60, true);
   kwl.add(prefix, "bore_sight_ty", theBoreSightTy*60, true);
   kwl.add(prefix, "bore_sight_tz", theBoreSightTz*60, true);
   kwl.add(prefix, "principal_point", ossimString::toString(thePrincipalPoint.x) + " " + ossimString::toString(thePrincipalPoint.y));
   kwl.add(prefix, "pixel_size",      ossimString::toString(thePixelSize.x) + " " + ossimString::toString(thePixelSize.y));
   kwl.add(prefix, "focal_length", theFocalLength);
   kwl.add(prefix, "ecef_platform_position",
           ossimString::toString(theEcefPlatformPosition.x()) + " " +
           ossimString::toString(theEcefPlatformPosition.y()) + " " +
           ossimString::toString(theEcefPlatformPosition.z()));
   kwl.add(prefix, "latlonh_platform_position",
           ossimString::toString(thePlatformPosition.latd()) + " " +
           ossimString::toString(thePlatformPosition.lond()) + " " +
           ossimString::toString(thePlatformPosition.height()) + " " +
           thePlatformPosition.datum()->code());
   kwl.add(prefix,
           "utm_platform_position",
           ossimString::toString(theUtmPlatformPosition.x) + " " +
           ossimString::toString(theUtmPlatformPosition.y) + " " +
           ossimString::toString(theUtmPlatformPosition.z) + " " +
           thePlatformPosition.datum()->code(),
           true);
   kwl.add(prefix,
           "utm_zone",
           theUtmZone,
           true);
   kwl.add(prefix,
           "utm_hemisphere",
           theUtmHemisphere,
           true);
   kwl.add(prefix,
           "shift_values",
           ossimString::toString(theShiftValues.x()) + " " + 
           ossimString::toString(theShiftValues.y()) + " " +
           ossimString::toString(theShiftValues.z()),
           true);
   if(theLensDistortion.valid())
   {
      ossimString lensPrefix = ossimString(prefix)+"distortion.";
      theLensDistortion->saveState(kwl,
                                   lensPrefix.c_str());
   }
   
   return true;
}

bool ossimApplanixUtmModel::loadState(const ossimKeywordlist& kwl,
                                   const char* prefix)
{
   if(traceDebug())
   {
      std::cout << "ossimApplanixUtmModel::loadState: ......... entered" << std::endl;
   }
   theImageClipRect = ossimDrect(0,0,4076,4091);
   theRefImgPt      = ossimDpt(2046.0, 2038.5);
   
   ossimSensorModel::loadState(kwl, prefix);

   if(getNumberOfAdjustableParameters() < 1)
   {
      initAdjustableParameters();
   }

   const char* eo_file           = kwl.find(prefix, "eo_file");
   const char* eo_id             = kwl.find(prefix, "eo_id");
   const char* omega             = kwl.find(prefix, "omega");
   const char* phi               = kwl.find(prefix, "phi");
   const char* kappa             = kwl.find(prefix, "kappa");
   const char* bore_sight_tx     = kwl.find(prefix, "bore_sight_tx");
   const char* bore_sight_ty     = kwl.find(prefix, "bore_sight_ty");
   const char* bore_sight_tz     = kwl.find(prefix, "bore_sight_tz");
   
   const char* principal_point   = kwl.find(prefix, "principal_point");
   const char* pixel_size        = kwl.find(prefix, "pixel_size");
   const char* focal_length      = kwl.find(prefix, "focal_length");
   const char* latlonh_platform_position = kwl.find(prefix, "latlonh_platform_position");
   const char* utm_platform_position = kwl.find(prefix, "utm_platform_position");
   const char* compute_gsd_flag  = kwl.find(prefix, "compute_gsd_flag");
   const char* utm_zone          = kwl.find(prefix, "utm_zone");
   const char* utm_hemisphere    = kwl.find(prefix, "utm_hemisphere");
   const char* camera_file       = kwl.find(prefix, "camera_file");
   const char* shift_values      = kwl.find(prefix, "shift_values");
   
   theCompositeMatrix          = ossimMatrix3x3::createIdentity();
   theCompositeMatrixInverse   = ossimMatrix3x3::createIdentity();
   theOmega                    = 0.0;
   thePhi                      = 0.0;
   theKappa                    = 0.0;
   theBoreSightTx              = 0.0;
   theBoreSightTy              = 0.0;
   theBoreSightTz              = 0.0;
   theFocalLength              = 55.0;
   thePixelSize = ossimDpt(.009, .009);
   theEcefPlatformPosition = ossimGpt(0.0,0.0, 1000.0);

   bool loadedFromEoFile = false;

   if(eo_id)
   {
      theImageID = eo_id;
   }
   // loading from standard eo file with given record id
   //
   if(eo_file)
   {
      ossimApplanixEOFile eoFile;
      
      if(eoFile.parseFile(ossimFilename(eo_file)))
      {
         ossimRefPtr<ossimApplanixEORecord> record = eoFile.getRecordGivenId(theImageID);

         if(record.valid())
         {
            loadedFromEoFile = true;
            theBoreSightTx = eoFile.getBoreSightTx()/60.0;
            theBoreSightTy = eoFile.getBoreSightTy()/60.0;
            theBoreSightTz = eoFile.getBoreSightTz()/60.0;
            theShiftValues = ossimEcefVector(eoFile.getShiftValuesX(),
                                             eoFile.getShiftValuesY(),
                                             eoFile.getShiftValuesZ());
            ossim_int32 easting  = eoFile.getFieldIdxLike("EASTING");
            ossim_int32 northing = eoFile.getFieldIdxLike("NORTHING");
            ossim_int32 height   = eoFile.getFieldIdxLike("HEIGHT");
            ossim_int32 omega    = eoFile.getFieldIdxLike("OMEGA");
            ossim_int32 phi      = eoFile.getFieldIdxLike("PHI");
            ossim_int32 kappa    = eoFile.getFieldIdxLike("KAPPA");

            if((omega>=0)&&
               (phi>=0)&&
               (kappa>=0)&&
               (height>=0)&&
               (easting>=0)&&
               (northing>=0))
            {
               theOmega = (*record)[omega].toDouble();
               thePhi   = (*record)[phi].toDouble();
               theKappa = (*record)[kappa].toDouble();
               double h = (*record)[height].toDouble();
               ossimString heightType = kwl.find(prefix, "height_type");
               if(eoFile.isUtmFrame())
               {
                  theUtmZone = eoFile.getUtmZone();
                  theUtmHemisphere = eoFile.getUtmHemisphere()=="North"?'N':'S';
                  ossimUtmProjection utmProj;
                  utmProj.setZone(theUtmZone);
                  utmProj.setHemisphere((char)theUtmHemisphere);
                  theUtmPlatformPosition.x = (*record)[easting].toDouble();
                  theUtmPlatformPosition.y = (*record)[northing].toDouble();
                  theUtmPlatformPosition.z = h;
                  thePlatformPosition = utmProj.inverse(ossimDpt(theUtmPlatformPosition.x,
                                                                 theUtmPlatformPosition.y));
                  thePlatformPosition.height(h);

                  if(eoFile.isHeightAboveMSL())
                  {
                     double offset = ossimGeoidManager::instance()->offsetFromEllipsoid(thePlatformPosition);
                     if(!ossim::isnan(offset))
                     {
                        thePlatformPosition.height(h + offset);
                        theUtmPlatformPosition.z = h + offset;
                     }
                  }
               }
               else
               {
                  return false;
               }
            }
            theEcefPlatformPosition = thePlatformPosition;
         }
         else
         {
            return false;
         }
      }
   }
   if(!loadedFromEoFile)
   {
      if(shift_values)
      {
         std::vector<ossimString> splitString;
         ossimString tempString(shift_values);
         tempString = tempString.trim();
         tempString.split(splitString, " " );
         if(splitString.size() == 3)
         {
            theShiftValues = ossimEcefVector(splitString[0].toDouble(),
                                             splitString[1].toDouble(),
                                             splitString[2].toDouble());
         }
      }
      if(omega&&phi&&kappa)
      {
         theOmega = ossimString(omega).toDouble();
         thePhi   = ossimString(phi).toDouble();
         theKappa = ossimString(kappa).toDouble();
      }
      if(bore_sight_tx&&bore_sight_ty&&bore_sight_tz)
      {
         theBoreSightTx = ossimString(bore_sight_tx).toDouble()/60.0;
         theBoreSightTy = ossimString(bore_sight_ty).toDouble()/60.0;
         theBoreSightTz = ossimString(bore_sight_tz).toDouble()/60.0;
      }
      double lat=0.0, lon=0.0, h=0.0;
      if(utm_zone)
      {
         theUtmZone = ossimString(utm_zone).toInt32();
      }
      if(utm_hemisphere)
      {
         ossimString hem = utm_hemisphere;
         hem = hem.trim();
         hem = hem.upcase();
         theUtmHemisphere = *(hem.begin());
      }
      if(utm_platform_position)
      {
         ossimUtmProjection utmProj;
         std::vector<ossimString> splitString;
         ossimString tempString(utm_platform_position);
         tempString = tempString.trim();
         ossimString datumString;
         utmProj.setZone(theUtmZone);
         utmProj.setHemisphere((char)theUtmHemisphere);
         tempString.split(splitString, " ");
         if(splitString.size() > 2)
         {
            theUtmPlatformPosition.x = splitString[0].toDouble();
            theUtmPlatformPosition.y = splitString[1].toDouble();
            theUtmPlatformPosition.z = splitString[2].toDouble();
         }
         if(splitString.size() > 3)
         {
            datumString = splitString[3];
         }
         const ossimDatum* datum = ossimDatumFactory::instance()->create(datumString);
         if(datum)
         {
            utmProj.setDatum(datum);
         }
         
         thePlatformPosition = utmProj.inverse(ossimDpt(theUtmPlatformPosition.x,
                                                        theUtmPlatformPosition.y));
         thePlatformPosition.height(theUtmPlatformPosition.z);
         
         ossimString heightType = kwl.find(prefix, "height_type");
         if(heightType == "msl")
         {
            double offset = ossimGeoidManager::instance()->offsetFromEllipsoid(thePlatformPosition);
            if(ossim::isnan(offset) == false)
            {
               thePlatformPosition.height(thePlatformPosition.height() + offset);
               theUtmPlatformPosition.z = thePlatformPosition.height();
            }
         }
         theEcefPlatformPosition = thePlatformPosition;
      }
      else if(latlonh_platform_position)
      {
         std::vector<ossimString> splitString;
         ossimString tempString(latlonh_platform_position);
         std::string datumString;
         tempString = tempString.trim();
         tempString.split(splitString, " ");
         if(splitString.size() > 2)
         {
            lat = splitString[0].toDouble();
            lon = splitString[1].toDouble();
            h = splitString[2].toDouble();
         }
         if(splitString.size() > 3)
         {
            datumString = splitString[3].string();
         }
         thePlatformPosition.latd(lat);
         thePlatformPosition.lond(lon);
         thePlatformPosition.height(h);
         const ossimDatum* datum = ossimDatumFactory::instance()->create(datumString);
         if(datum)
         {
            thePlatformPosition.datum(datum);
         }
         ossimString heightType = kwl.find(prefix, "height_type");
         if(heightType == "msl")
         {
            double offset = ossimGeoidManager::instance()->offsetFromEllipsoid(thePlatformPosition);
            if(ossim::isnan(offset) == false)
            {
               thePlatformPosition.height(thePlatformPosition.height() + offset);
            }
         }
         theEcefPlatformPosition = thePlatformPosition;
      }
   }
   if(principal_point)
   {
      std::vector<ossimString> splitString;
      ossimString tempString(principal_point);
      tempString = tempString.trim();
      tempString.split(splitString, " ");
      if(splitString.size() == 2)
      {
         thePrincipalPoint.x = splitString[0].toDouble();
         thePrincipalPoint.y = splitString[1].toDouble();
      }
   }
   if(pixel_size)
   {
      std::vector<ossimString> splitString;
      ossimString tempString(principal_point);
      tempString = tempString.trim();
      tempString.split(splitString, " ");
      if(splitString.size() == 2)
      {
         thePixelSize.x = splitString[0].toDouble();
         thePixelSize.y = splitString[1].toDouble();
      }
   }
   if(focal_length)
   {
      theFocalLength = ossimString(focal_length).toDouble();
   }

   if(camera_file)
   {
      ossimKeywordlist cameraKwl;
      ossimKeywordlist lensKwl;
      cameraKwl.add(camera_file);
      const char* sensor = cameraKwl.find("sensor");
      const char* image_size      = cameraKwl.find(prefix, "image_size");
      principal_point = cameraKwl.find("principal_point");
      focal_length    = cameraKwl.find("focal_length");
      pixel_size      = cameraKwl.find(prefix, "pixel_size");
      focal_length    = cameraKwl.find(prefix, "focal_length");
      const char* distortion_units = cameraKwl.find(prefix, "distortion_units");
      ossimUnitConversionTool tool;
      ossimUnitType unitType = OSSIM_MILLIMETERS;

      if(distortion_units)
      {
         unitType = (ossimUnitType)ossimUnitTypeLut::instance()->getEntryNumber(distortion_units);

         if(unitType == OSSIM_UNIT_UNKNOWN)
         {
            unitType = OSSIM_MILLIMETERS;
         }
      }
      if(image_size)
      {
         std::vector<ossimString> splitString;
         ossimString tempString(image_size);
         tempString = tempString.trim();
         tempString.split(splitString, " ");
         double w=1, h=1;
         if(splitString.size() == 2)
         {
            w = splitString[0].toDouble();
            h = splitString[1].toDouble();
         }
         theImageClipRect = ossimDrect(0,0,w-1,h-1);
         theRefImgPt      = ossimDpt(w/2.0, h/2.0);
      }
      if(sensor)
      {
         theSensorID = sensor;
      }
      if(principal_point)
      {
         std::vector<ossimString> splitString;
         ossimString tempString(principal_point);
         tempString = tempString.trim();
         tempString.split(splitString, " ");
         if(splitString.size() == 2)
         {
            thePrincipalPoint.x = splitString[0].toDouble();
            thePrincipalPoint.y = splitString[1].toDouble();
         }
      }
      if(pixel_size)
      {
         std::vector<ossimString> splitString;
         ossimString tempString(pixel_size);
         tempString = tempString.trim();
         tempString.split(splitString, " ");
         if(splitString.size() == 1)
         {
            thePixelSize.x = splitString[0].toDouble();
            thePixelSize.y = thePixelSize.x;
         }
         else if(splitString.size() == 2)
         {
            thePixelSize.x = splitString[0].toDouble();
            thePixelSize.y = splitString[1].toDouble();
         }
      }
      if(focal_length)
      {
         theFocalLength = ossimString(focal_length).toDouble();
      }

      cameraKwl.trimAllValues();
      
      
      ossimString regExpression =  ossimString("^(") + "d[0-9]+)";
      vector<ossimString> keys;
      cameraKwl.getSubstringKeyList( keys, regExpression );
      long numberOfDistortions = (long)keys.size();
      int offset = (int)ossimString("d").size();
      ossim_uint32 idx = 0;
      std::vector<int> numberList(numberOfDistortions);
      for(idx = 0; idx < numberList.size();++idx)
      {
         ossimString numberStr(keys[idx].begin() + offset,
                               keys[idx].end());
         numberList[idx] = numberStr.toInt();
      }
      std::sort(numberList.begin(), numberList.end());
      double distance=0.0, distortion=0.0;
     
      for(idx = 0; idx < numberList.size(); ++idx)
      {
         ossimString value = cameraKwl.find(ossimString("d")+ossimString::toString(numberList[idx]));
         if(!value.empty())
         {
            std::istringstream inStr(value.c_str());
            inStr >> distance;
            ossim::skipws(inStr);
            inStr >> distortion;
#if 0
            std::vector<ossimString> splitString;
            ossimString tempString(value);
            tempString = tempString.trim();
            tempString.split(splitString, " ");
            std::cout << splitString.size() << std::endl;
            if(splitString.size() >= 2)
            {
               distance = splitString[0].toDouble();
               distortion = splitString[1].toDouble();
            }
#endif
            
            tool.setValue(distortion, unitType);
            lensKwl.add(ossimString("distance") + ossimString::toString(idx),
                        distance,
                        true);
            lensKwl.add(ossimString("distortion") + ossimString::toString(idx),
                        tool.getMillimeters(),
                        true);
         }
         lensKwl.add("convergence_threshold",
                     .00001,
                     true);
         if(pixel_size)
         {
            lensKwl.add("dxdy",
                        ossimString::toString(thePixelSize.x) + " " +ossimString::toString(thePixelSize.y),
                        true);
         }
         else
         {
            lensKwl.add("dxdy",
                        ".009 .009",
                        true);
         }
      }
      if(theLensDistortion.valid())
      {
         theLensDistortion->loadState(lensKwl,"");
      }
   }
   else
   {
      
      if(principal_point)
      {
         std::vector<ossimString> splitString;
         ossimString tempString(principal_point);
         tempString = tempString.trim();
         tempString.split(splitString, " ");
         if(splitString.size() >= 2)
         {
            thePrincipalPoint.x = splitString[0].toDouble();
            thePrincipalPoint.y = splitString[1].toDouble();
         }
      }
      else
      {
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG) << "No principal_point found" << std::endl;
            return false;
         }
      }
      if(pixel_size)
      {
         std::vector<ossimString> splitString;
         ossimString tempString(pixel_size);
         tempString = tempString.trim();
         tempString.split(splitString, " ");
         if(splitString.size() == 1)
         {
            thePixelSize.x = splitString[0].toDouble();
            thePixelSize.y = thePixelSize.x;
         }
         else if(splitString.size() == 2)
         {
            thePixelSize.x = splitString[0].toDouble();
            thePixelSize.y = splitString[1].toDouble();
         }
      }
      else
      {
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG) << "No pixel size found" << std::endl;
            return false;
         }
      }
      if(focal_length)
      {
         theFocalLength = ossimString(focal_length).toDouble();
      }
      else
      {
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG) << "No focal length found" << std::endl;
            return false;
         }
      }
      
      if(theLensDistortion.valid())
      {
         ossimString lensPrefix = ossimString(prefix)+"distortion.";
         theLensDistortion->loadState(kwl,
                                      lensPrefix.c_str());
      }
   }
   theRefGndPt = thePlatformPosition;
   
   updateModel();

   lineSampleToWorld(theRefImgPt, theRefGndPt);
   if(compute_gsd_flag)
   {
      if(ossimString(compute_gsd_flag).toBool())
      {
         ossimGpt right;
         ossimGpt top;
         lineSampleToWorld(theRefImgPt + ossimDpt(1.0, 0.0),
                           right);
         lineSampleToWorld(theRefImgPt + ossimDpt(1.0, 0.0),
                           top);

         ossimEcefVector horizontal = ossimEcefPoint(theRefGndPt)-ossimEcefPoint(right);
         ossimEcefVector vertical   = ossimEcefPoint(theRefGndPt)-ossimEcefPoint(top);

         theGSD.x = horizontal.length();
         theGSD.y = vertical.length();
         theMeanGSD = (theGSD.x+theGSD.y)*.5;
      }
   }
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "theOmega:              " << theOmega << std::endl
                                          << "thePhi:                " << thePhi   << std::endl
                                          << "theKappa:              " << theKappa << std::endl;
      std::cout << "platform position:     " << thePlatformPosition << std::endl;
      std::cout << "platform position ECF: " << theEcefPlatformPosition << std::endl;
      std::cout << "ossimApplanixModel::loadState: ......... leaving" << std::endl;
   }

   return true;
}

bool ossimApplanixUtmModel::setupOptimizer(const ossimString& init_file)
{
   ossimKeywordlist kwl;
   kwl.addFile(init_file.c_str());

   return loadState(kwl);
}
