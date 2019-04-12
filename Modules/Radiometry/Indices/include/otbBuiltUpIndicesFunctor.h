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

#ifndef otbBuiltUpIndicesFunctor_h
#define otbBuiltUpIndicesFunctor_h

#include "otbRadiometricIndice.h"

namespace otb
{
namespace Functor
{
/**
   * \class TM4AndTM5IndexBase
   * \brief Base class for TM4 And TM5 channels of Land Sat
   * (equivalent to Red and NIR of SPOT5)
*/

/** \class NDBI
 *  \brief This functor computes the Normalized Difference Built Up Index (NDBI)
 *  Band mapping with respect to paper:
 *  TM4 = red
 *  TM5 = nir
 *  [Zha 2003]
 *
 *  \ingroup Functor
 * \ingroup Radiometry
 *
 * \ingroup OTBIndices
 */
template <class TInput, class TOutput>
class NDBI : public RadiometricIndice<TInput,TOutput>
{
public:
  NDBI() : RadiometricIndice<TInput,TOutput>({CommonBandNames::RED, CommonBandNames::NIR}) {}

  TOutput operator()(const itk::VariableLengthVector<TInput> & input) const override
  {
    auto red = this->Value(CommonBandNames::RED,input);
    auto nir = this->Value(CommonBandNames::NIR,input);

    if (std::abs(red+nir) < RadiometricIndice<TInput,TOutput>::Epsilon)
      {
      return static_cast<TOutput>(0.);
      }

    // TODO: Completely equivalent to NDVI ?
    return (static_cast<TOutput>((nir - red) / (nir + red)));
  }
};

/** \class ISU
 *  \brief This functor computes the Index surfaces built (ISU)
 *
 *  [Abdellaoui 1997 ]
 *
 *  \ingroup Functor
 * \ingroup Radiometry
 *
 * \ingroup OTBIndices
 */
template <class TInput, class TOutput>
class ISU : public RadiometricIndice<TInput,TOutput>
{
public:
  ISU()  : RadiometricIndice<TInput,TOutput>({CommonBandNames::RED, CommonBandNames::NIR}) {}

  TOutput operator()(const itk::VariableLengthVector<TInput> & input) const override
  {
    auto red = this->Value(CommonBandNames::RED,input);
    auto nir = this->Value(CommonBandNames::NIR,input);

    if (nir == 0)
      {
      return static_cast<TOutput>(0.);
      }

    return (static_cast<TOutput>(A - (B * red) / nir));
  }

  static constexpr double A = 100.;
  static constexpr double B = 25.;
};

} // namespace Functor
} // namespace otb

#endif
