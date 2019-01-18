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
#include <fstream>
#include <string>


#include "otbMacro.h"

int otbCompareAsciiTestsEpsilon3_WhiteSpace(int itkNotUsed(argc), char * argv[])
{
  std::cout << "ASCII COMPARISON TEST: it will use directly the baseline and input files in OTB-Data "
      "to avoid problem with otb-bot." << std::endl;

  // Open input file
  std::ifstream fileTestInput;
  fileTestInput.open(argv[1]);

  // Transfer to temporary object
  std::string strFileTest;
  std::vector<std::string> listLineFileTest;
  while (std::getline(fileTestInput, strFileTest))
    {
    listLineFileTest.push_back(strFileTest);
    }
  fileTestInput.close();

  // Write temporary object without modification
  std::ofstream fileTestOutput;
  fileTestOutput.open(argv[2]);
  for (unsigned int i=0; i < listLineFileTest.size(); ++i)
    {
    fileTestOutput << listLineFileTest[i];
    }
  fileTestOutput.close();

  return EXIT_SUCCESS;
}
