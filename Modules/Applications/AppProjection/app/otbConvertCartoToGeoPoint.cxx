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

#include "otbWrapperApplicationFactory.h"

#include <iostream>
#include <iomanip>

#include "otbWrapperNumericalParameter.h"
#include "otbWrapperMapProjectionParametersHandler.h"

#include "otbImageToGenericRSOutputParameters.h"

namespace otb
{
namespace Wrapper
{

class ConvertCartoToGeoPoint : public Application
{
public:
  /** Standard class typedefs. */
  typedef ConvertCartoToGeoPoint                    Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(ConvertCartoToGeoPoint, otb::Application);

  typedef otb::GenericRSTransform<>        TransformType;

private:
  void DoInit() override
  {
    SetName("ConvertCartoToGeoPoint");
    SetDescription("Convert cartographic coordinates to geographic ones.");

    // Documentation
    SetDocName("Cartographic to geographic coordinates conversion");
    SetDocLongDescription("This application computes the geographic coordinates from cartographic ones. User has to give the X and Y coordinate and the cartographic projection (see mapproj parameter for details).");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

    AddDocTag(Tags::Geometry);
	AddDocTag(Tags::Coordinates);

    AddParameter(ParameterType_Group, "carto", "Input cartographic coordinates");
    AddParameter(ParameterType_Float, "carto.x", "X cartographic coordinates");
    SetParameterDescription("carto.x", "X cartographic coordinates in the projection defined by mapproj parameter");

    AddParameter(ParameterType_Float, "carto.y", "Y cartographic coordinates");
    SetParameterDescription("carto.y", "Y cartographic coordinates in the projection defined by mapproj parameter");

    // Add the MapProjectionParameters
    MapProjectionParametersHandler::AddMapProjectionParameters(this, "mapproj");

    AddParameter(ParameterType_Float, "long", "Output long");
    SetParameterDescription("long", "Point longitude coordinates in decimal degrees.");
    SetParameterRole("long", Role_Output);

    AddParameter(ParameterType_Float, "lat", "Output lat");
    SetParameterDescription("lat", "Point latitude coordinates in decimal degrees.");
    SetParameterRole("lat", Role_Output);

    // Doc example parameter settings
    SetDocExampleParameterValue("carto.x", "367074.625");
    SetDocExampleParameterValue("carto.y", "4835740");
    SetDocExampleParameterValue("mapproj", "utm");
    SetDocExampleParameterValue("mapproj.utm.northhem", "true");
    SetDocExampleParameterValue("mapproj.utm.zone", "31");

    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {
  }

  void DoExecute() override
  {
    // Get the projectionRef
    std::string inputProjRef = MapProjectionParametersHandler::GetProjectionRefFromChoice(this, "mapproj");

    // Instantiate a GenericRSTransform
    // Input : coordiante system picked up by the user
    // Output : WGS84 corresponding to epsg code 4326
    TransformType::Pointer  transform = TransformType::New();
    transform->SetInputProjectionRef(inputProjRef);
    // Default to wgs84
    transform->SetOutputProjectionRef(otb::SpatialReference::FromWGS84().ToWkt());
    transform->InstantiateTransform();

    TransformType::InputPointType   cartoPoint;
    TransformType::OutputPointType  geoPoint;

    cartoPoint[0] = GetParameterFloat("carto.x");
    cartoPoint[1] = GetParameterFloat("carto.y");

    geoPoint = transform->TransformPoint(cartoPoint);

    otbAppLogINFO( << std::setprecision(10) << "Cartographic Point  (x , y)  : (" << cartoPoint[0] << ", " << cartoPoint[1] << ")" );
    otbAppLogINFO( << std::setprecision(10) << "Geographic   Point (Long, Lat) : (" << geoPoint[0] << ", " <<  geoPoint[1] << ")" );


    SetParameterFloat( "long",geoPoint[0]);
    SetParameterFloat( "lat",geoPoint[1]);
  }
};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::ConvertCartoToGeoPoint)
