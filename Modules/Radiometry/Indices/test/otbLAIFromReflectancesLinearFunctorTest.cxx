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

#include "itkMacro.h"
#include "otbVegetationIndicesFunctor.h"

int otbLAIFromReflectancesLinear(int itkNotUsed(argc), char * argv[])
{
  typedef double                           PixelType;

  typedef otb::Functor::LAIFromReflectancesLinear<PixelType, PixelType, PixelType> FunctorType;

  FunctorType laiFunct = FunctorType();

  double redValue = (::atof(argv[1]));
  double nirValue = (::atof(argv[2]));
  double redCoef(::atof(argv[3]));
  double nirCoef(::atof(argv[4]));

  double goodResult = redCoef*redValue+nirCoef*nirValue;

  laiFunct.SetRedCoef(redCoef);
  laiFunct.SetNirCoef(nirCoef);

  laiFunct.SetRedIndex(1);
  laiFunct.SetNIRIndex(2);

  itk::VariableLengthVector<PixelType> pixel;
  pixel.Reserve(2);
  pixel[0] = redValue;
  pixel[1] = nirValue;

  double result = laiFunct(pixel);

  if( result!=goodResult ) return EXIT_FAILURE;

  return EXIT_SUCCESS;
}
