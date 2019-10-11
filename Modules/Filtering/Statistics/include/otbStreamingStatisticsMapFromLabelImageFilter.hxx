/*
 * Copyright (C) 1999-2011 Insight Software Consortium
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbStreamingStatisticsMapFromLabelImageFilter_hxx
#define otbStreamingStatisticsMapFromLabelImageFilter_hxx
#include "otbStreamingStatisticsMapFromLabelImageFilter.h"

#include "itkInputDataObjectIterator.h"
#include "itkImageRegionIterator.h"
#include "itkProgressReporter.h"
#include "otbMacro.h"
#include <cmath>

namespace otb
{

template <class TInputVectorImage, class TLabelImage>
PersistentStreamingStatisticsMapFromLabelImageFilter<TInputVectorImage, TLabelImage>::PersistentStreamingStatisticsMapFromLabelImageFilter()
  : m_UseNoDataValue()
{
  // first output is a copy of the image, DataObject created by
  // superclass
  //
  // allocate the data objects for the outputs which are
  // just decorators around pixel types
  typename PixelValueMapObjectType::Pointer output = static_cast<PixelValueMapObjectType*>(this->MakeOutput(1).GetPointer());
  this->itk::ProcessObject::SetNthOutput(1, output.GetPointer());

  this->Reset();
}

template <class TInputVectorImage, class TLabelImage>
typename itk::DataObject::Pointer
PersistentStreamingStatisticsMapFromLabelImageFilter<TInputVectorImage, TLabelImage>::MakeOutput(DataObjectPointerArraySizeType itkNotUsed(output))
{
  return static_cast<itk::DataObject*>(PixelValueMapObjectType::New().GetPointer());
}

template <class TInputVectorImage, class TLabelImage>
void PersistentStreamingStatisticsMapFromLabelImageFilter<TInputVectorImage, TLabelImage>::SetInputLabelImage(const LabelImageType* input)
{
  // Process object is not const-correct so the const_cast is required here
  this->itk::ProcessObject::SetNthInput(1, const_cast<LabelImageType*>(input));
}

template <class TInputVectorImage, class TLabelImage>
const typename PersistentStreamingStatisticsMapFromLabelImageFilter<TInputVectorImage, TLabelImage>::LabelImageType*
PersistentStreamingStatisticsMapFromLabelImageFilter<TInputVectorImage, TLabelImage>::GetInputLabelImage()
{
  return static_cast<const TLabelImage*>(this->itk::ProcessObject::GetInput(1));
}

template <class TInputVectorImage, class TLabelImage>
typename PersistentStreamingStatisticsMapFromLabelImageFilter<TInputVectorImage, TLabelImage>::PixelValueMapType
PersistentStreamingStatisticsMapFromLabelImageFilter<TInputVectorImage, TLabelImage>::GetMeanValueMap() const
{
  return m_MeanRadiometricValue;
}

template <class TInputVectorImage, class TLabelImage>
typename PersistentStreamingStatisticsMapFromLabelImageFilter<TInputVectorImage, TLabelImage>::PixelValueMapType
PersistentStreamingStatisticsMapFromLabelImageFilter<TInputVectorImage, TLabelImage>::GetStandardDeviationValueMap() const
{
  return m_StDevRadiometricValue;
}

template <class TInputVectorImage, class TLabelImage>
typename PersistentStreamingStatisticsMapFromLabelImageFilter<TInputVectorImage, TLabelImage>::PixelValueMapType
PersistentStreamingStatisticsMapFromLabelImageFilter<TInputVectorImage, TLabelImage>::GetMinValueMap() const
{
  return m_MinRadiometricValue;
}

template <class TInputVectorImage, class TLabelImage>
typename PersistentStreamingStatisticsMapFromLabelImageFilter<TInputVectorImage, TLabelImage>::PixelValueMapType
PersistentStreamingStatisticsMapFromLabelImageFilter<TInputVectorImage, TLabelImage>::GetMaxValueMap() const
{
  return m_MaxRadiometricValue;
}

template <class TInputVectorImage, class TLabelImage>
typename PersistentStreamingStatisticsMapFromLabelImageFilter<TInputVectorImage, TLabelImage>::LabelPopulationMapType
PersistentStreamingStatisticsMapFromLabelImageFilter<TInputVectorImage, TLabelImage>::GetLabelPopulationMap() const
{
  return m_LabelPopulation;
}

template <class TInputVectorImage, class TLabelImage>
void PersistentStreamingStatisticsMapFromLabelImageFilter<TInputVectorImage, TLabelImage>::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();

  if (this->GetInput())
  {
    this->GetOutput()->CopyInformation(this->GetInput());
    this->GetOutput()->SetLargestPossibleRegion(this->GetInput()->GetLargestPossibleRegion());

    if (this->GetOutput()->GetRequestedRegion().GetNumberOfPixels() == 0)
    {
      this->GetOutput()->SetRequestedRegion(this->GetOutput()->GetLargestPossibleRegion());
    }
  }
}

template <class TInputVectorImage, class TLabelImage>
void PersistentStreamingStatisticsMapFromLabelImageFilter<TInputVectorImage, TLabelImage>::AllocateOutputs()
{
  // This is commented to prevent the streaming of the whole image for the first stream strip
  // It shall not cause any problem because the output image of this filter is not intended to be used.
  // InputImagePointer image = const_cast< TInputImage * >( this->GetInput() );
  // this->GraftOutput( image );
  // Nothing that needs to be allocated for the remaining outputs
}

template <class TInputVectorImage, class TLabelImage>
void PersistentStreamingStatisticsMapFromLabelImageFilter<TInputVectorImage, TLabelImage>::Synthetize()
{
  // Update temporary accumulator
  AccumulatorMapType outputAcc;
  auto               endAcc = outputAcc.end();

  for (auto const& threadAccMap : m_AccumulatorMaps)
  {
    for (auto const& it : threadAccMap)
    {
      auto label = it.first;
      auto itAcc = outputAcc.find(label);
      if (itAcc == endAcc)
      {
        outputAcc.emplace(label, it.second);
      }
      else
      {
        itAcc->second.Update(it.second);
      }
    }
  }

  // Publish output maps
  for (auto& it : outputAcc)
  {
    const LabelPixelType label     = it.first;
    const auto&          bandCount = it.second.GetBandCount();
    const auto&          sum       = it.second.GetSum();
    const auto&          sqSum     = it.second.GetSqSum();

    // Count
    m_LabelPopulation[label] = it.second.GetCount();

    // Mean & stdev
    RealVectorPixelType mean(sum);
    RealVectorPixelType std(sqSum);
    for (unsigned int band = 0; band < mean.GetSize(); band++)
    {
      // Number of valid pixels in band
      auto count = bandCount[band];
      // Mean
      mean[band] /= count;

      // Unbiased standard deviation (not sure unbiased is usefull here)
      const double variance = (sqSum[band] - (sum[band] * mean[band])) / (count - 1);
      std[band]             = std::sqrt(variance);
    }
    m_MeanRadiometricValue[label]  = mean;
    m_StDevRadiometricValue[label] = std;

    // Min & max
    m_MinRadiometricValue[label] = it.second.GetMin();
    m_MaxRadiometricValue[label] = it.second.GetMax();
  }
}

template <class TInputVectorImage, class TLabelImage>
void PersistentStreamingStatisticsMapFromLabelImageFilter<TInputVectorImage, TLabelImage>::Reset()
{
  m_AccumulatorMaps.clear();

  m_MeanRadiometricValue.clear();
  m_StDevRadiometricValue.clear();
  m_MinRadiometricValue.clear();
  m_MaxRadiometricValue.clear();
  m_LabelPopulation.clear();
  m_AccumulatorMaps.resize(this->GetNumberOfThreads());
}

template <class TInputVectorImage, class TLabelImage>
void PersistentStreamingStatisticsMapFromLabelImageFilter<TInputVectorImage, TLabelImage>::GenerateInputRequestedRegion()
{
  // The Requested Regions of all the inputs are set to their Largest Possible Regions
  this->itk::ProcessObject::GenerateInputRequestedRegion();

  // Iteration over all the inputs of the current filter (this)
  for (itk::InputDataObjectIterator it(this); !it.IsAtEnd(); it++)
  {
    // Check whether the input is an image of the appropriate dimension
    // dynamic_cast of all the input images as itk::ImageBase objects
    // in order to pass the if ( input ) test whatever the inputImageType (vectorImage or labelImage)
    ImageBaseType* input = dynamic_cast<ImageBaseType*>(it.GetInput());

    if (input)
    {
      // Use the function object RegionCopier to copy the output region
      // to the input.  The default region copier has default implementations
      // to handle the cases where the input and output are the same
      // dimension, the input a higher dimension than the output, and the
      // input a lower dimension than the output.
      InputImageRegionType inputRegion;
      this->CallCopyOutputRegionToInputRegion(inputRegion, this->GetOutput()->GetRequestedRegion());
      input->SetRequestedRegion(inputRegion);
    }
  }
}

template <class TInputVectorImage, class TLabelImage>
void PersistentStreamingStatisticsMapFromLabelImageFilter<TInputVectorImage, TLabelImage>::ThreadedGenerateData(const RegionType& outputRegionForThread,
                                                                                                                itk::ThreadIdType threadId)
{
  /**
   * Grab the input
   */
  InputVectorImagePointer inputPtr      = const_cast<TInputVectorImage*>(this->GetInput());
  LabelImagePointer       labelInputPtr = const_cast<TLabelImage*>(this->GetInputLabelImage());

  itk::ImageRegionConstIterator<TInputVectorImage> inIt(inputPtr, outputRegionForThread);
  itk::ImageRegionConstIterator<TLabelImage>       labelIt(labelInputPtr, outputRegionForThread);
  itk::ProgressReporter                            progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  auto& acc    = m_AccumulatorMaps[threadId];
  auto  endAcc = acc.end();

  // do the work
  for (inIt.GoToBegin(), labelIt.GoToBegin(); !inIt.IsAtEnd() && !labelIt.IsAtEnd(); ++inIt, ++labelIt)
  {
    const auto& value = inIt.Get();
    auto        label = labelIt.Get();

    // Update the accumulator
    auto itAcc = acc.find(label);
    if (itAcc == endAcc)
    {
      acc.emplace(label, AccumulatorType(this->GetNoDataValue(), this->GetUseNoDataValue(), value));
    }
    else
    {
      itAcc->second.Update(value);
    }

    progress.CompletedPixel();
  }
}

template <class TInputVectorImage, class TLabelImage>
void PersistentStreamingStatisticsMapFromLabelImageFilter<TInputVectorImage, TLabelImage>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb
#endif
