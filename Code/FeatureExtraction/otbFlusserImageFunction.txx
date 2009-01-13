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
#ifndef __otbFlusserImageFunction_txx
#define __otbFlusserImageFunction_txx

#include "otbFlusserImageFunction.h"
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
FlusserImageFunction<TInput,TOutput,TPrecision,TCoordRep>
::FlusserImageFunction()
{
  m_MomentNumber =-1;
}

/**
   *
   */
template < class TInput, class TOutput, class TPrecision, class TCoordRep>
void
FlusserImageFunction<TInput,TOutput,TPrecision,TCoordRep>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os,indent);
  os << indent << " m_MomentNumber           : "  << m_MomentNumber << std::endl;
}


template < class TInput, class TOutput, class TPrecision, class TCoordRep>
typename FlusserImageFunction<TInput,TOutput,TPrecision,TCoordRep>::RealType
FlusserImageFunction<TInput,TOutput,TPrecision,TCoordRep>
::EvaluateAtIndex(const IndexType& index) const
{

  RealType                            FlusserValue;
  ComplexType                         FlusserValueComplex;

  typedef otb::ComplexMomentImageFunction<InputType,ComplexType>   CMType;
  typename CMType::Pointer function =CMType::New();

  if( !this->GetInputImage() )
    {
    return ( itk::NumericTraits<RealType>::max() );
    }

  if ( !this->IsInsideBuffer( index ) )
    {
    return ( itk::NumericTraits<RealType>::max() );
    }

  assert(m_MomentNumber > 0);
  assert(m_MomentNumber < 12);

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
        FlusserValue = C11.real() ;
  }
  break;
    case 2:
        {
  ComplexType C21,C12;
  function->SetP(2);
  function->SetQ(1);
  C21 = function->EvaluateAtIndex( index );
  function->SetP(1);
  function->SetQ(2);
  C12 = function->EvaluateAtIndex( index );

  FlusserValue = vcl_abs( C21 * C12 ) ;
  }
  break;
    case 3:
        {
  ComplexType C20,C12;
  function->SetP(2);
  function->SetQ(0);
  C20 = function->EvaluateAtIndex( index );
  function->SetP(1);
  function->SetQ(2);
  C12 = function->EvaluateAtIndex( index );
  FlusserValueComplex = C20 * vcl_pow(C12,2);
  FlusserValue = FlusserValueComplex.real();
  }
  break;
    case 4:
        {
  ComplexType C20,C12;
  function->SetP(2);
  function->SetQ(0);
  C20 = function->EvaluateAtIndex( index );
  function->SetP(1);
  function->SetQ(2);
  C12 = function->EvaluateAtIndex( index );
  FlusserValueComplex = C20 * vcl_pow(C12,2);
  FlusserValue = FlusserValueComplex.imag();
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

  FlusserValueComplex = C30 * vcl_pow(C12,3) ;
  FlusserValue = FlusserValueComplex.real();
  }
  break;
    case 6:
        {
  ComplexType C30,C12;
  function->SetP(3);
  function->SetQ(0);
  C30 = function->EvaluateAtIndex( index );
  function->SetP(1);
  function->SetQ(2);
  C12 = function->EvaluateAtIndex( index );

  FlusserValueComplex = C30 * vcl_pow(C12,3) ;
  FlusserValue = FlusserValueComplex.imag();
  }
  break;
    case 7 :
        {
  ComplexType C22;
  function->SetP(2);
  function->SetQ(2);
  C22 = function->EvaluateAtIndex( index );
        FlusserValue = C22.real() ;
  }
  break;
    case 8:
        {
  ComplexType C31,C12;
  function->SetP(3);
  function->SetQ(1);
  C31 = function->EvaluateAtIndex( index );
  function->SetP(1);
  function->SetQ(2);
  C12 = function->EvaluateAtIndex( index );
  FlusserValueComplex = C31 * vcl_pow(C12,2);
  FlusserValue = FlusserValueComplex.real();
  }
  break;
    case 9:
        {
  ComplexType C31,C12;
  function->SetP(3);
  function->SetQ(1);
  C31 = function->EvaluateAtIndex( index );
  function->SetP(1);
  function->SetQ(2);
  C12 = function->EvaluateAtIndex( index );
  FlusserValueComplex = C31 * vcl_pow(C12,2);
  FlusserValue = FlusserValueComplex.imag();
  }
  break;
    case 10:
        {
  ComplexType C40,C12;
  function->SetP(4);
  function->SetQ(0);
  C40 = function->EvaluateAtIndex( index );
  function->SetP(1);
  function->SetQ(2);
  C12 = function->EvaluateAtIndex( index );
  FlusserValueComplex = C40 * vcl_pow(C12,4);
  FlusserValue = FlusserValueComplex.real();
  }
  break;
    case 11:
        {
  ComplexType C40,C12;
  function->SetP(4);
  function->SetQ(0);
  C40 = function->EvaluateAtIndex( index );
  function->SetP(1);
  function->SetQ(2);
  C12 = function->EvaluateAtIndex( index );
  FlusserValueComplex = C40 * vcl_pow(C12,4);
  FlusserValue = FlusserValueComplex.imag();
  }
  break;

    default:
  itkWarningMacro("Hu's invariant parameters are between 1 and 7");
    }


  return (static_cast<RealType>(FlusserValue) );

}


} // namespace otb

#endif
