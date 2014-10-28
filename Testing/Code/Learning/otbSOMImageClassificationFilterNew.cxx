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
#include "otbSOMImageClassificationFilter.h"
#include "otbImage.h"
#include "otbSOMMap.h"

int otbSOMImageClassificationFilterNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  const unsigned int Dimension = 2;
  typedef double         PixelType;
  typedef unsigned short LabeledPixelType;

  typedef otb::VectorImage<PixelType, Dimension>                                     ImageType;
  typedef otb::Image<LabeledPixelType, Dimension>                                    LabeledImageType;
  typedef otb::SOMMap<ImageType::PixelType>                                          SOMMapType;
  typedef otb::SOMImageClassificationFilter<ImageType, LabeledImageType, SOMMapType> ClassificationFilterType;

  // Instantiating object
  ClassificationFilterType::Pointer filter = ClassificationFilterType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
