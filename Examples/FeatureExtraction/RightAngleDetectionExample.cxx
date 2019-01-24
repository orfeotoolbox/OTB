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
#include "otbLineSegmentDetector.h"
#include "otbVectorDataFileWriter.h"

//  Software Guide : BeginCommandLineArgs
//    INPUTS: {Scene.png}
//    OUTPUTS: {rightAngleOutput.shp}
//  0.1 20
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
// This example illustrates the use of the
// \doxygen{otb}{VectorDataToRightAngleVectorDataFilter}.
// This filter detects the right angles in an image by exploiting the
// output of a line detection algorithm. Typically the
// \doxygen{otb}{LineSegmentDetector} class will be used. The right
// angle detection algorithm is described in detail in
// \cite{RightAngleDetection}.
//
// The first step required to use this filter is to include its header file.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbVectorDataToRightAngleVectorDataFilter.h"
// Software Guide : EndCodeSnippet

int main(int argc, char * argv[])
{
  if (argc != 5)
  {
    std::cerr << "Usage: ./RightAngleDetectionExample input rightAngleOutput angleThreshold distanceThreshold\n";
    return EXIT_FAILURE;
  }

  const char* infname                  = argv[1];
  const char* rightAngleOutputFilename = argv[2];
  double      angleThreshold           = atof(argv[3]);
  double      distanceThreshold        = atof(argv[4]);

  const unsigned int Dimension = 2;
  typedef unsigned char PixelType;
  typedef double        PrecisionType;

  typedef otb::Image<PixelType, Dimension> ImageType;
  typedef otb::ImageFileReader<ImageType>  ReaderType;

  auto reader = ReaderType::New();
  reader->SetFileName(infname);
  reader->GenerateOutputInformation();

  // Software Guide : BeginLatex
  //
  // After defining, as usual, the types for the input image and the
  // image reader, we define the specific types needed for this
  // example. First of all, we will use a vector data
  // to store the detected lines which will be provided by the line
  // segment detector.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::VectorData<PrecisionType>  VectorDataType;
  // Software Guide : EndCodeSnippet
  // Software Guide : BeginLatex
  //
  // The right angle detector's output is a vector data where each point
  // gives the coordinate of the detected angle.
  //
  // Next, We define the type for the line segment detector. A detailed
  // example for this detector can be found in section \ref{sec:LSD}.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::LineSegmentDetector<ImageType, PrecisionType> LsdFilterType;
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // We can finally define the type for the right angle detection
  // filter. This filter is templated over the input vector data type
  // provided by the line segment detector.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::VectorDataToRightAngleVectorDataFilter<VectorDataType>
    RightAngleFilterType;
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // We instantiate the line segment detector and the right angle detector.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  auto lsdFilter        = LsdFilterType::New();
  auto rightAngleFilter = RightAngleFilterType::New();
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // We plug the pipeline.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  lsdFilter->SetInput(reader->GetOutput());
  rightAngleFilter->SetInput(lsdFilter->GetOutput());
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // You can choose how far the right angle segments can be, and the tolerance
  // to consider an angle between two segments as an right one.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  rightAngleFilter->SetAngleThreshold(angleThreshold);
  rightAngleFilter->SetDistanceThreshold(distanceThreshold);
  // Software Guide : EndCodeSnippet

  typedef otb::VectorDataFileWriter<LsdFilterType::VectorDataType> WriterType;

  auto rightAngleWriter = WriterType::New();

  rightAngleWriter->SetInput(rightAngleFilter->GetOutput());
  rightAngleWriter->SetFileName(rightAngleOutputFilename);

  // Software Guide : BeginLatex
  //
  // Before calling the \code{Update()} method of the writers in order to
  // trigger the pipeline execution, we call the
  // \code{GenerateOutputInformation()} of the reader, so the
  // filter gets the information about image size and spacing.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  reader->GenerateOutputInformation();
  rightAngleWriter->Update();
  // Software Guide : EndCodeSnippet

  return EXIT_SUCCESS;
}
