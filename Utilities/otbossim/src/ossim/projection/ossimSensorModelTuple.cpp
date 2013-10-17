//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Hicks
//
// Description: Base class for tuple-based ossimSensorModel metric operations.
//
//----------------------------------------------------------------------------
// $Id: ossimSensorModelTuple.cpp 22045 2012-12-28 21:22:35Z dhicks $

#include <ossim/projection/ossimSensorModelTuple.h>
#include <ossim/projection/ossimPositionQualityEvaluator.h>
#include <ossim/projection/ossimRpcModel.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimDatum.h>
#include <ossim/base/ossimEcefPoint.h>
#include <ossim/base/ossimEllipsoid.h>
#include <ossim/base/ossimLsrSpace.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/imaging/ossimImageGeometry.h>

static ossimTrace traceDebug(ossimString("ossimSensorModelTuple:debug"));
static ossimTrace traceExec(ossimString("ossimSensorModelTuple:exec"));

#ifdef OSSIM_ID_ENABLED
static const char OSSIM_ID[] = "$Id: ossimSensorModelTuple.cpp 22045 2012-12-28 21:22:35Z dhicks $";
#endif

ossimRpcPqeInputs::ossimRpcPqeInputs()
   : theRpcElevationAngle(ossim::nan()),
     theRpcAzimuthAngle(ossim::nan()),
     theRpcBiasError(ossim::nan()),
     theRpcRandError(ossim::nan()),
     theSurfaceNormalVector(),
     theSurfaceCovMatrix(3,3)
{
}

ossimRpcPqeInputs::~ossimRpcPqeInputs()
{
}

//*****************************************************************************
//  METHOD: ossimSensorModelTuple::ossimSensorModelTuple()
//  
//  Constructor.
//  
//*****************************************************************************
ossimSensorModelTuple::ossimSensorModelTuple()
:
theNumImages(0),
theSurfCE90(0.0),
theSurfLE90(0.0),
theSurfAccSet(false),
theSurfAccRepresentsNoDEM(false),
theRpcPqeInputs()
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "\nossimSensorModelTuple::ossimSensorModelTuple DEBUG:"
         << std::endl;
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
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG)
      << "DEBUG: ~ossimSensorModelTuple(): entering..." << std::endl;
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG)
      << "DEBUG: ~ossimSensorModelTuple(): returning..." << std::endl;
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
   theNumImages++;
}


//*****************************************************************************
//  METHOD: ossimSensorModelTuple::setIntersectionSurfaceAccuracy()
//  
//  Allow calling program to set the DEM accuracy.
//
//  If both values are negative, it is assumed that no DEM is used.
//  
//*****************************************************************************
bool ossimSensorModelTuple::
setIntersectionSurfaceAccuracy(const ossim_float64& surfCE90,
                               const ossim_float64& surfLE90)
{
   bool setOK = false;
   
   if (surfCE90>=0.0 && surfLE90>=0.0)
   {
      theSurfCE90 = surfCE90;
      theSurfLE90 = surfLE90;
      theSurfAccSet = true;
      theSurfAccRepresentsNoDEM = false;
      setOK = true;
   }
   else
   {
      //***************************************************************
      // RPC_No_DEM_State Demo case
      // In this case, surfCE90/surfLE90 are interpreted differently:
      //    [1] surfCE90 contains the scale divisor for RPC hgtScale
      //    [2] surfLE90 contains the probability level divisor for
      //          RPC hgtScale to yield a 1-sigma value
      //    These coputations are performed in computeSingleInterCov.
      //***************************************************************
      theSurfCE90 = surfCE90;
      theSurfLE90 = surfLE90;
      theSurfAccSet = false;
      theSurfAccRepresentsNoDEM = true;
      setOK = true;
   }
   
   return setOK;
}



void ossimSensorModelTuple::getRpcPqeInputs(ossimRpcPqeInputs& obj) const
{
   obj = theRpcPqeInputs;
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
   for (int i=0; i<theNumImages; ++i)
      theImages[i]->print(out);

   return out;
}


//*****************************************************************************
//  METHOD: ossimSensorModelTuple::intersect()
//  
//  Perform multi-image intersection.
//  
//*****************************************************************************
ossimSensorModelTuple::IntersectStatus ossimSensorModelTuple::
intersect(const DptSet_t   obs,
          ossimEcefPoint&  pt,
          NEWMAT::Matrix&  covMat) const
{
   IntersectStatus opOK = OP_FAIL;
   bool covOK = true;
   bool epOK;
   ossim_int32 nImages = (ossim_int32)obs.size();

   if (nImages > 1)
   {

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
      
      // Get a priori ground estimate using first point
      ossimGpt estG;
      theImages[1]->lineSampleToWorld(obs[1], estG);
      
      for (int iter=0; iter<5; iter++)
      {   
         N = 0.0;
         C = 0.0;

   // cout<<"\n Iter: "<<iter;
         // Loop over observations
         for (int i=0; i<nImages; i++)
         {
            ossimDpt resid;
            // if (!getGroundObsEqComponents(i, iter, obs[i], estG, resid, B, W))
            if (!getGroundObsEqComponents(i, obs[i], estG, resid, B, W))
            {
               covOK = false;
            }
            
            F[0] = resid.x;
            F[1] = resid.y;
   // cout<<"\n  F{"<<i+1<<"}: "<<F[0]<<", "<<F[1];

            // Form coefficient matrix & discrepancy vector
            BtWF << B.t() * W * F;
            BtWB << B.t() * W * B;
            C += BtWF;
            N += BtWB;
         }

         // Solve system
         Ni = invert(N);
         dR = Ni * C;

   // cout<<"\n    dR: ("<<dR[0]<<", "<<dR[1]<<", "<<dR[2]<<")"<<endl;

         // Update estimate
         double latUpd = estG.latd()   - dR[0];
         double lonUpd = estG.lond()   - dR[1];
         double hgtUpd = estG.height() - dR[2];

         estG.latd(latUpd);
         estG.lond(lonUpd);
         estG.height(hgtUpd);


         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << "DEBUG: intersect:\n"
               << "  iteration:\n" << iter
               // << "  C:\n"  << C 
               // << "  Ni:\n" << Ni 
               << "  dR:\n" << dR <<std::endl;
         }
      
      } // iterative loop
      
      // Return intersected point
      ossimEcefPoint finalEst(estG);
      pt = finalEst;
      
      // Return propagated covariance matrix
      if (covOK)
      {
         covMat = Ni;
         epOK = true;
      }
      else
         epOK = false;
      
      // Set operation status
      if (epOK)
         opOK = OP_SUCCESS;
      else
         opOK = ERROR_PROP_FAIL;
   }
   
   return opOK;
}


//*****************************************************************************
//  METHOD: ossimSensorModelTuple::intersect()
//  
//  Perform single-image intersection at specified height.
//  
//*****************************************************************************
ossimSensorModelTuple::IntersectStatus ossimSensorModelTuple::
intersect(const ossim_int32&   img,
          const ossimDpt&      obs,
          const ossim_float64& atHeightAboveEllipsoid,
          ossimEcefPoint&      pt,
          NEWMAT::Matrix&      covMat)
{
   IntersectStatus opOK = OP_FAIL;
   ossimGpt ptG;
   
   // Intersection
   theImages[img]->lineSampleHeightToWorld(obs, atHeightAboveEllipsoid, ptG);
   ossimEcefPoint ptECF(ptG);
   pt = ptECF;
   
   // Error propagation
   bool epOK = computeSingleInterCov(img, obs, ptG, AT_HGT, covMat);
      
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
ossimSensorModelTuple::IntersectStatus ossimSensorModelTuple::
intersect(const ossim_int32&     img,
          const ossimDpt&        obs,
                ossimEcefPoint&  pt,
                NEWMAT::Matrix&  covMat)
{
   IntersectStatus opOK = OP_FAIL;
   ossimGpt ptG;
   
   // Intersection
   theImages[img]->lineSampleToWorld(obs, ptG);
   ossimEcefPoint ptECF(ptG);
   pt = ptECF;
   
   // Error propagation
   bool epOK = computeSingleInterCov(img, obs, ptG, AT_DEM, covMat);
   
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
bool ossimSensorModelTuple::getGroundObsEqComponents(
      const ossim_int32 img,
      const ossimDpt& obs,
      const ossimGpt& ptEst,
      ossimDpt& resid,
      NEWMAT::Matrix& B,
      NEWMAT::SymmetricMatrix& W) const
{
   // Temporary image geometry
   ossimImageGeometry* iGeom = new ossimImageGeometry(NULL, theImages[img]);

   // Evaluate residuals
   ossimDpt computedImg;
   iGeom->worldToLocal(ptEst, computedImg);
   resid = obs - computedImg;

   // Evaluate partials B(2X3)
   NEWMAT::Matrix Bt(3,2);
   iGeom->computeGroundToImagePartials(Bt, ptEst);
   B = Bt.t();

   // Get covariance matrix & form weight matrix
   NEWMAT::SymmetricMatrix Cov(2);
   ossimSensorModel::CovMatStatus covStatus;
   covStatus = theImages[img]->getObsCovMat(obs,Cov);

   if (covStatus == ossimSensorModel::COV_INVALID)
   {
      W = 0.0;
      W(1,1) = 1.0;
      W(2,2) = 1.0;
   }
   else
   {
      NEWMAT::Matrix Wfull = invert(Cov);
      W << Wfull;
   }
   
   bool covOK = false;
   if (covStatus == ossimSensorModel::COV_FULL)
      covOK = true;
   
   return covOK;
}


//*****************************************************************************
//  METHOD: ossimSensorModelTuple::computeSingleInterCov()
//  
//  Compute single image intersection covariance matrix.
//
//*****************************************************************************
bool ossimSensorModelTuple::computeSingleInterCov(
      const ossim_int32& img,
      const ossimDpt&    obs,
      const ossimGpt&    ptG,
      HeightRefType_t    cRefType,
      NEWMAT::Matrix&    covMat)
{
   NEWMAT::SymmetricMatrix BtWB(3);
   NEWMAT::Matrix B(2,3);
   NEWMAT::SymmetricMatrix W(2);
   NEWMAT::Matrix surfCovENU(3,3);
   ossimDpt resid;
   
   bool tCovOK;
   bool covOK;
   
   // Set the height reference
   ossimHgtRef hgtRef(cRefType);


   if (PTR_CAST(ossimRpcModel, theImages[img]))
   {
     ossimRpcModel* model = PTR_CAST(ossimRpcModel, theImages[img]);

     // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     // Special case for handling RPC LOS error components
     // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      ossimGpt ptObs(obs.samp,obs.line);
      theImages[img]->getForwardDeriv(OBS_INIT, ptObs);
      resid = theImages[img]->getForwardDeriv(EVALUATE, ptG);
      ossimDpt pWRTx = theImages[img]->getForwardDeriv(P_WRT_X, ptG);
      ossimDpt pWRTy = theImages[img]->getForwardDeriv(P_WRT_Y, ptG);
      ossimDpt pWRTz = theImages[img]->getForwardDeriv(P_WRT_Z, ptG);
     
      // Form required partials in local frame
      ossimLsrSpace enu(ptG);
      NEWMAT::Matrix jECF(3,2);
      jECF(1,1) = pWRTx.u;
      jECF(1,2) = pWRTx.v;
      jECF(2,1) = pWRTy.u;
      jECF(2,2) = pWRTy.v;
      jECF(3,1) = pWRTz.u;
      jECF(3,2) = pWRTz.v;
      NEWMAT::Matrix jLSR(3,2);
      jLSR = enu.ecefToLsrRotMatrix()*jECF;
      ossim_float64  dU_dx = jLSR(1,1);
      ossim_float64  dU_dy = jLSR(2,1);
      ossim_float64  dU_dz = jLSR(3,1);
      ossim_float64  dV_dx = jLSR(1,2);
      ossim_float64  dV_dy = jLSR(2,2);
      ossim_float64  dV_dz = jLSR(3,2);
      
      // Compute azimuth & elevation angles
      ossim_float64 den = dU_dy*dV_dx - dV_dy*dU_dx;
      ossim_float64 dY  = dU_dx*dV_dz - dV_dx*dU_dz;
      ossim_float64 dX  = dV_dy*dU_dz - dU_dy*dV_dz;
      ossim_float64 dy_dH = dY / den;
      ossim_float64 dx_dH = dX / den;
      ossim_float64 tAz = atan2(dx_dH, dy_dH);
      ossim_float64 tEl = atan2(1.0, sqrt(dy_dH*dy_dH+dx_dH*dx_dH));


      // Get the terrain surface info required by ossimPositionQualityEvaluator
      ossimColumnVector3d surfN = hgtRef.getLocalTerrainNormal(ptG);
      // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      // Currently, the CE/LE must be explicitly set by the
      // setIntersectionSurfaceAccuracy.  When DEM accuracy maintenance
      // is fully supported, the following call should be used.
      //      bool tCovOK = hgtRef.getHeightCovMatrix(ptG, covECF);
      // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      ossim_float64 surfCE;
      ossim_float64 surfLE;
      
      if (theSurfAccRepresentsNoDEM)
      {
         //***************************************************************
         // RPC_No_DEM_State Demo case
         // In this case, surfCE90/surfLE90 are interpreted differently:
         //    [1] surfCE90 contains the scale divisor for RPC hgtScale
         //    [2] surfLE90 contains the probability level divisor for
         //          RPC hgtScale to yield a 1-sigma value
         //***************************************************************
         
         // Reset the surface normal to vertical
         surfN = surfN.zAligned();
         
         // Set approximate surface CE/LE based on RPC parameters
         //  [1] Assume range (scale) only to be 90% vertical error for now
         //  [2] Assume bias could be used for height
         ossimRpcModel::rpcModelStruct rpcPar;
         model->getRpcParameters(rpcPar);
         ossim_float64 hgtRng  = rpcPar.hgtScale;
         surfCE = 0.0;
         ossim_float64 scaledHgtRng = abs(hgtRng/theSurfCE90);
         ossim_float64 scaled1SigmaHgtRng = abs(scaledHgtRng/theSurfLE90);
         surfLE = scaled1SigmaHgtRng*1.6449;
         
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_INFO)
               << "\n computeSingleInterCov() RPC NoDEM state selected..."
               << "\n   RPC Height Scale = " << rpcPar.hgtScale <<" m"
               << "\n    Scale Divisor   = " <<abs(theSurfCE90)
               << "\n    1-Sigma Divisor = "<<abs(theSurfLE90)
               << std::endl;
          }
     }
      else
      {
         surfCE = theSurfCE90;
         surfLE = theSurfLE90;
      }
      
      tCovOK = hgtRef.getSurfaceCovMatrix(surfCE, surfLE, surfCovENU);

      // Evaluate & retrieve the ENU covariance matrix
      if (tCovOK)
      {
         // Store for later retrieval...
         theRpcPqeInputs.theRpcElevationAngle   = tEl*DEG_PER_RAD;
         theRpcPqeInputs.theRpcAzimuthAngle     = tAz*DEG_PER_RAD;
         theRpcPqeInputs.theRpcBiasError        = model->getBiasError();
         theRpcPqeInputs.theRpcRandError        = model->getRandError();
         theRpcPqeInputs.theSurfaceNormalVector = surfN;
         theRpcPqeInputs.theSurfaceCovMatrix    = surfCovENU;
         
          if(traceDebug())
          {
             ossimNotify(ossimNotifyLevel_INFO)
                        << "\n RPC error prop parameters..."
                        << "\n   Elevation Angle  = " << tEl*DEG_PER_RAD<< " deg"
                        << "\n   Azimuth Angle    = " << tAz*DEG_PER_RAD<<" deg"
                        << "\n   RPC Bias Error   = " <<model->getBiasError() <<" m"
                        << "\n   RPC Random Error = " <<model->getRandError()<<" m"
                        << "\n            surfN   = " <<surfN
                        << "\n       surfCovENU   = \n" <<surfCovENU
                        << std::endl;
          }        

         ossimEcefPoint pt(ptG);
         
         ossimPositionQualityEvaluator qev
            (pt,model->getBiasError(),model->getRandError(),
             tEl,tAz,surfN,surfCovENU);
         NEWMAT::Matrix covENU(3,3);
         covOK = qev.getCovMatrix(covENU);

         // Transform to ECF for output
         if (covOK)
         {
            covMat = enu.lsrToEcefRotMatrix()*covENU*enu.ecefToLsrRotMatrix();
         }            
      }
      else
      {
         covOK = false;
      }
   }
   
   else
   {
      // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      // Standard covariance matrix formation
      // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      // Image contribution
      covOK = getGroundObsEqComponents(img, obs, ptG, resid, B, W);
      BtWB << B.t() * W * B;

      // Height contribution
      NEWMAT::Matrix St(3,3);
      // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      // Currently, the CE/LE must be explicitly set by the
      // setIntersectionSurfaceAccuracy.  When DEM accuracy maintenance
      // is fully supported, the following call should be used.
      // hgtRef.getHeightCovMatrix(ptG, St);
      // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      
      // Check for special case of "no DEM" error propagation, only used for RPC
      if (theSurfAccRepresentsNoDEM)
      {
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_INFO)
               << "\n computeSingleInterCov() RPC NoDEM state selected..."
               << " Not valid for this sensor model" << std::endl;
         }
      }
      
      if (hgtRef.getSurfaceCovMatrix(theSurfCE90, theSurfLE90, surfCovENU))
      {
         tCovOK = hgtRef.getSurfaceNormalCovMatrix(ptG, surfCovENU, St);
      }
      else
      {
         tCovOK = false;
      }
         
      if (tCovOK)
      {
         NEWMAT::Matrix Sti = invert(St);
         // Full ECF covariance matrix
         covMat = invert(BtWB + Sti);
      }
      else
      {
         // Don't include height
         covMat = invert(BtWB);
      }
   }
   
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
               << "DEBUG: ossimSensorModelTuple::invert(): "
               << "\nsingular matrix in SVD..."
               << std::endl;
         }
      }
   }

   //compute inverse of decomposed m;
   return v*d*u.t();
}
