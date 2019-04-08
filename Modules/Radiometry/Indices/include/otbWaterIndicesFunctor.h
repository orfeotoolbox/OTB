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
#include "otbRadiometricIndice.h"

namespace otb
{
namespace Functor
{

/** \class SRWI
 *  \brief This functor computes the Simple Ratio Water Index (SRWI)
 *  \brief For MODIS bands 860 & 1240
 *
 *   [Zarco-Tejada 2001]
 *
 *  \ingroup Functor
 * \ingroup Radiometry
 *
 * \ingroup OTBIndices
 */
template <class TInput, class TOutput>
class SRWI : public RadiometricIndice<TInput,TOutput>
{
public:

  SRWI() : RadiometricIndice<TInput,TOutput>("SRWI",{Band::MODIS_860, Band::MODIS_1240}) {}

  TOutput operator()(const itk::VariableLengthVector<TInput> & input) const override
  {
    double rho860 = this->Value(Band::MODIS_860,input);
    double rho1240 = this->Value(Band::MODIS_1240,input);
    if (std::abs(rho1240) < EpsilonToBeConsideredAsZero)
      {
      return static_cast<TOutput>(0.);
      }
    return (static_cast<TOutput>(rho860 / rho1240));
  }
};

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
class NDWI : public RadiometricIndice<TInput,TOutput>
{
public:

  NDWI(): RadiometricIndice<TInput,TOutput>("NDWI",{Band::NIR, Band::MIR}) {}

  TOutput operator()(const itk::VariableLengthVector<TInput> & input) const override
  {
    auto mir   = this->Value(Band::MIR,input);
    auto nir   = this->Value(Band::NIR,input);

        if (std::abs(nir + mir) < EpsilonToBeConsideredAsZero)
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
class NDWI2 : public RadiometricIndice<TInput,TOutput>
{
public:

  NDWI2() : RadiometricIndice<TInput,TOutput>("NDWI2",{Band::NIR, Band::GREEN}) {}

  TOutput operator()(const itk::VariableLengthVector<TInput> & input) const override
  {
    auto green   = this->Value(Band::GREEN,input);
    auto nir   = this->Value(Band::NIR,input);

    if (std::abs(nir + green) < EpsilonToBeConsideredAsZero)
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
 *  \ingroup Functor
 * \ingroup Radiometry
 *
 * \ingroup OTBIndices
 */
template <class TInput, class TOutput>
class MNDWI : public RadiometricIndice<TInput,TOutput>
{

  MNDWI() : RadiometricIndice<TInput,TOutput>("MNDWI",{Band::MIR, Band::GREEN}) {}

  TOutput operator()(const itk::VariableLengthVector<TInput> & input) const override
  {
    auto green   = this->Value(Band::GREEN,input);
    auto mir   = this->Value(Band::MIR,input);

    if (std::abs(mir + green) < EpsilonToBeConsideredAsZero)
      {
      return 0.;
      }

    return (green - mir) / (green + mir);
  }
};

/** \class NDPI
 *  \brief This functor computes the Normalized Difference Pond Index (NDPI)
 *
 *  [J.P Lacaux & al., 2006 ]
 *
 *  \ingroup Functor
 * \ingroup Radiometry
 *
 * \ingroup OTBIndices
 */
template <class TInput, class TOutput>
class NDPI : public RadiometricIndice<TInput,TOutput>
{
public:
  NDPI() : RadiometricIndice<TInput,TOutput>("NDPI",{Band::MIR, Band::GREEN}) {}

  TOutput operator()(const itk::VariableLengthVector<TInput> & input) const override
  {
    auto green   = this->Value(Band::GREEN,input);
    auto mir   = this->Value(Band::MIR,input);

    if (std::abs(mir + green) < EpsilonToBeConsideredAsZero)
      {
      return 0.;
      }
    // TODO: Completely equivalent to MNDWI ?
    return (mir -green) / (green + mir);
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
class NDTI : public RadiometricIndice<TInput,TOutput>
{
public:

  NDTI() : RadiometricIndice<TInput,TOutput>("NDPI",{Band::RED, Band::GREEN}) {}

  TOutput operator()(const itk::VariableLengthVector<TInput> & input) const override
  {
    auto green = this->Value(Band::GREEN,input);
    auto red   = this->Value(Band::RED,input);

    if (std::abs(red + green) < EpsilonToBeConsideredAsZero)
      {
      return 0.;
      }

    return (red -green) / (green + red);
  }
};

} // namespace Functor
} // namespace otb

#endif
