/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


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
