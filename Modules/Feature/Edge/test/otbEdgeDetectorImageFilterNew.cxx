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
#include "otbEdgeDetectorImageFilter.h"
#include "itkSobelEdgeDetectionImageFilter.h"

int otbEdgeDetectorImageFilterNew(int, char*[])
{
  typedef double PixelType;

  typedef otb::Image<PixelType, 2>                                      ImageType;
  typedef itk::SobelEdgeDetectionImageFilter<ImageType, ImageType>      SobelType;
  typedef otb::EdgeDetectorImageFilter<ImageType, ImageType, SobelType> FilterType;

  /**Instantiation of an object*/
  FilterType::Pointer filter = FilterType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
