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

#include "otbVectorImage.h"
#include "otbUrbanAreaDetectionImageFilter.h"

int otbUrbanAreaDetectionImageFilterNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{

  const unsigned int Dimension = 2;
  typedef double                                 PixelType;
  typedef otb::VectorImage<PixelType, Dimension> InputVectorImageType;
  typedef otb::Image<unsigned char, Dimension>   OutputImageType;

  typedef otb::UrbanAreaDetectionImageFilter<InputVectorImageType,
      OutputImageType> UrbanAreaDetectionFilterType;

  // Instantiating objects
  UrbanAreaDetectionFilterType::Pointer filter = UrbanAreaDetectionFilterType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
