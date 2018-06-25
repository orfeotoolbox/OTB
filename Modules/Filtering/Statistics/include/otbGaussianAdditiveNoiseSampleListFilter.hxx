/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbGaussianAdditiveNoiseSampleListFilter_hxx
#define otbGaussianAdditiveNoiseSampleListFilter_hxx

#include "otbGaussianAdditiveNoiseSampleListFilter.h"
#include "itkMersenneTwisterRandomVariateGenerator.h"
#include "itkProgressReporter.h"

namespace otb {
namespace Statistics {

template < class TInputSampleList, class TOutputSampleList >
GaussianAdditiveNoiseSampleListFilter<TInputSampleList, TOutputSampleList>
::GaussianAdditiveNoiseSampleListFilter()
{
  m_Mean = 0.;
  m_Variance = 1e-3;
  m_NumberOfIteration = 1;
}

template < class TInputSampleList, class TOutputSampleList >
void
GaussianAdditiveNoiseSampleListFilter<TInputSampleList, TOutputSampleList>
::GenerateRandomSequence()
{
  typedef itk::Statistics::MersenneTwisterRandomVariateGenerator GeneratorType;
  GeneratorType::Pointer generator = GeneratorType::GetInstance();

  // Clear the coefficients vector first
  m_WhiteGaussianNoiseCoefficients.clear();

  // Get the size of the measurement vectors
  unsigned int size = this->GetInput()->GetMeasurementVectorSize();
  if(size == 0)
    {
    itkExceptionMacro(<< "MeasurementVector size is  "<<size << " , expected non null size " );
    }
  else
    for(unsigned int i = 0; i <size; ++i)
      {
      double ran = generator->GetNormalVariate(m_Mean, m_Variance);
      m_WhiteGaussianNoiseCoefficients.push_back(ran);
      }
}

template < class TInputSampleList, class TOutputSampleList >
void
GaussianAdditiveNoiseSampleListFilter<TInputSampleList, TOutputSampleList>
::GenerateData()
{
  // Retrieve input and output pointers
  InputSampleListConstPointer inputSampleListPtr  = this->GetInput();
  OutputSampleListPointer     outputSampleListPtr = this->GetOutput();

  // Clear any previous output
  outputSampleListPtr->Clear();

  // Set the measurement vector size
  outputSampleListPtr->SetMeasurementVectorSize(inputSampleListPtr->GetMeasurementVectorSize());

  // Set-up progress reporting
  itk::ProgressReporter progress(this, 0, inputSampleListPtr->Size()*m_NumberOfIteration);

  // Iterate m_NumberOfIteration-times the noising process
  for (unsigned int currIteration = 0; currIteration< m_NumberOfIteration; currIteration++)
    {
    typename InputSampleListType::ConstIterator inputIt = inputSampleListPtr->Begin();

    // Iterate on the InputSampleList
    while(inputIt != inputSampleListPtr->End())
      {
      // Generate Random sequence
      this->GenerateRandomSequence();

      // Retrieve current input sample
      InputMeasurementVectorType currentInputMeasurement = inputIt.GetMeasurementVector();

      // Build current output sample
      OutputMeasurementVectorType currentOutputMeasurement;
      currentOutputMeasurement.SetSize(currentInputMeasurement.GetSize());

      // Add the white noise to each component of the sample
      for(unsigned int idx = 0; idx < inputSampleListPtr->GetMeasurementVectorSize(); ++idx)
        {
        currentOutputMeasurement[idx] = static_cast<OutputValueType>(
          (static_cast<double>(currentInputMeasurement[idx])+m_WhiteGaussianNoiseCoefficients[idx]));
        }

      // Add the current output sample to the output SampleList
      outputSampleListPtr->PushBack(currentOutputMeasurement);

      // Update progress
      progress.CompletedPixel();

      ++inputIt;
      }
    }
}

template < class TInputSampleList, class TOutputSampleList >
void
GaussianAdditiveNoiseSampleListFilter<TInputSampleList, TOutputSampleList>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  // Call superclass implementation
  Superclass::PrintSelf(os, indent);
}

} // End namespace Statistics
} // End namespace otb

#endif
