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
#include "otbConfigurationFile.h"
#include "otbMacro.h"
#include "itksys/SystemTools.hxx"

namespace otb
{
/** Initialize the singleton */
ConfigurationFile::Pointer ConfigurationFile::Instance = NULL;

ConfigurationFile
::ConfigurationFile()
{
  m_OTBConfig = NULL;

  const char* envVar = itksys::SystemTools::GetEnv("OTB_CONFIG_FILE");
  if (envVar)
    {
    if (!itksys::SystemTools::FileExists(envVar, true))
      {
      itkWarningMacro(<< "When loading the OTB configuration file, "
                         "the OTB_CONFIG env. var was found, but points a non-existant file : "
                      << envVar);
      }
    else
      {
      m_OTBConfig = LoadSilent(envVar);
      }
    }

  if ( !m_OTBConfig )
    {
    std::string OTBBinDir(OTB_CONFIG);
    m_OTBConfig = LoadSilent(OTBBinDir + "/otb.conf");
    }
}

ConfigurationFile
::~ConfigurationFile()
{
  delete m_OTBConfig;
}

ConfigFile *
ConfigurationFile
::LoadSilent(std::string path)
{
  ConfigFile * config = NULL;
  try
    {
    config = new ConfigFile(path);
    }
  catch (...)
    {
    otbMsgDevMacro(<< "Unable to load config file from " << path );
    }

  return config;
}


ConfigFile *
ConfigurationFile
::GetOTBConfig()
{
  return m_OTBConfig;
}

bool
ConfigurationFile
::IsValid() const
{
  return m_OTBConfig != NULL;
}

std::string
ConfigurationFile
::GetDEMDirectory() const
{
  std::string ret;
  try
  {
    std::string fromConfigFile = GetParameter<std::string>("OTB_DEM_DIRECTORY");

    if ( itksys::SystemTools::FileExists(fromConfigFile.c_str())
         && itksys::SystemTools::FileIsDirectory(fromConfigFile.c_str()) )
      {
      ret = fromConfigFile;
      }
  }
  catch (itk::ExceptionObject& ex)
  {
    otbMsgDevMacro(<< "Error caught when accessing OTB_DEM_DIRECTORY in the config file."
                   << "The error was " << ex);
  }
  return ret;
}

std::string
ConfigurationFile
::GetGeoidFile() const
{
  std::string ret;
  try
  {
    std::string fromConfigFile = GetParameter<std::string>("OTB_GEOID_FILE");

    if ( itksys::SystemTools::FileExists(fromConfigFile.c_str())
         && !itksys::SystemTools::FileIsDirectory(fromConfigFile.c_str()) )
      {
      ret = fromConfigFile;
      }
  }
  catch (itk::ExceptionObject& ex)
  {
    otbMsgDevMacro(<< "Error caught when accessing OTB_GEOID_FILE in the config file."
                   << "The error was " << ex);
  }
  return ret;
}

ConfigurationFile::Pointer
ConfigurationFile
::GetInstance()
{
  if (!Instance)
    {
    Instance = Self::New();
    }
  return Instance;
}

void
ConfigurationFile
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent;
  os << (*m_OTBConfig);

}
} // end namespace otb
