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
#include "otbImageFileWriter.h"

int otbGDALDriverDoubleWritingTest(int itkNotUsed(argc), char * argv[])
{
  const char * infname1 = argv[1];
  const char * infname2 = argv[2];
  const char * outfname1 = argv[3];
  const char * outfname2 = argv[4];
  const char * outfname3 = argv[5];
  const char * outfname4 = argv[6];

  typedef otb::VectorImage<double, 2>              ImageType;
  typedef otb::ImageFileReader<ImageType>          ReaderType;
  typedef otb::ImageFileWriter<ImageType>          WriterType;
  typedef otb::ImageFileWriter<ImageType> StreamingWriterType;

  ReaderType::Pointer          reader = ReaderType::New();
  WriterType::Pointer          writer = WriterType::New();
  StreamingWriterType::Pointer streamingWriter = StreamingWriterType::New();

  reader->SetFileName(infname1);
  writer->SetFileName(outfname1);
  writer->SetInput(reader->GetOutput());
  writer->Update();
  reader->SetFileName(infname2);
  writer->SetFileName(outfname2);
  writer->Update();

  reader->SetFileName(infname1);
  streamingWriter->SetFileName(outfname3);
  streamingWriter->SetInput(reader->GetOutput());
  streamingWriter->Update();

  reader->SetFileName(infname2);
  streamingWriter->SetFileName(outfname4);
  streamingWriter->SetInput(reader->GetOutput());
  streamingWriter->Update();

  return EXIT_SUCCESS;
}
