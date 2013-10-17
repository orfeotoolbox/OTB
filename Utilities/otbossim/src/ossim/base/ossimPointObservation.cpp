//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Hicks
//
// Description: Class container for multi-ray point observation.
//----------------------------------------------------------------------------

#include <ossim/base/ossimPointObservation.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/imaging/ossimImageHandlerRegistry.h>
#include <ossim/base/ossimAdjustableParameterInterface.h>
#include <iostream>
#include <iomanip>


static ossimTrace traceExec  ("ossimPointObservation:exec");
static ossimTrace traceDebug ("ossimPointObservation:debug");


ossimPointObservation::ossimPointObservation() :
   theID(""),
   theScore(0.0)
{
   thePt.makeNan();
   for (int i=0; i<3; ++i) theObsSigmas[i] = 1000.0;
}

ossimPointObservation::ossimPointObservation(const ossimString& anID) : 
   theID(anID),
   theScore(0.0)
{
   thePt.makeNan();
   for (int i=0; i<3; ++i) theObsSigmas[i] = 1000.0;
}

ossimPointObservation::ossimPointObservation(const ossimGpt& aPt,
                                             const ossimString& anID,
                                             const ossimColumnVector3d& latLonHgtSigmas) : 
   thePt(aPt),
   theID(anID),
   theScore(0.0)
{
   for (int i=0; i<3; ++i) theObsSigmas[i] = latLonHgtSigmas[i];    
}

ossimPointObservation::ossimPointObservation(const ossimPointObservation& tpt) : 
   thePt(tpt.thePt),
   theID(tpt.theID),
   theScore(tpt.theScore)
{
   for (int i=0; i<3; ++i) theObsSigmas[i] = tpt.theObsSigmas[i]; 
   theImageHandlers = tpt.theImageHandlers;
   theMeas = tpt.theMeas;  
   theMeasSigmas = tpt.theMeasSigmas;
   theImageFiles = tpt.theImageFiles;
   theNumPars = tpt.theNumPars;
}


ossimPointObservation::~ossimPointObservation()
{
   reset();

   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG)
      << "DEBUG: ~ossimPointObservation(): returning..." << std::endl;
}


void ossimPointObservation::reset()
{
   thePt.makeNan();
   theID = "";
   for (int i=0; i<3; ++i)
      theObsSigmas[i] = 0.0;
   theScore = 0.0;

   for (ossim_uint32 i=0; i<theImageHandlers.size(); ++i)
      theImageHandlers[i] = 0;
   theMeas.clear();
   theMeasSigmas.clear();
   theImageFiles.clear();
   theNumPars.clear();
   theImageHandlers.clear();
}


void ossimPointObservation::addMeasurement(const double& x,
                                           const double& y,
                                           const std::string& imgFile,
                                           const ossimDpt& measSigma)
{
   ossimDpt meas(x, y);
   addMeasurement(meas, imgFile, measSigma);
}


void ossimPointObservation::addMeasurement(const ossimDpt& meas,
                                           const ossimFilename& imgFile,
                                           const ossimDpt& measSigma)
{
   // Image handler
   ossimRefPtr<ossimImageHandler> ih =
      ossimImageHandlerRegistry::instance()->open(imgFile);
   if ( ih.valid() )
   {
      theImageFiles.push_back(imgFile);
      theImageHandlers.push_back(ih);
      
      // Image measurements & sigmas
      theMeas.push_back(meas);
      theMeasSigmas.push_back(measSigma);
      
      // Number of parameters
      ossimAdjustableParameterInterface* adjParInterface =
         ih->getImageGeometry().get()->getAdjustableParameterInterface();
      theNumPars.push_back(adjParInterface->getNumberOfAdjustableParameters());
   }
}


ossimImageGeometry* ossimPointObservation::getImageGeom(const int index)
{
   return theImageHandlers[index]->getImageGeometry().get();
}


void ossimPointObservation::setImageGeom(const int index, ossimImageGeometry* imgGeom)
{
   theImageHandlers[index]->setImageGeometry(imgGeom);
}


void ossimPointObservation::getMeasurement(const int index, NEWMAT::Matrix& meas)const
{
   meas = NEWMAT::Matrix(1, 2);
   meas[0][0] = theMeas[index].x;
   meas[0][1] = theMeas[index].y;
}


void ossimPointObservation::getMeasCov(const int index, NEWMAT::Matrix& cov)const
{
   cov = NEWMAT::Matrix(2,2);
   cov = 0.0;

   cov(1,1) = theMeasSigmas[index].x*theMeasSigmas[index].x;
   cov(2,2) = theMeasSigmas[index].y*theMeasSigmas[index].y;
}


void ossimPointObservation::getObsCov(NEWMAT::Matrix& cov)const
{
   cov = NEWMAT::Matrix(3,3);
   cov = 0.0;

   double stdrp = theObsSigmas[0]/thePt.metersPerDegree().y/DEG_PER_RAD; //lat
   double stdrl = theObsSigmas[1]/thePt.metersPerDegree().x/DEG_PER_RAD; //lon

   cov(1,1) = stdrp*stdrp;
   cov(2,2) = stdrl*stdrl;
   cov(3,3) = theObsSigmas[2]*theObsSigmas[2];
}


void ossimPointObservation::getResiduals(const int index, NEWMAT::Matrix& resid)
{
   ossimDpt computedImagePosition;
   getImageGeom(index)->worldToLocal(refGroundPoint(), computedImagePosition);
   ossimDpt residual  = theMeas[index] - computedImagePosition;

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)<<"\n========getResiduals======== "<<index;
      theMeas[index].print(ossimNotify(ossimNotifyLevel_DEBUG),1);
      computedImagePosition.print(ossimNotify(ossimNotifyLevel_DEBUG),1);
      refGroundPoint().print(ossimNotify(ossimNotifyLevel_DEBUG));
   }

   resid = NEWMAT::Matrix(1, 2);
   resid[0][0] = residual.x;
   resid[0][1] = residual.y;
}


void ossimPointObservation::getParameterPartials(const int index, NEWMAT::Matrix& parPartials)
{
   parPartials = NEWMAT::Matrix(theNumPars[index], 2);
   getImageGeom(index)->computeImageToGroundPartialsWRTAdjParams(parPartials, refGroundPoint());
}


void ossimPointObservation::getObjSpacePartials(const int index, NEWMAT::Matrix& objPartials)
{
   objPartials = NEWMAT::Matrix(3, 2);
   getImageGeom(index)->computeGroundToImagePartials(objPartials, refGroundPoint());
}


std::ostream& ossimPointObservation::print(std::ostream& os) const
{
   // Observation
   os << "\n "<<theID;
   if (ossim::isnan(theScore) == false)
   {
      os << std::setiosflags(std::ios::fixed) << std::setprecision(2);
      os << "\t" << theScore;
   }
   else
   {
      os << "\t" << "nan";
   }
   os << "\t" << dynamic_cast<const ossimGpt&>(*this);

   // Measurements
   for (ossim_uint32 i=0; i<numMeas(); ++i)
   {
      os << "\n\t\t\t";
      theMeas[i].print(os,1);
   }

   return os;
}


std::ostream& operator<<(std::ostream& os, const ossimPointObservation& pt)
{
   return pt.print(os);
}
