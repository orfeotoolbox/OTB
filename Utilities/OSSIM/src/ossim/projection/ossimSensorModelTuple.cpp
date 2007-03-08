//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Hicks
//
// Description: Base class for tuple-based ossimSensorModel metric operations.
//
//----------------------------------------------------------------------------
// $Id: ossimSensorModelTuple.cpp 10373 2007-01-25 20:00:06Z dburken $

#include <ossim/projection/ossimSensorModelTuple.h>
#include <ossim/elevation/ossimHgtRef.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimLsrSpace.h>

static ossimTrace traceDebug(ossimString("ossimSensorModelTuple:debug"));
static ossimTrace traceExec(ossimString("ossimSensorModelTuple:exec"));

#ifdef OSSIM_ID_ENABLED
static const char OSSIM_ID[] = "$Id: ossimSensorModelTuple.cpp,v 1.0 2006/10/02  ?????";
#endif

const ossim_uint32 nTableEntries = 21;
const ossim_uint32 nMultiplier = nTableEntries-1;
const ossim_float64 a[nTableEntries]=
  {1.644854,1.645623,1.647912,1.651786,1.657313,
   1.664580,1.673829,1.685227,1.699183,1.716257,
   1.737080,1.762122,1.791522,1.825112,1.862530,
   1.903349,1.947158,1.993595,2.042360,2.093214,2.145966};


//*****************************************************************************
//  METHOD: ossimSensorModelTuple::ossimSensorModelTuple()
//  
//  Constructor.
//  
//*****************************************************************************
ossimSensorModelTuple::ossimSensorModelTuple()
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimSensorModelTuple::ossimSensorModelTuple DEBUG:" << std::endl;
#ifdef OSSIM_ID_ENABLED
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "OSSIM_ID:  " << OSSIM_ID << std::endl;
#endif 
   }
   
}


//*****************************************************************************
//  DESTRUCTOR: ~ossimSensorModelTuple()
//  
//*****************************************************************************
ossimSensorModelTuple::~ossimSensorModelTuple()
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG DESTRUCTOR: ~ossimSensorModelTuple(): entering..." << std::endl;
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG DESTRUCTOR: ~ossimSensorModelTuple(): returning..." << std::endl;
}


//*****************************************************************************
//  METHOD: ossimSensorModelTuple::addImage()
//  
//  Add an image to the tuple.
//  
//*****************************************************************************
void ossimSensorModelTuple::addImage(ossimSensorModel* image)
{
   theImages.push_back(image);
}


//*****************************************************************************
//  METHOD: ossimSensorModelTuple::print()
//  
//  Print info.
//  
//*****************************************************************************
std::ostream& ossimSensorModelTuple::print(std::ostream& out) const
{
   out << "\n ossimSensorModelTuple::print:" << std::endl;
   theImages[0]->print(out);

   return out;
}


//*****************************************************************************
//  METHOD: ossimSensorModelTuple::intersect()
//  
//  Perform multi-image intersection.
//  
//*****************************************************************************
ossimSensorModelTuple::IntersectStatus
ossimSensorModelTuple::intersect(DptSet_t obs,
                                 ossimEcefPoint& pt,
                                 ossim_float64& CE90,
                                 ossim_float64& LE90)
{
   IntersectStatus opOK = OP_FAIL;
   bool covOK = true;
   bool epOK;
   ossim_int32 nImages = obs.size();
   
   // Matrices
   NEWMAT::SymmetricMatrix N(3);
   NEWMAT::SymmetricMatrix BtWB(3);
   NEWMAT::Matrix Ni(3,3);
   NEWMAT::ColumnVector C(3);
   NEWMAT::ColumnVector BtWF(3);
   NEWMAT::ColumnVector F(2);
   NEWMAT::ColumnVector dR(3);
   NEWMAT::Matrix B(2,3);
   NEWMAT::SymmetricMatrix W(2);
   
   // Get a priori ground estimate
   ossimGpt estG;
   theImages[0]->lineSampleHeightToWorld(obs[0], OSSIM_NAN, estG);
   
   for (int iter=0; iter<3; iter++)
   {   
      N = 0.0;
      C = 0.0;

      // Loop over observations
      for (int i=0; i<nImages; i++)
      {
         ossimDpt resid;
         if (!getGroundObsEqComponents(i, iter, obs[i], estG, resid, B, W))
            covOK = false;
         
         F[0] = resid.x;
         F[1] = resid.y;

         // Form coefficient matrix & discrepancy vector
         BtWF << B.t() * W * F;
         BtWB << B.t() * W * B;
         C += BtWF;
         N += BtWB;
      }

      // Solve system
      Ni = invert(N);
      dR = Ni * C;

      // Update estimate
      ossimEcefPoint estECF(estG);
      for (ossim_int32 i=0; i<3; i++)
         estECF[i] += dR[i];
      ossimGpt upd(estECF);
      estG = upd;
      
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG intersect:"<<endl;
         ossimNotify(ossimNotifyLevel_DEBUG) << "  iteration: "<<iter<<endl;
         ossimNotify(ossimNotifyLevel_DEBUG) << "  C:\n"<<C<<endl;
         ossimNotify(ossimNotifyLevel_DEBUG) << "  Ni:\n"<<Ni<<endl;
         ossimNotify(ossimNotifyLevel_DEBUG) << "  dR:\n"<<dR<<endl;
      }
   
   } // iterative loop
   
   
   ossimEcefPoint finalEst(estG);
   pt = finalEst;
   
   // Error propagation
   if (covOK)
      epOK = computeCE_LE(Ni, finalEst, CE90, LE90);
   else
      epOK = false;
   
   // Set operation status
   if (epOK)
      opOK = OP_SUCCESS;
   else
      opOK = ERROR_PROP_FAIL;
   
   return opOK;
}


//*****************************************************************************
//  METHOD: ossimSensorModelTuple::intersect()
//  
//  Perform single-image intersection at specified height.
//  
//*****************************************************************************
ossimSensorModelTuple::IntersectStatus
ossimSensorModelTuple::intersect(const ossimDpt& obs,
                                 const ossim_float64& atHeightAboveEllipsoid,
                                 ossimEcefPoint& pt,
                                 ossim_float64& CE90,
                                 ossim_float64& LE90)
{
   IntersectStatus opOK = OP_FAIL;
   ossimGpt ptG;
   
   // Intersection
   theImages[0]->lineSampleHeightToWorld(obs, atHeightAboveEllipsoid, ptG);
   ossimEcefPoint ptECF(ptG);
   pt = ptECF;
   
   // Error propagation
   NEWMAT::Matrix covMat(3,3);
   bool epOK = false;
   if (computeSingleInterCov(obs, ptG, AT_HGT, covMat))
      // Compute 90% CE/LE
      epOK = computeCE_LE(covMat, ptG, CE90, LE90);
      
   // Set operation status
   if (epOK)
      opOK = OP_SUCCESS;
   else
      opOK = ERROR_PROP_FAIL;
   
   return opOK;
}


//*****************************************************************************
//  METHOD: ossimSensorModelTuple::intersect()
//  
//  Perform single-image intersection at DEM.
//  
//*****************************************************************************
ossimSensorModelTuple::IntersectStatus
ossimSensorModelTuple::intersect(const ossimDpt& obs,
                                 ossimEcefPoint& pt,
                                 ossim_float64& CE90,
                                 ossim_float64& LE90)
{
   IntersectStatus opOK = OP_FAIL;
   ossimGpt ptG;
   
   // Intersection
   theImages[0]->lineSampleToWorld(obs, ptG);
   ossimEcefPoint ptECF(ptG);
   pt = ptECF;
   
   // Error propagation
   NEWMAT::Matrix covMat(3,3);
   bool epOK = false;
   if (computeSingleInterCov(obs, ptG, AT_DEM, covMat))
      // Compute 90% CE/LE
      epOK = computeCE_LE(covMat, ptG, CE90, LE90);
   
   // Set operation status
   if (epOK)
      opOK = OP_SUCCESS;
   else
      opOK = ERROR_PROP_FAIL;
   
   return opOK;
}


//*****************************************************************************
//  METHOD: ossimSensorModelTuple::getGroundObsEqComponents()
//  
//  Extract residuals, partials and weight matrix for observation.
//  
//*****************************************************************************
bool ossimSensorModelTuple::getGroundObsEqComponents(const ossim_int32 img,
                                                     const ossim_int32 iter,
                                                     const ossimDpt& obs,
                                                     const ossimGpt& ptEst,
                                                     ossimDpt& resid,
                                                     NEWMAT::Matrix& B,
                                                     NEWMAT::SymmetricMatrix& W)
{
    //   Initialize the observations on 1st iteration
   if (iter==0)
   {
      ossimGpt ptObs(obs.samp,obs.line);
      theImages[img]->getForwardDeriv(OBS_INIT, ptObs);
   }

   //   Evaluate the partials and residuals
   resid = theImages[img]->getForwardDeriv(EVALUATE, ptEst);

   //   Retrieve the partials
   ossimDpt pWRTx = theImages[img]->getForwardDeriv(P_WRT_X, ptEst);
   ossimDpt pWRTy = theImages[img]->getForwardDeriv(P_WRT_Y, ptEst);
   ossimDpt pWRTz = theImages[img]->getForwardDeriv(P_WRT_Z, ptEst);

   // Get covariance matrix & form weight matrix
   NEWMAT::SymmetricMatrix Cov(2);
   ossimSensorModel::CovMatStatus covStatus;
   covStatus = theImages[img]->getObsCovMat(obs,Cov);
   NEWMAT::Matrix Wfull = invert(Cov);
   W << Wfull;

   B[0][0] = pWRTx.u;
   B[1][0] = pWRTx.v;
   B[0][1] = pWRTy.u;
   B[1][1] = pWRTy.v;
   B[0][2] = pWRTz.u;
   B[1][2] = pWRTz.v;
   
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG getGroundObsEqComponents:"<<endl;
      ossimNotify(ossimNotifyLevel_DEBUG) << "  pWRTx: "<<pWRTx<<endl;
      ossimNotify(ossimNotifyLevel_DEBUG) << "  pWRTy: "<<pWRTy<<endl;
      ossimNotify(ossimNotifyLevel_DEBUG) << "  pWRTz: "<<pWRTz<<endl;
   }
   
   bool covOK = false;
   if (covStatus == ossimSensorModel::COV_FULL)
      covOK = true;
   
   return covOK;
}


//*****************************************************************************
//  METHOD: ossimSensorModelTuple::computeCE_LE()
//  
//  Compute 90% CE/LE (ft).
//  
//*****************************************************************************
bool ossimSensorModelTuple::computeCE_LE(const NEWMAT::Matrix& covMat,
                                         const ossimGpt& posG,
                                         ossim_float64& CE90,
                                         ossim_float64& LE90)
{
   // Define local ENU space at reference point
   ossimLsrSpace enu(posG);
   
   // Propagate ECF cov matrix to local
   NEWMAT::Matrix locCovMat =
      enu.ecefToLsrRotMatrix()*covMat*enu.lsrToEcefRotMatrix();
      
   // Compute eigenvalues (D) of horizontal 2X2 sub-matrix
   //  Note: eigenvectors (columns of V) contain unit vectors
   //        defining orientation of pMin/pMax error ellipse axes
   NEWMAT::SymmetricMatrix S(2);
   S<<locCovMat(1,1)<<locCovMat(1,2)<<locCovMat(2,2);
   NEWMAT::DiagonalMatrix D;
   NEWMAT::Matrix V;
   NEWMAT::Jacobi(S,D,V);
   ossim_float64 pMin = sqrt(D(1,1));
   ossim_float64 pMax = sqrt(D(2,2));
   
   // Evaluate CE function via linear interpolation
   ossim_float64 pRatio = pMin / pMax;
   ossim_uint32 ndx = int(floor(pRatio*nMultiplier));
   ossim_float64 alpha;
   if (ndx == nTableEntries)
      alpha = a[ndx];
   else
      alpha = (pRatio-float(ndx/nMultiplier))*(a[ndx+1]-a[ndx])+a[ndx];
   
   // Compute final CE/LE values
   CE90 = alpha * pMax / MTRS_PER_FT;
   LE90 = sqrt(locCovMat(3,3)) * 1.6449 / MTRS_PER_FT;
   
   return true;
}


//*****************************************************************************
//  METHOD: ossimSensorModelTuple::computeSingleInterCov()
//  
//  Compute single image intersection covariance matrix.
//  
//*****************************************************************************
bool ossimSensorModelTuple::computeSingleInterCov(const ossimDpt& obs,
                                                  const ossimGpt& ptG,
                                                  HeightRefType_t cRefType,
                                                  NEWMAT::Matrix& covMat)
{
   NEWMAT::SymmetricMatrix BtWB(3);
   NEWMAT::Matrix B(2,3);
   NEWMAT::SymmetricMatrix W(2);
   ossimDpt resid;

   // Image contribution
   bool covOK;
   covOK = getGroundObsEqComponents(0, 0, obs, ptG, resid, B, W);
   BtWB << B.t() * W * B;
   
   // Height contribution
   ossimHgtRef hgtRef(cRefType);
   NEWMAT::Matrix St(3,3);
   hgtRef.getHeightCovMatrix(ptG, St);
   NEWMAT::Matrix Sti = invert(St);
   
   // Full covariance matrix
   covMat = invert(BtWB+Sti);
   
   return covOK;
}


//*****************************************************************************
//  METHOD: ossimSensorModelTuple::invert()
//  
//  Perform SVD matrix inversion.
//  
//*****************************************************************************
NEWMAT::Matrix ossimSensorModelTuple::invert(const NEWMAT::Matrix& m) const
{
   ossim_uint32 idx = 0;
   NEWMAT::DiagonalMatrix d;
   NEWMAT::Matrix u;
   NEWMAT::Matrix v;

   // decompose m.t*m into the singular values and vectors.
   NEWMAT::SVD(m, d, u, v, true, true);
   
   // invert the diagonal
   for(idx=0; idx < (ossim_uint32)d.Ncols(); ++idx)
   {
      if(d[idx] > 1e-14) //TBC : use DBL_EPSILON ?
      {
         d[idx] = 1.0/d[idx];
      }
      else
      {
         d[idx] = 0.0;
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << "DEBUG ossimSensorModelTuple::invert(): "
               << "\nsingular matrix in SVD..."
               << std::endl;
         }
      }
   }

   //compute inverse of decomposed m;
   return v*d*u.t();
}

