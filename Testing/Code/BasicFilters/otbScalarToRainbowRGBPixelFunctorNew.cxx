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

#include "itkUnaryFunctorImageFilter.h"
#include "otbScalarToRainbowRGBPixelFunctor.h"

int otbScalarToRainbowRGBPixelFunctorNew(int itkNotUsed(argc), char ** itkNotUsed(argv))
{

  typedef unsigned char               PixelType;
  typedef itk::RGBPixel<PixelType>    RGBPixelType;
  typedef otb::Image<PixelType, 2>    ImageType;
  typedef otb::Image<RGBPixelType, 2> RGBImageType;

  typedef otb::Functor::ScalarToRainbowRGBPixelFunctor<PixelType>
  ColorMapFunctorType;
  typedef itk::UnaryFunctorImageFilter<ImageType,
      RGBImageType, ColorMapFunctorType> ColorMapFilterType;
  ColorMapFilterType::Pointer colormapper = ColorMapFilterType::New();
  colormapper->GetFunctor().SetMaximum(150);
  colormapper->GetFunctor().SetMinimum(70);

  std::cout << colormapper << std::endl;

  return EXIT_SUCCESS;
}
