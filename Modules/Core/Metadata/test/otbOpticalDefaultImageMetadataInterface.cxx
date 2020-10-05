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


#include "itkMacro.h"
#include "otbOpticalDefaultImageMetadataInterface.h"

int otbOpticalDefaultImageMetadataInterface(int itkNotUsed(argc), char* itkNotUsed(argv)[])
{
  typedef otb::OpticalDefaultImageMetadataInterface ObjectType;

  ObjectType::Pointer obj = ObjectType::New();

  try
  {
    std::cout << obj->GetSunElevation() << std::endl;
  }
  catch (itk::ExceptionObject& err)
  {
    std::cout << "ExceptionObject caught for GetSunElevation() !" << std::endl;
    std::cout << err << std::endl;
  }

  try
  {
    std::cout << obj->GetSunAzimuth() << std::endl;
  }
  catch (itk::ExceptionObject& err)
  {
    std::cout << "ExceptionObject caught for GetSunAzimuth() !" << std::endl;
    std::cout << err << std::endl;
  }

  try
  {
    std::cout << obj->GetSatElevation() << std::endl;
  }
  catch (itk::ExceptionObject& err)
  {
    std::cout << "ExceptionObject caught for GetSatElevation() !" << std::endl;
    std::cout << err << std::endl;
  }

  try
  {
    std::cout << obj->GetSatAzimuth() << std::endl;
  }
  catch (itk::ExceptionObject& err)
  {
    std::cout << "ExceptionObject caught for GetSatAzimuth() !" << std::endl;
    std::cout << err << std::endl;
  }

  try
  {
    std::cout << obj->GetPhysicalBias() << std::endl;
  }
  catch (itk::ExceptionObject& err)
  {
    std::cout << "ExceptionObject caught for GetPhysicalBias() !" << std::endl;
    std::cout << err << std::endl;
  }

  try
  {
    std::cout << obj->GetPhysicalGain() << std::endl;
  }
  catch (itk::ExceptionObject& err)
  {
    std::cout << "ExceptionObject caught for GetPhysicalGain() !" << std::endl;
    std::cout << err << std::endl;
  }

  try
  {
    std::cout << obj->GetSolarIrradiance() << std::endl;
  }
  catch (itk::ExceptionObject& err)
  {
    std::cout << "ExceptionObject caught for GetSolarIrradiance() !" << std::endl;
    std::cout << err << std::endl;
  }

  try
  {
    std::cout << obj->GetFirstWavelengths() << std::endl;
  }
  catch (itk::ExceptionObject& err)
  {
    std::cout << "ExceptionObject caught for GetFirstWavelengths() !" << std::endl;
    std::cout << err << std::endl;
  }

  try
  {
    std::cout << obj->GetLastWavelengths() << std::endl;
  }
  catch (itk::ExceptionObject& err)
  {
    std::cout << "ExceptionObject caught for GetLastWavelengths() !" << std::endl;
    std::cout << err << std::endl;
  }

  try
  {
    if (obj->CanRead() != false)
    {
      itkGenericExceptionMacro(<< "CanRead() must be false ");
    }

    std::vector<unsigned int> displayValue = obj->GetDefaultDisplay();
    if (displayValue[0] != 0 || displayValue[1] != 1 || displayValue[2] != 2)
    {
      itkGenericExceptionMacro(<< "GetDefaultDisplay() wrong value ");
    }
  }
  catch (itk::ExceptionObject& err)
  {
    std::cout << "ExceptionObject caught !" << std::endl;
    std::cout << err << std::endl;
    return EXIT_FAILURE;
  }

  try
  {
    std::cout << obj->GetSpectralSensitivity() << std::endl;
  }
  catch (itk::ExceptionObject& err)
  {
    std::cout << "ExceptionObject caught for GetSpectralSensitivity() !" << std::endl;
    std::cout << err << std::endl;
  }

  return EXIT_SUCCESS;
}
