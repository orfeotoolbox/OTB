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

#include "otbMachineLearningModel.h"

namespace otb
{

template <class TInputValue, class TOutputValue, class TConfidenceValue>
MachineLearningModel<TInputValue,TOutputValue,TConfidenceValue>
::MachineLearningModel() :
  m_RegressionMode(false),
  m_IsRegressionSupported(false),
  m_ConfidenceIndex(false)
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
  ls->PushBack(input);

  ConfidenceValueVectorType vquality;

  typename TargetListSampleType::Pointer ts = PredictBatch(ls,&vquality);

  if(vquality.Size()!=0)
    {
    *quality = vquality[0];
    }

  assert(ts->Size()==1&&"Measurement vector should have 1 element exactly");
  
  return ts->GetMeasurementVector(0)[0];
}


template <class TInputValue, class TOutputValue, class TConfidenceValue>
typename MachineLearningModel<TInputValue,TOutputValue,TConfidenceValue>
::TargetListSampleType * 
MachineLearningModel<TInputValue,TOutputValue,TConfidenceValue>
::PredictBatch(const InputListSampleType * input, ConfidenceValueVectorType * quality) const
{
  if(m_IsDoPredictBatchMultiThreaded)
    {
    // Simply calls DoPredictBatch
    return this->DoPredictBatch(input,quality);
    }
  else
    {
    // OpenMP threading here
    return this->DoPredictBatch(input,quality);
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
