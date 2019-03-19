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

#ifndef otbProjectiveProjectionImageFilter_h
#define otbProjectiveProjectionImageFilter_h

#include "itkMacro.h"
#include "otbFunctorImageFilter.h"

namespace otb
{

namespace Functor {

/** \class ProjectiveProjectionFunctor
 *
 * \brief Applies a projective projection to a pixel
 *
 * \sa ProjectiveProjectionImageFilter
 *
 * \ingroup OTBProjection
 */
template<class TInput, class TOutput, class TPrecision>
class ProjectiveProjectionFunctor
{
public:
  typedef TInput     InputType;
  typedef TOutput    OutputType;
  typedef TPrecision PrecisionType;

  ProjectiveProjectionFunctor() : m_OutputSize(0) {}
  virtual ~ProjectiveProjectionFunctor() {}

  size_t OutputSize(const std::array<size_t, 1>& nbBands) const;

  const InputType& GetProjectionDirection();

  void SetProjectionDirection(const InputType& p);

  OutputType operator()(const InputType& in);

private:
  unsigned int   m_OutputSize;
  InputType      m_ProjectionDirection;
};
}

/** \class ProjectiveProjectionImageFilter
 *
 * \brief Applies a projective projection to each pixel of an image
 *
 * \ingroup Streamed
 * \ingroup Threaded
 *
 * \ingroup OTBProjection
 */
template <typename TInputImage, typename TOutputImage, typename TPrecision>
using ProjectiveProjectionImageFilter =
    FunctorImageFilter<Functor::ProjectiveProjectionFunctor<typename TInputImage::PixelType, typename TOutputImage::PixelType, TPrecision>>;

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbProjectiveProjectionImageFilter.hxx"
#endif

#endif

