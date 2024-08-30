/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include "itkScalarToRGBColormapImageFilter.h"
#include "otbScalarToRainbowRGBPixelFunctor.h"

int otbScalarToRainbowRGBPixelFunctor(int itkNotUsed(argc), char* argv[])
{
  using PixelType    = unsigned char;
  using RGBPixelType = itk::RGBPixel<PixelType>;
  using ImageType    = otb::Image<PixelType, 2>;
  using RGBImageType = otb::Image<RGBPixelType, 2>;
  using ReaderType   = otb::ImageFileReader<ImageType>;
  using WriterType   = otb::ImageFileWriter<RGBImageType>;

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  reader->SetFileName(argv[1]);
  writer->SetFileName(argv[2]);

  using ColorMapFunctorType = otb::Functor::ScalarToRainbowRGBPixelFunctor<PixelType, RGBPixelType>;
  ColorMapFunctorType::Pointer colormap = ColorMapFunctorType::New();
  colormap->SetMinimumInputValue(70);
  colormap->SetMaximumInputValue(150);

  using ColorMapFilterType  = itk::ScalarToRGBColormapImageFilter<ImageType, RGBImageType>;
  ColorMapFilterType::Pointer colormapper = ColorMapFilterType::New();
  colormapper->SetColormap(colormap);

  colormapper->SetUseInputImageExtremaForScaling(false);
  colormapper->SetInput(reader->GetOutput());
  writer->SetInput(colormapper->GetOutput());

  writer->Update();

  return EXIT_SUCCESS;
}
