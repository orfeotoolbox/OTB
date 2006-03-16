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
::EvaluateAtIndex(const IndexType& index) const
{
  typename InputType::SizeType        ImageSize;
  ComplexType                         HuValue;
  typename CMType::Pointer            function =CMType::New();

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


  function->SetInputImage( this->GetInputImage() );

  switch(m_Number)
    {
    case 1 : 
        {
	ComplexType C11;
	function->SetP(1);
	function->SetQ(1);
	C11 = function->EvaluateAtIndex( index );
        HuValue = C11 ;
	}
    case 2:
        {
	ComplexType C20,C02;
	function->SetP(2);
	function->SetQ(0);
	C20 = function->EvaluateAtIndex( index );
	function->SetP(0);
	function->SetQ(2);
	C02 = function->EvaluateAtIndex( index );

	HuValue = C20 * C02 ;
	}
    case 3:
        {
	ComplexType C30,C03;
	function->SetP(3);
	function->SetQ(0);
	C30 = function->EvaluateAtIndex( index );
	function->SetP(0);
	function->SetQ(3);
	C03 = function->EvaluateAtIndex( index );

	HuValue = C30 * C03 ;
	}
    case 4:
        {
	ComplexType C21,C12;
	function->SetP(2);
	function->SetQ(1);
	C21 = function->EvaluateAtIndex( index );
	function->SetP(1);
	function->SetQ(2);
	C12 = function->EvaluateAtIndex( index );

	HuValue = C21 * C12 ;
	}	
		
    }

           
  return (static_cast<ComplexType>(HuValue) );

}


} // namespace otb

#endif
