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

#ifndef otbResetMarginFilter_hxx
#define otbResetMarginFilter_hxx

#include "otbResetMarginFilter.h"
#include "otbInterval.h"
#include "otbMacro.h"
#include "otbLogHelpers.h"
#include "itkImageScanlineConstIterator.h"
#include "itkImageScanlineIterator.h"
#include "itkProgressReporter.h"
#include <algorithm>
#include <cassert>
#include <ostream>

namespace otb
{

template<typename TImage>
void
ResetMarginFilter<TImage>
::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();
  OutputImageType* output = this->GetOutput();
  const InputImageType* input = this->GetInput();
  output->SetNumberOfComponentsPerPixel(input->GetNumberOfComponentsPerPixel());
}

template<typename TImage>
void
ResetMarginFilter<TImage>
::ThreadedGenerateData(
    OutputImageRegionType const& outputRegionForThread,
    itk::ThreadIdType            threadId)
{
  using InputIterator   = itk::ImageScanlineConstIterator<InputImageType const>;
  using OutputIterator  = itk::ImageScanlineIterator<OutputImageType>;

  auto const* input  = this->GetInput();
  auto      * output = this->GetOutput();
  assert(input);
  assert(output);

  auto curRoi = OutputRegionToInputRegion(outputRegionForThread);

  auto const& size      = outputRegionForThread.GetSize();
  auto const& index     = outputRegionForThread.GetIndex();
  auto const  sizeX     = size[0];
  auto const  sizeY     = size[1];
  auto const  startX    = index[0];
  auto const  startY    = index[1];
  itk::IndexValueType const  endX      = startX + sizeX;
  itk::IndexValueType const  endY      = startY + sizeY;

  auto const nBand = output->GetNumberOfComponentsPerPixel();

  itk::IndexValueType thrX1 = curRoi.GetIndex(0);
  itk::IndexValueType thrX2 = curRoi.GetIndex(0) + curRoi.GetSize(0);
  itk::IndexValueType thrY1 = curRoi.GetIndex(1);
  itk::IndexValueType thrY2 = curRoi.GetIndex(1) + curRoi.GetSize(1);

  assert(thrX1 <= endX && "Iterations shall stay within requested region");
  assert(thrX2 <= endX && "Iterations shall stay within requested region");
  assert(thrY1 <= endY && "Iterations shall stay within requested region");
  assert(thrY2 <= endY && "Iterations shall stay within requested region");

  auto const full_line = sizeX * nBand;
  auto const nb_z_l = (unsigned long)(thrX1 - startX) * nBand;
  auto const nb_c_m = (unsigned long)(thrX2 - thrX1) * nBand;
  auto const nb_z_r = (unsigned long)(endX - thrX2) * nBand;
  assert(nb_z_l + nb_c_m + nb_z_r == full_line);

  const InternalPixelType *inData = input->GetBufferPointer();
  InternalPixelType *outData = output->GetBufferPointer();
  const unsigned long inLineOff = nBand * input->GetBufferedRegion().GetSize(0);
  const unsigned long outLineOff = nBand * output->GetBufferedRegion().GetSize(0);
  bool hasContiguousLines( sizeX == output->GetBufferedRegion().GetSize(0) );
  // Go to begin
  outData += nBand * output->ComputeOffset(index);
  if (curRoi.GetNumberOfPixels())
    {
    inData += nBand * input->ComputeOffset(curRoi.GetIndex());
    }

  itk::ProgressReporter progress( this, threadId, outputRegionForThread.GetNumberOfPixels() / sizeY );

  auto y = startY;
  if (hasContiguousLines)
    {
    // if output lines are contiguous, we can do one call to std::fill_n
    std::fill_n(outData, full_line*(thrY1-y), m_Pad);
    for (
        ; y < thrY1
        ; ++y, outData+=outLineOff )
      {
      progress.CompletedPixel(); // Completed...Line()
      }
    }
  else
    {
    for (
        ; y < thrY1
        ; ++y, outData+=outLineOff )
      {
      // If there is any trimming of first lines, the inData will
      // directly point to the right region. we shall not increment it!
      // otbMsgDevMacro("o(" << y << ") <-- 0");
      std::fill_n(outData, full_line, m_Pad);
      progress.CompletedPixel(); // Completed...Line()
      }
    }
  assert(y == thrY1);
  otbMsgDevMacro("Y in ["<<thrY1<<".."<<thrY2<<"[  <<--- Input");
  for (
      ; y < thrY2
      ; ++y, inData+=inLineOff,  outData+=outLineOff)
  {
    auto const t1 = std::fill_n(outData, nb_z_l, m_Pad);
    // If there is any trimming of first columns, the inData iterator
    // will directly point to the right region. we shall not apply an offset!
    auto const t2 = std::copy_n(inData, nb_c_m, t1);
    std::fill_n(t2, nb_z_r, m_Pad);
    progress.CompletedPixel(); // Completed...Line()
  }
  assert(y == thrY2);
  otbMsgDevMacro("Y in ["<<thrY2<<".."<<endY<<"[  <<--- 0");
  if (hasContiguousLines)
    {
    // if output lines are contiguous, we can do one call to std::fill_n
    std::fill_n(outData, full_line*(endY-y), m_Pad);
    for (
        ; y < endY
        ; ++y, outData+=outLineOff )
      {
      progress.CompletedPixel(); // Completed...Line()
      }
    }
  else
    {
    for (
        ; y < endY
        ; ++y,  outData+=outLineOff)
      {
      // If there is any trimming of last lines, the inputIterator iterator will
      // directly point to the right region. we shall not increment it!
      // otbMsgDevMacro("o(" << y << ") <-- 0");
      std::fill_n(outData, full_line, m_Pad);
      progress.CompletedPixel(); // Completed...Line()
      }
    }
  assert(y == endY);
  otbMsgDevMacro("ThreadedGenerateData end");
}

template<typename TImage>
typename ResetMarginFilter<TImage>::InputImageRegionType
ResetMarginFilter<TImage>
::OutputRegionToInputRegion(OutputImageRegionType const& srcRegion)
{
  auto curROI = m_ROI;
  if (!curROI.Crop(srcRegion))
    {
    curROI.SetIndex(srcRegion.GetIndex() + srcRegion.GetSize());
    curROI.SetSize({0,0});
    }
  return curROI;
}

} // otb namespace

#endif  // otbResetMarginFilter_hxx
