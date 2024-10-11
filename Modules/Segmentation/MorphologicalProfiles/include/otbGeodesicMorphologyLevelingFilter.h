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

#ifndef otbGeodesicMorphologyLevelingFilter_h
#define otbGeodesicMorphologyLevelingFilter_h

#include "otbFunctorImageFilter.h"

namespace otb
{
namespace Functor
{
namespace LevelingFunctor_tags
{
struct pixel
{
};
struct convex_pixel
{
};
struct concave_pixel
{
};
}
/** \class LevelingFunctor
 * \brief This functor performs the pixel-wise leveling operation needed in the
 * geodesic morphology decomposition filter. For more details, please refer to
 * the documentation of this filter.
 *
 * Use otb::GeodesicMorphologyLevelingFilter to apply it image-wise.
 *
 * \sa GeodesicMorphologyDecompositionImageFilter
 *
 * \ingroup OTBMorphologicalProfiles
 */
template <class TInput, class TInputMap, class TOutput>
class LevelingFunctor
{
public:
  /// Constructor
  LevelingFunctor()
  {
  }
  /// Destructor
  virtual ~LevelingFunctor()
  {
  }

  inline TOutput operator()(const TInput& pixel, const TInputMap& convexPixel, const TInputMap& concavePixel) const
  {
    TOutput result;

    if (convexPixel > concavePixel)
    {
      result = static_cast<TOutput>(pixel - convexPixel);
    }
    else if (convexPixel < concavePixel)
    {
      result = static_cast<TOutput>(concavePixel + pixel);
    }
    else
    {
      result = static_cast<TOutput>(pixel);
    }
    return result;
  }
};
} // end namespace Functor

/** \typedef GeodesicMorphologyLevelingFilter
 *  \brief This filter performs the leveling operation defined in the documentation of
 *  the geodesic decomposition image filter, given the original image, convex and concave membership
 *  functions. Please refer to the documentation of this filter for more details.
 *
 * It applies the Functor::LevelingFunctor image-wise.
 *
 * \sa GeodesicMorphologyDecompositionImageFilter
 *
 * \ingroup OTBMorphologicalProfiles
 */

template <class TInputImage, class TInputMaps, class TOutputImage>
using GeodesicMorphologyLevelingFilter =
    FunctorImageFilter<Functor::LevelingFunctor<typename TInputImage::PixelType, typename TInputMaps::PixelType, typename TOutputImage::PixelType>,
                       std::tuple<typename Functor::LevelingFunctor_tags::pixel, typename Functor::LevelingFunctor_tags::convex_pixel,
                                  typename Functor::LevelingFunctor_tags::concave_pixel>>;
} // End namespace otb
#endif
