//*******************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Garrett Potts
//
//*******************************************************************
//  $Id: ossimApplanixEcefModel.cpp 22271 2013-05-27 18:24:22Z gpotts $
#include <sstream>
#include <ossim/projection/ossimApplanixEcefModel.h>
#include <ossim/base/ossimEllipsoid.h>
#include <ossim/base/ossimUnitConversionTool.h>
#include <ossim/base/ossimUnitTypeLut.h>
#include <ossim/base/ossimDatumFactory.h>
#include <ossim/base/ossimDatum.h>
#include <ossim/base/ossimException.h>
#include <ossim/base/ossimLsrSpace.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimGeoidManager.h>
#include <ossim/projection/ossimUtmProjection.h>
#include <ossim/support_data/ossimApplanixEOFile.h>
#include <ossim/base/ossimMatrix4x4.h>
#include <ossim/elevation/ossimElevManager.h>
static ossimTrace traceDebug("ossimApplanixEcefModel:debug");

RTTI_DEF1(ossimApplanixEcefModel, "ossimApplanixEcefModel", ossimSensorModel);

#ifdef OSSIM_ID_ENABLED
static const char OSSIM_ID[] = "$Id: ossimApplanixEcefModel.cpp 22271 2013-05-27 18:24:22Z gpotts $";
#endif

ossimApplanixEcefModel::ossimApplanixEcefModel()
{
   theCompositeMatrix          = ossimMatrix4x4::createIdentity();
   theCompositeMatrixInverse   = ossimMatrix4x4::createIdentity();
   theRoll                     = 0.0;
   thePitch                    = 0.0;
   theHeading                  = 0.0;
   theFocalLength              = 55.0;
   thePixelSize = ossimDpt(.009, .009);
   theEcefPlatformPosition = ossimGpt(0.0,0.0, 1000.0);
   theAdjEcefPlatformPosition = ossimGpt(0.0,0.0, 1000.0);
   theGSD.x = 0.1524;
   theGSD.y = 0.1524;
   theMeanGSD = 0.1524;
   theLensDistortion = new ossimMeanRadialLensDistortion;
   initAdjustableParameters();

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimApplanixEcefModel::ossimApplanixEcefModel DEBUG:" << endl;
#ifdef OSSIM_ID_ENABLED
      ossimNotify(ossimNotifyLevel_DEBUG)<< "OSSIM_ID:  " << OSSIM_ID << endl;
#endif
   }
}
ossimApplanixEcefModel::ossimApplanixEcefModel(const ossimDrect& imageRect,
                                               const ossimGpt& platformPosition,
                                               double roll,
                                               double pitch,
                                               double heading,
                                               const ossimDpt& /* principalPoint */, // in millimeters
                                               double focalLength, // in millimeters
                                               const ossimDpt& pixelSize) // in millimeters
{
   theImageClipRect = imageRect;
   theRefImgPt      = theImageClipRect.midPoint();
   theCompositeMatrix          = ossimMatrix4x4::createIdentity();
   theCompositeMatrixInverse   = ossimMatrix4x4::createIdentity();
   theRoll                     = roll;
   thePitch                    = pitch;
   theHeading                  = heading;
   theFocalLength              = focalLength;
   thePixelSize                = pixelSize;
   theEcefPlatformPosition     = platformPosition;
   theAdjEcefPlatformPosition  = platformPosition;
   theLensDistortion           = new ossimMeanRadialLensDistortion;
   initAdjustableParameters();
   updateModel();

   try
   {
      // Method throws ossimException.
      computeGsd();
   }
   catch (const ossimException& e)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimApplanixEcefModel Constructor caught Exception:\n"
         << e.what() << std::endl;
   }
   
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
      << "ossimApplanixEcefModel::ossimApplanixEcefModel DEBUG:" << endl;
#ifdef OSSIM_ID_ENABLED
      ossimNotify(ossimNotifyLevel_DEBUG)<< "OSSIM_ID:  " << OSSIM_ID << endl;
#endif
   }
}

ossimApplanixEcefModel::ossimApplanixEcefModel(const ossimApplanixEcefModel& src)
   :ossimSensorModel(src)
{
   initAdjustableParameters();
   
   if(src.theLensDistortion.valid())
   {
      theLensDistortion = new ossimMeanRadialLensDistortion(*(src.theLensDistortion.get()));
   }
   else
   {
      theLensDistortion = new ossimMeanRadialLensDistortion();
   }
}

ossimObject* ossimApplanixEcefModel::dup()const
{
   return new ossimApplanixEcefModel(*this);
}

void ossimApplanixEcefModel::imagingRay(const ossimDpt& image_point,
                                    ossimEcefRay&   image_ray) const
{
//    if(traceDebug())
//    {
//       ossimNotify(ossimNotifyLevel_DEBUG) << "ossimApplanixEcefModel::imagingRay: ..... entered" << std::endl;
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
//       ossimNotify(ossimNotifyLevel_DEBUG) << "ossimApplanixEcefModel::imagingRay: ..... leaving" << std::endl;
//    }
}

void ossimApplanixEcefModel::lineSampleToWorld(const ossimDpt& image_point,
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
//   if (!insideImage(image_point))
//   {
//      gpt.makeNan();
//       gpt = extrapolate(image_point);
//      return;
//   }

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

void ossimApplanixEcefModel::lineSampleHeightToWorld(const ossimDpt& image_point,
                                                 const double&   heightEllipsoid,
                                                 ossimGpt&       worldPoint) const
{
   if (!insideImage(image_point))
   {
      worldPoint.makeNan();
//       worldPoint = extrapolate(image_point, heightEllipsoid);
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

void ossimApplanixEcefModel::worldToLineSample(const ossimGpt& world_point,
                                           ossimDpt&       image_point) const
{
   if((theBoundGndPolygon.getNumberOfVertices() > 0)&&
      (!theBoundGndPolygon.hasNans()))
   {
      if (!(theBoundGndPolygon.pointWithin(world_point)))
      {
//         image_point.makeNan();
//          image_point = extrapolate(world_point);
//         return;
      }         
   }
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

void ossimApplanixEcefModel::updateModel()
{
   ossimGpt gpt;
   ossimGpt wgs84Pt;
   double metersPerDegree = wgs84Pt.metersPerDegree().x;
   double degreePerMeter = 1.0/metersPerDegree;
   double latShift = -computeParameterOffset(1)*theMeanGSD*degreePerMeter;
   double lonShift = computeParameterOffset(0)*theMeanGSD*degreePerMeter;

   gpt = theEcefPlatformPosition;
   double height = gpt.height();
   gpt.height(height + computeParameterOffset(5));
   gpt.latd(gpt.latd() + latShift);
   gpt.lond(gpt.lond() + lonShift);
   theAdjEcefPlatformPosition = gpt;
   ossimLsrSpace lsrSpace(theAdjEcefPlatformPosition, theHeading+computeParameterOffset(4));

   // make a left handed roational matrix;
   ossimMatrix4x4 lsrMatrix(lsrSpace.lsrToEcefRotMatrix());
   NEWMAT::Matrix orientation = (ossimMatrix4x4::createRotationXMatrix(thePitch+computeParameterOffset(3), OSSIM_LEFT_HANDED)*
                                 ossimMatrix4x4::createRotationYMatrix(theRoll+computeParameterOffset(2), OSSIM_LEFT_HANDED));
   theCompositeMatrix        = (lsrMatrix.getData()*orientation);
   theCompositeMatrixInverse = theCompositeMatrix.i();

   theBoundGndPolygon.resize(4);
   // ossim_float64 w = theImageClipRect.width()*2.0;
   // ossim_float64 h = theImageClipRect.height()*2.0;
   theExtrapolateImageFlag = false;
   theExtrapolateGroundFlag = false;

   lineSampleToWorld(theImageClipRect.ul(),gpt);//+ossimDpt(-w, -h), gpt);
   theBoundGndPolygon[0] = gpt;
   lineSampleToWorld(theImageClipRect.ur(),gpt);//+ossimDpt(w, -h), gpt);
   theBoundGndPolygon[1] = gpt;
   lineSampleToWorld(theImageClipRect.lr(),gpt);//+ossimDpt(w, h), gpt);
   theBoundGndPolygon[2] = gpt;
   lineSampleToWorld(theImageClipRect.ll(),gpt);//+ossimDpt(-w, h), gpt);
   theBoundGndPolygon[3] = gpt;
}

void ossimApplanixEcefModel::initAdjustableParameters()
{
   
   resizeAdjustableParameterArray(6);
   
   setAdjustableParameter(0, 0.0);
   setParameterDescription(0, "x_offset");
   setParameterUnit(0, "pixels");

   setAdjustableParameter(1, 0.0);
   setParameterDescription(1, "y_offset");
   setParameterUnit(1, "pixels");

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
   setParameterDescription(5, "altitude");
   setParameterUnit(5, "meters");
   
   
   setParameterSigma(0, 20.0);
   setParameterSigma(1, 20.0);
   setParameterSigma(2, .1);
   setParameterSigma(3, .1);
   setParameterSigma(4, .1);
   setParameterSigma(5, 50);
}

void ossimApplanixEcefModel::setPrincipalPoint(ossimDpt principalPoint)
{
   thePrincipalPoint = principalPoint;
}

void ossimApplanixEcefModel::setRollPitchHeading(double roll,
                                                 double pitch,
                                                 double heading)
{
   theRoll    = roll;
   thePitch   = pitch;
   theHeading = heading;
   
   updateModel();
}

void ossimApplanixEcefModel::setPixelSize(const ossimDpt& pixelSize)
{
   thePixelSize = pixelSize;
}

void ossimApplanixEcefModel::setImageRect(const ossimDrect& rect)
{
   theImageClipRect = rect;
   theRefImgPt = rect.midPoint();
}

void ossimApplanixEcefModel::setFocalLength(double focalLength)
{
   theFocalLength = focalLength;
}

void ossimApplanixEcefModel::setPlatformPosition(const ossimGpt& gpt)
{
   theRefGndPt            = gpt;
   theEcefPlatformPosition = gpt;
   updateModel();
   
}

bool ossimApplanixEcefModel::saveState(ossimKeywordlist& kwl,
                                   const char* prefix) const
{
   ossimSensorModel::saveState(kwl, prefix);
   
   kwl.add(prefix, "type", "ossimApplanixEcefModel", true);

   kwl.add(prefix, "roll", theRoll, true);
   kwl.add(prefix, "pitch", thePitch, true);
   kwl.add(prefix, "heading", theHeading, true);
   kwl.add(prefix, "principal_point", ossimString::toString(thePrincipalPoint.x) + " " + ossimString::toString(thePrincipalPoint.y));
   kwl.add(prefix, "pixel_size",      ossimString::toString(thePixelSize.x) + " " + ossimString::toString(thePixelSize.y));
   kwl.add(prefix, "focal_length", theFocalLength);
   kwl.add(prefix, "ecef_platform_position",
           ossimString::toString(theEcefPlatformPosition.x()) + " " +
           ossimString::toString(theEcefPlatformPosition.y()) + " " +
           ossimString::toString(theEcefPlatformPosition.z()));

   if(theLensDistortion.valid())
   {
      ossimString lensPrefix = ossimString(prefix)+"distortion.";
      theLensDistortion->saveState(kwl,
                                   lensPrefix.c_str());
   }
   
   return true;
}

bool ossimApplanixEcefModel::loadState(const ossimKeywordlist& kwl,
                                       const char* prefix)
{
   if(traceDebug())
   {
      std::cout << "ossimApplanixEcefModel::loadState: ......... entered" << std::endl;
   }

   theImageClipRect = ossimDrect(0,0,4076,4091);
   theRefImgPt      = ossimDpt(2046.0, 2038.5);

   ossimSensorModel::loadState(kwl, prefix);
   if(getNumberOfAdjustableParameters() < 1)
   {
      initAdjustableParameters();
   }
   theEcefPlatformPosition    = ossimGpt(0.0,0.0,1000.0);
   theAdjEcefPlatformPosition = ossimGpt(0.0,0.0,1000.0);
   theRoll    = 0.0;
   thePitch   = 0.0;
   theHeading = 0.0;
   // bool computeGsdFlag = false;
   const char* roll              = kwl.find(prefix, "roll");
   const char* pitch             = kwl.find(prefix, "pitch");
   const char* heading           = kwl.find(prefix, "heading");
   const char* principal_point   = kwl.find(prefix, "principal_point");
   const char* pixel_size        = kwl.find(prefix, "pixel_size");
   const char* focal_length      = kwl.find(prefix, "focal_length");
   const char* ecef_platform_position = kwl.find(prefix, "ecef_platform_position");
   const char* latlonh_platform_position = kwl.find(prefix, "latlonh_platform_position");
   const char* compute_gsd_flag  = kwl.find(prefix, "compute_gsd_flag");
   const char* eo_file           = kwl.find(prefix, "eo_file");
   const char* camera_file       = kwl.find(prefix, "camera_file");
   const char* eo_id             = kwl.find(prefix, "eo_id");
   bool result = true;
   if(eo_id)
   {
      theImageID = eo_id;
   }
   if(eo_file)
   {
      ossimApplanixEOFile eoFile;
      if(eoFile.parseFile(ossimFilename(eo_file)))
      {
         ossimRefPtr<ossimApplanixEORecord> record = eoFile.getRecordGivenId(theImageID);
         if(record.valid())
         {
            ossim_int32 rollIdx    = eoFile.getFieldIdx("ROLL");
            ossim_int32 pitchIdx   = eoFile.getFieldIdx("PITCH");
            ossim_int32 headingIdx = eoFile.getFieldIdx("HEADING");
            ossim_int32 xIdx       = eoFile.getFieldIdx("X");
            ossim_int32 yIdx       = eoFile.getFieldIdx("Y");
            ossim_int32 zIdx       = eoFile.getFieldIdx("Z");

            if((rollIdx >= 0)&&
               (pitchIdx >= 0)&&
               (headingIdx >= 0)&&
               (xIdx >= 0)&&
               (yIdx >= 0)&&
               (zIdx >= 0))
            {
               theRoll    = (*record)[rollIdx].toDouble();
               thePitch   = (*record)[pitchIdx].toDouble();
               theHeading = (*record)[headingIdx].toDouble();
               theEcefPlatformPosition = ossimEcefPoint((*record)[xIdx].toDouble(),
                                                        (*record)[yIdx].toDouble(),
                                                        (*record)[zIdx].toDouble());
               theAdjEcefPlatformPosition = theEcefPlatformPosition;
            }
            else
            {
               return false;
            }
         }
         else
         {
            ossimNotify(ossimNotifyLevel_WARN) << "ossimApplanixEcefModel::loadState()  Image id " << theImageID << " not found in eo file " << eo_file << std::endl;
            
            return false;
         }
      }
      else
      {
         return false;
      }
      // computeGsdFlag = true;
   }
   else
   {
      if(roll)
      {
         theRoll = ossimString(roll).toDouble();
      }
      if(pitch)
      {
         thePitch = ossimString(pitch).toDouble();
      }
      if(heading)
      {
         theHeading = ossimString(heading).toDouble();
      }
      if(ecef_platform_position)
      {
         std::vector<ossimString> splitString;
         ossimString tempString(ecef_platform_position);
         tempString.split(splitString, ossimString(" "));
         if(splitString.size() > 2)
         {
            theEcefPlatformPosition  = ossimEcefPoint(splitString[0].toDouble(),
                                                      splitString[1].toDouble(),
                                                      splitString[2].toDouble());
         }
      }
      else if(latlonh_platform_position)
      {
         std::vector<ossimString> splitString;
         ossimString tempString(latlonh_platform_position);
         tempString.split(splitString, ossimString(" "));
         std::string datumString;
         double lat=0.0, lon=0.0, h=0.0;
         if(splitString.size() > 2)
         {
            lat = splitString[0].toDouble();
            lon = splitString[1].toDouble();
            h = splitString[2].toDouble();
         }
         
         theEcefPlatformPosition = ossimGpt(lat,lon,h);
      }
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
         tempString.split(splitString, ossimString(" "));
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
         tempString.split(splitString, ossimString(" "));
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
         tempString.split(splitString, ossimString(" "));
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
                        ossimString(pixel_size) + " " + ossimString(pixel_size),
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
         tempString.split(splitString, ossimString(" "));
         if(splitString.size() == 2)
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
            result = false;
         }
      }
      if(pixel_size)
      {
         std::vector<ossimString> splitString;
         ossimString tempString(pixel_size);
         tempString.split(splitString, ossimString(" "));
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
            result = false;
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
            result = false;
         }
      }
      
      if(theLensDistortion.valid())
      {
         ossimString lensPrefix = ossimString(prefix)+"distortion.";
         if(!theLensDistortion->loadState(kwl,
                                          lensPrefix.c_str()))
         {
            result = false;
         }
      }
   }
   theRefGndPt = theEcefPlatformPosition;
   theRefGndPt.height(0.0);

   updateModel();

   if(compute_gsd_flag)
   {
      if(ossimString(compute_gsd_flag).toBool())
      {
         try
         {
            //---
            // This will set theGSD and theMeanGSD. Method throws
            // ossimException.
            //---
            computeGsd();
         }
         catch (const ossimException& e)
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << "ossimApplanixEcefModel::loadState Caught Exception:\n"
               << e.what() << std::endl;
         }
      }
   }
   
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << std::setprecision(15) << std::endl;
      ossimNotify(ossimNotifyLevel_DEBUG) << "roll:     " << theRoll << std::endl
                                          << "pitch:    " << thePitch << std::endl
                                          << "heading:  " << theHeading << std::endl
                                          << "platform: " << theEcefPlatformPosition << std::endl
                                          << "latlon Platform: " << ossimGpt(theEcefPlatformPosition) << std::endl
                                          << "focal len: " << theFocalLength << std::endl
                                          << "principal: " << thePrincipalPoint << std::endl
                                          << "Ground:    " << ossimGpt(theEcefPlatformPosition) << std::endl;
   }
   return result;
}

bool ossimApplanixEcefModel::setupOptimizer(const ossimString& init_file)
{
   ossimKeywordlist kwl;
   kwl.addFile(init_file.c_str());

   return loadState(kwl);
}
