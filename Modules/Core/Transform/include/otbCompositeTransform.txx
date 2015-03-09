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
#ifndef __otbCompositeTransform_txx
#define __otbCompositeTransform_txx

#include "otbCompositeTransform.h"

#include "otbGenericMapProjection.h"
#include "otbForwardSensorModel.h"
#include "otbInverseSensorModel.h"
#include "itkIdentityTransform.h"

namespace otb
{

template<class TFirstTransform,
    class TSecondTransform,
    class TScalarType,
    unsigned int NInputDimensions,
    unsigned int NOutputDimensions>
CompositeTransform<TFirstTransform,
    TSecondTransform,
    TScalarType,
    NInputDimensions,
    NOutputDimensions>
::CompositeTransform() : Superclass(ParametersDimension)
{
  m_FirstTransform = 0;
  m_SecondTransform = 0;
}

template<class TFirstTransform,
    class TSecondTransform,
    class TScalarType,
    unsigned int NInputDimensions,
    unsigned int NOutputDimensions>
CompositeTransform<TFirstTransform,
    TSecondTransform,
    TScalarType,
    NInputDimensions,
    NOutputDimensions>
::~CompositeTransform()
{
}

template<class TFirstTransform,
    class TSecondTransform,
    class TScalarType,
    unsigned int NInputDimensions,
    unsigned int NOutputDimensions>
typename CompositeTransform<TFirstTransform,
    TSecondTransform,
    TScalarType,
    NInputDimensions,
    NOutputDimensions>::SecondTransformOutputPointType
CompositeTransform<TFirstTransform,
    TSecondTransform,
    TScalarType,
    NInputDimensions,
    NOutputDimensions>
::TransformPoint(const FirstTransformInputPointType& point1) const
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
