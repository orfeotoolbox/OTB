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
#ifndef _otbInverseLogPolarTransform_txx
#define _otbInverseLogPolarTransform_txx

#include "otbInverseLogPolarTransform.h"
#include "otbMacro.h"

namespace otb
{
/**
 * Constructor.
 */
template <class TScalarType>
InverseLogPolarTransform<TScalarType>
::InverseLogPolarTransform()
  :Superclass(2,4)
{
  m_Center[0]=0.0;
  m_Center[1]=0.0;
  m_Scale[0]=1.0;
  m_Scale[1]=1.0;
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
::SetParameters(const ParametersType &parameters)
{
  m_Center[0]=parameters[0];
  m_Center[1]=parameters[1];
  m_Scale[0] =parameters[2];
  m_Scale[1] =parameters[3];
  otbMsgDebugMacro(<<"Call To SetParameters: Center="<<m_Center<<", Scale="<<m_Scale);
  this->m_Parameters=parameters;
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
  this->m_Parameters[0]=m_Center[0];
  this->m_Parameters[1]=m_Center[1];
  this->m_Parameters[2]=m_Scale[0];
  this->m_Parameters[3]=m_Scale[1];
  
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
::TransformPoint(const InputPointType &point)
{
  OutputPointType result;
  result[0]=0;
  result[1]=0;
  if((point[0]-m_Center[0]!=0)&&(point[1]-m_Center[1]!=0))
    {
      result[0]=(1/m_Scale[0])*asin((point[1]-m_Center[1])/sqrt(pow(point[0]-m_Center[0],2)+pow(point[1]-m_Center[1],2)));
      result[1]=(1/2*m_Scale[1])*log(sqrt(pow(point[0]-m_Center[0],2)+pow(point[1]-m_Center[1],2)));
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
::TransformVector(const InputVectorType &vector)
{
  OutputVectorType result;
  result[0]=0;
  result[1]=0;
  if((vector[0]!=0)&&(vector[1]!=0))
    {
      result[0]=(1/m_Scale[0])*asin((vector[1])/sqrt(pow(vector[0],2)+pow(vector[1],2)));
      result[1]=(1/2*m_Scale[1])*log(sqrt(pow(vector[0],2)+pow(vector[1],2)));
    }
  return result;
}
/**
 * Transform a vnl vector representing a point.
 * \param vector The point to transform.
 * \return The transformed point.
 */  
template <class TScalarType>
typename InverseLogPolarTransform<TScalarType>
::OutputVnlVectorType
InverseLogPolarTransform<TScalarType>
::TransformVector(const InputVnlVectorType &vector)
{
  OutputVnlVectorType result;
  result[0]=0;
  result[1]=0;
  if((vector[0]!=0)&&(vector[1]!=0))
    {
      result[0]=(1/m_Scale[0])*asin((vector[1])/sqrt(pow(vector[0],2)+pow(vector[1],2)));
      result[1]=(1/2*m_Scale[1])*log(sqrt(pow(vector[0],2)+pow(vector[1],2)));
    }
  return result;
}
/**
 * PrintSelf method.
 */
template <class TScalarType>
void
InverseLogPolarTransform<TScalarType>
::PrintSelf(std::ostream &os,itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  os<<indent<<"Center: "<<m_Center<<std::endl;
  os<<indent<<"Scale: "<<m_Scale<<std::endl;
}

} // end namespace otb
#endif
