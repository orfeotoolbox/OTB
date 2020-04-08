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

#include <fstream>
#include <iomanip>
#include "itkMacro.h"

#include "otbTerraSarBrightnessFunctor.h"

int otbTerraSarBrightnessFunctor(int itkNotUsed(argc), char* argv[])
{
  char*  outFilename = argv[5];
  double calFact     = atof(argv[4]);

  typedef double                   ScalarType;
  typedef std::complex<ScalarType> ComplexType;

  typedef otb::Functor::TerraSarBrightnessFunctor<ScalarType, ScalarType> FunctorType;

  FunctorType funct;
  funct.SetCalibrationFactor(calFact);
  funct.SetResultsInDecibels(false);

  ScalarType  inPix = static_cast<ScalarType>(atof(argv[1]));
  ComplexType inCplxPix(static_cast<ScalarType>(atof(argv[2])), static_cast<ScalarType>(atof(argv[3])));

  std::ofstream file;
  file.open(outFilename);

  file << std::fixed << std::setprecision(10);

  file << "Scalar pixel : " << inPix << " -> " << funct.operator()(inPix) << std::endl;

  file << "Complex pixel : " << inCplxPix << " -> " << funct.operator()(inCplxPix) << std::endl;

  file.close();

  return EXIT_SUCCESS;
}
