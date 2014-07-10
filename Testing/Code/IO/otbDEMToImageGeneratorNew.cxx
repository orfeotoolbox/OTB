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

#include "otbDEMToImageGenerator.h"
#include "otbImage.h"

int otbDEMToImageGeneratorNew(int itkNotUsed(argc), char ** itkNotUsed(argv))
{
  const unsigned int Dimension = 2;
  typedef otb::Image<unsigned char, Dimension> ImageType;
  typedef otb::DEMToImageGenerator<ImageType>  DEMToImageGeneratorType;

  // Instantiating object
  DEMToImageGeneratorType::Pointer object = DEMToImageGeneratorType::New();

  std::cout << object << std::endl;

  return EXIT_SUCCESS;
}
