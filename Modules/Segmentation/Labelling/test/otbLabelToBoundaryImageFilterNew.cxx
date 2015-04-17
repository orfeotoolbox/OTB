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

#include "otbLabelToBoundaryImageFilter.h"
#include "otbImage.h"

int otbLabelToBoundaryImageFilterNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  typedef unsigned int InputPixelType;
  typedef unsigned char OutputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<InputPixelType, Dimension>  InputImageType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;

  typedef otb::LabelToBoundaryImageFilter
  <InputImageType, OutputImageType> BoundaryFilterType;
  typedef BoundaryFilterType::Pointer BoundaryFilterPointerType;

  BoundaryFilterPointerType filter = BoundaryFilterType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
