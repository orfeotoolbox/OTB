/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbListSampleToVariableDimensionHistogramGenerator_txx
#define __otbListSampleToVariableDimensionHistogramGenerator_txx

#include "otbMacro.h"
#include "itkStatisticsAlgorithm.h"

namespace otb
{

template<class TListSample,
    class THistogramMeasurement,
    class TFrequencyContainer>
ListSampleToVariableDimensionHistogramGenerator<TListSample,
    THistogramMeasurement,
    TFrequencyContainer>
::ListSampleToVariableDimensionHistogramGenerator():m_Sizes(1)
{
  m_MarginalScale = 100;
  m_AutoMinMax = true;

  this->SetNumberOfRequiredInputs(1);
  this->SetNumberOfRequiredOutputs(1);

  this->itk::ProcessObject::SetNthOutput(0, this->MakeOutput(0).GetPointer());
}

// Set the input sample list
template<class TListSample, class THistogramMeasurement, class TFrequencyContainer>
void
ListSampleToVariableDimensionHistogramGenerator<TListSample, THistogramMeasurement,TFrequencyContainer>
::SetListSample(const ListSampleType* inputlist)
{
  // Process object is not const-correct so the const_cast is required here
  this->itk::ProcessObject::SetNthInput(0,
                                        const_cast<ListSampleType*>(inputlist));
}

// Get the input sample list
template<class TListSample, class THistogramMeasurement, class TFrequencyContainer>
const typename ListSampleToVariableDimensionHistogramGenerator<TListSample,THistogramMeasurement, TFrequencyContainer>
::ListSampleType*
ListSampleToVariableDimensionHistogramGenerator<TListSample, THistogramMeasurement,TFrequencyContainer>
::GetListSample() const
{
  if (this->GetNumberOfInputs() < 1)
    {
    return 0;
    }
  return static_cast<const ListSampleType* >
    (this->itk::ProcessObject::GetInput(0) );
}

// Get the output Histogram
template<class TListSample, class THistogramMeasurement, class TFrequencyContainer>
const typename ListSampleToVariableDimensionHistogramGenerator<TListSample,THistogramMeasurement, TFrequencyContainer>
::HistogramType*
ListSampleToVariableDimensionHistogramGenerator<TListSample, THistogramMeasurement,TFrequencyContainer>
::GetOutput()
{
  return dynamic_cast<HistogramType*>(this->itk::ProcessObject::GetOutput(0));
}

// MakeOutput implementation
template<class TListSample, class THistogramMeasurement, class TFrequencyContainer>
typename ListSampleToVariableDimensionHistogramGenerator<TListSample,THistogramMeasurement, TFrequencyContainer>
::DataObjectPointer
ListSampleToVariableDimensionHistogramGenerator<TListSample, THistogramMeasurement,TFrequencyContainer>
::MakeOutput(unsigned int itkNotUsed(idx))
{
  DataObjectPointer output;
  output = static_cast<itk::DataObject*>(HistogramType::New().GetPointer());
  return output;
}

// GenerateData method,
template<class TListSample,
         class THistogramMeasurement,
         class TFrequencyContainer>
void
ListSampleToVariableDimensionHistogramGenerator<TListSample,THistogramMeasurement, TFrequencyContainer>
::GenerateData()
{
  // Get the input ListSample
  const ListSampleType  *          inputList = this->GetListSample();

  // Get a pointer on the output
  typename HistogramType::Pointer histogram = const_cast<HistogramType*>(this->GetOutput());

  otbMsgDebugMacro(<< "ListSampleToVariableDimensionHistogramGenerator::GenerateData(): Entering");
  // TODO : Sanity checks
  if (m_Sizes.GetSize() != inputList->GetMeasurementVectorSize())
    {
    itkExceptionMacro("Sample list measurement vectors and histogram bin sizes vector have different dimensions !");
    }

  if (!m_AutoMinMax)
    {
    if (m_HistogramMin.GetSize() != inputList->GetMeasurementVectorSize())
      {
      itkExceptionMacro("Sample list measurement vectors and histogram min have different dimensions !");
      }
    if (m_HistogramMax.GetSize() != inputList->GetMeasurementVectorSize())
      {
      itkExceptionMacro("Sample list measurement vectors and histogram max have different dimensions !");
      }
    }

  typename TListSample::MeasurementVectorType lower(inputList->GetMeasurementVectorSize());
  typename TListSample::MeasurementVectorType upper(inputList->GetMeasurementVectorSize());

  typename HistogramType::MeasurementVectorType h_upper(inputList->GetMeasurementVectorSize());
  typename HistogramType::MeasurementVectorType h_lower(inputList->GetMeasurementVectorSize());

  // must test for the list size to avoid making FindSampleBound() segfault.
  // Also, the min and max can't be found automatically in that case. We can
  // only return an empty histogram
  if (m_AutoMinMax && inputList->Size() != 0)
    {
    itk::Statistics::Algorithm::FindSampleBound<ListSampleType>(inputList, inputList->Begin(),
                    inputList->End(), lower, upper);
    float margin;

    for (unsigned int i = 0; i < inputList->GetMeasurementVectorSize(); ++i)
      {
      if (!itk::NumericTraits<THistogramMeasurement>::is_integer)
        {
        margin =
          ((THistogramMeasurement) (upper[i] - lower[i]) /
           (THistogramMeasurement) m_Sizes[i]) /
          (THistogramMeasurement) m_MarginalScale;
        h_upper[i] = (THistogramMeasurement) (upper[i] + margin);
        if (h_upper[i] <= upper[i])
          {
          // an overflow has occurred therefore set upper to upper
          h_upper[i] = upper[i];
          // Histogram measurement type would force the clipping the max value.
          // Therefore we must call the following to include the max value:
          histogram->SetClipBinsAtEnds(false);
          // The above function is okay since here we are within the autoMinMax
          // computation and clearly the user intended to include min and max.
          }
        }
      else
        {
        h_upper[i] = ((THistogramMeasurement) upper[i]) +
                     itk::NumericTraits<THistogramMeasurement>::One;
        if (h_upper[i] <= upper[i])
          {
          // an overflow has occurred therefore set upper to upper
          h_upper[i] = upper[i];
          // Histogram measurement type would force the clipping the max value.
          // Therefore we must call the following to include the max value:
          histogram->SetClipBinsAtEnds(false);
          // The above function is okay since here we are within the autoMinMax
          // computation and clearly the user intended to include min and max.
          }
        }
      h_lower[i] = (THistogramMeasurement) lower[i];
      }
    }
  else
    {
    h_lower = m_HistogramMin;
    h_upper = m_HistogramMax;
    }

  // initialize the Histogram object using the sizes and
  // the upper and lower bound from the FindSampleBound function
  // Have to Set the MeasurementVectorSize
  histogram->SetMeasurementVectorSize(inputList->GetMeasurementVectorSize());


  otbMsgDevMacro(
    << "ListSampleToVariableDimensionHistogramGenerator::GenerateData(): Initializing histogram with (sizes= " <<
    m_Sizes << ", lower = " << h_lower << ", upper = " << h_upper << ")");
  histogram->Initialize(m_Sizes, h_lower, h_upper);
  otbMsgDevMacro(<< "ListSampleToVariableDimensionHistogramGenerator::GenerateData(): Histogram initialized");
  typename TListSample::ConstIterator iter = inputList->Begin();
  typename TListSample::ConstIterator last = inputList->End();
  typename HistogramType::IndexType index(inputList->GetMeasurementVectorSize());
  typename TListSample::MeasurementVectorType lvector(inputList->GetMeasurementVectorSize());
  typename HistogramType::MeasurementVectorType hvector(inputList->GetMeasurementVectorSize());
  unsigned int i;

  otbMsgDevMacro(<< "ListSampleToVariableDimensionHistogramGenerator::GenerateData(): Filling the histogram");
  while (iter != last)
    {
    lvector = iter.GetMeasurementVector();
    for (i = 0; i < inputList->GetMeasurementVectorSize(); ++i)
      {
      hvector[i] = (THistogramMeasurement) lvector[i];
      }
    histogram->GetIndex(hvector, index);

    if (!histogram->IsIndexOutOfBounds(index))
      {
      // if the measurement vector is out of bound then
      // the GetIndex method has returned an index set to the max size of
      // the invalid dimension - even if the hvector is less than the minimum
      // bin value.
      // If the index isn't valid, we don't increase the frequency.
      // See the comments in Histogram->GetIndex() for more info.
      histogram->IncreaseFrequencyOfIndex(index, 1);
      }
    ++iter;
    }

  otbMsgDebugMacro(<< "ListSampleToVariableDimensionHistogramGenerator::GenerateData(): Leaving");
}

template<class TListSample,
    class THistogramMeasurement,
    class TFrequencyContainer>
void
ListSampleToVariableDimensionHistogramGenerator<TListSample,
    THistogramMeasurement,
    TFrequencyContainer>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "AutoMinMax: " << m_AutoMinMax << std::endl;
  os << indent << "Sizes: " << m_Sizes << std::endl;
  os << indent << "MarginalScale: " << m_MarginalScale << std::endl;
  os << indent << "HistogramMin: " << m_HistogramMin << std::endl;
  os << indent << "HistogramMax: " << m_HistogramMax << std::endl;
}

} // end of namespace itk

#endif
