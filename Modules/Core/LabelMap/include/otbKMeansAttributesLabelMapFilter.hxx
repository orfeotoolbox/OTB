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

#ifndef otbKMeansAttributesLabelMapFilter_hxx
#define otbKMeansAttributesLabelMapFilter_hxx

#include "otbKMeansAttributesLabelMapFilter.h"
#include "itkNumericTraits.h"
#include "itkMersenneTwisterRandomVariateGenerator.h"

namespace otb {

template <class TInputImage>
KMeansAttributesLabelMapFilter<TInputImage>
::KMeansAttributesLabelMapFilter()
 : m_LabelMapToSampleListFilter(LabelMapToSampleListFilterType::New()),
   m_NumberOfClasses(1)
{
}

template<class TInputImage>
void
KMeansAttributesLabelMapFilter<TInputImage>
::Compute()
{
  m_LabelMapToSampleListFilter->SetInputLabelMap(m_InputLabelMap);
  m_LabelMapToSampleListFilter->Update();

  typename ListSampleType::Pointer listSamples = const_cast<ListSampleType*>(m_LabelMapToSampleListFilter->GetOutputSampleList());
  typename TrainingListSampleType::Pointer trainingSamples = const_cast<TrainingListSampleType*>(m_LabelMapToSampleListFilter->GetOutputTrainingSampleList());

  // Build the Kd Tree
  typename TreeGeneratorType::Pointer kdTreeGenerator = TreeGeneratorType::New();
  kdTreeGenerator->SetSample(listSamples);
  kdTreeGenerator->SetBucketSize(100);
  kdTreeGenerator->Update();
  // Randomly pick the initial means among the classes
  unsigned int sampleSize = listSamples->GetMeasurementVector(0).Size();
  const unsigned int OneClassNbCentroids = 10;
  unsigned int numberOfCentroids = (m_NumberOfClasses == 1 ? OneClassNbCentroids : m_NumberOfClasses);
  typename EstimatorType::ParametersType initialMeans(sampleSize * m_NumberOfClasses);
  initialMeans.Fill(0.);

  if (m_NumberOfClasses > 1)
    {
    // For each class, choose a centroid as the first sample of this class encountered
    for (ClassLabelType classLabel = 0; classLabel < m_NumberOfClasses; ++classLabel)
      {
      typename TrainingListSampleType::ConstIterator it = trainingSamples->Begin();
      // Iterate on the label list and stop when classLabel is found
      // TODO: add random initialization ?
      for (it = trainingSamples->Begin(); it != trainingSamples->End(); ++it)
        {
        std::cout <<" Training Samples is "<<  it.GetMeasurementVector()[0] << std::endl;
        if (it.GetMeasurementVector()[0] == classLabel)
          break;
        }
      if (it == trainingSamples->End())
        {
        itkExceptionMacro(<<"Unable to find a sample with class label "<< classLabel);
        }

      typename ListSampleType::InstanceIdentifier identifier = it.GetInstanceIdentifier();
      const typename ListSampleType::MeasurementVectorType& centroid = listSamples->GetMeasurementVector(identifier);
      for (unsigned int i = 0; i < centroid.Size(); ++i)
        {
        initialMeans[classLabel * sampleSize + i] = centroid[i];
        }
      }
    }
  else
    {
    typedef itk::Statistics::MersenneTwisterRandomVariateGenerator RandomGeneratorType;
    RandomGeneratorType::Pointer randomGenerator = RandomGeneratorType::GetInstance();
    unsigned int nbLabelObjects = listSamples->Size();

    // Choose arbitrarily OneClassNbCentroids centroids among all available LabelObject
    for (unsigned int centroidId = 0; centroidId < numberOfCentroids; ++centroidId)
      {
      typename ListSampleType::InstanceIdentifier identifier = randomGenerator->GetIntegerVariate(nbLabelObjects - 1);
      const typename ListSampleType::MeasurementVectorType& centroid = listSamples->GetMeasurementVector(identifier);
      for (unsigned int i = 0; i < centroid.Size(); ++i)
        {
        initialMeans[centroidId * sampleSize + i] = centroid[i];
        }
      }
    }

  // Run the KMeans algorithm
  // Do KMeans estimation
  typename EstimatorType::Pointer estimator = EstimatorType::New();
  estimator->SetParameters(initialMeans);
  estimator->SetKdTree(kdTreeGenerator->GetOutput());
  estimator->SetMaximumIteration(10000);
  estimator->SetCentroidPositionChangesThreshold(0.00001);
  estimator->StartOptimization();

  // Retrieve final centroids
  m_Centroids.clear();

  for(unsigned int cId = 0; cId < numberOfCentroids; ++cId)
    {
    VectorType newCenter(sampleSize);
    for(unsigned int i = 0; i < sampleSize; ++i)
      {
      newCenter[i] = estimator->GetParameters()[cId * sampleSize + i];
      }
    m_Centroids.push_back(newCenter);
    }
}

}// end namespace otb
#endif
