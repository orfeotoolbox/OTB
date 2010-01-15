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
#include "itkExceptionObject.h"

#include "otbTerraSarCalibrationFunctor.h"
#include "itkIndex.h"

int otbTerraSarCalibrationFunctor(int argc, char * argv[])
{
  typedef double                   ScalarType;
  typedef std::complex<ScalarType> ComplexType;
  typedef itk::Index<>             IndexType;

  typedef otb::Functor::TerraSarCalibrationFunctor<ScalarType, ScalarType>   FunctorType;

  FunctorType funct;
  
  IndexType id;
  id[0] = 125;   id[0] = 150;

  ScalarType inPix = 150.2;
  std::cout << inPix << " -> " << funct.operator()(inPix, id, 0.) << std::endl;

  ComplexType inCplxPix(12, 180);
  std::cout << inCplxPix << " -> " << funct.operator()(inCplxPix, id,0.) << std::endl;

  return EXIT_SUCCESS;
}
