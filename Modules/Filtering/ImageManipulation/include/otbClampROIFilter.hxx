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

#ifndef otbClampROIFilter_hxx
#define otbClampROIFilter_hxx

#include "otbClampROIFilter.h"
#include "otbInterval.h"
#include "otbMacro.h"
#include "otbLogHelpers.h"
#include "itkImageScanlineConstIterator.h"
#include "itkImageScanlineIterator.h"
#include "itkProgressReporter.h"
#include <boost/numeric/interval.hpp>
#include <algorithm>
#include <cassert>
#include <ostream>

template <typename T, typename P>
inline
std::ostream & operator<<(std::ostream & os, boost::numeric::interval<T,P> const& v)
{
  return os << '[' << v.lower() << ".." << v.upper() << '[';
}

namespace otb
{

template<typename TImage>
void
ClampROIFilter<TImage>
::ThreadedGenerateData(
    OutputImageRegionType const& outputRegionForThread,
    itk::ThreadIdType            threadId)
{
  // otbMsgDevMacro("ThreadedGenerateData begin("<<NeatRegionLogger{outputRegionForThread}<<")");
  using InputIterator   = itk::ImageScanlineConstIterator<InputImageType const>;
  using OutputIterator  = itk::ImageScanlineIterator<OutputImageType>;

  auto const* input  = this->GetInput();
  auto      * output = this->GetOutput();
  assert(input);
  assert(output);
  InputIterator  inputIterator (input,  OutputRegionToInputRegion(outputRegionForThread));
  OutputIterator outputIterator(output, outputRegionForThread);

  auto const& imgRegion = output->GetLargestPossibleRegion();
  auto const  imgSizeX  = imgRegion.GetSize()[0];
  auto const  imgSizeY  = imgRegion.GetSize()[1];
  itk::IndexValueType const  imgEndX   = imgRegion.GetIndex()[0] + imgSizeX;
  itk::IndexValueType const  imgEndY   = imgRegion.GetIndex()[1] + imgSizeY;

  auto const& size      = outputRegionForThread.GetSize();
  auto const& index     = outputRegionForThread.GetIndex();
  auto const  sizeX     = size[0];
  auto const  sizeY     = size[1];
  auto const  startX    = index[0];
  auto const  startY    = index[1];
  itk::IndexValueType const  endX      = startX + sizeX;
  itk::IndexValueType const  endY      = startY + sizeY;
  auto const  thrX1     = std::min<itk::IndexValueType>(endX, m_thresholdX);
  auto const  thrX2     = std::min<itk::IndexValueType>(endX, imgEndX - m_thresholdX);
  auto const  thrY1     = std::min<itk::IndexValueType>(endY, m_thresholdYtop);
  auto const  thrY2     = std::min<itk::IndexValueType>(endY, imgEndY - m_thresholdYbot);

  assert(thrX1 <= endX && "Iterations shall stay within requested region");
  assert(thrX2 <= endX && "Iterations shall stay within requested region");
  assert(thrY1 <= endY && "Iterations shall stay within requested region");
  assert(thrY2 <= endY && "Iterations shall stay within requested region");

  // using interval_t = boost::numeric::interval<long>;
  using interval_t = Interval;
  auto const region      = interval_t{startX, endX};
  auto const zero_left   = intersect(interval_t{startX, thrX1}, region);
  auto const copy_middle = intersect(interval_t{thrX1, thrX2},  region);
  auto const zero_right  = intersect(interval_t{thrX2, endX},   region);
  otbMsgDevMacro("X in " << zero_left   << " <<-- 0");
  otbMsgDevMacro("X in " << copy_middle << " <<-- copy input");
  otbMsgDevMacro("X in " << zero_right  << " <<-- 0");
  otbMsgDevMacro("Y in ["<<startY<<".."<<thrY1<<"[  <<--- 0");

  auto const nb_z_l = zero_left.upper()   - zero_left.lower();
  auto const nb_c_m = copy_middle.upper() - copy_middle.lower();
  auto const nb_z_r = zero_right.upper()  - zero_right.lower();
  assert(nb_z_l + nb_c_m + nb_z_r == sizeX);

  itk::ProgressReporter progress( this, threadId, outputRegionForThread.GetNumberOfPixels() / sizeY );
  outputIterator.GoToBegin();

  // TODO: Can we consider that lines are contiguous in memory?
  // If so, we could have only 2 fill_n and one copy_n!
  auto y = startY;
  for (
      ; y < thrY1
      ; ++y, outputIterator.NextLine())
  {
    // If there is any trimming of first lines, the inputIterator iterator will
    // directly point to the right region. we shall not increment it!
    // otbMsgDevMacro("o(" << y << ") <-- 0");
    assert(! outputIterator.IsAtEnd());
    outputIterator.GoToBeginOfLine();
    std::fill_n(&outputIterator.Value(), sizeX, OutputPixelType{});
    progress.CompletedPixel(); // Completed...Line()
  }
  assert(y == thrY1 || y == startY);
  otbMsgDevMacro("Y in ["<<thrY1<<".."<<thrY2<<"[  <<--- Input");
  inputIterator.GoToBegin();
  for (
      ; y < thrY2
      ; ++y, inputIterator.NextLine(), outputIterator.NextLine())
  {
    // otbMsgDevMacro("o(" << y << ") <-- Input");
    assert(! inputIterator.IsAtEnd());
    assert(! outputIterator.IsAtEnd());
    inputIterator.GoToBeginOfLine();
    outputIterator.GoToBeginOfLine();
    auto const t1 = std::fill_n(&outputIterator.Value(), nb_z_l, OutputPixelType{});
    // If there is any trimming of first columns, the inputIterator iterator
    // will directly point to the right region. we shall not apply an offset!
    auto const t2 = std::copy_n(&inputIterator.Value(), nb_c_m, t1);
    std::fill_n(t2, nb_z_r, OutputPixelType{});
    progress.CompletedPixel(); // Completed...Line()
  }
  assert(y == thrY2 || y == startY);
  otbMsgDevMacro("Y in ["<<thrY2<<".."<<endY<<"[  <<--- 0");
  for (
      ; y < endY
      ; ++y, outputIterator.NextLine())
  {
    // If there is any trimming of last lines, the inputIterator iterator will
    // directly point to the right region. we shall not increment it!
    // otbMsgDevMacro("o(" << y << ") <-- 0");
    assert(! outputIterator.IsAtEnd());
    outputIterator.GoToBeginOfLine();
    std::fill_n(&outputIterator.Value(), sizeX, OutputPixelType{});
    progress.CompletedPixel(); // Completed...Line()
  }
  assert(y == endY);
  otbMsgDevMacro("ThreadedGenerateData end");
}

template<typename TImage>
typename ClampROIFilter<TImage>::InputImageRegionType
ClampROIFilter<TImage>
::OutputRegionToInputRegion(OutputImageRegionType const& srcRegion)
{
  auto const* output = this->GetOutput();
  assert(output);

  auto const& maxRegion = output->GetLargestPossibleRegion();
  auto const& maxSize   = maxRegion.GetSize();
  auto const& maxStart  = maxRegion.GetIndex();

  auto const& reqRegion = srcRegion;
  auto const& reqSize   = reqRegion.GetSize();
  auto const& reqStart  = reqRegion.GetIndex();

  // using interval_t = boost::numeric::interval<long>;
  using interval_t = Interval;
  auto const maxRegionX = interval_t{
    maxStart[0]+m_thresholdX,
    static_cast<itk::IndexValueType>(maxStart[0]+maxSize[0]-m_thresholdX)
  };
  auto const maxRegionY = interval_t{
    maxStart[1]+m_thresholdYtop,
    static_cast<itk::IndexValueType>(maxStart[1]+maxSize[1]-m_thresholdYbot)
  };

  auto const reqRegionX = interval_t::OfLength(reqStart[0], reqSize[0]);
  auto const reqRegionY = interval_t::OfLength(reqStart[1], reqSize[1]);
#if 0
  otbMsgDevMacro("OutputRegionToInputRegion: "
      << "out="<< NeatRegionLogger{reqRegion}
      << ";    max: x="<<maxRegionX << "  y="<<maxRegionY
      << ";    req: x="<<reqRegionX << "  y="<<reqRegionY
      );
#endif

  auto const inRegionX = intersect(reqRegionX, maxRegionX);
  auto const inRegionY = intersect(reqRegionY, maxRegionY);
  // otbMsgDevMacro(" --> ∩X: " << inRegionX << " ∩Y: " << inRegionY);

  const InputIndexType inStart{inRegionX.lower(), inRegionY.lower()};
  assert(inRegionX.lower() <= inRegionX.upper());
  assert(inRegionY.lower() <= inRegionY.upper());
  const InputSizeType inSize{
    static_cast<unsigned long>(inRegionX.upper()-inRegionX.lower()),
    static_cast<unsigned long>(inRegionY.upper()-inRegionY.lower())
  };
  auto const inRegion = InputImageRegionType{inStart, inSize};
  otbMsgDevMacro("OutputRegionToInputRegion: out="<< NeatRegionLogger{reqRegion}<<"   --> in="<<NeatRegionLogger{inRegion});
  return inRegion;
}

} // otb namespace

#endif  // otbClampROIFilter_hxx

