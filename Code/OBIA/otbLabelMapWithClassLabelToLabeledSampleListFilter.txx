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
#include "itkDataObject.h"

namespace otb
{

template <class TInputLabelMap, class TOutputListSample, class TOutputTrainingListSample, class TMeasurementFunctor>
LabelMapWithClassLabelToLabeledSampleListFilter<TInputLabelMap, TOutputListSample, TOutputTrainingListSample, TMeasurementFunctor>
::LabelMapWithClassLabelToLabeledSampleListFilter()
{
  this->SetNumberOfRequiredOutputs(2);
  this->itk::ProcessObject::SetNthOutput(1, this->MakeOutput(1).GetPointer());
}

template <class TInputLabelMap, class TOutputListSample, class TOutputTrainingListSample, class TMeasurementFunctor>
LabelMapWithClassLabelToLabeledSampleListFilter<TInputLabelMap, TOutputListSample, TOutputTrainingListSample, TMeasurementFunctor>
::~LabelMapWithClassLabelToLabeledSampleListFilter()
{}


template <class TInputLabelMap, class TOutputListSample, class TOutputTrainingListSample, class TMeasurementFunctor>
typename LabelMapWithClassLabelToLabeledSampleListFilter<TInputLabelMap,TOutputListSample,TOutputTrainingListSample,TMeasurementFunctor>
::DataObjectPointerType
LabelMapWithClassLabelToLabeledSampleListFilter<TInputLabelMap,TOutputListSample,TOutputTrainingListSample,TMeasurementFunctor>
::MakeOutput(unsigned int idx)
{
  DataObjectPointerType output;
  
  switch (idx)
    {
    case 0:
      output = static_cast<itk::DataObject*>(OutputSampleListType::New().GetPointer());
      break;
    case 1:
      output = static_cast<itk::DataObject*>(OutputTrainingSampleListType::New().GetPointer());
      break;
    default:
      output = static_cast<itk::DataObject*>(OutputSampleListType::New().GetPointer());
      break;
    }
  
  return output;
}

// Get the output training sample list
template <class TInputLabelMap, class TOutputListSample, class TOutputTrainingListSample, class TMeasurementFunctor>
const typename LabelMapWithClassLabelToLabeledSampleListFilter<TInputLabelMap,TOutputListSample,TOutputTrainingListSample,TMeasurementFunctor>
::OutputTrainingSampleListType*
LabelMapWithClassLabelToLabeledSampleListFilter<TInputLabelMap,TOutputListSample,TOutputTrainingListSample,TMeasurementFunctor>
::GetOutputTrainingSampleList()
{
  return dynamic_cast<OutputTrainingSampleListType*>(this->itk::ProcessObject::GetOutput(1));
}

template <class TInputLabelMap, class TOutputListSample, class TOutputTrainingListSample, class TMeasurementFunctor>
void
LabelMapWithClassLabelToLabeledSampleListFilter<TInputLabelMap, TOutputListSample, TOutputTrainingListSample, TMeasurementFunctor>
::GenerateData()
{
  // Get input
  InputLabelMapConstPointerType inputLabelMap = this->GetInputLabelMap();
  
  // Get outputs
  OutputSampleListPointerType outputSampleList = const_cast<OutputSampleListType*>
    (this->GetOutputSampleList());
  
  OutputTrainingSampleListPointerType outputTrainingSampleList = const_cast<OutputTrainingSampleListType*>
    (this->GetOutputTrainingSampleList());

  // Clear  the ListSamples
  outputSampleList->Clear();
  outputTrainingSampleList->Clear();
  
  // Lets begin by declaring the iterator for the objects in the image.
  ConstIteratorType it = ConstIteratorType( inputLabelMap );

  bool isFirstIteration = true;
  // iterate on label objects
  while( !it.IsAtEnd() )
    {
    TraningVectorType label;
    if(it.GetLabelObject()->HasClassLabel())
      {
      label[0]=static_cast<typename TraningVectorType::ValueType>(it.GetLabelObject()->GetClassLabel());
      }
    else
      {
      label[0]=itk::NumericTraits<typename InputLabelMapType::LabelObjectType::ClassLabelType>::max();
      }
    // Before pushin back the measurement vectors to the sampleList
    // initialize their sizes : once
    if( isFirstIteration )
      {
      // initialize the output sampleList
      typename OutputSampleListType::MeasurementVectorSizeType measurementSize;
      measurementSize = m_MeasurementFunctor(it.GetLabelObject()).Size();
      outputSampleList->SetMeasurementVectorSize(measurementSize);
      
      // initialize the training samplelist
      outputTrainingSampleList->SetMeasurementVectorSize(label.Size());
      isFirstIteration = false;
      }
    
    outputSampleList->PushBack(m_MeasurementFunctor(it.GetLabelObject()));
    outputTrainingSampleList->PushBack(label);
    ++it;
    }
}

template <class TInputLabelMap, class TOutputListSample, class TOutputTrainingListSample, class TMeasurementFunctor>
void
LabelMapWithClassLabelToLabeledSampleListFilter<TInputLabelMap, TOutputListSample, TOutputTrainingListSample, TMeasurementFunctor>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb

#endif
