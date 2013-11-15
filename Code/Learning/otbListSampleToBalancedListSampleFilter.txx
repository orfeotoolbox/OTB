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
#ifndef __otbListSampleToBalancedListSampleFilter_txx
#define __otbListSampleToBalancedListSampleFilter_txx

#include "otbListSampleToBalancedListSampleFilter.h"
#include "itkProgressReporter.h"
#include "itkHistogram.h"
#include "itkNumericTraits.h"

namespace otb {
namespace Statistics {

// constructor
template < class TInputSampleList, class TLabelSampleList, class TOutputSampleList >
ListSampleToBalancedListSampleFilter<TInputSampleList, TLabelSampleList, TOutputSampleList>
::ListSampleToBalancedListSampleFilter()
{
  this->SetNumberOfRequiredInputs(2);
  this->SetNumberOfRequiredOutputs(2);

  // Create the second output
  //this->itk::ProcessObject::SetNthOutput(0, this->MakeOutput(0).GetPointer());
  this->itk::ProcessObject::SetNthOutput(1, this->MakeOutput(1).GetPointer());

  m_AddGaussianNoiseFilter = GaussianAdditiveNoiseType::New();
  m_BalancingFactor  = 5;
}

template < class TInputSampleList, class TLabelSampleList, class TOutputSampleList >
typename ListSampleToBalancedListSampleFilter<TInputSampleList, TLabelSampleList, TOutputSampleList>
::DataObjectPointer
ListSampleToBalancedListSampleFilter<TInputSampleList, TLabelSampleList, TOutputSampleList>
::MakeOutput(unsigned int idx)
{
  DataObjectPointer output;
  switch (idx)
    {
    case 0:
      Superclass::MakeOutput(0);
      break;
    case 1:
      {
      output = static_cast<itk::DataObject*>(LabelSampleListType::New().GetPointer());
      break;
      }
    default:
      output = static_cast<itk::DataObject*>(InputSampleListType::New().GetPointer());
      break;
    }
  return output;
}

// Method to set the SampleList as DataObject
template < class TInputSampleList, class TLabelSampleList, class TOutputSampleList >
void
ListSampleToBalancedListSampleFilter<TInputSampleList, TLabelSampleList, TOutputSampleList>
::SetInputLabel( const LabelSampleListType * labelPtr )
{
  // Process object is not const-correct so the const_cast is required here
  this->itk::ProcessObject::SetNthInput(1,
                                   const_cast< LabelSampleListType* >( labelPtr  ) );
}

// Method to get the SampleList as DataObject
template < class TInputSampleList, class TLabelSampleList, class TOutputSampleList >
const typename ListSampleToBalancedListSampleFilter<TInputSampleList, TLabelSampleList, TOutputSampleList>
::LabelSampleListType *
ListSampleToBalancedListSampleFilter<TInputSampleList, TLabelSampleList, TOutputSampleList>
::GetInputLabel() const
{
  if (this->GetNumberOfInputs() < 2)
    {
    return 0;
    }

  return static_cast<const  LabelSampleListType* >
    (this->itk::ProcessObject::GetInput(1) );
}

// Get the output label SampleList as DataObject
template < class TInputSampleList, class TLabelSampleList, class TOutputSampleList >
typename ListSampleToBalancedListSampleFilter<TInputSampleList,TLabelSampleList,TOutputSampleList>
::LabelSampleListType *
ListSampleToBalancedListSampleFilter<TInputSampleList, TLabelSampleList, TOutputSampleList>
::GetOutputLabel()
{
  return dynamic_cast<LabelSampleListType*>(this->itk::ProcessObject::GetOutput(1));
}


// Get the max sample number having the same label
template < class TInputSampleList, class TLabelSampleList, class TOutputSampleList >
void
ListSampleToBalancedListSampleFilter<TInputSampleList, TLabelSampleList, TOutputSampleList>
::ComputeMaxSampleFrequency()
{
  // Iterate on the labelSampleList to get the min and max label
  LabelValueType   maxLabel = itk::NumericTraits<LabelValueType>::min();

  // Number of bins to add to the histogram
  typename LabelSampleListType::ConstPointer  labelPtr = this->GetInputLabel();
  typename LabelSampleListType::ConstIterator labelIt = labelPtr->Begin();

  while(labelIt != labelPtr->End())
    {
    // Get the current label sample
    LabelMeasurementVectorType currentInputMeasurement = labelIt.GetMeasurementVector();

    if (currentInputMeasurement[0] > maxLabel)
      maxLabel = currentInputMeasurement[0];

    ++labelIt;
    }

  // Prepare histogram with dimension 1 : default template parameters
  typedef typename itk::Statistics::Histogram<unsigned int>    HistogramType;
  typename HistogramType::Pointer histogram = HistogramType::New();
  typename HistogramType::SizeType  size(1);
  size.Fill(maxLabel +1);
  histogram->SetMeasurementVectorSize(1); // we need only one dimension
  histogram->Initialize(size);

  labelIt = labelPtr->Begin();
  while (labelIt != labelPtr->End())
    {
    // Get the current label sample
    LabelMeasurementVectorType currentInputMeasurement = labelIt.GetMeasurementVector();
    histogram->IncreaseFrequency(currentInputMeasurement[0], 1);
    ++labelIt;
    }

  // Iterate through the histogram to get the maximum
  unsigned int maxvalue  = 0;
  HistogramType::Iterator iter = histogram->Begin();

  while ( iter != histogram->End() )
    {
    if( static_cast<unsigned int>(iter.GetFrequency()) > maxvalue )
      maxvalue = static_cast<unsigned int>(iter.GetFrequency());
    ++iter;
    }

  // Number of sample per label to reach in order to have a balanced
  // ListSample
  unsigned int balancedFrequency = m_BalancingFactor * maxvalue;

  // Guess how much noised samples must be added per sample to get
  // a balanced ListSample : Computed using the
  //  - Frequency of each label  (stored in the histogram)
  //  - The value maxvalue by m_BalancingFactor
  // The std::vector below stores the multiplicative factor
  iter = histogram->Begin();
  while ( iter != histogram->End() )
    {
    if(iter.GetFrequency() - 1e-10 < 0.)
      m_MultiplicativeCoefficient.push_back(0);
    else
      {
      unsigned int coeff = static_cast<unsigned int>(balancedFrequency/iter.GetFrequency());
      m_MultiplicativeCoefficient.push_back(coeff);
      }

    ++iter;
    }
}

template < class TInputSampleList, class TLabelSampleList, class TOutputSampleList >
void
ListSampleToBalancedListSampleFilter<TInputSampleList, TLabelSampleList, TOutputSampleList>
::GenerateData()
{
  // Get the how much each sample must be expanded
  this->ComputeMaxSampleFrequency();

  // Retrieve input and output pointers
  InputSampleListConstPointer  inputSampleListPtr  = this->GetInput();
  LabelSampleListConstPointer labelSampleListPtr  = this->GetInputLabel();
  OutputSampleListPointer     outputSampleListPtr = this->GetOutput();
  LabelSampleListPointer      outputLabel         = this->GetOutputLabel();

   // Clear any previous output
   outputSampleListPtr->Clear();

   // Set the measurement vectors size
   outputSampleListPtr->SetMeasurementVectorSize(inputSampleListPtr->GetMeasurementVectorSize());
   outputLabel->SetMeasurementVectorSize(labelSampleListPtr->GetMeasurementVectorSize());

   typename InputSampleListType::ConstIterator inputIt = inputSampleListPtr->Begin();
   typename LabelSampleListType::ConstIterator labelIt = labelSampleListPtr->Begin();

   // Set-up progress reporting
   itk::ProgressReporter progress(this, 0, inputSampleListPtr->Size());

   // Noising filter
   GaussianAdditiveNoisePointerType  noisingFilter = GaussianAdditiveNoiseType::New();

   // Iterate on the InputSampleList
   while(inputIt != inputSampleListPtr->End() && labelIt != labelSampleListPtr->End())
     {
     // Retrieve current input sample
     InputMeasurementVectorType currentInputMeasurement = inputIt.GetMeasurementVector();
     // Retrieve the current label
     LabelMeasurementVectorType currentLabelMeasurement = labelIt.GetMeasurementVector();

     // Build a temporary ListSample wiht the current
     // measurement vector to generate noised versions of this
     // measurement vector
     InputSampleListPointer tempListSample = InputSampleListType::New();
     tempListSample->SetMeasurementVectorSize(inputSampleListPtr->GetMeasurementVectorSize());
     tempListSample->PushBack(currentInputMeasurement);

     // Get how many times we have to noise this sample
     unsigned int iterations =  m_MultiplicativeCoefficient[currentLabelMeasurement[0]];

     // Noising filter
     //GaussianAdditiveNoisePointerType  noisingFilter = GaussianAdditiveNoiseType::New();
     noisingFilter->SetInput(tempListSample);
     noisingFilter->SetNumberOfIteration(iterations);
     noisingFilter->Update();

     // Build current output sample
     OutputMeasurementVectorType currentOutputMeasurement;
     currentOutputMeasurement.SetSize(currentInputMeasurement.GetSize());

     // Cast the current sample in outputSampleValue
     for(unsigned int idx = 0; idx < inputSampleListPtr->GetMeasurementVectorSize(); ++idx)
       currentOutputMeasurement[idx] = static_cast<OutputValueType>(currentInputMeasurement[idx]);

     // Add the current input casted sample to the output SampleList
     outputSampleListPtr->PushBack(currentOutputMeasurement);

     // Add the currentsample list label
     outputLabel->PushBack(currentLabelMeasurement);

     // Add the noised versions of the current sample to OutputSampleList
     typename OutputSampleListType::ConstIterator tempIt = noisingFilter->GetOutput()->Begin();

     while(tempIt != noisingFilter->GetOutput()->End())
       {
       // Get the noised sample of the current measurement vector
       OutputMeasurementVectorType currentTempMeasurement = tempIt.GetMeasurementVector();
       // Add to output SampleList
       outputSampleListPtr->PushBack(currentTempMeasurement);

       // Add a label in the output ListSample
       outputLabel->PushBack(currentLabelMeasurement);

       ++tempIt;
       }

     // Update progress
     progress.CompletedPixel();

     ++inputIt;
     ++ labelIt;
     }
}

template < class TInputSampleList, class TLabelSampleList, class TOutputSampleList >
void
ListSampleToBalancedListSampleFilter<TInputSampleList, TLabelSampleList, TOutputSampleList>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  // Call superclass implementation
  Superclass::PrintSelf(os, indent);
}

} // End namespace Statistics
} // End namespace otb

#endif
