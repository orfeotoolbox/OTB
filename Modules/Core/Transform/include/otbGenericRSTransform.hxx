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

#ifndef otbGenericRSTransform_hxx
#define otbGenericRSTransform_hxx

#include "otbGenericRSTransform.h"
#include "otbMacro.h"
#include "otbMetaDataKey.h"
#include "otbRPCForwardTransform.h"
#include "otbRPCInverseTransform.h"

#include "otbSpatialReference.h"

#include "ogr_spatialref.h"

namespace otb
{

template <class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
GenericRSTransform<TScalarType, NInputDimensions, NOutputDimensions>::GenericRSTransform() : Superclass(0)
{
  m_InputProjectionRef.clear();
  m_OutputProjectionRef.clear();
  m_InputSpacing.Fill(1);
  m_InputOrigin.Fill(0);
  m_OutputSpacing.Fill(1);
  m_OutputOrigin.Fill(0);

  m_InputImd          = nullptr;
  m_OutputImd         = nullptr;
  m_Transform         = nullptr;
  m_InputTransform    = nullptr;
  m_OutputTransform   = nullptr;
  m_TransformUpToDate = false;
  m_TransformAccuracy = Projection::UNKNOWN;
}

template <class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
const typename GenericRSTransform<TScalarType, NInputDimensions, NOutputDimensions>::TransformType*
GenericRSTransform<TScalarType, NInputDimensions, NOutputDimensions>::GetTransform() const
{
  itkDebugMacro("returning MapProjection address " << this->m_Transform);
  if ((!m_TransformUpToDate) || (m_Transform.IsNull()))
  {
    itkExceptionMacro(<< "m_Transform not up-to-date, call InstantiateTransform() first");
  }

  return this->m_Transform;
}

/**
 * Instantiate the transformation according to information
 */
template <class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
void GenericRSTransform<TScalarType, NInputDimensions, NOutputDimensions>::InstantiateTransform(void)
{
  m_Transform = TransformType::New();

  if (m_InputProjectionRef.empty() && m_InputImd != nullptr && m_InputImd->Has(MDGeom::ProjectionProj))
  {
    m_InputProjectionRef = boost::any_cast<std::string>(m_InputImd->operator[](MDGeom::ProjectionProj));
  }

  otbMsgDevMacro(<< "Information to instantiate transform: ");
  otbMsgDevMacro(<< " * Input Origin: " << m_InputOrigin);
  otbMsgDevMacro(<< " * Input Spacing: " << m_InputSpacing);
  otbMsgDevMacro(<< " * Input metadata: " << ((m_InputImd == nullptr) ? "Empty" : "Full"));
  otbMsgDevMacro(<< " * Input projection: " << m_InputProjectionRef);
  otbMsgDevMacro(<< " * Output metadata: " << ((m_OutputImd == nullptr) ? "Empty" : "Full"));
  otbMsgDevMacro(<< " * Output projection: " << m_OutputProjectionRef);
  otbMsgDevMacro(<< " * Output Origin: " << m_OutputOrigin);
  otbMsgDevMacro(<< " * Output Spacing: " << m_OutputSpacing);

  // Make sure that the state is clean:
  m_InputTransform  = nullptr;
  m_OutputTransform = nullptr;

  bool inputTransformIsSensor  = false;
  bool inputTransformIsMap     = false;
  bool outputTransformIsSensor = false;
  bool outputTransformIsMap    = false;

  //*****************************
  // Set the input transformation
  //*****************************

  // First, try to make a geo transform
  if (!m_InputProjectionRef.empty()) // map projection
  {
    typedef otb::GenericMapProjection<TransformDirection::INVERSE, ScalarType, InputSpaceDimension, InputSpaceDimension> InverseMapProjectionType;
    typename InverseMapProjectionType::Pointer mapTransform = InverseMapProjectionType::New();

    mapTransform->SetWkt(m_InputProjectionRef);
    if (mapTransform->IsProjectionDefined())
    {
      m_InputTransform    = mapTransform.GetPointer();
      inputTransformIsMap = true;
      otbMsgDevMacro(<< "Input projection set to map transform: " << m_InputTransform);
    }
  }

  // If not, try to make a RPC sensor model
  if ((m_InputTransform.IsNull()) && (m_InputImd != nullptr) && (m_InputImd->Has(MDGeom::RPC)))
  {
    typedef otb::RPCForwardTransform<double, InputSpaceDimension, OutputSpaceDimension> RPCForwardTransformType;
    typename RPCForwardTransformType::Pointer sensorModel = RPCForwardTransformType::New();

    sensorModel->SetMetadataModel((*m_InputImd)[MDGeom::RPC]);

    if (sensorModel->IsValidSensorModel())
    {
      m_InputTransform       = sensorModel.GetPointer();
      inputTransformIsSensor = true;
      otbMsgDevMacro(<< "Input projection set to sensor model.");
    }
  }

  //*****************************
  // Set the output transformation
  //*****************************
  if (!m_OutputProjectionRef.empty()) // map projection
  {
    typedef otb::GenericMapProjection<TransformDirection::FORWARD, ScalarType, InputSpaceDimension, OutputSpaceDimension> ForwardMapProjectionType;
    typename ForwardMapProjectionType::Pointer mapTransform = ForwardMapProjectionType::New();
    mapTransform->SetWkt(m_OutputProjectionRef);
    if (mapTransform->IsProjectionDefined())
    {
      m_OutputTransform    = mapTransform.GetPointer();
      outputTransformIsMap = true;
      otbMsgDevMacro(<< "Output projection set to map transform: " << m_OutputTransform);
    }
  }

  // If not, try to make a RPC sensor model
  if ((m_OutputTransform.IsNull()) && (m_OutputImd != nullptr) && (m_OutputImd->Has(MDGeom::RPC)))
  {
    typedef otb::RPCInverseTransform<double, InputSpaceDimension, OutputSpaceDimension> RPCInverseTransformType;
    typename RPCInverseTransformType::Pointer sensorModel = RPCInverseTransformType::New();

    sensorModel->SetMetadataModel((*m_OutputImd)[MDGeom::RPC]);

    if (sensorModel->IsValidSensorModel())
    {
      m_OutputTransform       = sensorModel.GetPointer();
      outputTransformIsSensor = true;
      otbMsgDevMacro(<< "Output projection set to sensor model");
    }
  }

  if (m_InputTransform.IsNull()) // default if we didn't manage to instantiate it before
  {
    // In this case, if output transform is set, we set
    // inputProjectionRef to wgs84 to ensure consistency
    if (outputTransformIsSensor || outputTransformIsMap)
    {
      m_InputProjectionRef = SpatialReference::FromWGS84().ToWkt();
    }

    m_InputTransform = itk::IdentityTransform<double, NInputDimensions>::New();
  }


  if (m_OutputTransform.IsNull()) // default if we didn't manage to instantiate it before
  {
    // In this case, if input transform is set, we set
    // outputProjectionRef to wgs84 to ensure consistency
    if (inputTransformIsSensor || inputTransformIsMap)
    {
      m_OutputProjectionRef = SpatialReference::FromWGS84().ToWkt();
    }


    m_OutputTransform = itk::IdentityTransform<double, NOutputDimensions>::New();
    otbMsgDevMacro(<< "Output projection set to identity");
  }

  m_Transform->SetFirstTransform(m_InputTransform);
  m_Transform->SetSecondTransform(m_OutputTransform);
  m_TransformUpToDate = true;

  // The accuracy information is a simplistic model for now and should be refined
  if ((inputTransformIsSensor || outputTransformIsSensor))
  {
    // Sensor model
    m_TransformAccuracy = Projection::ESTIMATE;
  }
  else if (!outputTransformIsSensor && !outputTransformIsMap)
  {
    // The original image was in lon/lat and we did not change anything
    m_TransformAccuracy = Projection::PRECISE;
  }
  else if (!inputTransformIsSensor && !outputTransformIsSensor && !inputTransformIsMap && !outputTransformIsMap)
  {
    // no transform
    m_TransformAccuracy = Projection::UNKNOWN;
  }
  else
  {
    m_TransformAccuracy = Projection::PRECISE;
  }
}

template <class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
typename GenericRSTransform<TScalarType, NInputDimensions, NOutputDimensions>::OutputPointType
GenericRSTransform<TScalarType, NInputDimensions, NOutputDimensions>::TransformPoint(const InputPointType& point) const
{
  InputPointType inputPoint = point;

  // Apply input origin/spacing
  inputPoint[0] = inputPoint[0] * m_InputSpacing[0] + m_InputOrigin[0];
  inputPoint[1] = inputPoint[1] * m_InputSpacing[1] + m_InputOrigin[1];

  // Transform point
  OutputPointType outputPoint;
  outputPoint = this->GetTransform()->TransformPoint(inputPoint);

  // Apply output origin/spacing
  outputPoint[0] = (outputPoint[0] - m_OutputOrigin[0]) / m_OutputSpacing[0];
  outputPoint[1] = (outputPoint[1] - m_OutputOrigin[1]) / m_OutputSpacing[1];

  //  otbMsgDevMacro("GenericRSTransform: " << point << " -> " << outputPoint);

  return outputPoint;
}

template <class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
bool GenericRSTransform<TScalarType, NInputDimensions, NOutputDimensions>::GetInverse(Self* inverseTransform) const
{
  // Test the inverseTransform pointer
  if (inverseTransform == nullptr)
  {
    return false;
  }

  // Swich projection refs
  inverseTransform->SetInputProjectionRef(m_OutputProjectionRef);
  inverseTransform->SetOutputProjectionRef(m_InputProjectionRef);

  // Switch ImageMetadatas
  inverseTransform->SetInputImageMetadata(m_OutputImd);
  inverseTransform->SetOutputImageMetadata(m_InputImd);

  // Switch spacings
  inverseTransform->SetInputSpacing(m_OutputSpacing);
  inverseTransform->SetOutputSpacing(m_InputSpacing);

  // Switch origins
  inverseTransform->SetInputOrigin(m_OutputOrigin);
  inverseTransform->SetOutputOrigin(m_InputOrigin);

  // Instantiate transform
  inverseTransform->InstantiateTransform();

  return true;
}

template <class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
typename GenericRSTransform<TScalarType, NInputDimensions, NOutputDimensions>::InverseTransformBasePointer
GenericRSTransform<TScalarType, NInputDimensions, NOutputDimensions>::GetInverseTransform() const
{
  Self* inverseTransform = Self::New();

  bool success = this->GetInverse(inverseTransform);

  if (!success)
  {
    itkExceptionMacro(<< "Failed to create inverse transform");
  }

  return inverseTransform;
}

template <class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
void GenericRSTransform<TScalarType, NInputDimensions, NOutputDimensions>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Up to date: " << m_TransformUpToDate << std::endl;
  if (m_TransformUpToDate)
  {
    os << indent << "Input transform: " << std::endl;
    m_InputTransform->Print(os, indent.GetNextIndent());
    os << indent << "Output transform: " << std::endl;
    m_OutputTransform->Print(os, indent.GetNextIndent());
  }
  else
  {
    os << indent << "Input transform: NULL" << std::endl;
    os << indent << "Output transform: NULL" << std::endl;
  }
  os << indent
     << "Accuracy: " << (m_TransformAccuracy == Projection::PRECISE ? "PRECISE" : (m_TransformAccuracy == Projection::ESTIMATE ? "ESTIMATE" : "UNKNOWN"))
     << std::endl;
}

} // namespace otb

#endif
