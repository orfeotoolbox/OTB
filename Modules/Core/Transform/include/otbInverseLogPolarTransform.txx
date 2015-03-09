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
#ifndef __otbInverseLogPolarTransform_txx
#define __otbInverseLogPolarTransform_txx

#include "otbInverseLogPolarTransform.h"
#include "otbMacro.h"
#include "otbMath.h"

namespace otb
{
/**
 * Constructor.
 */
template <class TScalarType>
InverseLogPolarTransform<TScalarType>
::InverseLogPolarTransform()
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
InverseLogPolarTransform<TScalarType>
::~InverseLogPolarTransform()
{}
/**
 * Set the transform parameters through the standard interface.
 * \param parameters The parameters of the transform.
  */
template <class TScalarType>
void
InverseLogPolarTransform<TScalarType>
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
typename InverseLogPolarTransform<TScalarType>
::ParametersType&
InverseLogPolarTransform<TScalarType>
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
typename InverseLogPolarTransform<TScalarType>
::OutputPointType
InverseLogPolarTransform<TScalarType>
::TransformPoint(const InputPointType& point) const
{
  OutputPointType result;
  double          rho = vcl_sqrt(vcl_pow(point[0] - m_Center[0], 2) + vcl_pow(point[1] - m_Center[1], 2));
  if (rho > 0)
    {
    result[0] = (1. / m_Scale[0]) * vcl_asin((point[1] - m_Center[1]) / rho);
    // degree conversion
    result[0] = result[0] * (180. / CONST_PI);
    // Deplacing the range to [0, 90], [270, 360]
    result[0] = result[0] > 0. ? result[0] : result[0] + 360.;
    // Avoiding asin indetermination
    if ((point[0] - m_Center[0]) >= 0)
      {
      result[0] = result[0] < 90. ? result[0] + 90. : result[0] - 90.;
      }
    result[1] = (1. / m_Scale[1]) * vcl_log(rho);
    // otbMsgDebugMacro(<<vcl_log(vcl_pow(point[0]-m_Center[0], 2)+vcl_pow(point[1]-m_Center[1], 2)));
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
typename InverseLogPolarTransform<TScalarType>
::OutputVectorType
InverseLogPolarTransform<TScalarType>
::TransformVector(const InputVectorType& vector) const
{
  OutputVectorType result;
  double           rho = vcl_sqrt(vcl_pow(vector[0] - m_Center[0], 2) + vcl_pow(vector[1] - m_Center[1], 2));
  if (rho > 0)
    {
    result[0] = (1 / m_Scale[0]) * vcl_asin((vector[1] - m_Center[1]) / rho);
    // degree conversion
    result[0] = result[0] * (180 / CONST_PI);
    // Deplacing the range to [0, 90], [270, 360]
    result[0] = result[0] > 0 ? result[0] : result[0] + 360;
    // Avoiding asin indetermination
    if ((vector[0] - m_Center[0]) >= 0)
      {
      result[0] = result[0] < 90 ? result[0] + 90 : result[0] - 90;
      }
    result[1] = (1 / m_Scale[1]) * vcl_log(rho);
    // otbMsgDebugMacro(<<vcl_log(vcl_pow(vector[0]-m_Center[0], 2)+vcl_pow(vector[1]-m_Center[1], 2)));
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
typename InverseLogPolarTransform<TScalarType>
::OutputVnlVectorType
InverseLogPolarTransform<TScalarType>
::TransformVector(const InputVnlVectorType& vector) const
{
  OutputVnlVectorType result;
  double              rho = vcl_sqrt(vcl_pow(vector[0], 2) + vcl_pow(vector[1], 2));
  if (rho > 0)
    {
    result[0] = (1 / m_Scale[0]) * vcl_asin((vector[1] - m_Center[1]) / rho);
    // degree conversion
    result[0] = result[0] * (180 / CONST_PI);
    // Deplacing the range to [0, 90], [270, 360]
    result[0] = result[0] > 0 ? result[0] : result[0] + 360;
    // Avoiding vcl_asin indetermination
    if ((vector[0] - m_Center[0]) >= 0)
      {
      result[0] = result[0] < 90 ? result[0] + 90 : result[0] - 90;
      }
    result[1] = (1 / m_Scale[1]) * vcl_log(rho);
    // otbMsgDebugMacro(<<log(vcl_pow(vector[0]-m_Center[0], 2)+vcl_pow(vector[1]-m_Center[1], 2)));
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
void
InverseLogPolarTransform<TScalarType>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Center: " << m_Center << std::endl;
  os << indent << "Scale: " << m_Scale << std::endl;
}

} // end namespace otb
#endif
