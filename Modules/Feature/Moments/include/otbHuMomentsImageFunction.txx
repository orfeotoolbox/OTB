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
#ifndef __otbHuMomentsImageFunction_txx
#define __otbHuMomentsImageFunction_txx

#include "otbHuMomentsImageFunction.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkNumericTraits.h"
#include <complex>

namespace otb
{

/**
   * Constructor
   */
template <class TInputImage, class TCoordRep>
HuMomentsImageFunction<TInputImage, TCoordRep>
::HuMomentsImageFunction()
{
  m_NeighborhoodRadius = 1;
}

template <class TInputImage, class TCoordRep>
void
HuMomentsImageFunction<TInputImage, TCoordRep>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os, indent);
  os << indent << " Neighborhood radius value   : "  << m_NeighborhoodRadius << std::endl;
}

template <class TInputImage, class TCoordRep>
typename HuMomentsImageFunction<TInputImage, TCoordRep>::OutputType
HuMomentsImageFunction<TInputImage, TCoordRep>
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
  if ( !this->IsInsideBuffer( index ) )
    {
    return moments;
    }

  // Define complex type
  typedef std::complex<ScalarRealType> ComplexType;

  // Define and intialize cumulants for complex moments
  ComplexType c11, c20, c02, c30, c03, c21, c12;
  c11 = itk::NumericTraits<ComplexType>::Zero;
  c20 = itk::NumericTraits<ComplexType>::Zero;
  c02 = itk::NumericTraits<ComplexType>::Zero;
  c30 = itk::NumericTraits<ComplexType>::Zero;
  c03 = itk::NumericTraits<ComplexType>::Zero;
  c21 = itk::NumericTraits<ComplexType>::Zero;
  c12 = itk::NumericTraits<ComplexType>::Zero;

  ScalarRealType c00 = itk::NumericTraits<ScalarRealType>::Zero;

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
    ScalarRealType x = static_cast<ScalarRealType>(it.GetOffset(i)[0])/(2*m_NeighborhoodRadius+1);
    ScalarRealType y = static_cast<ScalarRealType>(it.GetOffset(i)[1])/(2*m_NeighborhoodRadius+1);
    // Build complex value
    ComplexType xpy(x, y), xqy(x, -y);

    // Update cumulants
    c00 += value;
    c11 += xpy*xqy*value;
    c20 += xpy*xpy*value;
    c02 += xqy*xqy*value;
    c30 += xpy*xpy*xpy*value;
    c03 += xqy*xqy*xqy*value;
    c21 += xpy*xpy*xqy*value;
    c12 += xpy*xqy*xqy*value;
    }

  // Nomalisation
  c11 /= c00;
  c20 /= c00;
  c02 /= c00;
  c30 /= c00;
  c03 /= c00;
  c21 /= c00;
  c12 /= c00;

  // Compute moments combinations
  moments[0]  = static_cast<ScalarRealType>(c11.real());
  moments[1]  = static_cast<ScalarRealType>((c20*c02).real());
  moments[2]  = static_cast<ScalarRealType>((c30*c03).real());
  moments[3]  = static_cast<ScalarRealType>((c21*c12).real());
  moments[4]  = static_cast<ScalarRealType>((c30*c12*c12*c12).real());
  moments[5]  = static_cast<ScalarRealType>((c20*c12*c12).real());
  moments[6]  = static_cast<ScalarRealType>((c30*c12*c12*c12).imag());

  // Return result
  return moments;
}

} // namespace otb

#endif
