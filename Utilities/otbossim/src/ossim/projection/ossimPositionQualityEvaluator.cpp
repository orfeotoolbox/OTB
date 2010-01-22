//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Hicks
//
// Description: Base class for position quality evaluation operations.
//
//----------------------------------------------------------------------------
// $Id$

#include <ossim/projection/ossimPositionQualityEvaluator.h>
#include <ossim/base/ossimDatum.h>
#include <ossim/base/ossimEllipsoid.h>
#include <ossim/base/ossimDms.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimLsrSpace.h>

static ossimTrace traceDebug(ossimString("ossimPositionQualityEvaluator:debug"));
static ossimTrace traceExec(ossimString("ossimPositionQualityEvaluator:exec"));

#ifdef OSSIM_ID_ENABLED
static const char OSSIM_ID[] = "$Id: ossimPositionQualityEvaluator.cpp";
#endif

// 2D 90% function coefficients
const ossim_uint32 nTableEntries = 21;
const ossim_uint32 nMultiplier = nTableEntries-1;
const ossim_float64 table90[nTableEntries]=
  {1.644854,1.645623,1.647912,1.651786,1.657313,
   1.664580,1.673829,1.685227,1.699183,1.716257,
   1.737080,1.762122,1.791522,1.825112,1.862530,
   1.903349,1.947158,1.993595,2.042360,2.093214,2.145966};
   
// 1D conversion factors relative to ONE_SIGMA
const ossim_float64 Fac1D[NUM_PROB_LEVELS] =
   {1.0, 0.6745, 1.6449, 1.96};
   
// 2D conversion factors relative to ONE_SIGMA
const ossim_float64 Fac2D[NUM_PROB_LEVELS] =
   {1.0, 1.1774, 2.1460, 2.4477};
   
// 2D conversion factors relative to P90
const ossim_float64 Fac2D90[NUM_PROB_LEVELS] =
   {0.46598, 0.54865, 1.0, 1.14059};


//*****************************************************************************
//  METHOD: ossimPositionQualityEvaluator::ossimPositionQualityEvaluator()
//  
//  Constructor.
//  
//*****************************************************************************
ossimPositionQualityEvaluator::ossimPositionQualityEvaluator()
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "\nossimPositionQualityEvaluator::ossimPositionQualityEvaluator DEBUG:" << std::endl;
#ifdef OSSIM_ID_ENABLED
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "OSSIM_ID:  " << OSSIM_ID << std::endl;
#endif 
   }
   
   theEvaluatorValid = false;
   theRpcModel.theType = 'N';
}


//*****************************************************************************
//  METHOD: ossimPositionQualityEvaluator::ossimPositionQualityEvaluator()
//  
//  Covariance matrix-based constructor.
//  
//*****************************************************************************
ossimPositionQualityEvaluator::
ossimPositionQualityEvaluator(const ossimEcefPoint& pt,const NEWMAT::Matrix& covMat)
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "\nossimPositionQualityEvaluator::ossimPositionQualityEvaluator DEBUG:" << std::endl;
#ifdef OSSIM_ID_ENABLED
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "OSSIM_ID:  " << OSSIM_ID << std::endl;
#endif 
   }
   theRpcModel.theType = 'N';

   // Set the point
   ossimGpt ptG(pt);
   thePtG = ptG;
   
   // Define the local frame centered on the point
   ossimLsrSpace enu(ptG);
   theLocalFrame = enu;
   
   // Propagate input ECF cov matrix to local
   theCovMat = theLocalFrame.ecefToLsrRotMatrix()*covMat*
               theLocalFrame.lsrToEcefRotMatrix();

   // Compute evaluation parameters
   theEvaluatorValid = decomposeMatrix();
}


//*****************************************************************************
//  METHOD: ossimPositionQualityEvaluator::ossimPositionQualityEvaluator()
//  
//  LOS error/geometry-based constructor.
//  
//*****************************************************************************
ossimPositionQualityEvaluator::
ossimPositionQualityEvaluator(const ossimEcefPoint&      pt,
                              const ossim_float64&       errBiasLOS,
                              const ossim_float64&       errRandLOS,
                              const ossim_float64&       elevAngleLOS,
                              const ossim_float64&       azimAngleLOS,
                              const ossimColumnVector3d& surfN,
                              const NEWMAT::Matrix&      surfCovMat)
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "\nossimPositionQualityEvaluator::ossimPositionQualityEvaluator DEBUG:" << std::endl;
#ifdef OSSIM_ID_ENABLED
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "OSSIM_ID:  " << OSSIM_ID << std::endl;
#endif 
   }
   theRpcModel.theType = 'N';

   // Set the point
   ossimGpt ptG(pt);
   thePtG = ptG;
    
   // Define the local frame centered on the point
   ossimLsrSpace enu(ptG);
   theLocalFrame = enu;

   // Form the covariance matrix
   if (constructMatrix
      (errBiasLOS, errRandLOS, elevAngleLOS, azimAngleLOS, surfN, surfCovMat))
   {
      // Compute evaluation parameters
      theEvaluatorValid = decomposeMatrix();
   }
   else
   {
      theEvaluatorValid = false;
   }
}


//*****************************************************************************
//  METHOD: ossimPositionQualityEvaluator::ossimPositionQualityEvaluator()
//  
//  LOS error/coefficient-based constructor.
//  
//*****************************************************************************
ossimPositionQualityEvaluator::
ossimPositionQualityEvaluator(const ossimEcefPoint&      pt,
                              const ossim_float64&       errBiasLOS,
                              const ossim_float64&       errRandLOS,
                              const pqeRPCModel&         rpc,
                              const ossimColumnVector3d& surfN,
                              const NEWMAT::Matrix&      surfCovMat)
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "\nossimPositionQualityEvaluator::ossimPositionQualityEvaluator DEBUG:" << std::endl;
#ifdef OSSIM_ID_ENABLED
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "OSSIM_ID:  " << OSSIM_ID << std::endl;
#endif 
   }

   // Set the point
   ossimGpt ptG(pt);
   thePtG = ptG;
    
   // Define the local frame centered on the point
   ossimLsrSpace enu(ptG);
   theLocalFrame = enu;
   
   // Set the model parameters
   theRpcModel = rpc;
   
   // Compute the target elevation & azimuth angles
   double elevAngleLOS;
   double azimAngleLOS;
   computeElevAzim(rpc, elevAngleLOS, azimAngleLOS);
   
   // Form the covariance matrix
   if (constructMatrix
      (errBiasLOS, errRandLOS, elevAngleLOS, azimAngleLOS, surfN, surfCovMat))
   {
      // Compute evaluation parameters
      theEvaluatorValid = decomposeMatrix();
   }
   else
   {
      theEvaluatorValid = false;
   }
}


//*****************************************************************************
//  DESTRUCTOR: ~ossimPositionQualityEvaluator()
//  
//*****************************************************************************
ossimPositionQualityEvaluator::~ossimPositionQualityEvaluator()
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG)
      << "DEBUG: ~ossimPositionQualityEvaluator(): returning..." << std::endl;
}


//*****************************************************************************
//  METHOD: ossimPositionQualityEvaluator::print()
//  
//  Print info.
//  
//*****************************************************************************
std::ostream& ossimPositionQualityEvaluator::
print(std::ostream& out) const
{
   out << "\nPositionQualityEvaluator Summary..."<<std::endl;
   out << " theEvaluatorValid: ";
   if (theEvaluatorValid)
      out<<"True"<<std::endl;
   else
      out<<"False"<<std::endl;
   out << " thePtG: "<<thePtG<<std::endl;
   out << " theCovMat [m]:\n"<<theCovMat;
   out << fixed << setprecision(1);
   out << " theEllipse: "<<theEllipse.theSemiMajorAxis<<"  "
                         <<theEllipse.theSemiMinorAxis
                         <<" [m, 1 sigma] at ";
   out << theEllipse.theAzimAngle*DEG_PER_RAD<<" [deg] azimuth"<<endl;

   return out;
}


//*****************************************************************************
//  METHOD: ossimPositionQualityEvaluator::getCovMatrix()
//  
//  Access the covariance matrix.
//  
//*****************************************************************************
bool ossimPositionQualityEvaluator::getCovMatrix(NEWMAT::Matrix& covMat) const
{
   if (theEvaluatorValid)
   {
      covMat = theCovMat;
   }

   return theEvaluatorValid;
}


//*****************************************************************************
//  METHOD: ossimPositionQualityEvaluator::addContributingCovariance()
//  
//  Sum in a contributing covariance matrix.
//  
//*****************************************************************************
bool ossimPositionQualityEvaluator::
addContributingCovariance(NEWMAT::Matrix& covMat)
{
   bool matrixOK = (covMat.Nrows()==3) && (covMat.Nrows()==3);
   if (theEvaluatorValid && matrixOK)
   {
      // Add contribution
      theCovMat += covMat;
   
      // Update the ellipse parameters
      theEvaluatorValid = decomposeMatrix();
   }

   return (theEvaluatorValid && matrixOK);
}


//*****************************************************************************
//  METHOD: ossimPositionQualityEvaluator::addContributingCE_LE()
//  
//  Sum in a contributing CE/LE.
//  
//*****************************************************************************
bool ossimPositionQualityEvaluator::
addContributingCE_LE(const ossim_float64& cCE, const ossim_float64& cLE)
{
   NEWMAT::Matrix covMat(3,3);
   
   formCovMatrixFromCE_LE(cCE, cLE, covMat);

   return addContributingCovariance(covMat);
}


//*****************************************************************************
//  METHOD: ossimPositionQualityEvaluator::subtractContributingCovariance()
//  
//  Subtract out a contributing covariance matrix.
//  
//*****************************************************************************
bool ossimPositionQualityEvaluator::
subtractContributingCovariance(NEWMAT::Matrix& covMat)
{
   bool matrixOK = (covMat.Nrows()==3) && (covMat.Nrows()==3);
   if (theEvaluatorValid && matrixOK)
   {
      // Subtract contribution
      theCovMat -= covMat;
   
      // Update the ellipse parameters
      theEvaluatorValid = decomposeMatrix();
   }

   return (theEvaluatorValid && matrixOK);
}


//*****************************************************************************
//  METHOD: ossimPositionQualityEvaluator::subtractContributingCE_LE()
//  
//  Subtract out a contributing CE/LE.
//  
//*****************************************************************************
bool ossimPositionQualityEvaluator::
subtractContributingCE_LE(const ossim_float64& cCE, const ossim_float64& cLE)
{
   NEWMAT::Matrix covMat(3,3);
   
   formCovMatrixFromCE_LE(cCE, cLE, covMat);

   return subtractContributingCovariance(covMat);
}


//*****************************************************************************
//  METHOD: ossimPositionQualityEvaluator::computeCE_LE()
//  
//  Compute CE/LE (ft) @ pLev probability level.
//  
//*****************************************************************************
bool ossimPositionQualityEvaluator::
computeCE_LE(const pqeProbLev_t pLev, ossim_float64& CE, ossim_float64& LE) const
{
   if (theEvaluatorValid)
   {
      // Compute 1D LE
      LE = sqrt(theCovMat(3,3)) * (ossim_float64)Fac1D[pLev];

      // Compute 2D CE
      CE = (ossim_float64)Fac2D90[pLev] * compute90PCE();
   }
   
   return theEvaluatorValid;
}


//*****************************************************************************
//  METHOD: ossimPositionQualityEvaluator::extractErrorEllipse()
//  
//  Extract error ellipse parameters @ pLev probability level.
//  
//*****************************************************************************
bool ossimPositionQualityEvaluator::
extractErrorEllipse(const pqeProbLev_t pLev, pqeErrorEllipse& ellipse)
{
   if (theEvaluatorValid)
   {
      // Scale the axes
      ellipse.theSemiMinorAxis =
         (ossim_float64)Fac2D[pLev] * theEllipse.theSemiMinorAxis;
      ellipse.theSemiMajorAxis =
         (ossim_float64)Fac2D[pLev] * theEllipse.theSemiMajorAxis;
      
      // Orientation angle
      ellipse.theAzimAngle = theEllipse.theAzimAngle;
      
      // Center position
      ellipse.theCenter = thePtG;
   }

   return theEvaluatorValid;
}


//*****************************************************************************
//  METHOD: ossimPositionQualityEvaluator::extractErrorEllipse()
//  
//  Extract error ellipse parameters @ pLev probability level.
//  
//*****************************************************************************
bool ossimPositionQualityEvaluator::
extractErrorEllipse(const pqeProbLev_t     pLev,
                    const ossim_float64&   angularIncrement,
                          pqeErrorEllipse& ellipse,
                          pqeImageErrorEllipse_t& ellImage)
{
   bool computeImageEllipse = true;
   
   if (theRpcModel.theType == 'N')
      computeImageEllipse = false;
      
   if (theEvaluatorValid && computeImageEllipse)
   {
      // Get object space ellipse parameters
      extractErrorEllipse(pLev, ellipse);
      
      //***
      // Generate the image space ellipse point at 'angularIncrement' spacing
      //***
      int numSteps = 360/(int)angularIncrement;

      // Semi-axes components
      double dxMaj = ellipse.theSemiMajorAxis*sin(ellipse.theAzimAngle);
      double dyMaj = ellipse.theSemiMajorAxis*cos(ellipse.theAzimAngle);
      double dxMin = ellipse.theSemiMinorAxis*sin(ellipse.theAzimAngle+M_PI/2.0);
      double dyMin = ellipse.theSemiMinorAxis*cos(ellipse.theAzimAngle+M_PI/2.0);
      
      for (int j = 0; j<=numSteps; ++j)
      {
      
         // Compute current ENU ellipse point
         double ang = TWO_PI*j/numSteps;
         double sang = sin(ang);
         double cang = cos(ang);
         double x = dxMaj*cang + dxMin*sang;
         double y = dyMaj*cang + dyMin*sang;
         double z = 0.0;
         
         ossimLsrPoint cpLSR(x, y, z, theLocalFrame);
         ossimEcefPoint cp = ossimEcefPoint(cpLSR);
         ossimGpt cpG(cp);
         double lat = cpG.latd();
         double lon = cpG.lond();
         double hgt = cpG.height();
      
         // Normalize the lat, lon, hgt:
         double nlat = (lat - theRpcModel.theLatOffset) /
                        theRpcModel.theLatScale;
         double nlon = (lon - theRpcModel.theLonOffset) /
                        theRpcModel.theLonScale;
         double nhgt;

         if( ossim::isnan(hgt) )
         {
            nhgt = (theRpcModel.theHgtScale - theRpcModel.theHgtOffset) /
                    theRpcModel.theHgtScale;
         }
         else
         {
            nhgt = (hgt - theRpcModel.theHgtOffset) / theRpcModel.theHgtScale;
         }

         //***
         // Compute the normalized line (Un) and sample (Vn)
         //***
         double Pu = polynomial(nlat, nlon, nhgt, theRpcModel.theLineNumCoef);
         double Qu = polynomial(nlat, nlon, nhgt, theRpcModel.theLineDenCoef);
         double Pv = polynomial(nlat, nlon, nhgt, theRpcModel.theSampNumCoef);
         double Qv = polynomial(nlat, nlon, nhgt, theRpcModel.theSampDenCoef);
         double Un  = Pu / Qu;
         double Vn  = Pv / Qv;

         //***
         // Compute the actual line (U) and sample (V):
         //***
         double U  = Un*theRpcModel.theLineScale + theRpcModel.theLineOffset;
         double V  = Vn*theRpcModel.theSampScale + theRpcModel.theSampOffset;

         ossimDpt img(V,U);
         ellImage.push_back(img);
      
      }
   }

   return (theEvaluatorValid && computeImageEllipse);
}


//*****************************************************************************
//  PRIVATE METHOD: ossimPositionQualityEvaluator::decomposeMatrix()
//  
//  Perform eigenvector decomposition and extract ellipse parameters.
//  Compute eigenvalues (D) of horizontal 2X2 sub-matrix
//  Note: eigenvectors (columns of V) contain unit vectors
//        defining orientation of pMin/pMax error ellipse axes
//  
//*****************************************************************************
bool ossimPositionQualityEvaluator::decomposeMatrix()
{
      // Decompose upper left 2X2 partition
      NEWMAT::SymmetricMatrix S(2);
      S<<theCovMat(1,1)<<theCovMat(1,2)<<theCovMat(2,2);
      NEWMAT::DiagonalMatrix D;
      NEWMAT::Matrix V;
      NEWMAT::Jacobi(S,D,V);
      theEllipse.theSemiMinorAxis = sqrt(D(1,1));
      theEllipse.theSemiMajorAxis = sqrt(D(2,2));
      theEigenvectors = V;

      // Compute error ellipse orientation
      //    (ccw rotation of major axis from x-axis)
      ossim_float64 sin2theta = 2.0*theCovMat(1,2);
      ossim_float64 cos2theta = theCovMat(1,1)-theCovMat(2,2);
      if (cos2theta == 0.0)
      {
         return false;
      }
      else
      {
         // Convert "ccw from x-axis" to "cw from y-axis(N)"
         double rotAngle = atan3(sin2theta, cos2theta)/2.0;
         theEllipse.theAzimAngle = M_PI/2.0 - rotAngle;
         if (theEllipse.theAzimAngle < 0.0)
            theEllipse.theAzimAngle += TWO_PI;
      }

      return true;
}


//*****************************************************************************
//  PRIVATE METHOD: ossimPositionQualityEvaluator::constructMatrix()
//  
//  Construct covariance matrix from LOS-centered error components
//  and target acxquistion geometry.
//  
//*****************************************************************************
bool ossimPositionQualityEvaluator::
constructMatrix(const ossim_float64&       errBiasLOS,
                const ossim_float64&       errRandLOS,
                const ossim_float64&       elevAngleLOS,
                const ossim_float64&       azimAngleLOS,
                const ossimColumnVector3d& surfN,
                const NEWMAT::Matrix&      surfCovMat)
{
   bool constructOK = true;
   ossimColumnVector3d lsrNorm(0.0,0.0,1.0);
   
   // Set the total error
   ossim_float64 eTot = sqrt(errBiasLOS*errBiasLOS + errRandLOS*errRandLOS);
   if (eTot == 0.0)
      eTot = 0.001;
   
   // Set the LOS unit vector
   double elC = elevAngleLOS;
   double azC = azimAngleLOS;   
   ossimColumnVector3d  LOS(sin(azC)*cos(elC), cos(azC)*cos(elC), sin(elC));

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)<<"DEBUG: constructMatrix..."<<endl;
      ossimNotify(ossimNotifyLevel_DEBUG)
         <<"  tEl,tAz: "<<elC*DEG_PER_RAD<<"  "<<azC*DEG_PER_RAD<<endl;
      ossimNotify(ossimNotifyLevel_DEBUG)
         <<"  LOS:     "<<LOS<<endl;
      ossimNotify(ossimNotifyLevel_DEBUG)
         <<"  eTot:    "<<eTot<<endl;
   }

   
   // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   // Set ENU-referenced terrain slope normal
   // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   ossimColumnVector3d tSlopeN = surfN.unit();
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) <<"  tSlopeN: "<<tSlopeN<<endl;
   }
   
   // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   // Compute normal to plane containing LOS and terrain normal
   //   this is direction of minor axis unless geometry causes swap
   // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   ossimColumnVector3d pMinU(0,1,0);
   ossimColumnVector3d pMinAxis = LOS.cross(tSlopeN);
   if (pMinAxis.magnitude() > DBL_EPSILON)
   {
      pMinU = pMinAxis.unit();
   }
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) <<"  pMinU: "<<pMinU<<endl;
   }
   
   // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   // Compute nominal major axis direction from cross 
   // product of minor axis vector and terrain slope normal
   // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   ossimColumnVector3d pMaxU = (tSlopeN.cross(pMinU)).unit();
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) <<"  pMaxU: "<<pMaxU<<endl;
   }
   

   // Compute elevation angle relative to terrain plane and check for positive
   double elevAngTerr = acos(pMaxU.dot(LOS));
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)<<"  elev angle rel to surface: "
      <<elevAngTerr*DEG_PER_RAD<<endl;
   }
   
   if (elevAngTerr > 0.0)
   {
      // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      // Compute the LOS error the surface plane  
      // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      double planeErrorL = eTot/sin(elevAngTerr);
      
      // ~~~~~~~~~~~~~~~~~~~~~~~~~~~
      // Project axes to horizontal  
      // ~~~~~~~~~~~~~~~~~~~~~~~~~~~
      ossimColumnVector3d pL = vperp(pMaxU, lsrNorm);
      ossimColumnVector3d pN = vperp(pMinU, lsrNorm);
      
      ossimColumnVector3d eL = pL * planeErrorL;
      ossimColumnVector3d eN = pN * eTot;
      double magL = eL.magnitude();
      double magN = eN.magnitude();

      // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      // Compute vertical component due to intersection geometry 
      // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      ossimColumnVector3d pV = pMaxU - pL;
      ossimColumnVector3d eV = pV * planeErrorL;
      double magV = eV.magnitude();
      
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
         <<"  Projected horizontal/vertical components...."<<endl
         <<"   pL: "<<pL<<"  magL: "<<magL<<endl
         <<"   pN: "<<pN<<"  magN: "<<magN<<endl
         <<"   pV: "<<pV<<"  magV: "<<magV<<endl;
      }




      // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      // Now compute the contributions of the surface uncertainty
      // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

      // Terrain slope angle
      double tSlope = acos(tSlopeN.dot(lsrNorm));

      // Vertical & horizontal components from surface covariance matrix
      double surfSigV = sqrt(surfCovMat(3,3));
      double surfSigH = sqrt((surfCovMat(1,1)+surfCovMat(2,2))/2.0);
      double surfSigV_h = surfSigH * tan(tSlope);

      // Effective total vertical error component includes
      // horizontal uncertainty induced by local slope
      double vSigTot = sqrt(surfSigV*surfSigV + surfSigV_h*surfSigV_h);
      
      // Project to surface normal direction to bring it into the L-surfN plane
      ossimColumnVector3d s_surfN = (lsrNorm.dot(tSlopeN))*tSlopeN;
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG) <<" Surface uncertainty...."<<endl;
         ossimNotify(ossimNotifyLevel_DEBUG) <<"  tSlope angle: "<<tSlope*DEG_PER_RAD<<endl;
         ossimNotify(ossimNotifyLevel_DEBUG) <<"  s_surfN(unit): "<<s_surfN<<endl;
      }
      s_surfN = s_surfN * vSigTot;
      double sigTn = s_surfN.magnitude();
      
      // Compute corresponding error in LOS direction
      double sigVl = sigTn/sin(elevAngTerr);

      // Resolve total vertical error to components based on intersection geometry
      ossimColumnVector3d vSigHvec = sigVl * vperp(LOS, lsrNorm);
      ossimColumnVector3d vSigVvec = sigVl * (LOS.dot(lsrNorm))*lsrNorm;
      double vSigH = vSigHvec.magnitude();
      double vSigV = vSigVvec.magnitude();
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
         <<" s_surfN: "<<s_surfN
         <<"\n vSigHvec: "<<vSigHvec
         <<"\n vSigVvec: "<<vSigVvec<<endl;
      }
      

      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)<<"----------------------------"<<endl;
         ossimNotify(ossimNotifyLevel_DEBUG)<<" surfSigH: "<<surfSigH<<endl;
         ossimNotify(ossimNotifyLevel_DEBUG)<<" surfSigV: "<<surfSigV<<endl;
         ossimNotify(ossimNotifyLevel_DEBUG)<<"  vSigTot: "<<vSigTot<<endl;
         ossimNotify(ossimNotifyLevel_DEBUG)<<"    vSigH: "<<vSigH<<endl;
         ossimNotify(ossimNotifyLevel_DEBUG)<<"    vSigV: "<<vSigV<<endl;
         ossimNotify(ossimNotifyLevel_DEBUG)<<"----------------------------"<<endl;
      }



      // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      // Add vSigH contribution (vSigH in L-surfN plane)
      // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      magL = sqrt(magL*magL + vSigH*vSigH);

      // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      // Compute the axes magnitudes & rotation angle
      //    These parameters represent the horizontal error
      //    due to acquisition geometry & terrain slope
      // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      double theta;
      double pMax, pMin;
      if (magL > magN)
      {
         pMax = magL;
         pMin = magN;
         theta = atan3(pL[1],pL[0]);
      }
      else
      {
         pMax = magN;
         pMin = magL;
         theta = atan3(pN[1],pN[0]);
      }


      // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      // Form final covariance matrix from axes & rotation angle
      // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      NEWMAT::Matrix Cov(2,2);
      NEWMAT::Matrix Vcomp(2,2);
      NEWMAT::DiagonalMatrix Dcomp(2);
      
      Dcomp(1,1) = pMax*pMax;
      Dcomp(2,2) = pMin*pMin;
      Vcomp(1,1) = cos(theta);
      Vcomp(2,1) = sin(theta);
      Vcomp(1,2) =-Vcomp(2,1);
      Vcomp(2,2) = Vcomp(1,1);
      Cov = Vcomp*Dcomp*Vcomp.t();

      // Load full 3X3 matrix in local frame
      NEWMAT::Matrix covMat(3,3);
      covMat(1,1) = Cov(1,1);   
      covMat(1,2) = Cov(1,2);   
      covMat(1,3) = 0.0;   
      covMat(2,1) = Cov(2,1);
      covMat(2,2) = Cov(2,2);   
      covMat(2,3) = 0.0;
      covMat(3,1) = covMat(1,3);
      covMat(3,2) = covMat(2,3);
      covMat(3,3) = magV*magV + vSigV*vSigV;

      // Save the matrix in local frame
      theCovMat = covMat;
      
   }  //end if (elevAngTerr > 0.0)
   else
   {
      constructOK = false;
      ossimNotify(ossimNotifyLevel_WARN)
         << "WARNING: ossimPositionQualityEvaluator::constructMatrix(): "
         << "\n   terrain-referenced elev ang: "<<elevAngTerr*DEG_PER_RAD
         << std::endl;
   }
      

   return constructOK;
}


//*****************************************************************************
//  PRIVATE METHOD: ossimPositionQualityEvaluator::formCovMatrixFromCE_LE()
//  
//  Form 3X3 ENU covariance matrix from CE/LE.
//  
//*****************************************************************************
bool ossimPositionQualityEvaluator::
formCovMatrixFromCE_LE(const ossim_float64&  CE,
                       const ossim_float64&  LE,
                             NEWMAT::Matrix& covMat) const
{         
   covMat = 0.0;
   covMat(1,1) = CE/2.146;
   covMat(2,2) = CE/2.146;
   covMat(3,3) = LE/1.6449;
   covMat(1,1) *= covMat(1,1);
   covMat(2,2) *= covMat(2,2);
   covMat(3,3) *= covMat(3,3);

   return true;
}


//*****************************************************************************
//  PRIVATE METHOD: ossimPositionQualityEvaluator::compute90PCE()
//  
//  Compute CE @ 90% probability level.
//  
//*****************************************************************************
double ossimPositionQualityEvaluator::compute90PCE() const
{         
   // Evaluate CE function via linear interpolation
   ossim_float64 pRatio = theEllipse.theSemiMinorAxis/
                          theEllipse.theSemiMajorAxis;
   ossim_uint32 ndx = int(floor(pRatio*nMultiplier));
   ossim_float64 alpha;

   if (ndx == nMultiplier)
   {
      alpha = table90[ndx];
   }
   else
   {
      ossim_float64 fac = (pRatio-(ossim_float64)ndx/(ossim_float64)nMultiplier) / 0.05;
      alpha = fac * (table90[ndx+1]-table90[ndx]) + table90[ndx];
   }
              
   ossim_float64 CE90 = alpha * theEllipse.theSemiMajorAxis;
   
   return CE90;
}


//*****************************************************************************
//  PRIVATE METHOD: ossimPositionQualityEvaluator::computeElevAzim()
//  
//  Compute elevation and azimuth angles from RPC coefficients.
//  
//*****************************************************************************
bool ossimPositionQualityEvaluator::
computeElevAzim(const pqeRPCModel     rpc,
                      ossim_float64&  elevAngleLOS,
                      ossim_float64&  azimAngleLOS) const
{
   //***
   // Normalize the lat, lon, hgt:
   //***
   double nlat = (thePtG.lat - rpc.theLatOffset) / rpc.theLatScale;
   double nlon = (thePtG.lon - rpc.theLonOffset) / rpc.theLonScale;
   double nhgt;

   if( ossim::isnan(thePtG.hgt) )
   {
      nhgt = (rpc.theHgtScale - rpc.theHgtOffset) / rpc.theHgtScale;
   }
   else
   {
      nhgt = (thePtG.hgt - rpc.theHgtOffset) / rpc.theHgtScale;
   }
   
   //***
   // Compute the numerators & denominators
   //***
   double Pu = polynomial(nlat, nlon, nhgt, rpc.theLineNumCoef);
   double Qu = polynomial(nlat, nlon, nhgt, rpc.theLineDenCoef);
   double Pv = polynomial(nlat, nlon, nhgt, rpc.theSampNumCoef);
   double Qv = polynomial(nlat, nlon, nhgt, rpc.theSampDenCoef);

   //***
   // Compute the partials of each polynomial wrt lat, lon, hgt
   //***
   double dPu_dLat = dPoly_dLat(nlat, nlon, nhgt, rpc.theLineNumCoef);
   double dQu_dLat = dPoly_dLat(nlat, nlon, nhgt, rpc.theLineDenCoef);
   double dPv_dLat = dPoly_dLat(nlat, nlon, nhgt, rpc.theSampNumCoef);
   double dQv_dLat = dPoly_dLat(nlat, nlon, nhgt, rpc.theSampDenCoef);
   double dPu_dLon = dPoly_dLon(nlat, nlon, nhgt, rpc.theLineNumCoef);
   double dQu_dLon = dPoly_dLon(nlat, nlon, nhgt, rpc.theLineDenCoef);
   double dPv_dLon = dPoly_dLon(nlat, nlon, nhgt, rpc.theSampNumCoef);
   double dQv_dLon = dPoly_dLon(nlat, nlon, nhgt, rpc.theSampDenCoef);
   double dPu_dHgt = dPoly_dHgt(nlat, nlon, nhgt, rpc.theLineNumCoef);
   double dQu_dHgt = dPoly_dHgt(nlat, nlon, nhgt, rpc.theLineDenCoef);
   double dPv_dHgt = dPoly_dHgt(nlat, nlon, nhgt, rpc.theSampNumCoef);
   double dQv_dHgt = dPoly_dHgt(nlat, nlon, nhgt, rpc.theSampDenCoef);

   //***
   // Compute partials of quotients U and V wrt lat, lon, hgt 
   //***
   double dU_dLat = (Qu*dPu_dLat - Pu*dQu_dLat)/(Qu*Qu);
   double dU_dLon = (Qu*dPu_dLon - Pu*dQu_dLon)/(Qu*Qu);
   double dU_dHgt = (Qu*dPu_dHgt - Pu*dQu_dHgt)/(Qu*Qu);
   double dV_dLat = (Qv*dPv_dLat - Pv*dQv_dLat)/(Qv*Qv);
   double dV_dLon = (Qv*dPv_dLon - Pv*dQv_dLon)/(Qv*Qv);
   double dV_dHgt = (Qv*dPv_dHgt - Pv*dQv_dHgt)/(Qv*Qv);
   
    //***
    // Apply necessary scale factors 
    //***
   dU_dLat *= rpc.theLineScale/rpc.theLatScale;
   dU_dLon *= rpc.theLineScale/rpc.theLonScale;
   dU_dHgt *= rpc.theLineScale/rpc.theHgtScale;
   dV_dLat *= rpc.theSampScale/rpc.theLatScale;
   dV_dLon *= rpc.theSampScale/rpc.theLonScale;
   dV_dHgt *= rpc.theSampScale/rpc.theHgtScale;

   dU_dLat *= DEG_PER_RAD;
   dU_dLon *= DEG_PER_RAD;
   dV_dLat *= DEG_PER_RAD;
   dV_dLon *= DEG_PER_RAD;
   
   // Save the partials referenced to ECF
   ossimEcefPoint location(thePtG);
   NEWMAT::Matrix jMat(3,3);
   thePtG.datum()->ellipsoid()->jacobianWrtEcef(location, jMat);
   ossimDpt pWRTx;
   ossimDpt pWRTy;
   ossimDpt pWRTz;
   //  Line
   pWRTx.u = dU_dLat*jMat(1,1)+dU_dLon*jMat(2,1)+dU_dHgt*jMat(3,1);
   pWRTy.u = dU_dLat*jMat(1,2)+dU_dLon*jMat(2,2)+dU_dHgt*jMat(3,2);
   pWRTz.u = dU_dLat*jMat(1,3)+dU_dLon*jMat(2,3)+dU_dHgt*jMat(3,3);
   //  Samp
   pWRTx.v = dV_dLat*jMat(1,1)+dV_dLon*jMat(2,1)+dV_dHgt*jMat(3,1);
   pWRTy.v = dV_dLat*jMat(1,2)+dV_dLon*jMat(2,2)+dV_dHgt*jMat(3,2);
   pWRTz.v = dV_dLat*jMat(1,3)+dV_dLon*jMat(2,3)+dV_dHgt*jMat(3,3);

   // Form required partials in local frame
   NEWMAT::Matrix jECF(3,2);
   jECF(1,1) = pWRTx.u;
   jECF(1,2) = pWRTx.v;
   jECF(2,1) = pWRTy.u;
   jECF(2,2) = pWRTy.v;
   jECF(3,1) = pWRTz.u;
   jECF(3,2) = pWRTz.v;
   NEWMAT::Matrix jLSR(3,2);
   jLSR = theLocalFrame.ecefToLsrRotMatrix()*jECF;
   double dU_dx = jLSR(1,1);
   double dU_dy = jLSR(2,1);
   double dU_dz = jLSR(3,1);
   double dV_dx = jLSR(1,2);
   double dV_dy = jLSR(2,2);
   double dV_dz = jLSR(3,2);
   
   // Compute azimuth & elevation angles
   double den = dU_dy*dV_dx - dV_dy*dU_dx;
   double dY  = dU_dx*dV_dz - dV_dx*dU_dz;
   double dX  = dV_dy*dU_dz - dU_dy*dV_dz;
   double dy_dH = dY / den;
   double dx_dH = dX / den;
   
   azimAngleLOS = atan2(dx_dH, dy_dH);
   elevAngleLOS = atan2(1.0, sqrt(dy_dH*dy_dH+dx_dH*dx_dH));

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)<<"DEBUG: computeElevAzim..."<<endl;
      ossimNotify(ossimNotifyLevel_DEBUG)<<
         " el,az = "<<elevAngleLOS*DEG_PER_RAD<<" "<<azimAngleLOS*DEG_PER_RAD<<endl;
   }

   return true;
}


//*****************************************************************************
//  PRIVATE METHOD: ossimPositionQualityEvaluator::vperp()
//  
//  Perpendicular vector component.
//  
//*****************************************************************************
ossimColumnVector3d ossimPositionQualityEvaluator::
vperp(const ossimColumnVector3d& v1, const ossimColumnVector3d& v2) const
{
   
   double scale = v1.dot(v2)/v2.dot(v2);
   ossimColumnVector3d v = v2*scale;
   
   ossimColumnVector3d p = v1 - v;
   
   return p;
}


//*****************************************************************************
//  PRIVATE METHOD: ossimPositionQualityEvaluator::atan3()
//
//  arctan 0-360 counter-clockwise from x-axis
//  
//*****************************************************************************
double ossimPositionQualityEvaluator::atan3(const double  y,
                                            const double  x) const
{
   double u,v,pih=0.5*M_PI,result;

   if (x == 0.0)
      result = M_PI - pih * ossim::sgn(y);
   else
   {
      if (y == 0.0)
      {
         if (x > 0.0)
            result = 0.0;
         else
            result = M_PI;
      }
      else
      {
         u = y/x;
         v = fabs(u);
         result = atan(v);
         result *= v/u;
         if (x < 0.0)
            result += M_PI;
         else
            if (result < 0.0)
               result += TWO_PI;
      }
   }
   
   return result;

}


//*****************************************************************************
//  PRIVATE METHOD: ossimPositionQualityEvaluator::polynomial
//  
//  Evaluates polynomial function.
//  
//*****************************************************************************
double ossimPositionQualityEvaluator::polynomial(
   const double& P,
   const double& L,
   const double& H,
   const double* c) const
{
   double r;

   if (theRpcModel.theType == 'A')
   {
      r = c[ 0]       + c[ 1]*L     + c[ 2]*P     + c[ 3]*H     +
          c[ 4]*L*P   + c[ 5]*L*H   + c[ 6]*P*H   + c[ 7]*L*P*H +
          c[ 8]*L*L   + c[ 9]*P*P   + c[10]*H*H   + c[11]*L*L*L +
          c[12]*L*L*P + c[13]*L*L*H + c[14]*L*P*P + c[15]*P*P*P +
          c[16]*P*P*H + c[17]*L*H*H + c[18]*P*H*H + c[19]*H*H*H;
   }
   else
   {
      r = c[ 0]       + c[ 1]*L     + c[ 2]*P     + c[ 3]*H     +
          c[ 4]*L*P   + c[ 5]*L*H   + c[ 6]*P*H   + c[ 7]*L*L   +
          c[ 8]*P*P   + c[ 9]*H*H   + c[10]*L*P*H + c[11]*L*L*L +
          c[12]*L*P*P + c[13]*L*H*H + c[14]*L*L*P + c[15]*P*P*P +
          c[16]*P*H*H + c[17]*L*L*H + c[18]*P*P*H + c[19]*H*H*H;
   }
   
   return r;
}


//*****************************************************************************
//  PRIVATE METHOD: ossimPositionQualityEvaluator::dPoly_dLat
//  
//  Computes derivative of polynomial WRT normalized latitude P.
//  
//*****************************************************************************
double ossimPositionQualityEvaluator::dPoly_dLat(
   const double& P,
   const double& L,
   const double& H,
   const double* c) const
{
   double dr;

   if (theRpcModel.theType == 'A')
   {
      dr = c[2] + c[4]*L + c[6]*H + c[7]*L*H + 2*c[9]*P + c[12]*L*L +
           2*c[14]*L*P + 3*c[15]*P*P +2*c[16]*P*H + c[18]*H*H;
   }
   else
   {
      dr = c[2] + c[4]*L + c[6]*H + 2*c[8]*P + c[10]*L*H + 2*c[12]*L*P +
           c[14]*L*L + 3*c[15]*P*P + c[16]*H*H + 2*c[18]*P*H;
   }
   
   return dr;
}


//*****************************************************************************
//  PRIVATE METHOD: ossimPositionQualityEvaluator::dPoly_dLon
//  
//  Computes derivative of polynomial WRT normalized longitude L.
//  
//*****************************************************************************
double ossimPositionQualityEvaluator::dPoly_dLon(
   const double& P,
   const double& L,
   const double& H,
   const double* c) const
{
   double dr;

   if (theRpcModel.theType == 'A')
   {
      dr = c[1] + c[4]*P + c[5]*H + c[7]*P*H + 2*c[8]*L + 3*c[11]*L*L +
           2*c[12]*L*P + 2*c[13]*L*H + c[14]*P*P + c[17]*H*H;
   }
   else
   {
      dr = c[1] + c[4]*P + c[5]*H + 2*c[7]*L + c[10]*P*H + 3*c[11]*L*L +
           c[12]*P*P + c[13]*H*H + 2*c[14]*P*L + 2*c[17]*L*H;
   }
   return dr;
}


//*****************************************************************************
//  PRIVATE METHOD: ossimPositionQualityEvaluator::dPoly_dHgt
//  
//  Computes derivative of polynomial WRT normalized height H.
//  
//*****************************************************************************
double ossimPositionQualityEvaluator::dPoly_dHgt(
   const double& P,
   const double& L,
   const double& H,
   const double* c) const
{
   double dr;

   if (theRpcModel.theType == 'A')
   {
      dr = c[3] + c[5]*L + c[6]*P + c[7]*L*P + 2*c[10]*H + c[13]*L*L +
           c[16]*P*P + 2*c[17]*L*H + 2*c[18]*P*H + 3*c[19]*H*H;
   }
   else
   {
      dr = c[3] + c[5]*L + c[6]*P + 2*c[9]*H + c[10]*L*P + 2*c[13]*L*H +
           2*c[16]*P*H + c[17]*L*L + c[18]*P*P + 3*c[19]*H*H;
   }
   return dr;
}
