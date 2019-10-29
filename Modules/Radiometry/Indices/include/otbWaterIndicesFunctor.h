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

#ifndef otbWaterIndicesFunctor_h
#define otbWaterIndicesFunctor_h

#include "otbMath.h"
#include "otbRadiometricIndex.h"

namespace otb
{
namespace Functor
{
/** \class NDWI
 *  \brief This functor computes the Normalized Difference Water Index (NDWI)
 *  \brief Also called :
 *  \brief NDII (Normalized Difference Infrared Index)
 *  \brief LSWI (Land Surface Water Index)
 *  \brief NDWI (Normalized Difference Moisture Index)
 *
 *  [Gao, 1996 ]
 *
 *  \ingroup Functor
 * \ingroup Radiometry
 *
 * \ingroup OTBIndices
 */
template <class TInput, class TOutput>
class NDWI : public RadiometricIndex<TInput, TOutput>
{
public:
  NDWI() : RadiometricIndex<TInput, TOutput>({CommonBandNames::NIR, CommonBandNames::MIR})
  {
  }

  TOutput operator()(const itk::VariableLengthVector<TInput>& input) const override
  {
    auto mir = this->Value(CommonBandNames::MIR, input);
    auto nir = this->Value(CommonBandNames::NIR, input);

    if (std::abs(nir + mir) < RadiometricIndex<TInput, TOutput>::Epsilon)
    {
      return 0.;
    }

    return (nir - mir) / (nir + mir);
  }
};

/** \class NDWI2
 *  \brief This functor computes the Normalized Difference Water Index (NDWI2)
 *
 *  [McFeeters, 1996 ]
 *
 *  \ingroup Functor
 * \ingroup Radiometry
 *
 * \ingroup OTBIndices
 */
template <class TInput, class TOutput>
class NDWI2 : public RadiometricIndex<TInput, TOutput>
{
public:
  NDWI2() : RadiometricIndex<TInput, TOutput>({CommonBandNames::NIR, CommonBandNames::GREEN})
  {
  }

  TOutput operator()(const itk::VariableLengthVector<TInput>& input) const override
  {
    auto green = this->Value(CommonBandNames::GREEN, input);
    auto nir   = this->Value(CommonBandNames::NIR, input);

    if (std::abs(nir + green) < RadiometricIndex<TInput, TOutput>::Epsilon)
    {
      return 0.;
    }

    return (green - nir) / (green + nir);
  }
};

/** \class MNDWI
 *  \brief This functor computes the Modified Normalized Difference Water Index (MNDWI)
 *
 *  [Xu & al., 2006 ]
 *
 * Similar to Normalized Difference Pond Index (NDPI)
 *
 *  [J.P Lacaux & al., 2006 ]
 *
 *  \ingroup Functor
 * \ingroup Radiometry
 *
 * \ingroup OTBIndices
 */
template <class TInput, class TOutput>
class MNDWI : public RadiometricIndex<TInput, TOutput>
{
public:
  MNDWI() : RadiometricIndex<TInput, TOutput>({CommonBandNames::MIR, CommonBandNames::GREEN})
  {
  }

  TOutput operator()(const itk::VariableLengthVector<TInput>& input) const override
  {
    auto green = this->Value(CommonBandNames::GREEN, input);
    auto mir   = this->Value(CommonBandNames::MIR, input);

    if (std::abs(mir + green) < RadiometricIndex<TInput, TOutput>::Epsilon)
    {
      return 0.;
    }

    return (green - mir) / (green + mir);
  }
};

/** \class NDTI
 *  \brief This functor computes the Normalized Difference Turbidity Index (NDTI)
 *
 *  [J.P Lacaux & al., 2006 ]
 *
 *  \ingroup Functor
 * \ingroup Radiometry
 *
 * \ingroup OTBIndices
 */
template <class TInput, class TOutput>
class NDTI : public RadiometricIndex<TInput, TOutput>
{
public:
  NDTI() : RadiometricIndex<TInput, TOutput>({CommonBandNames::RED, CommonBandNames::GREEN})
  {
  }

  TOutput operator()(const itk::VariableLengthVector<TInput>& input) const override
  {
    auto green = this->Value(CommonBandNames::GREEN, input);
    auto red   = this->Value(CommonBandNames::RED, input);

    if (std::abs(red + green) < RadiometricIndex<TInput, TOutput>::Epsilon)
    {
      return 0.;
    }

    return (red - green) / (green + red);
  }
};

} // namespace Functor
} // namespace otb

#endif
