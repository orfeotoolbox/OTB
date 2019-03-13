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

#ifndef otbEigenvalueLikelihoodMaximisation_hxx
#define otbEigenvalueLikelihoodMaximisation_hxx

#include "otbEigenvalueLikelihoodMaximisation.h"

#include <algorithm>

namespace otb
{

template<class TPrecision>
EigenvalueLikelihoodMaximisation<TPrecision>
::EigenvalueLikelihoodMaximisation()
 : m_NumberOfPixels(0),
   m_NumberOfEndmembers(0)
{
}

template<class TInputImage>
void
EigenvalueLikelihoodMaximisation<TInputImage>
::Compute()
{
  // TODO check size
  const unsigned int nbBands = m_Covariance.rows();

  // Compute diagonalisation of covariance and correlation
  vnl_symmetric_eigensystem<PrecisionType> eigenK(m_Covariance);
  VectorType eigenCovariance = eigenK.D.diagonal();
  std::sort(eigenCovariance.begin(), eigenCovariance.end());
  eigenCovariance.flip();

  vnl_symmetric_eigensystem<PrecisionType> eigenR(m_Correlation);
  VectorType eigenCorrelation = eigenR.D.diagonal();
  std::sort(eigenCorrelation.begin(), eigenCorrelation.end());
  eigenCorrelation.flip();

  // Compute likelihood log
  m_Likelihood.set_size(nbBands);
  const double coef = 2.0/m_NumberOfPixels;

  for(unsigned int i=0; i < nbBands; ++i)
    {
    const unsigned int nl = nbBands - i;
    VectorType sigma(nl), t(nl);

    for (unsigned int j = 0; j < nl; ++j )
      {
      PrecisionType r = eigenCorrelation[j + i];
      PrecisionType k = eigenCovariance[j + i];

      sigma[j] = coef * (r * r + k * k);
      //std::cout << "sigma[" << j << "]=" << sigma[j] << std::endl;
      t[j] = (r - k) * (r - k) / (2*sigma[j]);
      //std::cout << "t[" << j <<"]=" << t[j] << std::endl;
      sigma[j] = std::log(std::sqrt(sigma[j]));
      }
    m_Likelihood(i) = -t.sum() - sigma.sum();
    }
  // Extract first local maximum
  //double max = m_Likelihood[0];
  unsigned int iMax = 0;
  for (unsigned int i = 1; i < m_Likelihood.size() - 1; ++i)
    {
    if ( (m_Likelihood[i] > m_Likelihood[i - 1])
         && (m_Likelihood[i] > m_Likelihood[i + 1]) )
      {
      //max = m_Likelihood[i];
      iMax = i;
      break;
      }
    }
  m_NumberOfEndmembers = iMax-1;
}

template <class TImage>
void
EigenvalueLikelihoodMaximisation<TImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "Covariance:         " << m_Covariance  << std::endl;
  os << indent << "Correlation:        " << m_Correlation << std::endl;
  os << indent << "NumberOfEndmembers: " << m_NumberOfEndmembers << std::endl;
  os << indent << "Likelihood:         " << m_Likelihood << std::endl;
}

} // end namespace otb
#endif
