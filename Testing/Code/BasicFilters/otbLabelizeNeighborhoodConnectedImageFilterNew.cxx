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


#include "otbLabelizeNeighborhoodConnectedImageFilter.h"
#include "otbImage.h"

int otbLabelizeNeighborhoodConnectedImageFilterNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  typedef unsigned char InputPixelType;
  typedef unsigned char OutputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<InputPixelType, Dimension>  InputImageType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;

  typedef otb::LabelizeNeighborhoodConnectedImageFilter
  <InputImageType, OutputImageType> LabelizeFilterType;
  typedef LabelizeFilterType::Pointer LabelizeFilterPointerType;

  LabelizeFilterPointerType filter = LabelizeFilterType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
