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

template < class TSampleList>
ConcatenateSampleListFilter<TSampleList>
::ConcatenateSampleListFilter()
 {}

template <class TSampleList>
void
ConcatenateSampleListFilter<TSampleList>
::AddInput( const SampleListType * input )
 {
 typename SampleListObjectType::Pointer inputPtr = SampleListObjectType::New();
 inputPtr->Set(input);
 this->AddInput(inputPtr);
 }

template <class TSampleList>
void
ConcatenateSampleListFilter<TSampleList>
::AddInput( const SampleListObjectType * inputPtr )
 {
 // Process object is not const-correct so the const_cast is required here
 Superclass::ProcessObject::AddInput(const_cast< SampleListObjectType* >( inputPtr ) );
 }


template <class TSampleList>
void
ConcatenateSampleListFilter<TSampleList>
::GenerateData()
 {
 // Retrieve output pointers
 typename SampleListObjectType::Pointer     outputPtr = this->GetOutput();
 SampleListPointer outputSampleListPtr    = const_cast<SampleListType *>(outputPtr->Get());

 // Clear any previous output
 outputSampleListPtr->Clear();

 // Evaluate the total number of samples for progress reporting
 unsigned long totalNumberOfSamples = 0;

 for(unsigned int inputIndex = 0; inputIndex<this->GetNumberOfInputs(); ++inputIndex)
  {
  // Retrieve the ListSample
  typename SampleListObjectType::ConstPointer inputPtr =
    static_cast<SampleListObjectType *>(Superclass::ProcessObject::GetInput(inputIndex));
  totalNumberOfSamples += inputPtr->Get()->Size();
  }

 // Set-up progress reporting
 itk::ProgressReporter progress(this, 0, totalNumberOfSamples);

 for(unsigned int inputIndex = 0; inputIndex<this->GetNumberOfInputs(); ++inputIndex)
 {
  // Retrieve the ListSample
  typename SampleListObjectType::ConstPointer inputPtr =
    static_cast<SampleListObjectType *>(Superclass::ProcessObject::GetInput(inputIndex));
  SampleListConstPointer inputSampleListPtr = inputPtr->Get();

  typename SampleListType::ConstIterator inputIt = inputSampleListPtr->Begin();

  // Iterate on the InputSampleList
  while(inputIt != inputSampleListPtr->End())
  {
  // Add the current output sample to the output SampleList
  outputSampleListPtr->PushBack(inputIt.GetMeasurementVector());

  // Update progress
  progress.CompletedPixel();

  ++inputIt;
  }
 }
 }

template <class TSampleList>
void
ConcatenateSampleListFilter<TSampleList>
::PrintSelf(std::ostream& os, itk::Indent indent) const
 {
 // Call superclass implementation
 Superclass::PrintSelf(os, indent);
 os<<indent<<"Number of input SampleList: "<<this->GetNumberOfInputs();
 }


} // End namespace Statistics
} // End namespace otb

#endif
