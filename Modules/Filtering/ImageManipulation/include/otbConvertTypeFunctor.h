/*
 * Copyright (C) 2005-2021 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbConvertTypeFunctor_h
#define otbConvertTypeFunctor_h


#include "otbPixelComponentIterator.h"
#include "otbAlgoClamp.h"
#include "itkNumericTraits.h"
#include <boost/type_traits/is_complex.hpp>
#include <limits>
#include <type_traits>

namespace otb
{
namespace Functor
{

template <class TInputPixelType, class TOutputPixelType>
class ConvertTypeFunctor
{
public:
  using InputPixelType          = TInputPixelType;
  using OutputPixelType         = TOutputPixelType;
  using Self                    = ConvertTypeFunctor;

  using InputInternalPixelType  = typename itk::NumericTraits<InputPixelType>::ValueType;
  using OutputInternalPixelType = typename itk::NumericTraits<OutputPixelType>::ValueType;

  using InputPixelValueType     = typename itk::NumericTraits<InputInternalPixelType>::ValueType;
  using OutputPixelValueType    = typename itk::NumericTraits<OutputInternalPixelType>::ValueType;
  using ThresholdPixelValueType = std::common_type_t<InputPixelValueType, OutputPixelValueType>;

  static constexpr bool m_cInPix          = boost::is_complex<InputPixelType>::value;
  static constexpr bool m_cOutPix         = boost::is_complex<OutputPixelType>::value;
  static constexpr bool m_cInInternalPix  = boost::is_complex<InputInternalPixelType>::value;
  static constexpr bool m_cOutInternalPix = boost::is_complex<OutputInternalPixelType>::value;

  ConvertTypeFunctor() = default;
  ~ConvertTypeFunctor() = default;

  // template < class InternalPixelType  >
  void SetInputComponents(unsigned int sizeIn)
  {
    m_CompIn = sizeIn;
    if (m_cInPix)
    {
      // needed as ITK thinks that one complex component is actually
      // two components...
      m_CompIn /= 2;
    }

    if (m_cInInternalPix || m_cInPix)
      m_Scal = 2 * m_CompIn;
    else
      m_Scal = m_CompIn;

    OutputPixelType out;
    unsigned int    size = itk::NumericTraits<OutputPixelType>::GetLength(out);
    if (size == 0) // That means it is a variable size container
    {
      if (m_cOutInternalPix)
        m_CompOut = (m_Scal + 1) / 2;
      else
        m_CompOut = m_Scal;
    }
    // It is a fixed size container, m_CompOut should be equal to its size
    else if (m_cOutPix) // one complex is one component
      m_CompOut = 1;
    else // fized size container or scalar
      m_CompOut = size;
  }

  unsigned int GetOutputSize() const noexcept
  {
    return m_CompOut;
  }

  void SetLowest(OutputPixelValueType const& lowest) noexcept
  {
    m_LowestB  = lowest;
    m_Zero      = clamp<ThresholdPixelValueType>(m_Zero, lowest, m_HighestB);
  }

  void SetHighest(OutputPixelValueType const& highest) noexcept
  {
    m_HighestB  = highest;
    m_Zero      = clamp<ThresholdPixelValueType>(m_Zero, m_LowestB, highest);
  }

  void operator()(OutputPixelType & out, InputPixelType const& in) const
  {
    // PERF: Because VLV pixel may be used, this operator is written
    // with an output parameter instead of a returned value.

    auto r_out = PixelRange(out);
    auto r_in  = PixelRange(in);

    auto       first  = r_in.begin();  // Source iterators
    auto const end    = r_in.end();
    auto       dfirst = r_out.begin(); // D)estination iterators
    auto const dend   = r_out.end();

    static_assert(std::is_same<OutputPixelValueType, std::decay_t<decltype(*dfirst)>>::value, "They should match");
    // PERF: Locally cache the member variable as the compiler cannot
    auto const lo = m_LowestB;
    auto const hi = m_HighestB;
    // As std:transform stop condition is only on `end`, and not on both
    // `end` and `dend`, we loop manually
    for (; first!=end && dfirst!=dend ; ++first, ++dfirst)
    {
      // The conversion to OutputPixelValueType needs to be done after
      // clamping!
      // Indeed, let's suppose InputType=short, OutputType=unsigned char
      // clamp<OutputType>(300) would yield 300-256=44 instead of 255.
      // Hence the use of ThresholdPixelValueType which is the common
      // type between Input and Output PixelValueTypes.
      *dfirst = static_cast<OutputPixelValueType>(otb::clamp<ThresholdPixelValueType>(*first, lo, hi));
    }
    // complete with extra 0 (case where we have less input bands, and we store into complex)
#if 0
    // It seems to be slightly slower with assertions on
    std::fill(dfirst, dend, m_Zero);
#else
    for (; dfirst != dend ; ++dfirst)
    {
      *dfirst = m_Zero;
    }
#endif
  }

private:

  ConvertTypeFunctor(const Self&) = delete;
  void operator=(const Self&) = delete;

  ThresholdPixelValueType m_LowestB  = std::numeric_limits<OutputPixelValueType>::lowest();
  ThresholdPixelValueType m_HighestB = std::numeric_limits<OutputPixelValueType>::max();
  OutputPixelValueType m_Zero     {}; // initialized to zero!
  unsigned int         m_CompIn, m_CompOut, m_Scal;
};

} // end namespace Functor

} // end namespace otb

#endif
