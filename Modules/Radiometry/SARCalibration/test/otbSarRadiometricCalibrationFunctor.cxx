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

#include "otbSarRadiometricCalibrationFunctor.h"

int otbSarRadiometricCalibrationFunctor(int itkNotUsed(argc), char * itkNotUsed(argv)[])
{
  typedef double                   ScalarType;

  typedef otb::Functor::SarRadiometricCalibrationFunctor<ScalarType, ScalarType> FunctorType;

  FunctorType funct;

  funct.SetNoise( 10.0);
  if( abs(funct.GetNoise() -10.0) > 0.0)
  {
    return false;
  }
  funct.SetScale( 10.0);
  if( abs(funct.GetScale() -10.0) > 0.0)
  {
    return false;
  }
  funct.SetAntennaPatternNewGain( 10.0);
  if( abs(funct.GetAntennaPatternNewGain() -10.0) > 0.0)
  {
    return false;
  }
  funct.SetAntennaPatternOldGain( 10.0);
  if( abs(funct.GetAntennaPatternOldGain() -10.0) > 0.0)
  {
    return false;
  }
  funct.SetIncidenceAngle( 10.0);
  if( abs(funct.GetIncidenceAngle() -10.0) > 0.0)
  {
    return false;
  }
  funct.SetRangeSpreadLoss( 10.0);
  if( abs(funct.GetRangeSpreadLoss() -10.0) > 0.0)
  {
    return false;
  }
  std::cout << "First  sigma : " << funct.operator ()( 0.) << std::endl;
  std::cout << "Second sigma : " << funct.operator ()( 1.) << std::endl;


  return EXIT_SUCCESS;
}
