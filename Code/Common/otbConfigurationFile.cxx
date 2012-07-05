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
                         "the OTB_CONFIG_FILE env. var was found, but points a non-existant file : "
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

bool
ConfigurationFile
::HasKey(const std::string& key) const
{
  return m_OTBConfig != NULL && m_OTBConfig->keyExists(key);
}

std::string
ConfigurationFile
::GetDEMDirectory() const
{
  std::string ret;

  if (HasKey("OTB_DEM_DIRECTORY"))
    {
    std::string fromConfigFile;
    try
      {
      fromConfigFile = GetParameter<std::string> ("OTB_DEM_DIRECTORY");
      }
    catch (itk::ExceptionObject& ex)
      {
      std::stringstream oss;
      oss << "Error caught when accessing OTB_DEM_DIRECTORY in the config file.";
      oss << "The error was " << ex;
      otbMsgDevMacro( << oss.str() );
      }

    if ( !itksys::SystemTools::FileExists(fromConfigFile.c_str()) )
      {
      otbMsgDevMacro( << "Error caught when accessing OTB_DEM_DIRECTORY in the config file: "
                      << fromConfigFile << " does not exist on the file system" );
      }
    else if ( !itksys::SystemTools::FileIsDirectory(fromConfigFile.c_str()) )
      {
      otbMsgDevMacro( << "Error caught when accessing OTB_DEM_DIRECTORY in the config file: "
                      << fromConfigFile << " is not a directory" );
      }
    else
      {
      ret = fromConfigFile;
      }
    }
  return ret;
}

std::string
ConfigurationFile
::GetGeoidFile() const
{
  std::string ret;

  if (HasKey("OTB_GEOID_FILE"))
    {
    std::string fromConfigFile;
    try
      {
      fromConfigFile = GetParameter<std::string> ("OTB_GEOID_FILE");
      }
    catch (itk::ExceptionObject& ex)
      {
      std::stringstream oss;
      oss << "Error caught when accessing OTB_GEOID_FILE in the config file.";
      oss << "The error was " << ex;
      otbMsgDevMacro( << oss.str() );
      }

    if ( !itksys::SystemTools::FileExists(fromConfigFile.c_str()) )
      {
      otbMsgDevMacro( << "Error caught when accessing OTB_GEOID_FILE in the config file: "
                      << fromConfigFile << " does not exist on the file system" );
      }
    else if ( itksys::SystemTools::FileIsDirectory(fromConfigFile.c_str()) )
      {
      otbMsgDevMacro( << "Error caught when accessing OTB_GEOID_FILE in the config file: "
                      << fromConfigFile << " is a directory" );
      }
    else
      {
      ret = fromConfigFile;
      }
    }
  return ret;
}

::itksysFundamentalType_UInt64
ConfigurationFile
::GetAvailableRAMInBytes() const
{
  ::itksysFundamentalType_UInt64 availableRAMInBytes = 0;

  otbMsgDevMacro(<< "Retrieving available RAM size from configuration");
  // Retrieve it from the configuration
  try
    {
    typedef otb::ConfigurationFile ConfigurationType;
    ConfigurationType::Pointer conf = ConfigurationType::GetInstance();
    
    availableRAMInBytes = conf->GetParameter< ::itksysFundamentalType_UInt64 >(
      "OTB_STREAM_MAX_SIZE_BUFFER_FOR_STREAMING");
    }
  catch(...)
    {
    // We should never have to go here if the configuration file is
    // correct and found.
    // In case it is not, fallback on the cmake
    // defined constants.
    return OTB_STREAM_MAX_SIZE_BUFFER_FOR_STREAMING;
    }
  return availableRAMInBytes;
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
