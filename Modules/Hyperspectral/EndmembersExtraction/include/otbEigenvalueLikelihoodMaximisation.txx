/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

Some parts of this code are derived from ITK. See ITKCopyright.txt
for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef otbEigenvalueLikelihoodMaximisation_txx
#define otbEigenvalueLikelihoodMaximisation_txx

#include "otbEigenvalueLikelihoodMaximisation.h"

#include <vcl_algorithm.h>

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
  vcl_sort(eigenCovariance.begin(), eigenCovariance.end());
  eigenCovariance.flip();

  vnl_symmetric_eigensystem<PrecisionType> eigenR(m_Correlation);
  VectorType eigenCorrelation = eigenR.D.diagonal();
  vcl_sort(eigenCorrelation.begin(), eigenCorrelation.end());
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
      t[j] = (r - k) * (r - k) / sigma[j];
      //std::cout << "t[" << j <<"]=" << t[j] << std::endl;
      sigma[j] = vcl_log(sigma[j]);
      }
    m_Likelihood(i) = -0.5*t.sum() - 0.5*sigma.sum();
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
  m_NumberOfEndmembers = iMax;
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
