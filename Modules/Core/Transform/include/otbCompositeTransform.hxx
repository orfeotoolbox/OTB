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

#ifndef otbCompositeTransform_hxx
#define otbCompositeTransform_hxx

#include "otbCompositeTransform.h"

#include "otbGenericMapProjection.h"
#include "itkIdentityTransform.h"

namespace otb
{

template <class TFirstTransform, class TSecondTransform, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
CompositeTransform<TFirstTransform, TSecondTransform, TScalarType, NInputDimensions, NOutputDimensions>::CompositeTransform()
  : Superclass(ParametersDimension), m_FirstTransform(nullptr), m_SecondTransform(nullptr)
{}

template <class TFirstTransform, class TSecondTransform, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
CompositeTransform<TFirstTransform, TSecondTransform, TScalarType, NInputDimensions, NOutputDimensions>::~CompositeTransform()
{
}

template <class TFirstTransform, class TSecondTransform, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
typename CompositeTransform<TFirstTransform, TSecondTransform, TScalarType, NInputDimensions, NOutputDimensions>::SecondTransformOutputPointType
CompositeTransform<TFirstTransform, TSecondTransform, TScalarType, NInputDimensions, NOutputDimensions>::TransformPoint(
    const FirstTransformInputPointType& point1) const
{
  FirstTransformOutputPointType geoPoint;
  geoPoint = m_FirstTransform->TransformPoint(point1);

  SecondTransformOutputPointType outputPoint;
  outputPoint = m_SecondTransform->TransformPoint(geoPoint);

  //  otbMsgDevMacro(<< std::setprecision(15) << "Converting: " << point1 << " -> " <<  geoPoint<< " -> " << outputPoint);

  return outputPoint;
}

/*template<class TFirstTransform, class TSecondTransform, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
  typename CompositeTransform<TFirstTransform, TSecondTransform, TScalarType, NInputDimensions, NOutputDimensions>::OutputVectorType
  CompositeTransform<TFirstTransform, TSecondTransform, TScalarType, NInputDimensions, NOutputDimensions>
  ::TransformVector(const InputVectorType &vector1) const
  {
  InputVectorType vectorTmp;
  OutputVectorType vector2;

  vectorTmp=m_FirstTransform->TransformVector(vector1);
  vector2=m_SecondTransform->TransformVector(vectorTmp);

  return vector2;
  }

  template<class TFirstTransform, class TSecondTransform, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
  typename CompositeTransform<TFirstTransform, TSecondTransform, TScalarType, NInputDimensions, NOutputDimensions>::OutputVnlVectorType
  CompositeTransform<TFirstTransform, TSecondTransform, TScalarType, NInputDimensions, NOutputDimensions>
  ::TransformVector(const InputVnlVectorType &vnlVector1) const
  {
  InputVnlVectorType vnlVectorTmp;
  OutputVnlVectorType vnlVector2;

  vnlVectorTmp=m_FirstTransform->TransformVector(vnlVector1);
  vnlVector2=m_SecondTransform->TransformVector(vnlVectorTmp);

  return vnlVector2;
  }

  template<class TFirstTransform, class TSecondTransform, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
  typename CompositeTransform<TFirstTransform, TSecondTransform, TScalarType, NInputDimensions, NOutputDimensions>::OutputCovariantVectorType
  CompositeTransform<TFirstTransform, TSecondTransform, TScalarType, NInputDimensions, NOutputDimensions>
  ::TransformCovariantVector(const InputCovariantVectorType &covariantVector1) const
  {
  InputCovariantVectorType covariantVectorTmp;
  OutputCovariantVectorType covariantVector2;

  covariantVectorTmp=m_FirstTransform->TransformCovariantVector(covariantVector1);
  covariantVector2=m_SecondTransform->TransformCovariantVector(covariantVectorTmp);

  return covariantVector2;
  }*/

} // namespace otb

#endif
