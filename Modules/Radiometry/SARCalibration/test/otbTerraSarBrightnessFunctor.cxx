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
#include <fstream>
#include <iomanip>
#include "itkMacro.h"

#include "otbTerraSarBrightnessFunctor.h"

int otbTerraSarBrightnessFunctor(int itkNotUsed(argc), char * argv[])
{
  char * outFilename = argv[5];
  double calFact =  atof(argv[4]);

  typedef double                   ScalarType;
  typedef std::complex<ScalarType> ComplexType;

  typedef otb::Functor::TerraSarBrightnessFunctor<ScalarType, ScalarType> FunctorType;

  FunctorType funct;
  funct.SetCalibrationFactor(calFact);
  funct.SetResultsInDecibels(false);

  ScalarType  inPix = static_cast<ScalarType>(atof(argv[1]));
  ComplexType inCplxPix(static_cast<ScalarType>(atof(argv[2])), static_cast<ScalarType>(atof(argv[3])));

  std::ofstream file;
  file.open(outFilename);

  file << std::fixed << std::setprecision(10);

  file << "Scalar pixel : " << inPix << " -> " << funct.operator ()(inPix) << std::endl;

  file << "Complex pixel : " << inCplxPix << " -> " << funct.operator ()(inCplxPix) << std::endl;

  file.close();

  return EXIT_SUCCESS;
}
