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
#include "otbEdgeDensityImageFilter.h"
#include "otbBinaryImageDensityFunction.h"
#include "itkCannyEdgeDetectionImageFilter.h"

int otbEdgeDensityImageFilterNew(int, char*[])
{

  const unsigned int Dimension = 2;
  typedef float PixelType;

  typedef otb::Image<PixelType, Dimension> ImageType;
  typedef otb::BinaryImageDensityFunction<ImageType>               CountFunctionType;
  typedef itk::CannyEdgeDetectionImageFilter<ImageType, ImageType> CannyDetectorType;

  typedef otb::EdgeDensityImageFilter<ImageType, ImageType, CannyDetectorType, CountFunctionType> EdgeDensityFilterType;

  /**Instantiation of an object*/
  EdgeDensityFilterType::Pointer filter =      EdgeDensityFilterType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
