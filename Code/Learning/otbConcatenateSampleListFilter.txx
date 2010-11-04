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
 {
	this->SetNumberOfRequiredInputs(1);
	this->SetNumberOfRequiredOutputs(1);

	// Generate the output sample list
	typename OutputSampleListObjectType::Pointer outputPtr =
			static_cast< OutputSampleListObjectType * >(this->MakeOutput(0).GetPointer());
	this->ProcessObject::SetNthOutput(0, outputPtr.GetPointer());
 }

template < class TInputSampleList, class TOutputSampleList >
typename ConcatenateSampleListFilter<TInputSampleList,TOutputSampleList>
::DataObjectPointer
 ConcatenateSampleListFilter<TInputSampleList,TOutputSampleList>
::MakeOutput(unsigned int itkNotUsed(idx))
 {
	typename OutputSampleListObjectType::Pointer outputPtr = OutputSampleListObjectType::New();
	OutputSampleListPointer outputSampleList = OutputSampleListType::New();
	outputPtr->Set(outputSampleList);
	return static_cast<DataObjectPointer>(outputPtr);
 }

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
	this->ProcessObject::AddInput(const_cast< InputSampleListObjectType* >( inputPtr ) );
 }

template < class TInputSampleList, class TOutputSampleList >
typename ConcatenateSampleListFilter<TInputSampleList,TOutputSampleList>
::OutputSampleListType *
ConcatenateSampleListFilter<TInputSampleList,TOutputSampleList>
::GetOutputSampleList()
 {
	typename OutputSampleListObjectType::Pointer dataObjectPointer = static_cast<OutputSampleListObjectType * >
	(this->ProcessObject::GetOutput(0) );
	return const_cast<OutputSampleListType *>(dataObjectPointer->Get());
 }

template < class TInputSampleList, class TOutputSampleList >
typename ConcatenateSampleListFilter<TInputSampleList,TOutputSampleList>
::OutputSampleListObjectType *
ConcatenateSampleListFilter<TInputSampleList,TOutputSampleList>
::GetOutput()
 {
	return static_cast<OutputSampleListObjectType * >
		(this->ProcessObject::GetOutput(0) );
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
				static_cast<InputSampleListObjectType *>(this->GetInput(inputIndex));
		totalNumberOfSamples+=inputPtr->Get()->Size();
		}

	// Set-up progress reporting
	itk::ProgressReporter progress(this,0,totalNumberOfSamples);

	for(unsigned int inputIndex = 0; inputIndex<this->GetNumberOfInputs();++inputIndex)
	{
		// Retrieve the ListSample
		typename InputSampleListObjectType::ConstPointer inputPtr =
				static_cast<InputSampleListObjectType *>(this->GetInput(inputIndex));
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
