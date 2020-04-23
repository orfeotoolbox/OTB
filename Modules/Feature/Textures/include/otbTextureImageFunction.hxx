/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbTextureImageFunction_hxx
#define otbTextureImageFunction_hxx

#include "otbTextureImageFunction.h"

namespace otb
{

/**
 * Constructor
 */
template <class TInputImage, class TFunctor, class TCoordRep>
TextureImageFunction<TInputImage, TFunctor, TCoordRep>::TextureImageFunction()
{
  m_Radius.Fill(0);
  m_Offset.Fill(0);
}

/**
 *
 */
template <class TInputImage, class TFunctor, class TCoordRep>
void TextureImageFunction<TInputImage, TFunctor, TCoordRep>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os, indent);
  os << indent << "Radius: " << m_Radius << std::endl;
  os << indent << "Offset: " << m_Offset << std::endl;
}

/**
 *
 */
template <class TInputImage, class TFunctor, class TCoordRep>
typename TextureImageFunction<TInputImage, TFunctor, TCoordRep>::RealType
TextureImageFunction<TInputImage, TFunctor, TCoordRep>::EvaluateAtIndex(const IndexType& index) const
{
  if (!this->GetInputImage())
  {
    return (itk::NumericTraits<RealType>::max());
  }

  if (!this->IsInsideBuffer(index))
  {
    return (itk::NumericTraits<RealType>::max());
  }

  SizeType radiusOff;
  radiusOff[0] = (m_Radius[0]) + std::abs(m_Offset[0]);
  radiusOff[1] = (m_Radius[1]) + std::abs(m_Offset[1]);
  IteratorType itOff(radiusOff, this->GetInputImage(), this->GetInputImage()->GetBufferedRegion());
  itOff.SetLocation(index);

  FunctorType funct;
  funct.SetOffset(m_Offset);

  return static_cast<RealType>(funct(itOff.GetNeighborhood()));
}

} // end namespace otb

#endif
