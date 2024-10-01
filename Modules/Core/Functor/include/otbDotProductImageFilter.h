/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbDotProductImageFilter_h
#define otbDotProductImageFilter_h

#include "itkMacro.h"
#include "otbFunctorImageFilter.h"

namespace otb
{

namespace Functor
{

/** \class DotProductFunctor
 *
 * \brief Computes the dot product against a specific vector
 *
 * \ingroup OTBFunctor
 */
template <class TInput, class TOutput>
class DotProductFunctor
{
public:
  typedef TInput  InputType;
  typedef TOutput OutputType;

  DotProductFunctor()          = default;
  virtual ~DotProductFunctor() = default;

  const InputType& GetVector()
  {
    return m_Vector;
  }

  void SetVector(const InputType& m)
  {
    m_Vector = m;
  }

  OutputType operator()(const InputType& in)
  {
    assert(in.Size() == m_Vector.Size());
    OutputType result = 0;
    for (unsigned int i = 0; i < in.Size(); ++i)
    {
      result += in[i] * m_Vector[i];
    }
    return result;
  }

private:
  InputType m_Vector;
};
} // namespace Functor

/** \typedef DotProductImageFilter
 *
 * \brief Applies pixel-wise dot product to a VectorImage
 *
 * Given a vector, this filter outputs the dot product of each pixel of a multiband image
 * with respect to the specified vector
 *
 * \sa otb::Functor::DotProductFunctor
 *
 * \ingroup Streamed
 * \ingroup Threaded
 *
 * \ingroup OTBFunctor
 */
template <typename TInputImage, typename TOutputImage>
using DotProductImageFilter = FunctorImageFilter<Functor::DotProductFunctor<typename TInputImage::PixelType, typename TOutputImage::PixelType>>;

} // namespace otb
#endif
