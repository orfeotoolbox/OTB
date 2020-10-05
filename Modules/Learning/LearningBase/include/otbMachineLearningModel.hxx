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

#ifndef otbMachineLearningModel_hxx
#define otbMachineLearningModel_hxx

#ifdef _OPENMP
#include <omp.h>
#endif

#include "otbMachineLearningModel.h"

#include "itkMultiThreader.h"

namespace otb
{

template <class TInputValue, class TOutputValue, class TConfidenceValue>
MachineLearningModel<TInputValue, TOutputValue, TConfidenceValue>::MachineLearningModel()
  : m_RegressionMode(false),
    m_IsRegressionSupported(false),
    m_ConfidenceIndex(false),
    m_ProbaIndex(false),
    m_IsDoPredictBatchMultiThreaded(false),
    m_Dimension(0)
{
}

template <class TInputValue, class TOutputValue, class TConfidenceValue>
void MachineLearningModel<TInputValue, TOutputValue, TConfidenceValue>::SetRegressionMode(bool flag)
{
  if (flag && !m_IsRegressionSupported)
  {
    itkGenericExceptionMacro(<< "Regression mode not implemented.");
  }
  if (m_RegressionMode != flag)
  {
    m_RegressionMode = flag;
    this->Modified();
  }
}

template <class TInputValue, class TOutputValue, class TConfidenceValue>
typename MachineLearningModel<TInputValue, TOutputValue, TConfidenceValue>::TargetSampleType
MachineLearningModel<TInputValue, TOutputValue, TConfidenceValue>::Predict(const InputSampleType& input, ConfidenceValueType* quality,
                                                                           ProbaSampleType* proba) const
{
  // Call protected specialization entry point
  return this->DoPredict(input, quality, proba);
}


template <class TInputValue, class TOutputValue, class TConfidenceValue>
typename MachineLearningModel<TInputValue, TOutputValue, TConfidenceValue>::TargetListSampleType::Pointer
MachineLearningModel<TInputValue, TOutputValue, TConfidenceValue>::PredictBatch(const InputListSampleType* input, ConfidenceListSampleType* quality,
                                                                                ProbaListSampleType* proba) const
{
  // std::cout << "Enter batch predict" << std::endl;
  typename TargetListSampleType::Pointer targets = TargetListSampleType::New();
  targets->Resize(input->Size());

  if (quality != nullptr)
  {
    quality->Clear();
    quality->Resize(input->Size());
  }
  if (proba != ITK_NULLPTR)
  {
    proba->Clear();
    proba->Resize(input->Size());
  }
  if (m_IsDoPredictBatchMultiThreaded)
  {
    // Simply calls DoPredictBatch
    this->DoPredictBatch(input, 0, input->Size(), targets, quality, proba);
    return targets;
  }
  else
  {
#ifdef _OPENMP
    // OpenMP threading here
    unsigned int nb_threads(0), threadId(0), nb_batches(0);

#pragma omp parallel shared(nb_threads, nb_batches) private(threadId)
    {
      // Get number of threads configured with ITK
      omp_set_num_threads(itk::MultiThreader::GetGlobalDefaultNumberOfThreads());
      nb_threads = omp_get_num_threads();
      threadId   = omp_get_thread_num();
      nb_batches = std::min(nb_threads, (unsigned int)input->Size());
      // Ensure that we do not spawn unnecessary threads
      if (threadId < nb_batches)
      {
        unsigned int batch_size  = ((unsigned int)input->Size() / nb_batches);
        unsigned int batch_start = threadId * batch_size;
        if (threadId == nb_threads - 1)
        {
          batch_size += input->Size() % nb_batches;
        }

        this->DoPredictBatch(input, batch_start, batch_size, targets, quality, proba);
      }
    }
#else
    this->DoPredictBatch(input, 0, input->Size(), targets, quality, proba);
#endif
    return targets;
  }
}


template <class TInputValue, class TOutputValue, class TConfidenceValue>
void MachineLearningModel<TInputValue, TOutputValue, TConfidenceValue>::DoPredictBatch(const InputListSampleType* input, const unsigned int& startIndex,
                                                                                       const unsigned int& size, TargetListSampleType* targets,
                                                                                       ConfidenceListSampleType* quality, ProbaListSampleType* proba) const
{
  assert(input != nullptr);
  assert(targets != nullptr);

  assert(input->Size() == targets->Size() && "Input sample list and target label list do not have the same size.");
  assert(((quality == nullptr) || (quality->Size() == input->Size())) &&
         "Quality samples list is not null and does not have the same size as input samples list");
  assert(((proba == nullptr) || (input->Size() == proba->Size())) && "Proba sample list and target label list do not have the same size.");

  if (startIndex + size > input->Size())
  {
    itkExceptionMacro(<< "requested range [" << startIndex << ", " << startIndex + size << "[ partially outside input sample list range.[0," << input->Size()
                      << "[");
  }

  if (proba != nullptr)
  {
    for (unsigned int id = startIndex; id < startIndex + size; ++id)
    {
      ProbaSampleType        prob;
      ConfidenceValueType    confidence = 0;
      const TargetSampleType target     = this->DoPredict(input->GetMeasurementVector(id), &confidence, &prob);
      quality->SetMeasurementVector(id, confidence);
      proba->SetMeasurementVector(id, prob);
      targets->SetMeasurementVector(id, target);
    }
  }
  else if (quality != ITK_NULLPTR)
  {
    for (unsigned int id = startIndex; id < startIndex + size; ++id)
    {
      ConfidenceValueType    confidence = 0;
      const TargetSampleType target     = this->DoPredict(input->GetMeasurementVector(id), &confidence);
      quality->SetMeasurementVector(id, confidence);
      targets->SetMeasurementVector(id, target);
    }
  }
  else
  {
    for (unsigned int id = startIndex; id < startIndex + size; ++id)
    {
      const TargetSampleType target = this->DoPredict(input->GetMeasurementVector(id));
      targets->SetMeasurementVector(id, target);
    }
  }
}

template <class TInputValue, class TOutputValue, class TConfidenceValue>
void MachineLearningModel<TInputValue, TOutputValue, TConfidenceValue>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  // Call superclass implementation
  Superclass::PrintSelf(os, indent);
}
}

#endif
