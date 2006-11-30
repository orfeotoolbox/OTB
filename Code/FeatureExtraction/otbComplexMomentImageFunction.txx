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
#ifndef _otbComplexMomentImageFunction_txx
#define _otbComplexMomentImageFunction_txx

#include "otbComplexMomentImageFunction.h"
#include "itkImageRegionIterator.h"
#include "itkImage.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkNumericTraits.h"
#include "otbMacro.h"

namespace otb
{

/**
   * Constructor
   */
template < class TInput, class TOutput, class TPrecision, class TCoordRep>
ComplexMomentImageFunction<TInput,TOutput,TPrecision,TCoordRep>
::ComplexMomentImageFunction()
{
  m_P = 0;
  m_Q = 0;
}

/**
   *
   */
template < class TInput, class TOutput, class TPrecision, class TCoordRep>
void
ComplexMomentImageFunction<TInput,TOutput,TPrecision,TCoordRep>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os,indent);
  os << indent << " p indice value      : "  << m_P << std::endl;
  os << indent << " q indice value      : "  << m_Q << std::endl;
}


template < class TInput, class TOutput, class TPrecision, class TCoordRep>
typename ComplexMomentImageFunction<TInput,TOutput,TPrecision,TCoordRep>::ComplexType
ComplexMomentImageFunction<TInput,TOutput,TPrecision,TCoordRep>
::EvaluateAtIndex(const IndexType& index) const
{  
  typename TInput::SizeType           ImageSize;
  ComplexPrecisionType                Sum;
  ComplexPrecisionType                ValP;
  ComplexPrecisionType                ValQ;
  IndexType                           IndexValue;
  IndexType                           indexPos = index;
  typename TInput::SizeType           kernelSize;

  if( !this->GetInputImage() )
    {
    otbMsgDevMacro( << "Pb with GetInputImage" );
    return ( static_cast<ComplexType>( itk::NumericTraits<PrecisionType>::max(), itk::NumericTraits<PrecisionType>::max() ) );
    }

   if(this->GetNeighborhoodRadius()<0)
     {
     ImageSize = this->GetInputImage()->GetBufferedRegion().GetSize();
     
     indexPos[0] = ImageSize[0] / 2 ;
     indexPos[1] = ImageSize[1] / 2;
     
     kernelSize[0] = indexPos[0];
     kernelSize[1] = indexPos[1];          
     }
     else
     {
       kernelSize.Fill( this->GetNeighborhoodRadius() );
     }  

   itk::ConstNeighborhoodIterator<TInput>
    it(kernelSize, this->GetInputImage(), this->GetInputImage()->GetBufferedRegion());


  // Set the iterator at the desired location
  it.SetLocation(indexPos);
  Sum = ComplexPrecisionType(0.0,0.0); 

  const unsigned int size = it.Size();
  for (unsigned int i = 0; i < size; ++i)
  {
    IndexValue = it.GetIndex(i);
    ValP = ComplexPrecisionType(1.0,0.0);
    ValQ = ComplexPrecisionType(1.0,0.0);
    unsigned int p  = m_P;
    while(p>0)
     {
      ValP *= ComplexPrecisionType(IndexValue[0], IndexValue[1]);
      --p; 
     }
    unsigned int q  = m_Q;
    while(q>0)
     {
      ValQ *= ComplexPrecisionType(IndexValue[0], -IndexValue[1]);
      --q; 
     }
          
    Sum += ( ValP * ValQ * ComplexPrecisionType(static_cast<PrecisionType>(it.GetPixel(i)),0.0) );

  }

  return (static_cast<ComplexType>(Sum) );


}


} // namespace otb

#endif
