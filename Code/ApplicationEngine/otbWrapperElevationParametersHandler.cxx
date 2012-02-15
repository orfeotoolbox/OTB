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

namespace otb
{
namespace Wrapper
{

void ElevationParametersHandler::AddElevationParameters(Application::Pointer app,
                                                         const std::string & key)
{
  app->AddParameter(ParameterType_Choice, key, "Elevation management");
  app->SetParameterDescription(key,
                               "This group of parameters allows to manage elevation values. Supported formats are SRTM, DTED or any geotiff processed by the DEM import application");
  app->MandatoryOff(key);

  // DEM directory
  std::ostringstream oss;
  oss << key<<".dem";
  app->AddChoice(oss.str(),"DEM directory");
  oss << ".path";
  app->AddParameter(ParameterType_Directory, oss.str(), "DEM directory");
  app->SetParameterDescription(oss.str(),
                               "This parameter allows to select a directory containing Digital Elevation Model tiles");
  app->SetParameterString(oss.str(), otb::ConfigurationFile::GetInstance()->GetDEMDirectory());

   // Geoid file
  oss.str("");
  oss << key<<".dem.geoid";
  app->AddParameter(ParameterType_InputFilename, oss.str(), "Geoid File");
  app->SetParameterDescription(oss.str(),"Use a geoid grid to get the height above the ellipsoid used");
  app->SetParameterString(oss.str(), otb::ConfigurationFile::GetInstance()->GetGeoidFile());
  app->MandatoryOff(oss.str());

  // Average elevation
  oss.str("");
  oss << key <<".average";
  app->AddChoice(oss.str(), "Average Elevation");
  oss << ".value";
  app->AddParameter(ParameterType_Float, oss.str(), "Average Elevation");
  app->SetParameterDescription(oss.str(),"This parameter allows to pick up an average elevation for all the points of the image.");
  app->SetDefaultParameterFloat(oss.str(), 0.);

 // TODO : not implemented yet
 //   // Tiff image
 //   oss << ".tiff";
 //   app->AddChoice(oss.str(), "Tiff file");
 //   app->AddParameter(ParameterType_InputImage, oss.str(), "Tiff file");
 //   app->SetParameterDescription(oss.str(),"Tiff file used to get elevation for each location in the image");

  // Set the default value
  app->SetParameterString(key, "average");
}


/**
 *
 * Get the Average elevation value
 **/
const std::string
ElevationParametersHandler::GetDEMDirectory(const Application::Pointer app, const std::string& key)
{
  std::ostringstream oss;
  oss << key <<".dem.path";
  return app->GetParameterString(oss.str());
}

/**
 *
 * Get the Average elevation value
 */
float
ElevationParametersHandler::GetAverageElevation(const Application::Pointer app, const std::string& key)
{
  std::ostringstream oss;
  oss << key <<".average.value";
  return app->GetParameterFloat(oss.str());
}

/**
 * Get the Geoid file
 */
const std::string
ElevationParametersHandler::GetGeoidFile(const Application::Pointer app, const std::string& key)
{
  std::ostringstream oss;
  oss << key <<".dem.geoid";
  if (IsGeoidUsed(app, key))
    {
    return app->GetParameterString(oss.str());
    }
  
  return "";
}

/**
 *
 * Get the Elevation mode choosen by the user
 */
ElevationType
ElevationParametersHandler::GetElevationType(const Application::Pointer app, const std::string& key)
{
  switch(app->GetParameterInt(key))
    {
    case Elevation_DEM:
      return Elevation_DEM;
      break;
    case Elevation_Average:
      return Elevation_Average;
      break;
     //  case Elevation_Tiff:
     //   return Eleavation_Tiff;
     //   break;
    default:
      // We should never be here
      itkGenericExceptionMacro(<< "Unknown Elevation type");
    }
}


/**
 *
 * Is Geoid used
 */
bool
ElevationParametersHandler::IsGeoidUsed(const Application::Pointer app, const std::string& key)
{
  std::ostringstream geoidKey;
  geoidKey<< key<<".dem.geoid";
  
  return app->IsParameterEnabled(geoidKey.str()) && app->HasValue(geoidKey.str());
}

bool
ElevationParametersHandler::IsElevationEnabled(const Application::Pointer app, const std::string & key)
{
  return app->IsParameterEnabled(key);
}

}// End namespace Wrapper
}// End namespace otb
