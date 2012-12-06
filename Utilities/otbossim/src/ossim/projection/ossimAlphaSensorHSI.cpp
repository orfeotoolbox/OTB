//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  Dave Hicks
//
// Description:  Alpha HSI Sensor Model
//
//*******************************************************************
//  $Id$
#include <ossim/projection/ossimAlphaSensorHSI.h>
#include <ossim/base/ossimException.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/elevation/ossimElevManager.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/matrix/newmatio.h>
#include <ossim/support_data/ossimAlphaSensorSupportData.h>

static ossimTrace traceExec ("ossimAlphaSensorHSI:exec");
static ossimTrace traceDebug("ossimAlphaSensorHSI:debug");

RTTI_DEF1(ossimAlphaSensorHSI, "ossimAlphaSensorHSI", ossimSensorModel);


ossimAlphaSensorHSI::ossimAlphaSensorHSI()
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimAlphaSensorHSI::ossimAlphaSensorHSI DEBUG:" << std::endl;
   }
   initAdjustableParameters();
   theSensorID = "AlphaHSI";
}

ossimAlphaSensorHSI::ossimAlphaSensorHSI(const ossimAlphaSensorHSI& src)
   :
   ossimAlphaSensor(src)
{
}

ossimObject* ossimAlphaSensorHSI::dup()const
{
   return new ossimAlphaSensorHSI(*this);
}

void ossimAlphaSensorHSI::imagingRay(const ossimDpt& imagePoint,
                                     ossimEcefRay& imageRay) const
{
   ossim_float64 line = imagePoint.y;

   // Form camera frame LOS vector
   ossim_float64 scanAngle = getScanAngle(line);
   ossimColumnVector3d camLOS(imagePoint.x - theImageSize.x/2,
                              m_adjustedFocalLength * tan(scanAngle),
                              m_adjustedFocalLength);

   // Compute camera position & orientation matrix
   ossimEcefPoint platPos;
   NEWMAT::Matrix cam2EcfRot;
   getPositionOrientation(line, platPos, cam2EcfRot);

   // Rotate camera vector to ECF
   ossimColumnVector3d ecfLOS = cam2EcfRot * camLOS.unit();

   // Construct ECF image ray
   imageRay.setOrigin(platPos);
   ossimEcefVector ecfRayDir(ecfLOS);
   imageRay.setDirection(ecfRayDir);

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimAlphaSensorHSI::imagingRay DEBUG:\n"
         << "  imagePoint = " << imagePoint << "\n"
         << "  imageRay = " << imageRay << "\n"
         << "  camLOS     = " << camLOS << "\n"
         << "  platPos    = " << platPos << "\n"
         << std::endl;
   }

}

void ossimAlphaSensorHSI::worldToLineSample(const ossimGpt& world_point,
                                                  ossimDpt& image_point) const
{   
   // Initialize at middle
   ossim_float64 refL = theImageSize.y/2;
   ossim_float64 drefL = 5;
   int nIter = 0;
   ossimColumnVector3d camLOS;
   ossimColumnVector3d camLOS1;

   // Iterate using Newton's method
   while (nIter<3)
   {
      ossim_float64 Fx[2];
      ossim_float64 refl[2];
      refl[0] = refL;
      refl[1] = refL + drefL;

      for (int ll=0; ll<2; ++ll)
      {
         // Compute camera position & orientation matrix
         ossimEcefPoint platPos;
         NEWMAT::Matrix cam2EcfRot;
         getPositionOrientation(refl[ll], platPos, cam2EcfRot);

         // Compute ECF vector
         ossimEcefPoint worldPointECF = ossimEcefPoint(world_point);
         ossimColumnVector3d ecfLOS = worldPointECF.data() - platPos.data();

         // Rotate to camera frame
         camLOS = cam2EcfRot.t() * ecfLOS;
         if (ll==0)
            camLOS1 = camLOS;

         // Set function value
         ossim_float64 cScanAngle = atan(camLOS[1]/camLOS[2]);
         ossim_float64 scanAngle = getScanAngle(refl[ll]);
         Fx[ll] = cScanAngle - scanAngle;
      }

      // Compute numeric 1st derivative & new estimate for reference line (refL)
      ossim_float64 dFx = (Fx[1]-Fx[0]) / drefL;
      refL -= Fx[0]/dFx;

      nIter++;
   }

   ossim_float64 samp = m_adjustedFocalLength*camLOS1[0]/camLOS1[2] + theImageSize.x/2;

   ossimDpt p(samp, refL);
    
   image_point = p;
}

ossim_float64 ossimAlphaSensorHSI::getScanAngle(const ossim_float64& line) const
{
   ossim_float64 scanAngle = evalPoly(m_scanPoly, line);

   return scanAngle;
}

void ossimAlphaSensorHSI::updateModel()
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimAlphaSensorHSI::updateModel DEBUG:" << std::endl;
   }
   ossimAlphaSensor::updateModel();  
     
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
         << "ossimAlphaSensorHSI::updateModel complete..." << std::endl;
   }

   // Ref point
   lineSampleToWorld(theRefImgPt, theRefGndPt);


   // Bounding rectangle
   ossimGpt gpt;
   theBoundGndPolygon.clear();
   
   lineSampleToWorld(theImageClipRect.ul(),gpt); //+ossimDpt(-w, -h), gpt);
   theBoundGndPolygon.addPoint(gpt.lond(), gpt.latd());

   lineSampleToWorld(theImageClipRect.ur(),gpt); //+ossimDpt(w, -h), gpt);
   theBoundGndPolygon.addPoint(gpt.lond(), gpt.latd());

   lineSampleToWorld(theImageClipRect.lr(),gpt); //+ossimDpt(w, h), gpt);
   theBoundGndPolygon.addPoint(gpt.lond(), gpt.latd());

   lineSampleToWorld(theImageClipRect.ll(),gpt); //+ossimDpt(-w, h), gpt);
   theBoundGndPolygon.addPoint(gpt.lond(), gpt.latd());
}

void ossimAlphaSensorHSI::initAdjustableParameters()
{
   if (traceExec())
      ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimAlphaSensorHSI::initAdjustableParameters: returning..." << std::endl;

   ossimAlphaSensor::initAdjustableParameters();
}

bool ossimAlphaSensorHSI::loadState(const ossimKeywordlist& kwl, const char* prefix)
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimAlphaSensorHSI::loadState DEBUG:" << std::endl;
   }

   ossimAlphaSensor::loadState(kwl, prefix);
   if(getNumberOfAdjustableParameters() < 1)
   {
      initAdjustableParameters();
   }
   
   updateModel();
   
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimAlphaSensorHSI::loadState complete..." << std::endl;
   }
   
   return true;
}

bool ossimAlphaSensorHSI::saveState(ossimKeywordlist& kwl, const char* prefix)const
{
   ossimAlphaSensor::saveState(kwl, prefix);

   return true;
}

bool ossimAlphaSensorHSI::initialize( const ossimAlphaSensorSupportData& supData )
{
   bool result = true; // Currently no error checking.

   ossimDpt imageSize = supData.getImageSize();
   setImageSize(imageSize);
   setImageRect(ossimDrect(0,0,imageSize.x-1, imageSize.y-1));
   setRefImgPt(ossimDpt(imageSize.x*.5, imageSize.y*.5));
   
   setFov(supData.getFov());
   setRollBias(supData.getRollBias());
   setPitchBias(supData.getPitchBias());
   setHeadingBias(supData.getHeadingBias());
   setSlitRot(supData.getSlitRot());
   
   setRollPoly(supData.getRollPoly());
   setPitchPoly(supData.getPitchPoly());
   setHeadingPoly(supData.getHeadingPoly());
   setLonPoly(supData.getLonPoly());
   setLatPoly(supData.getLatPoly());
   setAltPoly(supData.getAltPoly());
   setScanPoly(supData.getScanPoly());

   updateModel();

   return result;
}
