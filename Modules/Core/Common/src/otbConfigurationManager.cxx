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
#include "otbConfigurationManager.h"

#include "itksys/SystemTools.hxx"

#include <cstdlib>

namespace otb
{

std::string ConfigurationManager::GetDEMDirectory()
{
  std::string svalue;
  itksys::SystemTools::GetEnv("OTB_DEM_DIRECTORY",svalue);
  return svalue;
}

std::string ConfigurationManager::GetGeoidFile()
{
  std::string svalue;
  itksys::SystemTools::GetEnv("OTB_GEOID_FILE",svalue);
  return svalue;
}

ConfigurationManager::RAMValueType ConfigurationManager::GetMaxRAMHint()
{
  std::string svalue;
  
  RAMValueType value = 128;
  
  if(itksys::SystemTools::GetEnv("OTB_MAX_RAM_HINT",svalue))
    {

    unsigned long int tmp = strtoul(svalue.c_str(),ITK_NULLPTR,10);
    
    if(tmp)
      {
      value = static_cast<RAMValueType>(tmp);
      }
    }
  
  return value;

}
}
