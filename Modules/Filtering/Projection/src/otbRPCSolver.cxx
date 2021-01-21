/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "otbRPCSolver.h"
#include "otbMacro.h"

#include "vnl/algo/vnl_svd.h"
#include "vnl/algo/vnl_matrix_inverse.h"

#include "otbGDALRPCTransformer.h"

namespace otb
{

using PrecisionType = double;
using VectorType = vnl_vector<PrecisionType>;
using MatrixType = vnl_matrix<PrecisionType>;
using DiagonalMatrixType = vnl_diag_matrix<PrecisionType>;

const PrecisionType epsilon = 1e-7;

void UpdateMatrix(const VectorType & f, 
                    const std::vector<PrecisionType> & x,
                    const std::vector<PrecisionType> & y,
                    const std::vector<PrecisionType> & z,
                    MatrixType & M)
{
   for(unsigned int i = 0; i < f.size(); i++)
   {
      M[i][0]  = 1;
      M[i][1]  = x[i];
      M[i][2]  = y[i];
      M[i][3]  = z[i];
      M[i][4]  = x[i]*y[i];
      M[i][5]  = x[i]*z[i];
      M[i][6]  = y[i]*z[i];
      M[i][7]  = x[i]*x[i];
      M[i][8]  = y[i]*y[i];
      M[i][9]  = z[i]*z[i];
      M[i][10] = x[i]*y[i]*z[i];
      M[i][11] = x[i]*x[i]*x[i];
      M[i][12] = x[i]*y[i]*y[i];
      M[i][13] = x[i]*z[i]*z[i];
      M[i][14] = x[i]*x[i]*y[i];
      M[i][15] = y[i]*y[i]*y[i];
      M[i][16] = y[i]*z[i]*z[i];
      M[i][17] = x[i]*x[i]*z[i];
      M[i][18] = y[i]*y[i]*z[i];
      M[i][19] = z[i]*z[i]*z[i];
      M[i][20] = -f[i]*x[i];
      M[i][21] = -f[i]*y[i];
      M[i][22] = -f[i]*z[i];
      M[i][23] = -f[i]*x[i]*y[i];
      M[i][24] = -f[i]*x[i]*z[i];
      M[i][25] = -f[i]*y[i]*z[i];
      M[i][26] = -f[i]*x[i]*x[i];
      M[i][27] = -f[i]*y[i]*y[i];
      M[i][28] = -f[i]*z[i]*z[i];
      M[i][29] = -f[i]*x[i]*y[i]*z[i];
      M[i][30] = -f[i]*x[i]*x[i]*x[i];
      M[i][31] = -f[i]*x[i]*y[i]*y[i];
      M[i][32] = -f[i]*x[i]*z[i]*z[i];
      M[i][33] = -f[i]*x[i]*x[i]*y[i];
      M[i][34] = -f[i]*y[i]*y[i]*y[i];
      M[i][35] = -f[i]*y[i]*z[i]*z[i];
      M[i][36] = -f[i]*x[i]*x[i]*z[i];
      M[i][37] = -f[i]*y[i]*y[i]*z[i];
      M[i][38] = -f[i]*z[i]*z[i]*z[i];
   }
}

void UpdateWeights(const std::vector<PrecisionType> & x,
                    const std::vector<PrecisionType> & y,
                    const std::vector<PrecisionType> & z,
                    const VectorType & coeff,
                    DiagonalMatrixType & W)
{
   std::vector<PrecisionType> row(coeff.size());
   
    for(unsigned int i = 0; i < x.size(); i++)
    {
      W[i] = 0.;

      row[0]  = 1;
      row[1]  = x[i];
      row[2]  = y[i];
      row[3]  = z[i];
      row[4]  = x[i]*y[i];
      row[5]  = x[i]*z[i];
      row[6]  = y[i]*z[i];
      row[7]  = x[i]*x[i];
      row[8]  = y[i]*y[i];
      row[9]  = z[i]*z[i];
      row[10] = x[i]*y[i]*z[i];
      row[11] = x[i]*x[i]*x[i];
      row[12] = x[i]*y[i]*y[i];
      row[13] = x[i]*z[i]*z[i];
      row[14] = x[i]*x[i]*y[i];
      row[15] = y[i]*y[i]*y[i];
      row[16] = y[i]*z[i]*z[i];
      row[17] = x[i]*x[i]*z[i];
      row[18] = y[i]*y[i]*z[i];
      row[19] = z[i]*z[i]*z[i];

      for (unsigned int j =0; j < row.size(); j++)
      {
        W[i] += row[j]*coeff[j];
      }

      if(W[i] > epsilon)
      {
         W[i] = 1.0/W[i];
      }
    }
}

void computeCoefficients(const std::vector<PrecisionType> & f, 
                          const std::vector<PrecisionType> & x,
                          const std::vector<PrecisionType> & y,
                          const std::vector<PrecisionType> & z,
                          std::vector<PrecisionType> & outCoeffs)
{
  auto numberOfPoints = f.size();

  // Iteratively reweighted least square

  MatrixType M(numberOfPoints, 39);
  VectorType r(numberOfPoints, numberOfPoints, f.data());
  DiagonalMatrixType weights(numberOfPoints, 1.);

  PrecisionType res = std::numeric_limits<PrecisionType>::max();
  VectorType coeffs;

  for (int i =0; i<10; i++)
  {
    if (res < epsilon)
    {
      break;
    }
    auto w2 = weights * weights;
    
    UpdateMatrix(r, x, y ,z ,M);

    vnl_svd<PrecisionType>    svd(M.transpose()*w2*M);

    auto diag = svd.W();
    for (unsigned int idx = 0; idx < diag.size(); idx++)
    {
      if(diag[idx] > epsilon)
      {
         diag[idx] = 1.0/diag[idx];
      }
      else
      {
         diag[idx] = 0.0;
      }
    }
    
    // "svd.V() * diag * svd.U().transpose()" is the inverse of M.transpose()*w2*M
    coeffs =  svd.V() * diag * svd.U().transpose() * M.transpose()*w2*r;

    auto denominator = VectorType(&(coeffs[19]), 20);
    denominator[0] = 1;

    UpdateWeights( x, y ,z , denominator, weights);

    // compute the residual
    auto residual = M.transpose()*w2*(M*coeffs-r);

    auto residualValue = inner_product(residual,residual);
  }

  outCoeffs.assign(coeffs.begin(), coeffs.end());
}


void RPCSolver::Solve(const GCPsContainerType& gcpContainer, PrecisionType& rmsError, Projection::RPCParam& outputParams)
{
  // By default, use elevation provided with ground control points
  bool useElevation = true;
  auto numberOfPoints = gcpContainer.size();

  std::vector<PrecisionType> colNorm;
  colNorm.reserve(numberOfPoints);
  
  std::vector<PrecisionType> lineNorm;
  lineNorm.reserve(numberOfPoints);

  std::vector<PrecisionType> lonNorm;
  lonNorm.reserve(numberOfPoints);
  
  std::vector<PrecisionType> latNorm;
  latNorm.reserve(numberOfPoints);
  
  std::vector<PrecisionType> altNorm;
  altNorm.reserve(numberOfPoints);
  
  // Check for enough points
  if (numberOfPoints < 20)
  {
    itkGenericExceptionMacro(<< "At least 20 points are required to estimate the 40 parameters of a RPC model without elevation support, and 40 are required "
                                "to estimate the 80 parameters of a RPC model with elevation support. Only "
                             << numberOfPoints << " points were given.");
  }

  // If not enough points are given for a proper estimation of RPC
  // with elevation support, disable elevation. This will result in
  // all coefficients related to elevation set to zero.
  if (numberOfPoints < 40)
  {
    otbGenericWarningMacro("Only " << numberOfPoints << " ground control points are provided, can not estimate a RPC model with elevation support (at "
                                                             "least 40 points required). Elevation support will be disabled for RPC estimation. All "
                                                             "coefficients related to elevation will be set to zero, and elevation will have no effect on the "
                                                             "resulting transform.");
    useElevation = false;
  }

  // Compute Offsets

  // Find the ground points center of mass
  PrecisionType accLat = 0.;
  PrecisionType accLon = 0.;
  PrecisionType accAlt = 0.;

  for (const auto & gcp : gcpContainer)
  {
    const auto & groundPoint = gcp.second;

    accLon += groundPoint[0];
    accLat += groundPoint[1];
    if (useElevation)
    {
      accAlt += groundPoint[2];
    }
  }

  Point3DType groundCenter;
  groundCenter[0] = accLon / numberOfPoints;
  groundCenter[1] = accLat /numberOfPoints;
  groundCenter[2] = useElevation ? accAlt / numberOfPoints : 0.;


  PrecisionType minc = std::numeric_limits<PrecisionType>::max();
  PrecisionType minl = std::numeric_limits<PrecisionType>::max();
  PrecisionType maxc = std::numeric_limits<PrecisionType>::min();
  PrecisionType maxl = std::numeric_limits<PrecisionType>::min();

  for (const auto & gcp : gcpContainer)
  {
    const auto & imagePoint = gcp.first;

    minc = std::min(imagePoint[0], minl);
    maxc = std::max(imagePoint[0], maxc);

    minl = std::min(imagePoint[1], minl);
    maxl = std::max(imagePoint[1], maxl);
  }

  Point2DType imageCenter;
  imageCenter[0] = (minc + maxc -1)/2.0;
  imageCenter[1] = (minl + maxl -1)/2.0;

  auto height = std::abs(minl - maxl) +1;
  auto width = std::abs(minc - maxc) +1;

  PrecisionType maxDeltaLon = std::numeric_limits<PrecisionType>::min();
  PrecisionType maxDeltaLat = std::numeric_limits<PrecisionType>::min();
  PrecisionType maxDeltaAlt = std::numeric_limits<PrecisionType>::min();

  for (const auto & gcp : gcpContainer)
  {
    const auto & imagePoint = gcp.first;
    const auto & groundPoint = gcp.second;
    auto deltaLon = groundPoint[0] - groundCenter[0];
    auto deltaLat = groundPoint[1] - groundCenter[1];
    auto deltaAlt = useElevation ? groundPoint[2] - groundCenter[2] : 0.;
    auto alt = useElevation ? groundPoint[2] : 0.;

    maxDeltaLon = std::max(maxDeltaLon, std::abs(deltaLon));
    maxDeltaLat = std::max(maxDeltaLat, std::abs(deltaLat));
    maxDeltaAlt = std::max(maxDeltaAlt, std::abs(alt));

    colNorm.push_back( (imagePoint[0]- imageCenter[0] -0.5) / width * 2.);
    lineNorm.push_back( (imagePoint[1]- imageCenter[1] -0.5)/ height * 2.);
    lonNorm.push_back(deltaLon);
    latNorm.push_back(deltaLat);
    altNorm.push_back(deltaAlt);
  }

  if(maxDeltaLat < 1.0)
  {
    maxDeltaLat = 1.0;
  }
  else
  {
    for (auto & lat: latNorm)
    {
      lat /= maxDeltaLat;
    }
  }


  if(maxDeltaLon < 1.0)
  {
    maxDeltaLon = 1.0;
  }
  else
  {
    for (auto & lon: lonNorm)
    {
      lon /= maxDeltaLon;
    }
  }

  if(maxDeltaAlt < 1.0) 
  {
    maxDeltaAlt = 1.0;
  }
  else
  {
    for (auto & alt: altNorm)
    {
      alt /= maxDeltaAlt;
    }
  }

  std::vector<PrecisionType> colCoeffs;
  computeCoefficients(colNorm, lonNorm, latNorm, altNorm, colCoeffs);

  std::vector<PrecisionType> lineCoeffs;
  computeCoefficients(lineNorm, lonNorm, latNorm, altNorm, lineCoeffs);

    // Offsets
  outputParams.SampleOffset = imageCenter[0];
  outputParams.LineOffset = imageCenter[1];
  outputParams.LonOffset = groundCenter[0];
  outputParams.LatOffset = groundCenter[1];
  outputParams.HeightOffset = groundCenter[2];

  // Scales
  outputParams.SampleScale = width/2.0;
  outputParams.LineScale = height/2.0;
  outputParams.LatScale = maxDeltaLat;
  outputParams.LonScale = maxDeltaLon;
  outputParams.HeightScale = maxDeltaAlt;

  // Line numerator coefficients
  std::copy(lineCoeffs.begin(), lineCoeffs.begin() +20, outputParams.LineNum);
  // Line denominator coefficients
  outputParams.LineDen[0] = 1.;
  std::copy(lineCoeffs.begin()+20, lineCoeffs.end(), outputParams.LineDen +1);
  
  // Sample numerator coefficients
  std::copy(colCoeffs.begin(), colCoeffs.begin() +20, outputParams.SampleNum);
  // Sample denominator coefficients
  outputParams.SampleDen[0] = 1.;
  std::copy(colCoeffs.begin()+20, colCoeffs.end(), outputParams.SampleDen +1);

  // Compute rmse between input ground point and transformed image points
  GDALRPCTransformer transformer(outputParams, false);
  
  PrecisionType rmseAcc = 0.;

  for (const auto & gcp : gcpContainer)
  {
    auto outPoint = transformer.InverseTransform(gcp.second);
    rmseAcc += (gcp.first[0] - outPoint[0]) * (gcp.first[0] - outPoint[0])
                + (gcp.first[1] - outPoint[1]) * (gcp.first[1] - outPoint[1]);
  }

  rmsError = std::sqrt(rmseAcc)/numberOfPoints;
}

}