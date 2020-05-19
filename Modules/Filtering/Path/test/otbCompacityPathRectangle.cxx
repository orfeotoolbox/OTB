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


#include <cstdlib>
#include "otbCompacityPathFunction.h"
#include "itkPolyLineParametricPath.h"
#include "itkMacro.h"
#include "otbMath.h"

int otbCompacityPathRectangle(int itkNotUsed(argc), char* argv[])
{
  double A((double)::atof(argv[1]));
  double B((double)::atof(argv[2]));

  const unsigned int                             Dimension = 2;
  typedef itk::PolyLineParametricPath<Dimension> PathType;
  typedef otb::CompacityPathFunction<PathType>   FunctionType;
  typedef FunctionType::RealType                 RealType;

  PathType::ContinuousIndexType cindex;
  PathType::Pointer             pathElt = PathType::New();

  if (A < 0)
  {
    std::cout << "retangle must be greater than 0.0 !" << std::endl;
    return EXIT_FAILURE;
  }

  if (B < 0)
  {
    std::cout << "rectangle must be greater than 0.0 !" << std::endl;
    return EXIT_FAILURE;
  }

  pathElt->Initialize();

  cindex[0] = 100;
  cindex[1] = 100;
  pathElt->AddVertex(cindex);
  cindex[0] = 100 + A;
  cindex[1] = 100;
  pathElt->AddVertex(cindex);
  cindex[0] = 100 + A;
  cindex[1] = 100 + B;
  pathElt->AddVertex(cindex);
  cindex[0] = 100;
  cindex[1] = 100 + B;
  pathElt->AddVertex(cindex);

  FunctionType::Pointer function = FunctionType::New();
  function->SetInputPath(pathElt);

  RealType Result = function->Evaluate();
  std::cout << "Compacity result: " << Result << std::endl;

  RealType Error;
  Error = std::abs(Result - static_cast<RealType>(otb::CONST_PI * A * B / (A + B) / (A + B)));

  if (Error > 1.E-9)
  {
    std::cout << "Error in estimation !" << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
