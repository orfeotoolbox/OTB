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
#include "otbVectorImage.h"
#include "otbVectorData.h"
#include "otbImageFileReader.h"
#include "otbVectorDataFileWriter.h"
#include "otbStreamingConnectedComponentSegmentationOBIAToVectorDataFilter.h"

typedef float      InputPixelType;
const unsigned int Dimension = 2;
typedef otb::VectorImage<InputPixelType, Dimension> InputVectorImageType;
typedef otb::Image<unsigned int, Dimension>         LabelImageType;
typedef otb::Image<unsigned int, Dimension>         MaskImageType;
typedef otb::ImageFileReader<InputVectorImageType> ReaderType;

typedef otb::VectorData<double, Dimension> VectorDataType;
typedef VectorDataType::Pointer                   VectorDataPointerType;
typedef otb::VectorDataFileWriter<VectorDataType> VectorDataFileWriterType;
typedef VectorDataFileWriterType::Pointer         VectorDataFileWriterPointerType;

typedef otb::StreamingConnectedComponentSegmentationOBIAToVectorDataFilter<InputVectorImageType, LabelImageType, MaskImageType, VectorDataType>
    ConnectedComponentSegmentationOBIAToVectorDataFilterType;


int otbStreamingConnectedComponentSegmentationOBIAToVectorDataFilter(int itkNotUsed(argc), char* argv[])
{
  const char*  inputFilename          = argv[1];
  const char*  outputFilename         = argv[2];
  const char*  maskexpression         = argv[3];
  const char*  segmentationexpression = argv[4];
  unsigned int minobjectsize          = atoi(argv[5]);
  const char*  obiaexpression         = argv[6];
  unsigned int nbstreams              = atoi(argv[7]);

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename);
  reader->UpdateOutputInformation();

  ConnectedComponentSegmentationOBIAToVectorDataFilterType::FilterType::Pointer connected =
      ConnectedComponentSegmentationOBIAToVectorDataFilterType::FilterType::New();
  connected->GetFilter()->SetInput(reader->GetOutput());

  connected->GetFilter()->SetMaskExpression(maskexpression);
  connected->GetFilter()->SetConnectedComponentExpression(segmentationexpression);
  connected->GetFilter()->SetMinimumObjectSize(minobjectsize);
  connected->GetFilter()->SetOBIAExpression(obiaexpression);

  connected->GetStreamer()->SetNumberOfDivisionsStrippedStreaming(nbstreams);
  connected->Update();

  VectorDataFileWriterPointerType vdwriter = VectorDataFileWriterType::New();
  vdwriter->SetInput(connected->GetFilter()->GetOutputVectorData());
  vdwriter->SetFileName(outputFilename);
  vdwriter->Update();

  return EXIT_SUCCESS;
}
