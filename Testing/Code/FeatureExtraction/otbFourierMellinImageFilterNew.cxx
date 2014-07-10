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



#include "otbImage.h"
#include "otbForwardFourierMellinTransformImageFilter.h"
#include "itkLinearInterpolateImageFunction.h"

int otbFourierMellinImageFilterNew(int itkNotUsed(argc), char* argv[])
{

  typedef double PixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<PixelType, Dimension> InputImageType;
  typedef otb::Image<PixelType, Dimension> OutputImageType;

  typedef itk::LinearInterpolateImageFunction<InputImageType, double> InterpolatorType;
  typedef otb::ForwardFourierMellinTransformImageFilter<PixelType,
      InterpolatorType, Dimension>       FourierMellinTransformType;

  FourierMellinTransformType::Pointer filter = FourierMellinTransformType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
