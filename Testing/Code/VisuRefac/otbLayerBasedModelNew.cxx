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
#include "otbImageLayerBase.h"
#include "otbLayerBasedModel.h"
#include "otbImage.h"
#include "itkRGBPixel.h"

int otbLayerBasedModelNew( int argc, char * argv[] )
{
  typedef itk::RGBPixel<unsigned char>     RGBPixelType;
  typedef otb::Image<RGBPixelType,2>       ImageType;
  typedef otb::ImageLayerBase<ImageType>   LayerType;
  typedef otb::LayerBasedModel<LayerType>  ModelType;

  // Instantiation
  ModelType::Pointer model = ModelType::New();

  return EXIT_SUCCESS;
}
