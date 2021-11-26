/*
 * Copyright (C) 2005-2021 Centre National d'Etudes Spatiales (CNES)
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


#include "otbMPIConfig.h"
#include <iostream>
#include "itkMultiThreader.h"

int otbMPIConfigTest(int argc, char* argv[])
{

  // MPI Configuration
  typedef otb::MPIConfig MPIConfigType;
  MPIConfigType::Pointer config = MPIConfigType::Instance();
  config->Init(argc, argv, true);

  // Test
  std::cout << "OTB MPI config test. I am process " << config->GetMyRank() << " among " << config->GetNbProcs() << "." << std::endl;

  return EXIT_SUCCESS;
}
