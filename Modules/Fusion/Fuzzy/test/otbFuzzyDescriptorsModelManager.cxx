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


#include "otbFuzzyDescriptorsModelManager.h"
#include <iostream>
#include <stdlib.h>
#include "itkMacro.h"


int otbFuzzyDescriptorsModelManagerTest(int itkNotUsed(argc), char* argv[])
{
  otb::FuzzyDescriptorsModelManager::DescriptorsModelType model;
  otb::FuzzyDescriptorsModelManager::ParameterType        param;
  otb::FuzzyDescriptorsModelManager::PairType             couple;

  couple.first = "desc1";
  param.push_back(0);
  param.push_back(1);
  couple.second = param;
  otb::FuzzyDescriptorsModelManager::AddDescriptor(couple, model);

  param.clear();
  param.push_back(9);
  param.push_back(8);
  param.push_back(7);
  param.push_back(6);

  otb::FuzzyDescriptorsModelManager::AddDescriptor("desc2", param, model);

  otb::FuzzyDescriptorsModelManager::Save(argv[1], model);

  otb::FuzzyDescriptorsModelManager::DescriptorsModelType modelRead;
  modelRead = otb::FuzzyDescriptorsModelManager::Read(argv[1]);

  otb::FuzzyDescriptorsModelManager::DescriptorListType list;
  list = otb::FuzzyDescriptorsModelManager::GetDescriptorList(model);

  if (modelRead != model)
  {
    std::cout << "Written model and read model from the written one differs." << std::endl;
    std::cout << "Input is:" << std::endl;
    otb::FuzzyDescriptorsModelManager::Print(model);
    std::cout << "Output is:" << std::endl;
    otb::FuzzyDescriptorsModelManager::Print(modelRead);
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
