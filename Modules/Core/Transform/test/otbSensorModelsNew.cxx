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
#include "otbExtern.h"



#include <iostream>

#include "otbForwardSensorModel.h"
#include "otbInverseSensorModel.h"

int otbSensorModelsNew(int itkNotUsed(argc), char * itkNotUsed(argv)[])
{
  typedef otb::SensorModelBase<double> SensorModelBaseType;
  SensorModelBaseType::Pointer lSensorModelBase = SensorModelBaseType::New();

  std::cout << lSensorModelBase << std::endl;

  typedef otb::InverseSensorModel<double> InverseSensorModelType;
  InverseSensorModelType::Pointer lInverseSensorModel = InverseSensorModelType::New();

  std::cout << lInverseSensorModel << std::endl;

  typedef otb::ForwardSensorModel<double> ForwardSensorModelType;
  ForwardSensorModelType::Pointer lForwardSensorModel = ForwardSensorModelType::New();

  std::cout << lForwardSensorModel << std::endl;

  return EXIT_SUCCESS;
}
