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
                               "This parameter allows selecting a directory containing Digital Elevation Model tiles");
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
  app->SetParameterDescription(oss.str(),"Use a geoid grid to get the height above the ellipsoid in case there is no DEM available, no coverage for some points or pixels with no_data in the DEM tiles. A version of the geoid can be found on the OTB website (http://hg.orfeo-toolbox.org/OTB-Data/raw-file/404aa6e4b3e0/Input/DEM/egm96.grd).");
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
    oss.str("");
    oss<<"Elevation management: using DEM directory ("<<GetDEMDirectory(app,key)<<")"<<std::endl;
    otb::DEMHandler::Instance()->OpenDEMDirectory(GetDEMDirectory(app,key));
    app->GetLogger()->Info(oss.str());
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
  std::ostringstream geoidKey;
  geoidKey<< key<<".dem";

  return app->IsParameterEnabled(geoidKey.str()) && app->HasValue(geoidKey.str());
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
