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
#ifndef __otbGenericRSTransform_txx
#define __otbGenericRSTransform_txx

#include "otbGenericRSTransform.h"
#include "otbMacro.h"
#include "otbMetaDataKey.h"
#include "itkMetaDataObject.h"

#include "otbGeoInformationConversion.h"

#include "ogr_spatialref.h"

namespace otb
{

template<class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
GenericRSTransform<TScalarType, NInputDimensions, NOutputDimensions>
::GenericRSTransform() : Superclass(0)
{
  m_InputProjectionRef.clear();
  m_OutputProjectionRef.clear();
  m_InputKeywordList.Clear();
  m_OutputKeywordList.Clear();
  m_InputSpacing.Fill(1);
  m_InputOrigin.Fill(0);
  m_OutputSpacing.Fill(1);
  m_OutputOrigin.Fill(0);

  m_Transform = NULL;
  m_InputTransform = NULL;
  m_OutputTransform = NULL;
  m_TransformUpToDate = false;
  m_TransformAccuracy = Projection::UNKNOWN;
}

template<class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
const typename GenericRSTransform<TScalarType, NInputDimensions, NOutputDimensions>::TransformType*
GenericRSTransform<TScalarType, NInputDimensions, NOutputDimensions>
::GetTransform() const
{
  itkDebugMacro("returning MapProjection address " << this->m_Transform);
  if ((!m_TransformUpToDate) || (m_Transform.IsNull()))
    {
    itkExceptionMacro(<< "m_Transform not up-to-date, call InstanciateTransform() first");
    }

  return this->m_Transform;
}

/**
 * Instanciate the transformation according to information
 */
template<class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
void
GenericRSTransform<TScalarType, NInputDimensions, NOutputDimensions>
::InstanciateTransform(void)
{
  m_Transform = TransformType::New();

  if (m_InputKeywordList.GetSize()  == 0)
    {
    itk::ExposeMetaData<ImageKeywordlist>(m_InputDictionary, MetaDataKey::OSSIMKeywordlistKey, m_InputKeywordList);
    }
  if (m_InputProjectionRef.empty())
    {
    itk::ExposeMetaData<std::string>(m_InputDictionary, MetaDataKey::ProjectionRefKey, m_InputProjectionRef);
    }

  otbMsgDevMacro(<< "Information to instanciate transform: ");
  otbMsgDevMacro(<< " * Input Origin: " << m_InputOrigin);
  otbMsgDevMacro(<< " * Input Spacing: " << m_InputSpacing);
  otbMsgDevMacro(<< " * Input keyword list: "
                 << ((m_InputKeywordList.GetSize() == 0) ? "Empty" : "Full"));
  otbMsgDevMacro(<< " * Input projection: " << m_InputProjectionRef);
  otbMsgDevMacro(<< " * Output keyword list: "
                 << ((m_OutputKeywordList.GetSize() == 0) ? "Empty" : "Full"));
  otbMsgDevMacro(<< " * Output projection: " << m_OutputProjectionRef);
  otbMsgDevMacro(<< " * Output Origin: " << m_OutputOrigin);
  otbMsgDevMacro(<< " * Output Spacing: " << m_OutputSpacing);

  //Make sure that the state is clean:
  m_InputTransform = NULL;
  m_OutputTransform = NULL;

  bool firstTransformGiveGeo = true;
  bool inputTransformIsSensor = false;
  bool inputTransformIsMap = false;
  bool outputTransformIsSensor = false;
  bool outputTransformIsMap = false;

  // Prepare the projection ref (eventually convert the EPSG code into full WKT)
  //
  // Note that we do that at the GenericRSTransform level and not in the member
  // class for several reasons:
  // - at the GenericMapProjection and MapProjectionAdapter the method are
  // called SetWkt and thus should not take a SRID.
  // - we do not want to mix the GeoInformationConversion (which uses gdal) in
  // the MapProjectionAdapter to keep ossim and gdal dependencies as separated
  // as possible.
  m_InputProjectionRef = GeoInformationConversion::ToWKT(m_InputProjectionRef);
  m_OutputProjectionRef = GeoInformationConversion::ToWKT(m_OutputProjectionRef);

  //*****************************
  //Set the input transformation
  //*****************************

    // First, try to make a geo transform
  if (!m_InputProjectionRef.empty()) //map projection
    {
    typedef otb::GenericMapProjection<TransformDirection::INVERSE, ScalarType, InputSpaceDimension, InputSpaceDimension>
        InverseMapProjectionType;
    typename InverseMapProjectionType::Pointer mapTransform = InverseMapProjectionType::New();

    mapTransform->SetWkt(m_InputProjectionRef);
    if (mapTransform->IsProjectionDefined())
      {
      m_InputTransform = mapTransform.GetPointer();
      inputTransformIsMap = true;
      otbMsgDevMacro(<< "Input projection set to map transform: " << m_InputTransform);
      }
    }

  // If not, try to make a sensor model
  if ((m_InputTransform.IsNull()) && (m_InputKeywordList.GetSize() > 0))
    {
    typedef otb::ForwardSensorModel<double, InputSpaceDimension, InputSpaceDimension> ForwardSensorModelType;
    typename ForwardSensorModelType::Pointer sensorModel = ForwardSensorModelType::New();

    sensorModel->SetImageGeometry(m_InputKeywordList);

    if (sensorModel->IsValidSensorModel())
      {
      m_InputTransform = sensorModel.GetPointer();
      inputTransformIsSensor = true;
      otbMsgDevMacro(<< "Input projection set to sensor model.");
      }
    }

  if (m_InputTransform.IsNull()) //default if we didn't manage to instantiate it before
    {
    m_InputTransform = itk::IdentityTransform<double, NInputDimensions>::New();
//     firstTransformGiveGeo = false;

    OGRSpatialReferenceH hSRS = NULL;
    hSRS = OSRNewSpatialReference(NULL);
    const char * wktString = m_InputProjectionRef.c_str();
    if (OSRImportFromWkt(hSRS, (char **) &wktString) != OGRERR_NONE)
      {
      firstTransformGiveGeo = false;
      otbMsgDevMacro(<< "- Considering that the first transform does not give geo (WKT)")
      }

    else if ( OSRIsGeographic(hSRS) )
      {
      firstTransformGiveGeo = true;
      otbMsgDevMacro(<< "- Considering that the first transform gives geo")
      }
    else
      {
      firstTransformGiveGeo = false;
      otbMsgDevMacro(<< "- Considering that the first transform does not give geo (fallback)")
      }
    OSRRelease(hSRS);
    otbMsgDevMacro(<< "Input projection set to identity")
    }

  //*****************************
  //Set the output transformation
  //*****************************
  if (!m_OutputProjectionRef.empty()) //map projection
    {
    typedef otb::GenericMapProjection<TransformDirection::FORWARD, ScalarType, InputSpaceDimension,
    OutputSpaceDimension> ForwardMapProjectionType;
    typename ForwardMapProjectionType::Pointer mapTransform = ForwardMapProjectionType::New();
    mapTransform->SetWkt(m_OutputProjectionRef);
    if (mapTransform->IsProjectionDefined())
      {
      m_OutputTransform = mapTransform.GetPointer();
      outputTransformIsMap = true;
      otbMsgDevMacro(<< "Output projection set to map transform: " << m_OutputTransform);
      }
    }

  // If not, try to make a sensor model
  if ((m_OutputTransform.IsNull()) && (m_OutputKeywordList.GetSize() > 0))
    {
    typedef otb::InverseSensorModel<double, InputSpaceDimension, OutputSpaceDimension> InverseSensorModelType;
    typename InverseSensorModelType::Pointer sensorModel = InverseSensorModelType::New();

    sensorModel->SetImageGeometry(m_OutputKeywordList);

    if (sensorModel->IsValidSensorModel())
      {
      m_OutputTransform = sensorModel.GetPointer();
      outputTransformIsSensor = true;
      otbMsgDevMacro(<< "Output projection set to sensor model");
      }
    }


  if (m_OutputTransform.IsNull()) //default if we didn't manage to instantiate it before
    {
    m_OutputTransform = itk::IdentityTransform<double, NOutputDimensions>::New();
    if (firstTransformGiveGeo)
      {
      m_OutputProjectionRef =
        "GEOGCS[\"GCS_WGS_1984\", DATUM[\"D_WGS_1984\", SPHEROID[\"WGS_1984\", 6378137, 298.257223563]], PRIMEM[\"Greenwich\", 0], UNIT[\"Degree\", 0.017453292519943295]]";
      }
    otbMsgDevMacro(<< "Output projection set to identity");
    }

  m_Transform->SetFirstTransform(m_InputTransform);
  m_Transform->SetSecondTransform(m_OutputTransform);
  m_TransformUpToDate = true;

  //The accuracy information is a simplistic model for now and should be refined
  if ((inputTransformIsSensor || outputTransformIsSensor))
    {
    //Sensor model
    m_TransformAccuracy = Projection::ESTIMATE;
    }
  else if (firstTransformGiveGeo && !outputTransformIsSensor && !outputTransformIsMap)
    {
    //The original image was in lon/lat and we did not change anything
    m_TransformAccuracy = Projection::PRECISE;
    }
  else if (!inputTransformIsSensor && !outputTransformIsSensor && !inputTransformIsMap && !outputTransformIsMap)
    {
    //no transform
    m_TransformAccuracy = Projection::UNKNOWN;
    }
  else
    {
    m_TransformAccuracy = Projection::PRECISE;
    }

}

template<class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
typename GenericRSTransform<TScalarType, NInputDimensions, NOutputDimensions>::OutputPointType
GenericRSTransform<TScalarType, NInputDimensions, NOutputDimensions>
::TransformPoint(const InputPointType& point) const
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

template<class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
bool
GenericRSTransform<TScalarType, NInputDimensions, NOutputDimensions>
::GetInverse(Self * inverseTransform) const
{
  // Test the inverseTransform pointer
  if (inverseTransform == NULL)
    {
    return false;
    }

  // Swich projection refs
  inverseTransform->SetInputProjectionRef(m_OutputProjectionRef);
  inverseTransform->SetOutputProjectionRef(m_InputProjectionRef);

  // Switch keywordlists
  inverseTransform->SetInputKeywordList(m_OutputKeywordList);
  inverseTransform->SetOutputKeywordList(m_InputKeywordList);

  // Switch dictionnaries
  inverseTransform->SetInputDictionary(m_OutputDictionary);
  inverseTransform->SetOutputDictionary(m_InputDictionary);

  // Switch spacings
  inverseTransform->SetInputSpacing(m_OutputSpacing);
  inverseTransform->SetOutputSpacing(m_InputSpacing);

  // Switch origins
  inverseTransform->SetInputOrigin(m_OutputOrigin);
  inverseTransform->SetOutputOrigin(m_InputOrigin);

  // Instantiate transform
  inverseTransform->InstanciateTransform();

  return true;
}

template<class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
typename GenericRSTransform<TScalarType, NInputDimensions, NOutputDimensions>
::InverseTransformBasePointer
GenericRSTransform<TScalarType, NInputDimensions, NOutputDimensions>
::GetInverseTransform() const
{
  Self * inverseTransform = Self::New();

  bool success = this->GetInverse(inverseTransform);

  if (!success)
    {
    itkExceptionMacro(<< "Failed to create inverse transform");
    }

  return inverseTransform;
}

template<class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
void
GenericRSTransform<TScalarType, NInputDimensions, NOutputDimensions>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Up to date: " << m_TransformUpToDate << std::endl;
  if (m_TransformUpToDate)
    {
    os << indent << "Input transform: "<< std::endl;
    m_InputTransform->Print(os, indent.GetNextIndent());
    os << indent << "Output transform: " << std::endl;
    m_OutputTransform->Print(os, indent.GetNextIndent());
    }
  else
    {
    os << indent << "Input transform: NULL" << std::endl;
    os << indent << "Output transform: NULL" << std::endl;
    }
  os << indent << "Accuracy: "
      << (m_TransformAccuracy == Projection::PRECISE ?
                "PRECISE" : (m_TransformAccuracy == Projection::ESTIMATE ?
                                  "ESTIMATE" : "UNKNOWN")) << std::endl;
}

} // namespace otb

#endif
