//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Hicks
// test
//
// Description: Executive class for adjustment operations.
//----------------------------------------------------------------------------
#ifndef ossimAdjustmentExecutive_HEADER
#define ossimAdjustmentExecutive_HEADER

#include <ossim/base/ossimObject.h>
#include <ossim/base/ossimObservationSet.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimString.h>
#include <ossim/matrix/newmat.h>
#include <ossim/matrix/newmatap.h>
#include <ossim/matrix/newmatio.h>

#include <ctime>
#include <vector>
#include <iostream>

class ossimWLSBundleSolution;
class ossimAdjSolutionAttributes;


class OSSIM_DLL ossimAdjustmentExecutive : public ossimObject
{
public:

   /**
    * @brief constructor
    */
   ossimAdjustmentExecutive(std::ostream& report);
   
   /** @brief ossimObservationSet constructor
    *
    * @param obsSet     tiepoint observation set.
    */
   ossimAdjustmentExecutive(ossimObservationSet& obsSet, std::ostream& report);
   

   /** @brief destructor */
   ~ossimAdjustmentExecutive();
   
   /**
    * @brief initialize adjustment solution
    *
    * @param obsSet     tiepoint observation set.
    *
    * @return true on success, false on error.
    */
   bool initializeSolution(ossimObservationSet& obsSet);

   /**
    * @brief run adjustment solution
    *
    * @return true on success, false on error.
    */
   bool runSolution();

   /**
    * @brief summarize solution
    */
   void summarizeSolution() const;


   /**
    * @brief compute mean and RMS error of residuals
    *
    * @param res   image residual matrix.
    *
    * @return true on success, false on error.
    */
   bool computeResidualStatistics(NEWMAT::Matrix& res);


   /**
    * @brief compute SEUW
    *
    * @return   standard error of unit weight.
    */
   double computeSEUW();
   
   /**
    * @brief Print parameter correction method.
    */
   std::ostream& printParameterCorrectionSummary(std::ostream& out) const;
   
   /**
    * @brief Print observation correction method.
    */
   std::ostream& printObservationCorrectionSummary(std::ostream& out) const;
   
   /**
    * @brief Print residuals method.
    */
   std::ostream& printResidualSummary(std::ostream& out) const;


   inline bool isValid() const { return theExecValid; }


protected:
   bool theExecValid;

   // Observation set
   ossimObservationSet* theObsSet;

   // Optimizer
   ossimWLSBundleSolution* theSol;

   // Attribute interface
   ossimAdjSolutionAttributes* theSolAttributes;

   // Status parameters
   double theConvCriteria;
   int    theMaxIter;
   bool   theMaxIterExceeded;
   bool   theSolDiverged;
   bool   theSolConverged;

   // Traits
   int theNumObsInSet;
   int theNumImages;
   int theNumParams;
   int theNumMeasurements;
   int theRankN;

   // Solution arrays
   NEWMAT::Matrix theMeasResiduals; // theNumMeasurements X 2
   NEWMAT::Matrix theObjPartials;   // theNumObjObs*3 X 2
   NEWMAT::Matrix theParPartials;   // theNumImages*(npar/image) X 2

   // Statistics
   double theXrms;
   double theYrms;
   double theXmean;
   double theYmean;
   std::vector<double> theSEUW;

   // Adjustable parameter info
   std::vector<double> theParInitialValues;
   std::vector<double> theParInitialStdDev;
   std::vector<ossimString> theParDesc;

   // Observation info
   std::vector<double> theObsInitialValues;
   std::vector<double> theObsInitialStdDev;
   
   std::ostream& theRep;
                                  
   
   /**
    * @brief Update adjustable parameters.
    */
   bool updateParameters();
   
   /**
    * @brief Update observatin.
    */
   bool updateObservations();

   // Generate time stamp
   inline ossimString timeStamp()const
   {
      char timeString[22];
      time_t now = time(NULL);
      strftime(timeString, 22, "%a %m.%d.%y %H:%M:%S", localtime(&now));
      string timeStamp(timeString);
      return timeStamp;
   }

};

#endif // #ifndef ossimAdjustmentExecutive_HEADER
