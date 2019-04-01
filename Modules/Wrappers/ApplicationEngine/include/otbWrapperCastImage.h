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

#ifndef otbWrapperCastimage_h
#define otbWrapperCastimage_h


#include "otbClampImageFilter.h"

#include "OTBApplicationEngineExport.h"

#include "otbWrapperTypes.h"


namespace otb
{
namespace Wrapper
{
namespace details
{

/** \class CastImage
 *  \brief Helper class (private) which casts and clamps input-image type into
 *  output-image type.
 *
 * \ingroup OTBApplicationEngine
 */
template <typename TOutputImage, typename TInputImage>
struct OTBApplicationEngine_EXPORT_TEMPLATE CastImage
{
  /** Input clamping */
  using InputClampImageFilter = ClampImageFilter<TInputImage, DoubleVectorImageType>;

  /** Output clamping */
  using OutputClampImageFilter = ClampImageFilter<DoubleVectorImageType, TOutputImage>;


  /** Constructor. */
  CastImage(TInputImage* in) : icif(InputClampImageFilter::New()), ocif(OutputClampImageFilter::New()), out(ocif->GetOutput())
  {
    assert(in);

    icif->SetInput(in);

    ocif->SetInput(icif->GetOutput());
  }

  /** Input-image clamp filter. */
  typename InputClampImageFilter::Pointer icif;

  /** Output-image clamp filter. */
  typename OutputClampImageFilter::Pointer ocif;

  /** Output image. */
  TOutputImage* out;
};


/** \class CastImage
 *  \brief Partial template specialization which optimizes processing
 * pipeline when input-image is DoubleVectorImageType.
 *
 * \ingroup OTBApplicationEngine
 */
template <typename TOutputImage>
struct OTBApplicationEngine_EXPORT_TEMPLATE CastImage<TOutputImage, DoubleVectorImageType>
{
  /** Output clamping */
  using OutputClampImageFilter = ClampImageFilter<DoubleVectorImageType, TOutputImage>;


  /** Constructor. */
  CastImage(DoubleVectorImageType* in) : ocif(OutputClampImageFilter::New()), out(ocif->GetOutput())
  {
    assert(in);

    ocif->SetInput(in);
  }

  /** Input-image clamp filter. */
  itk::ProcessObject::Pointer icif;

  /** Output-image clamp filter. */
  typename OutputClampImageFilter::Pointer ocif;

  /** Output image. */
  TOutputImage* out;
};


/** \class CastImage
 *  \brief Template specialization which optimizes the processing
 *  pipeline when input-image and output-image types are identical.
 *
 * \ingroup OTBApplicationEngine
 */
template <typename T>
struct OTBApplicationEngine_EXPORT_TEMPLATE CastImage<T, T>
{
  CastImage(T* in) : out(in)
  {
    assert(in);
  }

  itk::ProcessObject::Pointer icif;
  itk::ProcessObject::Pointer ocif;
  T*                          out;
};


/** \class CastImage
 *  \brief Template specialization which optimizes the processing
 *  pipeline when input-image and output-image types are identical.
 *
 * \ingroup OTBApplicationEngine
 */
template <>
struct OTBApplicationEngine_EXPORT_TEMPLATE CastImage<DoubleVectorImageType, DoubleVectorImageType>
{
  CastImage(DoubleVectorImageType* in) : out(in)
  {
    assert(in);
  }

  itk::ProcessObject::Pointer icif;
  itk::ProcessObject::Pointer ocif;
  DoubleVectorImageType*      out;
};

} // namespace details.

} // namespace Wrapper

} // namespace otb

#endif
