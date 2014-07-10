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

#include "otbVectorImage.h"
#include "otbPhaseFunctor.h"
#include "otbUnaryFunctorImageFilter.h"

int otbUnaryFunctorImageFilterNew(int itkNotUsed(argc), char ** itkNotUsed(argv))
{
  const unsigned int Dimension = 2;
  typedef double                                         PixelType;
  typedef otb::VectorImage<PixelType, Dimension>         InputImageType;
  typedef InputImageType::InternalPixelType              InternalPixelType;
  typedef otb::Function::PhaseFunctor<InternalPixelType> FunctorType;

  typedef otb::UnaryFunctorImageFilter<InputImageType,
      InputImageType,
      FunctorType> FilterType;
  FilterType::Pointer filter = FilterType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
