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
#ifndef __otbLabelMapToSampleListFilter_txx
#define __otbLabelMapToSampleListFilter_txx

#include "otbLabelMapToSampleListFilter.h"

namespace otb
{

template <class TInputLabelMap, class TOutputListSample, class TMeasurementFunctor>
LabelMapToSampleListFilter<TInputLabelMap, TOutputListSample, TMeasurementFunctor>
::LabelMapToSampleListFilter()
{
  this->SetNumberOfRequiredInputs(1);
  this->SetNumberOfRequiredOutputs(1);

  this->itk::ProcessObject::SetNthOutput(0, this->MakeOutput(0).GetPointer());
}

template <class TInputLabelMap, class TOutputListSample, class TMeasurementFunctor>
LabelMapToSampleListFilter<TInputLabelMap, TOutputListSample, TMeasurementFunctor>
::~LabelMapToSampleListFilter()
{
}

template <class TInputLabelMap, class TOutputListSample, class TMeasurementFunctor>
typename LabelMapToSampleListFilter<TInputLabelMap,TOutputListSample,TMeasurementFunctor>
::DataObjectPointerType
LabelMapToSampleListFilter<TInputLabelMap,TOutputListSample,TMeasurementFunctor>
::MakeOutput(unsigned int idx)
{
  DataObjectPointerType output;
  output = static_cast<itk::DataObject*>(OutputSampleListType::New().GetPointer());
  return output;
}

// Set the input labelMap
template <class TInputLabelMap, class TOutputListSample, class TMeasurementFunctor>
void
LabelMapToSampleListFilter<TInputLabelMap, TOutputListSample, TMeasurementFunctor>
::SetInputLabelMap(const InputLabelMapType * inputLabelMap )
{
  // Process object is not const-correct so the const_cast is required here
  this->itk::ProcessObject::SetNthInput(0,
                                   const_cast<InputLabelMapType*>(inputLabelMap));
}

//Get the input labelMap
template <class TInputLabelMap, class TOutputListSample, class TMeasurementFunctor>
const typename LabelMapToSampleListFilter<TInputLabelMap,TOutputListSample,TMeasurementFunctor>
::InputLabelMapType *
LabelMapToSampleListFilter<TInputLabelMap,TOutputListSample,TMeasurementFunctor>
::GetInputLabelMap() const
{
  if (this->GetNumberOfInputs() < 1)
    {
    return 0;
    }

  return static_cast<const InputLabelMapType* >
    (this->itk::ProcessObject::GetInput(0) );
}

// Get the output  SampleList
template <class TInputLabelMap, class TOutputListSample, class TMeasurementFunctor>
const typename LabelMapToSampleListFilter<TInputLabelMap,TOutputListSample,TMeasurementFunctor>
::OutputSampleListType *
LabelMapToSampleListFilter<TInputLabelMap,TOutputListSample,TMeasurementFunctor>
::GetOutputSampleList()
{
  return dynamic_cast<OutputSampleListType*>(this->itk::ProcessObject::GetOutput(0));
}


template <class TInputLabelMap, class TOutputListSample, class TMeasurementFunctor>
void
LabelMapToSampleListFilter<TInputLabelMap,TOutputListSample,TMeasurementFunctor>
::GenerateData()
{
  // Get the input LabelMap
  InputLabelMapConstPointerType inputLabelMap = this->GetInputLabelMap();

  // Get the output ListSample
  OutputSampleListPointerType outputSampleList = const_cast<OutputSampleListType*>(this->GetOutputSampleList());

  ConstIteratorType it = ConstIteratorType( inputLabelMap );

  // iterate on label objects
  bool isFirstIteration = true;
  while( !it.IsAtEnd() )
    {
    // Get the measurement vector size of the output samplelist : once
    // in the begining of the iterator
    if( isFirstIteration )
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
void
LabelMapToSampleListFilter<TInputLabelMap, TOutputListSample, TMeasurementFunctor>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb

#endif
