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

#include "otbVectorImage.h"
#include "otbImage.h"
#include "otbImageFileWriter.h"
#include "otbImageFileReader.h"
#include "otbComputeGainLutFilter.h"
#include "otbMacro.h"

int otbComputeGainLutFilter(int itkNotUsed(argc), char* argv[])
{
  typedef int        InputPixelType;
  typedef double     OutputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::VectorImage<InputPixelType, Dimension>             HistoImageType;
  typedef otb::Image<InputPixelType, Dimension>                   InputImageType;
  typedef otb::VectorImage<OutputPixelType, Dimension>            LutImageType;
  typedef otb::ComputeGainLutFilter<HistoImageType, LutImageType> FilterType;

  typedef otb::ImageFileReader<InputImageType> ReaderImageType;
  typedef otb::ImageFileReader<HistoImageType> ReaderType;
  typedef otb::ImageFileWriter<LutImageType>   WriterType;
  ReaderImageType::Pointer                     readerImage(ReaderImageType::New());
  ReaderType::Pointer                          reader(ReaderType::New());
  WriterType::Pointer                          writer(WriterType::New());
  readerImage->SetFileName(argv[1]);
  reader->SetFileName(argv[2]);
  writer->SetFileName(argv[3]);
  reader->UpdateOutputInformation();
  readerImage->UpdateOutputInformation();

  FilterType::Pointer computeGainLut(FilterType::New());

  computeGainLut->SetInput(reader->GetOutput());
  computeGainLut->SetMin(0);
  computeGainLut->SetMax(255);
  auto size = readerImage->GetOutput()->GetLargestPossibleRegion().GetSize();
  size[0] /= 4;
  size[1] /= 4;
  auto nbPix = size[0] * size[1];
  computeGainLut->SetNbPixel(nbPix);

  writer->SetInput(computeGainLut->GetOutput());
  writer->Update();

  auto index = computeGainLut->GetOutput()->GetLargestPossibleRegion().GetIndex();
  otbLogMacro(Debug, << computeGainLut->GetOutput()->GetPixel(index));

  return EXIT_SUCCESS;
}
