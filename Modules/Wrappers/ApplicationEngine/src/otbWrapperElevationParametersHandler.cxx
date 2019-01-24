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

#include "otbWrapperElevationParametersHandler.h"
#include "otbDEMHandler.h"

namespace otb
{
namespace Wrapper
{

void ElevationParametersHandler::AddElevationParameters(Application::Pointer app,
                                                         const std::string & key)
{
  app->AddParameter(ParameterType_Group, key, "Elevation management");
  app->SetParameterDescription(key,
                               "This group of parameters allows managing elevation values. Supported formats are SRTM, DTED or any geotiff. DownloadSRTMTiles application could be a useful tool to list/download tiles related to a product.");

  // DEM directory
  std::ostringstream oss;
  oss << key<<".dem";
  app->AddParameter(ParameterType_Directory, oss.str(), "DEM directory");
  app->SetParameterDescription(oss.str(),
                               "This parameter allows selecting a directory containing Digital Elevation Model files. Note that this directory should contain only DEM files. Unexpected behaviour might occurs if other images are found in this directory.");
  app->MandatoryOff(oss.str());

  std::string demDirFromConfig = otb::ConfigurationManager::GetDEMDirectory();

  if(demDirFromConfig!="")
    {
    app->SetParameterString(oss.str(), demDirFromConfig);
    app->EnableParameter(oss.str());
    }
  else
    {
    app->DisableParameter(oss.str());
    }

   // Geoid file
  oss.str("");
  oss << key<<".geoid";
  app->AddParameter(ParameterType_InputFilename, oss.str(), "Geoid File");
  app->SetParameterDescription(oss.str(),"Use a geoid grid to get the height "
    "above the ellipsoid in case there is no DEM available, no coverage for "
    "some points or pixels with no_data in the DEM tiles. A version of the "
    "geoid can be found on the OTB website"
    "(https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb-data/blob/master/Input/DEM/egm96.grd).");
  app->MandatoryOff(oss.str());

  std::string geoidFromConfig = otb::ConfigurationManager::GetGeoidFile();

  if(geoidFromConfig!="")
    {
    app->SetParameterString(oss.str(), geoidFromConfig);
    app->EnableParameter(oss.str());
    }
  else
    {
    app->DisableParameter(oss.str());
    }

  // Average elevation
  oss.str("");
  oss << key <<".default";
  app->AddParameter(ParameterType_Float, oss.str(), "Default elevation");
  app->SetParameterDescription(oss.str(),"This parameter allows setting the default height above ellipsoid when there is no DEM available, no coverage for some points or pixels with no_data in the DEM tiles, and no geoid file has been set. This is also used by some application as an average elevation value.");
  app->SetDefaultParameterFloat(oss.str(), 0.);

 // TODO : not implemented yet
 //   // Tiff image
 //   oss << ".tiff";
 //   app->AddChoice(oss.str(), "Tiff file");
 //   app->AddParameter(ParameterType_InputImage, oss.str(), "Tiff file");
 //   app->SetParameterDescription(oss.str(),"Tiff file used to get elevation for each location in the image");

}

void ElevationParametersHandler::SetupDEMHandlerFromElevationParameters(const Application::Pointer app, const std::string& key)
{
  // Set default elevation
  otb::DEMHandler::Instance()->SetDefaultHeightAboveEllipsoid(GetDefaultElevation(app,key));

  std::ostringstream oss;
  oss<<"Elevation management: setting default height above ellipsoid to " << GetDefaultElevation(app,key) << " meters"<<std::endl;

  app->GetLogger()->Info(oss.str());

  // Set geoid if available
  if(IsGeoidUsed(app,key))
    {
    oss.str("");
    oss<<"Elevation management: using geoid file ("<<GetGeoidFile(app,key)<<")"<<std::endl;
    otb::DEMHandler::Instance()->OpenGeoidFile(GetGeoidFile(app,key));
    app->GetLogger()->Info(oss.str());
    }

  // Set DEM directory if available
  if(IsDEMUsed(app,key))
    {
    std::string demDirectory = GetDEMDirectory(app,key);
    if(otb::DEMHandler::Instance()->IsValidDEMDirectory(demDirectory.c_str()))
      {
      oss.str( "" );
      oss << "Elevation management: using DEM directory (" << demDirectory << ")" << std::endl;
      otb::DEMHandler::Instance()->OpenDEMDirectory( demDirectory );
      app->GetLogger()->Info( oss.str() );
      }
    else
      {
      oss.str( "" );
      oss << "DEM directory : " << demDirectory << " is not a valid DEM directory";
      app->GetLogger()->Warning( oss.str() );
      }
    }
}

/**
 *
 * Get the Average elevation value
 */
float
ElevationParametersHandler::GetDefaultElevation(const Application::Pointer app, const std::string& key)
{
  std::ostringstream oss;
  oss << key <<".default";
  return app->GetParameterFloat(oss.str());
}

/**
 * Get the Geoid file
 */
const std::string
ElevationParametersHandler::GetGeoidFile(const Application::Pointer app, const std::string& key)
{
  std::ostringstream oss;
  oss << key <<".geoid";
  if (IsGeoidUsed(app, key))
    {
    return app->GetParameterString(oss.str());
    }

  return "";
}

/**
 * Is Geoid used
 */
bool
ElevationParametersHandler::IsGeoidUsed(const Application::Pointer app, const std::string& key)
{
  std::ostringstream geoidKey;
  geoidKey<< key<<".geoid";

  return app->IsParameterEnabled(geoidKey.str()) && app->HasValue(geoidKey.str());
}

/**
 *
 * Is Geoid used
 */
bool
ElevationParametersHandler::IsDEMUsed(const Application::Pointer app, const std::string& key)
{
  std::ostringstream demKey;
  demKey<< key<<".dem";

  return app->IsParameterEnabled(demKey.str()) && app->HasValue(demKey.str());
}


const std::string
ElevationParametersHandler::GetDEMDirectory(const Application::Pointer app, const std::string& key)
{
  std::ostringstream oss;
  oss << key <<".dem";
  if (IsDEMUsed(app, key))
    {
    return app->GetParameterString(oss.str());
    }

  return "";
}


}// End namespace Wrapper
}// End namespace otb
