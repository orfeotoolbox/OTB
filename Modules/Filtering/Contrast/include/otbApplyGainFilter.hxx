/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbApplyGainFilter_hxx
#define otbApplyGainFilter_hxx

#include "otbApplyGainFilter.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkContinuousIndex.h"

#include <limits>

namespace otb
{
template <class TInputImage, class TLut, class TOutputImage>
ApplyGainFilter<TInputImage, TLut, TOutputImage>::ApplyGainFilter()
{
  this->SetNumberOfRequiredInputs(2);
  m_Min                  = std::numeric_limits<InputPixelType>::quiet_NaN();
  m_Max                  = std::numeric_limits<InputPixelType>::quiet_NaN();
  m_NoData               = std::numeric_limits<InputPixelType>::quiet_NaN();
  m_NoDataFlag           = false;
  m_ThumbSizeFromSpacing = true;
  m_Step                 = -1;
}

template <class TInputImage, class TLut, class TOutputImage>
void ApplyGainFilter<TInputImage, TLut, TOutputImage>::SetInputImage(const InputImageType* input)
{
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(0, const_cast<InputImageType*>(input));
}

template <class TInputImage, class TLut, class TOutputImage>
const TInputImage* ApplyGainFilter<TInputImage, TLut, TOutputImage>::GetInputImage() const
{
  return static_cast<const InputImageType*>(this->itk::ProcessObject::GetInput(0));
}

template <class TInputImage, class TLut, class TOutputImage>
void ApplyGainFilter<TInputImage, TLut, TOutputImage>::SetInputLut(const LutType* lut)
{
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(1, const_cast<LutType*>(lut));
}

template <class TInputImage, class TLut, class TOutputImage>
const TLut* ApplyGainFilter<TInputImage, TLut, TOutputImage>::GetInputLut() const
{
  return static_cast<const LutType*>(this->itk::ProcessObject::GetInput(1));
}

template <class TInputImage, class TLut, class TOutputImage>
void ApplyGainFilter<TInputImage, TLut, TOutputImage>::GenerateInputRequestedRegion()
{
  typename InputImageType::Pointer  input(const_cast<InputImageType*>(GetInputImage()));
  typename LutType::Pointer         lut(const_cast<LutType*>(GetInputLut()));
  typename OutputImageType::Pointer output(this->GetOutput());

  lut->SetRequestedRegion(lut->GetLargestPossibleRegion());
  input->SetRequestedRegion(output->GetRequestedRegion());
  if (input->GetRequestedRegion().GetNumberOfPixels() == 0)
  {
    input->SetRequestedRegionToLargestPossibleRegion();
  }
}

template <class TInputImage, class TLut, class TOutputImage>
void ApplyGainFilter<TInputImage, TLut, TOutputImage>::BeforeThreadedGenerateData()
{
  typename LutType::ConstPointer        lut(GetInputLut());
  typename InputImageType::ConstPointer input(GetInputImage());
  if (m_ThumbSizeFromSpacing)
  {
    m_ThumbSize[0] = std::round(lut->GetSignedSpacing()[0] / input->GetSignedSpacing()[0]);
    m_ThumbSize[1] = std::round(lut->GetSignedSpacing()[1] / input->GetSignedSpacing()[1]);
  }
  m_Step = static_cast<double>(m_Max - m_Min) / static_cast<double>(lut->GetVectorLength() - 1);
}

template <class TInputImage, class TLut, class TOutputImage>
void ApplyGainFilter<TInputImage, TLut, TOutputImage>::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                                                                            itk::ThreadIdType itkNotUsed(threadId))
{
  assert(m_Step > 0);
  // TODO error
  // support progress methods/callbacks
  // itk::ProgressReporter progress(this , threadId ,
  //               outputRegionForThread.GetNumberOfPixels() );

  typename InputImageType::ConstPointer input(GetInputImage());
  typename LutType::ConstPointer        lut(GetInputLut());
  typename OutputImageType::Pointer     output(this->GetOutput());
  typename InputImageType::RegionType   inputRegionForThread(outputRegionForThread);


  itk::ImageRegionConstIteratorWithIndex<InputImageType> it(input, inputRegionForThread);
  itk::ImageRegionIterator<OutputImageType>              oit(output, outputRegionForThread);

  unsigned int   pixelLutValue(0);
  double         gain(0.0), newValue(0);
  InputPixelType currentPixel(0);

  for (it.GoToBegin(), oit.GoToBegin(); !oit.IsAtEnd() || !it.IsAtEnd(); ++oit, ++it)
  {
    currentPixel = it.Get();
    newValue     = static_cast<double>(currentPixel);
    if (!((currentPixel == m_NoData && m_NoDataFlag) || currentPixel > m_Max || currentPixel < m_Min))
    {
      pixelLutValue = static_cast<unsigned int>(std::round((currentPixel - m_Min) / m_Step));
      gain          = InterpolateGain(lut, pixelLutValue, it.GetIndex());
      newValue *= gain;
    }
    oit.Set(static_cast<OutputPixelType>(newValue));
  }
  assert(oit.IsAtEnd() && it.IsAtEnd());
}

template <class TInputImage, class TLut, class TOutputImage>
double ApplyGainFilter<TInputImage, TLut, TOutputImage>::InterpolateGain(typename LutType::ConstPointer gridLut, unsigned int pixelLutValue,
                                                                         typename InputImageType::IndexType index)
{
  typename InputImageType::PointType pixelPoint;
  typename itk::ContinuousIndex<double, 2> pixelIndex;
  typename InputImageType::ConstPointer input(GetInputImage());
  typename LutType::ConstPointer        lut(GetInputLut());
  input->TransformIndexToPhysicalPoint(index, pixelPoint);
  lut->TransformPhysicalPointToContinuousIndex(pixelPoint, pixelIndex);
  std::vector<typename LutType::IndexType> neighbors(4);
  neighbors[0][0] = std::floor(pixelIndex[0]);
  neighbors[0][1] = std::floor(pixelIndex[1]);
  neighbors[1][0] = neighbors[0][0] + 1;
  neighbors[1][1] = neighbors[0][1];
  neighbors[2][0] = neighbors[0][0];
  neighbors[2][1] = neighbors[0][1] + 1;
  neighbors[3][0] = neighbors[0][0] + 1;
  neighbors[3][1] = neighbors[0][1] + 1;
  float                       gain(0.f), w(0.f), wtm(0.f);
  typename LutType::IndexType maxIndex;
  maxIndex[0] = lut->GetLargestPossibleRegion().GetSize()[0];
  maxIndex[1] = lut->GetLargestPossibleRegion().GetSize()[1];
  for (auto i : neighbors)
  {
    if (i[0] < 0 || i[1] < 0 || i[0] >= maxIndex[0] || i[1] >= maxIndex[1])
      continue;
    if (gridLut->GetPixel(i)[pixelLutValue] == -1)
      continue;
    wtm = (1 - std::abs(pixelIndex[0] - i[0])) * (1 - std::abs(pixelIndex[1] - i[1]));
    gain += gridLut->GetPixel(i)[pixelLutValue] * wtm;
    w += wtm;
  }
  if (w == 0)
  {
    w    = 1;
    gain = 1;
  }

  return gain / w;
}

/**
 * Standard "PrintSelf" method
 */
template <class TInputImage, class TLut, class TOutputImage>
void ApplyGainFilter<TInputImage, TLut, TOutputImage>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Is no data activated : " << m_NoDataFlag << std::endl;
  os << indent << "No Data : " << m_NoData << std::endl;
  os << indent << "Minimum : " << m_Min << std::endl;
  os << indent << "Maximum : " << m_Max << std::endl;
  os << indent << "Step : " << m_Step << std::endl;
  os << indent << "Look up table size : " << m_LutSize << std::endl;
  os << indent << "Is ThumbSize from sapcing is activated : " << m_NoDataFlag << std::endl;
  os << indent << "Thumbnail size : " << m_ThumbSize << std::endl;
}


} // End namespace otb

#endif
