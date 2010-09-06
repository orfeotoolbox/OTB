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

#include "otbOptResampleImageFilter.h"
#include "otbVectorImage.h"
#include "itkVector.h"
#include "otbImage.h"

int otbOptResampleImageFilterNew(int argc, char* argv[])
{
  // Images definition
  const unsigned int Dimension = 2;
  typedef double                                      PixelType;
  typedef otb::VectorImage<PixelType, Dimension>      ImageType;
  typedef itk::Vector<PixelType, 2>                   DeformationValueType;
  typedef otb::Image<DeformationValueType, Dimension> DeformationFieldType;

  // 
  typedef otb::OptResampleImageFilter<ImageType, ImageType, DeformationFieldType> ImageResamplerType;

  // 
  ImageResamplerType::Pointer resampler = ImageResamplerType::New();
  return 0;
}
