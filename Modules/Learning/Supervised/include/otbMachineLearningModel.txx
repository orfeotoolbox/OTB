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
#ifndef otbMachineLearningModel_txx
#define otbMachineLearningModel_txx

#ifdef _OPENMP
 # include <omp.h>
#endif

#include "otbMachineLearningModel.h"

namespace otb
{

template <class TInputValue, class TOutputValue, class TConfidenceValue>
MachineLearningModel<TInputValue,TOutputValue,TConfidenceValue>
::MachineLearningModel() :
  m_RegressionMode(false),
  m_IsRegressionSupported(false),
  m_ConfidenceIndex(false),
  m_IsDoPredictBatchMultiThreaded(false)
{}


template <class TInputValue, class TOutputValue, class TConfidenceValue>
MachineLearningModel<TInputValue,TOutputValue,TConfidenceValue>
::~MachineLearningModel()
{}

template <class TInputValue, class TOutputValue, class TConfidenceValue>
void
MachineLearningModel<TInputValue,TOutputValue,TConfidenceValue>
::SetRegressionMode(bool flag)
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
void
MachineLearningModel<TInputValue,TOutputValue,TConfidenceValue>
::PredictAll()
{
  typename TargetListSampleType::Pointer targets = this->GetTargetListSample();
  targets->Clear();

  typename TargetListSampleType::Pointer tmpTargets = this->PredictBatch(this->GetInputListSample());

  targets->Graft(tmpTargets);
}

template <class TInputValue, class TOutputValue, class TConfidenceValue>
typename MachineLearningModel<TInputValue,TOutputValue,TConfidenceValue>
::TargetSampleType
MachineLearningModel<TInputValue,TOutputValue,TConfidenceValue>
::Predict(const InputSampleType& input, ConfidenceValueType *quality) const
{
  // Call protected specialization entry point
  return this->DoPredict(input,quality);
}


template <class TInputValue, class TOutputValue, class TConfidenceValue>
typename MachineLearningModel<TInputValue,TOutputValue,TConfidenceValue>
::TargetListSampleType::Pointer
MachineLearningModel<TInputValue,TOutputValue,TConfidenceValue>
::PredictBatch(const InputListSampleType * input, ConfidenceListSampleType * quality) const
{  
  if(quality!=ITK_NULLPTR)
    {
    quality->Clear();
    }
  
  if(m_IsDoPredictBatchMultiThreaded)
    {
    // Simply calls DoPredictBatch
    return this->DoPredictBatch(input,quality);
    }
  else
    {
    
    #ifdef _OPENMP
    // OpenMP threading here
    
    std::vector<typename InputListSampleType::Pointer> batches;
    std::vector<typename TargetListSampleType::Pointer> target_batches;
    std::vector<typename ConfidenceListSampleType::Pointer> confidence_batches;
    
    typename TargetListSampleType::Pointer targets;
    
    targets = TargetListSampleType::New();
    targets->SetMeasurementVectorSize(1);

    unsigned int nb_threads(0), threadId(0), nb_batches(0);
    
#pragma omp parallel shared(nb_threads,nb_batches,batches,target_batches,confidence_batches) private(threadId)
    {
    nb_threads = omp_get_num_threads();
    threadId = omp_get_thread_num();
    nb_batches = std::min(nb_threads,(unsigned int)input->Size());
    unsigned int batch_size = ((unsigned int)input->Size()/nb_batches)+(threadId<input->Size()%nb_batches?1:0);
    
    if(threadId == 0)
      {
      batches.resize(nb_batches, ITK_NULLPTR);
      target_batches.resize(nb_batches,ITK_NULLPTR);
      confidence_batches.resize(nb_batches,ITK_NULLPTR);
      }
    #pragma omp barrier
    
    if(threadId<nb_batches)
      {
      batches[threadId] = InputListSampleType::New();
      batches[threadId]->SetMeasurementVectorSize(input->GetMeasurementVectorSize());
      if(quality != ITK_NULLPTR)
        {
        confidence_batches[threadId] = ConfidenceListSampleType::New();
        confidence_batches[threadId]->SetMeasurementVectorSize(1);
        }
      
    

    for(typename InputListSampleType::InstanceIdentifier id = threadId*batch_size;
        id<(threadId*batch_size)+batch_size;++id)
      {
      // Assign a proxy VariableLengthVector to avoid deep copy
      //batches[id%nb_batches]->PushBack(InputSampleType(input->GetMeasurementVector(id).GetDataPointer(),input->GetMeasurementVector(id).Size()));
      batches[threadId]->PushBack(input->GetMeasurementVector(id));
      }

    
      #pragma omp critical
      target_batches[threadId] = this->DoPredictBatch(batches[threadId],confidence_batches[threadId]);
      }
    }

    
    for(unsigned int batch_id = 0; batch_id < nb_batches;++batch_id)
      {
      typename InputSampleType::ElementIdentifier id = 0;
      for(typename TargetListSampleType::Iterator it = target_batches[batch_id]->Begin();
          it != target_batches[batch_id]->End();++it,++id)
        {
        targets->PushBack(it.GetMeasurementVector());

        if(quality!=ITK_NULLPTR && confidence_batches[batch_id].IsNotNull())
          {
          quality->PushBack(confidence_batches[batch_id]->GetMeasurementVector(id));
          }
        }
      }
    return targets;
    }
    #else
    return this->DoPredictBatch(input,quality);
    #endif
}



template <class TInputValue, class TOutputValue, class TConfidenceValue>
typename MachineLearningModel<TInputValue,TOutputValue,TConfidenceValue>
::TargetListSampleType::Pointer
MachineLearningModel<TInputValue,TOutputValue,TConfidenceValue>
::DoPredictBatch(const InputListSampleType * input, ConfidenceListSampleType * quality) const
{
  typename TargetListSampleType::Pointer targets = TargetListSampleType::New();
  targets->SetMeasurementVectorSize(1);
  
  if(quality != ITK_NULLPTR)
    {
    quality->Clear();
    quality->SetMeasurementVectorSize(1);
    }

  for(typename InputListSampleType::ConstIterator it = input->Begin();
      it!=input->End();++it)
    {
    TargetSampleType target;
    ConfidenceValueType confidence;

    if(quality!=ITK_NULLPTR)
      {
      target = this->DoPredict(it.GetMeasurementVector(),&confidence);
      quality->PushBack(confidence);
      }
    else
      {
      target = this->DoPredict(it.GetMeasurementVector());
      }
    targets->PushBack(target);
    }
  
  return targets;
}

template <class TInputValue, class TOutputValue, class TConfidenceValue>
void
MachineLearningModel<TInputValue,TOutputValue,TConfidenceValue>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  // Call superclass implementation
  Superclass::PrintSelf(os,indent);
}
}

#endif
