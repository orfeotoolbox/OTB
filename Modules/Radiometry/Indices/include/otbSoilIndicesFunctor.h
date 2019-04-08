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

#ifndef otbSoilIndicesFunctor_h
#define otbSoilIndicesFunctor_h

#include "otbMath.h"
#include "otbRadiometricIndice.h"

namespace otb
{
namespace Functor
{
/** \class IR
 *  \brief This functor computes the Redness Index (IR)
 *
 *  [Pouget et al., "Caracteristiques spectrales des surfaces sableuses
 *   de la region cotiere nord-ouest de l'Egypte: application aux donnees
 *   satellitaires Spot, In: 2eme Journeees de Teledetection: Caracterisation
 *   et suivi des milieux terrestres en regions arides et tropicales. 4-6/12/1990
 *   Ed. ORSTOM, Collection Colloques et Seminaires, Paris, pp. 27-38]
 *
 *  \ingroup Functor
 *  \ingroup Radiometry
 *
 * \ingroup OTBIndices
 */
template <class TInput, class TOutput>
class IR : public RadiometricIndice<TInput,TOutput>
{
public:
  IR() : RadiometricIndice<TInput,TOutput>("IR",{Band::RED, Band::GREEN}) {}

  TOutput operator()(const itk::VariableLengthVector<TInput> & input) const override  
  {
    auto green = this->Value(Band::GREEN,input);
    auto red = this->Value(Band::RED,input);
  
    if (std::abs(green) < this->EpsilonToBeConsideredAsZero)
      {
      return static_cast<TOutput>(0.);
      }

    return static_cast<TOutput>(red * red / (green * green * green));
  }
};

/** \class IC
 *  \brief This functor computes the Color Index (IC)
 *
 *  [Pouget et al., "Caracteristiques spectrales des surfaces sableuses
 *   de la region cotiere nord-ouest de l'Egypte: application aux donnees
 *   satellitaires Spot, In: 2eme Journeees de Teledetection: Caracterisation
 *   et suivi des milieux terrestres en regions arides et tropicales. 4-6/12/1990
 *   Ed. ORSTOM, Collection Colloques et Seminaires, Paris, pp. 27-38]
 *
 *  \ingroup Functor
 * \ingroup Radiometry
 *
 * \ingroup OTBIndices
 */
template <class TInput, class TOutput>
class IC : public RadiometricIndice<TInput,TOutput>
{
public:
  IC() : RadiometricIndice<TInput,TOutput>("IC",{Band::RED, Band::GREEN}) {}

  TOutput operator()(const itk::VariableLengthVector<TInput> & input) const override
  {
    auto green = this->Value(Band::GREEN,input);
    auto red = this->Value(Band::RED,input);

    if (std::abs(green + red) < EpsilonToBeConsideredAsZero)
      {
      return static_cast<TOutput>(0.);
      }

    return (static_cast<TOutput>((red - green) / (red + green)));
  }
};

/** \class IB
 *  \brief This functor computes the Brilliance Index (IB)
 *
 *  [ ]
 *
 *  \ingroup Functor
 * \ingroup Radiometry
 *
 * \ingroup OTBIndices
 */
template <class TInput, class TOutput>
class IB : public RadiometricIndice<TInput,TOutput>
{
public:
  IB() : RadiometricIndice<TInput,TOutput>("IB",{Band::RED, Band::GREEN}) {}

  TOutput operator()(const itk::VariableLengthVector<TInput> & input) const override
  {
    auto green = this->Value(Band::GREEN,input);
    auto red = this->Value(Band::RED,input);

    return (static_cast<TOutput>(std::sqrt((red * red + green * green) / 2.)));
  }
};

/** \class IB2
 *  \brief This functor computes the Brilliance Index (IB2)
 *
 *  [ ]
 *
 *  \ingroup Functor
 * \ingroup Radiometry
 *
 * \ingroup OTBIndices
 */
template <class TInput, class TOutput>
class IB2 : public RadiometricIndice<TInput,TOutput>
{
public:
  
  IB2() : RadiometricIndice<TInput,TOutput>("IB2",{Band::RED, Band::GREEN, Band::NIR}) {}
 
 TOutput operator()(const itk::VariableLengthVector<TInput> & input) const override
  {
    auto green = this->Value(Band::GREEN,input);
    auto red = this->Value(Band::RED,input);
    auto nir = this->Value(Band::NIR,input);

    return (static_cast<TOutput>(std::sqrt((red * red + green * green + nir * nir) / 3.)));
  }
};

} // namespace Functor
} // namespace otb

#endif
