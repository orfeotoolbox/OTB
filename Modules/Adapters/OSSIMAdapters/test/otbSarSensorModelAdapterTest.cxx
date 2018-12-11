/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#include "otbSarSensorModelAdapter.h"
#include "otbImageKeywordlist.h"


int otbSarSensorModelAdapterTest(int itkNotUsed(argc), char* argv[])
{
  std::string infname = argv[1];

  otb::SarSensorModelAdapter::Pointer sensorModel = otb::SarSensorModelAdapter::New();

  auto kwl = otb::ReadGeometryFromGEOMFile(infname);

  bool success = sensorModel->LoadState(kwl);

  if(!success)
    {
    std::cerr<<"Could not LoadState() from keyword list read from"<<infname<<std::endl;
    return EXIT_FAILURE;
    }

  std::vector<std::pair<unsigned long, unsigned long> > lines;
  std::pair<unsigned long, unsigned long> samples;
  success = sensorModel->Deburst(lines, samples);

  if(!success)
    {
    std::cerr<<"Deburst() call failed."<<std::endl;
    return EXIT_FAILURE;
    }
  

  otb::ImageKeywordlist outKwl;
  success = sensorModel->SaveState(outKwl);
  if(!success)
    {
    std::cerr<<"SaveState() call failed."<<std::endl;
    return EXIT_FAILURE;
    }
  

  otb::SarSensorModelAdapter::Point2DType out1,out2;
  otb::SarSensorModelAdapter::Point3DType in, out3, out4, out5, out6, out7;

  // GCP 99 from input geom file
  //support_data.geom.gcp[99].world_pt.hgt:  2.238244926818182e+02
  //support_data.geom.gcp[99].world_pt.lat:  4.323458093295080e+01
  //support_data.geom.gcp[99].world_pt.lon:  1.116316013091967e+00
    
  in[0] = 4.323458093295080e+01;
  in[1] = 1.116316013091967e+00;
  in[2] = 2.238244926818182e+02;

  sensorModel->WorldToLineSample(in,out1);
  sensorModel->WorldToLineSampleYZ(in,out1,out2);

  sensorModel->WorldToCartesian(in, out5);
  sensorModel->WorldToSatPositionAndVelocity(in,out3, out4);

  unsigned int ind_Line = 2;
  sensorModel->LineToSatPositionAndVelocity(ind_Line, out6, out7);
  
  return EXIT_SUCCESS;
}
