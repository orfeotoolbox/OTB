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

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#ifdef __BORLANDC__
#define ITK_LEAN_AND_MEAN
#endif



#include "otbImage.h"

#include "itkBinaryFunctorImageFilter.h"
#include "otbAmplitudePhaseToRGBFunctor.h"

int otbFunctorNew(int argc, char * argv[])
{

  typedef double PixelType;
  typedef otb::Image<PixelType, 2> ImageType;

  typedef itk::RGBPixel<unsigned char> RGBPixelType;
  typedef otb::Image<RGBPixelType, 2> RGBImageType;

  typedef otb::Functor::AmplitudePhaseToRGBFunctor
      <PixelType,PixelType,RGBPixelType> ColorMapFunctorType;
  typedef itk::BinaryFunctorImageFilter
      <ImageType, ImageType, RGBImageType, ColorMapFunctorType> ColorMapFilterType;
  ColorMapFilterType::Pointer colormapper = ColorMapFilterType::New();
  colormapper->GetFunctor().SetMaximum(150);
  colormapper->GetFunctor().SetMinimum(70);


  return EXIT_SUCCESS;
}
