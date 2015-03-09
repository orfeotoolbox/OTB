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


#include "otbMRFOptimizerICM.h"
#include <cstdlib>

int otbMRFOptimizerICM(int itkNotUsed(argc), char * itkNotUsed(argv)[])
{
  typedef otb::MRFOptimizerICM MRFOptimizerICMType;

  MRFOptimizerICMType::Pointer object = MRFOptimizerICMType::New();

  if (object->Compute(-1) != true || object->Compute(1) != false)
    {
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
