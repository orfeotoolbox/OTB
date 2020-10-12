/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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


/* Example usage:
./HillShadingExample Output/HillShadingExample.png Output/HillShadingColorExample.png 6.5 45.5 500 500 0.002 -0.002 Input/DEM/srtm_directory
*/


#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include "otbDEMToImageGenerator.h"
#include "otbHillShadingFilter.h"

#include "itkScalarToRGBColormapImageFilter.h"
#include "otbReliefColormapFunctor.h"
#include "itkMultiplyImageFilter.h"
#include "itkShiftScaleImageFilter.h"
#include "otbWorldFile.h"

int main(int argc, char* argv[])
{
  if (argc < 10)
  {
    std::cout << argv[0] << " <output_filename> <output_color_filename> "
              << " <Longitude Output Origin point> <Latitude Output Origin point>"
              << " <X Output Size> <Y Output size>"
              << " <X Spacing> <Y Spacing> <DEM folder path>"
              << " [Projection Ref]" << std::endl;
    return EXIT_FAILURE;
  }

  using PixelType        = double;
  using UCharPixelType   = unsigned char;
  using RGBPixelType     = itk::RGBPixel<UCharPixelType>;
  using ImageType        = otb::Image<PixelType, 2>;
  using RGBImageType     = otb::Image<RGBPixelType, 2>;
  using ScalarImageType  = otb::Image<UCharPixelType, 2>;
  using WriterType       = otb::ImageFileWriter<RGBImageType>;
  using ScalarWriterType = otb::ImageFileWriter<ScalarImageType>;

  ScalarWriterType::Pointer writer = ScalarWriterType::New();
  writer->SetFileName(argv[1]);

  WriterType::Pointer writer2 = WriterType::New();
  writer2->SetFileName(argv[2]);

  using DEMToImageGeneratorType = otb::DEMToImageGenerator<ImageType>;

  DEMToImageGeneratorType::Pointer demToImage = DEMToImageGeneratorType::New();

  using SizeType    = DEMToImageGeneratorType::SizeType;
  using SpacingType = DEMToImageGeneratorType::SpacingType;
  using PointType   = DEMToImageGeneratorType::PointType;

  otb::DEMHandler::GetInstance().OpenDEMDirectory(argv[9]);

  PointType origin;
  origin[0] = ::atof(argv[3]);
  origin[1] = ::atof(argv[4]);

  demToImage->SetOutputOrigin(origin);

  SizeType size;
  size[0] = ::atoi(argv[5]);
  size[1] = ::atoi(argv[6]);

  demToImage->SetOutputSize(size);

  SpacingType spacing;
  spacing[0] = ::atof(argv[7]);
  spacing[1] = ::atof(argv[8]);

  demToImage->SetOutputSpacing(spacing);
  double res = 0;

  if (argc > 10)
  {
    demToImage->SetOutputProjectionRef(argv[10]);
    res = spacing[0];
  }
  else
  {
    // Compute the resolution (Vincenty formula)
    double lon1 = origin[0];
    double lon2 = origin[0] + size[0] * spacing[0];
    double lat1 = origin[1];
    double lat2 = origin[1] + size[1] * spacing[1];
    double R    = 6371; // km
    double d    = std::acos(std::sin(lat1) * std::sin(lat2) + std::cos(lat1) * std::cos(lat2) * std::cos(lon2 - lon1)) * R;
    res         = d / std::sqrt(2.0);
  }
  // After generating the DEM image as in the DEMToImageGenerator example, you can declare
  // the hill shading mechanism. The hill shading is implemented as a functor doing some
  // operations in its neighborhood. A convenient filter called \doxygen{otb}{HillShadingFilter}
  // is defined around this mechanism.

  using HillShadingFilterType                = otb::HillShadingFilter<ImageType, ImageType>;
  HillShadingFilterType::Pointer hillShading = HillShadingFilterType::New();
  hillShading->SetRadius(1);
  hillShading->SetInput(demToImage->GetOutput());

  hillShading->GetFunctor().SetXRes(res);
  hillShading->GetFunctor().SetYRes(res);

  using RescalerType             = itk::ShiftScaleImageFilter<ImageType, ScalarImageType>;
  RescalerType::Pointer rescaler = RescalerType::New();
  rescaler->SetScale(255.0);
  rescaler->SetInput(hillShading->GetOutput());

  writer->SetInput(rescaler->GetOutput());

  using ColorMapFilterType                = itk::ScalarToRGBColormapImageFilter<ImageType, RGBImageType>;
  ColorMapFilterType::Pointer colormapper = ColorMapFilterType::New();
  colormapper->UseInputImageExtremaForScalingOff();

  using ColorMapFunctorType             = otb::Functor::ReliefColormapFunctor<PixelType, RGBPixelType>;
  ColorMapFunctorType::Pointer colormap = ColorMapFunctorType::New();
  colormap->SetMinimumInputValue(0);
  colormap->SetMaximumInputValue(4000);
  colormapper->SetColormap(colormap);

  colormapper->SetInput(demToImage->GetOutput());

  using MultiplyFilterType =
      itk::BinaryFunctorImageFilter<RGBImageType, ImageType, RGBImageType, otb::Functor::HillShadeModulationFunctor<RGBPixelType, PixelType, RGBPixelType>>;

  MultiplyFilterType::Pointer multiply = MultiplyFilterType::New();
  multiply->SetInput1(colormapper->GetOutput());
  multiply->SetInput2(hillShading->GetOutput());

  writer2->SetInput(multiply->GetOutput());

  writer->Update();
  writer2->Update();

  otb::WorldFile::Pointer worldFile = otb::WorldFile::New();
  worldFile->SetLonOrigin(origin[0]);
  worldFile->SetLatOrigin(origin[1]);
  worldFile->SetLonSpacing(spacing[0]);
  worldFile->SetLatSpacing(spacing[1]);

  worldFile->SetImageFilename(argv[1]);
  worldFile->Update();
  worldFile->SetImageFilename(argv[2]);
  worldFile->Update();
}
