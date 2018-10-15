/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#include "itkTernaryFunctorImageFilter.h"

namespace otb
{
namespace Functor
{
/** \class LevelingFunctor
 * \brief This functor performs the pixel-wise leveling operation needed in the
 * geodesic morphology decomposition filter. For more details, please refer to
 * the documentation of this filter.
 * \sa GeodesicMorphologyDecompositionImageFilter
 *
 * \ingroup OTBMorphologicalProfiles
 */
template <class TInput, class TInputMap, class TOutput>
class LevelingFunctor
{
public:
  /// Constructor
  LevelingFunctor() {}
  /// Destructor
  virtual ~LevelingFunctor() {}

  inline TOutput operator ()(const TInput& pixel, const TInputMap& convexPixel, const TInputMap& concavePixel)
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

/** \class GeodesicMorphologyLevelingFilter
 *  \brief This filter performs the leveling operation defined in the documentation of
 *  the geodesic decomposition image filter, given the original image, convex and concave membership
 *  functions. Please refer to the documentation of this filter for more details.
 *
 * \sa GeodesicMorphologyDecompositionImageFilter
 *
 * \ingroup OTBMorphologicalProfiles
 */
template <class TInputImage, class TInputMaps, class TOutputImage>
class ITK_EXPORT GeodesicMorphologyLevelingFilter
  : public itk::TernaryFunctorImageFilter<TInputImage, TInputImage,
      TInputImage, TOutputImage,
      Functor::LevelingFunctor<typename TInputImage::PixelType,
          typename TInputMaps::PixelType,
          typename TOutputImage::PixelType> >
{
public:
  /** Standard typedefs */
  typedef GeodesicMorphologyLevelingFilter Self;
  typedef   itk::TernaryFunctorImageFilter<TInputImage, TInputImage,
      TInputImage, TOutputImage,
      Functor::LevelingFunctor<typename TInputImage::PixelType,
          typename TInputMaps::PixelType,
          typename TOutputImage::PixelType> > Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(GeodesicMorphologyLevelingFilter, TernaryFunctorImageFilter);

  /**
   * Set the convex membership image.
   */
  void SetInputConvexMap(const TInputMaps * convexMap)
  {
    this->SetInput2(convexMap);
  }
  /**
   * Set the concave membership image.
   */
  void SetInputConcaveMap(const TInputMaps * concaveMap)
  {
    this->SetInput3(concaveMap);
  }
  /**
   * Set the original input image
   */
  using Superclass::SetInput;
  void SetInput(const TInputImage * input) override
  {
    this->SetInput1(input);
  }

protected:
  /** Constructor */
  GeodesicMorphologyLevelingFilter() {};
  /** Destructor */
  ~GeodesicMorphologyLevelingFilter() override {}
  /**PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override
  {
    Superclass::PrintSelf(os, indent);
  }

private:
  GeodesicMorphologyLevelingFilter(const Self &) = delete;
  void operator =(const Self&) = delete;
};
} // End namespace otb
#endif
