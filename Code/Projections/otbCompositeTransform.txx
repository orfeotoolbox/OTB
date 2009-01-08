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
#include "otbMapProjections.h"
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
  ::CompositeTransform() : Superclass(SpaceDimension,ParametersDimension)
  {
    m_FirstTransform = 0;
    m_SecondTransform = 0;
    m_FirstTransformProjectionType = PROJDEFAULT;
    m_SecondTransformProjectionType = PROJDEFAULT;
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
  ::TransformPoint(const FirstTransformInputPointType &point1) const
  {

    typedef itk::IdentityTransform< double, 2 > IdentityTransformType;
    typedef otb::GenericMapProjection<otb::FORWARD> ForwardMapProjectionType;
    typedef otb::GenericMapProjection<otb::INVERSE> InverseMapProjectionType;
    typedef otb::ForwardSensorModel<double> ForwardSensorModelType;
    typedef otb::InverseSensorModel<double> InverseSensorModelType;

    FirstTransformOutputPointType geoPoint;

    switch(m_FirstTransformProjectionType)
    {
      case PROJDEFAULT:
      {
        geoPoint=m_FirstTransform->TransformPoint(point1);
        break;
      }
      case PROJIDENTITY:
      {
        geoPoint=dynamic_cast<IdentityTransformType*>(m_FirstTransform)->TransformPoint(point1);
        break;
      }
      case PROJMAPFORWARD:
      {
        geoPoint=dynamic_cast<ForwardMapProjectionType*>(m_FirstTransform)->TransformPoint(point1);
        break;
      }
      case PROJMAPINVERSE:
      {
        geoPoint=dynamic_cast<InverseMapProjectionType*>(m_FirstTransform)->TransformPoint(point1);
        break;
      }
      case PROJSENSORFORWARD:
      {
        geoPoint=dynamic_cast<ForwardSensorModelType*>(m_FirstTransform)->TransformPoint(point1);
        break;
      }
      case PROJSENSORINVERSE:
      {
        geoPoint=dynamic_cast<InverseSensorModelType*>(m_FirstTransform)->TransformPoint(point1);
        break;
      }
    }

    otbMsgDevMacro(<< "Geographic point is ("<<  geoPoint[0]<< ","<<  geoPoint[1]<< ")");

    SecondTransformOutputPointType outputPoint;

    switch(m_SecondTransformProjectionType)
    {
      case PROJDEFAULT:
      {
        outputPoint=m_SecondTransform->TransformPoint(geoPoint);
        break;
      }
      case PROJIDENTITY:
      {
        outputPoint=dynamic_cast<IdentityTransformType*>(m_SecondTransform)->TransformPoint(geoPoint);
        break;
      }
      case PROJMAPFORWARD:
      {
        outputPoint=dynamic_cast<ForwardMapProjectionType*>(m_SecondTransform)->TransformPoint(geoPoint);
        break;
      }
      case PROJMAPINVERSE:
      {
        outputPoint=dynamic_cast<InverseMapProjectionType*>(m_SecondTransform)->TransformPoint(geoPoint);
        break;
      }
      case PROJSENSORFORWARD:
      {
        outputPoint=dynamic_cast<ForwardSensorModelType*>(m_SecondTransform)->TransformPoint(geoPoint);
        break;
      }
      case PROJSENSORINVERSE:
      {
        outputPoint=dynamic_cast<InverseSensorModelType*>(m_SecondTransform)->TransformPoint(geoPoint);
        break;
      }
    }

    otbMsgDevMacro(<< "Corresponding coordinates in sensor geometry are: " << std::endl
        << outputPoint[0] << ","<< outputPoint[1] );

    return outputPoint;
  }

  /*template<class TFirstTransform, class TSecondTransform, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
    typename CompositeTransform<TFirstTransform, TSecondTransform, TScalarType, NInputDimensions, NOutputDimensions>::OutputVectorType
    CompositeTransform<TFirstTransform, TSecondTransform, TScalarType,NInputDimensions, NOutputDimensions>
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
    CompositeTransform<TFirstTransform, TSecondTransform, TScalarType,NInputDimensions, NOutputDimensions>
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
    CompositeTransform<TFirstTransform, TSecondTransform, TScalarType,NInputDimensions, NOutputDimensions>
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

