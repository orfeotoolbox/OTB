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
#include "otbMath.h"
#include "otbOrientationPathFunction.h"
#include "itkPolyLineParametricPath.h"
#include "itkMacro.h"

int otbOrientationPath(int itkNotUsed(argc), char* argv[])
{
  double Theta((double)::atof(argv[1]));

  const unsigned int                             Dimension = 2;
  typedef itk::PolyLineParametricPath<Dimension> PathType;
  typedef otb::OrientationPathFunction<PathType> FunctionType;
  typedef FunctionType::RealType                 RealType;

  PathType::ContinuousIndexType cindex;
  PathType::Pointer             pathElt = PathType::New();

  Theta *= otb::CONST_PI / 180.;

  pathElt->Initialize();

  cindex[0] = 30;
  cindex[1] = 30;
  pathElt->AddVertex(cindex);
  cindex[0] += 100 * std::cos(Theta);
  cindex[1] += 100 * std::sin(Theta);
  pathElt->AddVertex(cindex);

  FunctionType::Pointer function = FunctionType::New();
  function->SetInputPath(pathElt);

  RealType ResultTheta = function->Evaluate();
  std::cout.precision(10);
  std::cout << "Orientation found : " << ResultTheta << std::endl;
  if (std::abs(static_cast<double>(ResultTheta - Theta)) >= 10e-15)
  {
    std::cout << "Error in Theta estimation:" << (ResultTheta - Theta) << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
