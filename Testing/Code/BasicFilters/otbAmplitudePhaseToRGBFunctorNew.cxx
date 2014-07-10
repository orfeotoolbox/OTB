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

#include "itkTernaryFunctorImageFilter.h"
#include "otbAmplitudePhaseToRGBFunctor.h"

int otbAmplitudePhaseToRGBFunctorNew(int itkNotUsed(argc), char ** itkNotUsed(argv))
{

  typedef double                   PixelType;
  typedef otb::Image<PixelType, 2> ImageType;

  typedef itk::RGBPixel<unsigned char> RGBPixelType;
  typedef otb::Image<RGBPixelType, 2>  RGBImageType;

  typedef otb::Functor::AmplitudePhaseToRGBFunctor
  <PixelType, PixelType, PixelType, RGBPixelType> ColorMapFunctorType;
  typedef itk::TernaryFunctorImageFilter
  <ImageType, ImageType, ImageType, RGBImageType, ColorMapFunctorType> ColorMapFilterType;
  ColorMapFilterType::Pointer colormapper = ColorMapFilterType::New();
  colormapper->GetFunctor().SetMaximum(150);
  colormapper->GetFunctor().SetMinimum(70);

  std::cout << colormapper << std::endl;

  return EXIT_SUCCESS;
}
