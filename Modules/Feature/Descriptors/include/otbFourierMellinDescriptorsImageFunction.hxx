/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbFourierMellinDescriptorsImageFunction_hxx
#define otbFourierMellinDescriptorsImageFunction_hxx

#include "otbFourierMellinDescriptorsImageFunction.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkNumericTraits.h"

namespace otb
{

/**
  * Constructor
  */
template <class TInputImage, class TCoordRep>
FourierMellinDescriptorsImageFunction<TInputImage, TCoordRep>
::FourierMellinDescriptorsImageFunction()
{
  m_NeighborhoodRadius = 1;
  m_Pmax = 3;
  m_Qmax = 3;
  m_Sigma = 0.5;
}

template <class TInputImage, class TCoordRep>
void
FourierMellinDescriptorsImageFunction<TInputImage, TCoordRep>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os, indent);
  os << indent << " p indice maximum value      : "  << m_Pmax << std::endl;
  os << indent << " q indice maximum value      : "  << m_Qmax << std::endl;
  os << indent << " Neighborhood radius value   : "  << m_NeighborhoodRadius << std::endl;
}

template <class TInputImage, class TCoordRep>
typename FourierMellinDescriptorsImageFunction<TInputImage, TCoordRep>::OutputType
FourierMellinDescriptorsImageFunction<TInputImage, TCoordRep>
::EvaluateAtIndex(const IndexType& index) const
{
  // Build Fourier-Mellin Harmonics Matrix
  ComplexType coefs;
  coefs.resize(m_Pmax+1);
  OutputType descriptors;
  descriptors.resize(m_Pmax+1);

  // Initialize moments
  for (unsigned int p = 0; p <= m_Pmax; p++)
    {
    coefs.at(p).resize(m_Qmax+1);
    descriptors.at(p).resize(m_Qmax+1);
    for (unsigned int q = 0; q <= m_Qmax; q++)
      {
      coefs.at(p).at(q) =  itk::NumericTraits<ScalarComplexType>::Zero;
      descriptors.at(p).at(q) = itk::NumericTraits<ScalarRealType>::Zero;
      }
    }

  // Check for input image
  if( !this->GetInputImage() )
    {
    return descriptors;
    }

  // Check for out of buffer
  if ( !this->IsInsideBuffer( index ) )
    {
    return descriptors;
    }

  // Create an N-d neighborhood kernel, using a zeroflux boundary condition
  typename InputImageType::SizeType kernelSize;
  kernelSize.Fill( m_NeighborhoodRadius );

  itk::ConstNeighborhoodIterator<InputImageType>
    it(kernelSize, this->GetInputImage(), this->GetInputImage()->GetBufferedRegion());

  // Set the iterator at the desired location
  it.SetLocation(index);

  // Walk the neighborhood
  const unsigned int size = it.Size();
  for (unsigned int i = 0; i < size; ++i)
    {
    // Retrieve value, and centered-reduced position
    ScalarRealType value = static_cast<ScalarRealType>(it.GetPixel(i));
    ScalarRealType     x = static_cast<ScalarRealType>(it.GetOffset(i)[0])/(2*m_NeighborhoodRadius+1);
    ScalarRealType     y = static_cast<ScalarRealType>(it.GetOffset(i)[1])/(2*m_NeighborhoodRadius+1);

    // Build complex value
    ScalarComplexType xplusiy(x, y), x2plusy2(x*x+y*y, 0.0);

    // Update cumulants
    for (unsigned int p = 0; p <= m_Pmax; p++)
      {
      for (unsigned int q= 0; q <= m_Qmax; q++)
        {
        ScalarComplexType power(double(p-2.0+m_Sigma)/2.0, -double(q)/2.0);
	
	if(x!=0 || y !=0) // vcl_pow limitation
	  {	    
	    coefs.at(p).at(q) += vcl_pow(xplusiy, -static_cast<double>(p)) * vcl_pow(x2plusy2, power) * value;
	  }
	}
      }
    }

  // Normalisation

  for (int p = m_Pmax; p >= 0; p--)
    {
    for (int q = m_Qmax; q >= 0; q--)
      {
      coefs.at(p).at(q) /= 2*CONST_PI * coefs.at(0).at(0);

      descriptors.at(p).at(q) = vcl_sqrt((coefs.at(p).at(q).real() * coefs.at(p).at(q).real()
                                          + coefs.at(p).at(q).imag() * coefs.at(p).at(q).imag()));
      }
    }

  // Return result
  return descriptors;
}

} // namespace otb

#endif
