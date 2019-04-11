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

using namespace otb::Functor;

int otbLAIFromNDVILogarithmic(int itkNotUsed(argc), char * argv[])
{
  typedef double                           PixelType;

  typedef otb::Functor::LAIFromNDVILogarithmic<PixelType, PixelType> FunctorType;

  FunctorType laiFunct = FunctorType();

  double redValue = (::atof(argv[1]));
  double nirValue = (::atof(argv[2]));
  double ndviSoil(::atof(argv[3]));
  double ndviInf(::atof(argv[4]));
  double extCoef(::atof(argv[5]));

  double ndvi = (nirValue-redValue)/(nirValue+redValue);
  double goodResult = -1/extCoef*std::log((ndvi-ndviInf)/(ndviSoil-ndviInf));

  laiFunct.SetNdviInf(ndviInf);
  laiFunct.SetNdviSoil(ndviSoil);
  laiFunct.SetExtinctionCoefficient(extCoef);

  laiFunct.SetBandIndex(CommonBandNames::RED,1);
  laiFunct.SetBandIndex(CommonBandNames::NIR,2);

  itk::VariableLengthVector<PixelType> pixel;
  pixel.Reserve(2);
  pixel[0] = redValue;
  pixel[1] = nirValue;

  double result = laiFunct(pixel);

  if( result!=goodResult ) return EXIT_FAILURE;

  return EXIT_SUCCESS;
}
