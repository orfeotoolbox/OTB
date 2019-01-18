/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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
#include "otbSarBrightnessToImageFilter.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkExtractImageFilter.h"

int otbSarBrightnessToImageFilterTestWithoutNoise(int argc, char * argv[])
{
  const unsigned int Dimension = 2;
  typedef float                                                            RealType;
  typedef RealType                                                         PixelType;
  typedef otb::Image<PixelType, Dimension>                                 InputImageType;
  typedef otb::Image<RealType, Dimension>                                  OutputImageType;
  typedef otb::ImageFileReader<InputImageType>                             ReaderType;
  typedef otb::ImageFileWriter<OutputImageType>                            WriterType;
  typedef otb::SarBrightnessToImageFilter<InputImageType, OutputImageType> FilterType;
  typedef itk::ExtractImageFilter<OutputImageType, OutputImageType>        ExtractorType;

  // Instantiating object
  FilterType::Pointer filter = FilterType::New();
  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  ExtractorType::Pointer extractor = ExtractorType::New();

  reader->SetFileName(argv[1]);
  writer->SetFileName(argv[2]);
  filter->SetInput(reader->GetOutput());
  filter->SetEnableNoise(false);

  if (argc > 3)
    {
    // Generate an extract from the large input
    OutputImageType::RegionType region;
    OutputImageType::IndexType  id;
    id[0] = atoi(argv[3]);   id[1] = atoi(argv[4]);
    OutputImageType::SizeType size;
    size[0] = atoi(argv[5]);   size[1] = atoi(argv[6]);
    region.SetIndex(id);
    region.SetSize(size);
    extractor->SetExtractionRegion(region);

    extractor->SetInput(filter->GetOutput());
    writer->SetInput(extractor->GetOutput());
    }
  else
    {
    // Calibrate the whole image
    writer->SetInput(filter->GetOutput());
    }
  writer->Update();

  return EXIT_SUCCESS;
}
