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
./ScalingFilterExample Input/QB_Toulouse_Ortho_PAN.tif Output/QB_Toulouse_Ortho_PAN_rescaled.png Output/QB_Toulouse_Ortho_PAN_casted.png
*/

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkUnaryFunctorImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkCastImageFilter.h"

int main(int argc, char* argv[])
{

  if (argc != 4)
  {
    std::cerr << "Usage: " << argv[0] << " <inputImageFile> ";
    std::cerr << " <outputRescaledImageFile> <outputCastedImageFile>" << std::endl;
    return EXIT_FAILURE;
  }

  using InputPixelType  = unsigned short;
  using OutputPixelType = unsigned char;
  using InputImageType  = otb::Image<InputPixelType, 2>;
  using OutputImageType = otb::Image<OutputPixelType, 2>;

  using ReaderType           = otb::ImageFileReader<InputImageType>;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);

  // The RescaleIntensityImageFilter is used to rescale the value
  using RescalerType             = itk::RescaleIntensityImageFilter<InputImageType, OutputImageType>;
  RescalerType::Pointer rescaler = RescalerType::New();
  rescaler->SetInput(reader->GetOutput());

  using WriterType           = otb::ImageFileWriter<OutputImageType>;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(argv[2]);
  writer->SetInput(rescaler->GetOutput());
  writer->Update();

  using CasterType           = itk::CastImageFilter<InputImageType, OutputImageType>;
  CasterType::Pointer caster = CasterType::New();
  caster->SetInput(reader->GetOutput());

  writer->SetFileName(argv[3]);
  writer->SetInput(caster->GetOutput());
  writer->Update();
}
