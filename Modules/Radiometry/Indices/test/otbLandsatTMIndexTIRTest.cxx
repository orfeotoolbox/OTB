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

#include "itkFixedArray.h"
#include "otbLandsatTMIndices.h"

int otbLandsatTMIndexTIR(int itkNotUsed(argc), char* argv[])
{
  typedef double OutputPixelType;
  typedef itk::FixedArray<double, 8> L7InputPixelType;
  typedef itk::FixedArray<double, 7> L5InputPixelType;

  typedef otb::Functor::LandsatTM::TIR<L7InputPixelType, OutputPixelType> L7FunctorType;

  L7FunctorType tirFunctL7 = L7FunctorType();

  double TM1  = (::atof(argv[1]));
  double TM2  = (::atof(argv[2]));
  double TM3  = (::atof(argv[3]));
  double TM4  = (::atof(argv[4]));
  double TM5  = (::atof(argv[5]));
  double TM60 = (::atof(argv[6]));
  double TM61 = (::atof(argv[6]));
  double TM62 = (::atof(argv[7]));
  double TM7  = (::atof(argv[8]));

  tirFunctL7.SetSAT(otb::Functor::LandsatTM::L7);
  double goodResult = TM62;

  std::cout << goodResult;

  L7InputPixelType pixelL7;
  pixelL7[0] = TM1;
  pixelL7[1] = TM2;
  pixelL7[2] = TM3;
  pixelL7[3] = TM4;
  pixelL7[4] = TM5;
  pixelL7[5] = TM61;
  pixelL7[6] = TM62;
  pixelL7[7] = TM7;

  double result = tirFunctL7(pixelL7);

  std::cout << " " << result << std::endl;

  if (result != goodResult)
    return EXIT_FAILURE;

  typedef otb::Functor::LandsatTM::TIR<L5InputPixelType, OutputPixelType> L5FunctorType;

  L5FunctorType tirFunctL5 = L5FunctorType();


  tirFunctL5.SetSAT(otb::Functor::LandsatTM::L5);
  goodResult = TM60;

  std::cout << goodResult;

  L5InputPixelType pixelL5;
  pixelL5[0] = TM1;
  pixelL5[1] = TM2;
  pixelL5[2] = TM3;
  pixelL5[3] = TM4;
  pixelL5[4] = TM5;
  pixelL5[5] = TM60;
  pixelL5[6] = TM7;

  result = tirFunctL5(pixelL5);

  std::cout << " " << result << std::endl;

  if (result != goodResult)
    return EXIT_FAILURE;
  return EXIT_SUCCESS;
}
