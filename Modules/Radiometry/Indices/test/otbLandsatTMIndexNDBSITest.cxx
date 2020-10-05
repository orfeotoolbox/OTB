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

#include "itkFixedArray.h"
#include "otbLandsatTMIndices.h"

int otbLandsatTMIndexNDBSI(int itkNotUsed(argc), char* argv[])
{
  typedef double OutputPixelType;
  typedef itk::FixedArray<double, 8> InputPixelType;

  typedef otb::Functor::LandsatTM::NDBSI<InputPixelType, OutputPixelType> FunctorType;

  FunctorType ndbsiFunct = FunctorType();

  double TM1  = (::atof(argv[1]));
  double TM2  = (::atof(argv[2]));
  double TM3  = (::atof(argv[3]));
  double TM4  = (::atof(argv[4]));
  double TM5  = (::atof(argv[5]));
  double TM61 = (::atof(argv[6]));
  double TM62 = (::atof(argv[7]));
  double TM7  = (::atof(argv[8]));

  double goodResult = (TM5 - TM4) / (TM5 + TM4 + ndbsiFunct.GetEpsilonToBeConsideredAsZero());

  std::cout << goodResult;

  InputPixelType pixel;
  pixel[0] = TM1;
  pixel[1] = TM2;
  pixel[2] = TM3;
  pixel[3] = TM4;
  pixel[4] = TM5;
  pixel[5] = TM61;
  pixel[6] = TM62;
  pixel[7] = TM7;

  double result = ndbsiFunct(pixel);

  std::cout << " " << result << std::endl;

  if (result != goodResult)
    return EXIT_FAILURE;

  return EXIT_SUCCESS;
}
