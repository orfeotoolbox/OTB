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
#include "otbSarDefaultImageMetadataInterface.h"

int otbSarDefaultImageMetadataInterface(int itkNotUsed(argc), char** itkNotUsed(argv))
{
  typedef otb::SarDefaultImageMetadataInterface ObjectType;

  ObjectType::Pointer obj = ObjectType::New();

  try
    {
      std::cout << obj->GetRadiometricCalibrationScale() << std::endl;
    }
  catch (itk::ExceptionObject& err)
    {
    std::cout << "ExceptionObject caught for GetRadiometricCalibrationScale() !" << std::endl;
    std::cout << err << std::endl;
    }

  try
    {
      std::cout << obj->GetRadiometricCalibrationAntennaPatternNewGain() << std::endl;
    }
  catch (itk::ExceptionObject& err)
    {
    std::cout << "ExceptionObject caught for GetRadiometricCalibrationAntennaPatternNewGain() !" << std::endl;
    std::cout << err << std::endl;
    }

  try
    {
      std::cout << obj->GetRadiometricCalibrationAntennaPatternOldGain() << std::endl;
    }
  catch (itk::ExceptionObject& err)
    {
    std::cout << "ExceptionObject caught for GetRadiometricCalibrationAntennaPatternOldGain() !" << std::endl;
    std::cout << err << std::endl;
    }

  try
    {
      std::cout << obj->GetRadiometricCalibrationIncidenceAngle() << std::endl;
    }
  catch (itk::ExceptionObject& err)
    {
    std::cout << "ExceptionObject caught for GetRadiometricCalibrationIncidenceAngle() !" << std::endl;
    std::cout << err << std::endl;
    }

  try
    {
      std::cout << obj->GetRadiometricCalibrationRangeSpreadLoss() << std::endl;
    }
  catch (itk::ExceptionObject& err)
    {
    std::cout << "ExceptionObject caught for GetRadiometricCalibrationRangeSpreadLoss() !" << std::endl;
    std::cout << err << std::endl;
    }

  try
    {
      std::cout << obj->GetRadiometricCalibrationNoise() << std::endl;
    }
  catch (itk::ExceptionObject& err)
    {
    std::cout << "ExceptionObject caught for GetRadiometricCalibrationNoise() !" << std::endl;
    std::cout << err << std::endl;
    }

  try
    {
      std::cout << obj->GetRadiometricCalibrationAntennaPatternNewGainPolynomialDegree() << std::endl;
    }
  catch (itk::ExceptionObject& err)
    {
    std::cout << "ExceptionObject caught for GetRadiometricCalibrationAntennaPatternNewGainPolynomialDegree() !" << std::endl;
    std::cout << err << std::endl;
    }

  try
    {
      std::cout << obj->GetRadiometricCalibrationAntennaPatternOldGainPolynomialDegree() << std::endl;
    }
  catch (itk::ExceptionObject& err)
    {
    std::cout << "ExceptionObject caught for GetRadiometricCalibrationAntennaPatternOldGainPolynomialDegree() !" << std::endl;
    std::cout << err << std::endl;
    }

  try
    {
      std::cout << obj->GetRadiometricCalibrationIncidenceAnglePolynomialDegree() << std::endl;
    }
  catch (itk::ExceptionObject& err)
    {
    std::cout << "ExceptionObject caught for GetRadiometricCalibrationIncidenceAnglePolynomialDegree() !" << std::endl;
    std::cout << err << std::endl;
    }

  try
    {
      std::cout << obj->GetRadiometricCalibrationRangeSpreadLossPolynomialDegree() << std::endl;
    }
  catch (itk::ExceptionObject& err)
    {
    std::cout << "ExceptionObject caught for GetRadiometricCalibrationRangeSpreadLossPolynomialDegree() !" << std::endl;
    std::cout << err << std::endl;
    }

  try
    {
      std::cout << obj->GetRadiometricCalibrationNoisePolynomialDegree() << std::endl;
    }
  catch (itk::ExceptionObject& err)
    {
    std::cout << "ExceptionObject caught for GetRadiometricCalibrationNoisePolynomialDegree() !" << std::endl;
    std::cout << err << std::endl;
    }

  try
    {
      std::cout << obj->GetDay() << std::endl;
    }
  catch (itk::ExceptionObject& err)
    {
    std::cout << "ExceptionObject caught for GetDay() !" << std::endl;
    std::cout << err << std::endl;
    }

  try
    {
      std::cout << obj->GetMonth() << std::endl;
    }
  catch (itk::ExceptionObject& err)
    {
    std::cout << "ExceptionObject caught for GetMonth() !" << std::endl;
    std::cout << err << std::endl;
    }

  try
    {
      std::cout << obj->GetYear() << std::endl;
    }
  catch (itk::ExceptionObject& err)
    {
    std::cout << "ExceptionObject caught for GetYear() !" << std::endl;
    std::cout << err << std::endl;
    }

  try
    {
      std::cout << obj->GetHour() << std::endl;
    }
  catch (itk::ExceptionObject& err)
    {
    std::cout << "ExceptionObject caught for GetHour() !" << std::endl;
    std::cout << err << std::endl;
    }

  try
    {
      std::cout << obj->GetMinute() << std::endl;
    }
  catch (itk::ExceptionObject& err)
    {
    std::cout << "ExceptionObject caught for GetMinute() !" << std::endl;
    std::cout << err << std::endl;
    }

  try
    {
      std::cout << obj->GetProductionDay() << std::endl;
    }
  catch (itk::ExceptionObject& err)
    {
    std::cout << "ExceptionObject caught for GetProductionDay() !" << std::endl;
    std::cout << err << std::endl;
    }

  try
    {
      std::cout << obj->GetProductionMonth() << std::endl;
    }
  catch (itk::ExceptionObject& err)
    {
    std::cout << "ExceptionObject caught for GetProductionMonth() !" << std::endl;
    std::cout << err << std::endl;
    }

  try
    {
      std::cout << obj->GetProductionYear() << std::endl;
    }
  catch (itk::ExceptionObject& err)
    {
    std::cout << "ExceptionObject caught for GetProductionYear() !" << std::endl;
    std::cout << err << std::endl;
    }

  try
    {
      if(obj->CanRead() != false)
        {
        itkGenericExceptionMacro( << "CanRead() must be false ");
        }

      std::vector<unsigned int> displayValue = obj->GetDefaultDisplay();
      if(displayValue[0] != 0 || displayValue[1] != 0 || displayValue[2] != 0)
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


  return EXIT_SUCCESS;

}
