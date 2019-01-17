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

#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include "otbVectorDataFileReader.h"
#include "otbVectorDataProjectionFilter.h"

#include "otbRasterizeVectorDataFilter.h"
#include "otbStandardOneLineFilterWatcher.h"

typedef otb::VectorImage<float, 2>                            ImageType;
typedef otb::VectorData<>                                     VectorDataType;
typedef otb::ImageFileReader<ImageType>                       ReaderType;
typedef otb::VectorDataFileReader<VectorDataType>             VDReaderType;
typedef otb::VectorDataProjectionFilter<VectorDataType,
                                        VectorDataType>       VDProjectionType;
typedef otb::ImageFileWriter<ImageType>              WriterType;
typedef otb::RasterizeVectorDataFilter<VectorDataType,
                                       ImageType,
                                       ImageType>             RasterizationFilterType;


int otbVectorDataRasterizeFilter(int itkNotUsed(argc), char * argv[])
{

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);
  reader->UpdateOutputInformation();

  // Read and project the vd into the raster coordinates images
  VDReaderType::Pointer vdreader = VDReaderType::New();
  vdreader->SetFileName(argv[2]);

  VDProjectionType::Pointer vdproj = VDProjectionType::New();
  vdproj->SetInput(vdreader->GetOutput());
  vdproj->SetInputProjectionRef(vdreader->GetOutput()->GetProjectionRef());
  vdproj->SetOutputProjectionRef(reader->GetOutput()->GetProjectionRef());
  vdproj->Update();

  // rasterize
  RasterizationFilterType::Pointer  rasterization = RasterizationFilterType::New();
  rasterization->SetInput(reader->GetOutput());
  rasterization->AddVectorData(vdproj->GetOutput());

  // choose the colors
  ImageType::PixelType outputcolor;
  outputcolor.SetSize(reader->GetOutput()->GetNumberOfComponentsPerPixel());
  outputcolor[0] = 0.;
  outputcolor[1] = 0.;
  outputcolor[2] = 255.;
  outputcolor[3] = 0.;
  rasterization->AddColor(outputcolor);

  WriterType::Pointer writer  = WriterType::New();
  writer->SetFileName(argv[3]);
  writer->SetInput(rasterization->GetOutput());
  writer->Update();

return EXIT_SUCCESS;
}
