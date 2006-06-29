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
#ifndef _otbHuImageFunction_txx
#define _otbHuImageFunction_txx

#include "otbHuImageFunction.h"
#include "otbComplexMomentImageFunction.h"
#include "itkNumericTraits.h"
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
  m_MomentNumber =-1; 
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
  os << indent << " m_MomentNumber           : "  << m_MomentNumber << std::endl;
}


template < class TInput, class TOutput, class TCoordRep>
typename HuImageFunction<TInput,TOutput,TCoordRep>::RealType
HuImageFunction<TInput,TOutput,TCoordRep>
::EvaluateAtIndex(const IndexType& index) const
{
  typename InputType::SizeType        ImageSize;
  RealType                         HuValue;
  ComplexType                      HuValueComplex;

  typedef otb::ComplexMomentImageFunction<InputType,ComplexType>   CMType;
  typename CMType::Pointer function =CMType::New();

  if( !this->GetInputImage() )
    {
    return ( itk::NumericTraits<RealType>::max() );
    }


/*  std::cout << "Start" << this->GetStartIndex() << std::endl;
  std::cout << "End" << this->GetEndIndex() << std::endl; */
  
  if ( !this->IsInsideBuffer( index ) )
    {
    std::cout << index << std::endl;
    return ( itk::NumericTraits<RealType>::max() );
    }

  assert(m_MomentNumber > 0);
  assert(m_MomentNumber < 8);

   function->SetInputImage( this->GetInputImage() );
//   std::cout << "Neighbor " <<this->GetNeighborhoodRadius()<<std::endl;
   function->SetNeighborhoodRadius(this->GetNeighborhoodRadius() );

  switch(m_MomentNumber)
    {
    case 1 : 
        {
	ComplexType C11;
	function->SetP(1);
	function->SetQ(1);
	C11 = function->EvaluateAtIndex( index );
        HuValue = C11.real() ;
	}
	break;
    case 2:
        {
	ComplexType C20,C02;
	function->SetP(2);
	function->SetQ(0);	
	C20 = function->EvaluateAtIndex( index );
	function->SetP(0);
	function->SetQ(2);
	C02 = function->EvaluateAtIndex( index );

	HuValue = abs( C20 * C02 ) ;

	}
	break;
    case 3:
        {
	ComplexType C30,C03;
	function->SetP(3);
	function->SetQ(0);
	C30 = function->EvaluateAtIndex( index );
	function->SetP(0);
	function->SetQ(3);
	C03 = function->EvaluateAtIndex( index );

	HuValue = abs( C30 * C03 );
	}
	break;
    case 4:
        {
	ComplexType C21,C12;
	function->SetP(2);
	function->SetQ(1);
	C21 = function->EvaluateAtIndex( index );
	function->SetP(1);
	function->SetQ(2);
	C12 = function->EvaluateAtIndex( index );

	HuValue = abs( C21 * C12 );
	}	
	break;

    case 5:
        {
	ComplexType C30,C12;
	function->SetP(3);
	function->SetQ(0);
	C30 = function->EvaluateAtIndex( index );
	function->SetP(1);
	function->SetQ(2);
	C12 = function->EvaluateAtIndex( index );

	HuValueComplex = C30 * pow(C12,3) ;
	HuValue = HuValueComplex.real();       
	}	
	break;

    case 6:
        {
	ComplexType C20,C12;
	function->SetP(2);
	function->SetQ(0);
	C20 = function->EvaluateAtIndex( index );
	function->SetP(1);
	function->SetQ(2);
	C12 = function->EvaluateAtIndex( index );

	HuValueComplex = C20 * pow( C12 ,2 );
	HuValue = HuValueComplex.real();         
	}	
	break;

    case 7:
        {
	ComplexType C30,C12;
	function->SetP(3);
	function->SetQ(0);
	C30 = function->EvaluateAtIndex( index );
	function->SetP(1);
	function->SetQ(2);
	C12 = function->EvaluateAtIndex( index );

	HuValueComplex = C30 * pow( C12 , 3);
	HuValue = HuValueComplex.imag();         
	}	
	break;
	
    default:
	itkWarningMacro("Hu's invariant parameters are between 1 and 7");	
    }


  return (static_cast<RealType>(HuValue) );

}


} // namespace otb

#endif
