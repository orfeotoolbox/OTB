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
#include <iostream>
#include <cstdlib>
#include "otbConfigurationManager.h"

int otbConfigurationManagerTest(int argc, char * argv[])
{
  if(argc<1)
    {
    std::cerr<<"Usage: "<<argv[0]<<" refMaxRAMHint [refDEMDir refGeoidFile]"<<std::endl;
    return EXIT_FAILURE;
    }

  bool failed = false;

  otb::ConfigurationManager::RAMValueType refMaxRAMHint = atoi(argv[1]);

  std::string refDEMDir = (argc>2)?argv[2]:"";
  std::string refGeoidFile = (argc>3)?argv[3]:"";

  otb::ConfigurationManager::RAMValueType maxRam = otb::ConfigurationManager::GetMaxRAMHint();

  std::cout<<"GetMaxRAMHint(): "<<maxRam<<std::endl;

  if(maxRam != refMaxRAMHint)
    {
    failed = true;
    std::cerr<<"GetMaxRAMHint(): Value differs from expected value ("<<refMaxRAMHint<<")"<<std::endl;
    }

  std::string demDir = otb::ConfigurationManager::GetDEMDirectory();

  std::cout<<"GetDEMDirectory(): "<<demDir<<std::endl;

  if(demDir != refDEMDir)
    {
    failed = true;
    std::cerr<<"GetDEMDirectory(): Value differs from expected value ("<<refDEMDir<<")"<<std::endl;
    }


  std::string geoidFile = otb::ConfigurationManager::GetGeoidFile();

  std::cout<<"GetGeoidFile(): "<< geoidFile<<std::endl;

  if(geoidFile != refGeoidFile)
    {
    failed = true;
    std::cerr<<"GetGeoidFile(): Value differs from expected value ("<<refGeoidFile<<")"<<std::endl;
    }

  if(failed)
    return EXIT_FAILURE;

  return EXIT_SUCCESS;
}
