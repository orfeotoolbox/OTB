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

#include "otbRectangle.h"

#include <iostream>
#include <fstream>

int otbRectangle(int itkNotUsed(argc), char* argv[])
{
  const char* outfname = argv[1];

  typedef otb::Rectangle<>                   RectangleType;
  typedef RectangleType::ContinuousIndexType ContinuousIndexType;

  // Instantiating object
  RectangleType::Pointer rectangle1 = RectangleType::New();

  ContinuousIndexType newVertex;

  newVertex[0] = atof(argv[2]);
  newVertex[1] = atof(argv[3]);
  rectangle1->AddVertex(newVertex);

  newVertex[0] = atof(argv[4]);
  newVertex[1] = atof(argv[5]);
  rectangle1->AddVertex(newVertex);

  rectangle1->SetWidth(atof(argv[6]));
  rectangle1->SetOrientation(atof(argv[7]));

  /** Inside the rectangle test*/
  ContinuousIndexType InsideVertex;
  InsideVertex[0] = atof(argv[8]);
  InsideVertex[1] = atof(argv[9]);

  std::ofstream outfile(outfname);

  if (rectangle1->IsInside(InsideVertex))
    outfile << "The point " << InsideVertex << " Is Inside the rectangle" << std::endl;
  else
    outfile << "The point " << InsideVertex << " Is Outside the rectangle" << std::endl;

  outfile << "region Size" << rectangle1->GetBoundingRegion().GetSize() << std::endl;
  outfile << "region Origin" << rectangle1->GetBoundingRegion().GetIndex() << std::endl;

  outfile.close();

  return EXIT_SUCCESS;
}
