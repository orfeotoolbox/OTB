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

#ifndef otbPolarimetricSynthesisFunctor_h
#define otbPolarimetricSynthesisFunctor_h

#include "otbMath.h"
#include <complex>
#include "itkFixedArray.h"

namespace otb
{
namespace Functor
{
/** \class PolarimetricSynthesisFunctor
 *  \brief Calculate the polarimetric synthesis
 *
 * This functor calculate the polarimetric synthesis using the electroMagneticField vectors as follow:
 * \f$ \sigma(\psi_{i},\chi_{i},\psi_{r},\chi_{r}) = \vec(E_{r})\cdot\left[ S \right] \vec(E_{i}) \f$
 *
 *  \ingroup Functor
 *  \ingroup SARPolarimetry
 *
 *
 * \ingroup OTBPolarimetry
 */
template <class TInput1, class TInput2, class TInput3, class TInput4, class TOutput>
class PolarimetricSynthesisFunctor
{
public:
  /** Some typedefs. */
  typedef typename     std::complex <double>           ComplexType;
  typedef typename     itk::FixedArray<ComplexType, 2> ComplexArrayType;

  /** Set the ElectroMagneticField Incident */
  void SetEi(ComplexArrayType ei)
  {
    m_Ei = ei;
  }

  /** Set the ElectroMagneticField Reflected */
  void SetEr(ComplexArrayType er)
  {
    m_Er = er;
  }

  /** Constructor */
  PolarimetricSynthesisFunctor()
  {
    m_Ei.Fill(1);
    m_Er.Fill(1);
  }
  /** Destructor */
  virtual ~PolarimetricSynthesisFunctor() {}
  inline TOutput operator ()(const TInput1& Shh, const TInput2& Shv, const TInput3& Svh, const TInput4& Svv)
  {
    ComplexType tmp;
    double      scalar;

    tmp =   std::conj(m_Er[0]) * (m_Ei[0] * static_cast<ComplexType>(Shh) + m_Ei[1] * static_cast<ComplexType>(Shv))
          + std::conj(m_Er[1]) * (m_Ei[0] * static_cast<ComplexType>(Svh) + m_Ei[1] * static_cast<ComplexType>(Svv));

    scalar = static_cast<double>(std::abs(tmp)) * static_cast<double>(std::abs(tmp));

    return (static_cast<TOutput>(scalar));
  }

private:
  /** Electromagnetic Field Incident */
  ComplexArrayType m_Ei;
  /** Electromagnetic Field Reflected */
  ComplexArrayType m_Er;

};

} // namespace Functor
} // namespace otb

#endif
