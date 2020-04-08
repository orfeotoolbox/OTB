/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbInverseLogPolarTransform_hxx
#define otbInverseLogPolarTransform_hxx

#include "otbInverseLogPolarTransform.h"
#include "otbMacro.h"
#include "otbMath.h"

namespace otb
{
/**
 * Constructor.
 */
template <class TScalarType>
InverseLogPolarTransform<TScalarType>::InverseLogPolarTransform() : Superclass(4)
{
  m_Center[0] = 0.0;
  m_Center[1] = 0.0;
  m_Scale[0]  = 1.0;
  m_Scale[1]  = 1.0;
}
/**
 * Destructor.
 */
template <class TScalarType>
InverseLogPolarTransform<TScalarType>::~InverseLogPolarTransform()
{
}
/**
 * Set the transform parameters through the standard interface.
 * \param parameters The parameters of the transform.
  */
template <class TScalarType>
void InverseLogPolarTransform<TScalarType>::SetParameters(const ParametersType& parameters)
{
  m_Center[0] = parameters[0];
  m_Center[1] = parameters[1];
  m_Scale[0]  = parameters[2];
  m_Scale[1]  = parameters[3];
  otbMsgDebugMacro(<< "Call To SetParameters: Center=" << m_Center << ", Scale=" << m_Scale);
  this->m_Parameters = parameters;
  this->Modified();
}
/**
 * Get the transform parameters through the standard interface.
 * \return The parameters of the transform.
 */
template <class TScalarType>
typename InverseLogPolarTransform<TScalarType>::ParametersType& InverseLogPolarTransform<TScalarType>::GetParameters(void) const
{
  // Filling parameters vector
  this->m_Parameters[0] = m_Center[0];
  this->m_Parameters[1] = m_Center[1];
  this->m_Parameters[2] = m_Scale[0];
  this->m_Parameters[3] = m_Scale[1];

  return this->m_Parameters;
}

/**
 * Transform a point.
 * \param point The point to transform.
 * \return The transformed point.
 */
template <class TScalarType>
typename InverseLogPolarTransform<TScalarType>::OutputPointType InverseLogPolarTransform<TScalarType>::TransformPoint(const InputPointType& point) const
{
  OutputPointType result;
  double          rho = std::sqrt(std::pow(point[0] - m_Center[0], 2) + std::pow(point[1] - m_Center[1], 2));
  if (rho > 0)
  {
    result[0] = (1. / m_Scale[0]) * std::asin((point[1] - m_Center[1]) / rho);
    // degree conversion
    result[0] = result[0] * (180. / CONST_PI);
    // Deplacing the range to [0, 90], [270, 360]
    result[0] = result[0] > 0. ? result[0] : result[0] + 360.;
    // Avoiding asin indetermination
    if ((point[0] - m_Center[0]) >= 0)
    {
      result[0] = result[0] < 90. ? result[0] + 90. : result[0] - 90.;
    }
    result[1] = (1. / m_Scale[1]) * std::log(rho);
    // otbMsgDebugMacro(<<std::log(std::pow(point[0]-m_Center[0], 2)+std::pow(point[1]-m_Center[1], 2)));
  }
  else
  {
    // for rho=0, reject the point outside the angular range to avoid nan error
    result[0] = 400.;
    result[1] = 0.;
  }
  return result;
}
/**
 * Transform a vector representing a point.
 * \param vector The point to transform.
 * \return The transformed point.
 */
template <class TScalarType>
typename InverseLogPolarTransform<TScalarType>::OutputVectorType InverseLogPolarTransform<TScalarType>::TransformVector(const InputVectorType& vector) const
{
  OutputVectorType result;
  double           rho = std::sqrt(std::pow(vector[0] - m_Center[0], 2) + std::pow(vector[1] - m_Center[1], 2));
  if (rho > 0)
  {
    result[0] = (1 / m_Scale[0]) * std::asin((vector[1] - m_Center[1]) / rho);
    // degree conversion
    result[0] = result[0] * (180 / CONST_PI);
    // Deplacing the range to [0, 90], [270, 360]
    result[0] = result[0] > 0 ? result[0] : result[0] + 360;
    // Avoiding asin indetermination
    if ((vector[0] - m_Center[0]) >= 0)
    {
      result[0] = result[0] < 90 ? result[0] + 90 : result[0] - 90;
    }
    result[1] = (1 / m_Scale[1]) * std::log(rho);
    // otbMsgDebugMacro(<<std::log(std::pow(vector[0]-m_Center[0], 2)+std::pow(vector[1]-m_Center[1], 2)));
  }
  else
  {
    // for rho=0, reject the vector outside the angular range to avoid nan error
    result[0] = 400;
    result[1] = 0;
  }
  return result;
}
/**
 * Transform a vnl vector representing a vector.
 * \param vector The vector to transform.
 * \return The transformed vector.
 */
template <class TScalarType>
typename InverseLogPolarTransform<TScalarType>::OutputVnlVectorType
InverseLogPolarTransform<TScalarType>::TransformVector(const InputVnlVectorType& vector) const
{
  OutputVnlVectorType result;
  double              rho = std::sqrt(std::pow(vector[0], 2) + std::pow(vector[1], 2));
  if (rho > 0)
  {
    result[0] = (1 / m_Scale[0]) * std::asin((vector[1] - m_Center[1]) / rho);
    // degree conversion
    result[0] = result[0] * (180 / CONST_PI);
    // Deplacing the range to [0, 90], [270, 360]
    result[0] = result[0] > 0 ? result[0] : result[0] + 360;
    // Avoiding std::asin indetermination
    if ((vector[0] - m_Center[0]) >= 0)
    {
      result[0] = result[0] < 90 ? result[0] + 90 : result[0] - 90;
    }
    result[1] = (1 / m_Scale[1]) * std::log(rho);
    // otbMsgDebugMacro(<<log(std::pow(vector[0]-m_Center[0], 2)+std::pow(vector[1]-m_Center[1], 2)));
  }
  else
  {
    // for rho=0, reject the vector outside the angular range to avoid nan error
    result[0] = 400;
    result[1] = 0;
  }
  return result;
}
/**
 * PrintSelf method.
 */
template <class TScalarType>
void InverseLogPolarTransform<TScalarType>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Center: " << m_Center << std::endl;
  os << indent << "Scale: " << m_Scale << std::endl;
}

} // end namespace otb
#endif
