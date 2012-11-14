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
#ifndef __otbRadiometricMomentsImageFunction_txx
#define __otbRadiometricMomentsImageFunction_txx

#include "otbRadiometricMomentsImageFunction.h"
#include "itkNumericTraits.h"
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
