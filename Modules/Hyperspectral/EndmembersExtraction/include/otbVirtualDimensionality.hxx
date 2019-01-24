/*
 * Copyright (C) 1999-2011 Insight Software Consortium
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbVirtualDimensionality_hxx
#define otbVirtualDimensionality_hxx

#include "otbVirtualDimensionality.h"

#include <algorithm>
#include "otb_boost_math_normal_header.h"

namespace otb
{

template<class TPrecision>
VirtualDimensionality<TPrecision>
::VirtualDimensionality()
 : m_NumberOfPixels(0),
   m_NumberOfEndmembers(0),
   m_FAR(1.0E-3)
{
}

template<class TInputImage>
void
VirtualDimensionality<TInputImage>
::Compute()
{
  // TODO check size
  const unsigned int nbBands = m_Covariance.rows();

  // Compute diagonalisation of sample covariance and correlation matrices
  vnl_symmetric_eigensystem<PrecisionType> eigenK(m_Covariance);
  VectorType eigenCovariance = eigenK.D.diagonal();
  std::sort(eigenCovariance.begin(), eigenCovariance.end());
  eigenCovariance.flip();

  vnl_symmetric_eigensystem<PrecisionType> eigenR(m_Correlation);
  VectorType eigenCorrelation = eigenR.D.diagonal();
  std::sort(eigenCorrelation.begin(), eigenCorrelation.end());
  eigenCorrelation.flip();

  m_NumberOfEndmembers = 0;
  for(unsigned int i = 0; i < nbBands; ++i)
    {
    double sigma = std::sqrt( 2.0 / m_NumberOfPixels
                              * (eigenCovariance[i] + eigenCorrelation[i]
                                 + eigenCovariance[i] * eigenCorrelation[i]) );
    boost::math::normal normalDist(0, sigma);
    double tau = -boost::math::quantile(normalDist, m_FAR);
    if (eigenCorrelation[i] - eigenCovariance[i] > tau )
      m_NumberOfEndmembers++;
    }
}

template <class TImage>
void
VirtualDimensionality<TImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "Covariance:         " << m_Covariance  << std::endl;
  os << indent << "Correlation:        " << m_Correlation << std::endl;
  os << indent << "NumberOfEndmembers: " << m_NumberOfEndmembers << std::endl;
}

} // end namespace otb
#endif
