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
#ifndef __otbTextureImageFunction_txx
#define __otbTextureImageFunction_txx

#include "otbTextureImageFunction.h"

namespace otb
{

/**
 * Constructor
 */
template <class TInputImage, class TFunctor, class TCoordRep>
TextureImageFunction<TInputImage, TFunctor, TCoordRep>
::TextureImageFunction()
{
  m_Radius.Fill(0);
  m_Offset.Fill(0);
}

/**
 *
 */
template <class TInputImage, class TFunctor, class TCoordRep>
void
TextureImageFunction<TInputImage, TFunctor, TCoordRep>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os, indent);
  os << indent << "Radius: "  << m_Radius << std::endl;
  os << indent << "Offset: "  << m_Offset << std::endl;
}

/**
 *
 */
template <class TInputImage, class TFunctor, class TCoordRep>
typename TextureImageFunction<TInputImage, TFunctor, TCoordRep>
::RealType
TextureImageFunction<TInputImage, TFunctor, TCoordRep>
::EvaluateAtIndex(const IndexType& index) const
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
  radiusOff[0] = (m_Radius[0]) + vcl_abs(m_Offset[0]);
  radiusOff[1] = (m_Radius[1]) + vcl_abs(m_Offset[1]);
  IteratorType itOff(radiusOff, this->GetInputImage(), this->GetInputImage()->GetBufferedRegion());
  itOff.SetLocation(index);

  FunctorType funct;
  funct.SetOffset(m_Offset);

  return static_cast<RealType>(funct(itOff.GetNeighborhood()));
}

} // end namespace otb

#endif
