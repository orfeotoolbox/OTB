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

#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"

#include "otbSpatialReference.h"

namespace otb
{
namespace Wrapper
{

class ObtainUTMZoneFromGeoPoint : public Application
{
public:
  /** Standard class typedefs. */
  typedef ObtainUTMZoneFromGeoPoint     Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(ObtainUTMZoneFromGeoPoint, otb::Application);

private:
  ObtainUTMZoneFromGeoPoint()
  {
  }

  ~ObtainUTMZoneFromGeoPoint() override
  {
  }

  void DoInit() override
  {
    SetName("ObtainUTMZoneFromGeoPoint");
    SetDescription("UTM zone determination from a geographic point.");

    // Documentation
    SetDocName("Obtain UTM Zone From Geo Point");
    SetDocLongDescription("This application returns the UTM zone of an input geographic point.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

    AddDocTag(Tags::Geometry);

    AddParameter(ParameterType_Float,  "lat", "Latitude");
    SetParameterDescription("lat", "Latitude value of desired point.");

    AddParameter(ParameterType_Float,  "lon", "Longitude");
    SetParameterDescription("lon", "Longitude value of desired point.");

    AddParameter(ParameterType_Int,"utm","UTMZone");
    SetParameterDescription("utm","UTM Zone");
    MandatoryOff("utm");
    SetParameterRole("utm", Role_Output);

    SetExampleComment("Obtain a UTM Zone", 0);
    SetDocExampleParameterValue("lat","10.0");
    SetDocExampleParameterValue("lon","124.0");

    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {
    // Nothing to do
  }

  void DoExecute() override
  {
    unsigned int utmZone = 0;
    otb::SpatialReference::hemisphere hem;


    otb::SpatialReference::UTMFromGeoPoint(GetParameterFloat("lon"),
                                                     GetParameterFloat("lat"),utmZone,hem);

    SetParameterInt("utm",utmZone);
  }

};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::ObtainUTMZoneFromGeoPoint)
