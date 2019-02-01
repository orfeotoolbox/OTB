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

#include "otbAeronetData.h"
#include "otbAeronetFileReader.h"
#include "itksys/SystemTools.hxx"

#include <fstream>
#include <iomanip>

int otbAeronetExtractData(int itkNotUsed(argc), char * argv[])
{
  otb::AeronetFileReader::Pointer reader = otb::AeronetFileReader::New();

  reader->SetFileName(argv[1]);
  reader->SetDay(atoi(argv[2]));
  reader->SetMonth(atoi(argv[3]));
  reader->SetYear(atoi(argv[4]));
  reader->SetHour(atoi(argv[5]));
  reader->SetMinute(atoi(argv[6]));
  reader->SetEpsilon(atof(argv[7]));
  reader->Update();

  otb::AeronetData::Pointer data = reader->GetOutput();

  std::ofstream fout(argv[8]);

  fout << "Aeronet data extracted:" << std::endl;
  fout << "Input file name: " << itksys::SystemTools::GetFilenameName(std::string(argv[1])) << std::endl;
  fout << "Inputs user parameters:" << std::endl;
  fout << "    Day:                       " << reader->GetDay() << std::endl;
  fout << "    Month:                     " << reader->GetMonth() << std::endl;
  fout << "    Year:                      " << reader->GetYear() << std::endl;
  fout << "    Hour:                      " << reader->GetHour() << std::endl;
  fout << "    Minute:                    " << reader->GetMinute() << std::endl;
  fout << "Outputs results:" << std::endl;
  fout << std::setprecision(15) << std::endl;
  fout << "    AerosolOpticalThickness:   " << data->GetAerosolOpticalThickness() << std::endl;
  fout << "    AngstromCoefficient:       " << data->GetAngstromCoefficient() << std::endl;
  fout << "    Water:                     " << data->GetWater() << std::endl;
  fout << "    CloudEstimation:           " << data->GetCloudEstimation() << std::endl;
  fout << "    SolarZenithAngle:          " << data->GetSolarZenithAngle() << std::endl;
  fout << " Statisctics:" << std::endl;
  fout << "    Date:                      " << data->GetDataDate() << std::endl;
  fout << "    Epsilon date:              " << data->GetEpsilonDate() << std::endl;
  fout << "    Number Of date used:       " << data->GetNumberOfDateUsed() << std::endl;
  fout << "    StdDev Aero. Optical Thic.:" << data->GetStdDev() << std::endl;

  fout.close();
  return EXIT_SUCCESS;
}
