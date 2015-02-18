/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbLogPolarTransform_txx
#define __otbLogPolarTransform_txx

#include "otbLogPolarTransform.h"
#include "otbMacro.h"
#include "otbMath.h"

namespace otb
{
/**
 * Constructor.
 */
template <class TScalarType>
LogPolarTransform<TScalarType>
::LogPolarTransform()
  : Superclass(4)
{
  m_Center[0] = 0.0;
  m_Center[1] = 0.0;
  m_Scale[0] = 1.0;
  m_Scale[1] = 1.0;
}
/**
 * Destructor.
 */
template <class TScalarType>
LogPolarTransform<TScalarType>
::~LogPolarTransform()
{}
/**
 * Set the transform parameters through the standard interface.
 * \param parameters The parameters of the transform.
  */
template <class TScalarType>
void
LogPolarTransform<TScalarType>
::SetParameters(const ParametersType& parameters)
{
  m_Center[0] = parameters[0];
  m_Center[1] = parameters[1];
  m_Scale[0] = parameters[2];
  m_Scale[1] = parameters[3];
  otbMsgDebugMacro(<< "Call To SetParameters: Center=" << m_Center << ", Scale=" << m_Scale);
  this->m_Parameters = parameters;
  this->Modified();
}
/**
 * Get the transform parameters through the standard interface.
 * \return The parameters of the transform.
 */
template <class TScalarType>
typename LogPolarTransform<TScalarType>
::ParametersType&
LogPolarTransform<TScalarType>
::GetParameters(void) const
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
typename LogPolarTransform<TScalarType>
::OutputPointType
LogPolarTransform<TScalarType>
::TransformPoint(const InputPointType& point) const
{
  OutputPointType result;
  double          theta = point[0] * m_Scale[0] * CONST_PI_180;
  double          logRho   = point[1] * m_Scale[1];
  result[0] = m_Center[0];
  result[1] = m_Center[1];
  result[0] += vcl_exp(logRho) * vcl_cos(theta);
  result[1] += vcl_exp(logRho) * vcl_sin(theta);
  return result;
}
/**
 * Transform a vector representing a point.
 * \param vector The point to transform.
 * \return The transformed point.
 */
template <class TScalarType>
typename LogPolarTransform<TScalarType>
::OutputVectorType
LogPolarTransform<TScalarType>
::TransformVector(const InputVectorType& vector) const
{
  OutputVectorType result;
  double           theta = vector[0] * m_Scale[0] * CONST_PI_180;
  double           logRho   = vector[1] * m_Scale[1];
  result[0] = 0.;
  result[1] = 0.;
  result[0] += vcl_exp(logRho) * vcl_cos(theta);
  result[1] += vcl_exp(logRho) * vcl_sin(theta);

  return result;
}
/**
 * Transform a vnl vector representing a point.
 * \param vector The point to transform.
 * \return The transformed point.
 */
template <class TScalarType>
typename LogPolarTransform<TScalarType>
::OutputVnlVectorType
LogPolarTransform<TScalarType>
::TransformVector(const InputVnlVectorType& vector) const
{
  OutputVnlVectorType result;
  double              theta = vector[0] * m_Scale[0] * CONST_PI_180;
  double              logRho   = vector[1] * m_Scale[1];
  result[0] = 0.;
  result[1] = 0.;
  result[0] += vcl_exp(logRho) * vcl_cos(theta);
  result[1] += vcl_exp(logRho) * vcl_sin(theta);

  return result;
}
/**
 * PrintSelf method.
 */
template <class TScalarType>
void
LogPolarTransform<TScalarType>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Center: " << m_Center << std::endl;
  os << indent << "Scale: " << m_Scale << std::endl;
}

} // end namespace otb
#endif
