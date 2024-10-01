/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbLeastSquareBilinearTransformEstimator_hxx
#define otbLeastSquareBilinearTransformEstimator_hxx

#include <vnl/algo/vnl_lsqr.h>
#include <vnl/vnl_sparse_matrix_linear_system.h>
#include <vnl/vnl_least_squares_function.h>
#include <vnl/vnl_inverse.h>
#include <vnl/vnl_matrix.h>

#include "otbMacro.h"
#include "otbLeastSquareBilinearTransformEstimator.h"

namespace otb
{

template <class TPoint>
LeastSquareBilinearTransformEstimator<TPoint>::LeastSquareBilinearTransformEstimator()
  : m_TiePointsContainer(), m_RMSError(), m_RelativeResidual(), bl_a(0.0), bl_b(0.0), bl_c(0.0), bl_d(0.0), Ata(), Atb()
{
  Ata.fill(0.0);
  Atb.fill(0.0);
}

template <class TPoint>
void LeastSquareBilinearTransformEstimator<TPoint>::ClearTiePoints()
{
  // Clear the container
  m_TiePointsContainer.clear();
}

template <class TPoint>
void LeastSquareBilinearTransformEstimator<TPoint>::AddTiePoints(const PointType& src, const PrecisionType& dst)
{
  vnl_matrix_fixed<double, 4, 1> Ata_layer;
  Ata_layer(0,0) = 1.0;
  Ata_layer(1,0) = src[0];
  Ata_layer(2,0) = src[1];
  Ata_layer(3,0) = src[0] * src[1];

  Ata += Ata_layer * Ata_layer.transpose();
  Atb += Ata_layer * dst;
}




template <class TPoint>
void LeastSquareBilinearTransformEstimator<TPoint>::Compute()
{
  vnl_matrix_fixed<double, 4, 1> soln;
  // vnl_matrix_inverse<double> inv;
  vnl_matrix<double> inv; 
  vnl_matrix_fixed<double, 4, 4> inv2;
  vnl_matrix_fixed<double, 4, 4> id;

  // vnl_inverse
  inv = vnl_matrix_inverse<double>(Ata);
  inv2 = vnl_matrix_fixed<double, 4, 4>(inv);
  soln = vnl_matrix_fixed<double, 4, 4>(vnl_matrix_inverse<double>(Ata)) * Atb;
  bl_a = soln(0,0);
  bl_b = soln(1,0);
  bl_c = soln(2,0);
  bl_d = soln(3,0);

  id = Ata * inv;

}

template <class TPoint>
void LeastSquareBilinearTransformEstimator<TPoint>::lsFitValue(const PointType& point, PrecisionType& dst) const
{
  dst = (bl_a + bl_b*point[0] + bl_c*point[1] + bl_d*point[0]*point[1]);
}

template <class TPoint>
void LeastSquareBilinearTransformEstimator<TPoint>::PrintSelf(std::ostream& /*os*/, itk::Indent /*indent*/) const {}

} // end namespace otb

#endif
