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
#include "otbSVMImageClassificationWithRuleFilter.h"
#include "otbImage.h"

int otbSVMImageClassificationWithRuleFilterNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  const unsigned int Dimension = 2;
  typedef double         PixelType;
  typedef unsigned short LabeledPixelType;

  typedef otb::VectorImage<PixelType, Dimension>  ImageType;
  typedef otb::Image<LabeledPixelType, Dimension> LabeledImageType;

  typedef otb::SVMImageClassificationWithRuleFilter<ImageType, LabeledImageType> ClassificationWithRuleFilterType;

  // Instantiating object
  ClassificationWithRuleFilterType::Pointer filter = ClassificationWithRuleFilterType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
