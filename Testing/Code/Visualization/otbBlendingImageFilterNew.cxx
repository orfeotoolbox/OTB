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
#include "otbBlendingImageFilter.h"
#include "otbImage.h"
#include "itkRGBPixel.h"

int otbBlendingImageFilterNew( int argc, char * argv[] )
{
  typedef unsigned char                 ValueType;
  typedef itk::RGBPixel<ValueType>      PixelType;
  typedef otb::Image<PixelType,2>       ImageType;
  typedef otb::BlendingImageFilter<ImageType> BlendingFilterType;

  BlendingFilterType::Pointer rendering = BlendingFilterType::New();

  return EXIT_SUCCESS;
}
