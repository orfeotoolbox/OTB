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
#ifndef __otbStandardDSCostFunction_txx
#define __otbStandardDSCostFunction_txx

#include "otbStandardDSCostFunction.h"

namespace otb
{
// Constructor
template <class TDSValidationFilter>
StandardDSCostFunction<TDSValidationFilter>
::StandardDSCostFunction() :
  m_CriterionFormula("((Belief + Plausibility)/2.)"),
  m_Weight(0.5)
{
 m_GTVectorData = VectorDataType::New();
 m_NSVectorData = VectorDataType::New();
 m_Parser =  ParserType::New();
}

template <class TDSValidationFilter>
unsigned int
StandardDSCostFunction<TDSValidationFilter>
::GetNumberOfParameters() const
 {
  return m_DescriptorList.size()*4;
 }

template <class TDSValidationFilter>
typename StandardDSCostFunction<TDSValidationFilter>
::MeasureType
 StandardDSCostFunction<TDSValidationFilter>
::GetValue(const ParametersType & parameters) const
 {
  if (parameters.size() != m_DescriptorList.size()*4)
    {
    itkExceptionMacro(<< "Wrong model!" )
    }

  //Initialize parser
  m_Parser->SetExpr(m_CriterionFormula);

  DescriptorsModelType descModel;
  for (unsigned int i = 0; i < m_DescriptorList.size(); ++i)
    {
    std::vector<double> tmp;
    for (unsigned int j = 0; j < 4; ++j)
      {
      tmp.push_back(parameters[4*i+j]);
      }
    PairType pair( m_DescriptorList[i], tmp);
    descModel.push_back(pair);
    }

  typename DSValidationFilterType::Pointer internalFunctionGT = DSValidationFilterType::New();
  internalFunctionGT->SetCriterionFormula("1");
  internalFunctionGT->SetInput(m_GTVectorData);
  internalFunctionGT->SetBeliefHypothesis(m_BeliefHypothesis);
  internalFunctionGT->SetPlausibilityHypothesis(m_PlausibilityHypothesis);
  try
    {
    internalFunctionGT->SetDescriptorModels(descModel);
    }
  catch (itk::ExceptionObject /*& err*/)
    {
    return 1;
    }
  internalFunctionGT->Update();

  typename DSValidationFilterType::Pointer internalFunctionNS = DSValidationFilterType::New();
  internalFunctionNS->SetCriterionFormula("1");
  internalFunctionNS->SetInput(m_NSVectorData);
  internalFunctionNS->SetBeliefHypothesis(m_BeliefHypothesis);
  internalFunctionNS->SetBeliefHypothesis(m_PlausibilityHypothesis);
  try
    {
    internalFunctionNS->SetDescriptorModels(descModel);
    }
  catch (itk::ExceptionObject &)
    {
    return 1;
    }
  internalFunctionNS->Update();

  double accGT = 0.0;
  double accNS = 0.0;
  double nGT = 0.0;
  double nNS = 0.0;

  TreeIteratorType itVectorGT(internalFunctionGT->GetOutput()->GetDataTree());
  itVectorGT.GoToBegin();
  while (!itVectorGT.IsAtEnd())
    {
    if (!itVectorGT.Get()->IsRoot() && !itVectorGT.Get()->IsDocument() && !itVectorGT.Get()->IsFolder())
      {
      double belief = itVectorGT.Get()->GetFieldAsDouble("Belief");
      double plausibility = itVectorGT.Get()->GetFieldAsDouble("Plausi");

      m_Parser->DefineVar("Belief", &belief);
      m_Parser->DefineVar("Plausibility", &plausibility);

      accGT += ((1 - m_Parser->Eval()) * (1 - m_Parser->Eval()));
      nGT += 1.0;

      m_Parser->ClearVar();
      }
    itVectorGT++;
    }

  TreeIteratorType itVectorNS(internalFunctionNS->GetOutput()->GetDataTree());
  itVectorNS.GoToBegin();
  while (!itVectorNS.IsAtEnd())
    {
    if (!itVectorNS.Get()->IsRoot() && !itVectorNS.Get()->IsDocument() && !itVectorNS.Get()->IsFolder())
      {
      double belief = itVectorNS.Get()->GetFieldAsDouble("Belief");
      double plausibility = itVectorNS.Get()->GetFieldAsDouble("Plausi");

      m_Parser->DefineVar("Belief", &belief);
      m_Parser->DefineVar("Plausibility", &plausibility);

      accNS += (m_Parser->Eval() * m_Parser->Eval());
      nNS += 1.0;

      m_Parser->ClearVar();
      }
    itVectorNS++;
    }
  return (m_Weight * accGT / nGT + (1 - m_Weight) * accNS / nNS);
}

template <class TDSValidationFilter>
void
StandardDSCostFunction<TDSValidationFilter>
::GetDerivative(const ParametersType & itkNotUsed(parameters), DerivativeType & itkNotUsed(derivative)) const
 {
  //Not necessary for Amoeba Optimizer
  itkExceptionMacro(<< "Not Supposed to be used when using Amoeba Optimizer!")
 }

// PrintSelf Method
template <class TDSValidationFilter>
void
StandardDSCostFunction<TDSValidationFilter>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}


}// end namespace otb

#endif
