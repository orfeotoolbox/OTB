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
#include "otbImage.h"
#include <iostream>

#include "otbImageToModulusAndDirectionImageFilter.h"
#include "otbModulusAndDirectionImageToImageFilter.h"

int otbModulusAndDirectionImageFiltersNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  typedef unsigned char InputPixelType;
  typedef double        OutputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<InputPixelType,  Dimension> InputImageType;
  typedef otb::Image<OutputPixelType, Dimension> InputImageDirectionType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageDirectionType;

  typedef otb::ImageToModulusAndDirectionImageFilter<InputImageType,
      OutputImageType>          FilterType;
  typedef otb::ImageToModulusAndDirectionImageFilter<InputImageType, OutputImageType,
      OutputImageDirectionType> Filter2Type;

  FilterType::Pointer  filter = FilterType::New();
  Filter2Type::Pointer filter2 = Filter2Type::New();

  typedef otb::ModulusAndDirectionImageToImageFilter<InputImageType, InputImageDirectionType,
      OutputImageType> Filter3Type;
  Filter3Type::Pointer filter3 = Filter3Type::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
