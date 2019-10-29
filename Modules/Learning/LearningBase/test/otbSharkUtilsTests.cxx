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

#include "itkMacro.h"
#include "otbSharkUtils.h"


int otbSharkNormalizeLabels(int itkNotUsed(argc), char* itkNotUsed(argv)[])
{
  std::vector<unsigned int> inLabels           = {2, 2, 3, 20, 1};
  std::vector<unsigned int> expectedDictionary = {1, 2, 3, 20};
  std::vector<unsigned int> expectedLabels     = {1, 1, 2, 3, 0};

  auto                      newLabels = inLabels;
  std::vector<unsigned int> labelDict;
  otb::Shark::NormalizeLabelsAndGetDictionary(newLabels, labelDict);

  if (newLabels != expectedLabels)
  {
    std::cout << "Wrong new labels\n";
    for (size_t i = 0; i < newLabels.size(); ++i)
      std::cout << "Got " << newLabels[i] << " expected " << expectedLabels[i] << '\n';

    return EXIT_FAILURE;
  }

  if (labelDict != expectedDictionary)
  {
    std::cout << "Wrong dictionary\n";
    for (size_t i = 0; i < labelDict.size(); ++i)
      std::cout << "Got " << labelDict[i] << " expected " << expectedDictionary[i] << '\n';

    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
