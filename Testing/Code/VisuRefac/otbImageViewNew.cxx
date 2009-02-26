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
#include "otbImageViewerModel.h"
#include "otbImage.h"
#include "itkRGBPixel.h"
#include "otbImageView.h"

int otbImageViewNew( int argc, char * argv[] )
{
  typedef itk::RGBPixel<unsigned char>     RGBPixelType;
  typedef otb::Image<RGBPixelType,2>       ImageType;
  typedef otb::ImageViewerModel<ImageType> ModelType;
  typedef otb::ImageView<ModelType>        ViewType;

  // Instantiation
  ViewType::Pointer model = ViewType::New();

  return EXIT_SUCCESS;
}
