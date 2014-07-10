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
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include "itkUnaryFunctorImageFilter.h"
#include "otbScalarToRainbowRGBPixelFunctor.h"

int otbScalarToRainbowRGBPixelFunctor(int itkNotUsed(argc), char * argv[])
{
  typedef unsigned char                               PixelType;
  typedef itk::RGBPixel<PixelType>                    RGBPixelType;
  typedef otb::Image<PixelType, 2>                    ImageType;
  typedef otb::Image<RGBPixelType, 2>                 RGBImageType;
  typedef otb::ImageFileReader<ImageType>             ReaderType;
  typedef otb::ImageFileWriter<RGBImageType> WriterType;

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  reader->SetFileName(argv[1]);
  writer->SetFileName(argv[2]);

  typedef otb::Functor::ScalarToRainbowRGBPixelFunctor<PixelType>
  ColorMapFunctorType;
  typedef itk::UnaryFunctorImageFilter<ImageType,
      RGBImageType, ColorMapFunctorType> ColorMapFilterType;
  ColorMapFilterType::Pointer colormapper = ColorMapFilterType::New();
  colormapper->GetFunctor().SetMaximumInputValue(150);
  colormapper->GetFunctor().SetMinimumInputValue(70);

  colormapper->SetInput(reader->GetOutput());
  writer->SetInput(colormapper->GetOutput());

  writer->Update();

  return EXIT_SUCCESS;
}
