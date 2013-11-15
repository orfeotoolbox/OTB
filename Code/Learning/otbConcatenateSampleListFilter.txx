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
::AddInput( const SampleListType * inputPtr )
 {
 // Process object is not const-correct so the const_cast is required here
 Superclass::ProcessObject::AddInput(const_cast< SampleListType* >( inputPtr ) );
 }


template <class TSampleList>
void
ConcatenateSampleListFilter<TSampleList>
::GenerateData()
 {
 // Retrieve output pointers
 SampleListPointer    outputSampleListPtr = this->GetOutput();

 // Clear any previous output
 outputSampleListPtr->Clear();

 // Set the measurement vector size (based on the first listsample)
 outputSampleListPtr->SetMeasurementVectorSize(static_cast<SampleListType *>(
                                                 Superclass::ProcessObject::GetInput(0))->GetMeasurementVectorSize());

 // Evaluate the total number of samples for progress reporting
 unsigned long totalNumberOfSamples = 0;

 for(unsigned int inputIndex = 0; inputIndex<this->GetNumberOfInputs(); ++inputIndex)
  {
  // Retrieve the ListSample
  typename SampleListType::ConstPointer inputPtr = static_cast<SampleListType *>
    (Superclass::ProcessObject::GetInput(inputIndex));

  totalNumberOfSamples += inputPtr->Size();
  }

 // Set-up progress reporting
 itk::ProgressReporter progress(this, 0, totalNumberOfSamples);

 for(unsigned int inputIndex = 0; inputIndex<this->GetNumberOfInputs(); ++inputIndex)
 {
  // Retrieve the ListSample
 typename SampleListType::ConstPointer inputSampleListPtr =  static_cast<SampleListType *>
   (Superclass::ProcessObject::GetInput(inputIndex));

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
