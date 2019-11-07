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


#include <cstdlib>
#include "otbCompacityPathFunction.h"
#include "itkPolyLineParametricPath.h"
#include "itkMacro.h"
#include "otbMath.h"

int otbCompacityPathCircle(int itkNotUsed(argc), char* argv[])
{
  unsigned int NbOfPoints((unsigned int)::atoi(argv[1]));

  const unsigned int                             Dimension = 2;
  typedef itk::PolyLineParametricPath<Dimension> PathType;
  typedef otb::CompacityPathFunction<PathType>   FunctionType;
  typedef FunctionType::RealType                 RealType;

  PathType::ContinuousIndexType cindex;
  PathType::Pointer             pathElt = PathType::New();

  if (NbOfPoints < 2)
  {
    std::cout << "NbOfPoints must be greater than 2 !" << std::endl;
    return EXIT_FAILURE;
  }

  RealType deltaTheta;
  RealType Rho = 100.0;

  deltaTheta = 2. * otb::CONST_PI / static_cast<RealType>(NbOfPoints);

  pathElt->Initialize();

  for (unsigned int noTheta = 0; noTheta < NbOfPoints; noTheta++)
  {
    RealType Theta = deltaTheta * static_cast<RealType>(noTheta);

    cindex[0] = (Rho * std::cos(Theta));
    cindex[1] = (Rho * std::sin(Theta));
    pathElt->AddVertex(cindex);
  }

  FunctionType::Pointer function = FunctionType::New();
  function->SetInputPath(pathElt);

  RealType Result = function->Evaluate();
  std::cout << "Compacity result: " << Result << std::endl;

  RealType Error;
  Error = std::abs(Result - static_cast<RealType>(1.0));

  if (Error > 1.E-5)
  {
    std::cout << "Error in estimation !" << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
