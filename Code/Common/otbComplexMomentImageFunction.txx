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

#include "itkNumericTraits.h"
#include "itkConstNeighborhoodIterator.h"

namespace otb
{

/**
   * Constructor
   */
template <class TInputImage, class TCoordRep>
ComplexMomentImageFunction<TInputImage,TCoordRep>
::ComplexMomentImageFunction()
{
  m_p = 0;
  m_q = 0;
}

#if 0
/**
   *
   */
template <class TInputImage, class TCoordRep = float >
void
ComplexMomentImageFunction<TInputImage,TCoordRep>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os,indent);
  os << indent << " p indice value: "  << m_p << std::endl;
  os << indent << " q indice value: "  << m_q << std::endl;
}


/**
 *
 */
template <class TInputImage, class TCoordRep = float >
typename ComplexMomentImageFunction<TInputImage,TCoordRep>
::RealType
ComplexMomentImageFunction<TInputImage,TCoordRep>
::Evaluate()
{

  ComplexType                         Sum;
  ComplexType                         valA;
  ComplexType                         valB;
  
  typename InputImageType::IndexType   IndexValue;
  
  Sum = NumericTraits<ComplexType>::Zero;
  
  if( !this->GetInputImage() )
    {
    return ( NumericTraits<ComplexType>::max() );
    }
   
  ConstNeighborhoodIterator<InputImageType>
    it(this->GetInputImage(), this->GetInputImage(), this->GetInputImage()->GetBufferedRegion());

  it.GoToBegin();
  while (!it.IsAtEnd())
  {
    IndexValue = is.GetIndex();
    valA = ( complex(IndexValue[0], IndexValue[1]) ) ^ (m_p) ;
    valB = ( complex(IndexValue[0],-IndexValue[1]) ) ^ (m_q) ;
    sum += valA * valB * static_cast<complex>( it.Get() );
  }
             
  return ( sum );
}

#endif

} // namespace otb

#endif
