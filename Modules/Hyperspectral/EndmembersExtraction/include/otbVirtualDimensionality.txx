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
#ifndef otbVirtualDimensionality_txx
#define otbVirtualDimensionality_txx

#include "otbVirtualDimensionality.h"

#include <vcl_algorithm.h>
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
  vcl_sort(eigenCovariance.begin(), eigenCovariance.end());
  eigenCovariance.flip();

  vnl_symmetric_eigensystem<PrecisionType> eigenR(m_Correlation);
  VectorType eigenCorrelation = eigenR.D.diagonal();
  vcl_sort(eigenCorrelation.begin(), eigenCorrelation.end());
  eigenCorrelation.flip();

  m_NumberOfEndmembers = 0;
  for(unsigned int i = 0; i < nbBands; ++i)
    {
    double sigma = vcl_sqrt( 2.0 / m_NumberOfPixels
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
