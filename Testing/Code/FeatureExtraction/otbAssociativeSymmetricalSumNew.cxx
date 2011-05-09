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



#include "itkExceptionObject.h"
#include "itkImage.h"
#include <iostream>

#include "otbAssociativeSymmetricalSumImageFilter.h"

int otbAssociativeSymmetricalSumNew(int argc, char* argv[])
{
  typedef double InputPixelType;
  typedef double OutputPixelType;
  const unsigned int Dimension = 2;

  typedef itk::Image<InputPixelType,  Dimension> InputImageType1;
  typedef itk::Image<InputPixelType,  Dimension> InputImageType2;
  typedef itk::Image<OutputPixelType, Dimension> OutputImageType;

  typedef otb::AssociativeSymmetricalSumImageFilter<InputImageType1, InputImageType2, OutputImageType> FilterType;

  FilterType::Pointer filter = FilterType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
