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

namespace otb
{
/** Initialize the singleton */
ConfigurationFile::Pointer ConfigurationFile::Instance = NULL;

ConfigurationFile
::ConfigurationFile()
{
  m_OTBConfig = NULL;

  std::string OTBBinDir(OTB_CONFIG);
  try
    {
    m_OTBConfig = new ConfigFile(OTBBinDir + "/otb.conf");
    }
//  catch (ConfigFile::file_not_found& e)
  catch (...)
    {
//    otbMsgDevMacro(<< "Error - File '" << e.filename << "' not found.");
    otbMsgDevMacro(<< "Error - File not found.");
    }
}

ConfigurationFile
::~ConfigurationFile()
{
  delete m_OTBConfig;
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
