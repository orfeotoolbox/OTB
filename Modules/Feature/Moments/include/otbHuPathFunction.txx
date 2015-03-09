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
#ifndef __otbHuPathFunction_txx
#define __otbHuPathFunction_txx

#include "otbHuPathFunction.h"
#include "otbComplexMomentPathFunction.h"
#include "itkNumericTraits.h"

#include "otbMacro.h"

namespace otb
{

/**
   * Constructor
   */
template <class TInputPath, class TOutput, class TPrecision>
HuPathFunction<TInputPath, TOutput, TPrecision>
::HuPathFunction()
{
  m_MomentNumber = -1;
}

/**
   *
   */
template <class TInputPath, class TOutput, class TPrecision>
void
HuPathFunction<TInputPath, TOutput, TPrecision>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os, indent);
  os << indent << " m_MomentNumber           : "  << m_MomentNumber << std::endl;
}

template <class TInputPath, class TOutput, class TPrecision>
typename HuPathFunction<TInputPath, TOutput, TPrecision>::RealType
HuPathFunction<TInputPath, TOutput, TPrecision>
::Evaluate(const PathType& itkNotUsed(path)) const
{
  typedef ComplexMomentPathFunction<PathType> FunctionType;
  typedef typename FunctionType::ComplexType  ComplexType;

  RealType    HuValue = 0.;
  ComplexType HuValueComplex;

  typename FunctionType::Pointer function = FunctionType::New();

  function->SetInputPath(this->GetInputPath());

  switch (m_MomentNumber)
    {
    case 1:
      {
      ComplexType C11;
      function->SetP(1);
      function->SetQ(1);
      C11 = function->Evaluate();
      HuValue = C11.real();
      }
      break;
    case 2:
      {
      ComplexType C20, C02;
      function->SetP(2);
      function->SetQ(0);
      C20 = function->Evaluate();
      function->SetP(0);
      function->SetQ(2);
      C02 = function->Evaluate();

      HuValue = vcl_abs(C20 * C02);

      }
      break;
    case 3:
      {
      ComplexType C30, C03;
      function->SetP(3);
      function->SetQ(0);
      C30 = function->Evaluate();
      function->SetP(0);
      function->SetQ(3);
      C03 = function->Evaluate();

      HuValue = vcl_abs(C30 * C03);
      }
      break;
    case 4:
      {
      ComplexType C21, C12;
      function->SetP(2);
      function->SetQ(1);
      C21 = function->Evaluate();
      function->SetP(1);
      function->SetQ(2);
      C12 = function->Evaluate();

      HuValue = vcl_abs(C21 * C12);
      }
      break;

    case 5:
      {
      ComplexType C30, C12;
      function->SetP(3);
      function->SetQ(0);
      C30 = function->Evaluate();
      function->SetP(1);
      function->SetQ(2);
      C12 = function->Evaluate();

      HuValueComplex = C30 * vcl_pow(C12, 3);
      HuValue = HuValueComplex.real();
      }
      break;

    case 6:
      {
      ComplexType C20, C12;
      function->SetP(2);
      function->SetQ(0);
      C20 = function->Evaluate();
      function->SetP(1);
      function->SetQ(2);
      C12 = function->Evaluate();

      HuValueComplex = C20 * vcl_pow(C12, 2);
      HuValue = HuValueComplex.real();
      }
      break;

    case 7:
      {
      ComplexType C30, C12;
      function->SetP(3);
      function->SetQ(0);
      C30 = function->Evaluate();
      function->SetP(1);
      function->SetQ(2);
      C12 = function->Evaluate();

      HuValueComplex = C30 * vcl_pow(C12, 3);
      HuValue = HuValueComplex.imag();
      }
      break;

    default:
      itkWarningMacro("Hu's invariant parameters are between 1 and 7");
    }

  return (static_cast<RealType>(HuValue));

}

template <class TInputPath, class TOutput, class TPrecision>
typename HuPathFunction<TInputPath, TOutput, TPrecision>::RealType
HuPathFunction<TInputPath, TOutput, TPrecision>
::Evaluate() const
{
  if (!this->GetInputPath())
    {
    otbMsgDevMacro(<< "Pb with GetInputPath");
    return static_cast<RealType>(itk::NumericTraits<PrecisionType>::max());
    }

  RealType Result =  Evaluate(*(this->GetInputPath()));

  return Result;

}

} // namespace otb

#endif
