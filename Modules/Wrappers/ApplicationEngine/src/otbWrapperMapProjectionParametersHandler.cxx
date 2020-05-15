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

#include "otbWrapperMapProjectionParametersHandler.h"

// Needed for methods relative to projections
#include "otbImageToGenericRSOutputParameters.h"
#include "otbWrapperInputImageListParameter.h"


namespace otb
{
namespace Wrapper
{

void MapProjectionParametersHandler::AddMapProjectionParameters(Application::Pointer app, const std::string& key)
{
  app->AddParameter(ParameterType_Choice, key, "Map Projection");
  app->SetParameterDescription(key, "Defines the map projection to be used.");

  // utm
  std::ostringstream oss;
  oss << key << ".utm";
  app->AddChoice(oss.str(), "Universal Trans-Mercator (UTM)");
  app->SetParameterDescription(oss.str(), "A system of transverse mercator projections dividing the surface of Earth between 80S and 84N latitude.");

  oss << ".zone";
  app->AddParameter(ParameterType_Int, oss.str(), "Zone number");
  app->SetParameterDescription(oss.str(),
                               "The zone number ranges from 1 to 60 and allows defining the transverse mercator projection (along with the hemisphere)");
  app->SetMinimumParameterIntValue(oss.str(), 1);
  app->SetDefaultParameterInt(oss.str(), 31);

  oss.str("");
  oss << key << ".utm"
      << ".northhem";
  app->AddParameter(ParameterType_Bool, oss.str(), "Northern Hemisphere");
  app->SetParameterDescription(oss.str(),
                               "The transverse mercator projections are defined by their zone number as well as the hemisphere. Activate this parameter if "
                               "your image is in the northern hemisphere.");


  // lambert2
  oss.str("");
  oss << key << ".lambert2";
  app->AddChoice(oss.str(), "Lambert II Etendu");
  app->SetParameterDescription(oss.str(), "This is a Lambert Conformal Conic projection mainly used in France.");

  // lambert93
  oss.str("");
  oss << key << ".lambert93";
  app->AddChoice(oss.str(), "Lambert93");
  app->SetParameterDescription(oss.str(), "This is a Lambert 93 projection mainly used in France.");

  // wgs84
  oss.str("");
  oss << key << ".wgs";
  app->AddChoice(oss.str(), "WGS 84");
  app->SetParameterDescription(oss.str(), "This is a Geographical projection");

  // epsg code
  oss.str("");
  oss << key << ".epsg";
  app->AddChoice(oss.str(), "EPSG Code");
  app->SetParameterDescription(oss.str(),
                               "This code is a generic way of identifying map projections, and allows specifying a large amount of them. See "
                               "www.spatialreference.org to find which EPSG code is associated to your projection;");

  oss << ".code";
  app->AddParameter(ParameterType_Int, oss.str(), "EPSG Code");
  app->SetParameterDescription(oss.str(), "See www.spatialreference.org to find which EPSG code is associated to your projection");
  app->SetDefaultParameterInt(oss.str(), 4326);

  app->SetParameterString(key, "utm");
}

/**
    * Helper method : Compute the ProjectionRef knowing the map
    * projection picked up by the user
    *
    */
const std::string MapProjectionParametersHandler::GetProjectionRefFromChoice(const Application::Pointer app, const std::string& key)
{
  std::ostringstream zoneKey;
  zoneKey << key << ".utm.zone";

  std::ostringstream hemKey;
  hemKey << key << ".utm.northhem";

  std::ostringstream epsgKey;
  epsgKey << key << ".epsg.code";

  // Get the user choice
  switch (app->GetParameterInt(key))
  {
  case Map_Utm:
  {
    return SpatialReference::FromUTM(app->GetParameterInt(zoneKey.str()),
                                     app->GetParameterInt(hemKey.str()) ? SpatialReference::hemisphere::north : SpatialReference::hemisphere::south)
        .ToWkt();
  }
  break;
  case Map_Lambert2:
  {
    return SpatialReference::FromDescription("EPSG:27572").ToWkt();
  }
  break;
  case Map_Lambert93:
  {
    return SpatialReference::FromDescription("EPSG:2154").ToWkt();
  }
  break;
  case Map_WGS84:
  {
    return SpatialReference::FromWGS84().ToWkt();
  }
  break;
  case Map_Epsg:
  {
    return SpatialReference::FromEPSG(app->GetParameterInt(epsgKey.str())).ToWkt();
  }
  break;
  }

  return "";
}

/**
    * Helper method : Compute the UTM parameters relative to an image Origin
    * Note: The key of the image must be set to be able to get the image.
    *       The key must be totally if the InputImageParameter belongs
    *       to a ParameterGroup, ie set io.in
    */
void MapProjectionParametersHandler::InitializeUTMParameters(Application::Pointer app, const std::string& imageKey, const std::string& mapKey)
{
  // Get the UTM params keys
  std::ostringstream zoneKey;
  zoneKey << mapKey << ".utm.zone";

  std::ostringstream hemKey;
  hemKey << mapKey << ".utm.northhem";

  // Compute the zone and the hemisphere if not UserValue defined
  if (!app->HasUserValue(zoneKey.str()) && app->HasValue(imageKey))
  {
    // Compute the Origin lat/long coordinate
    typedef otb::ImageToGenericRSOutputParameters<FloatVectorImageType> OutputParametersEstimatorType;
    OutputParametersEstimatorType::Pointer                              genericRSEstimator = OutputParametersEstimatorType::New();

    Parameter* param = app->GetParameterByKey(imageKey);
    if (dynamic_cast<InputImageParameter*>(param))
    {
      genericRSEstimator->SetInput(app->GetParameterImage(imageKey));
    }
    else if (dynamic_cast<InputImageListParameter*>(param))
    {
      genericRSEstimator->SetInput(app->GetParameterImageList(imageKey)->GetNthElement(0));
    }

    genericRSEstimator->SetOutputProjectionRef(SpatialReference::FromWGS84().ToWkt());
    genericRSEstimator->Compute();

    unsigned int                 zone(0);
    SpatialReference::hemisphere hem;

    otb::SpatialReference::UTMFromGeoPoint(genericRSEstimator->GetOutputOrigin()[0], genericRSEstimator->GetOutputOrigin()[1], zone, hem);

    // Update the UTM Gui fields
    app->SetParameterInt(zoneKey.str(), zone);
    app->SetParameterInt(hemKey.str(), (hem == SpatialReference::hemisphere::north));
    app->AutomaticValueOn(zoneKey.str());
    app->AutomaticValueOn(hemKey.str());
  }
}


} // End namespace Wrapper
} // End namespace otb
