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

//  Software Guide : BeginCommandLineArgs
//    INPUTS: {Scene.png}
//    OUTPUTS: {LSDOutput.shp}
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
// This example illustrates the use of the
// \doxygen{otb}{LineSegmentDetector}\cite{LSD}, also known as {\em Lucy in the
// Sky with Diamonds}.
// This filter is designed to extract segments in mono channel images.
//
// The first step required to use this filter is to include its header file.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbLineSegmentDetector.h"
// Software Guide : EndCodeSnippet

int main(int argc, char * argv[])
{
  if (argc != 3)
  {
    std::cerr << "Usage: ./LineSegmentDetectorExample input output\n";
    return EXIT_FAILURE;
  }

  const char* infname  = argv[1];
  const char* outfname = argv[2];

  typedef unsigned char InputPixelType;
  typedef double        PrecisionType;
  const unsigned int Dimension = 2;

  // Software Guide : BeginLatex
  //
  // As usual, we start by defining the types for the input image and
  // the image file reader.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::Image<InputPixelType,  Dimension> ImageType;
  typedef otb::ImageFileReader<ImageType>        ReaderType;
  // Software Guide : EndCodeSnippet
  // Software Guide : BeginLatex
  //
  // We instantiate the reader and set the file name for the input image.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);
  reader->GenerateOutputInformation();
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // We define now the type for the segment detector filter. It is
  // templated over the input image type and the precision with which
  // the coordinates of the detected segments will be given. It is
  // recommended to set this precision to a real type. The output of the
  // filter will be a \doxygen{otb}{VectorData}.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::LineSegmentDetector<ImageType, PrecisionType> LsdFilterType;

  LsdFilterType::Pointer lsdFilter = LsdFilterType::New();
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // We can now define the type for the writer, instantiate it and set
  // the file name for the output vector data.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::VectorDataFileWriter<LsdFilterType::VectorDataType> WriterType;

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outfname);
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // We plug the pipeline.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  lsdFilter->SetInput(reader->GetOutput());
  writer->SetInput(lsdFilter->GetOutput());
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Before calling the \code{Update()} method of the writer in order to
  // trigger the pipeline execution, we call the
  // \code{GenerateOutputInformation()} of the reader, so the LSD
  // filter gets the information about image size and spacing.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  reader->GenerateOutputInformation();
  writer->Update();
  // Software Guide : EndCodeSnippet

  return EXIT_SUCCESS;
}
