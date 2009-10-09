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
#include "itkExceptionObject.h"
#include "otbMacro.h"
#include <fstream>
#include "otbConfigurationFile.h"

int otbConfigurationTest(int argc, char * argv[])
{
//   std::cout << "begin config test" << std::endl;
  typedef otb::ConfigurationFile        ConfigurationType;
  
  //Instantiation
//   ConfigurationType::Pointer conf = ConfigurationType::GetInstance();
  ConfigurationType::Pointer conf = ConfigurationType::New();
//   conf->Load();
  
  std::string lang = conf->GetLanguage();
  
  std::cout << "config language " << lang << std::endl;
  
  if (lang != "fr")
      return EXIT_FAILURE;
  
  return EXIT_SUCCESS;
}
