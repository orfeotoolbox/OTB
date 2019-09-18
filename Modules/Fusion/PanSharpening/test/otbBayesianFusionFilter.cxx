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


#include "itkMacro.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImage.h"

#include "otbBayesianFusionFilter.h"

int otbBayesianFusionFilter(int itkNotUsed(argc), char* argv[])
{
  const char* multispect       = argv[1];
  const char* multispectinterp = argv[2];
  const char* panchro          = argv[3];
  const char* output           = argv[4];

  const unsigned int Dimension = 2;
  typedef double     PixelType;

  typedef otb::VectorImage<PixelType, Dimension> VectorImageType;
  typedef otb::Image<PixelType, Dimension>       PanchroImageType;
  typedef otb::ImageFileReader<VectorImageType>  VectorReaderType;
  typedef otb::ImageFileReader<PanchroImageType> ImageReaderType;
  typedef otb::ImageFileWriter<VectorImageType>  VectorImageWriterType;
  typedef otb::BayesianFusionFilter<VectorImageType, VectorImageType, PanchroImageType, VectorImageType> FilterType;

  VectorReaderType::Pointer      multiSpectReader       = VectorReaderType::New();
  VectorReaderType::Pointer      multiSpectInterpReader = VectorReaderType::New();
  ImageReaderType::Pointer       panchroReader          = ImageReaderType::New();
  FilterType::Pointer            filter                 = FilterType::New();
  VectorImageWriterType::Pointer writer                 = VectorImageWriterType::New();

  multiSpectReader->SetFileName(multispect);
  multiSpectInterpReader->SetFileName(multispectinterp);
  panchroReader->SetFileName(panchro);

  filter->SetMultiSpect(multiSpectReader->GetOutput());
  filter->SetMultiSpectInterp(multiSpectInterpReader->GetOutput());
  filter->SetPanchro(panchroReader->GetOutput());
  writer->SetInput(filter->GetOutput());
  writer->SetFileName(output);
  writer->Update();

  return EXIT_SUCCESS;
}
