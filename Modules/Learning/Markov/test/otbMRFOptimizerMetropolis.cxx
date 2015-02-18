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


#include "otbMRFOptimizerMetropolis.h"
#include <fstream>
#include <cstdlib>

int otbMRFOptimizerMetropolis(int itkNotUsed(argc), char * argv[])
{
  const char * outputFile = argv[1];

  typedef otb::MRFOptimizerMetropolis MRFOptimizerType;

  MRFOptimizerType::Pointer object = MRFOptimizerType::New();

  MRFOptimizerType::ParametersType param(1);
  param.Fill(10.);
  object->SetParameters(param);
  object->InitializeSeed(0); // USED TO OVERPASS RANDOM CALCULATION

  std::ofstream file;
  file.open(outputFile);

  file << "Compute(0)                                       : " << object->Compute(0.) << std::endl;
  file << "Compute(-1.)                                     : " << object->Compute(-1.) << std::endl;

  file.close();

  return EXIT_SUCCESS;
}
