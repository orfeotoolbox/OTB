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

#ifndef otbPolarimetricFilters_h
#define otbPolarimetricFilters_h

// Generic polarimetry functors
#include "otbPolarimetryTags.h"
#include "otbFunctorImageFilter.h"

// Other reciprocal filters
#include "otbReciprocalCoherencyToReciprocalMuellerFunctor.h"
#include "otbReciprocalCovarianceToCoherencyDegreeFunctor.h"
#include "otbReciprocalCovarianceToReciprocalCoherencyFunctor.h"
#include "otbReciprocalLinearCovarianceToReciprocalCircularCovarianceFunctor.h"

namespace otb
{
//#########################
// Other reciprocal filters
//#########################

// This is the entire declaration of ReciprocalCoherencyToReciprocalMuellerImageFilter
template <typename TInputImage, typename TOutputImage>
using ReciprocalCoherencyToReciprocalMuellerImageFilter =
  FunctorImageFilter<Functor::
                     ReciprocalCoherencyToReciprocalMuellerFunctor<typename TInputImage::PixelType, typename TOutputImage::PixelType>>;

// This is the entire declaration of ReciprocalCovarianceToCoherencyDegreeImageFilter
template <typename TInputImage, typename TOutputImage>
using ReciprocalCovarianceToCoherencyDegreeImageFilter =
  FunctorImageFilter<Functor::ReciprocalCovarianceToCoherencyDegreeFunctor<typename TInputImage::PixelType, typename TOutputImage::PixelType>>;

// This is the entire declaration of ReciprocalCovarianceToReciprocalCoherencyImageFilter
template <typename TInputImage, typename TOutputImage>
using ReciprocalCovarianceToReciprocalCoherencyImageFilter =
  FunctorImageFilter<Functor::ReciprocalCovarianceToReciprocalCoherencyFunctor<typename TInputImage::PixelType, typename TOutputImage::PixelType>>;

// This is the entire declaration of ReciprocalLinearCovarianceToReciprocalCircularCovarianceImageFilter
template <typename TInputImage, typename TOutputImage>
using ReciprocalLinearCovarianceToReciprocalCircularCovarianceImageFilter =
  FunctorImageFilter<Functor::ReciprocalLinearCovarianceToReciprocalCircularCovarianceFunctor<typename TInputImage::PixelType, typename TOutputImage::PixelType>>;

} // end namespace otb

#endif
