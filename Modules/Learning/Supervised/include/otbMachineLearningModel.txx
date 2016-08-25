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
  typename InputListSampleType::Pointer ls = InputListSampleType::New();
  ls->SetMeasurementVectorSize(input.Size());
  ls->PushBack(input);

  typename ConfidenceListSampleType::Pointer vquality = ITK_NULLPTR;

  if(quality != ITK_NULLPTR)
    {
    vquality = ConfidenceListSampleType::New();
    vquality->SetMeasurementVectorSize(1);
    }
  
  typename TargetListSampleType::Pointer ts = PredictBatch(ls,vquality);

  if(vquality.IsNotNull() && quality != ITK_NULLPTR && vquality->Size() != 0)
    {
    *quality = vquality->GetMeasurementVector(0)[0];
    }
  assert(ts->Size()==1&&"Measurement vector should have 1 element exactly");
  
  return ts->GetMeasurementVector(0)[0];
}


template <class TInputValue, class TOutputValue, class TConfidenceValue>
typename MachineLearningModel<TInputValue,TOutputValue,TConfidenceValue>
::TargetListSampleType::Pointer
MachineLearningModel<TInputValue,TOutputValue,TConfidenceValue>
::PredictBatch(const InputListSampleType * input, ConfidenceListSampleType * quality) const
{  
  if(m_IsDoPredictBatchMultiThreaded)
    {
    // Simply calls DoPredictBatch
    return this->DoPredictBatch(input,quality);
    }
  else
    {
    
    #ifdef _OPENMP
    // OpenMP threading here

    typename TargetListSampleType::Pointer targets = TargetListSampleType::New();
    targets->SetMeasurementVectorSize(1);
    
    #pragma omp parallel
    {
    unsigned int nb_threads = omp_get_num_threads();
    unsigned int nb_batches = vcl_ceil((double)input->Size()/(double)nb_threads);

    std::vector<typename InputListSampleType::Pointer> batches(nb_batches,InputListSampleType::New());
    std::vector<typename TargetListSampleType::Pointer> target_batches(nb_batches);
    std::vector<typename ConfidenceListSampleType::Pointer> confidence_batches(nb_batches,quality!=ITK_NULLPTR?ConfidenceListSampleType::New():ITK_NULLPTR);

    if(quality!=ITK_NULLPTR)
      {
      quality->Clear();
      }

    #pragma omp for    
    for(unsigned int batch_id = 0; batch_id < nb_batches;++batch_id)
      {
      batches[batch_id]->SetMeasurementVectorSize(input->GetMeasurementVectorSize());
      if(confidence_batches[batch_id].IsNotNull())
        {
        confidence_batches[batch_id]->SetMeasurementVectorSize(1);
        }
      }
    
    #pragma omp for
    for(typename InputListSampleType::InstanceIdentifier id = 0;
        id<input->Size();++id)
      {
      // Assign a proxy VariableLengthVector to avoid deep copy
      // batches[id%nb_batches]->PushBack(InputSampleType(input->GetMeasurementVector(id).GetDataPointer(),input->GetMeasurementVector(id).Size()));
      batches[id%nb_batches]->PushBack(input->GetMeasurementVector(id));
      }
    
    #pragma omp for    
    for(unsigned int batch_id = 0; batch_id < nb_batches;++batch_id)
      {
      // TODO handle quality layer here
      target_batches[batch_id] = this->DoPredictBatch(batches[batch_id],confidence_batches[batch_id]);
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
    }
    return targets;
    #else
    return this->DoPredictBatch(input,quality);
    #endif
    }
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
