/*=========================================================================

    Program:   Insight Segmentation & Registration Toolkit
    Module:    $RCSfile: itkMeanImageFunction.txx,v $
    Language:  C++
    Date:      $Date: 2004/12/12 22:07:24 $
    Version:   $Revision: 1.12 $

    Copyright (c) Insight Software Consortium. All rights reserved.
    See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

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

#include <complex>
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
  m_NeighborhoodRadius = -1;
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
  os << indent << " m_NeighborhoodRadius: "  << m_NeighborhoodRadius << std::endl;
}


template < class TInput, class TOutput, class TCoordRep>
typename ComplexMomentImageFunction<TInput,TOutput,TCoordRep>::ComplexType
ComplexMomentImageFunction<TInput,TOutput,TCoordRep>
::EvaluateAtIndex(const IndexType& index) const
{
  typename InputType::SizeType        ImageSize;
  ComplexType                         Sum;
  ComplexType                         ValP;
  ComplexType                         ValQ;
  IndexType                           IndexValue;
  IndexType                           indexPos = index;
  typename InputType::SizeType        kernelSize;

  if( !this->GetInputImage() )
    {
      return std::complex<float>(0.,0.);  // A modifier
//    return ( NumericTraits<RealType>::max() );
    }
  
  if ( !this->IsInsideBuffer( index ) )
    {
      return std::complex<float>(0.,0.); // A modifier
//    return ( NumericTraits<RealType>::max() );
    }

   if(m_NeighborhoodRadius<0)
     {
     ImageSize = this->GetInputImage()->GetBufferedRegion().GetSize();
     indexPos[0] = ImageSize[0] / 2 ;
     indexPos[1] = ImageSize[1] / 2;
     
       kernelSize[0] = indexPos[0];
       kernelSize[1] = indexPos[1];          
     }
     else
     {
       kernelSize.Fill( m_NeighborhoodRadius );
     }  
 
  itk::ConstNeighborhoodIterator<InputType>
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
//    std::cout<< i <<"--> "<< IndexValue << " p:"<<ValP <<" Q: "<<ValQ;  
          
    Sum += ( ValP * ValQ * std::complex<float>(static_cast<float>(it.GetPixel(i)),0.0) ); 
//    std::cout<< "Val Pixel: " << static_cast<float>(it.GetPixel(i)) <<" Result :" << Sum<<std::endl;
  }

//   std::cout<<"Result dans la procedure: " <<Sum <<std::endl;
           
  return (static_cast<ComplexType>(Sum) );

}


} // namespace otb

#endif
