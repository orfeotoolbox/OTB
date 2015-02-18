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
#ifndef __otbSVMModelEstimator_txx
#define __otbSVMModelEstimator_txx

#include "otbSVMModelEstimator.h"
#include "otbSVMCrossValidationCostFunction.h"
#include "otbExhaustiveExponentialOptimizer.h"
#include "itkRegularStepGradientDescentOptimizer.h"
#include "otbMacro.h"
#include "itkCommand.h"

namespace otb
{
template<class InputPixelType, class LabelPixelType>
SVMModelEstimator<InputPixelType, LabelPixelType>
::SVMModelEstimator(void) :
  m_NumberOfClasses(0)
{
  // Cross validation accuracy measures
  m_InitialCrossValidationAccuracy = 0.;
  m_FinalCrossValidationAccuracy = 0.;
  m_ParametersOptimization = false;
  m_NumberOfCrossValidationFolders = 5;
  m_CoarseOptimizationNumberOfSteps = 5;
  m_FineOptimizationNumberOfSteps = 5;

  this->SetNumberOfRequiredInputs(0);
  this->SetNumberOfRequiredOutputs(1);
  this->SetNthOutput(0, ModelType::New());
}

template<class InputPixelType, class LabelPixelType>
typename SVMModelEstimator<InputPixelType, LabelPixelType>::ModelType *
SVMModelEstimator<InputPixelType, LabelPixelType>
::GetModel()
{
  if (this->GetNumberOfOutputs() < 1)
    {
    return 0;
    }
  return static_cast<ModelType *>(this->itk::ProcessObject::GetOutput(0));
}

template<class InputPixelType, class LabelPixelType>
const typename SVMModelEstimator<InputPixelType, LabelPixelType>::ModelType *
SVMModelEstimator<InputPixelType, LabelPixelType>
::GetModel() const
{
  if (this->GetNumberOfOutputs() < 1)
    {
    return 0;
    }
  return static_cast<const ModelType *>(this->itk::ProcessObject::GetOutput(0));
}

template<class InputPixelType, class LabelPixelType>
SVMModelEstimator<InputPixelType, LabelPixelType>
::~SVMModelEstimator(void)
{}

/*
 * PrintSelf
 */
template<class InputPixelType, class LabelPixelType>
void
SVMModelEstimator<InputPixelType, LabelPixelType>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

} // end PrintSelf

/**
 * Generate data (start the model building process)
 */
template<class InputPixelType, class LabelPixelType>
void
SVMModelEstimator<InputPixelType, LabelPixelType>
::GenerateData()
{
  // Get the pointer to the output model
  ModelType * outputPtr = this->GetModel();

  // Reset the model
  outputPtr->Reset();

  // Prepare data
  this->PrepareData();

  // Compute accuracy and eventually optimize parameters
  this->OptimizeParameters();

  // Train the model
  outputPtr->Train();

} // end Generate data

template<class InputPixelType, class LabelPixelType>
void
SVMModelEstimator<InputPixelType, LabelPixelType>
::OptimizeParameters()
{
  typedef SVMCrossValidationCostFunction<SVMModelType> CrossValidationFunctionType;

  typename CrossValidationFunctionType::Pointer crossValidationFunction = CrossValidationFunctionType::New();

  crossValidationFunction->SetModel(this->GetModel());
  crossValidationFunction->SetNumberOfCrossValidationFolders(m_NumberOfCrossValidationFolders);

  typename CrossValidationFunctionType::ParametersType initialParameters, coarseBestParameters, fineBestParameters;

  switch (this->GetModel()->GetKernelType())
    {
    case LINEAR:
      // C
      initialParameters.SetSize(1);
      initialParameters[0] = this->GetModel()->GetC();
      break;

    case POLY:
      // C, gamma and coef0
      initialParameters.SetSize(3);
      initialParameters[0] = this->GetModel()->GetC();
      initialParameters[1] = this->GetModel()->GetKernelGamma();
      initialParameters[2] = this->GetModel()->GetKernelCoef0();
      break;

    case RBF:
      // C and gamma
      initialParameters.SetSize(2);
      initialParameters[0] = this->GetModel()->GetC();
      initialParameters[1] = this->GetModel()->GetKernelGamma();
      break;

    case SIGMOID:
      // C, gamma and coef0
      initialParameters.SetSize(3);
      initialParameters[0] = this->GetModel()->GetC();
      initialParameters[1] = this->GetModel()->GetKernelGamma();
      initialParameters[2] = this->GetModel()->GetKernelCoef0();
      break;

    default:
      // Only C
      initialParameters.SetSize(1);
      initialParameters[0] = this->GetModel()->GetC();
      break;
    }

  m_InitialCrossValidationAccuracy = crossValidationFunction->GetValue(initialParameters);
  m_FinalCrossValidationAccuracy = m_InitialCrossValidationAccuracy;

  otbMsgDebugMacro(<< "Initial accuracy : " << m_InitialCrossValidationAccuracy
                   << ", Parameters Optimization" << m_ParametersOptimization);

  if (m_ParametersOptimization)
    {
    otbMsgDebugMacro(<< "Model parameters optimization");

    typename ExhaustiveExponentialOptimizer::Pointer coarseOptimizer = ExhaustiveExponentialOptimizer::New();
    typename ExhaustiveExponentialOptimizer::StepsType coarseNbSteps(initialParameters.Size());
    coarseNbSteps.Fill(m_CoarseOptimizationNumberOfSteps);

    coarseOptimizer->SetNumberOfSteps(coarseNbSteps);
    coarseOptimizer->SetCostFunction(crossValidationFunction);
    coarseOptimizer->SetInitialPosition(initialParameters);
    coarseOptimizer->StartOptimization();

    coarseBestParameters = coarseOptimizer->GetMaximumMetricValuePosition();

    otbMsgDevMacro( << "Coarse minimum accuracy: " << coarseOptimizer->GetMinimumMetricValue() << " " <<
      coarseOptimizer->GetMinimumMetricValuePosition() );
    otbMsgDevMacro( << "Coarse maximum accuracy: " << coarseOptimizer->GetMaximumMetricValue() << " " <<
      coarseOptimizer->GetMaximumMetricValuePosition() );

    typename ExhaustiveExponentialOptimizer::Pointer fineOptimizer = ExhaustiveExponentialOptimizer::New();
    typename ExhaustiveExponentialOptimizer::StepsType fineNbSteps(initialParameters.Size());
    fineNbSteps.Fill(m_FineOptimizationNumberOfSteps);

    double stepLength = 1. / static_cast<double>(m_FineOptimizationNumberOfSteps);

    fineOptimizer->SetNumberOfSteps(fineNbSteps);
    fineOptimizer->SetStepLength(stepLength);
    fineOptimizer->SetCostFunction(crossValidationFunction);
    fineOptimizer->SetInitialPosition(coarseBestParameters);
    fineOptimizer->StartOptimization();

    otbMsgDevMacro(<< "Fine minimum accuracy: " << fineOptimizer->GetMinimumMetricValue() << " " <<
      fineOptimizer->GetMinimumMetricValuePosition() );
    otbMsgDevMacro(<< "Fine maximum accuracy: " << fineOptimizer->GetMaximumMetricValue() << " " <<
      fineOptimizer->GetMaximumMetricValuePosition() );

    fineBestParameters = fineOptimizer->GetMaximumMetricValuePosition();

    m_FinalCrossValidationAccuracy = fineOptimizer->GetMaximumMetricValue();

    switch (this->GetModel()->GetKernelType())
      {
      case LINEAR:
        // C
        this->GetModel()->SetC(fineBestParameters[0]);
        break;

      case POLY:
        // C, gamma and coef0
        this->GetModel()->SetC(fineBestParameters[0]);
        this->GetModel()->SetKernelGamma(fineBestParameters[1]);
        this->GetModel()->SetKernelCoef0(fineBestParameters[2]);
        break;

      case RBF:
        // C and gamma
        this->GetModel()->SetC(fineBestParameters[0]);
        this->GetModel()->SetKernelGamma(fineBestParameters[1]);
        break;

      case SIGMOID:
        // C, gamma and coef0
        this->GetModel()->SetC(fineBestParameters[0]);
        this->GetModel()->SetKernelGamma(fineBestParameters[1]);
        this->GetModel()->SetKernelCoef0(fineBestParameters[2]);
        break;

      default:
        // Only C
        this->GetModel()->SetC(fineBestParameters[0]);
        break;
      }
    }
}

} //End namespace OTB
#endif
