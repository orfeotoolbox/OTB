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
#ifndef otbSVMCrossValidationCostFunction_txx
#define otbSVMCrossValidationCostFunction_txx

#include "otbSVMCrossValidationCostFunction.h"

namespace otb
{
template<class TModel>
SVMCrossValidationCostFunction<TModel>
::SVMCrossValidationCostFunction() : m_Model(), m_NumberOfCrossValidationFolders(10), m_DerivativeStep(0.001)
{}
template<class TModel>
SVMCrossValidationCostFunction<TModel>
::~SVMCrossValidationCostFunction()
{}
template<class TModel>
typename SVMCrossValidationCostFunction<TModel>
::MeasureType
SVMCrossValidationCostFunction<TModel>
::GetValue(const ParametersType& parameters) const
{
  // Check the input model
  if (!m_Model)
    {
    itkExceptionMacro(<< "Model is null, can not evaluate accuracy.");
    }

  // Check for a positive and non-null C
  if (parameters[0] <= 0)
    {
    return 0;
    }

  // Updates vm_parameters according to current parameters
  this->UpdateParameters(m_Model->GetParameters(), parameters);

  return m_Model->CrossValidation(m_NumberOfCrossValidationFolders);
}

template<class TModel>
void
SVMCrossValidationCostFunction<TModel>
::GetDerivative(const ParametersType& parameters, DerivativeType& derivative) const
{
  // Set derivative size
  derivative.SetSize(parameters.Size());
  derivative.Fill(itk::NumericTraits<ParametersValueType>::Zero);

  for (unsigned int i = 0; i < parameters.Size(); ++i)
    {
    MeasureType    y1, y2;
    ParametersType x1, x2;

    x1 = parameters;
    x1[i] -= m_DerivativeStep;
    y1 = this->GetValue(x1);

    x2 = parameters;
    x2[i] += m_DerivativeStep;
    y2 = this->GetValue(x2);

    derivative[i] = (y2 - y1) / (2 * m_DerivativeStep);
    otbMsgDevMacro( << "x1= " << x1 << " x2= " << x2 << ", y1= " << y1 << ", y2= " << y2 );
    }
  otbMsgDevMacro( "Position: " << parameters << ", Value: " << this->GetValue(parameters)
                  << ", Derivatives: " << derivative );
}

template<class TModel>
unsigned int
SVMCrossValidationCostFunction<TModel>
::GetNumberOfParameters(void) const
{
  if (!m_Model)
    {
    itkExceptionMacro(<< "Model is null, can not evaluate number of parameters.");
    }

  switch (m_Model->GetKernelType())
    {
    case LINEAR:
      // C
      return 1;

    case POLY:
      // C, gamma and coef0
      return 3;

    case RBF:
      // C and gamma
      return 2;

    case SIGMOID:
      // C, gamma and coef0
      return 3;

    default:
      // C
      return 1;
    }
}

template<class TModel>
void
SVMCrossValidationCostFunction<TModel>
::UpdateParameters(struct svm_parameter& svm_parameters, const ParametersType& parameters) const
{
  switch (m_Model->GetKernelType())
    {
    case LINEAR:
      // C
      svm_parameters.C = parameters[0];
      break;

    case POLY:
      // C, gamma and coef0
      svm_parameters.C = parameters[0];
      svm_parameters.gamma = parameters[1];
      svm_parameters.coef0 = parameters[2];
      break;

    case RBF:
      // C and gamma
      svm_parameters.C = parameters[0];
      svm_parameters.gamma = parameters[1];
      break;

    case SIGMOID:
      // C, gamma and coef0
      svm_parameters.C = parameters[0];
      svm_parameters.gamma = parameters[1];
      svm_parameters.coef0 = parameters[2];
      break;

    default:
      svm_parameters.C = parameters[0];
      break;
    }
}

} // namespace otb

#endif
