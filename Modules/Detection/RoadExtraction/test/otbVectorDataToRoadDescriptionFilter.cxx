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



#include "otbVectorDataToRoadDescriptionFilter.h"

#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbVectorDataFileReader.h"
#include "otbVectorDataFileWriter.h"
#include "otbVectorDataProperties.h"
#include "otbRemoteSensingRegion.h"
#include "otbVectorDataIntoImageProjectionFilter.h"





int otbVectorDataToRoadDescriptionFilter(int itkNotUsed(argc), char* argv[])
{
  const char * inputVD  = argv[1];
  const char * inputImg = argv[2];
  const char * inputDB  = argv[3];
  const char * outputVD = argv[4];
  const char * DEMDirectory = argv[5];
  int DisplayWarnings   = atoi(argv[6]);

  typedef double                             PrecisionType;
  typedef otb::VectorData<PrecisionType, 2>  VectorDataType;
  typedef otb::VectorImage<PrecisionType>    ImageType;

  typedef otb::VectorDataToRoadDescriptionFilter<VectorDataType, ImageType>
                                          VectorDataToRoadDescriptionFilterType;

  typedef otb::ImageFileReader<ImageType> ImageReaderType;
  typedef otb::VectorDataFileReader<VectorDataType>
                                          VectorDataReaderType;
  typedef otb::VectorDataFileWriter<VectorDataType>
                                          VectorDataWriterType;
  typedef otb::VectorDataIntoImageProjectionFilter<VectorDataType, ImageType>
                                          VectorDataReProjFilter;


  VectorDataReaderType::Pointer vdReader = VectorDataReaderType::New();
  VectorDataReaderType::Pointer DBReader = VectorDataReaderType::New();
  ImageReaderType::Pointer      reader   = ImageReaderType::New();
  VectorDataWriterType::Pointer vdWriter = VectorDataWriterType::New();
  VectorDataReProjFilter::Pointer vdReProjFilter = VectorDataReProjFilter::New();

  VectorDataToRoadDescriptionFilterType::Pointer filter =
    VectorDataToRoadDescriptionFilterType::New();

  if (!DisplayWarnings)
    {
    reader->SetGlobalWarningDisplay(0);
    }
  otb::DEMHandler::Instance()->OpenDEMDirectory(DEMDirectory);

  reader->SetFileName(inputImg);
  reader->UpdateOutputInformation();

  DBReader->SetFileName(inputDB);
  DBReader->Update();

  vdReader->SetFileName(inputVD);
  vdReader->Update();

  vdReProjFilter->SetInputImage(reader->GetOutput());
  vdReProjFilter->SetInputVectorData(vdReader->GetOutput());
  vdReProjFilter->SetUseOutputSpacingAndOriginFromImage(true);
  vdReProjFilter->Update();

  filter->SetInput(vdReProjFilter->GetOutput());
  filter->AddOpticalImage(reader->GetOutput());
  filter->AddBuildingsDB(DBReader->GetOutput());

  vdWriter->SetFileName(outputVD);
  vdWriter->SetInput(filter->GetOutput());
  vdWriter->Update();

  return EXIT_SUCCESS;
}
