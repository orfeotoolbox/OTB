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

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"


#include "otbOGRDataSourceToLabelImageFilter.h"
#include "otbStandardOneLineFilterWatcher.h"

typedef otb::Image<unsigned int, 2> ImageType;
typedef otb::ImageFileReader<ImageType> ReaderType;

typedef otb::ImageFileWriter<ImageType>                 WriterType;
typedef otb::OGRDataSourceToLabelImageFilter<ImageType> RasterizationFilterType;


int otbOGRDataSourceToLabelImageFilter(int itkNotUsed(argc), char* argv[])
{

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);
  reader->UpdateOutputInformation();

  bool          mode       = atoi(argv[4]);
  unsigned char background = atoi(argv[5]);
  unsigned char foreground = atoi(argv[6]);

  otb::ogr::DataSource::Pointer ogrDS = otb::ogr::DataSource::New(argv[2], otb::ogr::DataSource::Modes::Read);

  // rasterize
  RasterizationFilterType::Pointer rasterization = RasterizationFilterType::New();
  rasterization->AddOGRDataSource(ogrDS);
  rasterization->SetOutputParametersFromImage(reader->GetOutput());
  rasterization->SetBurnAttribute("DN");
  rasterization->SetBurnAttributeMode(mode);
  rasterization->SetBackgroundValue(background);
  rasterization->SetForegroundValue(foreground);

  /*otb::StandardOneLineFilterWatcher * watch = new otb::StandardOneLineFilterWatcher(rasterization.GetPointer(),
                                                                          "rasterization"); */

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(argv[3]);
  writer->SetInput(rasterization->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
