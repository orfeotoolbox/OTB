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



#include "itkMacro.h"
#include "otbOpticalDefaultImageMetadataInterface.h"

int otbOpticalDefaultImageMetadataInterface(int itkNotUsed(argc), char* itkNotUsed(argv) [])
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
      if(obj->CanRead() != false)
        {
        itkGenericExceptionMacro( << "CanRead() must be false ");
        }

      std::vector<unsigned int> displayValue = obj->GetDefaultDisplay();
      if(displayValue[0] != 0 || displayValue[1] != 1 || displayValue[2] != 2)
        {
        itkGenericExceptionMacro( << "GetDefaultDisplay() wrong value ");
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
