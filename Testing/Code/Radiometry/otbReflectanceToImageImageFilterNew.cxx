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

#include "otbReflectanceToImageImageFilter.h"
#include "otbVectorImage.h"

int otbReflectanceToImageImageFilterNew(int itkNotUsed(argc), char * itkNotUsed(argv)[])
{
  const unsigned int Dimension = 2;
  typedef double                                 PixelType;
  typedef otb::VectorImage<PixelType, Dimension> InputImageType;

  typedef otb::ReflectanceToImageImageFilter<InputImageType, InputImageType> ReflectanceToImageImageFilterType;

  // Instantiating object
  ReflectanceToImageImageFilterType::Pointer filter = ReflectanceToImageImageFilterType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
