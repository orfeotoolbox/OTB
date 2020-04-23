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

#include <iostream>
#include <fstream>

#include "otbMacro.h"

int otbCompareAsciiTestScientificNotation(int argc, char* argv[])
{
  if (argc != 3)
  {
    std::cerr << "Usage: " << argv[0];
    std::cerr << " referenceFile testFile" << std::endl;
    return EXIT_FAILURE;
  }

  std::ofstream fileRef;
  fileRef.open(argv[1]);
  fileRef << "1e15\n";
  fileRef << "2e15\n";
  fileRef << "3e15\n";
  fileRef << "4e15\n";
  fileRef << "5e-3\n";
  fileRef << "6e-3\n";
  fileRef << "7e-3\n";
  fileRef << "8e-3\n";
  fileRef << "A = 1e15\n";
  fileRef << "A = 2e15\n";
  fileRef << "A = 3e15\n";
  fileRef << "A = 4e15\n";
  fileRef << "A = 5e-3\n";
  fileRef << "A = 6e-3\n";
  fileRef << "A = 7e-3\n";
  fileRef << "A = 8e-3\n";
  fileRef.close();

  std::ofstream fileTest;
  fileTest.open(argv[2]);
  fileTest << "1e15\n";
  fileTest << "2E15\n";
  fileTest << "3e015\n";
  fileTest << "4E015\n";
  fileTest << "5e-3\n";
  fileTest << "6E-3\n";
  fileTest << "7e-03\n";
  fileTest << "8E-03\n";
  fileTest << "A = 1e15\n";
  fileTest << "A = 2E15\n";
  fileTest << "A = 3e015\n";
  fileTest << "A = 4E015\n";
  fileTest << "A = 5e-3\n";
  fileTest << "A = 6E-3\n";
  fileTest << "A = 7e-03\n";
  fileTest << "A = 8E-03\n";
  fileTest.close();

  return EXIT_SUCCESS;
}
