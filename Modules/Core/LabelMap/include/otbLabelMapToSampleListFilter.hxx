/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbLabelMapToSampleListFilter_hxx
#define otbLabelMapToSampleListFilter_hxx

#include "otbLabelMapToSampleListFilter.h"

namespace otb
{

template <class TInputLabelMap, class TOutputListSample, class TMeasurementFunctor>
LabelMapToSampleListFilter<TInputLabelMap, TOutputListSample, TMeasurementFunctor>::LabelMapToSampleListFilter()
{
  this->SetNumberOfRequiredInputs(1);
  this->SetNumberOfRequiredOutputs(1);

  this->itk::ProcessObject::SetNthOutput(0, this->MakeOutput(0).GetPointer());
}

template <class TInputLabelMap, class TOutputListSample, class TMeasurementFunctor>
LabelMapToSampleListFilter<TInputLabelMap, TOutputListSample, TMeasurementFunctor>::~LabelMapToSampleListFilter()
{
}

template <class TInputLabelMap, class TOutputListSample, class TMeasurementFunctor>
typename LabelMapToSampleListFilter<TInputLabelMap, TOutputListSample, TMeasurementFunctor>::DataObjectPointerType
    LabelMapToSampleListFilter<TInputLabelMap, TOutputListSample, TMeasurementFunctor>::MakeOutput(DataObjectPointerArraySizeType)
{
  DataObjectPointerType output;
  output = static_cast<itk::DataObject*>(OutputSampleListType::New().GetPointer());
  return output;
}

// Set the input labelMap
template <class TInputLabelMap, class TOutputListSample, class TMeasurementFunctor>
void LabelMapToSampleListFilter<TInputLabelMap, TOutputListSample, TMeasurementFunctor>::SetInputLabelMap(const InputLabelMapType* inputLabelMap)
{
  // Process object is not const-correct so the const_cast is required here
  this->itk::ProcessObject::SetNthInput(0, const_cast<InputLabelMapType*>(inputLabelMap));
}

// Get the input labelMap
template <class TInputLabelMap, class TOutputListSample, class TMeasurementFunctor>
const typename LabelMapToSampleListFilter<TInputLabelMap, TOutputListSample, TMeasurementFunctor>::InputLabelMapType*
LabelMapToSampleListFilter<TInputLabelMap, TOutputListSample, TMeasurementFunctor>::GetInputLabelMap() const
{
  if (this->GetNumberOfInputs() < 1)
  {
    return nullptr;
  }

  return static_cast<const InputLabelMapType*>(this->itk::ProcessObject::GetInput(0));
}

// Get the output  SampleList
template <class TInputLabelMap, class TOutputListSample, class TMeasurementFunctor>
const typename LabelMapToSampleListFilter<TInputLabelMap, TOutputListSample, TMeasurementFunctor>::OutputSampleListType*
LabelMapToSampleListFilter<TInputLabelMap, TOutputListSample, TMeasurementFunctor>::GetOutputSampleList()
{
  return dynamic_cast<OutputSampleListType*>(this->itk::ProcessObject::GetOutput(0));
}


template <class TInputLabelMap, class TOutputListSample, class TMeasurementFunctor>
void LabelMapToSampleListFilter<TInputLabelMap, TOutputListSample, TMeasurementFunctor>::GenerateData()
{
  // Get the input LabelMap
  InputLabelMapConstPointerType inputLabelMap = this->GetInputLabelMap();

  // Get the output ListSample
  OutputSampleListPointerType outputSampleList = const_cast<OutputSampleListType*>(this->GetOutputSampleList());

  ConstIteratorType it = ConstIteratorType(inputLabelMap);

  // iterate on label objects
  bool isFirstIteration = true;
  while (!it.IsAtEnd())
  {
    // Get the measurement vector size of the output samplelist : once
    // in the beginning of the iterator
    if (isFirstIteration)
    {
      typename OutputSampleListType::MeasurementVectorSizeType measurementSize;
      measurementSize = m_MeasurementFunctor(it.GetLabelObject()).Size();
      outputSampleList->SetMeasurementVectorSize(measurementSize);
      isFirstIteration = false;
    }
    outputSampleList->PushBack(m_MeasurementFunctor(it.GetLabelObject()));
    ++it;
  }
}

template <class TInputLabelMap, class TOutputListSample, class TMeasurementFunctor>
void LabelMapToSampleListFilter<TInputLabelMap, TOutputListSample, TMeasurementFunctor>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb

#endif
