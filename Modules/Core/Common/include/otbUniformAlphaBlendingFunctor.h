/*
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

#ifndef otbUniformAlphaBlendingFunctor_h
#define otbUniformAlphaBlendingFunctor_h

#include "itkMacro.h"
#include "itkNumericTraits.h"
#include "otbMath.h"

namespace otb
{
namespace Functor
{
/**
 * \class UniformAlphaBlendingFunctor
 * \brief Implements simple blending
 * For each channel the blending function is as follows:
 * \f[ P_{o} = \alpha P_{i1} + (1 - \alpha) P_{i2} \f]
 *
 * \ingroup OTBCommon
 */

template <class TInputPixel, class TInputPixel2, class TOutputPixel>
class ITK_EXPORT UniformAlphaBlendingFunctor
{
public:
  UniformAlphaBlendingFunctor() : m_Alpha(0.5)
  {
  }
  ~UniformAlphaBlendingFunctor()
  {
  }

  typedef TInputPixel                         InputPixelType;
  typedef TInputPixel2                        InputPixel2Type;
  typedef TOutputPixel                        OutputPixelType;
  typedef typename OutputPixelType::ValueType OutputInternalPixelType;

  void SetAlpha(double a)
  {
    // Keep alpha between 0 and 1
    m_Alpha = a < 0.0 ? 0.0 : (a > 1.0 ? 1.0 : a);
  }
  double GetAlpha() const
  {
    return m_Alpha;
  }

  inline OutputPixelType operator()(InputPixelType input1, InputPixel2Type input2) const
  {
    OutputPixelType resp;
    resp.Fill(itk::NumericTraits<OutputInternalPixelType>::max());
    resp.SetRed(static_cast<OutputInternalPixelType>(
        std::floor(m_Alpha * static_cast<double>(input1.GetRed()) + (1 - m_Alpha) * static_cast<double>(input2.GetRed()) + 0.5)));
    resp.SetGreen(static_cast<OutputInternalPixelType>(
        std::floor(m_Alpha * static_cast<double>(input1.GetGreen()) + (1 - m_Alpha) * static_cast<double>(input2.GetGreen()) + 0.5)));
    resp.SetBlue(static_cast<OutputInternalPixelType>(
        std::floor(m_Alpha * static_cast<double>(input1.GetBlue()) + (1 - m_Alpha) * static_cast<double>(input2.GetBlue()) + 0.5)));
    return resp;
  }

protected:
  double m_Alpha;
};
}
}
#endif
