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

#include "otbAeronetData.h"
#include "otbAeronetFileReader.h"
#include "otbSystem.h"

int otbAeronetExtractDataBadData(int itkNotUsed(argc), char* argv[])
{
  otb::AeronetFileReader::Pointer reader = otb::AeronetFileReader::New();

  reader->SetFileName(argv[1]);
  reader->SetDay(atoi(argv[2]));
  reader->SetMonth(atoi(argv[3]));
  reader->SetYear(atoi(argv[4]));
  reader->SetHour(atoi(argv[5]));
  reader->SetMinute(atoi(argv[6]));
  reader->SetEpsilon(atof(argv[7]));

  try
  {
    reader->Update();
  }
  catch (itk::ExceptionObject&)
  {
    return EXIT_SUCCESS;
  }
  return EXIT_FAILURE;
}
