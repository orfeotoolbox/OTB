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
  typename TargetListSampleType::Pointer targets = TargetListSampleType::New();
  targets->Resize(input->Size());
  
  if(quality!=ITK_NULLPTR)
    {
    quality->Clear();
    }
  
  if(m_IsDoPredictBatchMultiThreaded)
    {
    // Simply calls DoPredictBatch
    this->DoPredictBatch(input,0,input->Size(),targets,quality);
    return targets;
    }
  else
    {
    
    #ifdef _OPENMP
    // OpenMP threading here
    unsigned int nb_threads(0), threadId(0), nb_batches(0);
    
    #pragma omp parallel shared(nb_threads,nb_batches) private(threadId)
    {
    nb_threads = omp_get_num_threads();
    threadId = omp_get_thread_num();
    nb_batches = std::min(nb_threads,(unsigned int)input->Size());
    unsigned int batch_size = ((unsigned int)input->Size()/nb_batches);
    unsigned int batch_start = threadId*batch_size;
    if(threadId == nb_threads-1)
      {
      batch_size+=input->Size()%nb_batches;
      }
    
    this->DoPredictBatch(input,batch_start,batch_size,targets,quality);
    }
    #else
    this->DoPredictBatch(input,0,input->Size(),targets,quality);
    #endif
    return targets;
    }
}



template <class TInputValue, class TOutputValue, class TConfidenceValue>
void
MachineLearningModel<TInputValue,TOutputValue,TConfidenceValue>
::DoPredictBatch(const InputListSampleType * input, const unsigned int & startIndex, const unsigned int & size, TargetListSampleType * targets, ConfidenceListSampleType * quality) const
{
  assert(input != ITK_NULLPTR);
  assert(targets != ITK_NULLPTR);
  
  assert(input->Size()==targets->Size()&&"Input sample list and target label list do not have the same size.");
  assert(((quality==ITK_NULLPTR)||(quality->Size()==input->Size()))&&"Quality samples list is not null and does not have the same size as input samples list");

  if(startIndex+size>input->Size())
    {
    itkExceptionMacro(<<"requested range ["<<startIndex<<", "<<startIndex+size<<"[ partially outside input sample list range.[0,"<<input->Size()<<"[");
    }

  for(unsigned int id = startIndex;id<startIndex+size;++id)
    {
    TargetSampleType target;
    ConfidenceValueType confidence;

    if(quality!=ITK_NULLPTR)
      {
      target = this->DoPredict(input->GetMeasurementVector(id),&confidence);
      quality->SetMeasurementVector(id,confidence);
      }
    else
      {
      target = this->DoPredict(input->GetMeasurementVector(id));
      }
    targets->SetMeasurementVector(id,target);
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
