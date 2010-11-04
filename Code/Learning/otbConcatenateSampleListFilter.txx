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
#ifndef __otbConcatenateSampleListFilter_txx
#define __otbConcatenateSampleListFilter_txx

#include "otbConcatenateSampleListFilter.h"
#include "itkProgressReporter.h"

namespace otb {
namespace Statistics {

template < class TInputSampleList, class TOutputSampleList >
ConcatenateSampleListFilter<TInputSampleList,TOutputSampleList>
::ConcatenateSampleListFilter()
 {}

template < class TInputSampleList, class TOutputSampleList >
void
ConcatenateSampleListFilter<TInputSampleList,TOutputSampleList>
::AddInput( const InputSampleListType * input )
 {
 typename InputSampleListObjectType::Pointer inputPtr = InputSampleListObjectType::New();
 inputPtr->Set(input);
 this->AddInput(inputPtr);
 }

template < class TInputSampleList, class TOutputSampleList >
void
ConcatenateSampleListFilter<TInputSampleList,TOutputSampleList>
::AddInput( const InputSampleListObjectType * inputPtr )
 {
 // Process object is not const-correct so the const_cast is required here
 Superclass::ProcessObject::AddInput(const_cast< InputSampleListObjectType* >( inputPtr ) );
 }


template < class TInputSampleList, class TOutputSampleList >
void
ConcatenateSampleListFilter<TInputSampleList,TOutputSampleList>
::GenerateData()
 {
 // Retrieve output pointers
 typename OutputSampleListObjectType::Pointer     outputPtr = this->GetOutput();
 OutputSampleListPointer outputSampleListPtr    = const_cast<OutputSampleListType *>(outputPtr->Get());

 // Clear any previous output
 outputSampleListPtr->Clear();

 // Evaluate the total number of samples for progress reporting
 unsigned long totalNumberOfSamples = 0;

 for(unsigned int inputIndex = 0; inputIndex<this->GetNumberOfInputs();++inputIndex)
  {
  // Retrieve the ListSample
  typename InputSampleListObjectType::ConstPointer inputPtr =
    static_cast<InputSampleListObjectType *>(Superclass::ProcessObject::GetInput(inputIndex));
  totalNumberOfSamples+=inputPtr->Get()->Size();
  }

 // Set-up progress reporting
 itk::ProgressReporter progress(this,0,totalNumberOfSamples);

 for(unsigned int inputIndex = 0; inputIndex<this->GetNumberOfInputs();++inputIndex)
 {
  // Retrieve the ListSample
  typename InputSampleListObjectType::ConstPointer inputPtr =
    static_cast<InputSampleListObjectType *>(Superclass::ProcessObject::GetInput(inputIndex));
  InputSampleListConstPointer inputSampleListPtr = inputPtr->Get();

  typename InputSampleListType::ConstIterator inputIt = inputSampleListPtr->Begin();

  // Iterate on the InputSampleList
  while(inputIt != inputSampleListPtr->End())
  {
  // Retrieve current input sample
  InputMeasurementVectorType currentInputMeasurement = inputIt.GetMeasurementVector();

  // Build current output sample
  OutputMeasurementVectorType currentOutputMeasurement;
  currentOutputMeasurement.SetSize(currentInputMeasurement.GetSize());

  // Center and reduce each component
  for(unsigned int idx = 0;idx < currentInputMeasurement.Size();++idx)
   {
   currentOutputMeasurement[idx] = static_cast<OutputValueType>(
   currentInputMeasurement[idx]);
   }

  // Add the current output sample to the output SampleList
  outputSampleListPtr->PushBack(currentOutputMeasurement);

  // Update progress
  progress.CompletedPixel();

  ++inputIt;
  }
 }
 }

template < class TInputSampleList, class TOutputSampleList >
void
ConcatenateSampleListFilter<TInputSampleList,TOutputSampleList>
::PrintSelf(std::ostream& os, itk::Indent indent) const
 {
 // Call superclass implementation
 Superclass::PrintSelf(os,indent);
 os<<indent<<"Number of input SampleList: "<<this->GetNumberOfInputs();
 }


} // End namespace Statistics
} // End namespace otb

#endif
