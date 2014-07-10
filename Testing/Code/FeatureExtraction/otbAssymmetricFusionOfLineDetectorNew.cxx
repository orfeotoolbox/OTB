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
#include "itkLinearInterpolateImageFunction.h"

#include <iostream>

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include "otbAssymmetricFusionOfLineDetectorImageFilter.h"

int otbAssymmetricFusionOfLineDetectorNew(int itkNotUsed(argc), char ** itkNotUsed(argv))
{
  typedef unsigned char InputPixelType;
  typedef double        OutputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<InputPixelType,  Dimension> InputImageType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageDirectionType;

  typedef itk::LinearInterpolateImageFunction<InputImageType, double> InterpolatorType;

  typedef otb::AssymmetricFusionOfLineDetectorImageFilter<InputImageType, OutputImageType, OutputImageDirectionType,
      InterpolatorType> FilterType;

  FilterType::Pointer filter = FilterType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
