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
ShiftScaleSampleListFilter<TInputSampleList, TOutputSampleList>
::ShiftScaleSampleListFilter(){}

template < class TInputSampleList, class TOutputSampleList >
void
ShiftScaleSampleListFilter<TInputSampleList, TOutputSampleList>
::GenerateData()
{
  // Retrieve input and output pointers
  InputSampleListConstPointer  inputSampleListPtr  = this->GetInput();

  OutputSampleListPointer      outputSampleListPtr = this->GetOutput();
  outputSampleListPtr->SetMeasurementVectorSize(inputSampleListPtr->GetMeasurementVectorSize());

  // Check if the inputSampleList is not empty
  if(inputSampleListPtr->Size() == 0)
    itkExceptionMacro(<< "Input Sample List is empty");
  
  // Check if the size of the scale and the shift measurement vectors
  // are the same than the input vector
  if(inputSampleListPtr->GetMeasurementVectorSize() != m_Scales.Size()
     || inputSampleListPtr->GetMeasurementVectorSize() != m_Shifts.Size())
    itkExceptionMacro(<< "Inconsistent measurement vector size : Input Sample List size "
                      << inputSampleListPtr->GetMeasurementVectorSize()
                      << " Scale measurement vector size "
                      <<m_Scales.Size()
                      <<" Shift measurement vector size "
                      <<m_Shifts.Size());
  
  // Compute the 1/(sigma) vector
  InputMeasurementVectorType invertedScales = m_Scales;
  for(unsigned int idx = 0; idx < invertedScales.Size(); ++idx)
    {
    if(m_Scales[idx]-1e-10 < 0.)
      invertedScales[idx] = 0.;
    else
      invertedScales[idx] = 1 / m_Scales[idx];
    }

  // Clear any previous output
  outputSampleListPtr->Clear();

  typename InputSampleListType::ConstIterator inputIt = inputSampleListPtr->Begin();

  // Set-up progress reporting
  itk::ProgressReporter progress(this, 0, inputSampleListPtr->Size());

  // Iterate on the InputSampleList
  while(inputIt != inputSampleListPtr->End())
    {
    // Retrieve current input sample
    InputMeasurementVectorType currentInputMeasurement = inputIt.GetMeasurementVector();
  
    // Build current output sample
    OutputMeasurementVectorType currentOutputMeasurement;
    currentOutputMeasurement.SetSize(currentInputMeasurement.GetSize());
  
    // Center and reduce each component
    for(unsigned int idx = 0; idx < invertedScales.Size(); ++idx)
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
ShiftScaleSampleListFilter<TInputSampleList, TOutputSampleList>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  // Call superclass implementation
  Superclass::PrintSelf(os, indent);
}

} // End namespace Statistics
} // End namespace otb

#endif
