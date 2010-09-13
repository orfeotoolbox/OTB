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
#ifndef __otbLabelMapWithClassLabelToLabeledSampleListFilter_txx
#define __otbLabelMapWithClassLabelToLabeledSampleListFilter_txx

#include "otbLabelMapWithClassLabelToLabeledSampleListFilter.h"

namespace otb
{

template <class TInputLabelMap, class TOutputListSample, class TOutputTrainingListSample, class TMeasurementFunctor>
LabelMapWithClassLabelToLabeledSampleListFilter<TInputLabelMap,TOutputListSample,TOutputTrainingListSample,TMeasurementFunctor>
::LabelMapWithClassLabelToLabeledSampleListFilter()
{
  m_OutputSampleList = OutputSampleListType::New();
  m_OutputTrainingSampleList = OutputTrainingSampleListType::New();
}

template <class TInputLabelMap, class TOutputListSample, class TOutputTrainingListSample, class TMeasurementFunctor>
LabelMapWithClassLabelToLabeledSampleListFilter<TInputLabelMap,TOutputListSample,TOutputTrainingListSample,TMeasurementFunctor>
::~LabelMapWithClassLabelToLabeledSampleListFilter()
{}

template <class TInputLabelMap, class TOutputListSample, class TOutputTrainingListSample, class TMeasurementFunctor>
void 
LabelMapWithClassLabelToLabeledSampleListFilter<TInputLabelMap,TOutputListSample,TOutputTrainingListSample,TMeasurementFunctor>
::Update()
{
  // Clear output sample list
  m_OutputSampleList->Clear();
  m_OutputTrainingSampleList->Clear();
  
  typename InputLabelMapType::LabelObjectContainerType::const_iterator it 
    = m_InputLabelMap->GetLabelObjectContainer().begin();

  // iterate on label objects
  while(it != m_InputLabelMap->GetLabelObjectContainer().end())
    {
    TraningVectorType label;
    if(it->second->HasClassLabel())
      {
      label[0]=static_cast<typename TraningVectorType::ValueType>(it->second->GetClassLabel());
      }
    else
      {
      label[0]=itk::NumericTraits<typename InputLabelMapType::LabelObjectType::ClassLabelType>::max();
      }
    m_OutputSampleList->PushBack(m_MeasurementFunctor(it->second));
    m_OutputTrainingSampleList->PushBack(label);
    ++it;
    }
}

template <class TInputLabelMap, class TOutputListSample, class TOutputTrainingListSample, class TMeasurementFunctor>
void 
LabelMapWithClassLabelToLabeledSampleListFilter<TInputLabelMap,TOutputListSample,TOutputTrainingListSample,TMeasurementFunctor>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}

} // end namespace otb

#endif
