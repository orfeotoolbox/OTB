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

#ifndef otbProlateInterpolateImageFunction_hxx
#define otbProlateInterpolateImageFunction_hxx

#include "otbProlateInterpolateImageFunction.h"

namespace otb
{
namespace Function
{
template <class TInput, class TOutput>
double ProlateFunction<TInput, TOutput>::ComputeEnergy(double resampleRatio) const
{
  vnl_vector<std::complex<double>> resampledProfile(1024);
  resampledProfile.fill(0);

  for (unsigned int i = 0; i < m_Radius + 1; ++i)
  {
    unsigned int ival   = static_cast<unsigned int>(static_cast<double>(m_OriginalProfile.size() * i) / static_cast<double>(m_Radius + 1));
    resampledProfile[i] = m_OriginalProfile[ival];
  }
  vnl_fft_1d<double> v1d(1024);
  v1d.fwd_transform(resampledProfile);

  // Carrful, spectrum is symmetrical
  unsigned int sampleNb = static_cast<unsigned int>(1024 / (2 * resampleRatio));

  double energy = 0.;
  // First part of spectrum
  for (unsigned int j = 0; j < sampleNb + 1; ++j)
  {
    energy += std::abs(resampledProfile[j]) * std::abs(resampledProfile[j]);
  }
  // Last part of spectrum
  for (unsigned int j = 1023; j > 1023 - sampleNb; j--)
  {
    energy += std::abs(resampledProfile[j]) * std::abs(resampledProfile[j]);
  }

  double totalEnergy = energy;
  // Middle part
  for (unsigned int j = sampleNb + 1; j < 1023 - sampleNb + 1; ++j)
  {
    totalEnergy += std::abs(resampledProfile[j]) * std::abs(resampledProfile[j]);
  }

  return std::sqrt(energy) / std::sqrt(totalEnergy);
}

} // end namespace Function

/** Constructor */
template <class TInputImage, class TBoundaryCondition, class TCoordRep, class TInputInterpolator, class TOutputInterpolator>
ProlateInterpolateImageFunction<TInputImage, TBoundaryCondition, TCoordRep, TInputInterpolator, TOutputInterpolator>::ProlateInterpolateImageFunction()
{
  // VectorType m_ResampledProfil(1, 0.);
  this->SetNormalizeWeight(true);
}

/** Destructor */
template <class TInputImage, class TBoundaryCondition, class TCoordRep, class TInputInterpolator, class TOutputInterpolator>
ProlateInterpolateImageFunction<TInputImage, TBoundaryCondition, TCoordRep, TInputInterpolator, TOutputInterpolator>::~ProlateInterpolateImageFunction()
{
}

template <class TInputImage, class TBoundaryCondition, class TCoordRep, class TInputInterpolator, class TOutputInterpolator>
void ProlateInterpolateImageFunction<TInputImage, TBoundaryCondition, TCoordRep, TInputInterpolator, TOutputInterpolator>::PrintSelf(std::ostream& os,
                                                                                                                                     itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // namespace otb

#endif
