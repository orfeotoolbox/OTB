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


#include "otbLabelImageToOGRDataSourceFilter.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbVectorDataFileWriter.h"


int otbLabelImageToOGRDataSourceFilter(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputLabelImageFile " << std::endl;
    return EXIT_FAILURE;
  }
  const char* infname = argv[1];

  const unsigned int     Dimension = 2;
  typedef unsigned short LabelType;
  typedef otb::Image<LabelType, Dimension> InputLabelImageType;

  typedef otb::LabelImageToOGRDataSourceFilter<InputLabelImageType> FilterType;
  typedef otb::ImageFileReader<InputLabelImageType>                 LabelImageReaderType;


  FilterType::Pointer           filter = FilterType::New();
  LabelImageReaderType::Pointer reader = LabelImageReaderType::New();

  reader->SetFileName(infname);

  filter->SetInput(reader->GetOutput());
  filter->Update();


  return EXIT_SUCCESS;
}
