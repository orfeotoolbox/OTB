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
#include "itkConstNeighborhoodIterator.h"
#include "itkNumericTraits.h"
#include "itkMacro.h"
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
typename RadiometricMomentsImageFunction<TInputImage,TCoordRep>::OutputType
RadiometricMomentsImageFunction<TInputImage,TCoordRep>
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

  // Define and intialize cumulants
  ScalarRealType sum1, sum2, sum3, sum4;
  sum1 = itk::NumericTraits<ScalarRealType>::Zero;
  sum2 = itk::NumericTraits<ScalarRealType>::Zero;
  sum3 = itk::NumericTraits<ScalarRealType>::Zero;
  sum4 = itk::NumericTraits<ScalarRealType>::Zero;

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
    ScalarRealType value = static_cast<ScalarRealType>(it.GetPixel(i));
    ScalarRealType value2 = value * value;

    // Update cumulants
    sum1 += value;
    sum2 += value2;
    sum3 += value * value2;
    sum4 += value2 * value2;

    }

  // final computations

  // Mean
  moments[0] = sum1 / size;
  // Variance
  moments[1] = (sum2 - (sum1 * moments[0])) / (size - 1);
  
  
  ScalarRealType sigma      = vcl_sqrt(moments[1]);
  ScalarRealType mean2      = moments[0] * moments[0];
 
  const double epsilon = 1E-10;
  if (vcl_abs(moments[1]) > epsilon)
    {
    // Skewness
    moments[2] = ((sum3 - 3.0 * moments[0] * sum2) / size + 2.0 * moments[0] * mean2) / (moments[1] * sigma);
    // Kurtosis
    moments[3] = ((sum4 - 4.0 * moments[0] * sum3 + 6.0 * mean2 * sum2) / size - 3.0 * mean2 * mean2) 
      / (moments[1] * moments[1]) - 3.0;
    }
   
  // Return result
  return moments;
}

} // namespace otb

#endif
