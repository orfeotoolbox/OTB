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
#ifndef __otbShiftScaleSampleListFilter_txx
#define __otbShiftScaleSampleListFilter_txx

#include "otbShiftScaleSampleListFilter.h"
#include "itkProgressReporter.h"

namespace otb {
namespace Statistics {

template < class TInputSampleList, class TOutputSampleList >
ShiftScaleSampleListFilter<TInputSampleList,TOutputSampleList>
::ShiftScaleSampleListFilter()
 {
 this->SetNumberOfRequiredInputs(1);
 this->SetNumberOfRequiredOutputs(1);

 // Generate the output sample list
 typename OutputSampleListObjectType::Pointer outputPtr =
   static_cast< OutputSampleListObjectType * >(this->MakeOutput(0).GetPointer());
 this->ProcessObject::SetNthOutput(0, outputPtr.GetPointer());
 }

template < class TInputSampleList, class TOutputSampleList >
typename ShiftScaleSampleListFilter<TInputSampleList,TOutputSampleList>
::DataObjectPointer
 ShiftScaleSampleListFilter<TInputSampleList,TOutputSampleList>
::MakeOutput(unsigned int itkNotUsed(idx))
 {
 typename OutputSampleListObjectType::Pointer outputPtr = OutputSampleListObjectType::New();
 OutputSampleListPointer outputSampleList = OutputSampleListType::New();
 outputPtr->Set(outputSampleList);
 return static_cast<DataObjectPointer>(outputPtr);
 }

template < class TInputSampleList, class TOutputSampleList >
void
ShiftScaleSampleListFilter<TInputSampleList,TOutputSampleList>
::SetInput( const InputSampleListType * input )
 {
 typename InputSampleListObjectType::Pointer inputPtr = InputSampleListObjectType::New();
 inputPtr->Set(input);
 this->SetInput(inputPtr);
 }

template < class TInputSampleList, class TOutputSampleList >
void
ShiftScaleSampleListFilter<TInputSampleList,TOutputSampleList>
::SetInput( const InputSampleListObjectType * inputPtr )
 {
 // Process object is not const-correct so the const_cast is required here
 this->ProcessObject::SetNthInput(0,
                                    const_cast< InputSampleListObjectType* >( inputPtr ) );
 }

template < class TInputSampleList, class TOutputSampleList >
const typename ShiftScaleSampleListFilter<TInputSampleList,TOutputSampleList>
::InputSampleListObjectType *
ShiftScaleSampleListFilter<TInputSampleList,TOutputSampleList>
::GetInput() const
 {
 if (this->GetNumberOfInputs() < 1)
 {
  return 0;
 }

 return static_cast<const InputSampleListObjectType * >
 (this->ProcessObject::GetInput(0) );
 }

template < class TInputSampleList, class TOutputSampleList >
const typename ShiftScaleSampleListFilter<TInputSampleList,TOutputSampleList>
::InputSampleListType *
ShiftScaleSampleListFilter<TInputSampleList,TOutputSampleList>
::GetInputSampleList() const
 {
 if (this->GetNumberOfInputs() < 1)
 {
  return 0;
 }

 typename InputSampleListObjectType::Pointer dataObjectPointer = static_cast<const InputSampleListObjectType * >
 (this->ProcessObject::GetInput(0) );
 return dataObjectPointer->Get();
 }

template < class TInputSampleList, class TOutputSampleList >
typename ShiftScaleSampleListFilter<TInputSampleList,TOutputSampleList>
::OutputSampleListType *
ShiftScaleSampleListFilter<TInputSampleList,TOutputSampleList>
::GetOutputSampleList()
 {
 typename OutputSampleListObjectType::Pointer dataObjectPointer = static_cast<OutputSampleListObjectType * >
 (this->ProcessObject::GetOutput(0) );
 return const_cast<OutputSampleListType *>(dataObjectPointer->Get());
 }

template < class TInputSampleList, class TOutputSampleList >
typename ShiftScaleSampleListFilter<TInputSampleList,TOutputSampleList>
::OutputSampleListObjectType *
ShiftScaleSampleListFilter<TInputSampleList,TOutputSampleList>
::GetOutput()
 {
 return static_cast<OutputSampleListObjectType * >
  (this->ProcessObject::GetOutput(0) );
 }

template < class TInputSampleList, class TOutputSampleList >
void
ShiftScaleSampleListFilter<TInputSampleList,TOutputSampleList>
::GenerateData()
 {
 // Retrieve input and output pointers
 typename InputSampleListObjectType::ConstPointer inputPtr = this->GetInput();
 typename OutputSampleListObjectType::Pointer     outputPtr = this->GetOutput();

 // Retrieve the ListSample
 InputSampleListConstPointer inputSampleListPtr = inputPtr->Get();
 OutputSampleListPointer outputSampleListPtr    = const_cast<OutputSampleListType *>(outputPtr->Get());

 // Compute the 1/(sigma) vector
 InputMeasurementVectorType invertedScales = m_Scales;
 for(unsigned int idx = 0;idx < invertedScales.Size();++idx)
 {
  invertedScales[idx] = 1 / m_Scales[idx];
 }

 // Clear any previous output
 outputSampleListPtr->Clear();

 typename InputSampleListType::ConstIterator inputIt = inputSampleListPtr->Begin();

 // Set-up progress reporting
 itk::ProgressReporter progress(this,0,inputSampleListPtr->Size());

 // Iterate on the InputSampleList
 while(inputIt != inputSampleListPtr->End())
  {
  // Retrieve current input sample
  InputMeasurementVectorType currentInputMeasurement = inputIt.GetMeasurementVector();

  // Build current output sample
  OutputMeasurementVectorType currentOutputMeasurement;
  currentOutputMeasurement.SetSize(currentInputMeasurement.GetSize());

  // Center and reduce each component
  for(unsigned int idx = 0;idx < invertedScales.Size();++idx)
   {
   currentOutputMeasurement[idx] = static_cast<OutputValueType>(
   (currentInputMeasurement[idx]-m_Shifts[idx])*invertedScales[idx]);
   }

  // Add the current output sample to the output SampleList
  outputSampleListPtr->PushBack(currentOutputMeasurement);

  // Update progress
  progress.CompletedPixel();

  ++inputIt;
  }
 }

template < class TInputSampleList, class TOutputSampleList >
void
ShiftScaleSampleListFilter<TInputSampleList,TOutputSampleList>
::PrintSelf(std::ostream& os, itk::Indent indent) const
 {
 // Call superclass implementation
 Superclass::PrintSelf(os,indent);
 }


} // End namespace Statistics
} // End namespace otb

#endif
