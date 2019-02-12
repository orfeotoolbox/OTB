/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbSVMMarginSampler_hxx
#define otbSVMMarginSampler_hxx

#include "otbSVMMarginSampler.h"
#include "otbMacro.h"

namespace otb
{

template< class TSample, class TModel >
SVMMarginSampler< TSample, TModel >
::SVMMarginSampler()
{
  m_NumberOfCandidates = 10;
}

template< class TSample, class TModel >
void
SVMMarginSampler< TSample, TModel >
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

template< class TSample, class TModel >
void
SVMMarginSampler< TSample, TModel >
::GenerateData()
{
  if(!m_Model)
    {
    itkExceptionMacro("No model, can not do classification.");
    }

  if(m_Model->GetNumberOfSupportVectors() == 0)
    {
    itkExceptionMacro(<<"SVM model does not contain any support vector, can not perform margin sampling.");
    }

  OutputType* output = const_cast<OutputType*>(this->GetOutput());
  output->SetSample(this->GetInput());

  this->DoMarginSampling();
}

template< class TSample, class TModel >
void
SVMMarginSampler< TSample, TModel >
::DoMarginSampling()
{
  IndexAndDistanceVectorType idDistVector;
  OutputType* output = const_cast<OutputType*>(this->GetOutput());

  typename TSample::ConstIterator iter = this->GetInput()->Begin();
  typename TSample::ConstIterator end  = this->GetInput()->End();

  typename OutputType::ConstIterator iterO = output->Begin();
  typename OutputType::ConstIterator endO  = output->End();
  typename TSample::MeasurementVectorType measurements;

  m_Model->SetConfidenceMode(TModel::CM_HYPER);

  int numberOfComponentsPerSample  = iter.GetMeasurementVector().Size();

  int nbClass = static_cast<int>(m_Model->GetNumberOfClasses());
  std::vector<double> hdistances(nbClass * (nbClass - 1) / 2);

  otbMsgDevMacro(  << "Starting iterations " );
  while (iter != end && iterO != endO)
  {
    int i = 0;
    typename SVMModelType::InputSampleType     modelMeasurement(numberOfComponentsPerSample);

    measurements = iter.GetMeasurementVector();
    // otbMsgDevMacro(  << "Loop on components " << svm_type );
    for (i=0; i<numberOfComponentsPerSample; ++i)
    {
    modelMeasurement[i] = measurements[i];
    }

    // Get distances to the hyperplanes
    m_Model->Predict(modelMeasurement, &(hdistances[0]));
    double minDistance = std::abs(hdistances[0]);

    // Compute th min distances
    for(unsigned int j = 1; j<hdistances.size(); ++j)
      {
      if(std::abs(hdistances[j])<minDistance)
        {
        minDistance = std::abs(hdistances[j]);
        }
      }
    // Keep index and min distance
    IndexAndDistanceType value(iter.GetInstanceIdentifier(), minDistance);
    idDistVector.push_back(value);

    ++iter;
    ++iterO;
  }

  // Sort index by increasing distances
  sort(idDistVector.begin(), idDistVector.end(), &Compare);

  // Display the first 10 values
  otbMsgDevMacro( <<" Margin Sampling: " );

  // Clear previous margin samples
  m_MarginSamples.clear();

  for(unsigned int i = 0; i<m_NumberOfCandidates && i<idDistVector.size(); ++i)
    {
    otbMsgDevMacro( "Sample "<<idDistVector[i].first<<" (distance= "<<idDistVector[i].second<<")" )
    m_MarginSamples.push_back(idDistVector[i].first);
    }

}

} // end of namespace otb

#endif
