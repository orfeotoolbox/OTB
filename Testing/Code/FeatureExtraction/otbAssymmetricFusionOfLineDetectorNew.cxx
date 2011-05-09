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
#include "itkImageFileWriter.h"
#include "itkLinearInterpolateImageFunction.h"

#include <iostream>

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include "otbAssymmetricFusionOfLineDetectorImageFilter.h"

int otbAssymmetricFusionOfLineDetectorNew(int argc, char * argv[])
{
  typedef unsigned char InputPixelType;
  typedef double        OutputPixelType;
  const unsigned int Dimension = 2;

  typedef itk::Image<InputPixelType,  Dimension> InputImageType;
  typedef itk::Image<OutputPixelType, Dimension> OutputImageType;
  typedef itk::Image<OutputPixelType, Dimension> OutputImageDirectionType;

  typedef itk::LinearInterpolateImageFunction<InputImageType, double> InterpolatorType;

  typedef otb::AssymmetricFusionOfLineDetectorImageFilter<InputImageType, OutputImageType, OutputImageDirectionType,
      InterpolatorType> FilterType;

  FilterType::Pointer filter = FilterType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
