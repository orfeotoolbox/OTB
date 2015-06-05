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

template <class TInputValue, class TOutputValue>
MachineLearningModel<TInputValue,TOutputValue>
::MachineLearningModel() : m_RegressionMode(false)
{}


template <class TInputValue, class TOutputValue>
MachineLearningModel<TInputValue,TOutputValue>
::~MachineLearningModel()
{}

template <class TInputValue, class TOutputValue>
void
MachineLearningModel<TInputValue,TOutputValue>
::Train()
{
  if(m_RegressionMode)
    return this->TrainRegression();
  else
    return this->TrainClassification();
}

template <class TInputValue, class TOutputValue>
typename MachineLearningModel<TInputValue,TOutputValue>::TargetSampleType
MachineLearningModel<TInputValue,TOutputValue>
::Predict(const typename MachineLearningModel<TInputValue,TOutputValue>::InputSampleType& input) const
{
  if(m_RegressionMode)
    return this->PredictRegression(input);
  else
    return this->PredictClassification(input);
}

template <class TInputValue, class TOutputValue>
void
MachineLearningModel<TInputValue,TOutputValue>
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

template <class TInputValue, class TOutputValue>
void
MachineLearningModel<TInputValue,TOutputValue>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  // Call superclass implementation
  Superclass::PrintSelf(os,indent);
}
}

#endif
