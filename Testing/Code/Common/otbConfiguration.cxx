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
#include "otbConfiguration.h"

int otbConfiguration(int argc, char * argv[])
{

  typedef otb::Configuration        ConfigurationType;
  
  //Instantiation
  ConfigurationType::Pointer conf = ConfigurationType::New();
  
//   conf->Load();
  
  std::string lang = conf->GetLanguage(); 
  
  if (lang != "fr")
      return EXIT_FAILURE;
  
  std::cout << "config language " << lang << std::endl;
  
  return EXIT_SUCCESS;
}
