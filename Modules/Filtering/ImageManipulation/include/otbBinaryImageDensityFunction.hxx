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


#ifndef otbBinaryImageDensityFunction_hxx
#define otbBinaryImageDensityFunction_hxx

#include "otbBinaryImageDensityFunction.h"

#include "itkConstNeighborhoodIterator.h"

namespace otb
{

/**
 * Constructor
 */
template <class TInputImage, class TCoordRep>
BinaryImageDensityFunction<TInputImage, TCoordRep>::BinaryImageDensityFunction()
{
  m_NeighborhoodRadius.Fill(1);
}

/**
 *
 */
template <class TInputImage, class TCoordRep>
void BinaryImageDensityFunction<TInputImage, TCoordRep>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os, indent);
  os << indent << "NeighborhoodRadius: " << m_NeighborhoodRadius << std::endl;
}

/**
 *
 */
template <class TInputImage, class TCoordRep>
typename BinaryImageDensityFunction<TInputImage, TCoordRep>::RealType
BinaryImageDensityFunction<TInputImage, TCoordRep>::EvaluateAtIndex(const IndexType& index) const
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

  itk::ConstNeighborhoodIterator<InputImageType> it(kernelSize, this->GetInputImage(), this->GetInputImage()->GetBufferedRegion());

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
  var                = sum / num;

  return var;
}

} // end namespace otb

#endif
