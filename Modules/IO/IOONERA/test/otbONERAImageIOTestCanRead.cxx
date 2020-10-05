/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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


#include "otbONERAImageIO.h"
#include "itkMacro.h"
#include <iostream>

int otbONERAImageIOTestCanRead(int itkNotUsed(argc), char* argv[])
{
  otb::ONERAImageIO::Pointer lONERAImageIO = otb::ONERAImageIO::New();
  bool                       lCanRead      = lONERAImageIO->CanReadFile(argv[1]);
  if (lCanRead == false)
  {
    std::cerr << "Erreur otb::ONERAImageIO : impossible d'ouvrir l'image " << argv[1] << "." << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
