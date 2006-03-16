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
#ifndef _otbHuImageFunction_txx
#define _otbHuImageFunction_txx

#include "otbHuImageFunction.h"
#include "itkMacro.h"
#include <complex>

namespace otb
{

/**
   * Constructor
   */
template < class TInput, class TOutput, class TCoordRep>
HuImageFunction<TInput,TOutput,TCoordRep>
::HuImageFunction()
{
  m_Number =-1; 
}

/**
   *
   */
template < class TInput, class TOutput, class TCoordRep>
void
HuImageFunction<TInput,TOutput,TCoordRep>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os,indent);
  os << indent << " m_Number           : "  << m_Number << std::endl;
}


template < class TInput, class TOutput, class TCoordRep>
typename HuImageFunction<TInput,TOutput,TCoordRep>::ComplexType
HuImageFunction<TInput,TOutput,TCoordRep>
::EvaluateAtIndex(const IndexType& index) 
{
  typename InputType::SizeType        ImageSize;
  ComplexType                         HuValue;

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

  assert(m_Number > 0);
  assert(m_Number < 8);

  switch(m_Number)
    {
    case 1 : 
        {
	ComplexType C11;
	this->SetP(1);
	this->SetQ(1);
	C11 = this->Superclass::EvaluateAtIndex( index );
        HuValue = C11 ;
	}
	break;
    case 2:
        {
	ComplexType C20,C02;
	this->SetP(2);
	this->SetQ(0);
	C20 = this->Superclass::EvaluateAtIndex( index );
	this->SetP(0);
	this->SetQ(2);
	C02 = this->Superclass::EvaluateAtIndex( index );

	HuValue = C20 * C02 ;
	}
	break;
    case 3:
        {
	ComplexType C30,C03;
	this->SetP(3);
	this->SetQ(0);
	C30 = this->Superclass::EvaluateAtIndex( index );
	this->SetP(0);
	this->SetQ(3);
	C03 = this->Superclass::EvaluateAtIndex( index );

	HuValue = C30 * C03 ;
	}
	break;
    case 4:
        {
	ComplexType C21,C12;
	this->SetP(2);
	this->SetQ(1);
	C21 = this->Superclass::EvaluateAtIndex( index );
	this->SetP(1);
	this->SetQ(2);
	C12 = this->Superclass::EvaluateAtIndex( index );

	HuValue = C21 * C12 ;
	}	
	break;

    case 5:
        {
	ComplexType C30,C12;
	this->SetP(3);
	this->SetQ(0);
	C30 = this->Superclass::EvaluateAtIndex( index );
	this->SetP(1);
	this->SetQ(2);
	C12 = this->Superclass::EvaluateAtIndex( index );

	HuValue = C30 * C12 * C12 * C12 ;
//	HuValue = HuValue.real();  // Attention        
	}	
	break;

    case 6:
        {
	ComplexType C20,C12;
	this->SetP(2);
	this->SetQ(0);
	C20 = this->Superclass::EvaluateAtIndex( index );
	this->SetP(1);
	this->SetQ(2);
	C12 = this->Superclass::EvaluateAtIndex( index );

	HuValue = C20 * C12 * C12;
//	HuValue = HuValue.real();  // Attention        
	}	
	break;

    case 7:
        {
	ComplexType C30,C12;
	this->SetP(3);
	this->SetQ(0);
	C30 = this->Superclass::EvaluateAtIndex( index );
	this->SetP(1);
	this->SetQ(2);
	C12 = this->Superclass::EvaluateAtIndex( index );

	HuValue = C30 * C12 * C12 * C12 ;
//	HuValue = HuValue.imag();  // Attention        
	}	
	break;
	
    default:
	itkWarningMacro("Hu's invariant parameters are between 1 and 7");	
    }

  return (static_cast<ComplexType>(HuValue) );

}


} // namespace otb

#endif
