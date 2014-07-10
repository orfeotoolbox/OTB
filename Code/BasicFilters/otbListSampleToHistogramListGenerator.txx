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
#ifndef __otbListSampleToHistogramListGenerator_txx
#define __otbListSampleToHistogramListGenerator_txx

#include "otbMacro.h"
#include "itkArray.h"

namespace otb
{

template<class TListSample,
    class THistogramMeasurement,
    class TFrequencyContainer>
ListSampleToHistogramListGenerator<TListSample,
    THistogramMeasurement,
    TFrequencyContainer>
::ListSampleToHistogramListGenerator()
 : m_Size(10),
   m_MarginalScale(100),
   m_AutoMinMax(true),
   m_NoDataFlag(false),
   m_NoDataValue(itk::NumericTraits<THistogramMeasurement>::Zero)
{
  m_Size.Fill(255);

  this->SetNumberOfRequiredInputs(1);
  this->SetNumberOfRequiredOutputs(1);

  this->itk::ProcessObject::SetNthOutput(0, this->MakeOutput(0).GetPointer());
}

// Set the input ListSample
template<class TListSample, class THistogramMeasurement, class TFrequencyContainer>
void
ListSampleToHistogramListGenerator<TListSample,THistogramMeasurement, TFrequencyContainer>
::SetListSample(const ListSampleType* inputlist)
{
  // Process object is not const-correct so the const_cast is required here
  this->itk::ProcessObject::SetNthInput(0,
                                        const_cast<ListSampleType*>(inputlist));

}

// Get the input ListSample
template<class TListSample, class THistogramMeasurement, class TFrequencyContainer>
const typename ListSampleToHistogramListGenerator<TListSample,THistogramMeasurement, TFrequencyContainer>
::ListSampleType*
ListSampleToHistogramListGenerator<TListSample,THistogramMeasurement, TFrequencyContainer>
::GetListSample() const
{
  if (this->GetNumberOfInputs() < 1)
    {
    return 0;
    }
  return static_cast<const ListSampleType* >
    (this->itk::ProcessObject::GetInput(0) );
}

// Get the output
template<class TListSample, class THistogramMeasurement, class TFrequencyContainer>
const typename ListSampleToHistogramListGenerator<TListSample,THistogramMeasurement, TFrequencyContainer>
::HistogramListType*
ListSampleToHistogramListGenerator<TListSample,THistogramMeasurement, TFrequencyContainer>
::GetOutput()
{
  return dynamic_cast<HistogramListType*>(this->itk::ProcessObject::GetOutput(0));
}

// MakeOutput implementation
template<class TListSample, class THistogramMeasurement, class TFrequencyContainer>
typename ListSampleToHistogramListGenerator<TListSample,THistogramMeasurement, TFrequencyContainer>
::DataObjectPointer
ListSampleToHistogramListGenerator<TListSample,THistogramMeasurement, TFrequencyContainer>
::MakeOutput(DataObjectPointerArraySizeType itkNotUsed(idx))
{
  DataObjectPointer output;
  output = static_cast<itk::DataObject*>(HistogramListType::New().GetPointer());
  return output;
}

// GenerateData
template<class TListSample,
    class THistogramMeasurement,
    class TFrequencyContainer>
void
ListSampleToHistogramListGenerator<TListSample,
    THistogramMeasurement,
    TFrequencyContainer>
::GenerateData()
{
  otbMsgDebugMacro(<< "ListSampleToHistogramListGenerator::GenerateData(): Entering");

  // Get the input ListSample
  const ListSampleType*            inputList = this->GetListSample();

  // Get a pointer on the output
  typename HistogramListType::Pointer histogramList = const_cast<HistogramListType*>(this->GetOutput());

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

  typename TListSample::MeasurementVectorType h_upper(inputList->GetMeasurementVectorSize());
  typename TListSample::MeasurementVectorType h_lower(inputList->GetMeasurementVectorSize());

  bool clipBinsAtEnds = true;

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
           (THistogramMeasurement) m_Size[0]) /
          (THistogramMeasurement) m_MarginalScale;
        h_upper[i] = (THistogramMeasurement) (upper[i] + margin);
        if (h_upper[i] <= upper[i])
          {
          // an overflow has occurred therefore set upper to upper
          h_upper[i] = upper[i];
          // Histogram measurement type would force the clipping the max value.
          // Therefore we must call the following to include the max value:
          clipBinsAtEnds = false;
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
          clipBinsAtEnds = false;
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

  // Clearing previous histograms
  histogramList->Clear();

  // For each dimension
  for (unsigned int comp = 0; comp < inputList->GetMeasurementVectorSize(); ++comp)
    {
    // initialize the Histogram object using the sizes and
    // the upper and lower bound from the FindSampleBound function
    typename HistogramType::MeasurementVectorType comp_lower(inputList->GetMeasurementVectorSize());
    typename HistogramType::MeasurementVectorType comp_upper(inputList->GetMeasurementVectorSize());

    comp_lower[0] = h_lower[comp];
    comp_upper[0] = h_upper[comp];

    otbMsgDevMacro(
      << "ListSampleToHistogramListGenerator::GenerateData(): Initializing histogram " << comp << " with (size= " <<
      m_Size << ", lower = " << comp_lower << ", upper = " << comp_upper << ")");

    // Create a new histogrma for this component : size of the
    // measurement vector is : 1
    histogramList->PushBack(HistogramType::New());
    histogramList->Back()->SetMeasurementVectorSize(1);
    histogramList->Back()->SetClipBinsAtEnds(clipBinsAtEnds);
    histogramList->Back()->Initialize(m_Size, comp_lower, comp_upper);

    typename TListSample::ConstIterator           iter = inputList->Begin();
    typename TListSample::ConstIterator           last = inputList->End();
    typename HistogramType::IndexType             index;
    typename HistogramType::MeasurementVectorType hvector(inputList->GetMeasurementVectorSize());

    while (iter != last)
      {
      hvector[0] = static_cast<THistogramMeasurement>(iter.GetMeasurementVector()[comp]);
      histogramList->Back()->GetIndex(hvector, index);
      if( (!m_NoDataFlag) || hvector[0]!=m_NoDataValue )
        {


      if (!histogramList->Back()->IsIndexOutOfBounds(index))
          {
          // if the measurement vector is out of bound then
          // the GetIndex method has returned an index set to the max size of
          // the invalid dimension - even if the hvector is less than the minimum
          // bin value.
          // If the index isn't valid, we don't increase the frequency.
          // See the comments in Histogram->GetIndex() for more info.
        histogramList->Back()->IncreaseFrequencyOfIndex(index, 1);
          }
        }
      ++iter;
      }
    }
  otbMsgDebugMacro(<< "ListSampleToHistogramListGenerator::GenerateData(): Leaving");
}

template<class TListSample,
    class THistogramMeasurement,
    class TFrequencyContainer>
void
ListSampleToHistogramListGenerator<TListSample,
    THistogramMeasurement,
    TFrequencyContainer>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "AutoMinMax: " << m_AutoMinMax << std::endl;
  os << indent << "Size: " << m_Size << std::endl;
  os << indent << "MarginalScale: " << m_MarginalScale << std::endl;
  os << indent << "HistogramMin: " << m_HistogramMin << std::endl;
  os << indent << "HistogramMax: " << m_HistogramMax << std::endl;
}

} // end of namespace itk

#endif
