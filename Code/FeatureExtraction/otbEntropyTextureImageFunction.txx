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
#ifndef __otbEntropyTextureImageFunction_txx
#define __otbEntropyTextureImageFunction_txx

#include "otbEntropyTextureImageFunction.h"
#include "itkConstNeighborhoodIterator.h"
#include "otbEntropyTextureFunctor.h"
#include "itkVariableLengthVector.h"

namespace otb
{

/**
 * Constructor
 */
template <class TInputImage, class TCoordRep>
EntropyTextureImageFunction<TInputImage,TCoordRep>
::EntropyTextureImageFunction()
{
  m_Radius.Fill(0);
  m_Offset.Fill(0);
}


/**
 *
 */
template <class TInputImage, class TCoordRep>
void
EntropyTextureImageFunction<TInputImage,TCoordRep>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os,indent);
  os << indent << "Radius: "  << m_Radius << std::endl;
  os << indent << "Offset: "  << m_Offset << std::endl;
}


/**
 *
 */
template <class TInputImage, class TCoordRep>
typename EntropyTextureImageFunction<TInputImage,TCoordRep>
::RealType
EntropyTextureImageFunction<TInputImage,TCoordRep>
::EvaluateAtIndex(const IndexType& index) const
{
  if ( !this->GetInputImage() )
  {
    return ( itk::NumericTraits<RealType>::max() );
  }

  if ( !this->IsInsideBuffer( index ) )
  {
    return ( itk::NumericTraits<RealType>::max() );
  }

  typedef itk::ConstNeighborhoodIterator<InputImageType> IterType;
  typedef typename IterType::NeighborhoodType NeighborhoodType;
  IterType it(m_Radius, this->GetInputImage(), this->GetInputImage()->GetBufferedRegion());
  it.SetLocation(index);
  SizeType radiusOff;
  radiusOff[0] = m_Radius[0] + vcl_abs(m_Offset[0]);
  radiusOff[1] = m_Radius[1] + vcl_abs(m_Offset[1]);
  IterType itOff(radiusOff, this->GetInputImage(), this->GetInputImage()->GetBufferedRegion());
  itOff.SetLocation(index);

  // false third template.. but no impact
  typedef itk::VariableLengthVector<double> VecType;
  typedef typename Functor::EntropyTextureFunctor<IterType, IterType, VecType /*RealType*/> FunctType;
  FunctType funct;
  funct.SetOffset(m_Offset); 

  return static_cast<RealType>(funct.ComputeOverSingleChannel( it.GetNeighborhood(), itOff.GetNeighborhood()) );

}


} // end namespace otb

#endif
