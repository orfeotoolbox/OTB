//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  Dave Hicks
//
// Description:  Alpha HRI Sensor Model
//
//*******************************************************************
//  $Id$
#include <ossim/projection/ossimAlphaSensorHRI.h>
#include <ossim/base/ossimException.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/elevation/ossimElevManager.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/matrix/newmatio.h>
#include <ossim/support_data/ossimAlphaSensorSupportData.h>

static ossimTrace traceExec ("ossimAlphaSensorHRI:exec");
static ossimTrace traceDebug("ossimAlphaSensorHRI:debug");

RTTI_DEF1(ossimAlphaSensorHRI, "ossimAlphaSensorHRI", ossimSensorModel);


ossimAlphaSensorHRI::ossimAlphaSensorHRI()
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimAlphaSensorHRI::ossimAlphaSensorHRI DEBUG:" << std::endl;
   }
   initAdjustableParameters();
   theSensorID = "AlphaHRI";
}

ossimAlphaSensorHRI::ossimAlphaSensorHRI(const ossimAlphaSensorHRI& src)
   :
   ossimAlphaSensor(src)
{
}

ossimObject* ossimAlphaSensorHRI::dup()const
{
   return new ossimAlphaSensorHRI(*this);
}

void ossimAlphaSensorHRI::imagingRay(const ossimDpt& imagePoint,
                                     ossimEcefRay& imageRay) const
{
   ossim_float64 line = imagePoint.y;

   ossim_float64 samp = imagePoint.x - theImageSize.x/2;

   // Flip x because raw image is mirrored in sample direction
   samp = -samp;


   // Form camera frame LOS vector
   ossimColumnVector3d camLOS(0.0, samp, -m_adjustedFocalLength);

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
         << "ossimAlphaSensorHRI::imagingRay DEBUG:\n"
         << "  imagePoint = " << imagePoint << "\n"
         << "  imageRay = " << imageRay << "\n"
         << "  camLOS     = " << camLOS << "\n"
         << "  platPos    = " << platPos << "\n"
         << std::endl;
   }

}

void ossimAlphaSensorHRI::worldToLineSample(const ossimGpt& world_point,
                                                  ossimDpt& image_point) const
{   
   // Initialize at middle
   ossim_float64 refL = theImageSize.y/2;
   ossim_float64 drefL = 5;
   int nIter = 0;
   ossimColumnVector3d camLOS;

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

         // Set function value
         Fx[ll] = camLOS[0];
      }

      // Compute numeric 1st derivative & new estimate for reference line (refL)
      ossim_float64 dFx = (Fx[1]-Fx[0]) / drefL;
      refL -= Fx[0]/dFx;

      nIter++;
   }

   ossim_float64 samp = -m_adjustedFocalLength*camLOS[1]/camLOS[2] + theImageSize.x/2;

   // Flip x because raw image is mirrored in sample direction
   samp = theImageSize.x - samp;

   ossimDpt p(samp, refL);
    
   image_point = p;
}

void ossimAlphaSensorHRI::updateModel()
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimAlphaSensorHRI::updateModel DEBUG:" << std::endl;
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
         << "ossimAlphaSensorHRI::updateModel complete..." << std::endl;
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

void ossimAlphaSensorHRI::initAdjustableParameters()
{
   if (traceExec())
      ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimAlphaSensorHRI::initAdjustableParameters: returning..." << std::endl;

   ossimAlphaSensor::initAdjustableParameters();
}

bool ossimAlphaSensorHRI::loadState(const ossimKeywordlist& kwl, const char* prefix)
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimAlphaSensorHRI::loadState DEBUG:" << std::endl;
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
         << "ossimAlphaSensorHRI::loadState complete..." << std::endl;
   }
   
   return true;
}

bool ossimAlphaSensorHRI::saveState(ossimKeywordlist& kwl, const char* prefix)const
{
   ossimAlphaSensor::saveState(kwl, prefix);

   return true;
}

bool ossimAlphaSensorHRI::initialize( const ossimAlphaSensorSupportData& supData )
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
