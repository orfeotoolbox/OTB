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

#include "otbMacro.h"
#include "otbImage.h"
#include "otbGCPsToRPCSensorModelImageFilter.h"

int otbGCPsToRPCSensorModelImageFilterNew(int itkNotUsed(argc), char * itkNotUsed(argv)[])
{
  typedef otb::Image<float, 2>                            ImageType;
  typedef otb::GCPsToRPCSensorModelImageFilter<ImageType> GCPToSensorModelFilterType;

  // Instantiation
  GCPToSensorModelFilterType::Pointer filter = GCPToSensorModelFilterType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
