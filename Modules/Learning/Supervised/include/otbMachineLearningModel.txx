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
#ifndef __otbMachineLearningModel_txx
#define __otbMachineLearningModel_txx

#include "otbMachineLearningModel.h"

namespace otb
{

template <class TInputValue, class TOutputValue, class TConfidenceValue>
MachineLearningModel<TInputValue,TOutputValue,TConfidenceValue>
::MachineLearningModel() : m_RegressionMode(false)
{}


template <class TInputValue, class TOutputValue, class TConfidenceValue>
MachineLearningModel<TInputValue,TOutputValue,TConfidenceValue>
::~MachineLearningModel()
{}

template <class TInputValue, class TOutputValue, class TConfidenceValue>
void
MachineLearningModel<TInputValue,TOutputValue,TConfidenceValue>
::Train()
{
  if(m_RegressionMode)
    return this->TrainRegression();
  else
    return this->TrainClassification();
}

template <class TInputValue, class TOutputValue, class TConfidenceValue>
typename MachineLearningModel<TInputValue,TOutputValue,TConfidenceValue>::TargetSampleType
MachineLearningModel<TInputValue,TOutputValue,TConfidenceValue>
::Predict(const InputSampleType& input, ConfidenceValueType *quality) const
{
  if(m_RegressionMode)
    return this->PredictRegression(input);
  else
    return this->PredictClassification(input,quality);
}

template <class TInputValue, class TOutputValue, class TConfidenceValue>
void
MachineLearningModel<TInputValue,TOutputValue,TConfidenceValue>
::PredictAll()
{
  TargetListSampleType * targets = this->GetTargetListSample();
  targets->Clear();

  for(typename InputListSampleType::ConstIterator sIt = this->GetInputListSample()->Begin();
      sIt!=this->GetInputListSample()->End(); ++sIt)
    {
    targets->PushBack(this->Predict(sIt.GetMeasurementVector()));
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
