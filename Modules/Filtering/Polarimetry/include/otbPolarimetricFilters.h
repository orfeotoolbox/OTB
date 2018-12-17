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

// Bistatic functors
#include "otbSinclairToCovarianceMatrixFunctor.h"
#include "otbSinclairToCircularCovarianceMatrixFunctor.h"
#include "otbSinclairToCoherencyMatrixFunctor.h"
#include "otbSinclairToMuellerMatrixFunctor.h"

// Monostatic functors
#include "otbSinclairToReciprocalCoherencyMatrixFunctor.h"
#include "otbSinclairToReciprocalCovarianceMatrixFunctor.h"
#include "otbSinclairToReciprocalCircularCovarianceMatrixFunctor.h"

// Decomposition functors
#include "otbReciprocalBarnesDecompFunctor.h"
#include "otbReciprocalHAlphaImageFunctor.h"
#include "otbReciprocalHuynenDecompFunctor.h"
#include "otbReciprocalPauliDecompFunctor.h"

// Mueller functor
#include "otbMuellerToPolarisationDegreeAndPowerFunctor.h"
#include "otbMuellerToReciprocalCovarianceFunctor.h"

// Other reciprocal filters
#include "otbReciprocalCoherencyToReciprocalMuellerFunctor.h"
#include "otbReciprocalCovarianceToCoherencyDegreeFunctor.h"
#include "otbReciprocalCovarianceToReciprocalCoherencyFunctor.h"
#include "otbReciprocalLinearCovarianceToReciprocalCircularCovarianceFunctor.h"

namespace otb
{

//###################
// Monostatic filters
//###################

// This is the entire declaration of SinclairToReciprocalCoherencyMatrixFilter
template <typename TInputImage, typename TOutputImage>
using SinclairToReciprocalCoherencyMatrixFilter =
    FunctorImageFilter<Functor::SinclairToReciprocalCoherencyMatrixFunctor<typename TInputImage::PixelType, typename TInputImage::PixelType,
                                                                           typename TInputImage::PixelType, typename TOutputImage::PixelType>,
                       std::tuple<polarimetry_tags::hh, polarimetry_tags::hv_or_vh, polarimetry_tags::vv>>;

// This is the entire declaration of SinclairToReciprocalCoherencyMatrixFilter
template <typename TInputImage, typename TOutputImage>
using SinclairToReciprocalCovarianceMatrixFilter =
    FunctorImageFilter<Functor::SinclairToReciprocalCovarianceMatrixFunctor<typename TInputImage::PixelType, typename TInputImage::PixelType,
                                                                            typename TInputImage::PixelType, typename TOutputImage::PixelType>,
                       std::tuple<polarimetry_tags::hh, polarimetry_tags::hv_or_vh, polarimetry_tags::vv>>;

// This is the entire declaration of SinclairToReciprocalCircularCovarianceMatrixFilter
template <typename TInputImage, typename TOutputImage>
using SinclairToReciprocalCircularCovarianceMatrixFilter =
    FunctorImageFilter<Functor::SinclairToReciprocalCircularCovarianceMatrixFunctor<typename TInputImage::PixelType, typename TInputImage::PixelType,
                                                                                    typename TInputImage::PixelType, typename TOutputImage::PixelType>,
                       std::tuple<polarimetry_tags::hh, polarimetry_tags::hv_or_vh, polarimetry_tags::vv>>;


//############################
// Polarimetric decompositions
//############################

// This is the entire declaration of ReciprocalHAlphaImageFilter
template <typename TInputImage, typename TOutputImage>
using ReciprocalHAlphaImageFilter =
  FunctorImageFilter<Functor::ReciprocalHAlphaFunctor<typename TInputImage::PixelType, typename TOutputImage::PixelType>>;

// This is the entire declaration of ReciprocalBarnesDecompImageFilter
template <typename TInputImage, typename TOutputImage>
using ReciprocalBarnesDecompImageFilter =
  FunctorImageFilter<Functor::ReciprocalBarnesDecompFunctor<typename TInputImage::PixelType, typename TOutputImage::PixelType>>;

// This is the entire declaration of ReciprocalHuynenDecompImageFilter
template <typename TInputImage, typename TOutputImage>
using ReciprocalHuynenDecompImageFilter =
  FunctorImageFilter<Functor::ReciprocalHuynenDecompFunctor<typename TInputImage::PixelType, typename TOutputImage::PixelType>>;

// This is the entire declaration of ReciprocalPauliDecompImageFilter
template <typename TInputImage, typename TOutputImage>
using ReciprocalPauliDecompImageFilter =
  FunctorImageFilter<Functor::ReciprocalPauliDecompFunctor<typename TInputImage::PixelType, typename TOutputImage::PixelType>>;

//################
// Mueller filters
//################

// This is the entire declaration of MuellerToPolarisationDegreeAndPowerFilter
template <typename TInputImage, typename TOutputImage>
using MuellerToPolarisationDegreeAndPowerImageFilter =
  FunctorImageFilter<Functor::MuellerToPolarisationDegreeAndPowerFunctor<typename TInputImage::PixelType, typename TOutputImage::PixelType>>;

// This is the entire declaration of MuellerToReciprocalCovarianceImageFilter
template <typename TInputImage, typename TOutputImage>
using MuellerToReciprocalCovarianceImageFilter =
  FunctorImageFilter<Functor::MuellerToReciprocalCovarianceFunctor<typename TInputImage::PixelType, typename TOutputImage::PixelType>>;

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
