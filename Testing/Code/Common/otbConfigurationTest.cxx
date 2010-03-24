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
#include "otbI18n.h"
#include <fstream>
#include "otbConfigurationFile.h"

int otbConfigurationTest(int argc, char * argv[])
{
  otbI18nMacro();

//   std::cout << "begin config test" << std::endl;
  typedef otb::ConfigurationFile        ConfigurationType;
  
  //Instantiation
//   ConfigurationType::Pointer conf = ConfigurationType::GetInstance();
  ConfigurationType::Pointer conf = ConfigurationType::GetInstance();
//   conf->Load();
  std::string lang = conf->GetParameter<std::string>("OTB_LANG");
  
  std::cout << conf << std::endl;
  
  if( lang.compare("fr_FR.UTF-8") != 0  && lang.compare("en_EN.UTF-8") !=0 )
  {
    std::cout << "Locale language " << lang << std::endl;
    return EXIT_FAILURE;
  }
  
  return EXIT_SUCCESS;
}
