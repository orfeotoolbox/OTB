/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) CS Systemes d'information. All rights reserved.
  See CSCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __otbBinaryImageDensityFunction_txx
#define __otbBinaryImageDensityFunction_txx

#include "otbBinaryImageDensityFunction.h"

#include "itkConstNeighborhoodIterator.h"

namespace otb
{

/**
 * Constructor
 */
template <class TInputImage, class TCoordRep>
BinaryImageDensityFunction<TInputImage, TCoordRep>
::BinaryImageDensityFunction()
{
  m_NeighborhoodRadius.Fill(1);
}

/**
 *
 */
template <class TInputImage, class TCoordRep>
void
BinaryImageDensityFunction<TInputImage, TCoordRep>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os, indent);
  os << indent << "NeighborhoodRadius: "  << m_NeighborhoodRadius << std::endl;
}

/**
 *
 */
template <class TInputImage, class TCoordRep>
typename BinaryImageDensityFunction<TInputImage, TCoordRep>
::RealType
BinaryImageDensityFunction<TInputImage, TCoordRep>
::EvaluateAtIndex(const IndexType& index) const
{
  RealType sum;
  RealType var;

  sum = itk::NumericTraits<RealType>::Zero;

  if (!this->GetInputImage())
    {
    return (itk::NumericTraits<RealType>::max());
    }

  if (!this->IsInsideBuffer(index))
    {
    return (itk::NumericTraits<RealType>::max());
    }

  // Create an N-d neighborhood kernel, using a zeroflux boundary condition
  typename InputImageType::SizeType kernelSize = m_NeighborhoodRadius;

  itk::ConstNeighborhoodIterator<InputImageType>
  it(kernelSize, this->GetInputImage(), this->GetInputImage()->GetBufferedRegion());

  // Set the iterator at the desired location
  it.SetLocation(index);

  // Walk the neighborhood
  const unsigned int size = it.Size();
  for (unsigned int i = 0; i < size; ++i)
    {
    const RealType value = static_cast<RealType>(it.GetPixel(i));
    sum += value;
    }

  const RealType num = static_cast<RealType>(size);
  var = sum / num;

  return var;
}

} // end namespace otb

#endif
