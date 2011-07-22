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
#include "otbExhaustiveExponentialOptimizer.h"
#include "itkCommand.h"
#include "itkEventObject.h"

namespace otb
{

/**
 * Constructor
 */
ExhaustiveExponentialOptimizer
::ExhaustiveExponentialOptimizer()
{
  itkDebugMacro("Constructor");

  m_GeometricProgression = 2.0;
  m_StepLength = 1.0;
  m_CurrentIteration   =   0;
  m_CurrentValue = 0;
  m_CurrentParameter = 0;
  m_CurrentIndex.Fill(0);
  m_Stop = false;
  m_NumberOfSteps.Fill(0);
}

/**
 * Start walking
 */

void ExhaustiveExponentialOptimizer::StartOptimization(void)
{
  this->StartWalking();
}

void
ExhaustiveExponentialOptimizer
::StartWalking(void)
{
  itkDebugMacro("StartWalking");
  this->InvokeEvent(itk::StartEvent());

  ParametersType initialPos = this->GetInitialPosition();
  m_MinimumMetricValuePosition = initialPos;
  m_MaximumMetricValuePosition = initialPos;

  MeasureType initialValue = this->GetValue(this->GetInitialPosition());
  m_MaximumMetricValue = initialValue;
  m_MinimumMetricValue = initialValue;

  m_CurrentIteration          = 0;
  m_MaximumNumberOfIterations = 1;

  const unsigned int spaceDimension = this->GetInitialPosition().GetSize();

  for (unsigned int i = 0; i < spaceDimension; ++i)
    {
    m_MaximumNumberOfIterations *= (2 * m_NumberOfSteps[i] + 1);
    }

  m_CurrentIndex.SetSize(spaceDimension);
  m_CurrentIndex.Fill(0);

  ScalesType scales = this->GetScales();

  // Make sure the scales have been set properly
  if (scales.size() != spaceDimension)
    {
    itkExceptionMacro(<< "The size of Scales is "
                      << scales.size()
                      << ", but the NumberOfParameters is "
                      << spaceDimension
                      << ".");
    }

  // Setup first grid position.
  ParametersType position(spaceDimension);
  for (unsigned int i = 0; i < spaceDimension; ++i)
    {
    position[i] = this->GetInitialPosition()[i] *
                  vcl_pow(m_GeometricProgression, -static_cast<double>(m_NumberOfSteps[i]) * m_StepLength) * scales[i];
    }
  this->SetCurrentPosition(position);

  itkDebugMacro("Calling ResumeWalking");

  this->ResumeWalking();
}

/**
 * Resume the optimization
 */
void
ExhaustiveExponentialOptimizer
::ResumeWalking(void)
{
  itkDebugMacro("ResumeWalk");
  m_Stop = false;

  while (!m_Stop)
    {
    ParametersType currentPosition = this->GetCurrentPosition();

    if (m_Stop)
      {
      StopWalking();
      break;
      }

    m_CurrentValue = this->GetValue(currentPosition);

    if (m_CurrentValue > m_MaximumMetricValue)
      {
      m_MaximumMetricValue = m_CurrentValue;
      m_MaximumMetricValuePosition = currentPosition;
      }
    if (m_CurrentValue < m_MinimumMetricValue)
      {
      m_MinimumMetricValue = m_CurrentValue;
      m_MinimumMetricValuePosition = currentPosition;
      }

    if (m_Stop)
      {
      this->StopWalking();
      break;
      }

    this->InvokeEvent(itk::IterationEvent());
    this->AdvanceOneStep();
    m_CurrentIteration++;
    }
}

void
ExhaustiveExponentialOptimizer
::StopWalking(void)
{
  itkDebugMacro("StopWalking");

  m_Stop = true;
  this->InvokeEvent(itk::EndEvent());
}

void
ExhaustiveExponentialOptimizer
::AdvanceOneStep(void)
{
  itkDebugMacro("AdvanceOneStep");

  const unsigned int spaceDimension = m_CostFunction->GetNumberOfParameters();

  ParametersType newPosition(spaceDimension);
  IncrementIndex(newPosition);

  itkDebugMacro(<< "new position = " << newPosition);

  this->SetCurrentPosition(newPosition);
}

void
ExhaustiveExponentialOptimizer
::IncrementIndex(ParametersType& newPosition)
{
  unsigned int       idx = 0;
  const unsigned int spaceDimension = m_CostFunction->GetNumberOfParameters();

  while (idx < spaceDimension)
    {
    m_CurrentIndex[idx]++;

    if (m_CurrentIndex[idx] > (2 * m_NumberOfSteps[idx]))
      {
      m_CurrentIndex[idx] = 0;
      ++idx;
      }
    else
      {
      break;
      }
    }

  if (idx == spaceDimension)
    {
    m_Stop = true;
    }

  for (unsigned int i = 0; i < spaceDimension; ++i)
    {
    newPosition[i] = this->GetInitialPosition()[i]
                     * this->GetScales()[i]
                     * vcl_pow(m_GeometricProgression,
                               static_cast<double>(m_CurrentIndex[i] - m_NumberOfSteps[i]) * m_StepLength);
    }
}

void
ExhaustiveExponentialOptimizer
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "CurrentValue = " << m_CurrentValue << std::endl;
  os << indent << "NumberOfSteps = " << m_NumberOfSteps << std::endl;
  os << indent << "CurrentIteration = " << m_CurrentIteration << std::endl;
  os << indent << "Stop = " << m_Stop << std::endl;
  os << indent << "CurrentParameter = " << m_CurrentParameter << std::endl;
  os << indent << "GeometricProgression = " << m_GeometricProgression << std::endl;
  os << indent << "CurrentIndex = " << m_CurrentIndex << std::endl;
  os << indent << "MaximumNumberOfIterations = " << m_MaximumNumberOfIterations << std::endl;
  os << indent << "MaximumMetricValue = " << m_MaximumMetricValue << std::endl;
  os << indent << "MinimumMetricValue = " << m_MinimumMetricValue << std::endl;
  os << indent << "MinimumMetricValuePosition = " << m_MinimumMetricValuePosition << std::endl;
  os << indent << "MaximumMetricValuePosition = " << m_MaximumMetricValuePosition << std::endl;
}

} // end namespace itk
