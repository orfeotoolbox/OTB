//*****************************************************************************
// FILE: ossimRpcModel.h
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// AUTHOR: Garrett Potts
//
//*****************************************************************************
//  $Id: ossimRpcSolver.cpp 18960 2011-02-25 12:07:18Z gpotts $

#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <iterator>

#include <ossim/projection/ossimRpcSolver.h>
#include <ossim/projection/ossimRpcModel.h>
#include <ossim/projection/ossimProjection.h>
#include <ossim/matrix/newmatap.h>
#include <ossim/matrix/newmatio.h>
#include <ossim/matrix/newmatnl.h>
#include <ossim/matrix/newmatio.h>
#include <ossim/elevation/ossimElevManager.h>
#include <ossim/support_data/ossimNitfRpcBTag.h>
#include <ossim/imaging/ossimImageGeometry.h>
#include <ossim/base/ossim2dTo2dIdentityTransform.h>
ossimRpcSolver::ossimRpcSolver(bool useElevation,
                               bool useHeightAboveMSLFlag)
{
   theUseElevationFlag   = useElevation;
   theHeightAboveMSLFlag = useHeightAboveMSLFlag;
   theXNumCoeffs.resize(20);
   theXDenCoeffs.resize(20);
   theYNumCoeffs.resize(20);
   theYDenCoeffs.resize(20);

   std::fill(theXNumCoeffs.begin(),
             theXNumCoeffs.end(), 0.0);
   std::fill(theXDenCoeffs.begin(),
             theXDenCoeffs.end(), 0.0);
   std::fill(theYNumCoeffs.begin(),
             theYNumCoeffs.end(), 0.0);
   std::fill(theYDenCoeffs.begin(),
             theYDenCoeffs.end(), 0.0);
   theXNumCoeffs[0] = 1.0;
   theXDenCoeffs[0] = 1.0;
   theYNumCoeffs[0] = 1.0;
   theYDenCoeffs[0] = 1.0;
}

void ossimRpcSolver::solveCoefficients(const ossimDrect& imageBounds,
                                       ossimProjection* proj,
                                       ossim_uint32 xSamples,
                                       ossim_uint32 ySamples,
                                       bool shiftTo0Flag)
{
   ossimRefPtr<ossimImageGeometry> geom = new ossimImageGeometry();
   geom->setProjection(proj);
   solveCoefficients(imageBounds, geom.get(), xSamples, ySamples, shiftTo0Flag);
}

void ossimRpcSolver::solveCoefficients(const ossimDrect& imageBounds,
                                       ossimImageGeometry* geom,
                                       ossim_uint32 xSamples,
                                       ossim_uint32 ySamples,
                                       bool shiftTo0Flag)
{
   std::vector<ossimGpt> theGroundPoints;
   std::vector<ossimDpt> theImagePoints;
   ossim_uint32 x,y;
   ossim_float64 w = imageBounds.width();
   ossim_float64 h = imageBounds.height();
   ossimGpt gpt;
   ossimGpt defaultGround;
   if(ySamples < 1) ySamples = 12;
   if(xSamples < 1) xSamples = 12;
   srand(time(0));
   double xnorm;
   double ynorm;
   ossimDpt ul = imageBounds.ul();
   ossimDpt shiftTo0(-ul.x,
                     -ul.y);
   for(y = 0; y < ySamples; ++y)
   {
      for(x = 0; x < xSamples; ++x)
      {
         if(ySamples > 1)
         {
//            ynorm = (double)y/(double)(ySamples-1.0);
            ynorm = (double)y/(double)(ySamples);
         }
         else
         {
            ynorm = 0.0;
         }
         if(xSamples > 1)
         {
//            xnorm = (double)x/(double)(xSamples-1.0);
            xnorm = (double)x/(double)(xSamples);
         }
         else
         {
            xnorm = 0.0;
         }
         
//          ossimDpt dpt((.25 + .5*xnorm)*w + ul.x,
//                       (.25 + .5*ynorm)*h + ul.y);
         ossimDpt dpt(w*xnorm + ul.x,
                      h*ynorm + ul.y);
         
         geom->localToWorld(dpt, gpt);
         gpt.changeDatum(defaultGround.datum());

         if(shiftTo0Flag)
         {
            theImagePoints.push_back(dpt+shiftTo0);
         }
         else
         {
            theImagePoints.push_back(dpt);
         }
         if(theHeightAboveMSLFlag)
         {
            double h = ossimElevManager::instance()->getHeightAboveMSL(gpt);
            if(ossim::isnan(h) == false)
            {
               gpt.height(h);
            }
         }
         if(gpt.isHgtNan())
         {
            gpt.height(0.0);
         }
         theGroundPoints.push_back(gpt);
      }
   }
   solveCoefficients(theImagePoints,
                     theGroundPoints);
}

void ossimRpcSolver::solveCoefficients(const std::vector<ossimDpt>& imagePoints,
                                       const std::vector<ossimGpt>& groundControlPoints,
                                       const ossimDpt& /* imageShift */)
{
   if((imagePoints.size() != groundControlPoints.size()))
   {
      return;
   }

   // we will first create f which holds the result of f(x,y,z).
   // This basically holds the cooresponding image point for each
   // ground control point.  One for x and a second array for y
   //
   std::vector<double> f[2];

   //  Holds the x, y, z vectors
   //
   std::vector<double> x;
   std::vector<double> y;
   std::vector<double> z;
   ossim_uint32 c = 0;
   f[0].resize(imagePoints.size());
   f[1].resize(imagePoints.size());
   x.resize(imagePoints.size());
   y.resize(imagePoints.size());
   z.resize(imagePoints.size());

   // compute the image bounds for the given image points
   //
   ossimDrect rect(imagePoints);

   // get the widtha dn height that will be used
   // in data normalization
   //
   ossim_float64 w = rect.width();
   ossim_float64 h = rect.height();

   // setup scales for normalization
//   ossim_float64 xScale = w/2.0;
//   ossim_float64 yScale = h/2.0;

   // get the shift for the cneter of the data
   ossimDpt centerImagePoint  = rect.midPoint();
   
   double latSum=0.0;
   double lonSum=0.0;
   double heightSum=0.0;

   // find the center ground  Use elevation only if its enabled
   //
   for(c = 0; c < groundControlPoints.size();++c)
   {
      if(ossim::isnan(groundControlPoints[c].latd()) == false)
      {
         latSum += groundControlPoints[c].latd();
      }
      if(ossim::isnan(groundControlPoints[c].lond()) == false)
      {
         lonSum += groundControlPoints[c].lond();
      }
      if(!groundControlPoints[c].isHgtNan())
      {
         if(theUseElevationFlag)
         {
            heightSum += groundControlPoints[c].height();
         }
      }
   }

   // set the center ground for the offset
   //
   ossimGpt centerGround(latSum/groundControlPoints.size(),
                         lonSum/groundControlPoints.size(),
                         heightSum/groundControlPoints.size());

   // set up ground scales and deltas for normalization
   //
//   ossim_float64 latScale       = 0.0;
//   ossim_float64 lonScale       = 0.0;
//   ossim_float64 heightScale    = 0.0;
   ossim_float64 deltaLat       = 0.0;
   ossim_float64 deltaLon       = 0.0;
   ossim_float64 deltaHeight    = 0.0;
   ossim_float64 maxDeltaLat    = 0.0;
   ossim_float64 maxDeltaLon    = 0.0;
   ossim_float64 maxDeltaHeight = 0.0;
   ossim_float64 heightTest       = 0.0;
   for(c = 0; c < groundControlPoints.size(); ++c)
   {
      deltaLat = (groundControlPoints[c].latd()-centerGround.latd());
      deltaLon = (groundControlPoints[c].lond()-centerGround.lond());
      if(!groundControlPoints[c].isHgtNan())
      {
         if(theUseElevationFlag)
         {
            deltaHeight = groundControlPoints[c].height() - centerGround.height();
            heightTest  = groundControlPoints[c].height();
         }
         else
         {
            deltaHeight = 0.0;
            heightTest  = 0.0;
         }
      }
      else
      {
         deltaHeight = 0.0;
      }
      f[0][c] = (imagePoints[c].x - centerImagePoint.x)/(w/2.0);
      f[1][c] = (imagePoints[c].y - centerImagePoint.y)/(h/2.0);
      
      x[c] = deltaLon;
      y[c] = deltaLat;
      z[c] = deltaHeight;

      if(fabs(deltaLat) > maxDeltaLat) maxDeltaLat          = fabs(deltaLat);
      if(fabs(deltaLon) > maxDeltaLon) maxDeltaLon          = fabs(deltaLon);
      if(fabs(heightTest) > maxDeltaHeight) maxDeltaHeight  = fabs(heightTest);
   }
   bool elevationEnabled = theUseElevationFlag;
   // always normalize, except if too small
   //
   // if max delta is less than a degree set it to 1 degree.
   //
   if(maxDeltaLat < 1.0)    maxDeltaLat = 1.0;
   // if max delta is less than 1 degree then set it to 1.0 degree
   if(maxDeltaLon < 1.0)    maxDeltaLon = 1.0;

   if(fabs(maxDeltaHeight) < FLT_EPSILON) elevationEnabled = false;
   // if max delta is less than a meter then set it to a meter.
   if(maxDeltaHeight < 1.0) maxDeltaHeight = 1.0;

   // set the height scale to something pretty large
   if(!elevationEnabled)
   {
      maxDeltaHeight = 1.0/DBL_EPSILON;
      centerGround.height(0.0);
   }
   // normalize the ground points
   for(c = 0; c < groundControlPoints.size(); ++c)
   {
      x[c] /= maxDeltaLon;
      y[c] /= maxDeltaLat;
      z[c] /= maxDeltaHeight;
   }

   theLatScale    = maxDeltaLat;
   theLonScale    = maxDeltaLon;
   theHeightScale = maxDeltaHeight;

   theImageOffset = centerImagePoint;
   theImageScale  = ossimDpt(w/2.0,
                             h/2.0);
   theGroundOffset = centerGround;

   if(ossim::isnan(theGroundOffset.height()))
   {
      theGroundOffset.height(0.0);
   }

   // now lets solve the coefficients
   //
   std::vector<double> coeffx;
   std::vector<double> coeffy;

   NEWMAT::ColumnVector coeffxVec;
   NEWMAT::ColumnVector coeffyVec;
   // perform a least squares fit for sample values found in f
   // given the world values with variables x, y, z
   //
   solveCoefficients(coeffxVec,
                     f[0],
                     x,
                     y,
                     z);

   
   // perform a least squares fit for line values found in f
   // given the world values with variables x, y, z
   //
   solveCoefficients(coeffyVec,
                     f[1],
                     x,
                     y,
                     z);

   coeffx.resize(coeffxVec.Nrows());
   coeffy.resize(coeffyVec.Nrows());
   
   for(c = 0; c < coeffx.size();++c)
   {
      coeffx[c] = coeffxVec[c];
      coeffy[c] = coeffyVec[c];
   }
   // there are 20 numerator coefficients
   // and 19 denominator coefficients
   // I believe that the very first one for the
   // denominator coefficients is fixed at 1.0
   //
   std::copy(coeffx.begin(),
             coeffx.begin()+20,
             theXNumCoeffs.begin());
   std::copy(coeffx.begin()+20,
             coeffx.begin()+39,
             theXDenCoeffs.begin()+1);
   std::copy(coeffy.begin(),
             coeffy.begin()+20,
             theYNumCoeffs.begin());
   std::copy(coeffy.begin()+20,
             coeffy.begin()+39,
             theYDenCoeffs.begin()+1);
   theXDenCoeffs[0] = 1.0;
   theYDenCoeffs[0] = 1.0;


   // now lets compute the RMSE for the given control points by feeding it
   // back through the modeled RPC
   //
   ossim_float64  sumSquareError = 0.0;
   ossim_uint32 idx = 0;

//    std::cout << "ground offset height = " << theGroundOffset.height()
//              << "Height scale         = " << theHeightScale << std::endl;
   for (idx = 0; idx<imagePoints.size(); idx++)
   {
      ossim_float64 x = (groundControlPoints[idx].lond() - theGroundOffset.lond())/theLonScale;
      ossim_float64 y = (groundControlPoints[idx].latd() - theGroundOffset.latd())/theLatScale;
      ossim_float64 z = (groundControlPoints[idx].height() - theGroundOffset.height())/theHeightScale;
      
      if(ossim::isnan(z))
      {
         z = 0.0;
      }
      else
      {
         z = (z - theGroundOffset.height()/theHeightScale);
      }
       ossim_float64 imageX = ((eval(theXNumCoeffs, x, y, z)/
                                eval(theXDenCoeffs, x, y, z))*theImageScale.x) + theImageOffset.x;
      
       ossim_float64 imageY = ((eval(theYNumCoeffs, x, y, z)/
                                eval(theYDenCoeffs, x, y, z))*theImageScale.y) + theImageOffset.y;
      
      ossimDpt evalPt(imageX, imageY);
      ossim_float64 len = (evalPt - imagePoints[idx]).length();
      
      sumSquareError += (len*len);
   }

   // set the error
   //
   theError = sqrt(sumSquareError/imagePoints.size());
}

ossimImageGeometry* ossimRpcSolver::createRpcModel()const
{
   ossimRpcModel* model = new ossimRpcModel;
   
   model->setAttributes(theImageOffset.x,
                        theImageOffset.y,
                        theImageScale.x,
                        theImageScale.y,
                        theGroundOffset.latd(),
                        theGroundOffset.lond(),
                        theGroundOffset.height(),
                        theLatScale,
                        theLonScale,
                        theHeightScale,
                        theXNumCoeffs,
                        theXDenCoeffs,
                        theYNumCoeffs,
                        theYDenCoeffs);
   return new ossimImageGeometry(new ossim2dTo2dIdentityTransform, model);
}

ossimImageGeometry* ossimRpcSolver::createRpcProjection()const
{
   ossimRpcProjection* proj = new ossimRpcProjection;
   
   proj->setAttributes(theImageOffset.x,
                       theImageOffset.y,
                       theImageScale.x,
                       theImageScale.y,
                       theGroundOffset.latd(),
                       theGroundOffset.lond(),
                       theGroundOffset.height(),
                       theLatScale,
                       theLonScale,
                       theHeightScale,
                       theXNumCoeffs,
                       theXDenCoeffs,
                       theYNumCoeffs,
                       theYDenCoeffs);
   return new ossimImageGeometry(new ossim2dTo2dIdentityTransform, proj);
}

const std::vector<double>& ossimRpcSolver::getImageXNumCoefficients()const
{
   return theXNumCoeffs;
}

const std::vector<double>& ossimRpcSolver::getImageXDenCoefficients()const
{
   return theXDenCoeffs;
}

const std::vector<double>& ossimRpcSolver::getImageYNumCoefficients()const
{
   return theYNumCoeffs;
}

const std::vector<double>& ossimRpcSolver::getImageYDenCoefficients()const
{
   return theYDenCoeffs;
}

double ossimRpcSolver::getImageXOffset()const
{
   return theImageOffset.x;
}

double ossimRpcSolver::getImageYOffset()const
{
   return theImageOffset.y;
}

double ossimRpcSolver::getLatOffset()const
{
   return theGroundOffset.latd();
}

double ossimRpcSolver::getLonOffset()const
{
   return theGroundOffset.lond();
}

double ossimRpcSolver::getHeightOffset()const
{
   return theGroundOffset.height();
}

double ossimRpcSolver::getImageXScale()const
{
   return theImageScale.x;
}

double ossimRpcSolver::getImageYScale()const
{
   return theImageScale.y;
}

double ossimRpcSolver::getLatScale()const
{
   return theLatScale;
}

double ossimRpcSolver::getLonScale()const
{
   return theLonScale;
}

double ossimRpcSolver::getHeightScale()const
{
   return theHeightScale;
}

double ossimRpcSolver::getRmsError()const
{
   return theError;
}

void ossimRpcSolver::solveInitialCoefficients(NEWMAT::ColumnVector& coeff,
                                              const std::vector<double>& f,
                                              const std::vector<double>& x,
                                              const std::vector<double>& y,
                                              const std::vector<double>& z)const
{
   ossim_uint32 idx = 0;
   NEWMAT::Matrix m;
   NEWMAT::ColumnVector r((int)f.size());
   for(idx = 0; idx < f.size(); ++idx)
   {
      r[idx] = f[idx];
   }
   setupSystemOfEquations(m,
                          r,
                          x,
                          y,
                          z);
   
   coeff = invert(m.t()*m)*m.t()*r;
}

void ossimRpcSolver::solveCoefficients(NEWMAT::ColumnVector& coeff,
                                       const std::vector<double>& f,
                                       const std::vector<double>& x,
                                       const std::vector<double>& y,
                                       const std::vector<double>& z)const
{
   // this is an iterative  linear least square fit.  We really pobably need
   // a nonlinear fit instead
   //
   ossim_uint32 idx = 0;
   NEWMAT::Matrix m;

   NEWMAT::ColumnVector r((int)f.size());

   for(idx = 0; idx < f.size(); ++idx)
   {
      r[idx] = f[idx];
   }

   NEWMAT::ColumnVector tempCoeff;
   NEWMAT::DiagonalMatrix weights((int)f.size());
   NEWMAT::ColumnVector denominator(20);

   // initialize the weight matrix to the identity
   //
   for(idx = 0; idx < f.size(); ++idx)
   {
      weights[idx] = 1.0;
   }

   double residualValue = 1.0/FLT_EPSILON;
   ossim_uint32 iterations = 0;
   NEWMAT::Matrix w2;
   do
   {
      w2 = weights*weights;

      // sets up the matrix to hold the system of
      // equations
      setupSystemOfEquations(m,
                             r,
                             x,
                             y,
                             z);

      // solve the least squares solution.  Note: the invert is used
      // to do a Singular Value Decomposition for the inverse since the
      // matrix is more than likely singular.  Slower but more robust
      //
      tempCoeff = invert(m.t()*w2*m)*m.t()*w2*r;

      // set up the weight matrix by using the denominator
      //
      for(idx = 0; idx < 19; ++idx)
      {
         denominator[idx+1] = tempCoeff[20+idx];
      }
      denominator[0] = 1.0;
      
      setupWeightMatrix(weights,
                        denominator,
                        r,
                        x,
                        y,
                        z);

      // compute the residual
      //
      NEWMAT::ColumnVector residual = m.t()*w2*(m*tempCoeff-r);

      // now get the innerproduct
      //
      NEWMAT::Matrix tempRes = (residual.t()*residual);
      residualValue = tempRes[0][0];

      ++iterations;
      
   }while((residualValue >FLT_EPSILON)&&
          (iterations < 10));
   coeff = tempCoeff;

}

NEWMAT::Matrix ossimRpcSolver::invert(const NEWMAT::Matrix& m)const
{
   ossim_uint32 idx = 0;
   NEWMAT::DiagonalMatrix d;
   NEWMAT::Matrix u;
   NEWMAT::Matrix v;

   // decompose m.t*m which is stored in Temp into the singular values and vectors.
   //
   NEWMAT::SVD(m, d, u, v, true, true);
   
   // invert the diagonal
   // this is just doing the reciprical fo all diagonal components and store back int
   // d.  ths compute d inverse.
   //
   for(idx=0; idx < (ossim_uint32)d.Ncols(); ++idx)
   {
      if(d[idx] > FLT_EPSILON)
      {
         d[idx] = 1.0/d[idx];
      }
      else
      {
         d[idx] = 0.0;
      }
   }

   //compute inverse of decomposed m;
   return v*d*u.t();
}


void ossimRpcSolver::setupSystemOfEquations(NEWMAT::Matrix& equations,
                                            const NEWMAT::ColumnVector& f,
                                            const std::vector<double>& x,
                                            const std::vector<double>& y,
                                            const std::vector<double>& z)const
{
   ossim_uint32 idx;
   equations.ReSize(f.Nrows(),
                    39);
   
   for(idx = 0; idx < (ossim_uint32)f.Nrows();++idx)
   {
      equations[idx][0]  = 1;
      equations[idx][1]  = x[idx];
      equations[idx][2]  = y[idx];
      equations[idx][3]  = z[idx];
      equations[idx][4]  = x[idx]*y[idx];
      equations[idx][5]  = x[idx]*z[idx];
      equations[idx][6]  = y[idx]*z[idx];
      equations[idx][7]  = x[idx]*x[idx];
      equations[idx][8]  = y[idx]*y[idx];
      equations[idx][9]  = z[idx]*z[idx];
      equations[idx][10] = x[idx]*y[idx]*z[idx];
      equations[idx][11] = x[idx]*x[idx]*x[idx];
      equations[idx][12] = x[idx]*y[idx]*y[idx];
      equations[idx][13] = x[idx]*z[idx]*z[idx];
      equations[idx][14] = x[idx]*x[idx]*y[idx];
      equations[idx][15] = y[idx]*y[idx]*y[idx];
      equations[idx][16] = y[idx]*z[idx]*z[idx];
      equations[idx][17] = x[idx]*x[idx]*z[idx];
      equations[idx][18] = y[idx]*y[idx]*z[idx];
      equations[idx][19] = z[idx]*z[idx]*z[idx];
      equations[idx][20] = -f[idx]*x[idx];
      equations[idx][21] = -f[idx]*y[idx];
      equations[idx][22] = -f[idx]*z[idx];
      equations[idx][23] = -f[idx]*x[idx]*y[idx];
      equations[idx][24] = -f[idx]*x[idx]*z[idx];
      equations[idx][25] = -f[idx]*y[idx]*z[idx];
      equations[idx][26] = -f[idx]*x[idx]*x[idx];
      equations[idx][27] = -f[idx]*y[idx]*y[idx];
      equations[idx][28] = -f[idx]*z[idx]*z[idx];
      equations[idx][29] = -f[idx]*x[idx]*y[idx]*z[idx];
      equations[idx][30] = -f[idx]*x[idx]*x[idx]*x[idx];
      equations[idx][31] = -f[idx]*x[idx]*y[idx]*y[idx];
      equations[idx][32] = -f[idx]*x[idx]*z[idx]*z[idx];
      equations[idx][33] = -f[idx]*x[idx]*x[idx]*y[idx];
      equations[idx][34] = -f[idx]*y[idx]*y[idx]*y[idx];
      equations[idx][35] = -f[idx]*y[idx]*z[idx]*z[idx];
      equations[idx][36] = -f[idx]*x[idx]*x[idx]*z[idx];
      equations[idx][37] = -f[idx]*y[idx]*y[idx]*z[idx];
      equations[idx][38] = -f[idx]*z[idx]*z[idx]*z[idx];
   }
}

void ossimRpcSolver::setupWeightMatrix(NEWMAT::DiagonalMatrix& result, // holds the resulting weights
                                       const NEWMAT::ColumnVector& coefficients,
                                       const NEWMAT::ColumnVector& f,
                                       const std::vector<double>& x,
                                       const std::vector<double>& y,
                                       const std::vector<double>& z)const
{
   result.ReSize(f.Nrows());
   ossim_uint32 idx = 0;
   ossim_uint32 idx2 = 0;
   NEWMAT::RowVector row(coefficients.Nrows());
   
    for(idx = 0; idx < (ossim_uint32)f.Nrows(); ++idx)
    {
       row[0]  = 1;
       row[1]  = x[idx];
       row[2]  = y[idx];
       row[3]  = z[idx];
       row[4]  = x[idx]*y[idx];
       row[5]  = x[idx]*z[idx];
       row[6]  = y[idx]*z[idx];
       row[7]  = x[idx]*x[idx];
       row[8]  = y[idx]*y[idx];
       row[9]  = z[idx]*z[idx];
       row[10] = x[idx]*y[idx]*z[idx];
       row[11] = x[idx]*x[idx]*x[idx];
       row[12] = x[idx]*y[idx]*y[idx];
       row[13] = x[idx]*z[idx]*z[idx];
       row[14] = x[idx]*x[idx]*y[idx];
       row[15] = y[idx]*y[idx]*y[idx];
       row[16] = y[idx]*z[idx]*z[idx];
       row[17] = x[idx]*x[idx]*z[idx];
       row[18] = y[idx]*y[idx]*z[idx];
       row[19] = z[idx]*z[idx]*z[idx];

      result[idx] = 0.0;
      for(idx2 = 0; idx2 < (ossim_uint32)row.Ncols(); ++idx2)
      {
         result[idx] += row[idx2]*coefficients[idx2];
      }

      if(result[idx] > FLT_EPSILON)
      {
         result[idx] = 1.0/result[idx];
      }
    }
}

double ossimRpcSolver::eval(const std::vector<double>& coeff,
                            double x,
                            double y,
                            double z)const
{
   return coeff[ 0]       + coeff[ 1]*x     + coeff[ 2]*y     + coeff[ 3]*z     +
          coeff[ 4]*x*y   + coeff[ 5]*x*z   + coeff[ 6]*y*z   + coeff[ 7]*x*x   +
          coeff[ 8]*y*y   + coeff[ 9]*z*z   + coeff[10]*x*y*z + coeff[11]*x*x*x +
          coeff[12]*x*y*y + coeff[13]*x*z*z + coeff[14]*x*x*y + coeff[15]*y*y*y +
          coeff[16]*y*z*z + coeff[17]*x*x*z + coeff[18]*y*y*z + coeff[19]*z*z*z;
}


ossimRefPtr<ossimNitfRegisteredTag> ossimRpcSolver::getNitfRpcBTag() const
{
   ossimNitfRpcBTag* rpcbTag = new ossimNitfRpcBTag();

   // success always true
   rpcbTag->setSuccess(true);

   // temp "0"...
   rpcbTag->setErrorBias(0.0);

   // temp "0"...
   rpcbTag->setErrorRand(0.0);

   // line offset
   rpcbTag->setLineOffset(static_cast<ossim_uint32>(getImageYOffset()));

   // sample offset
   rpcbTag->setSampleOffset(static_cast<ossim_uint32>(getImageXOffset()));

   // latitude offset
   rpcbTag->setGeodeticLatOffset(getLatOffset());

   // longitude offset
   rpcbTag->setGeodeticLonOffset(getLonOffset());

   // height offset
   rpcbTag->setGeodeticHeightOffset(
      static_cast<ossim_int32>(getHeightOffset()));

   // line scale
   rpcbTag->setLineScale(static_cast<ossim_uint32>(getImageYScale()));

   // sample scale
   rpcbTag->setSampleScale(static_cast<ossim_uint32>(getImageXScale()));

   // latitude scale
   rpcbTag->setGeodeticLatScale(getLatScale());

   // longitude scale
   rpcbTag->setGeodeticLonScale(getLonScale());

   // height scale
   rpcbTag->setGeodeticHeightScale(static_cast<ossim_int32>(getHeightScale()));

   // line numerator coefficients
   rpcbTag->setLineNumeratorCoeff(getImageYNumCoefficients());
   
   // line denominator coefficients
   rpcbTag->setLineDenominatorCoeff(getImageYDenCoefficients());

   // sample numerator coefficients
   rpcbTag->setSampleNumeratorCoeff(getImageXNumCoefficients());

   // sample denominator coefficients
   rpcbTag->setSampleDenominatorCoeff(getImageXDenCoefficients());

   // Return it as an ossimRefPtr<ossimNitfRegisteredTag>...
   ossimRefPtr<ossimNitfRegisteredTag> tag = rpcbTag;
   
   return tag;
}
