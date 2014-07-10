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

#include "otbImageListSource.h"
#include "otbImage.h"

int otbImageListSourceNew(int itkNotUsed(argc), char ** itkNotUsed(argv))
{
  const unsigned int Dimension = 2;
  typedef unsigned char                         InputPixelType;
  typedef otb::Image<InputPixelType, Dimension> InputImageType;
  typedef otb::ImageListSource<InputImageType>  ImageListSourceType;

  // Instantiating ImageListSource object
  ImageListSourceType::Pointer imageList = ImageListSourceType::New();

  std::cout << imageList << std::endl;

  return EXIT_SUCCESS;
}
