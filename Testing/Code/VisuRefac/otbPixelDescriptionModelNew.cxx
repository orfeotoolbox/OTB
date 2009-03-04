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
#include "otbPixelDescriptionModel.h"
#include "otbImage.h"
#include "otbImageLayer.h"

int otbPixelDescriptionModelNew( int argc, char * argv[] )
{
  typedef otb::Image<double,2>                  ImageType;
  typedef otb::ImageLayer<ImageType>            LayerType;
  typedef otb::PixelDescriptionModel<LayerType> ModelType;

  // Instantiation
  ModelType::Pointer model = ModelType::New();

  return EXIT_SUCCESS;
}
