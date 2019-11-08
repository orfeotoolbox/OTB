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
#include "otbStreamingLineSegmentDetector.h"
#include "otbImageFileReader.h"
#include "otbVectorDataFileWriter.h"


int otbStreamingLineSegmentDetector(int itkNotUsed(argc), char* argv[])
{

  typedef float      InputPixelType;
  const unsigned int Dimension = 2;

  // Typedefs
  typedef otb::Image<InputPixelType, Dimension> ImageType;
  typedef otb::ImageFileReader<ImageType> ReaderType;

  typedef otb::StreamingLineSegmentDetector<ImageType>::FilterType           StreamingLineSegmentDetectorType;
  typedef StreamingLineSegmentDetectorType::FilterType::OutputVectorDataType OutputVectorDataType;
  typedef otb::VectorDataFileWriter<OutputVectorDataType>                    WriterType;

  ReaderType::Pointer                       reader    = ReaderType::New();
  StreamingLineSegmentDetectorType::Pointer lsdFilter = StreamingLineSegmentDetectorType::New();
  WriterType::Pointer                       writer    = WriterType::New();

  reader->SetFileName(argv[1]);
  reader->GenerateOutputInformation();
  lsdFilter->GetFilter()->SetInput(reader->GetOutput());
  lsdFilter->GetStreamer()->SetNumberOfLinesStrippedStreaming(atoi(argv[3]));
  lsdFilter->Update();

  writer->SetFileName(argv[2]);
  writer->SetInput(lsdFilter->GetFilter()->GetOutputVectorData());
  writer->Update();

  return EXIT_SUCCESS;
}
