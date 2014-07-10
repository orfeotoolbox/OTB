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
#include "otbMacro.h"
#include <fstream>
#include "otbConfigurationFile.h"

int otbConfigurationTest(int itkNotUsed(argc), char ** itkNotUsed(argv))
{

//   std::cout << "begin config test" << std::endl;
  typedef otb::ConfigurationFile ConfigurationType;

  //Instantiation
//   ConfigurationType::Pointer conf = ConfigurationType::GetInstance();
  ConfigurationType::Pointer conf = ConfigurationType::GetInstance();
//   conf->Load();
  std::string lang;
  try
    {
    lang = conf->GetParameter<std::string> ("OTB_LANG");
    }
  catch (...)
    {
    std::cout << "Can't get parameter " << lang << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << conf << std::endl;

  if (lang.compare("fr_FR.UTF-8") != 0  && lang.compare("en_EN.UTF-8") != 0)
    {
    std::cout << "Locale language " << lang << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}


int otbConfigurationTestDEM(int argc, char *argv[])
{
  typedef otb::ConfigurationFile ConfigurationType;
  std::string demDir = otb::ConfigurationFile::GetInstance()->GetDEMDirectory();

  if (argc > 1)
    {
    // expected something
    std::string expected = argv[1];

    if (demDir.empty())
      {
      std::cerr << "GetDEMDirectory returned nothing but " << expected << " was expected" << std::endl;
      return EXIT_FAILURE;
      }
    else
      {
      if (demDir == expected)
        {
        return EXIT_SUCCESS;
        }
      else
        {
        std::cerr << "GetDEMDirectory returned " <<  demDir << " but " << expected << " was expected" << std::endl;
        return EXIT_FAILURE;
        }
      }
    }
  else
    {
    if (!demDir.empty())
      {
      std::cerr << "GetDEMDirectory returned " << demDir << " but nothing was expected" << std::endl;
      return EXIT_FAILURE;
      }
    else
      {
      return EXIT_SUCCESS;
      }
    }
  return EXIT_SUCCESS;
}
