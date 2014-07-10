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


#include "otbObjectList.h"
#include "otbPolygon.h"
#include "otbMacro.h"
#include <vector>

int otbObjectList2(int itkNotUsed(argc), char ** itkNotUsed(argv))
{

  typedef otb::Polygon<double>         PolygonType;
  typedef otb::ObjectList<PolygonType> PolygonListType;

  PolygonType::Pointer     polygon = PolygonType::New();
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
