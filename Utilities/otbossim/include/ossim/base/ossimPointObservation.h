//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Hicks
//
// Description: Class container for multi-ray point observation.
//----------------------------------------------------------------------------
#ifndef ossimPointObservation_HEADER
#define ossimPointObservation_HEADER

#include <iostream>
#include <vector>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimObject.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimColumnVector3d.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimImageGeometry.h>


class OSSIM_DLL ossimPointObservation : public ossimObject
{
public:

   ossimPointObservation();

   ossimPointObservation(const ossimString& anID);

   ossimPointObservation(const ossimGpt& aPt,
                         const ossimString& anID,
                         const ossimColumnVector3d& latLonHgtSigmas);
         
   ossimPointObservation(const ossimPointObservation& tpt);

   ~ossimPointObservation();

   void reset();

   inline ossimGpt& Gpt() { return thePt; }
   inline ossimString ID()const { return theID; }
   inline void setID(const std::string& anID) { theID=anID; }
   inline unsigned int numImages()const { return (unsigned int)theImageFiles.size(); }
   inline unsigned int   numMeas()const { return (unsigned int)theMeas.size(); }

   // Image geometry access
   ossimImageGeometry* getImageGeom(const int index);
   void setImageGeom(const int index, ossimImageGeometry* imgGeom);

   // Image file access
   inline ossimFilename imageFile(const int index)const { return theImageFiles[index]; }

   // Measurement addition
   void addMeasurement(const ossimDpt& meas,
                       const ossimFilename& imgFile,
                       const ossimDpt& measSigma = ossimDpt(1.0,1.0));
   void addMeasurement(const double& x,
                       const double& y,
                       const std::string& imgFile,
                       const ossimDpt& measSigma = ossimDpt(1.0,1.0));

   // Measurement access
   void getResiduals(const int index, NEWMAT::Matrix& resid);

   void getMeasCov(const int index, NEWMAT::Matrix& cov)const;
   void getObsCov(NEWMAT::Matrix& cov)const;

   void getMeasurement(const int index, NEWMAT::Matrix& meas)const;
   inline ossimDpt getMeasurement(const int index)const { return theMeas[index]; }

   inline int numPars(const int index)const { return theNumPars[index]; }


   // Partial derivative access
   void getParameterPartials(const int index, NEWMAT::Matrix& parPartials);
   void  getObjSpacePartials(const int index, NEWMAT::Matrix& objPartials);


   const ossimPointObservation& operator=(const ossimPointObservation&);

   inline void setGroundPoint(const double& lat,
                              const double& lon,
                              const double& hgt)
   {
      thePt.latd(lat);
      thePt.lond(lon);
      thePt.height(hgt);
   }

   inline void setGroundSigmas(const double& latSig,
                               const double& lonSig,
                               const double& hgtSig)
   {
      theObsSigmas[0] = latSig;
      theObsSigmas[1] = lonSig;
      theObsSigmas[2] = hgtSig;
   }

   inline void            setGroundPoint(const ossimGpt& mPt) { thePt = mPt; }
   inline const ossimGpt& getGroundPoint()const               { return thePt; }
   inline       ossimGpt& refGroundPoint()                    { return thePt; }

   // Note: Member variable "theScore" is currently unused.  Eventually, it is
   //       intended as a figure-of-merit representation.
   inline void          setScore(const double& s) { theScore = s; }
   inline const double& getScore()const           { return theScore; }
   inline       double& refScore()                { return theScore; }

   void makeNan() 
   {
      thePt.makeNan();
      theScore=ossim::nan();
   }
   
   bool hasNans()const
   {
      return (thePt.hasNans() || (ossim::isnan(theScore)));
   }
   
   bool isNan()const
   {
      return (thePt.isNan() && (ossim::isnan(theScore)));
   }
   std::ostream& print(std::ostream& os) const;

   friend OSSIMDLLEXPORT std::ostream& operator<<(std::ostream& os,
                                                  const ossimPointObservation& pt);
   /**
    * Method to input the formatted string of the "operator<<".
    *
    * Expected format:  ( ( ossimGpt ), ( ossimDpt ), 0.50000000000000 )
    *                     --*this---- , ----tie-----, ---score--------
    */
   friend OSSIMDLLEXPORT std::istream& operator>>(std::istream& is,
                                                  ossimPointObservation& pt);

protected:
   ossimGpt thePt;
   ossimString theID;
   double theObsSigmas[3];
   double theScore;

   std::vector<ossimDpt> theMeas;
   std::vector<ossimDpt> theMeasSigmas;
   std::vector<ossimFilename> theImageFiles;
   std::vector<int> theNumPars;
   std::vector< ossimRefPtr<ossimImageHandler> > theImageHandlers;
};

#endif // #ifndef ossimPointObservation_HEADER
