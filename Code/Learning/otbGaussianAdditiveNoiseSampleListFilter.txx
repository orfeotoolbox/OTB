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
#ifndef __otbGaussianAdditiveNoiseSampleListFilter_txx
#define __otbGaussianAdditiveNoiseSampleListFilter_txx

#include "otbGaussianAdditiveNoiseSampleListFilter.h"
#include "itkMersenneTwisterRandomVariateGenerator.h"
#include "itkProgressReporter.h"

namespace otb {
namespace Statistics {

template < class TInputSampleList, class TOutputSampleList >
GaussianAdditiveNoiseSampleListFilter<TInputSampleList,TOutputSampleList>
::GaussianAdditiveNoiseSampleListFilter()
{
  m_Mean = 0.;
  m_Variance = 1.;
}

template < class TInputSampleList, class TOutputSampleList >
void
GaussianAdditiveNoiseSampleListFilter<TInputSampleList,TOutputSampleList>
::GenerateRandomSequence()
{
  typedef itk::Statistics::MersenneTwisterRandomVariateGenerator GeneratorType;
  GeneratorType::Pointer   generator   = GeneratorType::New();

  unsigned int size = this->GetInput()->Get()->GetMeasurementVectorSize();
  std::cout << "size " << size<< std::endl;
  if(size == 0)
    {
    itkExceptionMacro(<< "MeasurementVector size is  "<<size << " , excpect non null size " );
    }
  else
    for(unsigned int i = 0; i <size  ; i++)
      {
      double ran = generator->GetNormalVariate(m_Mean,m_Variance);
      m_WhiteGaussianNoiseCoefficients.push_back(ran);
      }
}

template < class TInputSampleList, class TOutputSampleList >
void
GaussianAdditiveNoiseSampleListFilter<TInputSampleList,TOutputSampleList>
::GenerateData()
{

  // Generate Random sequence 
  this->GenerateRandomSequence();

  // Retrieve input and output pointers
  typename InputSampleListObjectType::ConstPointer inputPtr = this->GetInput();
  typename OutputSampleListObjectType::Pointer     outputPtr = this->GetOutput();

  // Retrieve the ListSample
  InputSampleListConstPointer inputSampleListPtr = inputPtr->Get();
  OutputSampleListPointer outputSampleListPtr    = const_cast<OutputSampleListType *>(outputPtr->Get());
  
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
    for(unsigned int idx = 0;idx < inputSampleListPtr->GetMeasurementVectorSize();++idx)
      {
      currentOutputMeasurement[idx] = static_cast<OutputValueType>(
        (static_cast<double>(currentInputMeasurement[idx])+m_WhiteGaussianNoiseCoefficients[idx]));
      }
    std::cout <<"input sample "<<currentInputMeasurement << " output sample "<<  currentOutputMeasurement  << std::endl;

    // Add the current output sample to the output SampleList
    outputSampleListPtr->PushBack(currentOutputMeasurement);

    // Update progress
    progress.CompletedPixel();

    ++inputIt;
    }
}

template < class TInputSampleList, class TOutputSampleList >
void
GaussianAdditiveNoiseSampleListFilter<TInputSampleList,TOutputSampleList>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  // Call superclass implementation
  Superclass::PrintSelf(os,indent);
}


} // End namespace Statistics
} // End namespace otb

#endif
