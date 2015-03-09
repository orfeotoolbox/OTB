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

#include "otbSarBrightnessFunctor.h"

int otbSarBrightnessFunctorWithoutNoise(int itkNotUsed(argc), char * itkNotUsed(argv)[])
{
  typedef double                   ScalarType;

  typedef otb::Functor::SarBrightnessFunctor<ScalarType, ScalarType> FunctorType;

  FunctorType functWithNoise;
  FunctorType functWithoutNoise;

  // With Noise
  functWithNoise.SetNoise( 0.0);
  if( abs(functWithNoise.GetNoise() -0.0) > 0.0)
  {
    return false;
  }
  functWithNoise.SetScale( 10.0);
  if( abs(functWithNoise.GetScale() -10.0) > 0.0)
  {
    return false;
  }
  functWithNoise.SetAntennaPatternNewGain( 10.0);
  if( abs(functWithNoise.GetAntennaPatternNewGain() -10.0) > 0.0)
  {
    return false;
  }
  functWithNoise.SetAntennaPatternOldGain( 10.0);
  if( abs(functWithNoise.GetAntennaPatternOldGain() -10.0) > 0.0)
  {
    return false;
  }

  functWithNoise.SetRangeSpreadLoss( 10.0);
  if( abs(functWithNoise.GetRangeSpreadLoss() -10.0) > 0.0)
  {
    return false;
  }

  // Without Noise
  functWithoutNoise.SetScale(10.0);
  if (abs(functWithoutNoise.GetScale() - 10.0) > 0.0)
    {
    return false;
    }
  functWithoutNoise.SetAntennaPatternNewGain(10.0);
  if (abs(functWithoutNoise.GetAntennaPatternNewGain() - 10.0) > 0.0)
    {
    return false;
    }
  functWithoutNoise.SetAntennaPatternOldGain(10.0);
  if (abs(functWithoutNoise.GetAntennaPatternOldGain() - 10.0) > 0.0)
    {
    return false;
    }

  functWithoutNoise.SetRangeSpreadLoss(10.0);
  if (abs(functWithoutNoise.GetRangeSpreadLoss() - 10.0) > 0.0)
    {
    return false;
    }

  std::cout << "First BetaNaught (With Noise = 0): " << functWithNoise.operator ()( 0.) << " *** " \
            << "First BetaNaught (Without Noise): " << functWithoutNoise.operator ()( 0.) \
            << std::endl;
  std::cout << "Second BetaNaught (With Noise = 0): " << functWithNoise.operator ()( 1.) << " *** " \
            << "Second BetaNaught (Without Noise): " << functWithoutNoise.operator ()( 1.) \
              << std::endl;

  return EXIT_SUCCESS;
}
