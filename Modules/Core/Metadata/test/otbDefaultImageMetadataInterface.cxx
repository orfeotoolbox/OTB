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
#include "otbDefaultImageMetadataInterface.h"

int otbDefaultImageMetadataInterface(int itkNotUsed(argc), char* itkNotUsed(argv) [])
{
  typedef otb::DefaultImageMetadataInterface ObjectType;

  ObjectType::Pointer obj = ObjectType::New();

  bool hasRisedException;

  hasRisedException = false;

  try
    {
      std::cout << obj->GetPhysicalBias() << std::endl;
    }
  catch (itk::ExceptionObject& err)
    {
    std::cout << "ExceptionObject caught for GetPhysicalBias() !" << std::endl;
    std::cout << err << std::endl;
    hasRisedException =true;
    }

  if(!hasRisedException)
    {
    std::cerr << " Should has raise an exception !"<< std::endl;
    return EXIT_FAILURE;
    }

  hasRisedException = false;


  try
    {
      std::cout << obj->GetPhysicalGain() << std::endl;
    }
  catch (itk::ExceptionObject& err)
    {
    std::cout << "ExceptionObject caught for GetPhysicalGain() !" << std::endl;
    std::cout << err << std::endl;
    hasRisedException =true;
    }

  if(!hasRisedException)
    {
    std::cerr << " Should has raise an exception !"<< std::endl;
    return EXIT_FAILURE;
    }

  hasRisedException = false;

  try
    {
      std::cout << obj->GetSolarIrradiance() << std::endl;
    }
  catch (itk::ExceptionObject& err)
    {
    std::cout << "ExceptionObject caught for GetSolarIrradiance() !" << std::endl;
    std::cout << err << std::endl;
    hasRisedException =true;
    }

  if(!hasRisedException)
    {
    std::cerr << " Should has raise an exception !"<< std::endl;
    return EXIT_FAILURE;
    }

  hasRisedException = false;

  try
    {
      std::cout << obj->GetDay() << std::endl;
    }
  catch (itk::ExceptionObject& err)
    {
    std::cout << "ExceptionObject caught for GetDay() !" << std::endl;
    std::cout << err << std::endl;
    hasRisedException =true;
    }

  if(!hasRisedException)
    {
    std::cerr << " Should has raise an exception !"<< std::endl;
    return EXIT_FAILURE;
    }

  hasRisedException = false;

  try
    {
      std::cout << obj->GetMonth() << std::endl;
    }
  catch (itk::ExceptionObject& err)
    {
    std::cout << "ExceptionObject caught for GetMonth() !" << std::endl;
    std::cout << err << std::endl;
    hasRisedException =true;
    }

  if(!hasRisedException)
    {
    std::cerr << " Should has raise an exception !"<< std::endl;
    return EXIT_FAILURE;
    }

  hasRisedException = false;

  try
    {
      std::cout << obj->GetYear() << std::endl;
    }
  catch (itk::ExceptionObject& err)
    {
    std::cout << "ExceptionObject caught for GetYear() !" << std::endl;
    std::cout << err << std::endl;
    hasRisedException =true;
    }

  if(!hasRisedException)
    {
    std::cerr << " Should has raise an exception !"<< std::endl;
    return EXIT_FAILURE;
    }

  hasRisedException = false;

  try
    {
      std::cout << obj->GetHour() << std::endl;
    }
  catch (itk::ExceptionObject& err)
    {
    std::cout << "ExceptionObject caught for GetHour() !" << std::endl;
    std::cout << err << std::endl;
    hasRisedException =true;
    }

  if(!hasRisedException)
    {
    std::cerr << " Should has raise an exception !"<< std::endl;
    return EXIT_FAILURE;
    }

  hasRisedException = false;

  try
    {
      std::cout << obj->GetMinute() << std::endl;
    }
  catch (itk::ExceptionObject& err)
    {
    std::cout << "ExceptionObject caught for GetMinute() !" << std::endl;
    std::cout << err << std::endl;
    hasRisedException =true;
    }

  if(!hasRisedException)
    {
    std::cerr << " Should has raise an exception !"<< std::endl;
    return EXIT_FAILURE;
    }

  hasRisedException = false;

  try
    {
      std::cout << obj->GetProductionDay() << std::endl;
    }
  catch (itk::ExceptionObject& err)
    {
    std::cout << "ExceptionObject caught for GetProductionDay() !" << std::endl;
    std::cout << err << std::endl;
    hasRisedException =true;
    }

  if(!hasRisedException)
    {
    std::cerr << " Should has raise an exception !"<< std::endl;
    return EXIT_FAILURE;
    }

  hasRisedException = false;

  try
    {
      std::cout << obj->GetProductionMonth() << std::endl;
    }
  catch (itk::ExceptionObject& err)
    {
    std::cout << "ExceptionObject caught for GetProductionMonth() !" << std::endl;
    std::cout << err << std::endl;
    hasRisedException =true;
    }

  if(!hasRisedException)
    {
    std::cerr << " Should has raise an exception !"<< std::endl;
    return EXIT_FAILURE;
    }

  hasRisedException = false;

  try
    {
      std::cout << obj->GetProductionYear() << std::endl;
    }
  catch (itk::ExceptionObject& err)
    {
    std::cout << "ExceptionObject caught for GetProductionYear() !" << std::endl;
    std::cout << err << std::endl;
    hasRisedException =true;
    }

  if(!hasRisedException)
    {
    std::cerr << " Should has raise an exception !"<< std::endl;
    return EXIT_FAILURE;
    }

  hasRisedException = false;

  try
    {
      std::cout << obj->GetSatElevation() << std::endl;
    }
  catch (itk::ExceptionObject& err)
    {
    std::cout << "ExceptionObject caught for GetSatElevation() !" << std::endl;
    std::cout << err << std::endl;
    hasRisedException =true;
    }

  if(!hasRisedException)
    {
    std::cerr << " Should has raise an exception !"<< std::endl;
    return EXIT_FAILURE;
    }

  hasRisedException = false;

  try
    {
      std::cout << obj->GetSatAzimuth() << std::endl;
    }
  catch (itk::ExceptionObject& err)
    {
    std::cout << "ExceptionObject caught for GetSatAzimuth() !" << std::endl;
    std::cout << err << std::endl;
    hasRisedException =true;
    }

  if(!hasRisedException)
    {
    std::cerr << " Should has raise an exception !"<< std::endl;
    return EXIT_FAILURE;
    }

  hasRisedException = false;

  try
    {
      std::cout << obj->GetFirstWavelengths() << std::endl;
    }
  catch (itk::ExceptionObject& err)
    {
    std::cout << "ExceptionObject caught for GetFirstWavelengths() !" << std::endl;
    std::cout << err << std::endl;
    hasRisedException =true;
    }

  if(!hasRisedException)
    {
    std::cerr << " Should has raise an exception !"<< std::endl;
    return EXIT_FAILURE;
    }

  hasRisedException = false;

  try
    {
      std::cout << obj->GetLastWavelengths() << std::endl;
    }
  catch (itk::ExceptionObject& err)
    {
    std::cout << "ExceptionObject caught for GetLastWavelengths() !" << std::endl;
    std::cout << err << std::endl;
    hasRisedException =true;
    }

  if(!hasRisedException)
    {
    std::cerr << " Should has raise an exception !"<< std::endl;
    return EXIT_FAILURE;
    }

  hasRisedException = false;

  try
    {
      if( obj->CanRead() != true)
        {
        std::cout << "wrong value for CanRead() method!" << std::endl;
        return EXIT_FAILURE;
        }
    }
  catch (itk::ExceptionObject& err)
    {
    std::cout << "ExceptionObject caught for CanRead() !" << std::endl;
    std::cout << err << std::endl;
    return EXIT_FAILURE;
    }

  hasRisedException = false;

  try
    {
    std::vector<unsigned int> rgb(3);
    rgb = obj->GetDefaultDisplay();

    if (obj->GetNumberOfBands() != 0 && rgb[0] != 0 && rgb[1] != 1 && rgb[2] != 2)
      {
      std::cout << "wrong value for GetDefaultDisplay() method!" << std::endl;
      return EXIT_FAILURE;
      }
    }
  catch (itk::ExceptionObject& err)
    {
    std::cout << "ExceptionObject caught for GetDefaultDisplay() !" << std::endl;
    std::cout << err << std::endl;
    }

  return EXIT_SUCCESS;

}
