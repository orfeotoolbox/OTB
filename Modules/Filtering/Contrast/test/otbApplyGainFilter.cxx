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

#include "otbImageFileWriter.h"
#include "otbImageFileReader.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbApplyGainFilter.h"

int otbApplyGainFilter(int itkNotUsed(argc), char* argv[])
{
  typedef int        InputPixelType;
  typedef double     LutPixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<InputPixelType, Dimension>     InputImageType;
  typedef otb::VectorImage<LutPixelType, Dimension> LutImageType;
  typedef otb::ApplyGainFilter<InputImageType, LutImageType, InputImageType> FilterType;


  typedef otb::ImageFileReader<InputImageType> ReaderType;
  typedef otb::ImageFileReader<LutImageType>   ReaderLutType;
  typedef otb::ImageFileWriter<InputImageType> WriterType;
  ReaderType::Pointer                          reader(ReaderType::New());
  ReaderLutType::Pointer                       readerLut(ReaderLutType::New());
  WriterType::Pointer                          writer(WriterType::New());
  reader->SetFileName(argv[1]);
  readerLut->SetFileName(argv[2]);
  writer->SetFileName(argv[3]);
  reader->UpdateOutputInformation();
  readerLut->UpdateOutputInformation();

  FilterType::Pointer appGain(FilterType::New());

  appGain->SetInputLut(readerLut->GetOutput());
  appGain->SetInputImage(reader->GetOutput());
  appGain->SetMin(0);
  appGain->SetMax(255);
  appGain->ThumbSizeFromSpacingOn();

  auto size = reader->GetOutput()->GetLargestPossibleRegion().GetSize();
  appGain->SetThumbSize(size);

  writer->SetInput(appGain->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}