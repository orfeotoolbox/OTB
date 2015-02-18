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

#include "otbShiftScaleVectorImageFilter.h"
#include "otbVectorImage.h"

int otbShiftScaleVectorImageFilterNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  const unsigned int Dimension = 2;
  typedef double                                                                  InputPixelType;
  typedef int                                                                     OutputPixelType;
  typedef otb::VectorImage<InputPixelType, Dimension>                             InputImageType;
  typedef otb::VectorImage<OutputPixelType, Dimension>                            OutputImageType;
  typedef otb::ShiftScaleVectorImageFilter<InputImageType, OutputImageType>       ShiftScaleVImageFilterType;

  // Instantiating object
  ShiftScaleVImageFilterType::Pointer filter = ShiftScaleVImageFilterType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
