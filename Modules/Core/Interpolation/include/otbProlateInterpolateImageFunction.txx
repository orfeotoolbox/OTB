/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbProlateInterpolateImageFunction_txx
#define __otbProlateInterpolateImageFunction_txx

#include "otbProlateInterpolateImageFunction.h"

namespace otb
{
namespace Function
{
template<class TInput, class TOutput>
double
ProlateFunction<TInput, TOutput>
::ComputeEnergy(double resampleRatio) const
{
  vnl_vector<vcl_complex<double> > resampledProfile(1024);
  resampledProfile.fill(0);

  for (unsigned int i = 0; i < m_Radius + 1; ++i)
    {
    unsigned int ival =
      static_cast<unsigned int>(static_cast<double>(m_OriginalProfile.size() * i) / static_cast<double>(m_Radius + 1));
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
template<class TInputImage, class TBoundaryCondition, class TCoordRep, class TInputInterpolator,
    class TOutputInterpolator>
ProlateInterpolateImageFunction<TInputImage, TBoundaryCondition, TCoordRep, TInputInterpolator, TOutputInterpolator>
::ProlateInterpolateImageFunction()
{
  //VectorType m_ResampledProfil(1, 0.);
  this->SetNormalizeWeight(true);
}

/** Destructor */
template<class TInputImage, class TBoundaryCondition, class TCoordRep, class TInputInterpolator,
    class TOutputInterpolator>
ProlateInterpolateImageFunction<TInputImage, TBoundaryCondition, TCoordRep, TInputInterpolator, TOutputInterpolator>
::~ProlateInterpolateImageFunction()
{
}

template<class TInputImage, class TBoundaryCondition, class TCoordRep, class TInputInterpolator,
    class TOutputInterpolator>
void
ProlateInterpolateImageFunction<TInputImage, TBoundaryCondition, TCoordRep, TInputInterpolator, TOutputInterpolator>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} //namespace otb

#endif
