/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <iostream>
#include <cstdlib>
#include "otbConfigurationManager.h"

int otbConfigurationManagerTest(int argc, char* argv[])
{
  if (argc < 1)
  {
    std::cerr << "Usage: " << argv[0] << " refMaxRAMHint [refDEMDir refGeoidFile]" << std::endl;
    return EXIT_FAILURE;
  }

  bool failed = false;

  otb::ConfigurationManager::RAMValueType refMaxRAMHint = atoi(argv[1]);

  std::string refDEMDir    = (argc > 2) ? argv[2] : "";
  std::string refGeoidFile = (argc > 3) ? argv[3] : "";

  otb::ConfigurationManager::RAMValueType maxRam = otb::ConfigurationManager::GetMaxRAMHint();

  std::cout << "GetMaxRAMHint(): " << maxRam << std::endl;

  if (maxRam != refMaxRAMHint)
  {
    failed = true;
    std::cerr << "GetMaxRAMHint(): Value differs from expected value (" << refMaxRAMHint << ")" << std::endl;
  }

  std::string demDir = otb::ConfigurationManager::GetDEMDirectory();

  std::cout << "GetDEMDirectory(): " << demDir << std::endl;

  if (demDir != refDEMDir)
  {
    failed = true;
    std::cerr << "GetDEMDirectory(): Value differs from expected value (" << refDEMDir << ")" << std::endl;
  }


  std::string geoidFile = otb::ConfigurationManager::GetGeoidFile();

  std::cout << "GetGeoidFile(): " << geoidFile << std::endl;

  if (geoidFile != refGeoidFile)
  {
    failed = true;
    std::cerr << "GetGeoidFile(): Value differs from expected value (" << refGeoidFile << ")" << std::endl;
  }

  if (failed)
    return EXIT_FAILURE;

  return EXIT_SUCCESS;
}
