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
#ifndef __otbHuImageFunction_txx
#define __otbHuImageFunction_txx

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
template < class TInput, class TOutput, class TPrecision, class TCoordRep>
HuImageFunction<TInput,TOutput,TPrecision,TCoordRep>
::HuImageFunction()
{
  m_MomentNumber =-1;
}

/**
   *
   */
template < class TInput, class TOutput, class TPrecision, class TCoordRep>
void
HuImageFunction<TInput,TOutput,TPrecision,TCoordRep>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os,indent);
  os << indent << " m_MomentNumber           : "  << m_MomentNumber << std::endl;
}


template < class TInput, class TOutput, class TPrecision, class TCoordRep>
typename HuImageFunction<TInput,TOutput,TPrecision,TCoordRep>::RealType
HuImageFunction<TInput,TOutput,TPrecision,TCoordRep>
::EvaluateAtIndex(const IndexType& index) const
{
  //typename InputType::SizeType        ImageSize;
  RealType                         HuValue = 0.;
  ComplexType                      HuValueComplex;

  typedef otb::ComplexMomentImageFunction<InputType,ComplexType>   CMType;
  typename CMType::Pointer function =CMType::New();

  if( !this->GetInputImage() )
    {
    return ( itk::NumericTraits<RealType>::max() );
    }


  if ( !this->IsInsideBuffer( index ) )
    {
    otbMsgDevMacro( << index );
    return ( itk::NumericTraits<RealType>::max() );
    }

  assert(m_MomentNumber > 0);
  assert(m_MomentNumber < 8);

   function->SetInputImage( this->GetInputImage() );
   function->SetNeighborhoodRadius(this->GetNeighborhoodRadius() );

  switch(m_MomentNumber)
    {
    case 1 :
        {
  ComplexType C11;
  function->SetP(1);
  function->SetQ(1);
  C11 = function->EvaluateAtIndex( index );
        HuValue = C11.real();
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

  HuValue = vcl_abs( C20 * C02 );

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

  HuValue = vcl_abs( C30 * C03 );
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

  HuValue = vcl_abs( C21 * C12 );
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

  HuValueComplex = C30 * vcl_pow(C12,3);
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

  HuValueComplex = C20 * vcl_pow( C12 ,2 );
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

  HuValueComplex = C30 * vcl_pow( C12 , 3);
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
