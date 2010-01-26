//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Hicks
//
// Description: Base class for position quality evaluation operations.
//----------------------------------------------------------------------------
#ifndef ossimPositionQualityEvaluator_HEADER
#define ossimPositionQualityEvaluator_HEADER

#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimEcefPoint.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimLsrVector.h>
#include <ossim/base/ossimColumnVector3d.h>
#include <ossim/base/ossimMatrix3x3.h>
#include <ossim/matrix/newmat.h>
#include <ossim/matrix/newmatap.h>
#include <ossim/matrix/newmatio.h>


// Probability level specifiers
enum pqeProbLev_t
{
   ONE_SIGMA = 0,
   P50,
   P90,
   P95,
   NUM_PROB_LEVELS
};

// Ground space ellipse
struct pqeErrorEllipse
{
   double   theSemiMinorAxis;
   double   theSemiMajorAxis;
   double   theAzimAngle;
   ossimGpt theCenter;
};

// RPC model parameters
struct pqeRPCModel
{
   double theLineScale;
   double theSampScale;
   double theLatScale;
   double theLonScale;
   double theHgtScale;
   double theLineOffset;
   double theSampOffset;
   double theLatOffset;
   double theLonOffset;
   double theHgtOffset;
   double theLineNumCoef[20];
   double theLineDenCoef[20];
   double theSampNumCoef[20];
   double theSampDenCoef[20];
   char   theType; // A or B
};

// Image space ellipse point set
typedef std::vector<ossimDpt> pqeImageErrorEllipse_t;



class OSSIM_DLL ossimPositionQualityEvaluator
{
public:

   /** @brief default constructor */
   ossimPositionQualityEvaluator();
   
   /** @brief covariance matrix-based constructor
    *
    * @param pt     Current ECF ground estimate.
    * @param covMat 3X3 ECF covariance matrix.
    */
   ossimPositionQualityEvaluator(
      const ossimEcefPoint& pt,
      const NEWMAT::Matrix& covMat);
   
   /** @brief LOS error/geometry-based constructor
    *
    * @param pt           Current ECF ground estimate.
    * @param errBiasLOS   0.68p LOS bias component.
    * @param errRandLOS   0.68p LOS random component.
    * @param elevAngleLOS target elevation angle.
    * @param azimAngleLOS target azimuth angle.
    * @param surfN        surface normal unit vector (defaults to unit Z).
    * @param surfCovMat   surface ENU 3X3 covariance matrix (defaults to zero).
    */
   ossimPositionQualityEvaluator(
      const ossimEcefPoint&      pt,
      const ossim_float64&       errBiasLOS,
      const ossim_float64&       errRandLOS,
      const ossim_float64&       elevAngleLOS,
      const ossim_float64&       azimAngleLOS,
      const ossimColumnVector3d& surfN = ossimColumnVector3d(0,0,1),
      const NEWMAT::Matrix&      surfCovMat = ossimMatrix3x3::createZero());
   
   /** @brief LOS error/coefficient-based constructor
    *
    * @param pt         Current ECF ground estimate.
    * @param errBiasLOS 0.68p LOS bias component.
    * @param errRandLOS 0.68p LOS random component.
    * @param rpc        RPC coefficients.
    * @param surfN      surface normal unit vector (defaults to unit Z).
    * @param surfCovMat surface ENU 3X3 covariance matrix (defaults to zero).
    */
   ossimPositionQualityEvaluator(
      const ossimEcefPoint&      pt,
      const ossim_float64&       errBiasLOS,
      const ossim_float64&       errRandLOS,
      const pqeRPCModel&         rpc,
      const ossimColumnVector3d& surfN = ossimColumnVector3d(0,0,1),
      const NEWMAT::Matrix&      surfCovMat = ossimMatrix3x3::createZero());

   /** @brief virtual destructor */
   ~ossimPositionQualityEvaluator();
   
   /**
    * @brief Add contributing covariance matrix
    *
    * @param covMat 3X3 covariance matrix.
    *
    * @return true on success, false on error.
    */
   bool addContributingCovariance(
      NEWMAT::Matrix& covMat);
   
   /**
    * @brief Add contributing CE/LE
    *
    * @param cCE contributing 90% circular error (m).
    * @param cLE contributing 90% linear error (m).
    *
    * @return true on success, false on error.
    */
   bool addContributingCE_LE(
      const ossim_float64& cCE,
      const ossim_float64& cLE);
   
   /**
    * @brief Subtract contributing covariance matrix
    *
    * @param covMat 3X3 covariance matrix.
    *
    * @return true on success, false on error.
    */
   bool subtractContributingCovariance(
      NEWMAT::Matrix& covMat);
   
   /**
    * @brief Subtract contributing CE/LE
    *
    * @param cCE contributing 90% circular error (m).
    * @param cLE contributing 90% linear error (m).
    *
    * @return true on success, false on error.
    */
   bool subtractContributingCE_LE(
      const ossim_float64& cCE,
      const ossim_float64& cLE);
   
   /**
    * @brief Covariance matrix access
    *
    * @param covMat 3X3 covariance matrix.
    *
    * @return true on success, false on error.
    */
   bool getCovMatrix(
      NEWMAT::Matrix& covMat) const;
   
   /**
    * @brief Compute circular/linear error (CE/LE).
    *
    * @param pLev Probability level.
    * @param CE   pLev% circular error (m).
    * @param LE   pLev% linear error (m).
    *
    * @return true on success, false on error.
    */
   bool computeCE_LE(
      const pqeProbLev_t   pLev,
            ossim_float64& CE,
            ossim_float64& LE) const;

   
   /**
    * @brief Extract error ellipse parameters
    *
    * @param pLev    Probability level.
    * @param ellipse pLev% error ellipse.
    *
    * @return true on success, false on error.
    */
   bool extractErrorEllipse(
      const pqeProbLev_t     pLev,
            pqeErrorEllipse& ellipse);

   
   /**
    * @brief Extract error ellipse parameters; valid only with RPC parameters
    *
    * @param pLev             Probability level.
    * @param angularIncrement Angular increment for ellipse point spacing (deg)
    * @param ellipse          pLev% error ellipse.
    * @param ellImage         pLev% image space error ellipse.
    *
    * @return true on success, false on error.
    */
   bool extractErrorEllipse(
      const pqeProbLev_t            pLev,
      const ossim_float64&          angularIncrement,
            pqeErrorEllipse&        ellipse,
            pqeImageErrorEllipse_t& ellImage);
                                  
   
   /**
    * @brief State accessor.
    */
   inline bool isValid() const { return theEvaluatorValid; }
                                  
   
   /**
    * @brief Print method.
    */
   std::ostream& print(std::ostream& out) const;

protected:

private:
   //***
   // Members
   //***
   bool            theEvaluatorValid;
   ossimGpt        thePtG;
   NEWMAT::Matrix  theCovMat;  //local ENU frame
   ossimLsrSpace   theLocalFrame;
   pqeErrorEllipse theEllipse;
   NEWMAT::Matrix  theEigenvectors;
   pqeRPCModel     theRpcModel;
   
   //***
   // Method for eigenvector decomposition of covariance matrix
   //***
   bool decomposeMatrix();
   
   //***
   // Method for covariance matrix formation from 1-sigma LOS
   // error componenents and imaging geometry
   //***
   bool constructMatrix(const ossim_float64&       errBiasLOS,
                        const ossim_float64&       errRandLOS,
                        const ossim_float64&       elevAngleLOS,
                        const ossim_float64&       azimAngleLOS,
                        const ossimColumnVector3d& surfN,
                        const NEWMAT::Matrix&      surfCovMat);

   //***
   // Method for forming 3X3 ENU covariance matrix from CE/LE
   //***
   bool formCovMatrixFromCE_LE(const ossim_float64&  CE,
                               const ossim_float64&  LE,
                                     NEWMAT::Matrix& covMat) const;

   //***
   // Method for computing 90% CE using interpolating function
   //***
   double compute90PCE() const;
   
   //***
   // Method for computing acquisition angles from RPC coefficients
   //***
   bool computeElevAzim(const pqeRPCModel     rpc,
                              ossim_float64&  elevAngleLOS,
                              ossim_float64&  azimAngleLOS) const;
   
   //***
   // Methods for computing RPC polynomial and its derivatives
   //***
   double polynomial(const double& nlat,
                     const double& nlon,
                     const double& nhgt,
                     const double* coeffs) const;
   double dPoly_dLat(const double& nlat,
                     const double& nlon,
                     const double& nhgt,
                     const double* coeffs) const;
   double dPoly_dLon(const double& nlat,
                     const double& nlon,
                     const double& nhgt,
                     const double* coeffs) const;
   double dPoly_dHgt(const double& nlat,
                     const double& nlon,
                     const double& nhgt,
                     const double* coeffs) const;
    
    
    
   //***
   // Utility methods
   //***
   ossimColumnVector3d vperp(const ossimColumnVector3d& v1,
                             const ossimColumnVector3d& v2) const;
                             
   double atan3(const ossim_float64 y, const ossim_float64 x) const;

};

#endif /* #ifndef ossimPositionQualityEvaluator_HEADER */
