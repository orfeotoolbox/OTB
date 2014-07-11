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

#include "otbUnaryImageFunctorWithVectorImageFilter.h"
#include "otbVectorImage.h"
#include "otbImage.h"
#include "itkCosImageFilter.h"

int otbUnaryImageFunctorWithVectorImageFilterNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  const unsigned int Dimension = 2;
  typedef double                                                  PixelType;
  typedef otb::VectorImage<PixelType, Dimension>                  InputImageType;
  typedef InputImageType::InternalPixelType                       InternalPixelType;
  typedef itk::Functor::Cos<InternalPixelType, InternalPixelType> FunctorType;
  typedef otb::UnaryImageFunctorWithVectorImageFilter<InputImageType,
      InputImageType,
      FunctorType>  UnaryImageFunctorWithVectorImageFilterType;

  // Instantiating object
  UnaryImageFunctorWithVectorImageFilterType::Pointer filter = UnaryImageFunctorWithVectorImageFilterType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
