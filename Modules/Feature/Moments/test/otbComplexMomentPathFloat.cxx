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
#include <iomanip>
#include <fstream>
#include "otbImageFileReader.h"
#include "otbComplexMomentPathFunction.h"
#include "itkMacro.h"
#include "itkPolyLineParametricPath.h"

int otbComplexMomentPathFloat(int itkNotUsed(argc), char * argv[])
{
  unsigned int p((unsigned int) ::atoi(argv[1]));
  unsigned int q((unsigned int) ::atoi(argv[2]));
  const char * outputFilename  = argv[3];

  const unsigned int Dimension = 2;

  typedef itk::PolyLineParametricPath<Dimension>                PathType;
  typedef std::complex<float>                                   ComplexType;
  typedef otb::ComplexMomentPathFunction<PathType, ComplexType> CMType;

  // Draw a square
  PathType::ContinuousIndexType cindex;
  PathType::Pointer             pathElt = PathType::New();

  pathElt->Initialize();

  cindex[0] = 30;
  cindex[1] = 30;
  pathElt->AddVertex(cindex);
  cindex[0] = 30;
  cindex[1] = 130;
  pathElt->AddVertex(cindex);
  cindex[0] = 130;
  cindex[1] = 130;
  pathElt->AddVertex(cindex);
  cindex[0] = 130;
  cindex[1] = 30;
  pathElt->AddVertex(cindex);

  CMType::Pointer function = CMType::New();

  function->SetQ(q);
  function->SetP(p);

  ComplexType Result;

  std::ofstream outputStream(outputFilename);
  Result = function->Evaluate(*pathElt);
  outputStream << std::setprecision(10) << "function->Evaluate(Path)" << Result << std::endl;

  outputStream.close();

  return EXIT_SUCCESS;
}
