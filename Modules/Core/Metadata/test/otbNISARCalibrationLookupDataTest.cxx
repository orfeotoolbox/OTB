/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

#include "otbNISARCalibrationLookupData.h"
#include "otbMacro.h"

#include <cmath>

namespace
{
bool CloseEnough(double lhs, double rhs)
{
  return std::abs(lhs - rhs) < 1e-6;
}
}

int otbNISARCalibrationLookupDataTest(int itkNotUsed(argc), char * itkNotUsed(argv)[])
{
  auto lut = otb::NISARCalibrationLookupData::New();
  lut->Initialize(otb::SarCalibrationLookupData::SIGMA,
                  0.0,
                  5.0,
                  100.0,
                  10.0,
                  {0.0, 10.0},
                  {100.0, 200.0},
                  {1.0F, 2.0F,
                   3.0F, 4.0F});

  if (!CloseEnough(lut->GetValue(0, 0), 1.0))
  {
    otbGenericExceptionMacro(itk::ExceptionObject, << "Unexpected LUT value at origin.");
  }

  if (!CloseEnough(lut->GetValue(5, 1), 2.5))
  {
    otbGenericExceptionMacro(itk::ExceptionObject, << "Unexpected bilinear interpolation result.");
  }

  otb::MetaData::Keywordlist kwl;
  lut->ToKeywordlist(kwl, "LUT.");

  auto roundTrip = otb::NISARCalibrationLookupData::New();
  roundTrip->FromKeywordlist(kwl, "LUT.");

  if (!CloseEnough(roundTrip->GetValue(5, 1), 2.5))
  {
    otbGenericExceptionMacro(itk::ExceptionObject, << "Unexpected round-trip interpolation result.");
  }

  return EXIT_SUCCESS;
}
