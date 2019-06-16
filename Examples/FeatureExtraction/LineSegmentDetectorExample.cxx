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


#include "otbImageFileReader.h"
#include "itkBinaryFunctorImageFilter.h"
#include "otbImageFileWriter.h"
#include "otbVectorDataFileWriter.h"

/* Example usage:
./LineSegmentDetectorExample Input/Scene.png Output/LSDOutput.shp
*/


// This example illustrates the use of the LineSegmentDetector.
// This filter is designed to extract segments in mono channel images.

#include "otbLineSegmentDetector.h"

int main(int argc, char* argv[])
{
  if (argc != 3)
  {
    std::cerr << "Usage: ./LineSegmentDetectorExample input output\n";
    return EXIT_FAILURE;
  }

  const char* infname  = argv[1];
  const char* outfname = argv[2];

  using InputPixelType         = unsigned char;
  using PrecisionType          = double;
  const unsigned int Dimension = 2;

  // As usual, we start by defining the types for the input image and
  // the image file reader.
  using ImageType  = otb::Image<InputPixelType, Dimension>;
  using ReaderType = otb::ImageFileReader<ImageType>;

  // We instantiate the reader and set the file name for the input image.
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);
  reader->GenerateOutputInformation();

  // We define now the type for the segment detector filter. It is
  // templated over the input image type and the precision with which
  // the coordinates of the detected segments will be given. It is
  // recommended to set this precision to a real type. The output of the
  // filter will be a \doxygen{otb}{VectorData}.
  using LsdFilterType = otb::LineSegmentDetector<ImageType, PrecisionType>;

  LsdFilterType::Pointer lsdFilter = LsdFilterType::New();

  // We can now define the type for the writer, instantiate it and set
  // the file name for the output vector data.
  using WriterType = otb::VectorDataFileWriter<LsdFilterType::VectorDataType>;

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outfname);

  // We plug the pipeline.
  lsdFilter->SetInput(reader->GetOutput());
  writer->SetInput(lsdFilter->GetOutput());

  // Before calling the Update() method of the writer in order to
  // trigger the pipeline execution, we call the
  // GenerateOutputInformation() of the reader, so the LSD
  // filter gets the information about image size and spacing.
  reader->GenerateOutputInformation();
  writer->Update();

  return EXIT_SUCCESS;
}
