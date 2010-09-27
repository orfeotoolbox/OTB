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
#include "otbImageLayer.h"
#include "otbImage.h"

int otbImageLayerNew(int argc, char * argv[])
{
  typedef otb::Image<double, 2>                        ImageType;
  typedef otb::Image<itk::RGBAPixel<unsigned char>, 2> OutputImageType;
  typedef otb::ImageLayer<ImageType, OutputImageType>  LayerType;

  LayerType::Pointer layer = LayerType::New();

  std::cout << layer << std::endl;

  return EXIT_SUCCESS;
}
