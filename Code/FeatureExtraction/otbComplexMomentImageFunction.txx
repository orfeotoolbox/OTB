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
namespace otb
{

/**
   * Constructor
   */
template < class TInput, class TOutput, class TCoordRep>
ComplexMomentImageFunction<TInput,TOutput,TCoordRep>
::ComplexMomentImageFunction()
{
  m_P = 0;
  m_Q = 0;
}

/**
   *
   */
template < class TInput, class TOutput, class TCoordRep>
void
ComplexMomentImageFunction<TInput,TOutput,TCoordRep>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os,indent);
  os << indent << " p indice value      : "  << m_P << std::endl;
  os << indent << " q indice value      : "  << m_Q << std::endl;
}


template < class TInput, class TOutput, class TCoordRep>
typename ComplexMomentImageFunction<TInput,TOutput,TCoordRep>::ComplexType
ComplexMomentImageFunction<TInput,TOutput,TCoordRep>
::EvaluateAtIndex(const IndexType& index) const
{  
  typename TInput::SizeType           ImageSize;
  ComplexType                         Sum;
  ComplexType                         ValP;
  ComplexType                         ValQ;
  IndexType                           IndexValue;
  IndexType                           indexPos = index;
  typename TInput::SizeType           kernelSize;

  if( !this->GetInputImage() )
    {
    std::cout << "Pb with GetInputImage" << std::endl;
    return ( std::complex<float>( itk::NumericTraits<float>::max(), itk::NumericTraits<float>::max() ) );
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
  Sum = std::complex<float>(0.0,0.0); 

  const unsigned int size = it.Size();
  for (unsigned int i = 0; i < size; ++i)
  {
    IndexValue = it.GetIndex(i);
    ValP = std::complex<float>(1.0,0.0);
    ValQ = std::complex<float>(1.0,0.0);
    unsigned int p  = m_P;
    while(p>0)
     {
      ValP *= std::complex<float>(IndexValue[0], IndexValue[1]);
      --p; 
     }
    unsigned int q  = m_Q;
    while(q>0)
     {
      ValQ *= std::complex<float>(IndexValue[0], -IndexValue[1]);
      --q; 
     }
          
    Sum += ( ValP * ValQ * std::complex<float>(static_cast<float>(it.GetPixel(i)),0.0) );

    //std::cout << Sum << std::endl;
  }

  return (static_cast<ComplexType>(Sum) );


}


} // namespace otb

#endif
