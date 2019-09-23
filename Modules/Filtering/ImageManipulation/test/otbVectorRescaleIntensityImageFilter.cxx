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


#include "otbVectorRescaleIntensityImageFilter.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbMultiChannelExtractROI.h"

int otbVectorRescaleIntensityImageFilter(int itkNotUsed(argc), char* argv[])
{
  const char*         infname  = argv[1];
  const char*         outfname = argv[2];
  const unsigned char min      = atoi(argv[3]);
  const unsigned char max      = atoi(argv[4]);

  const unsigned int    Dimension = 2;
  typedef double        InputPixelType;
  typedef unsigned char OutputPixelType;
  typedef otb::VectorImage<InputPixelType, Dimension>                             InputImageType;
  typedef otb::VectorImage<OutputPixelType, Dimension>                            OutputImageType;
  typedef otb::VectorRescaleIntensityImageFilter<InputImageType, OutputImageType> VectorRescaleIntensityImageFilterType;
  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;
  typedef otb::MultiChannelExtractROI<OutputPixelType, OutputPixelType> SelectBandsFilterType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);
  reader->GenerateOutputInformation();

  // Instantiating object
  VectorRescaleIntensityImageFilterType::Pointer filter = VectorRescaleIntensityImageFilterType::New();
  filter->SetInput(reader->GetOutput());
  OutputImageType::PixelType minPixel, maxPixel;
  minPixel.SetSize(reader->GetOutput()->GetNumberOfComponentsPerPixel());
  maxPixel.SetSize(reader->GetOutput()->GetNumberOfComponentsPerPixel());
  minPixel.Fill(min);
  maxPixel.Fill(max);
  filter->SetOutputMinimum(minPixel);
  filter->SetOutputMaximum(maxPixel);
  filter->SetClampThreshold(0.01);

  SelectBandsFilterType::Pointer selecter = SelectBandsFilterType::New();
  selecter->SetInput(filter->GetOutput());
  selecter->SetChannel(1);
  selecter->SetChannel(2);
  selecter->SetChannel(3);
  selecter->SetExtractionRegion(reader->GetOutput()->GetLargestPossibleRegion());

  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(selecter->GetOutput());
  writer->SetFileName(outfname);
  writer->Update();

  return EXIT_SUCCESS;
}
