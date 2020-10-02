/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbFlusserPathFunction_hxx
#define otbFlusserPathFunction_hxx

#include "otbFlusserPathFunction.h"
#include "otbComplexMomentPathFunction.h"
#include "itkNumericTraits.h"
#include "otbMacro.h"

namespace otb
{

/**
   * Constructor
   */
template <class TInputPath, class TOutput, class TPrecision>
FlusserPathFunction<TInputPath, TOutput, TPrecision>::FlusserPathFunction()
{
  m_MomentNumber = -1;
}

/**
   *
   */
template <class TInputPath, class TOutput, class TPrecision>
void FlusserPathFunction<TInputPath, TOutput, TPrecision>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os, indent);
  os << indent << " m_MomentNumber           : " << m_MomentNumber << std::endl;
}

template <class TInputPath, class TOutput, class TPrecision>
typename FlusserPathFunction<TInputPath, TOutput, TPrecision>::RealType
FlusserPathFunction<TInputPath, TOutput, TPrecision>::Evaluate(const PathType& itkNotUsed(path)) const
{
  typedef ComplexMomentPathFunction<PathType> FunctionType;
  typedef typename FunctionType::ComplexType  ComplexType;

  RealType    FlusserValue(itk::NumericTraits<RealType>::Zero);
  ComplexType FlusserValueComplex(itk::NumericTraits<ComplexType>::Zero);

  typename FunctionType::Pointer function = FunctionType::New();

  function->SetInputPath(this->GetInputPath());

  switch (m_MomentNumber)
  {
  case 1:
  {
    ComplexType C11;
    function->SetP(1);
    function->SetQ(1);
    C11          = function->Evaluate();
    FlusserValue = C11.real();
  }
  break;
  case 2:
  {
    ComplexType C21, C12;
    function->SetP(2);
    function->SetQ(1);
    C21 = function->Evaluate();
    function->SetP(1);
    function->SetQ(2);
    C12 = function->Evaluate();

    FlusserValue = std::abs(C21 * C12);
  }
  break;
  case 3:
  {
    ComplexType C20, C12;
    function->SetP(2);
    function->SetQ(0);
    C20 = function->Evaluate();
    function->SetP(1);
    function->SetQ(2);
    C12                 = function->Evaluate();
    FlusserValueComplex = C20 * std::pow(C12, 2);
    FlusserValue        = FlusserValueComplex.real();
  }
  break;
  case 4:
  {
    ComplexType C20, C12;
    function->SetP(2);
    function->SetQ(0);
    C20 = function->Evaluate();
    function->SetP(1);
    function->SetQ(2);
    C12                 = function->Evaluate();
    FlusserValueComplex = C20 * std::pow(C12, 2);
    FlusserValue        = FlusserValueComplex.imag();
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

    FlusserValueComplex = C30 * std::pow(C12, 3);
    FlusserValue        = FlusserValueComplex.real();
  }
  break;
  case 6:
  {
    ComplexType C30, C12;
    function->SetP(3);
    function->SetQ(0);
    C30 = function->Evaluate();
    function->SetP(1);
    function->SetQ(2);
    C12 = function->Evaluate();

    FlusserValueComplex = C30 * std::pow(C12, 3);
    FlusserValue        = FlusserValueComplex.imag();
  }
  break;
  case 7:
  {
    ComplexType C22;
    function->SetP(2);
    function->SetQ(2);
    C22          = function->Evaluate();
    FlusserValue = C22.real();
  }
  break;
  case 8:
  {
    ComplexType C31, C12;
    function->SetP(3);
    function->SetQ(1);
    C31 = function->Evaluate();
    function->SetP(1);
    function->SetQ(2);
    C12                 = function->Evaluate();
    FlusserValueComplex = C31 * std::pow(C12, 2);
    FlusserValue        = FlusserValueComplex.real();
  }
  break;
  case 9:
  {
    ComplexType C31, C12;
    function->SetP(3);
    function->SetQ(1);
    C31 = function->Evaluate();
    function->SetP(1);
    function->SetQ(2);
    C12                 = function->Evaluate();
    FlusserValueComplex = C31 * std::pow(C12, 2);
    FlusserValue        = FlusserValueComplex.imag();
  }
  break;
  case 10:
  {
    ComplexType C40, C12;
    function->SetP(4);
    function->SetQ(0);
    C40 = function->Evaluate();
    function->SetP(1);
    function->SetQ(2);
    C12                 = function->Evaluate();
    FlusserValueComplex = C40 * std::pow(C12, 4);
    FlusserValue        = FlusserValueComplex.real();
  }
  break;
  case 11:
  {
    ComplexType C40, C12;
    function->SetP(4);
    function->SetQ(0);
    C40 = function->Evaluate();
    function->SetP(1);
    function->SetQ(2);
    C12                 = function->Evaluate();
    FlusserValueComplex = C40 * std::pow(C12, 4);
    FlusserValue        = FlusserValueComplex.imag();
  }
  break;

  default:
    itkWarningMacro("Flusser's invariant parameters are between 1 and 11");
  }

  return (static_cast<RealType>(FlusserValue));
}

template <class TInputPath, class TOutput, class TPrecision>
typename FlusserPathFunction<TInputPath, TOutput, TPrecision>::RealType FlusserPathFunction<TInputPath, TOutput, TPrecision>::Evaluate() const
{
  if (!this->GetInputPath())
  {
    otbMsgDevMacro(<< "Pb with GetInputPath");
    return static_cast<RealType>(itk::NumericTraits<PrecisionType>::max());
  }

  RealType Result = Evaluate(*(this->GetInputPath()));

  return Result;
}

} // namespace otb

#endif
