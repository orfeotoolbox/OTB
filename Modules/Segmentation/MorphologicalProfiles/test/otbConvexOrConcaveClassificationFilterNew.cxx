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
#include "otbConvexOrConcaveClassificationFilter.h"
#include "otbImage.h"

int otbConvexOrConcaveClassificationFilterNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  typedef otb::Image<double, 2>        InputImageType;
  typedef otb::Image<unsigned char, 2> OutputImageType;

  typedef otb::ConvexOrConcaveClassificationFilter<InputImageType, OutputImageType> ClassificationFilterType;

  // Instantiating object
  ClassificationFilterType::Pointer filter = ClassificationFilterType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
