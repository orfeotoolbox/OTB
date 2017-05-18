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
#ifndef DimensionalityReductionModel_txx
#define DimensionalityReductionModel_txx

#ifdef _OPENMP
 # include <omp.h>
#endif

#include "DimensionalityReductionModel.h"

#include "itkMultiThreader.h"

namespace otb
{

template <class TInputValue, class TOutputValue>
DimensionalityReductionModel<TInputValue,TOutputValue>
::DimensionalityReductionModel()
{ this->m_IsDoPredictBatchMultiThreaded=false;}


template <class TInputValue, class TOutputValue>
DimensionalityReductionModel<TInputValue,TOutputValue>
::~DimensionalityReductionModel()
{}



template <class TInputValue, class TOutputValue>
typename DimensionalityReductionModel<TInputValue,TOutputValue>
::TargetSampleType
DimensionalityReductionModel<TInputValue,TOutputValue>
::Predict(const InputSampleType& input) const
{
  // Call protected specialization entry point
  return this->DoPredict(input);
}


template <class TInputValue, class TOutputValue>
typename DimensionalityReductionModel<TInputValue,TOutputValue>
::TargetListSampleType::Pointer
DimensionalityReductionModel<TInputValue,TOutputValue>
::PredictBatch(const InputListSampleType * input) const
{
  typename TargetListSampleType::Pointer targets = TargetListSampleType::New();
  targets->Resize(input->Size());
  
  
  if(m_IsDoPredictBatchMultiThreaded)
    {
    // Simply calls DoPredictBatch
    this->DoPredictBatch(input,0,input->Size(),targets);
    return targets;
    }
  else
    {
    
    #ifdef _OPENMP
    // OpenMP threading here
    unsigned int nb_threads(0), threadId(0), nb_batches(0);
    
    #pragma omp parallel shared(nb_threads,nb_batches) private(threadId)
    {
    // Get number of threads configured with ITK
    omp_set_num_threads(itk::MultiThreader::GetGlobalDefaultNumberOfThreads());
    nb_threads = omp_get_num_threads();
    threadId = omp_get_thread_num();
    nb_batches = std::min(nb_threads,(unsigned int)input->Size());
    // Ensure that we do not spawn unncessary threads
    if(threadId<nb_batches)
      {
      unsigned int batch_size = ((unsigned int)input->Size()/nb_batches);
      unsigned int batch_start = threadId*batch_size;
      if(threadId == nb_threads-1)
        {
        batch_size+=input->Size()%nb_batches;
        }
    
      this->DoPredictBatch(input,batch_start,batch_size,targets);
      }
    }
    #else
    this->DoPredictBatch(input,0,input->Size(),targets);
    #endif
    return targets;
    }
}



template <class TInputValue, class TOutputValue>
void
DimensionalityReductionModel<TInputValue,TOutputValue>
::DoPredictBatch(const InputListSampleType * input, const unsigned int & startIndex, const unsigned int & size, TargetListSampleType * targets) const
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
    const TargetSampleType target = this->DoPredict(input->GetMeasurementVector(id));
    targets->SetMeasurementVector(id,target);
    }
    
}

template <class TInputValue, class TOutputValue>
void
DimensionalityReductionModel<TInputValue,TOutputValue>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  // Call superclass implementation
  Superclass::PrintSelf(os,indent);
}
}

#endif
