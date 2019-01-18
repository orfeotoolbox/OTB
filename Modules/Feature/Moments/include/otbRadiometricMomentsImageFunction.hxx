/*
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

#ifndef otbRadiometricMomentsImageFunction_hxx
#define otbRadiometricMomentsImageFunction_hxx

#include "otbRadiometricMomentsImageFunction.h"
#include "itkMacro.h"
#include "itkVariableLengthVector.h"
#include <complex>

namespace otb
{

/**
 * Constructor
 */
template <class TInputImage, class TCoordRep>
RadiometricMomentsImageFunction<TInputImage, TCoordRep>
::RadiometricMomentsImageFunction()
{
  m_NeighborhoodRadius = 1;
}

template <class TInputImage, class TCoordRep>
void
RadiometricMomentsImageFunction<TInputImage, TCoordRep>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os, indent);
  os << indent << " Neighborhood radius value   : "  << m_NeighborhoodRadius << std::endl;
}

template <class TInputImage, class TCoordRep>
typename RadiometricMomentsImageFunction<TInputImage, TCoordRep>::OutputType
RadiometricMomentsImageFunction<TInputImage, TCoordRep>
::EvaluateAtIndex(const IndexType& index) const
{
  // Build moments vector
  OutputType moments;

  // Initialize moments
  moments.Fill( itk::NumericTraits< ScalarRealType >::Zero );

  // Check for input image
  if( !this->GetInputImage() )
    {
    return moments;
    }

  // Check for out of buffer
  //Self* _this = const_cast<Self*>(this);
  //_this->SetInputImage( this->GetInputImage() );
  if ( !this->IsInsideBuffer( index ) )
    {
    return moments;
    }

  // Create an N-d neighborhood kernel, using a zeroflux boundary condition
  typename InputImageType::SizeType kernelSize;
  kernelSize.Fill( m_NeighborhoodRadius );

  itk::ConstNeighborhoodIterator<InputImageType>
  it(kernelSize, this->GetInputImage(), this->GetInputImage()->GetBufferedRegion());

  // Set the iterator at the desired location
  it.SetLocation(index);

  itk::VariableLengthVector<ScalarRealType> tmp(m_Functor(it));

  for (unsigned int i=0; i<4; i++)
    {
    moments[i] = tmp[i];
    }

  // Return result
  return moments;
}

} // namespace otb

#endif
