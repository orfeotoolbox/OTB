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
#ifndef __otbComplexMomentImageFunction_txx
#define __otbComplexMomentImageFunction_txx

#include "otbComplexMomentImageFunction.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkNumericTraits.h"
#include "itkMacro.h"

namespace otb
{

/**
   * Constructor
   */
template <class TInputImage, class TCoordRep>
ComplexMomentImageFunction<TInputImage, TCoordRep>
::ComplexMomentImageFunction()
{
  m_NeighborhoodRadius = 1;
  m_Pmax = 4;
  m_Qmax = 4;
}

/**
   *
   */
template <class TInputImage, class TCoordRep>
void
ComplexMomentImageFunction<TInputImage, TCoordRep>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os, indent);
  os << indent << " p indice maximum value      : "  << m_Pmax << std::endl;
  os << indent << " q indice maximum value      : "  << m_Qmax << std::endl;
  os << indent << " Neighborhood radius value   : "  << m_NeighborhoodRadius << std::endl;
}

template <class TInputImage, class TCoordRep>
typename ComplexMomentImageFunction<TInputImage, TCoordRep>::ComplexType
ComplexMomentImageFunction<TInputImage, TCoordRep>
::EvaluateAtIndex(const IndexType& index) const
{
  // Build moments vector
  ComplexType moments;
  moments.resize(m_Pmax+1);
  
  // Initialize moments
  for (unsigned int p = 0; p <= m_Pmax; p++)
    {
    moments.at(p).resize(m_Qmax+1);
    for (unsigned int q = 0; q <= m_Qmax; q++)
      {
      moments.at(p).at(q) =  ScalarComplexType(0.0,0.0);
      }
    }

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
    ScalarComplexType xpy(x,y),xqy(x,-y);
    
    // Update cumulants
    for (unsigned int p = 0; p <= m_Pmax; p++)
      {
      for (unsigned int q= 0; q <= m_Qmax; q++)
        {
        moments.at(p).at(q) += vcl_pow(xpy,p) * vcl_pow(xqy,q) * value;   
        }
      }
    }
  
  // Normalisation
  for (unsigned int p = 0; p <= m_Pmax; p++)
    {
    for (unsigned int q= 0; q <= m_Qmax; q++)
      {
      if (p+q != 0)
        {
        moments.at(p).at(q) /= moments.at(0).at(0);
        }
      }
    }

  // Return result
  return moments;
}

} // namespace otb

#endif
