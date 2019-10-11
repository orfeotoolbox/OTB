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


#include "otbObjectList.h"
#include "otbPolygon.h"
#include "otbMacro.h"
#include <vector>

int otbObjectList2(int itkNotUsed(argc), char* itkNotUsed(argv)[])
{

  typedef otb::Polygon<double>         PolygonType;
  typedef otb::ObjectList<PolygonType> PolygonListType;

  PolygonType::Pointer     polygon     = PolygonType::New();
  PolygonListType::Pointer listPolygon = PolygonListType::New();

  otbTestingCheckNotValidCommand(listPolygon->Erase(1));
  otbTestingCheckValidCommand(listPolygon->Clear());
  otbTestingCheckNotValidCommand(listPolygon->SetNthElement(0, polygon));
  otbTestingCheckNotValidCommand(listPolygon->SetNthElement(2, polygon));
  otbTestingCheckNotValidCommand(listPolygon->GetNthElement(2));

  std::vector<int> tab;
  tab.reserve(3);
  otbControlConditionTestMacro(tab.capacity() != 3, "Check std::vector Capacity");
  otbControlConditionTestMacro(tab.size() != 0, "Check std::vector Size");
  tab.resize(2);
  otbControlConditionTestMacro(tab.capacity() != 3, "Check std::vector Capacity");
  otbControlConditionTestMacro(tab.size() != 2, "Check std::vector Size");

  listPolygon->Reserve(3);
  otbControlConditionTestMacro(listPolygon->Capacity() != 3, "Check Reserve and Capacity coherence");
  listPolygon->Resize(2);
  otbControlConditionTestMacro(listPolygon->Size() != 2, "Check Resize and Size coherence");

  otbTestingCheckValidCommand(listPolygon->SetNthElement(0, polygon));
  otbTestingCheckNotValidCommand(listPolygon->SetNthElement(2, polygon));
  return EXIT_SUCCESS;
}
