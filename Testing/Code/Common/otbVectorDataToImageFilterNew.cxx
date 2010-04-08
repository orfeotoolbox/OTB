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

#include "itkRGBAPixel.h"
#include "otbImage.h"
#include "otbVectorData.h"
#include "otbVectorDataToImageFilter.h"

int otbVectorDataToImageFilterNew(int argc, char * argv[])
{

  typedef otb::VectorData<>             VectorDataType;
  typedef itk::RGBAPixel<unsigned char> PixelType;
  typedef otb::Image<PixelType, 2>      ImageType;

  typedef otb::VectorDataToImageFilter<VectorDataType, ImageType> VectorDataToImageFilterType;
  VectorDataToImageFilterType::Pointer vectorDataRendering = VectorDataToImageFilterType::New();

  return EXIT_SUCCESS;
}
