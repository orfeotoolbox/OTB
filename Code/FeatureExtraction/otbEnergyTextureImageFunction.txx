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
#ifndef __otbEnergyTextureImageFunction_txx
#define __otbEnergyTextureImageFunction_txx

#include "otbEnergyTextureImageFunction.h"
#include "itkConstNeighborhoodIterator.h"
#include "otbEnergyTextureFunctor.h"
          

namespace otb
{

/**
 * Constructor
 */
template <class TInputImage, class TCoordRep>
EnergyTextureImageFunction<TInputImage,TCoordRep>
::EnergyTextureImageFunction()
{
  m_Radius.Fill(0);
  m_Offset.Fill(0);
}


/**
 *
 */
template <class TInputImage, class TCoordRep>
void
EnergyTextureImageFunction<TInputImage,TCoordRep>
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
typename EnergyTextureImageFunction<TInputImage,TCoordRep>
::RealType
EnergyTextureImageFunction<TInputImage,TCoordRep>
::EvaluateAtIndex(const IndexType& index) const
{
  if( !this->GetInputImage() )
    {
      return ( itk::NumericTraits<RealType>::max() );
    }
  
  if ( !this->IsInsideBuffer( index ) )
    {
      return ( itk::NumericTraits<RealType>::max() );
    }
  
  typedef itk::ConstNeighborhoodIterator<InputImageType> IterType;
  IterType it(m_Radius, this->GetInputImage(), this->GetInputImage()->GetBufferedRegion());
  it.SetLocation(index);
  SizeType radiusOff;
  radiusOff[0] = m_Radius[0] + vcl_abs(m_Offset[0]);
  radiusOff[1] = m_Radius[1] + vcl_abs(m_Offset[1]);   
  IterType itOff(radiusOff, this->GetInputImage(), this->GetInputImage()->GetBufferedRegion());
  itOff.SetLocation(index);

  /*
  typedef typename Functor::EnergyTextureFunctor<IterType, IterType, RealType> FunctType;
  FunctType funct;
  funct.SetRadius(m_Radius);
  funct.SetOffset(m_Offset);
  funct(it, itOff);
  //FunctType::operator()(it, itOff);
  
  return 0;
  */
  double area = static_cast<double>(m_Radius[0]*m_Radius[1]);
  RealType output;
  
  OffsetType offset;
  offset.Fill(0);
  OffsetType offsetOff;
  OffsetType offsetOffInit;  
  
  offsetOffInit[0] = -m_Radius[0]+m_Offset[0]-1; //++ in for
  offsetOffInit[1] = -m_Radius[1]+m_Offset[1]-1; //++ in for
  
  double temp = 0.;
  int ll = 0;
  double norm = 0.;
  offsetOff = offsetOffInit;
  temp = 0.;
  for( int l = -static_cast<int>(m_Radius[0]); l <= static_cast<int>(m_Radius[0]); l++ )
    {
      offsetOff[0]++;
      offset[0] = l;
      ll = l*l;
      offsetOff[1] = offsetOffInit[1];
      for( int k = -static_cast<int>(m_Radius[1]); k <= static_cast<int>(m_Radius[1]); k++)
	{
	  offsetOff[1]++;
	  offset[1] = k;
	  norm = vcl_pow(static_cast<double>(itOff.GetPixel(offsetOff)-itOff.GetCenterPixel()), 2);
	  temp += norm;
	}
      temp /= area;
      output = static_cast<RealType>( vcl_pow(temp, 2) );
    }
  return output;
  
  

}


} // end namespace otb

#endif
