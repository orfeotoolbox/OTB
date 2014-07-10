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
#include "otbVectorDataToMapFilter.h"

int otbVectorDataToMapFilterNew(int itkNotUsed(argc), char ** itkNotUsed(argv))
{

  typedef otb::VectorData<>             VectorDataType;
  typedef itk::RGBAPixel<unsigned char> PixelType;
  typedef otb::Image<PixelType, 2>      ImageType;

  typedef otb::VectorDataToMapFilter<VectorDataType, ImageType> VectorDataToMapFilterType;
  VectorDataToMapFilterType::Pointer vectorDataRendering = VectorDataToMapFilterType::New();

  std::cout << vectorDataRendering << std::endl;

  return EXIT_SUCCESS;
}
