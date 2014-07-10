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

#include "otbGaborFilterGenerator.h"

int otbGaborFilterGeneratorNew(int itkNotUsed(argc), char ** itkNotUsed(argv))
{
  typedef double                                   PrecisionType;
  typedef otb::GaborFilterGenerator<PrecisionType> GaborGeneratorType;

  // Instantiating object
  GaborGeneratorType::Pointer gabor = GaborGeneratorType::New();

  std::cout << gabor << std::endl;

  return EXIT_SUCCESS;
}
